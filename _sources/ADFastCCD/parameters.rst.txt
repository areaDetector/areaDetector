Implementation of standard driver parameters
============================================

The driver redefines the choices for several of the parameters defined
in ADDriver.h. The ADTriggerMode choices for the FastCCD are:

*  Internal (Software trigger)
*  External 1 (external trigger on input 1)
*  External 2 (external trigger on input 2)
*  External 1+2 (external trigger on input 1 OR input 2)

The NDDataType choices for the FastCCD are:

*  NDUInt16 (16 bit data)

The NDColorMode choices for the FastCCD are:

*  NDColorModeMono (monochromatic data)


Implementation of FastCCD specific parameters
=============================================

The FastCCD driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h:

FastCCD Image Size and Overscan Settings
----------------------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <td>
    FastCCDOverscanRows
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Specify the number of overscan rows to use when descrambling
    </td>
    <td>
    OVERSCAN_ROWS
    </td>
    <td>
    $(P)$(R)OverscanRows<br />$(P)$(R)OverscanRows_RBV
    </td>
    <td>
    longout<br />longin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDOverscanCols
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Specify the number of overscan columns to use when descrambling
    </td>
    <td>
    OVERSCAN_COLS
    </td>
    <td>
    $(P)$(R)OverscanCols<br />$(P)$(R)OverscanCols_RBV
    </td>
    <td>
    longout<br />longin
    </td>
    </tr>
    </tbody>
    </table>


FastCCD CIN Power Settings
--------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDPower
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Powers on and off the CIN (0 = off, 1 = on)
    </td>
    <td>
    CIN_POWER
    </td>
    <td>
    $(P)$(R)Power<br />$(P)$(R)Power_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFPPower
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Powers on and off the CIN front panel (0 = off, 1 = on)
    </td>
    <td>
    CIN_POWER
    </td>
    <td>
    $(P)$(R)FPPower<br />$(P)$(R)FPPower_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    </tbody>
    </table>


FastCCD Power settings (Bias and Clocking)
------------------------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDCameraPowerMode
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Set the power mode of the FastCCD cmaera head to be turned on and off by <em>FastCCDCameraPower</em>. Possible values are: NONE<br /> CLOCKS<br /> BIAS<br /> BIAS AND CLOCKS
    </td>
    <td>
    CAMERA_POWER_MODE
    </td>
    <td>
    $(P)$(R)CameraPwrMode<br />$(P)$(R)CameraPwrMode_RBV
    </td>
    <td>
    mbbo<br />mbbi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDCameraPower
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Powers on and off the FastCCD camera head (0 = off, 1 = on)
    </td>
    <td>
    CAMERA_POWER
    </td>
    <td>
    $(P)$(R)CameraPwr<br />$(P)$(R)CameraPwr_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDBias
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    ro
    </td>
    <td>
    Indicates if the CIN is requesting the camera head to turn on or off the bias power (0 = off, 1 = on)
    </td>
    <td>
    BIAS
    </td>
    <td>
    $(P)$(R)Bias_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDClocks
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    ro
    </td>
    <td>
    Indicates if the CIN is requesting the camera head to turn on or off the clocking power (0 = off, 1 = on)
    </td>
    <td>
    CLOCKS
    </td>
    <td>
    $(P)$(R)Clocks_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    </tbody>
    </table>


