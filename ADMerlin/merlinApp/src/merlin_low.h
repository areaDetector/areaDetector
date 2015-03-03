/**
 * Interface library to medipix Labview system.
 * 
 * Matthew Pearson
 * Nov 2011
 */

#ifndef MPX_LOW_H
#define MPX_LOW_H

#include <asynOctetSyncIO.h>

extern int mpxSet(const char *command, const char *value);
extern int mpxGet(const char *command, char *value);
extern int mpxCmd(const char *command);
extern int mpxData(unsigned int *data);
extern int mpxConnect(const char *host, int commandPort, int dataPort);
extern int mpxIsConnected(int *conn);
extern int mpxDisconnect(void);

/* Fixed protocol names*/
#define MPX_HEADER "MPX"
#define MPX_SET (char*)"SET"
#define MPX_GET (char*)"GET"
#define MPX_CMD (char*)"CMD"
#define MPX_DATA_HEADER "CMD"
#define MPX_DATA_ACQ_HDR "HDR"
#define MPX_DATA_12 "12B"
#define MPX_DATA_24 "24B"
#define MPX_GENERIC_IMAGE "IMG"
#define MPX_PROFILE_12 "P12"
#define MPX_PROFILE_24 "P24"
#define MPX_GENERIC_PROFILE "PRF"
#define MPX_QUAD_DATA "MQ1"
#define MPX_MSG_LEN_DIGITS 10
#define MPX_MSG_DATATYPE_LEN 3

#define MPX_MAXLINE 256
#define MPX_IMG_HDR_LEN 256
#define MPX_ACQUISITION_HEADER_LEN 2044

#define MPX_X_SIZE 256
#define MPX_Y_SIZE 256
#define MPX_IMAGE_PIXELS 65536
#define MPX_IMAGE_BYTES MPX_IMAGE_PIXELS * 2 // 16 bit pixels
// size of buffer for image frame body including leading comma
#define MPX_IMG_FRAME_LEN MPX_IMG_HDR_LEN + MPX_IMAGE_BYTES + MPX_MSG_DATATYPE_LEN + 2
#define MPX_IMG_FRAME_LEN24 MPX_IMG_HDR_LEN + MPX_IMAGE_BYTES * 2 + MPX_MSG_DATATYPE_LEN + 2 // 32 bit pixels in 12 bit mode
#define MAX_BUFF_MERLIN_QUAD 2000000
#define MAX_BUFF_UOM 5300000  // TODO verify has this got an extra 0 on it??

// error definitions
#define MPX_OK 0    			/*Ok*/
#define MPX_ERR 1               /*Unknown Error*/
#define MPX_ERR_CMD 2   		/*Command not known.*/
#define MPX_ERR_PARAM 3         /*Param out of range.*/
#define MPX_ERR_BUSY 4          /*The controller is busy and not accepting commands */
#define MPX_ERR_CONN 100  		/*Not connected to detector.*/
#define MPX_ERR_WRITE 101 		/*Error writing to socket.*/
#define MPX_ERR_READ 102 		/*Error reading from socket.*/
#define MPX_ERR_LEN 103 		/*Length of command and value too long, or NULL*/
#define MPX_ERR_DATA 110 		/*Data formatting error.*/
#define MPX_ERR_UNEXPECTED 111 	/*unexpected response from labview */

// variables - Acqusition and Trigger control
#define MPXVAR_GETSOFTWAREVERSION 		(char*) "SOFTWAREVERSION"
#define MPXVAR_DETECTORSTATUS 			(char*) "DETECTORSTATUS"
#define MPXVAR_NUMFRAMESTOACQUIRE 		(char*) "NUMFRAMESTOACQUIRE"
#define MPXVAR_ACQUISITIONTIME 			(char*) "ACQUISITIONTIME"
#define MPXVAR_ACQUISITIONPERIOD 		(char*) "ACQUISITIONPERIOD"
#define MPXVAR_TRIGGERSTART	 			(char*) "TRIGGERSTART"
#define MPXVAR_TRIGGERSTOP 	 			(char*) "TRIGGERSTOP"
#define MPXVAR_NUMFRAMESPERTRIGGER 		(char*) "NUMFRAMESPERTRIGGER"
#define MPXVAR_COUNTERDEPTH		 		(char*) "COUNTERDEPTH"
#define MPXVAR_ENABLECOUNTER1           (char*) "ENABLECOUNTER1"
#define MPXVAR_CONTINUOUSRW             (char*) "CONTINUOUSRW"
#define MPXVAR_ROI                      (char*) "ROI"
#define MPXVAR_ENABLEBACKROUNDCORR      (char*) "BCKGRNDCORRECTION"
#define MPXVAR_BACKGROUNDCOUNT          (char*) "BCKGRND"
#define MPXVAR_ENABLEIMAGEAVERAGE       (char*) "IMGAVERAGE"
#define MPXVAR_IMAGESTOSUM              (char*) "IMAGESTOSUM"
#define MPXVAR_COLOURMODE               (char*) "COLOURMODE"
#define MPXVAR_CHARGESUMMING            (char*) "CHARGESUMMING"

// variables Threshold Scan Control
#define MPXVAR_THSSCAN 					(char*) "THSCAN"
#define MPXVAR_THWINDOWMODE 			(char*) "THWINDOWMODE"
#define MPXVAR_THWINDOWSIZE 			(char*) "THWINDOWSIZE"
#define MPXVAR_THSTART 					(char*) "THSTART"
#define MPXVAR_THSTOP 					(char*) "THSTOP"
#define MPXVAR_THSTEP	 				(char*) "THSTEP"
#define MPXVAR_THRESHOLD0               (char*) "THRESHOLD0"
#define MPXVAR_THRESHOLD1               (char*) "THRESHOLD1"
#define MPXVAR_THRESHOLD2               (char*) "THRESHOLD2"
#define MPXVAR_THRESHOLD3               (char*) "THRESHOLD3"
#define MPXVAR_THRESHOLD4               (char*) "THRESHOLD4"
#define MPXVAR_THRESHOLD5               (char*) "THRESHOLD5"
#define MPXVAR_THRESHOLD6               (char*) "THRESHOLD6"
#define MPXVAR_THRESHOLD7               (char*) "THRESHOLD7"
#define MPXVAR_OPERATINGENERGY			(char*) "OPERATINGENERGY"

// commands
#define MPXCMD_STARTACQUISITION         (char*) "STARTACQUISITION"
#define MPXCMD_STOPACQUISITION          (char*) "STOPACQUISITION"
#define MPXCMD_THSCAN                   (char*) "THSCAN"
#define MPXCMD_SOFTWARETRIGGER          (char*) "SWTRIGGER"
#define MPXCMD_RESET                    (char*) "RESET"
#define MPXCMD_PROFILECONTROL           (char*) "PROFILES"
#define MPXCMD_PROFILES                 (char*) "PROFILES"
#define MPXCMD_BACKGROUNDACQUIRE        (char*) "BCKGRND"


// Bits in the PROFILES selection mask
#define MPXPROFILES_IMAGE               1
#define MPXPROFILES_XPROFILE            2
#define MPXPROFILES_YPROFILE            4
#define MPXPROFILES_SUM                 8


#endif /* MPX_LOW_H */
