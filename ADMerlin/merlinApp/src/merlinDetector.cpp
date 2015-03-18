/* merlinDetector.cpp
 *
 * This is a driver for a Medipix 3 detector chip.
 *
 * The driver is designed to communicate with the chip via the matching Labview controller over TCP/IP
 *
 * Author: Giles Knap
 *         Diamond Light Source Ltd.
 *
 * Created:  Jan 06 2012
 * Modified: Mar 3, 2015 - Matthew Moore prep for AD2 and canged to be a little more Merlin centered
 *

 * Original Source from pilatusDetector by Mark Rivers
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

// #include <epicsTime.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsMutex.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <cantProceed.h>
#include <iocsh.h>
#include <epicsExport.h>

#include <asynOctetSyncIO.h>

#include "ADDriver.h"

#include "mpxConnection.h"
#include "merlinDetector.h"

#define MAX(a,b) a>b ? a : b
#define MIN(a,b) a<b ? a : b

/** This thread controls acquisition, reads image files to get the image data, and
 * does the callbacks to send it to higher layers
 * It is totally decoupled from the command thread and simply waits for data
 * frames to be sent on the data channel (TCP) regardless of the state in the command
 * thread and TCP channel */
void merlinDetector::merlinTask()
{
    int status = asynSuccess;
    int imageCounter;      // number of ndarrays sent to plugins
    int numImagesCounter;  // number of images received
    int counterDepth;
    int imagSize;
    NDArray * pImage;
    epicsTimeStamp startTime;
    const char *functionName = "merlinTask";
    size_t dims[2], dummy;
    int arrayCallbacks;
    int dummy2;
    int nread;
    char *bigBuff;
    char aquisitionHeader[MPX_ACQUISITION_HEADER_LEN + 1];
    int triggerMode;
    NDAttributeList *imageAttr = new NDAttributeList();

    // do not enter this thread until the IOC is initialised. This is because we are getting blocks of
    // data on the data channel at startup after we have had a buffer overrun
    while (startingUp)
    {
        epicsThreadSleep(.5);
    }

    this->lock();

    // allocate a buffer for reading in images from labview over network
    switch (detType)
    {
    case UomXBPM:
        imagSize = MAX_BUFF_UOM;
        break;
    case Merlin:
    case MedipixXBPM: imagSize = MPX_IMG_FRAME_LEN24;
        break;
    case MerlinQuad:
        imagSize = MAX_BUFF_MERLIN_QUAD;
        break;
    default:
        imagSize = MAX_BUFF_UOM;
        break;
    }

    bigBuff = (char*) calloc(imagSize, 1);

    /* Loop forever */
    while (1)
    {
        // Get the current time
        epicsTimeGetCurrent(&startTime);

        // Acquire an image from the data channel
        memset(bigBuff, 0, MPX_IMG_FRAME_LEN);

        /* We release the mutex when waiting because this takes a long time and
         * we need to allow abort operations to get through */
        this->unlock();

        // wait for the next data frame packet - this function spends most of its time here
        status = cmdConnection->mpxRead(this->pasynLabViewData, bigBuff,
                imagSize, &nread, 10);

        /* If there was an error jump to bottom of loop */
        if (status)
        {
            if (status == asynTimeout)
                status = asynSuccess;   // timeouts are expected
            else
            {
                asynPrint(this->pasynLabViewData, ASYN_TRACE_ERROR,
                        "%s:%s: error in Labview data channel response, status=%d\n",
                        driverName, functionName, status);
                setStringParam(ADStatusMessage,
                        "Error in Labview data channel response");
                // wait before trying again - otherwise socket error creates a tight loop
                epicsThreadSleep(5);
            }
            this->lock();
            continue;
        }
        this->lock();

        asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                "\nReceived image frame of %d bytes\n", nread);

        if (pasynTrace->getTraceMask((pasynUserSelf))
                & (ASYN_TRACE_MPX_VERBOSE))
        {
            dataConnection->dumpData(bigBuff, nread);
        }

        merlinDataHeader header = dataConnection->parseDataHeader(bigBuff);
        if (header != MPXAcquisitionHeader)
        {
            getIntegerParam(ADNumImagesCounter, &numImagesCounter);
            numImagesCounter++;
            setIntegerParam(ADNumImagesCounter, numImagesCounter);
            if (imagesRemaining > 0)
                imagesRemaining--;

            getIntegerParam(NDArrayCounter, &imageCounter);
            imageCounter++;
            setIntegerParam(NDArrayCounter, imageCounter);
        }

        getIntegerParam(NDArrayCallbacks, &arrayCallbacks);

        if (arrayCallbacks)
        {
            getIntegerParam(merlinCounterDepth, &counterDepth);

            int idim;
            /* Get an image buffer from the pool */
            getIntegerParam(ADMaxSizeX, &idim);
            dims[0] = idim;
            getIntegerParam(ADMaxSizeY, &idim);
            dims[1] = idim;

            if (header == MPXAcquisitionHeader)
            {
                // this is an acquisition header
                strncpy(aquisitionHeader, bigBuff, MPX_ACQUISITION_HEADER_LEN);
                aquisitionHeader[MPX_ACQUISITION_HEADER_LEN] = 0;
            }
            else if (header == MPXDataHeader12)
            {
                asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                        "Creating a 12bit Array\n");

                pImage = copyToNDArray16(dims, bigBuff, MPX_IMG_HDR_LEN);
                if (pImage == NULL)
                    continue;
                dataConnection->parseDataFrame(pImage->pAttributeList, bigBuff,
                        header, &dummy, &dummy, &dummy2, &dummy2);
            }
            else if (header == MPXDataHeader24)
            {
                asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                        "Creating a 24bit Array\n");

                pImage = copyToNDArray32(dims, bigBuff, MPX_IMG_HDR_LEN);
                if (pImage == NULL)
                    continue;
                dataConnection->parseDataFrame(pImage->pAttributeList, bigBuff,
                        header, &dummy, &dummy, &dummy2, &dummy2);
            }
            else if (header == MPXGenericImageHeader)
            {
                int pixelSize;
                asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                        "Creating a generic Image NDArray\n");

                // Parse the header and use the information to determine the
                // size of the NDArray
                imageAttr->clear();
                dataConnection->parseDataFrame(imageAttr, bigBuff, header,
                        &(dims[0]), &(dims[1]), &pixelSize, &dummy2);
                pImage = NULL;
                if (pixelSize == 16)
                {
                    pImage = copyToNDArray16(dims, bigBuff, MPX_IMG_HDR_LEN);
                }
                else if (pixelSize == 32)
                {
                    pImage = copyToNDArray32(dims, bigBuff, MPX_IMG_HDR_LEN);
                }
                if (pImage == NULL)
                    continue;
                imageAttr->copy(pImage->pAttributeList);
            }
            else if (header == MPXQuadDataHeader)
            {
                int pixelSize;
                int offset;
                asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                        "Creating a Quad Merlin Image NDArray\n");

                // Parse the header and use the information to determine the
                // size of the NDArray
                imageAttr->clear();
                dataConnection->parseMqDataFrame(imageAttr, bigBuff, &(dims[0]),
                        &(dims[1]), &pixelSize, &offset);
                pImage = NULL;
                if (pixelSize == 16)
                {
                    pImage = copyToNDArray16(dims, bigBuff, offset);
                }
                else if (pixelSize == 32)
                {
                    pImage = copyToNDArray32(dims, bigBuff, offset);
                }
                else
                {
                    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                        "Unsupported bit depth %d\n", header);
                    setStringParam(ADStatusMessage,
                            "Error: Unsupported bit depth");
                }

                if (pImage == NULL)
                {
                    continue;
                }
                imageAttr->copy(pImage->pAttributeList);
            }
            else if (header == MPXProfileHeader12
                    || header == MPXProfileHeader24
                    || header == MPXGenericProfileHeader)
            {
                int profileMask = 0;
                asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
                        "Creating a Profile NDArray\n");

                imageAttr->clear();
                pImage = NULL;

                if (header == MPXGenericProfileHeader)
                    dataConnection->parseDataFrame(imageAttr, bigBuff, header,
                            &(dims[0]), &(dims[1]), &dummy2, &profileMask);
                else
                    dataConnection->parseDataFrame(imageAttr, bigBuff, header,
                            &dummy, &dummy, &dummy2, &profileMask);

                if (profileMask
                        != (MPXPROFILES_XPROFILE | MPXPROFILES_YPROFILE
                                | MPXPROFILES_SUM))
                {
                    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                            "%s:%s: unsupported PROFILES mode %d\n", driverName,
                            functionName, profileMask);
                }
                else
                {
                    pImage = copyProfileToNDArray32(dims, bigBuff, profileMask);
                }
                if (pImage == NULL)
                    continue;
                imageAttr->copy(pImage->pAttributeList);
            }
            else
            {
                asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                        "Unknown header type %d\n", header);
            }

            // for Data frames - complete the NDAttributes, pass the NDArray on
            if (header == MPXDataHeader12 || header == MPXDataHeader24
                    || header == MPXProfileHeader12
                    || header == MPXProfileHeader24
                    || header == MPXGenericImageHeader
                    || header == MPXGenericProfileHeader
                    || header == MPXQuadDataHeader)
            {
                // Put the frame number and time stamp into the buffer
                pImage->uniqueId = imageCounter;
                pImage->timeStamp = startTime.secPastEpoch
                        + startTime.nsec / 1.e9;

                // string attributes are global in HDF5 plugin so the most recent
                // acquisition header is applied to all files
                pImage->pAttributeList->add("Acquisition Header", "",
                        NDAttrString, aquisitionHeader);

                /* Get any attributes that have been defined for this driver */
                this->getAttributes(pImage->pAttributeList);

                // Call the NDArray callback
                // Must release the lock here, to avoid a deadlock: we can
                // block on the plugin lock, and the plugin can be calling us
                this->unlock();
                if (header == MPXDataHeader12 || header == MPXDataHeader24
                    || header == MPXGenericImageHeader || header == MPXQuadDataHeader)
                {
                    doCallbacksGenericPointer(pImage, NDArrayData, 0);
                }
                else
                {
                    // address 1 on the port is used for profiles
                    // TODO use of port 1 is not working in NDPluginBase so
                    // currently reverting to use the same address
                    // (i.e. setting Merlin1:ROI:NDArrayAddress has no effect
                    doCallbacksGenericPointer(pImage, NDArrayData, 0);
                }
                this->lock();

                /* Free the image buffer */
                pImage->release();
            }
        }

        // If we are using SW triggers then reset the trigger to 0 when an image is
        // received
        status = getIntegerParam(ADTriggerMode, &triggerMode);
        if (triggerMode == TMSoftwareTrigger)
        {
            // software trigger resets  when image received
            setIntegerParam(merlinSoftwareTrigger, 0);
        }

        // If all the expected images have been received then the driver can
        // complete the acquisition and return to waiting for acquisition state
        if (imagesRemaining == 0)
        {
            setIntegerParam(ADAcquire, 0);
            setIntegerParam(ADStatus, ADStatusIdle);
        }

        /* Call the callbacks to update any changes */
        callParamCallbacks();
    }
    // release the image buffer (in reality this does not get called
    // We need a thread shutdown signal)
    free(bigBuff);
}