FastCCD fCRIC and CIN Settings
------------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDMux1
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Set the output muxer on the CIN to monitor timing signals. Allowed values are:<br /> EXPOSE<br /> VCLK1<br /> VCLK2<br /> VCLK3<br /> ATG<br /> VFSCLK1<br /> VFSCLK2<br /> VFSCLK3<br /> HCLK1<br /> HCLK2<br /> OSW<br /> RST<br /> CONVERT<br /> SHUTTER<br /> SWTRIGGER<br /> TRIGMON
    </td>
    <td>
    FCCD_MUX1
    </td>
    <td>
    $(P)$(R)Mux1<br />$(P)$(R)Mux1_RBV
    </td>
    <td>
    mbbo<br />mbbi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDMux2
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Set the output muxer on the CIN to monitor timing signals. Allowed values are:<br /> EXPOSE<br /> VCLK1<br /> VCLK2<br /> VCLK3<br /> ATG<br /> VFSCLK1<br /> VFSCLK2<br /> VFSCLK3<br /> HCLK1<br /> HCLK2<br /> HCLK3<br /> OSW<br /> RST<br /> CONVERT<br /> SAVE<br /> HWTRIG
    </td>
    <td>
    FCCD_MUX2
    </td>
    <td>
    $(P)$(R)Mux2<br />$(P)$(R)Mux2_RBV
    </td>
    <td>
    mbbo<br />mbbi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFCRICGain
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the fCRIC gain. Allowed values are:<br /> Auto<br /> x2<br /> x1
    </td>
    <td>
    FCRIC_GAIN
    </td>
    <td>
    $(P)$(R)FCRICGain<br />$(P)$(R)FCRICGain_RBV
    </td>
    <td>
    mbbo<br />mbbi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFCRICClamp
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the fCRIC clamp mode. Allowed values are:<br /> CLAMP OFF<br /> CLAMP ON
    </td>
    <td>
    FCRIC_CLAMP
    </td>
    <td>
    $(P)$(R)FCRICClamp<br />$(P)$(R)FCRICClamp_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFOTest
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the fiberoptic module test mode. Allowed values are:<br /> Off<br /> On
    </td>
    <td>
    FO_TEST
    </td>
    <td>
    $(P)$(R)FOTest<br />$(P)$(R)FOTest_RBV
    </td>
    <td>
    bo<br />bi
    </td>
    </tr>
    </tbody>
    </table>
    
    
FastCCD CIN Setting Upload
--------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDFirmwarePath
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the path for the CIN firmware file to upload
    </td>
    <td>
    FIRMWARE_PATH
    </td>
    <td>
    $(P)$(R)FirmwarePath<br />$(P)$(R)FirmwarePath_RBV
    </td>
    <td>
    waveform
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFirmwareUpload
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Uploads the firmware to the CIN specified in the <em>FastCCDFirmwarePath</em>
    </td>
    <td>
    FIRMWARE_UPLOAD
    </td>
    <td>
    $(P)$(R)FwUpload<br />$(P)$(R)FwUpload_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDClockPath
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the path for the clocking (timing) file to upload
    </td>
    <td>
    CLOCK_PATH
    </td>
    <td>
    $(P)$(R)ClockPath<br />$(P)$(R)ClockPath_RBV
    </td>
    <td>
    waveform
    </td>
    </tr>
    <tr>
    <td>
    FastCCDClockUpload
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Uploads the clock (timing) to the CIN specified in the <em>FastCCDClockPath</em>
    </td>
    <td>
    CLOCK_UPLOAD
    </td>
    <td>
    $(P)$(R)ClockUpload<br />$(P)$(R)ClockUpload_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFCRICPath
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the path for the fCRIC configuration file to upload
    </td>
    <td>
    FCRIC_PATH
    </td>
    <td>
    $(P)$(R)FCRICPath<br />$(P)$(R)FCRICPath_RBV
    </td>
    <td>
    waveform
    </td>
    </tr>
    <tr>
    <td>
    FastCCDFCRICUpload
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Uploads the fCRIC configuration to the CIN specified in the <em>FastCCDFCRICPath</em>
    </td>
    <td>
    FCRIC_UPLOAD
    </td>
    <td>
    $(P)$(R)FCRICUpload<br />$(P)$(R)FCRICUpload_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDBiasPath
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    r/w
    </td>
    <td>
    Sets the path for the sensor bias configuration file to upload
    </td>
    <td>
    BIAS_PATH
    </td>
    <td>
    $(P)$(R)BiasPath<br />$(P)$(R)BiasPath_RBV
    </td>
    <td>
    waveform
    </td>
    </tr>
    <tr>
    <td>
    FastCCDBiasUpload
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Uploads the bias configuration to the CIN specified in the <em>FastCCDBiasPath</em>
    </td>
    <td>
    BIAS_UPLOAD
    </td>
    <td>
    $(P)$(R)BiasUpload<br />$(P)$(R)BiasUpload_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    </tbody>
    </table>

