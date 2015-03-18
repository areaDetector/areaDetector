#ifndef MPXCONN_H_
#define MPXCONN_H_

#define ASYN_TRACE_MPX          0x0100
#define ASYN_TRACE_MPX_VERBOSE  0x0200

#include "merlin_low.h"

/** data header types */
typedef enum
{
    MPXDataHeaderNone,
    MPXDataHeader12,
    MPXDataHeader24,
    MPXGenericImageHeader,
    MPXProfileHeader12,
    MPXProfileHeader24,
    MPXGenericProfileHeader,
    MPXAcquisitionHeader,
    MPXQuadDataHeader,
    MPXUnknownHeader
} merlinDataHeader;

class merlinDetector;

class mpxConnection
{
public:
    /* input and output from the labView controller     */
    char toLabview[MPX_MAXLINE];
    char fromLabview[MPX_MAXLINE];
    char fromLabviewHeader[MPX_MAXLINE];
    char fromLabviewBody[MPX_MAXLINE];
    char fromLabviewValue[MPX_MAXLINE];
    int fromLabviewError;

public:
    // Constructor
    mpxConnection(asynUser* parentUser, asynUser* tcpUser,
            merlinDetector* parentObj);

    /* The labview communication primitives */
    asynStatus mpxGet(char* valueId, double timeout);
    asynStatus mpxSet(char* valueId, char* value, double timeout);
    asynStatus mpxCommand(char* commandId, double timeout);
    asynStatus mpxWrite(double timeout);
    asynStatus mpxReadCmd(char* cmdType, char* cmdName, double timeout);
    asynStatus mpxWriteRead(char* cmdType, char* cmdName, double timeout);
    asynStatus mpxRead(asynUser* pasynUser, char* bodyBuf, int bufSize,
            int* bytesRead, double timeout);

    /* Helper functions */
    merlinDataHeader parseDataHeader(const char* header);
    void parseDataFrame(NDAttributeList* pAttr, const char* header,
            merlinDataHeader headerType, size_t *xsize, size_t *ysize,
            int* pixelSize, int* profileMask);
    void parseMqDataFrame(NDAttributeList* pAttr, const char* header,
    		size_t *xsize, size_t *ysize, int* pixelDepth, int* offset);

    void dumpData(char* sdata, int size);

private:
    asynUser* parentUser;
    asynUser* tcpUser;
    merlinDetector* parentObj;
};

#endif