/** helper functions for endian conversion
 *
 */
inline void merlinDetector::endian_swap(unsigned short& x)
{
    if (detType == Merlin || detType == MerlinQuad)
    {
        x = (x >> 8) | (x << 8);
    }
}

inline void merlinDetector::endian_swap(unsigned int& x)
{
    if (detType == Merlin || detType == MerlinQuad)
    {
        x = (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00)
                | (x << 24);
    }
}

inline void merlinDetector::endian_swap(uint64_t& x)
{
    if (detType == Merlin || detType == MerlinQuad)
    {
        x = ((((x) & 0x00000000000000FFLL) << 0x38)
                | (((x) & 0x000000000000FF00LL) << 0x28)
                | (((x) & 0x0000000000FF0000LL) << 0x18)
                | (((x) & 0x00000000FF000000LL) << 0x08)
                | (((x) & 0x000000FF00000000LL) >> 0x08)
                | (((x) & 0x0000FF0000000000LL) >> 0x18)
                | (((x) & 0x00FF000000000000LL) >> 0x28)
                | (((x) & 0xFF00000000000000LL) >> 0x38));
    }
}

void merlinDetector::fromLabViewStr(const char *str)
{
    setStringParam(ADStringFromServer, str);
}

void merlinDetector::toLabViewStr(const char *str)
{
    setStringParam(ADStringToServer, str);
}

/** Helper function to copy a 64bit profile buffer into a 32Bit NDArray
 *
 *
 */