FastCCD Auto Setting Parameters
-------------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDTimingMode
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Set the timing mode to use for auto booting the FastCCD
    </td>
    <td>
    TIMING_MODE
    </td>
    <td>
    $(P)$(R)TimingMode<br />$(P)$(R)TimingMode_RBV
    </td>
    <td>
    mbbo<br />mbbi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode last set by the FastCCD auto boot sequence.
    </td>
    <td>
    TIMING_NAME
    </td>
    <td>
    $(P)$(R)TimingName_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName0
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 1
    </td>
    <td>
    TIMING_NAME_0
    </td>
    <td>
    $(P)$(R)TimingName1_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName1
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 2
    </td>
    <td>
    TIMING_NAME_1
    </td>
    <td>
    $(P)$(R)TimingName1_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName2
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 3
    </td>
    <td>
    TIMING_NAME_2
    </td>
    <td>
    $(P)$(R)TimingName1_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName3
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 4
    </td>
    <td>
    TIMING_NAME_3
    </td>
    <td>
    $(P)$(R)TimingName3_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName4
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 5
    </td>
    <td>
    TIMING_NAME_4
    </td>
    <td>
    $(P)$(R)TimingName4_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName5
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 6
    </td>
    <td>
    TIMING_NAME_5
    </td>
    <td>
    $(P)$(R)TimingName5_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName6
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 7
    </td>
    <td>
    TIMING_NAME_6
    </td>
    <td>
    $(P)$(R)TimingName6_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName7
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 8
    </td>
    <td>
    TIMING_NAME_7
    </td>
    <td>
    $(P)$(R)TimingName7_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName8
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 9
    </td>
    <td>
    TIMING_NAME_8
    </td>
    <td>
    $(P)$(R)TimingName8_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    <tr>
    <td>
    FastCCDTimingName9
    </td>
    <td>
    asynParamOctet
    </td>
    <td>
    ro
    </td>
    <td>
    The name of the timing mode assigned to mode number 10
    </td>
    <td>
    TIMING_NAME_9
    </td>
    <td>
    $(P)$(R)TimingName9_RBV
    </td>
    <td>
    stringin
    </td>
    </tr>
    </tbody>
    </table>


FastCCD Auto Setting Commands
-----------------------------

.. raw:: html

    <table class="table table-striped">
    <thead>
    <tr>
    <th>
    Parameter index variable
    </th>
    <th>
    asyn interface
    </th>
    <th>
    Access
    </th>
    <th>
    Description
    </th>
    <th>
    drvInfo string
    </th>
    <th>
    EPICS record name
    </th>
    <th>
    EPICS record type
    </th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td>
    FastCCDBoot
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Boot the CIN using the timing mode defined by <em>FastCCDTimingMode</em>. This causes the CIN to be reset (power cycled), the firmware to be uploaded and the timing to be uploaeded to the CIN.
    </td>
    <td>
    BOOT
    </td>
    <td>
    $(P)$(R)Boot<br />$(P)$(R)Boot_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDSendTiming
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Send the timing information to the CIN as set by <em>FastCCDTimingMode</em>. This causes the CIN timing to change.
    </td>
    <td>
    SEND_TIMING
    </td>
    <td>
    $(P)$(R)SendTiming<br />$(P)$(R)SendTiming_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDSendFCRIC
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Send the fCRIC configuration information to the CIN as set by <em>FastCCDTimingMode</em>.
    </td>
    <td>
    SEND_FCRIC
    </td>
    <td>
    $(P)$(R)SendFCRIC<br />$(P)$(R)SendFCRIC_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    <tr>
    <td>
    FastCCDSendBias
    </td>
    <td>
    asynParamInt32
    </td>
    <td>
    r/w
    </td>
    <td>
    Send the bias voltage configuration information to the CIN as set by <em>FastCCDTimingMode</em>.
    </td>
    <td>
    SEND_BIAS
    </td>
    <td>
    $(P)$(R)SendBias<br />$(P)$(R)SendBias_RBV
    </td>
    <td>
    busy<br />bi
    </td>
    </tr>
    </tbody>
    </table>
