/**
 * Simple TCP server to simulate a medipix Labview system.
 * Arguments:
 *   port number - port number to listen for connections
 * 
 * Matthew Pearson
 * Oct 2011
 *
 * Giles Knap
 * Jan 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#include <time.h>

#define MAXLINE 256
#define MAXDATA 256*256*2*2 // 256 X by 256 Y by 2 bytes per pixel * 2 for 24 bit depth (= 32 bits data)
#define DATAHEADERLEN 252
#define MPX_PROFILE_LEN 256 * 8
#define MPX_SUM_LEN 4
#define CMDLEN 4
#define HEADER_LEN 15 // this includes 2 commas + the header and length fields
/*Function prototypes.*/
void sig_chld(int signo);
int echo_request(int socket_fd);
int produce_data(int socket_fd);
void *commandThread(void* command_fd);
void *dataThread(void* data_fd);
int produce_data(int data_fd);

int frame_count = 0;
int frames_to_send = 0;

int incrementer = 0;

int data_exit = 0;
int do_data = 0;
pthread_mutex_t do_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t do_data_cond = PTHREAD_COND_INITIALIZER;

int Depth = 12;

int main(int argc, char *argv[])
{
    int fd, fd2, fd_data, fd2_data;
    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr, server_addr_data,
            client_addr_data;

    pthread_t tid, tid_data;

    printf("Started Medipix simulation server...\n");

    if (argc != 3)
    {
        printf("  ERROR: Use: %s {command socket} {data socket}\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create a TCP socket.*/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    fd_data = socket(AF_INET, SOCK_STREAM, 0);

    /* Create and initialise a socket address structure.*/
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY );
    server_addr.sin_port = htons(atoi(argv[1]));

    memset(&server_addr_data, 0, sizeof(struct sockaddr_in));
    server_addr_data.sin_family = AF_INET;
    server_addr_data.sin_addr.s_addr = htonl(INADDR_ANY );
    server_addr_data.sin_port = htons(atoi(argv[2]));

    /* Bind the socket address to the socket buffer.*/
    bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    bind(fd_data, (struct sockaddr *) &server_addr_data,
            sizeof(server_addr_data));

    /* data socket timeout	 */
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    setsockopt(fd_data, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
            sizeof(timeout));

    /* Listen for incoming connections. */
    listen(fd, 10);
    listen(fd_data, 10);

    /* Install signal handler to clean up children.*/
    //  signal(SIGCHLD, sig_chld);
    /*Loop forever.*/
    while (1)
    {

        /*Command socket*/
        printf("Waiting for command socket...\n");
        client_size = sizeof(client_addr);
        if ((fd2 = accept(fd, (struct sockaddr *) &client_addr, &client_size))
                < 0)
        {
            if (errno == EINTR)
            {
                continue; /*Deal with interupted system call, since this blocks.*/
            }
            else
            {
                perror(argv[0]);
                exit(EXIT_FAILURE);
            }
        }

        if (fd2 > 0)
        {

            /*Data socket*/
            printf("Waiting for data socket...\n");
            client_size = sizeof(client_addr_data);
            if ((fd2_data = accept(fd_data,
                    (struct sockaddr *) &client_addr_data, &client_size)) < 0)
            {
                if (errno == EINTR)
                {
                    continue; /*Deal with interupted system call, since this blocks.*/
                }
                else
                {
                    perror(argv[0]);
                    exit(EXIT_FAILURE);
                }
            }

            if (fd2_data > 0)
            {
                pthread_create(&tid, NULL, &commandThread, (void *) fd2);
                pthread_create(&tid_data, NULL, &dataThread, (void *) fd2_data);

                /*Block here waiting for the threads to finish. This only allows a single client to
                 connect, to keep it simple.*/
                pthread_join(tid, NULL );
                printf("thread 1 finished.\n");
                pthread_join(tid_data, NULL );
                printf("thread 2 finished.\n");
            }
            else
            {
                close(fd2);
            }
        }

    }

    /*Should never get here.*/
    printf("Finishing Medipix server.\n");
    return EXIT_SUCCESS;

}

void * commandThread(void *command_fd)
{
    printf("Started commandThread.\n");

    if (echo_request((int) command_fd) != EXIT_SUCCESS)
    {
        printf("  Client failed to handle protocol, or connection closed.\n");
        /*close connected socket*/
        close((int) command_fd);
        /*signal data thread to exit.*/
        printf("***signalling data thread to exit.\n");
        pthread_mutex_lock(&do_data_mutex);
        do_data = 1;
        data_exit = 1;
        pthread_cond_signal(&do_data_cond);
        pthread_mutex_unlock(&do_data_mutex);
        return (void *) EXIT_FAILURE;
    }
    return (void *) EXIT_SUCCESS;
}

void * dataThread(void *data_fd)
{
    printf("Started dataThread.\n");

    if (produce_data((int) data_fd) != EXIT_SUCCESS)
    {
        printf(
                "  Data client failed to handle protocol, or connection closed.\n");
        /*close connected socket*/
        close((int) data_fd);
        return (void *) EXIT_FAILURE;
    }
    return (void *) EXIT_SUCCESS;
}

/**
 * Signal handler for SIGCHLD. NOT NEEDED NOW WE ARE USING PTHREADS.
 *
 */
void sig_chld(int signo)
{
    pid_t pid = 0;
    int status = 1;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("Child %d has terminated. Status = %d\n", pid, status);
    }
}