NDArray* merlinDetector::copyProfileToNDArray32(size_t *dims, char *buffer,
        int profileMask)
{
    epicsUInt32 *pData;
    epicsUInt32 *pWaveForm;
    uint64_t *pSrc;
    size_t x;
    int y;

    size_t profileDims[2];
    profileDims[0] = MAX(dims[0], dims[1]);
    profileDims[1] = 2;

    // for profiles we do a max dim * 2 array to hold both x
    // and y profile
    NDArray* pImage = this->pNDArrayPool->alloc(2, profileDims, NDUInt32, 0,
            NULL);

    asynPrint(this->pasynUserSelf, ASYN_TRACE_MPX,
            "%s:%s: Creating profile waveforms xsize %lu. ysize %lu\n",
            driverName, "copyProfileToNDArray32", dims[0], dims[1]);

    if (pImage == NULL)
    {
        asynPrint(this->pasynLabViewData, ASYN_TRACE_ERROR,
                "%s:%s: unable to allocate NDArray from pool\n", driverName,
                "copyProfileToNDArray32");
        setStringParam(ADStatusMessage,
                "Error: run out of buffers in detector driver");
    }
    else
    {
        // Copy the X,Y profile data into the (size * 2) NDArray
        // and into the X,Y waveforms
        pData = (epicsUInt32*) pImage->pData;
        for (x = 0, pWaveForm = (epicsUInt32 *) profileX, pSrc =
                (uint64_t *) ((buffer + MPX_IMG_HDR_LEN)); x < dims[0];
                x++, pWaveForm++, pSrc++, pData++)
        {
            endian_swap(*pSrc);
            *pWaveForm = (epicsUInt32) *pSrc;
            *pData = (epicsUInt32) *pSrc;
        }

        // Invert the Y profile (merlin origin is at bottom left)
        for (y = dims[1] - 1, pWaveForm = (epicsUInt32 *) profileY; y >= 0;
                y--, pWaveForm++, pSrc++, pData++)
        {
            endian_swap(*pSrc);
            *pWaveForm = (epicsUInt32) *pSrc;
            *pData = (epicsUInt32) *pSrc;
        }

        doCallbacksInt32Array(profileY, dims[1], merlinProfileY, 0);
        doCallbacksInt32Array(profileX, dims[0], merlinProfileX, 0);
    }
    return pImage;
}

/** Helper function to copy a 16 bit buffer into an NDArray
 *
 */
NDArray* merlinDetector::copyToNDArray16(size_t *dims, char *buffer, int offset)
{
    // copy the data into NDArray, switching to little endien and
    // Inverting in the Y axis (merlin origin is at bottom left)
    epicsUInt16 *pData, *pSrc;
    size_t x, y;

    NDArray* pImage = this->pNDArrayPool->alloc(2, dims, NDUInt16, 0, NULL);

    if (pImage == NULL)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "%s:%s: unable to allocate NDArray from pool\n", driverName,
                "copyToNDArray16");
        setStringParam(ADStatusMessage,
                "Error: run out of buffers in detector driver");
    }
    else
    {
        for (y = 0; y < dims[1]; y++)
        {
            for (x = 0, pData = (epicsUInt16 *) pImage->pData + y * dims[0], pSrc =
                    (epicsUInt16 *) (buffer + offset)
                            + (dims[1] - y) * dims[0]; x < dims[0];
                    x++, pData++, pSrc++)
            {
                *pData = *pSrc;
                endian_swap(*pData);
            }
        }
    }
    return pImage;
}

/** Helper function to copy a 32 bit buffer into an NDArray
 *
 */
