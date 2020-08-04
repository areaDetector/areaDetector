## Modules needed by IOCs

# AUTOSAVE provides save_restore
@AUTOSAVE@

# BUSY provides the busy record
@BUSY@

# CALC provides the sseq record for the reset in NDStats.template
# and which is loaded into the example IOCs for general purpose use.
# CALC also provides the scalcout record for SSCAN
@CALC@

# SNCSEQ is needed if CALC was built with SNCSEQ support (see CALC/configure/RELEASE)
@SNCSEQ@

# SSCAN is needed for scanning threshold on Pilatus and general scanning.
# SSCAN also provides support for the swait record in CALC
@SSCAN@

## Optional modules

# DEVIOCSTATS is used to monitor IOC resources
@DEVIOCSTATS@

# ALIVE can send IOC status data to a central server
@ALIVE@

# ADPLUGINEDGE is an optional plugin
@ADPLUGINEDGE@

