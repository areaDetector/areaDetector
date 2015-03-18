/**
 * Test program for interface library to medipix Labview system.
 * 
 * Matthew Pearson
 * Nov 2011
 */

#include "merlin_low.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
  char value[MPX_MAXLINE] = {'\0'};
  char errMsg[MPX_MAXLINE] = {'\0'};
  int conn_flag = 0;
  int status = 0;
  int i = 0;

  unsigned int data[MPX_DATAFRAME] = {0};

  printf("Test program for medipix_low interface library.\n");

  if ((status = mpxConnect("127.0.0.1", 14000, 14001)) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }
  
  
  if ((status = mpxIsConnected(&conn_flag)) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  } else {
    if (conn_flag == 1) {
      printf("We are connected.\n");
    } else {
      printf("We are NOT connected.\n");
    }
  }

  if ((status = mpxGet("NUMFRAMESTOACQUIRE", value)) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }
  printf("NUMFRAMESTOACQUIRE: %s\n", value);

  if ((status = mpxSet("NUMFRAMESTOACQUIRE", "2")) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }
  
  if ((status = mpxGet("NUMFRAMESTOACQUIRE", value)) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }
  printf("NUMFRAMESTOACQUIRE: %s\n", value);

  printf("Sending STARTACQUISITION...\n");
  if ((status = mpxCmd("STARTACQUISITION")) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }

  printf("Getting data...\n");
  if ((status = mpxData(data)) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }

  for (i=0; i<5; i++) {
    printf("data[%d]=%x\n", i, data[i]);
  }

  if ((status = mpxDisconnect()) != MPX_OK) {
    printf("ERROR. status: %d, message: %s\n", status, mpxError(status, errMsg));
  }

  printf("Testing error status 0: %s\n", mpxError(0, errMsg));
  printf("Testing error status 1: %s\n", mpxError(1, errMsg));
  printf("Testing error status 2: %s\n", mpxError(2, errMsg));
  printf("Testing error status 3: %s\n", mpxError(3, errMsg));
  


  return EXIT_SUCCESS;
}