NDArray* merlinDetector::copyToNDArray32(size_t* dims, char* buffer, int offset)
{
    epicsUInt32 *pData, *pSrc;
    size_t x, y;

    NDArray* pImage = this->pNDArrayPool->alloc(2, dims, NDUInt32, 0, NULL);

    if (pImage == NULL)
    {
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
                "%s:%s: unable to allocate NDArray from pool\n", driverName,
                "copyToNDArray32");
        setStringParam(ADStatusMessage,
                "Error: run out of buffers in detector driver");
    }
    else
    {
        for (y = 0; y < dims[1]; y++)
        {
            for (x = 0, pData = (epicsUInt32 *) pImage->pData + y * dims[0], pSrc =
                    (epicsUInt32 *) (buffer + offset)
                            + (dims[1] - y) * dims[0]; x < dims[0];
                    x++, pData++, pSrc++)
            {
                *pData = *pSrc;
                endian_swap(*pData);
            }
        }
    }
    return pImage;
}
asynStatus merlinDetector::setModeCommands(int function)
{
    asynStatus status;
    char value[MPX_MAXLINE];
    int counter1Enabled, continuousEnabled;

    if (function == merlinEnableCounter1)
    {
        status = getIntegerParam(merlinEnableCounter1, &counter1Enabled);
        if ((status != asynSuccess)
                || (counter1Enabled < 0 || counter1Enabled > 1))
        {
            counter1Enabled = 0;
            setIntegerParam(merlinEnableCounter1, counter1Enabled);
        }
        epicsSnprintf(value, MPX_MAXLINE, "%d", counter1Enabled);
        cmdConnection->mpxSet(MPXVAR_ENABLECOUNTER1, value,
                Labview_DEFAULT_TIMEOUT);
    }

    if (function == merlinContinuousRW)
    {
        status = getIntegerParam(merlinContinuousRW, &continuousEnabled);
        if ((status != asynSuccess)
                || (continuousEnabled < 0 || continuousEnabled > 1))
        {
            continuousEnabled = 0;
            setIntegerParam(merlinContinuousRW, continuousEnabled);
        }
        epicsSnprintf(value, MPX_MAXLINE, "%d", continuousEnabled);
        cmdConnection->mpxSet(MPXVAR_CONTINUOUSRW, value,
                Labview_DEFAULT_TIMEOUT);
    }

    epicsThreadSleep(.01);

    // now get the values again from the device -- it may reset them to consistent values
    // (presently only one of merlinContinuousRW or merlinEnableCounter1 can be set at a time)
    status = cmdConnection->mpxGet(MPXVAR_ENABLECOUNTER1,
            Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setIntegerParam(merlinEnableCounter1,
                atoi(cmdConnection->fromLabviewValue));

    status = cmdConnection->mpxGet(MPXVAR_CONTINUOUSRW,
            Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setIntegerParam(merlinContinuousRW,
                atoi(cmdConnection->fromLabviewValue));

    return (asynSuccess);
}

/* Set ROI parameters on detector - only supported on Manchester BPM
 *
 */
asynStatus merlinDetector::setROI()
{
    char value[MPX_MAXLINE];
    NDDimension_t arrayDims[DIMS];
    bool roiRequired;
    int param;

    if (detType == UomXBPM)
    {
        // determine ROI parameters
        memset(arrayDims, 0, sizeof(NDDimension_t) * DIMS);
        getIntegerParam(ADMinX, &param);
        arrayDims[0].offset = param;
        getIntegerParam(ADMinY, &param);
        arrayDims[1].offset = param;
        getIntegerParam(ADSizeX, &param);
        arrayDims[0].size = param;
        getIntegerParam(ADSizeY, &param);
        arrayDims[1].size = param;
        getIntegerParam(ADBinX, &arrayDims[0].binning);
        getIntegerParam(ADBinY, &arrayDims[1].binning);
        getIntegerParam(ADReverseX, &arrayDims[0].reverse);
        getIntegerParam(ADReverseY, &arrayDims[1].reverse);

        // validate ROI Parameters
        for (int dim = 0; dim < DIMS; dim++)
        {
            NDDimension_t* pDim = &arrayDims[dim];
            pDim->offset = MAX(pDim->offset, 0);
            pDim->offset = MIN(pDim->offset, maxSize[dim] - 1);
            pDim->size = MAX(pDim->size, 1);
            pDim->size = MIN(pDim->size,
                    maxSize[dim] - pDim->offset);
            pDim->binning = MAX(pDim->binning, 1);
            pDim->binning = MIN(pDim->binning, (int) pDim->size);
        }

        // Write back ROI parameters that may have changed
        setIntegerParam(ADMinX, arrayDims[0].offset);
        setIntegerParam(ADMinY, arrayDims[1].offset);
        setIntegerParam(ADSizeX, arrayDims[0].size);
        setIntegerParam(ADSizeY, arrayDims[1].size);
        setIntegerParam(ADBinX, arrayDims[0].binning);
        setIntegerParam(ADBinY, arrayDims[1].binning);

        roiRequired = arrayDims[0].offset != 0 || arrayDims[1].offset != 0
                || arrayDims[0].size != maxSize[0]
                || arrayDims[1].size != maxSize[1] || arrayDims[0].binning != 1
                || arrayDims[1].binning != 1 || arrayDims[0].reverse != 0
                || arrayDims[1].reverse != 0;

        epicsSnprintf(value, MPX_MAXLINE, "%lu %lu %lu %lu", arrayDims[0].offset,
                arrayDims[1].offset, arrayDims[0].size, arrayDims[1].size);
        cmdConnection->mpxSet(MPXVAR_ROI, value, Labview_DEFAULT_TIMEOUT);
    }
    return asynSuccess;
}

asynStatus merlinDetector::setAcquireParams()
{
    int triggerMode;
    char value[MPX_MAXLINE];
    asynStatus status;
//	char *substr = NULL;
//	int pixelCutOff = 0;

    // avoid chatty startup which keeps setting these values
    if (startingUp)
        return asynSuccess;

    if (detType == MedipixXBPM || detType == UomXBPM)
    {
        int exposures, val;

        getIntegerParam(ADNumExposures, &exposures);
        epicsSnprintf(value, MPX_MAXLINE, "%d", exposures);
        cmdConnection->mpxSet(MPXVAR_IMAGESTOSUM, value,
                Labview_DEFAULT_TIMEOUT);

        getIntegerParam(merlinEnableBackgroundCorr, &val);
        epicsSnprintf(value, MPX_MAXLINE, "%d", val);
        cmdConnection->mpxSet(MPXVAR_ENABLEBACKROUNDCORR, value,
                Labview_DEFAULT_TIMEOUT);

        getIntegerParam(merlinEnableImageSum, &val);
        epicsSnprintf(value, MPX_MAXLINE, "%d", val);
        cmdConnection->mpxSet(MPXVAR_ENABLEIMAGEAVERAGE, value,
                Labview_DEFAULT_TIMEOUT);
    }

    int numImages;
    status = getIntegerParam(ADNumImages, &numImages);
    if ((status != asynSuccess) || (numImages < 1))
    {
        numImages = 1;
        setIntegerParam(ADNumImages, numImages);
    }

    int numExposures;
    status = getIntegerParam(ADNumExposures, &numExposures);
    if ((status != asynSuccess) || (numExposures < 1))
    {
        numExposures = 1;
        setIntegerParam(ADNumExposures, numExposures);
    }

    int counterDepth;
    status = getIntegerParam(merlinCounterDepth, &counterDepth);
    if ((status != asynSuccess) || (counterDepth != 12 && counterDepth != 24)) // currently limited to 12/24 bit
    {
        counterDepth = 12;
        setIntegerParam(merlinCounterDepth, counterDepth);
    }

    double acquireTime;
    status = getDoubleParam(ADAcquireTime, &acquireTime);
    if ((status != asynSuccess) || (acquireTime < 0.))
    {
        acquireTime = 1.;
        setDoubleParam(ADAcquireTime, acquireTime);
    }

    double acquirePeriod;
    status = getDoubleParam(ADAcquirePeriod, &acquirePeriod);
    if ((status != asynSuccess) || (acquirePeriod < 0.))
    {
        acquirePeriod = 1.0;
        setDoubleParam(ADAcquirePeriod, acquirePeriod);
    }
    callParamCallbacks();

    // set the values enmasse - an attempt to fix the strange GUI updates caused by slow comms (failed)
    epicsSnprintf(value, MPX_MAXLINE, "%d", numExposures);
    cmdConnection->mpxSet(MPXVAR_NUMFRAMESPERTRIGGER, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%d", counterDepth);
    cmdConnection->mpxSet(MPXVAR_COUNTERDEPTH, value, Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%f", acquireTime * 1000); // translated into millisec
    cmdConnection->mpxSet(MPXVAR_ACQUISITIONTIME, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%f", acquirePeriod * 1000); // translated into millisec
    cmdConnection->mpxSet(MPXVAR_ACQUISITIONPERIOD, value,
            Labview_DEFAULT_TIMEOUT);

    status = getIntegerParam(ADTriggerMode, &triggerMode);
    if (status != asynSuccess)
        triggerMode = TMInternal;
    // merlin individually controls how start and stop triggers are read
    // here we translate the chosen trigger mode into a combination of start
    // and stop modes
    switch (triggerMode)
    {
    case TMInternal:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigInternal,
                Labview_DEFAULT_TIMEOUT);
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTOP, TMTrigInternal,
                Labview_DEFAULT_TIMEOUT);
        break;
    case TMExternalEnable:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigRising,
                Labview_DEFAULT_TIMEOUT);
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTOP, TMTrigFalling,
                Labview_DEFAULT_TIMEOUT);
        break;
    case TMExternalTriggerLow:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigFalling,
                Labview_DEFAULT_TIMEOUT);
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTOP, TMTrigInternal,
                Labview_DEFAULT_TIMEOUT);
        break;
    case TMExternalTriggerHigh:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigRising,
                Labview_DEFAULT_TIMEOUT);
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTOP, TMTrigInternal,
                Labview_DEFAULT_TIMEOUT);
        break;
    case TMExternalTriggerRising:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigRising,
                Labview_DEFAULT_TIMEOUT);
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTOP, TMTrigRising,
                Labview_DEFAULT_TIMEOUT);
        break;
    case TMSoftwareTrigger:
        cmdConnection->mpxSet(MPXVAR_TRIGGERSTART, TMTrigSoftware,
                Labview_DEFAULT_TIMEOUT);
        break;
    }

    // read the acquire period back from the server so that it can insert
    // the readback time if necessary
    cmdConnection->mpxGet(MPXVAR_ACQUISITIONPERIOD, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(ADAcquirePeriod,
                atof(cmdConnection->fromLabviewValue) / 1000); // translated into secs

    return (asynSuccess);

}

