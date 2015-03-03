/**
 * Interface library to medipix Labview system.
 * 
 * Matthew Pearson
 * Nov 2011
 *
 * NOTE - this code is no longer in use but kept for reference.
 *
 * The replacement for this code is implemented in the section of medipixDetector.cpp that is commented
 * "Labview communications primitives" - but note that all socket communication is deferred to ASYN
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "merlin_low.h"

/* local functions*/
static int mpxWriteRead(const char *buff, char *response);
static int mpxRead(char *input);
static int mpxWrite(const char *buff);
static int mpxSocketConnect(const char *host, int port, int *sock); 

static int connected = 0;
static int fd = 0;
static int fd_data = 0;

/**
 * Set a value for the specified command.
 * @arg command - command name
 * @arg value - value to set
 * @return int - error code
 */
int mpxSet(const char *command, const char *value) 
{
  char buff[MPX_MAXLINE] = {'\0'};
  int buff_len = 0;
  int status = 0;

  //printf("mpxSet input. command: %s, value: %s\n", command, value);

  if (!connected) {
    return MPX_CONN;
  }

  if ((command == NULL) || (value == NULL)) {
    return MPX_LEN;
  }

  /*Build up comand to be sent.*/
  buff_len = strlen(command) + strlen(value) + strlen(MPX_HEADER) + strlen(MPX_SET) + 5;
  if (buff_len > MPX_MAXLINE) {
    return MPX_LEN;
  }
  sprintf(buff, "%s,%s,%s,%s\r\n", MPX_HEADER, MPX_SET, command, value);
  
  //printf("mpxSet: Command:%s", buff);
  
  if ((status = mpxWriteRead(buff, NULL)) != MPX_OK) {
    return status;
  }

  return MPX_OK;
}


/**
 * Get a value for the specified command.
 * @arg command - command name
 * @arg value - returned value
 * @return int - error code
 */
int mpxGet(const char *command, char *value) 
{
  char buff[MPX_MAXLINE] = {'\0'};
  char input[MPX_MAXLINE] = {'\0'};
  int buff_len = 0;
  int status = 0;
  char *tok = NULL;

  if (!connected) {
    return MPX_CONN;
  }

  if ((command == NULL) || (value == NULL)) {
    return MPX_LEN;
  }
  
  /*Build up comand to be sent.*/
  buff_len = strlen(command) + strlen(MPX_HEADER) + strlen(MPX_GET) + 4;
  if (buff_len > MPX_MAXLINE) {
    return MPX_LEN;
  }
  sprintf(buff, "%s,%s,%s\r\n", MPX_HEADER, MPX_GET, command);
  
  //printf("mpxGet: Command:%s", buff);
  
  if ((status = mpxWriteRead(buff, input)) != MPX_OK) {
    return status;
  }

  //printf ("mpsGet: input = %s\n", input);
  tok = strtok(input, ",");
  tok = strtok(NULL, ",");
  strncpy(value, tok, MPX_MAXLINE);

  return MPX_OK;
}

/**
 * Send a cmd command.
 * @arg command - command name
 * @return int - error code
 */
int mpxCmd(const char *command) 
{
  char buff[MPX_MAXLINE] = {'\0'};
  int buff_len = 0;
  int status = 0;

  if (!connected) {
    return MPX_CONN;
  }

  if (command == NULL) {
    return MPX_LEN;
  }
  
  /*Build up comand to be sent.*/
  buff_len = strlen(command) + strlen(MPX_HEADER) + strlen(MPX_COMMAND) + 4;
  if (buff_len > MPX_MAXLINE) {
    return MPX_LEN;
  }
  sprintf(buff, "%s,%s,%s\r\n", MPX_HEADER, MPX_COMMAND, command);
  
  if ((status = mpxWriteRead(buff, NULL)) != MPX_OK) {
    return status;
  }

  return MPX_OK;
}


/**
 * Connect to the Labview program. This opens two TCP/IP sockets.
 *
 * @arg host - IP address
 * @commandPort - port number to send commands
 * @dataPort - port number to read data from
 * @return int - error code
 */