/**
 * Read from socket and send back response. 
 * Read a maximum of MAXLINE, or until a newline, then echo the command back.
 */
int echo_request(int socket_fd)
{
    int nleft = HEADER_LEN;
    int nread = 0;
    char buffer[MAXLINE + 1] =
    { '\0' };
    char response[MAXLINE] =
    { '\0' };
    char *bptr = NULL;
    int inheader = 1;
    char *tok = NULL;
    char *cmdType, *cmdName, *cmdValue;
    int cmdIntValue;
    int bodylen = 0;

    bptr = buffer;

    if (socket_fd == 0)
    {
        printf("NULL socket fd in echo_request.\n");
        return EXIT_FAILURE;
    }

    // keep reading and responding until an error occurs
    while (1)
    {
        // clear response
        response[0] = (char) NULL;
        // first time around following loop we read header_len bytes
        nleft = HEADER_LEN;
        // first pass of the while loop is reading header
        inheader = 1;

        /// Read until end of current block (header or body)
        while (nleft > 0)
        {
            printf("reading socket...\n");
            if ((nread = read(socket_fd, bptr, nleft)) < 0)
            {
                if (errno == EINTR)
                {
                    nread = 0;
                    printf("EINTR");
                }
                else
                {
                    printf("Unknown read error.\n");
                    return EXIT_FAILURE;
                }
            }
            else if (nread == 0)
            {
                printf("Read nothing. Socket closed.\n");
                return EXIT_FAILURE; //Done. Client has probably disconnected.
            }

            nleft = nleft - nread;
            if (inheader)
            {
                if (nleft == 0)
                {
                    inheader = 0;
                    bptr[HEADER_LEN] = (char) NULL;

                    printf("received command header: %s\n", buffer);

                    nleft = MAXLINE - HEADER_LEN;
                    tok = strtok(bptr, ",");
                    tok = strtok(NULL, ",");
                    if (tok == NULL )
                    {
                        printf("MPX Command header is missing length field.\n");
                        continue;
                    }
                    else
                    {
                        // subtract 1 from bodylen since we already read the 1st comma
                        nleft = bodylen = atoi(tok) - 1;
                    }
                }
            }
            else
            {
            }

        }

        bptr[bodylen] = (char) NULL;
        printf("received command body: %s\n", buffer);

        //Handle command
        bptr = buffer;

        cmdType = strtok(bptr, ",");
        cmdName = strtok(NULL, ",");
        if (cmdType == NULL || cmdName == NULL )
        {
            printf("badly formed MPX command\n");
            sprintf(response, "MPX,0000000008,ERROR,1");
            continue;
        }

        if (!strncmp(cmdType, "SET", 3))
        {
            cmdValue = strtok(NULL, ",");
            cmdIntValue = atoi(cmdValue);

            if (!strncmp(cmdName, "NUMFRAMESTOACQUIRE", MAXLINE))
            {
                printf("setting frame count to %d\n\n", cmdIntValue);
                frame_count = cmdIntValue;
            }
            else if (!strcmp(cmdName, "COUNTERDEPTH"))
            {
                if (!strcmp(cmdValue, "12"))
                    Depth = 12;
                else
                    Depth = 24;
                printf("switching counter depth to %d\n\n", Depth);
            }

            // default response
            bodylen = strlen(cmdName) + 7;
            sprintf(response, "MPX,%010u,SET,%s,0", bodylen, cmdName);
        }
        else if (!strncmp(cmdType, "GET", 3))
        {
            char strResp[MAXLINE];
            sprintf(strResp, "%d", ++incrementer * 100);

            if (!strncmp(cmdName, "DETECTORSTATUS", MAXLINE))
                strcpy(strResp, "0");
            if (!strncmp(cmdName, "GETSOFTWAREVERSION", MAXLINE))
                strcpy(strResp, "2.2");

            if (!strncmp(cmdName, "CONTINUOUSRW", MAXLINE))
                strcpy(strResp, "1");

            if (!strncmp(cmdName, "ENABLECOUNTER1", MAXLINE))
                strcpy(strResp, "0");

            /*
             if(!strncmp(cmdName,"THSTART",MAXLINE))
             strcpy(strResp, "2.0");
             if(!strncmp(cmdName,"THSTOP",MAXLINE))
             strcpy(strResp, "8.0");
             if(!strncmp(cmdName,"THSTEP",MAXLINE))
             strcpy(strResp, "1.0");
             */

            bodylen = strlen(cmdName) + strlen(strResp) + 8;
            sprintf(response, "MPX,%010u,GET,%s,%s,0", bodylen, cmdName,
                    strResp);
        }
        else if (!strncmp(cmdType, "CMD", 3))
        {
            if (!strncmp(cmdName, "STARTACQUISITION", 16)
                    || !strncmp(cmdName, "THSTART", 16)
                    || !strncmp(cmdName, "PROFILES", 16))
            {
                if (!strncmp(cmdName, "THSTART", 16))
                    frames_to_send = 7;
                else
                    frames_to_send = frame_count;

                /*signal data thread to send some data back.*/
                printf("***signalling data thread.\n");
                pthread_mutex_lock(&do_data_mutex);
                if (!strncmp(cmdName, "PROFILES", 16))
                {
                    // use do_data = 2 to indicate a profiles request
                    do_data = 2;
                }
                else
                {
                    // use do_data = 1 to indicate an image request
                    do_data = 1;
                }
                pthread_cond_signal(&do_data_cond);
                pthread_mutex_unlock(&do_data_mutex);
                printf("***data thread singnalled.\n");
            }

            // construct response
            bodylen = strlen(cmdName) + 7;
            sprintf(response, "MPX,%010u,CMD,%s,0", bodylen, cmdName);
        }
        else
        {
            printf("Unknown MPX command: '%s'\n", cmdName);
            sprintf(response, "MPX,0000000008,ERROR,1");
        }

        // deliberately send junk to test re-synch capability
        /*
         printf("sending garbage..\n");
         if(write(socket_fd, "garbage MP garbage", 15) <=0)
         printf("garbage 1 failed\n");
         */

        printf("sending response: %s \n", response);
        if (write(socket_fd, response, strlen(response)) <= 0)
        {
            printf("Error writing back to client.\n");
            // return EXIT_FAILURE;
        }

        //Clear buffer for next command.
        memset(&buffer[0], '\0', sizeof(buffer));
    }

    printf("Buffer full. From client: %s\n", buffer);

    return EXIT_SUCCESS;
}