asynStatus merlinDetector::getThreshold()
{
    int status;

    if (startingUp)
        return asynSuccess;

    /* Read back the actual setting, in case we are out of bounds.*/
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD0, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold0,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD1, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold1,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD2, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold2,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD3, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold3,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD4, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold4,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD5, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold5,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD6, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold6,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THRESHOLD7, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinThreshold7,
                atof(cmdConnection->fromLabviewValue));

    status = cmdConnection->mpxGet(MPXVAR_OPERATINGENERGY,
            Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinOperatingEnergy,
                atof(cmdConnection->fromLabviewValue));

    callParamCallbacks();

    return (asynSuccess);
}

asynStatus merlinDetector::updateThresholdScanParms()
{
    asynStatus status = asynSuccess;
    char valueStr[MPX_MAXLINE];
    int thresholdScan;
    double start, stop, step;

    if (startingUp)
        return asynSuccess;

    getDoubleParam(merlinStartThresholdScan, &start);
    getDoubleParam(merlinStopThresholdScan, &stop);
    getDoubleParam(merlinStepThresholdScan, &step);
    getIntegerParam(merlinThresholdScan, &thresholdScan);

    epicsSnprintf(valueStr, MPX_MAXLINE, "%f", start);
    status = cmdConnection->mpxSet(MPXVAR_THSTART, valueStr,
            Labview_DEFAULT_TIMEOUT);

    if (status == asynSuccess)
    {
        epicsSnprintf(valueStr, MPX_MAXLINE, "%f", stop);
        status = cmdConnection->mpxSet(MPXVAR_THSTOP, valueStr,
                Labview_DEFAULT_TIMEOUT);
    }
    if (status == asynSuccess)
    {
        epicsSnprintf(valueStr, MPX_MAXLINE, "%f", step);
        status = cmdConnection->mpxSet(MPXVAR_THSTEP, valueStr,
                Labview_DEFAULT_TIMEOUT);
    }
    if (status == asynSuccess)
    {
        epicsSnprintf(valueStr, MPX_MAXLINE, "%d", thresholdScan);
        status = cmdConnection->mpxSet(MPXVAR_THSSCAN, valueStr,
                Labview_DEFAULT_TIMEOUT);
    }

    /* Read back the actual setting, in case we are out of bounds.*/
    status = cmdConnection->mpxGet(MPXVAR_THSTART, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinStartThresholdScan,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THSTEP, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinStepThresholdScan,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THSTOP, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setDoubleParam(merlinStopThresholdScan,
                atof(cmdConnection->fromLabviewValue));
    status = cmdConnection->mpxGet(MPXVAR_THSSCAN, Labview_DEFAULT_TIMEOUT);
    if (status == asynSuccess)
        setIntegerParam(merlinThresholdScan,
                atoi(cmdConnection->fromLabviewValue));

    return status;
}

static void merlinTaskC(void *drvPvt)
{
    merlinDetector *pPvt = (merlinDetector *) drvPvt;

    pPvt->merlinTask();
}

static void merlinStatusC(void *drvPvt)
{
    merlinDetector *pPvt = (merlinDetector *) drvPvt;

    pPvt->merlinStatus();
}

/** This thread periodically read the detector status (temperature, humidity, etc.)
 It does not run if we are acquiring data, to avoid polling Labview when taking data.*/
void merlinDetector::merlinStatus()
{
    int result = asynSuccess;
    int status = 0;
    int statusCode;

// let the startup script complete before attempting I/O
    epicsThreadSleep(4);
    startingUp = 0;

    this->lock();

// make sure important grouped variables are set to agree with
// IOCs auto saved values
    setAcquireParams();
    setROI();
    updateThresholdScanParms();
    getThreshold();

    result = cmdConnection->mpxGet(MPXVAR_GETSOFTWAREVERSION,
            Labview_DEFAULT_TIMEOUT);
    statusCode = atoi(cmdConnection->fromLabviewValue);

// initial status
    setIntegerParam(ADStatus, ADStatusIdle);

    this->unlock();

    while (1)
    {
        epicsThreadSleep(4);
        this->lock();
        getIntegerParam(ADStatus, &status);

        if (status == ADStatusIdle)
        {
            setStringParam(ADStatusMessage, "Waiting for acquire command");
            callParamCallbacks();
        }
        this->unlock();
    }

}

/** sets one of the 6 modes for Merlin versions since Quad Merlin
 * these modes combine sensible combinations of 5 individual settings
 * on the device
 *
 * \param[in] mode the mode number
 */
asynStatus merlinDetector::SetQuadMode(int mode)
{
    char value[MPX_MAXLINE];
    asynStatus result = asynSuccess;
    int bits = 12;
    int colourMode = 0;
    int enableCounter1 = 0;
    int continuousRW = 0;
    int chargeSumming = 0;

    this->framesPerAcquire = 1;

    switch(mode)
    {
    case MPXQuadMode12Bit:
        break;
    case MPXQuadMode24Bit:
        bits = 24;
        break;
    case MPXQuadMode2Threshold:
        enableCounter1 = 1;
        this->framesPerAcquire = 2;
        enableCounter1 = 2;
        break;
    case MPXQuadModeContinuousRW:
        continuousRW = 1;
        break;
    case MPXQuadModeColour:
        colourMode = 1;
        enableCounter1 = 2;
        this->framesPerAcquire = 8;
        break;
    case MPXQuadModeSumming:
        chargeSumming = 1;
        enableCounter1 = 1;
        break;
    default:
        result = asynError;
        break;
    }

    setIntegerParam(merlinCounterDepth, bits);

    epicsSnprintf(value, MPX_MAXLINE, "%d", bits);
    cmdConnection->mpxSet(MPXVAR_COUNTERDEPTH, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%d", enableCounter1);
    cmdConnection->mpxSet(MPXVAR_ENABLECOUNTER1, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%d", continuousRW);
    cmdConnection->mpxSet(MPXVAR_CONTINUOUSRW, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%d", colourMode);
    cmdConnection->mpxSet(MPXVAR_COLOURMODE, value,
            Labview_DEFAULT_TIMEOUT);
    epicsSnprintf(value, MPX_MAXLINE, "%d", chargeSumming);
    cmdConnection->mpxSet(MPXVAR_CHARGESUMMING, value,
            Labview_DEFAULT_TIMEOUT);

    return result;
}

/** Called when asyn clients call pasynInt32->write().
 * This function performs actions for some parameters, including ADAcquire, ADTriggerMode, etc.
 * For all parameters it sets the value in the parameter library and calls any registered callbacks..
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Value to write. */
asynStatus merlinDetector::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    char strVal[MPX_MAXLINE];
    int function = pasynUser->reason;
    int adstatus;
    int imageMode, imagesToAcquire, profileMaskParm;
    asynStatus status = asynSuccess;
    const char *functionName = "writeInt32";

    status = setIntegerParam(function, value);

    if (function == merlinReset)
    {
        cmdConnection->mpxCommand(MPXCMD_RESET, Labview_DEFAULT_TIMEOUT);
// I cannot successfully reconnect to the server after a reset
// the only solution found so far is to restart the ioc
        exit(0);
    }
    else if (function == merlinQuadMerlinMode)
    {
        this->SetQuadMode(value);
    }
    else if (function == merlinSoftwareTrigger)
    {
        cmdConnection->mpxCommand(MPXCMD_SOFTWARETRIGGER,
                Labview_DEFAULT_TIMEOUT);
    }
    else if (function == ADAcquire)
    {
        getIntegerParam(ADStatus, &adstatus);
        if (value && (adstatus == ADStatusIdle || adstatus == ADStatusError))
        {
            setIntegerParam(ADStatus, ADStatusAcquire);
            setStringParam(ADStatusMessage, "Acquiring...");
            // reset the image count - this is then used to determine when acquisition is complete
            setIntegerParam(ADNumImagesCounter, 0);
            getIntegerParam(ADNumImages, &imagesToAcquire);
            // set number of images to acquire based on the capture mode
            getIntegerParam(ADImageMode, &imageMode);
            getIntegerParam(merlinProfileControl, &profileMaskParm);

            switch (imageMode)
            {
            case MPXImageSingle:
                imagesRemaining = framesPerAcquire;
                break;
            case MPXImageMultiple:
                imagesRemaining = imagesToAcquire * framesPerAcquire;
                break;
            case MPXImageContinuous:
                imagesToAcquire = 0;
                imagesRemaining = -1;
                break;
            case MPXThresholdScan:
                double start, stop, step;
                getDoubleParam(merlinStartThresholdScan, &start);
                getDoubleParam(merlinStopThresholdScan, &stop);
                getDoubleParam(merlinStepThresholdScan, &step);
                imagesRemaining = (int) ((stop - start) / step);
                setStringParam(ADStatusMessage, "Performing Threshold Scan...");
                setIntegerParam(ADNumImages, 1); // internally Merlin does this so we set EPICS PV to match
                break;
            case MPXBackgroundCalibrate:
                // we only get a single image back from the server for all the
                // background images we take
                imagesRemaining = 1;
                break;
            }

            if (imageMode == MPXThresholdScan)
            {
                status = cmdConnection->mpxCommand(MPXCMD_THSCAN,
                        Labview_DEFAULT_TIMEOUT);
            }
            else if (imageMode == MPXBackgroundCalibrate)
            {
                epicsSnprintf(strVal, MPX_MAXLINE, "%d", imagesToAcquire);
                cmdConnection->mpxSet(MPXVAR_BACKGROUNDCOUNT, strVal,
                        Labview_DEFAULT_TIMEOUT);
                status = cmdConnection->mpxCommand(MPXCMD_BACKGROUNDACQUIRE,
                        Labview_DEFAULT_TIMEOUT);
            }
            else // a standard image acquisition (or profile acquisition)
            {
                epicsSnprintf(strVal, MPX_MAXLINE, "%d", imagesToAcquire);
                cmdConnection->mpxSet(MPXVAR_NUMFRAMESTOACQUIRE, strVal,
                        Labview_DEFAULT_TIMEOUT);

                if (profileMaskParm & (MPXPROFILES_IMAGE == MPXPROFILES_IMAGE))
                {
                    cmdConnection->mpxCommand(MPXCMD_STARTACQUISITION,
                            Labview_DEFAULT_TIMEOUT);
                }
                else
                {
                    cmdConnection->mpxCommand(MPXCMD_PROFILES,
                            Labview_DEFAULT_TIMEOUT);
                }
            }
        }
        if (!value && (adstatus == ADStatusAcquire))
        {
            setIntegerParam(ADStatus, ADStatusIdle);
            cmdConnection->mpxCommand(MPXCMD_STOPACQUISITION,
                    Labview_DEFAULT_TIMEOUT);
        }
    }
    else if ((function == ADTriggerMode) || (function == ADNumImages)
            || (function == ADNumExposures) || (function == merlinCounterDepth)
            || (function == merlinEnableBackgroundCorr)
            || (function == merlinEnableImageSum))
    {
        setAcquireParams();
    }
    else if ((function == ADSizeX) || (function == ADSizeY)
            || (function == ADMinX) || (function == ADMinY))
    {
        setROI();
    }
    else if ((function == merlinEnableCounter1
            || function == merlinContinuousRW))
    {
        setModeCommands(function);
    }
    else if (function == merlinThresholdApply)
    {
        getThreshold();
    }
    else if (function == merlinProfileControl)
    {
        epicsSnprintf(strVal, MPX_MAXLINE, "%d", value);
        cmdConnection->mpxSet(MPXCMD_PROFILECONTROL, strVal,
                Labview_DEFAULT_TIMEOUT);
        setIntegerParam(merlinProfileControl, value);
    }
    else
    {
// function numbers are assigned sequentially via createParam in the constructor and hence
// any function numbers lower than our first function is handled by a (the) super class
        if (function < FIRST_merlin_PARAM)
            status = ADDriver::writeInt32(pasynUser, value);
    }

    /* Do callbacks so higher layers see any changes */
    callParamCallbacks();

    if (status)
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                "%s:%s: error, status=%d, reason=%d value=%d\n",
                driverName, functionName, status, function, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                "%s:%s: reason=%d, value=%d\n", driverName, functionName,
                function, value);
    return status;
}

/** Called when asyn clients call pasynFloat64->write().
 * This function performs actions for some parameters, including ADAcquireTime, ADGain, etc.
 * For all parameters it sets the value in the parameter library and calls any registered callbacks..
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Value to write. */
asynStatus merlinDetector::writeFloat64(asynUser *pasynUser,
        epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    const char *functionName = "writeFloat64";
    char value_str[MPX_MAXLINE];
    double oldValue;

    /* Set the parameter and readback in the parameter library.  This may be overwritten when we read back the
     * status at the end, but that's OK */
    getDoubleParam(function, &oldValue);
    status = setDoubleParam(function, value);

    /* Changing any of the following parameters requires recomputing the base image */
    if (function == merlinThreshold0)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD0, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold1)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD1, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold2)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD2, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold3)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD3, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold4)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD4, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold5)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD5, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold6)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD6, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinThreshold7)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_THRESHOLD7, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if (function == merlinOperatingEnergy)
    {
        epicsSnprintf(value_str, MPX_MAXLINE, "%f", value);
        status = cmdConnection->mpxSet(MPXVAR_OPERATINGENERGY, value_str,
                Labview_DEFAULT_TIMEOUT);
        getThreshold();
    }
    else if ((function == ADAcquireTime) || (function == ADAcquirePeriod))
    {
        setAcquireParams();
    }
    else if ((function == merlinStartThresholdScan)
            || (function == merlinStopThresholdScan)
            || (function == merlinStepThresholdScan))
    {
        updateThresholdScanParms();
    }
    else
    {
        /* If this parameter belongs to a base class call its method */
        if (function < FIRST_merlin_PARAM)
            status = ADDriver::writeFloat64(pasynUser, value);
    }

    if (status)
    {
        /* Something went wrong so we set the old value back */
        setDoubleParam(function, oldValue);
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                "%s:%s error, status=%d function=%d, value=%f\n", driverName,
                functionName, status, function, value);
    }
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                "%s:%s: function=%d, value=%f\n", driverName, functionName,
                function, value);

    /* Do callbacks so higher layers see any changes */
    callParamCallbacks();
    return status;
}


