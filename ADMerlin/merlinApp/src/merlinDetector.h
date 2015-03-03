/*
 * merlinDetector.h
 *
 *  Created on: 16 Oct 2013
 *      Author: hgv27681
 */

#ifndef MERLINDETECTOR_H_
#define MERLINDETECTOR_H_

/** Messages to/from Labview command channel */
#define MAX_MESSAGE_SIZE 256
#define MAX_FILENAME_LEN 256
#define MAX_BAD_PIXELS 100
/** Time to poll when reading from Labview */
#define ASYN_POLL_TIME .01
#define Labview_DEFAULT_TIMEOUT 2.0
/** Time between checking to see if image file is complete */
#define FILE_READ_DELAY .01

#define DIMS 2

/** Detector Types */
typedef enum
{
    Merlin, MedipixXBPM, UomXBPM, MerlinQuad
} merlinDetectorType;

/** Trigger modes */
typedef enum
{
    TMInternal,
    TMExternalEnable,
    TMExternalTriggerHigh,
    TMExternalTriggerLow,
    TMExternalTriggerRising,
    TMSoftwareTrigger
} merlinTriggerMode;/** Trigger modes */

/** Enumeration of image collection modes */
typedef enum
{
    MPXImageSingle,      /**< Collect a single image per Acquire command */
    MPXImageMultiple,    /**< Collect ADNumImages images per Acquire command */
    MPXImageContinuous,  /**< Collect images continuously until Acquire is set to 0 */
    MPXThresholdScan,
    MPXBackgroundCalibrate
} MPXImageMode_t;


/** Enumeration of merlin quad modes */
typedef enum
{
    MPXQuadMode12Bit,
    MPXQuadMode24Bit,
    MPXQuadMode2Threshold,
    MPXQuadModeContinuousRW,
    MPXQuadModeColour,
    MPXQuadModeSumming
} MPXQuadMode_t;

/** Medipix Individual Trigger types */

#define TMTrigInternal  (char*)"0"
#define TMTrigRising    (char*)"1"
#define TMTrigFalling   (char*)"2"
#define TMTrigSoftware  (char*)"3"

/** ASYN PARAMETER NAMES **/

#define merlinDelayTimeString              "DELAY_TIME"
#define merlinThreshold0String             "THRESHOLD0"
#define merlinThreshold1String             "THRESHOLD1"
#define merlinThreshold2String             "THRESHOLD2"
#define merlinThreshold3String             "THRESHOLD3"
#define merlinThreshold4String             "THRESHOLD4"
#define merlinThreshold5String             "THRESHOLD5"
#define merlinThreshold6String             "THRESHOLD6"
#define merlinThreshold7String             "THRESHOLD7"
#define merlinOperatingEnergyString        "OPERATINGENERGY"

#define merlinThresholdApplyString         "THRESHOLD_APPLY"
#define merlinThresholdAutoApplyString     "THRESHOLD_AUTO_APPLY"
#define merlinArmedString                  "ARMED"

#define merlinmedpixThresholdScanString    "THRESHOLDSCAN"
#define merlinStartThresholdScanString     "THRESHOLDSTART"
#define merlinStopThresholdScanString      "THRESHOLDSTOP"
#define merlinStepThresholdScanString      "THRESHOLDSTEP"
#define merlinStartThresholdScanningString "STARTTHRESHOLDSCANNING"
#define merlinCounterDepthString           "COUNTERDEPTH"
#define merlinResetString                  "RESET"
#define merlinSoftwareTriggerString        "SOFTWARETRIGGER"
#define merlinEnableCounter1String         "ENABLECOUNTER1"
#define merlinContinuousRWString           "CONTINUOUSRW"

// Medipix XBPM SPECIFIC
#define merlinProfileControlString         "PROFILECONTROL"
#define merlinProfileXString               "PROFILE_AVERAGE_X"
#define merlinProfileYString               "PROFILE_AVERAGE_Y"