void endian_swap(uint64_t* x)
{
    *x =     ((((*x) & 0x00000000000000FFLL) << 0x38)
            | (((*x) & 0x000000000000FF00LL) << 0x28)
            | (((*x) & 0x0000000000FF0000LL) << 0x18)
            | (((*x) & 0x00000000FF000000LL) << 0x08)
            | (((*x) & 0x000000FF00000000LL) >> 0x08)
            | (((*x) & 0x0000FF0000000000LL) >> 0x18)
            | (((*x) & 0x00FF000000000000LL) >> 0x28)
            | (((*x) & 0xFF00000000000000LL) >> 0x38));
}

/**
 * Produce simulation data when we start acquisition. 
 */
int produce_data(int data_fd)
{
    //char *data = "Here is some data.\r\n";
    //unsigned int trailer = 0xDA; /* CR LF */

    // frame = header (including comma) + data frame type (3 chars) +
    //		comma + data frame header + image data
    int frameSize24 = HEADER_LEN + CMDLEN + DATAHEADERLEN + MAXDATA;
    int frameSize12 = HEADER_LEN + CMDLEN + DATAHEADERLEN + MAXDATA / 2;
    int frameSizeProfile = HEADER_LEN + CMDLEN + DATAHEADERLEN
            + MPX_PROFILE_LEN * 2+ MPX_SUM_LEN;

    char data[frameSize24];
    unsigned int i, j;
    int headersLength = HEADER_LEN + CMDLEN + DATAHEADERLEN;

    while (1)
    {
        printf("***taking mutex in data thread.\n");
        /*Wait for signal to produce some data.*/
        pthread_mutex_lock(&do_data_mutex);
        while (do_data == 0)
        {
            printf("***waiting in data thread.\n");
            pthread_cond_wait(&do_data_cond, &do_data_mutex);
        }

        printf("***got signal in data thread.\n");

        //printf("data_fd: %d\n", data_fd);
        //printf("data_exit: %d\n", data_exit);
        //printf("do_data: %d\n", do_data);

        //printf("MAXDATA: %d\n", MAXDATA);

        if ((do_data > 0) && (data_exit == 0))
        {
            printf("starting frame sending with do_data = %d\n", do_data);

            // send a silly acquisition header
            if (write(data_fd,
                    "MPX,0000000433,HDR,dummy acquisition header.0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789END",
                    433 + HEADER_LEN - 1) <= 0)
            {
                printf("Error writing acquisition header to client.\n");
                do_data = 0;
                pthread_mutex_unlock(&do_data_mutex);
                //return EXIT_FAILURE;
            }
            printf("*** wrote data - acquisition header\n");

            printf("*** writing %d data frames\n", frames_to_send);

            for (i = 0; i < frames_to_send; i++)
            {
                char timebuf[255];
                char buf2[255];
                time_t now;
                struct tm *current;
                int thisFrameSize = Depth == 12 ? frameSize12 : frameSize24;

                if (do_data == 2)
                {
                    thisFrameSize = frameSizeProfile;
                }

                now = time(0); //current time in C representation
                current = localtime(&now);

                strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S.123",
                        current);

                // ??? getting spurious one byte garbage - so adding 1 to length of data packet ???
                snprintf(data, HEADER_LEN, "MPX,%010u,",
                        thisFrameSize - HEADER_LEN + 1);
                sprintf(buf2,
                        "%4d,1,%s.007,1.5E-2,6.0,8.01E2,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,1,20,21,22,23,24,25,00014",
                        i, timebuf);

                if (do_data == 1)
                {
                    static int offset = 0;

                    printf("preparing image data\n");
                    sprintf((data + HEADER_LEN), "%dB,%-251s", Depth, buf2);

                    // create dummy data
                    for (j = headersLength;
                            j < MAXDATA / (24 / Depth) + headersLength; j++)
                    {
                        data[j] = (j+offset) / 20 % 255;
                    }
                    offset += 20;
                }
                else
                {
                    printf("preparing profile data\n");
                    sprintf((data + HEADER_LEN), "P%d,%-251s", Depth, buf2);

                    uint64_t* profileData = (uint64_t*) (data + headersLength);

                    // create dummy data
                    for (j = 0; j < 256 * 2; j++)
                    {
                        profileData[j] = (uint64_t)j;
                        //endian_swap(&profileData[j]);
                    }
                }
                // write an image
                if (write(data_fd, data, thisFrameSize) <= 0)
                {
                    printf("Error writing data frame 1 to client.\n");
                    do_data = 0;
                    pthread_mutex_unlock(&do_data_mutex);
                    //return EXIT_FAILURE;
                }
                printf("*** wrote data - image %d at time %s\n", i, timebuf);
            }

            do_data = 0;
        }
        else
        {
            if (data_exit)
            {
                printf("Exiting data thread.\n");
                data_exit = 0;
                do_data = 0;
                pthread_mutex_unlock(&do_data_mutex);
                break;
            }
        }

        pthread_mutex_unlock(&do_data_mutex);

    }
    pthread_mutex_unlock(&do_data_mutex);

    return EXIT_SUCCESS;
}