/** Report status of the driver.
 * Prints details about the driver if details>0.
 * It then calls the ADDriver::report() method.
 * \param[in] fp File pointed passed by caller where the output is written to.
 * \param[in] details If >0 then driver details are printed.
 */
void merlinDetector::report(FILE *fp, int details)
{

    fprintf(fp, "merlin detector %s\n", this->portName);
    if (details > 0)
    {
        int nx, ny, dataType;
        getIntegerParam(ADSizeX, &nx);
        getIntegerParam(ADSizeY, &ny);
        getIntegerParam(NDDataType, &dataType);
        fprintf(fp, "  NX, NY:            %d  %d\n", nx, ny);
        fprintf(fp, "  Data type:         %d\n", dataType);
    }
    /* Invoke the base class method */
    ADDriver::report(fp, details);
}

extern "C" int merlinDetectorConfig(const char *portName,
        const char *LabviewCommandPort, const char *LabviewDataPort,
        int maxSizeX, int maxSizeY, int detectorType, int maxBuffers,
        size_t maxMemory, int priority, int stackSize)
{
    new merlinDetector(portName, LabviewCommandPort, LabviewDataPort, maxSizeX,
            maxSizeY, detectorType, maxBuffers, maxMemory, priority, stackSize);
    return (asynSuccess);
}