int mpxConnect(const char *host, int commandPort, int dataPort)
{

  printf("mpxConnect. host: %s, command port: %d, data port: %d\n", host, commandPort, dataPort);

  if (!connected) {

    if (mpxSocketConnect(host, commandPort, &fd) != MPX_OK) {
      return MPX_CONN;
    }

    if (mpxSocketConnect(host, dataPort, &fd_data) != MPX_OK) {
      return MPX_CONN;
    }

    connected = 1;
  }
  
  return MPX_OK;
}

static int mpxSocketConnect(const char *host, int port, int *sock) 
{
  struct sockaddr_in server_addr;
  struct timeval tv;

  char *function = "mpxSocketConnect";

  /*Create a TCP socket*/
  if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(function);
    return MPX_CONN;
  }
  
  /*Create and initialise a socket address structure*/
  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(host);
  server_addr.sin_port = htons(port);
  
  /*Connect to the server.*/
  if ((connect(*sock, (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0) {
    perror(function);
    return MPX_CONN;
  }

  /* Set a 5s timeout on read functions.*/
  tv.tv_sec = 5;  
  setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
}


/**
 * Test the connection status.
 * @arg int pointer - 1=connected 0=not connected
 * @return error code.
 */
int mpxIsConnected(int *conn)
{
  *conn = 0;
  if (connected == 1) {
    *conn = 1;
  }

  return MPX_OK;
}

/**
 * Disconnect from the Labview program. This closes the sockets 
 * opened using mpxConnect.
 * 
 * @return int - error code
 */
int mpxDisconnect(void) 
{
  char *function = "mpxDisconnect";

  //printf("mpxDisconnect.\n");

  if (connected == 1) {
    if (close(fd)) {
      perror(function);
      return MPX_CONN;
    } else {
      if (close(fd_data)) {
	perror(function);
	return MPX_CONN;
      } else {
	connected = 0;
      }
    }
  } else {
    return MPX_CONN;
  }

  return MPX_OK;
}

/**
 * Return the error string for this error code.
 * @arg int - error number
 * @char pointer - pointer to char array. Must be at least MPX_MAXLINE bytes.
 */
char * mpxError(int error, char *errMsg)
{
	if (error == MPX_OK)
	{
		strncpy(errMsg, "OK", MPX_MAXLINE);
	}
	else if (error == MPX_ERROR)
	{
		strncpy(errMsg, "Unknown Error", MPX_MAXLINE);
	}
	else if (error == MPX_CMD)
	{
		strncpy(errMsg, "Unknown Command", MPX_MAXLINE);
	}
	else if (error == MPX_PARAM)
	{
		strncpy(errMsg, "Param Out Of Range", MPX_MAXLINE);
	}
	else if (error == MPX_CONN)
	{
		strncpy(errMsg, "Not Connected To Detector", MPX_MAXLINE);
	}
	else if (error == MPX_WRITE)
	{
		strncpy(errMsg, "Error Writing To Socket", MPX_MAXLINE);
	}
	else if (error == MPX_READ)
	{
		strncpy(errMsg, "Error Reading From Socket", MPX_MAXLINE);
	}
	else if (error == MPX_DATA)
	{
		strncpy(errMsg, "Data Format Error", MPX_MAXLINE);
	}
	else if (error == MPX_LEN)
	{
		strncpy(errMsg, "Length of command and value too long", MPX_MAXLINE);
	}
	else if (error == MPX_UNEXPECTED)
	{
		strncpy(errMsg, "unexpected response from Labview", MPX_MAXLINE);
	}
	else
	{
		strncpy(errMsg, "Unknown Error Code", MPX_MAXLINE);
	}
	errMsg[MPX_MAXLINE - 1] = '\0';

	return errMsg;
}



static int mpxWriteRead(const char *buff, char *input)
{
  char response[MPX_MAXLINE] = {'\0'};
  int status = 0;
  char *tok = NULL;
  char command[MPX_MAXLINE] = {'\0'};
  char value[MPX_MAXLINE] = {'\0'};

  if ((status = mpxWrite(buff)) != MPX_OK) {
    return status;
  }

  if ((status = mpxRead(response)) != MPX_OK) {
    return status;
  }

  //printf("mpxWriteRead got back: %s\n", response);

  /* Parse the response to detect an error. 
     If no error, return the response.
     If an error, return the error code.*/
   tok = strtok(response, "\r\n");
  tok = strtok(tok, ",");
  //printf("tok: %s\n", tok);
  if (!strncmp(tok,"MPX",3)) {
    tok = strtok(NULL, ",");
    //printf("tok: %s\n", tok);
    if (atoi(tok) != 0) {
      //printf("Returning atoi(tok): %d\n", atoi(tok));
      return atoi(tok);
    } else {
      //printf("Returning command and value\n");
      tok = strtok(NULL, ",");
      //printf("tok: %s\n", tok);
      if ((tok != NULL) && (input != NULL)) {
	strncpy(command, tok, MPX_MAXLINE);
	tok = strtok(NULL, ",");
	if (tok != NULL) {
	  strncpy(value, tok, MPX_MAXLINE);
	}
	sprintf(input, "%s,%s", command, value);
      } 
    }
  } else {
    return MPX_READ;
    }
    
  return MPX_OK;
}

static int mpxWrite(const char *buff)
{
  char *function = "mpxWrite";

  if (write(fd, buff, strlen(buff)) <= 0) {
    perror(function);
    return MPX_WRITE;
  }

  return MPX_OK;
}


static int mpxRead(char *input) 
{
  int nleft = MPX_MAXLINE-1;
  int nread = 0;
  char buffer[MPX_MAXLINE] = {'\0'};
  char *bptr = NULL;
  int i = 0;
  char *function = "mpxRead";

  bptr = buffer;

  ///Read until nothing left in socket.
  while (nleft > 0) {
    if ((nread = read(fd, bptr, nleft)) < 0) {
      if (errno == EINTR) {
	nread = 0;
      } else {
	perror(function);
	return MPX_READ;
      }
    } else if (nread == 0) {
      return MPX_READ; //Done. Socket may have closed.
    }

    nleft = nleft - nread;
    
    //Read until '\r\n'.
    for (i=0; i<nread; i++) {
      if ((*bptr=='\r')&&(*(bptr+1)=='\n')) {
	nleft = 0;
	break;
      }
      bptr++;
    }
  }
  
  strncpy(input, buffer, MPX_MAXLINE);
  
  return MPX_OK;
}



/**
 * Read the latest data frame. 
 *
 * This blocks until either a frame is read or
 * the socket is closed (either using mpxDisconnect
 * or another mechanism).
 *
 * The data buffer must be assigned by the user, and it
 * won't be overwritten until this function is called again.
 */
int mpxData(unsigned int *data)
{

  int nleft = MPX_DATAFRAME;
  int nread = 0;
  char *bptr = NULL;
  int i = 0;
  char *function = "mpxData";

  bptr = (char *)data;

  if (connected) {

    /*Read from socket until we get a complete frame.*/
    while (nleft > 0) {
      if ((nread = read(fd_data, bptr, nleft)) < 0) {
	if (errno == EINTR) {
	  nread = 0;
	} else {
	  perror(function);
	  return MPX_READ;
	}
      } else if (nread == 0) {
	return MPX_READ; //Done. Socket may have closed.
      }
      
      nleft = nleft - nread;

      bptr+=nread;
    }
    
    bptr = (char *)data;
      
    //Read until '\r\n'.
    for (i=0; i<MPX_DATAFRAME; i++) {
      //printf("  data[%d]: %x\n", i, (*bptr)&0xFF); 
      if ((*bptr=='\r')&&(*(bptr+1)=='\n')) {
	nleft = 0;
	break;
      }
      bptr++;
    }
  
  } else {
    return MPX_CONN;
  }

  return EXIT_SUCCESS;
}