// UoM BPM SPECIFIC
#define merlinEnableBackgroundCorrString   "ENABLEBACKGROUNDCORR"
#define merlinEnableImageSumString         "ENABLESUMAVERAGE"

// Medipix Quad
#define merlinQuadMerlinModeString         "QUADMERLINMODE"
#define merlinSelectGuiString              "SELECTGUI"

class mpxConnection;

/** Driver for Quantum Detectors merlin pixel array detectors using their Labview server over TCP/IP socket */
class merlinDetector: public ADDriver
{
public:
    merlinDetector(const char *portName, const char *LabviewCmdPort,
            const char *LabviewDataPort, int maxSizeX, int maxSizeY,
            int detectorType, int maxBuffers, size_t maxMemory, int priority,
            int stackSize);

    /* These are the methods that we override from ADDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
//    virtual asynStatus writeOctet(asynUser *pasynUser, const char *value,
//            size_t nChars, size_t *nActual);
    void report(FILE *fp, int details);
    void merlinTask(); /* This should be private but is called from C so must be public */
    void merlinStatus(); /* This should be private but is called from C so must be public */

    void fromLabViewStr(const char *str);
    void toLabViewStr(const char *str);

protected:
    int merlinDelayTime;
#define FIRST_merlin_PARAM merlinDelayTime
    int merlinThreshold0;
    int merlinThreshold1;
    int merlinThreshold2;
    int merlinThreshold3;
    int merlinThreshold4;
    int merlinThreshold5;
    int merlinThreshold6;
    int merlinThreshold7;
    int merlinOperatingEnergy;
    int merlinThresholdApply;
    int merlinThresholdAutoApply;
    int merlinArmed;
    int merlinThresholdScan;
    int merlinStartThresholdScan;
    int merlinStopThresholdScan;
    int merlinStepThresholdScan;
    int merlinTvxVersion;
    int merlinCounterDepth;
    int merlinSoftwareTrigger;
    int merlinReset;
    int merlinEnableCounter1;
    int merlinContinuousRW;
    int merlinProfileControl;
    int merlinProfileX;
    int merlinProfileY;
    int merlinEnableBackgroundCorr;
    int merlinEnableImageSum;
    int merlinQuadMerlinMode;
    int merlinSelectGui;

#define LAST_merlin_PARAM merlinSelectGui

private:
    /* These are the methods that are new to this class */
    void abortAcquisition();
    asynStatus SetQuadMode(int mode);
    asynStatus setModeCommands(int function);
    asynStatus setAcquireParams();
    asynStatus getThreshold();
    asynStatus updateThresholdScanParms();
    asynStatus setROI();

    NDArray* copyProfileToNDArray32(size_t *dims, char *buffer,
            int profileMask);
    NDArray* copyToNDArray16(size_t *dims, char *buffer, int offset);
    NDArray* copyToNDArray32(size_t *dims, char *buffer, int offset);
    inline void endian_swap(unsigned short& x);
    inline void endian_swap(unsigned int& x);
    inline void endian_swap(uint64_t& x);
    unsigned int maxSize[2];

    /* Our data */
    int imagesRemaining;
    NDArray *pFlatField;
    int multipleFileNumber;
    asynUser *pasynLabViewCmd;
    asynUser *pasynLabViewData;
    double averageFlatField;
    int framesPerAcquire;

    int *profileX;
    int *profileY;

    bool startingUp;  // used to avoid very chatty initialisation

    char LabviewCommandPortName[20];
    char LabviewDataPortName[20];

    merlinDetectorType detType;

    mpxConnection *cmdConnection;
    mpxConnection *dataConnection;
};

#define NUM_merlin_PARAMS (&LAST_merlin_PARAM - &FIRST_merlin_PARAM + 1)

static const char *driverName = "merlinDetector";

#endif /* MERLINDETECTOR_H_ */