/** Constructor for merlin driver; most parameters are simply passed to ADDriver::ADDriver.
 * After calling the base class constructor this method creates a thread to collect the detector data,
 * and sets reasonable default values for the parameters defined in this class, asynNDArrayDriver, and ADDriver.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] LabviewPort The name of the asyn port previously created with drvAsynIPPortConfigure to
 *            communicate with Labview.
 * \param[in] maxSizeX The size of the merlin detector in the X direction.
 * \param[in] maxSizeY The size of the merlin detector in the Y direction.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] maxBuffers The maximum number of NDArray buffers that the NDArrayPool for this driver is
 *            allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
 * \param[in] maxMemory The maximum amount of memory that the NDArrayPool for this driver is
 *            allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
 * \param[in] priority The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
 * \param[in] stackSize The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
 */
merlinDetector::merlinDetector(const char *portName,
        const char *LabviewCommandPort, const char *LabviewDataPort,
        int maxSizeX, int maxSizeY, int detectorType, int maxBuffers,
        size_t maxMemory, int priority, int stackSize)

:
        ADDriver(portName, 1, NUM_merlin_PARAMS, maxBuffers, maxMemory,
                asynInt32ArrayMask | asynFloat64ArrayMask
                        | asynGenericPointerMask | asynInt16ArrayMask,
                asynInt32ArrayMask | asynFloat64ArrayMask
                        | asynGenericPointerMask | asynInt16ArrayMask,
                ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=0, autoConnect=1 */
                priority, stackSize),
        imagesRemaining(0)

{
    int status = asynSuccess;
    const char *functionName = "merlinDetector";
    size_t dims[2];

    startingUp = 1;
    strcpy(LabviewCommandPortName, LabviewCommandPort);
    strcpy(LabviewDataPortName, LabviewDataPort);

    detType = (merlinDetectorType) detectorType;

    /* Allocate the raw buffer we use to read image files.  Only do this once */
    dims[0] = maxSizeX;
    dims[1] = maxSizeY;
    /* Allocate the raw buffer we use for flat fields. */
    this->pFlatField = this->pNDArrayPool->alloc(2, dims, NDUInt32, 0, NULL);

    // merlin is upside down by area detector standards
    // this does not work - I need to invert using my own memory copy function
    // this->ADReverseY = 1;
    // Update: this could be made to work by calling the copy NDArray function
    // (in base class) but perhaps more efficient to do in memory inversion alone

    /* Connect to Labview */
    status = pasynOctetSyncIO->connect(LabviewCommandPort, 0,
            &this->pasynLabViewCmd, NULL);
    status = pasynOctetSyncIO->connect(LabviewDataPort, 0,
            &this->pasynLabViewData, NULL);

    cmdConnection = new mpxConnection(pasynUserSelf, pasynLabViewCmd, this);
    dataConnection = new mpxConnection(pasynUserSelf, pasynLabViewData, this);

    cmdConnection->mpxCommand(MPXCMD_STOPACQUISITION, Labview_DEFAULT_TIMEOUT);

    createParam(merlinDelayTimeString, asynParamFloat64, &merlinDelayTime);
    createParam(merlinThreshold0String, asynParamFloat64, &merlinThreshold0);
    createParam(merlinThreshold1String, asynParamFloat64, &merlinThreshold1);
    createParam(merlinThreshold2String, asynParamFloat64, &merlinThreshold2);
    createParam(merlinThreshold3String, asynParamFloat64, &merlinThreshold3);
    createParam(merlinThreshold4String, asynParamFloat64, &merlinThreshold4);
    createParam(merlinThreshold5String, asynParamFloat64, &merlinThreshold5);
    createParam(merlinThreshold6String, asynParamFloat64, &merlinThreshold6);
    createParam(merlinThreshold7String, asynParamFloat64, &merlinThreshold7);
    createParam(merlinOperatingEnergyString, asynParamFloat64,
            &merlinOperatingEnergy);
    createParam(merlinThresholdApplyString, asynParamInt32,
            &merlinThresholdApply);
    createParam(merlinThresholdAutoApplyString, asynParamInt32,
            &merlinThresholdAutoApply);
    createParam(merlinArmedString, asynParamInt32, &merlinArmed);

    createParam(merlinmedpixThresholdScanString, asynParamInt32,
            &merlinThresholdScan);
    createParam(merlinStartThresholdScanString, asynParamFloat64,
            &merlinStartThresholdScan);
    createParam(merlinStopThresholdScanString, asynParamFloat64,
            &merlinStopThresholdScan);
    createParam(merlinStepThresholdScanString, asynParamFloat64,
            &merlinStepThresholdScan);
    createParam(merlinCounterDepthString, asynParamInt32,
            &merlinCounterDepth);
    createParam(merlinResetString, asynParamInt32, &merlinReset);
    createParam(merlinSoftwareTriggerString, asynParamInt32,
            &merlinSoftwareTrigger);

    createParam(merlinEnableCounter1String, asynParamInt32,
            &merlinEnableCounter1);
    createParam(merlinContinuousRWString, asynParamInt32,
            &merlinContinuousRW);

    // XBPM Specific parameters
    createParam(merlinProfileControlString, asynParamInt32,
            &merlinProfileControl);
    int res = createParam(merlinProfileXString, asynParamInt32Array,
            &merlinProfileX);
    res = createParam(merlinProfileYString, asynParamInt32Array,
            &merlinProfileY);

    // UoM BPM specific
    createParam(merlinEnableBackgroundCorrString, asynParamInt32,
            &merlinEnableBackgroundCorr);
    createParam(merlinEnableImageSumString, asynParamInt32,
            &merlinEnableImageSum);

    // Merlin Quad(and greater) Specific
    createParam(merlinQuadMerlinModeString, asynParamInt32,
            &merlinQuadMerlinMode);
    createParam(merlinSelectGuiString, asynParamOctet,
            &merlinSelectGui);

    setStringParam(merlinSelectGui, "merlinEmbedded.edl");

    /* Set some default values for parameters */
    switch (detectorType)
    {
    case MedipixXBPM:
        setStringParam(ADManufacturer, "Medipix Consortium");
        setStringParam(ADModel, "Lancelot XBPM");
        break;
    case Merlin:
        setStringParam(ADManufacturer, "Medipix Consortium");
        setStringParam(ADModel, "Merlin");
        break;
    case UomXBPM:
        setStringParam(ADManufacturer, "University of Manchester");
        setStringParam(ADModel, "UoM XBPM");
        break;
    case MerlinQuad:
        setStringParam(ADManufacturer, "Medipix Consortium");
        setStringParam(ADModel, "Merlin Quad");
        setStringParam(merlinSelectGui, "merlinQuadEmbedded.edl");
        break;

    }
    status = setIntegerParam(ADMaxSizeX, maxSizeX);
    status |= setIntegerParam(ADMaxSizeY, maxSizeY);
    status |= setIntegerParam(ADSizeX, maxSizeX);
    status |= setIntegerParam(ADSizeX, maxSizeX);
    status |= setIntegerParam(ADSizeY, maxSizeY);
    status |= setIntegerParam(NDArraySizeX, maxSizeX);
    status |= setIntegerParam(NDArraySizeY, maxSizeY);
    status |= setIntegerParam(NDArraySize, 0);
    status |= setIntegerParam(NDDataType, NDUInt32);
    status |= setIntegerParam(ADImageMode, ADImageContinuous);
    status |= setIntegerParam(ADTriggerMode, TMInternal);
    status |= setIntegerParam(merlinProfileControl, MPXPROFILES_IMAGE);

    this->maxSize[0] = maxSizeX;
    this->maxSize[1] = maxSizeY;

// attempt to clear the spurious error on startup (failed - not sure where this is coming from?)
//      Medipix1TestFileName devAsynOctet: writeIt requested 0 but sent 10780660 bytes
    printf("****************************\n");
    status |= setStringParam(NDFileName, "image.bmp");

// allocate space for the waveforms
    this->profileX = (int*) malloc(maxSizeX * sizeof(int));
    this->profileY = (int*) malloc(maxSizeY * sizeof(int));

    if (status)
    {
        printf("%s: unable to set camera parameters\n", functionName);
        return;
    }

    /* Create the thread that updates the images */
    status = (epicsThreadCreate("merlinDetTask", epicsThreadPriorityMedium,
            epicsThreadGetStackSize(epicsThreadStackMedium),
            (EPICSTHREADFUNC) merlinTaskC, this) == NULL);
    if (status)
    {
        printf("%s:%s epicsThreadCreate failure for image task\n", driverName,
                functionName);
        return;
    }

    /* Create the thread that monitors detector status (temperature, humidity, etc). */
    status = (epicsThreadCreate("merlinStatusTask", epicsThreadPriorityMedium,
            epicsThreadGetStackSize(epicsThreadStackMedium),
            (EPICSTHREADFUNC) merlinStatusC, this) == NULL);
    if (status)
    {
        printf("%s:%s epicsThreadCreate failure for status task\n", driverName,
                functionName);
        return;
    }

}

/* Code for iocsh registration */
static const iocshArg merlinDetectorConfigArg0 =
{ "Port name", iocshArgString };
static const iocshArg merlinDetectorConfigArg1 =
{ "Labview cmd port", iocshArgString };
static const iocshArg merlinDetectorConfigArg2 =
{ "Labview data port", iocshArgString };
static const iocshArg merlinDetectorConfigArg3 =
{ "maxSizeX", iocshArgInt };
static const iocshArg merlinDetectorConfigArg4 =
{ "maxSizeY", iocshArgInt };
static const iocshArg merlinDetectorConfigArg5 =
{ "detectorType", iocshArgInt };
static const iocshArg merlinDetectorConfigArg6 =
{ "maxBuffers", iocshArgInt };
static const iocshArg merlinDetectorConfigArg7 =
{ "maxMemory", iocshArgInt };
static const iocshArg merlinDetectorConfigArg8 =
{ "priority", iocshArgInt };
static const iocshArg merlinDetectorConfigArg9 =
{ "stackSize", iocshArgInt };
static const iocshArg * const merlinDetectorConfigArgs[] =
{ &merlinDetectorConfigArg0, &merlinDetectorConfigArg1,
        &merlinDetectorConfigArg2, &merlinDetectorConfigArg3,
        &merlinDetectorConfigArg4, &merlinDetectorConfigArg5,
        &merlinDetectorConfigArg6, &merlinDetectorConfigArg7,
        &merlinDetectorConfigArg8, &merlinDetectorConfigArg9 };
static const iocshFuncDef configmerlinDetector =
{ "merlinDetectorConfig", 9, merlinDetectorConfigArgs };
static void configmerlinDetectorCallFunc(const iocshArgBuf *args)
{
    merlinDetectorConfig(args[0].sval, args[1].sval, args[2].sval,
            args[3].ival, args[4].ival, args[5].ival, args[6].ival,
            args[7].ival, args[8].ival, args[9].ival);
}

static void merlinDetectorRegister(void)
{

    iocshRegister(&configmerlinDetector, configmerlinDetectorCallFunc);
}

extern "C"
{
epicsExportRegistrar(merlinDetectorRegister);
}

