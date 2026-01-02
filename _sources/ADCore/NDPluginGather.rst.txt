NDPluginGather
==============
:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

This plugin is used to gather NDArrays from multiple upstream plugins
and merge them into a single stream. When used together with
:doc:`NDPluginScatter` it allows multiple intances
of a plugin to process NDArrays in parallel, utilizing multiple cores to
increase throughput.

This plugin works differently from other plugins that receive callbacks
from upstream plugins. Other plugins subscribe to NDArray callbacks from
a single upstream plugin or driver. NDPluginGather allows subscribing to
callbacks from any number of upstream plugins. It combines the NDArrays
it receives into a single stream which it passes to all downstream
plugins. The example commonPlugins.cmd and medm files in ADCore allow up
to 8 upstream plugins, but this number can easily be changed by editing
the startup script and operator display file.

NDPluginGather inherits from NDPluginDriver. NDPluginGather does not do
any modification to the NDArrays that it receives except for possibly
adding new NDAttributes if an attribute file is specified. The
`NDPluginGather class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_gather.html>`__
describes this class in detail.

NDPluginGather.h defines the following parameters. It also implements
all of the standard plugin parameters from
:doc:`NDPluginDriver`. It extends the
standard NDPluginDriverArrayPort and NDPluginDriverArrayAddr parameters
by supporting more than one asyn address field for each, i.e. there can
be multiple NDArrayPort and NDArrayAddr records, each specifying a
different upstream plugin. There are 2 EPICS databases for the
NDPluginGather plugin. NDGather.template provides access to global
parameters that are not specific to each input source. There are
currently no such global parameters, so NDGather.template does not
define any new records. NDGatherN.template provides access to the
parameters for each individual NDArray input source. Note that to reduce
the width of this table the parameter index variable names have been
split into 2 lines, but these are just a single name, for example
``NDPluginGatherSortMode``.

.. |br| raw:: html

    <br>

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * - 
    - Parameter Definitions in NDPluginDriver.h and EPICS Record
      Definitions in NDPluginBase.template and NDGatherN.template
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginDriver |br|
      ArrayPort
    - asynOctet
    - r/w
    - asyn port name for NDArray driver that will make callbacks to this plugin. This
      port can be changed at run time, connecting the plugin to a different NDArray driver.
      There can be more than one such input port. The maximum is number is specified in
      the NDGatherConfigure command in the startup script.
    - NDARRAY_PORT
    - $(P)$(R)NDArrayPort_$(N), (P)$(R)NDArrayPort_$(N)_RBV
    - stringout, stringin
  * - NDPluginDriver |br|
      ArrayAddr
    - asynInt32
    - r/w
    - asyn port address for NDArray driver that will make callbacks to this plugin. This
      address can be changed at run time, connecting the plugin to a different address
      in the NDArray driver. There can be more than one such input port. The maximum is
      number is specified in the NDGatherConfigure command in the startup script.
    - NDARRAY_ADDR
    - $(P)$(R)NDArrayAddress_$(N), $(P)$(R)NDArrayAddress_$(N)_RBV
    - longout, longin

Configuration
-------------

The NDPluginGather plugin is created with the ``NDGatherConfigure`` command,
either from C/C++ or from the EPICS IOC shell.

::

   NDGatherConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                      maxPorts, size_t maxMemory, 
                      int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDGatherConfigure function in the
`NDPluginGather.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_gather_8cpp.html>`__
and in the documentation for the constructor for the `NDPluginGather
class <../areaDetectorDoxygenHTML/class_n_d_plugin_gather.html>`__.

Screen shots
------------

The following is the MEDM screen that provides control of the
NDPluginGather plugin.

.. image:: NDGather.png
    :align: center

Detailed example
----------------

The following is a detailed example of using the NDPluginScatter and
NDPluginGather plugins. In this example the simDetector is generating
1024x1024 Float32 images at about 535 frames/s. The simDetector output
goes to the NDPluginScatter plugin.

There are 5 NDPluginStats statistics plugins that all receive NDArrays
from the NDPluginScatter plugin. Each statistics plugin can only process
about 115 frames/s before it uses 100% of the CPU time on its core. Thus
in order to be able to generate statistics on all 535 frames/s it is
necessary to run 5 statistics plugins in parallel.

The NDPluginGather plugin is configured to get its input arrays from the
5 statistics plugins. It is thus receiving about 535 frames/s. For this
test the NDPluginGather plugin was run in both Sorted and Unsorted
modes. The SortTime was set to 0.1 second which was found to be long
enough to ensure that all of the arrays would arrive in time to be
correctly sorted. There should be 54 frames arriving in the 0.1 second
time interval between when the NDPluginGather plugin processes. The
SortSize was set to 100 to provide a safety margin that prevented
dropped output arrays.

The output of the NDPluginGather plugin was sent to the NDFileNetCDF
plugin which saved images to disk. For the files tests the simDetector
was set to ImageMode=Multiple with NumImages=1000 and the NDFileNetCDF
plugin was set to StreamMode with NumCapture=1000. Two files were saved,
one with NDPluginGather set to Unsorted and the other with
NDPluginGather set to Sorted. The files were read into IDL and the value
of UniqueId for each array was printed to test that the sorting worked
correctly.

The following show the configuration of the simDetector driver. It is
generating 1024x1024 Float32 frames at about 535 frames/s, which is over
2 GB/s. It is running in LinearRamp mode.

.. image:: scatterGatherExample_simDetector.png
    :align: center


.. image:: scatterGatherExample_simDetectorSetup.png
    :align: center


The following shows the setup of all of the plugins. Note that the
NDScatter plugin is running at the full frame rate of about 535
frames/s, while each of the 5 statistics plugins is running at 1/5 of
this speed, about 107 frames/s.

.. image:: scatterGatherExample_commonPlugins.png
    :align: center


The following shows the setup of the NDPluginScatter plugin.

.. image:: scatterGatherExample_NDScatter.png
    :align: center


The following shows the setup of the first NDPluginStats plugin,
performing all statistics calculations. All statistics plugins were
configured identically.

.. image:: scatterGatherExample_NDStats.png
    :align: center


The following shows the setup of the NDPluginGather plugin. Note that it
is getting its input from the 5 statistics plugins and is receiving
frames at about 535 frames/s.

.. image:: scatterGatherExample_NDGather.png
    :align: center


The following shows the full setup of the NDPluginGather plugin. It has
a SortTime of 0.1 seconds, a SortSize of 200, and SortMode is set to
Sorted.

.. image:: scatterGatherExample_NDGatherFull.png
    :align: center


The following shows the output of the Linux "top" command when the IOC
was running as shown above. The top program was running with the -H
option which displays the statistics for each thread, sorted by top CPU
usage. Note that each of the 5 statistics plugins is using about 98% of
a core. The simDetector, NDPluginGather, and NDPluginScatter are each
using about 55% of a core. The total CPU usage on the machine is 40%,
and this is a 20-core machine, so there about 8 cores being fully
utilized.

.. image:: scatterGatherExample_top_threads.png
    :align: center


The following shows the setup of the NDFileNetCDF plugin. It is getting
its input from the NDPluginGather plugin and is running in Stream mode,
saving 1000 arrays. Note that it is only able to save about 116
frames/s, while it is receiving about 535 frames/s, so it needs a large
input queue to avoid losing frames. The input queue is currently set to
1000 frames, which is large enough to save 1000 frames without dropping
any.

.. image:: scatterGatherExample_NDFileNetCDF.png
    :align: center


The following shows the output when reading the netCDF file that was
written when NDPluginGather was set to ``SortMode=Unsorted``.

``attr[0].pvalue`` is the value of the UniqueId attribute for all 1000
NDArrays. Note that the arrays are not in the correct ``UniqueId`` order.

::

   IDL> t = read_nd_netcdf('gather_test_sorted_001.nc', attr=attr)
   IDL> u=*attr[0].pvalue
   IDL> print, u
         155438      155440      155439      155441      155442      155445      155443      155446      155447      155444
         155450      155451      155452      155449      155448      155455      155456      155457      155454      155460
         155461      155453      155462      155465      155459      155466      155470      155467      155458      155471
         155464      155475      155472      155476      155469      155463      155480      155477      155481      155474
         155485      155468      155482      155486      155490      155479      155473      155487      155495      155491
         155484      155478      155500      155492      155489      155496      155483      155505      155497      155494
         155488      155501      155510      155502      155499      155493      155506      155515      155507      155504
         155498      155511      155512      155509      155520      155516      155503      155517      155514      155521
         155508      155525      155522      155519      155526      155513      155527      155524      155530      155531
         155518      155532      155529      155536      155535      155523      155537      155534      155541      155542
         155540      155528      155539      155546      155547      155545      155533      155544      155551      155552
         155550      155549      155556      155538      155555      155557      155554      155561      155560      155562
         155543      155559      155566      155565      155567      155571      155548      155570      155572      155564
         155576      155575      155577      155553      155581      155569      155580      155582      155558      155586
         155585      155587      155574      155563      155591      155590      155592      155579      155596      155568
         155595      155597      155601      155600      155602      155584      155573      155606      155607      155605
         155611      155578      155589      155612      155610      155616      155617      155583      155594      155615
         155621      155622      155588      155620      155626      155627      155599      155593      155625      155631
         155632      155598      155604      155636      155630      155603      155637      155641      155635      155609
         155608      155642      155646      155640      155647      155613      155651      155614      155645      155652
         155618      155656      155650      155657      155623      155661      155619      155662      155655      155628
         155666      155667      155624      155633      155660      155671      155672      155638      155665      155676
         155629      155677      155643      155670      155681      155682      155648      155634      155686      155675
         155687      155653      155691      155639      155680      155658      155692      155696      155663      155685
         155697      155644      155668      155701      155702      155690      155673      155649      155706      155707
         155695      155678      155711      155712      155654      155700      155683      155716      155717      155688
         155705      155659      155721      155722      155693      155710      155726      155727      155698      155664
         155715      155731      155732      155703      155720      155669      155708      155736      155737      155725
         155713      155741      155674      155742      155718      155730      155746      155747      155723      155679
         155735      155751      155752      155728      155756      155740      155757      155684      155733      155761
         155762      155745      155738      155689      155766      155767      155743      155750      155694      155771
         155772      155748      155755      155776      155699      155777      155753      155760      155781      155704
         155782      155758      155765      155786      155787      155709      155763      155770      155791      155792
         155714      155768      155775      155796      155797      155719      155773      155780      155801      155724
         155802      155778      155785      155806      155729      155783      155807      155790      155811      155734
         155788      155795      155812      155816      155739      155793      155800      155817      155744      155798
         155821      155805      155749      155822      155803      155826      155810      155754      155827      155831
         155808      155815      155832      155759      155836      155820      155837      155813      155825      155841
         155764      155842      155830      155818      155846      155847      155769      155835      155851      155852
         155774      155823      155840      155856      155857      155779      155845      155861      155828      155862
         155784      155850      155866      155867      155833      155789      155855      155871      155872      155794
         155860      155838      155876      155877      155799      155865      155881      155882      155804      155843
         155870      155886      155809      155887      155875      155848      155814      155891      155892      155880
         155819      155896      155853      155885      155897      155824      155901      155890      155902      155829
         155858      155906      155895      155907      155834      155863      155900      155911      155912      155839
         155905      155916      155844      155868      155917      155910      155849      155921      155922      155915
         155873      155854      155927      155926      155920      155932      155859      155931      155878      155925
         155937      155936      155864      155930      155942      155883      155941      155869      155935      155947
         155946      155874      155888      155940      155952      155951      155879      155957      155945      155956
         155893      155884      155962      155950      155961      155889      155898      155967      155955      155966
         155894      155972      155960      155971      155899      155903      155977      155965      155976      155904
         155982      155908      155981      155970      155909      155987      155986      155975      155914      155913
         155992      155991      155980      155919      155997      155996      155918      155985      155924      156002
         156001      155923      155990      155929      156007      156006      155928      155934      156012      156011
         155995      155933      155939      156017      156016      156000      155944      155938      156022      156021
         156005      155949      156027      155943      156026      156010      156032      155954      156031      155948
         156015      156037      155959      156036      156020      155953      156042      155964      156041      156025
         155958      156047      155969      156046      156030      155963      155974      156052      156051      156035
         155968      155979      156057      156056      156040      155984      155973      156062      156061      156045
         155989      155978      156067      156066      156050      155994      155983      156072      156071      156055
         155999      155988      156077      156076      156060      156004      155993      156082      156081      156065
         156009      155998      156086      156087      156070      156014      156003      156092      156091      156019
         156075      156097      156008      156096      156024      156080      156102      156013      156029      156101
         156085      156107      156034      156018      156106      156090      156112      156023      156111      156039
         156095      156117      156028      156116      156044      156122      156100      156033      156121      156049
         156127      156105      156126      156038      156132      156054      156110      156131      156137      156043
         156115      156136      156059      156142      156120      156048      156141      156147      156064      156125
         156146      156152      156053      156130      156151      156069      156157      156058      156135      156156
         156162      156074      156140      156161      156167      156063      156145      156166      156079      156172
         156068      156171      156150      156177      156084      156176      156155      156073      156182      156181
         156089      156160      156187      156186      156078      156165      156094      156192      156191      156083
         156170      156197      156196      156099      156175      156202      156088      156201      156104      156180
         156207      156206      156093      156185      156109      156212      156211      156190      156098      156114
         156217      156216      156195      156103      156222      156221      156119      156200      156227      156108
         156226      156205      156124      156232      156231      156113      156210      156237      156236      156129
         156215      156118      156242      156241      156220      156134      156246      156247      156123      156225
         156251      156139      156252      156230      156128      156256      156257      156144      156261      156235
         156133      156262      156266      156240      156149      156267      156271      156138      156245      156154
         156272      156276      156250      156143      156277      156281      156159      156255      156148      156282
         156286      156260      156164      156287      156291      156153      156265      156292      156296      156169
         156270      156158      156297      156301      156275      156174      156302      156306      156163      156307
         156280      156311      156179      156168      156312      156285      156316      156317      156184      156290
         156321      156173      156322      156295      156189      156327      156326      156178      156300      156194
         156332      156331      156305      156183      156337      156336      156310      156199      156188      156341
         156342      156315      156204      156346      156193      156347      156320      156351      156352      156209
         156325      156198      156356      156357      156330      156361      156214      156362      156203      156335
         156366      156367      156340      156219      156208      156371      156372      156345      156224      156213
         156377      156376      156350      156218      156382      156229      156355      156381      156223      156387
         156234      156360      156386      156228      156392      156365      156239      156391      156233      156397
         156370      156244      156396      156238      156402      156375      156249      156401      156243      156407
         156380      156254      156406      156248      156412      156385      156259      156411      156253      156417
         156390      156264      156416      156258      156422      156395      156269      156421      156263      156427
         156400      156274      156432      156268      156426      156405      156279      156437      156273      156431
         156410      156284      156278      156436      156415      156289      156283      156420      156294      156288
         156425      156299      156293      156304      156430      156298      156309      156435      156303      156314
         156308      156319      156313      156324      156318      156329      156323      156334      156328      156339
         156333      156344      156338      156349      156343      156354      156348      156359      156353      156358
         156364      156363      156369      156368      156374      156373      156379      156378      156384      156383
         156389      156388      156394      156393      156398      156399      156403      156404      156408      156409
         156413      156414      156418      156419      156423      156424      156428      156429      156433      156434

The following shows the output when reading the netCDF file that was
written when NDPluginGather was set to ``SortMode=Sorted``.

``attr[0].pvalue`` is the value of the UniqueId attribute for all 1000
NDArrays. Note that the arrays are now in the correct ``UniqueId`` order.

::

   IDL> t = read_nd_netcdf('gather_test_sorted_001.nc', attr=attr) 
   IDL> u=*attr[0].pvalue
   IDL> print, u 
         157438      157439      157440      157441      157442      157443      157444      157445      157446      157447
         157448      157449      157450      157451      157452      157453      157454      157455      157456      157457
         157458      157459      157460      157461      157462      157463      157464      157465      157466      157467
         157468      157469      157470      157471      157472      157473      157474      157475      157476      157477
         157478      157479      157480      157481      157482      157483      157484      157485      157486      157487
         157488      157489      157490      157491      157492      157493      157494      157495      157496      157497
         157498      157499      157500      157501      157502      157503      157504      157505      157506      157507
         157508      157509      157510      157511      157512      157513      157514      157515      157516      157517
         157518      157519      157520      157521      157522      157523      157524      157525      157526      157527
         157528      157529      157530      157531      157532      157533      157534      157535      157536      157537
         157538      157539      157540      157541      157542      157543      157544      157545      157546      157547
         157548      157549      157550      157551      157552      157553      157554      157555      157556      157557
         157558      157559      157560      157561      157562      157563      157564      157565      157566      157567
         157568      157569      157570      157571      157572      157573      157574      157575      157576      157577
         157578      157579      157580      157581      157582      157583      157584      157585      157586      157587
         157588      157589      157590      157591      157592      157593      157594      157595      157596      157597
         157598      157599      157600      157601      157602      157603      157604      157605      157606      157607
         157608      157609      157610      157611      157612      157613      157614      157615      157616      157617
         157618      157619      157620      157621      157622      157623      157624      157625      157626      157627
         157628      157629      157630      157631      157632      157633      157634      157635      157636      157637
         157638      157639      157640      157641      157642      157643      157644      157645      157646      157647
         157648      157649      157650      157651      157652      157653      157654      157655      157656      157657
         157658      157659      157660      157661      157662      157663      157664      157665      157666      157667
         157668      157669      157670      157671      157672      157673      157674      157675      157676      157677
         157678      157679      157680      157681      157682      157683      157684      157685      157686      157687
         157688      157689      157690      157691      157692      157693      157694      157695      157696      157697
         157698      157699      157700      157701      157702      157703      157704      157705      157706      157707
         157708      157709      157710      157711      157712      157713      157714      157715      157716      157717
         157718      157719      157720      157721      157722      157723      157724      157725      157726      157727
         157728      157729      157730      157731      157732      157733      157734      157735      157736      157737
         157738      157739      157740      157741      157742      157743      157744      157745      157746      157747
         157748      157749      157750      157751      157752      157753      157754      157755      157756      157757
         157758      157759      157760      157761      157762      157763      157764      157765      157766      157767
         157768      157769      157770      157771      157772      157773      157774      157775      157776      157777
         157778      157779      157780      157781      157782      157783      157784      157785      157786      157787
         157788      157789      157790      157791      157792      157793      157794      157795      157796      157797
         157798      157799      157800      157801      157802      157803      157804      157805      157806      157807
         157808      157809      157810      157811      157812      157813      157814      157815      157816      157817
         157818      157819      157820      157821      157822      157823      157824      157825      157826      157827
         157828      157829      157830      157831      157832      157833      157834      157835      157836      157837
         157838      157839      157840      157841      157842      157843      157844      157845      157846      157847
         157848      157849      157850      157851      157852      157853      157854      157855      157856      157857
         157858      157859      157860      157861      157862      157863      157864      157865      157866      157867
         157868      157869      157870      157871      157872      157873      157874      157875      157876      157877
         157878      157879      157880      157881      157882      157883      157884      157885      157886      157887
         157888      157889      157890      157891      157892      157893      157894      157895      157896      157897
         157898      157899      157900      157901      157902      157903      157904      157905      157906      157907
         157908      157909      157910      157911      157912      157913      157914      157915      157916      157917
         157918      157919      157920      157921      157922      157923      157924      157925      157926      157927
         157928      157929      157930      157931      157932      157933      157934      157935      157936      157937
         157938      157939      157940      157941      157942      157943      157944      157945      157946      157947
         157948      157949      157950      157951      157952      157953      157954      157955      157956      157957
         157958      157959      157960      157961      157962      157963      157964      157965      157966      157967
         157968      157969      157970      157971      157972      157973      157974      157975      157976      157977
         157978      157979      157980      157981      157982      157983      157984      157985      157986      157987
         157988      157989      157990      157991      157992      157993      157994      157995      157996      157997
         157998      157999      158000      158001      158002      158003      158004      158005      158006      158007
         158008      158009      158010      158011      158012      158013      158014      158015      158016      158017
         158018      158019      158020      158021      158022      158023      158024      158025      158026      158027
         158028      158029      158030      158031      158032      158033      158034      158035      158036      158037
         158038      158039      158040      158041      158042      158043      158044      158045      158046      158047
         158048      158049      158050      158051      158052      158053      158054      158055      158056      158057
         158058      158059      158060      158061      158062      158063      158064      158065      158066      158067
         158068      158069      158070      158071      158072      158073      158074      158075      158076      158077
         158078      158079      158080      158081      158082      158083      158084      158085      158086      158087
         158088      158089      158090      158091      158092      158093      158094      158095      158096      158097
         158098      158099      158100      158101      158102      158103      158104      158105      158106      158107
         158108      158109      158110      158111      158112      158113      158114      158115      158116      158117
         158118      158119      158120      158121      158122      158123      158124      158125      158126      158127
         158128      158129      158130      158131      158132      158133      158134      158135      158136      158137
         158138      158139      158140      158141      158142      158143      158144      158145      158146      158147
         158148      158149      158150      158151      158152      158153      158154      158155      158156      158157
         158158      158159      158160      158161      158162      158163      158164      158165      158166      158167
         158168      158169      158170      158171      158172      158173      158174      158175      158176      158177
         158178      158179      158180      158181      158182      158183      158184      158185      158186      158187
         158188      158189      158190      158191      158192      158193      158194      158195      158196      158197
         158198      158199      158200      158201      158202      158203      158204      158205      158206      158207
         158208      158209      158210      158211      158212      158213      158214      158215      158216      158217
         158218      158219      158220      158221      158222      158223      158224      158225      158226      158227
         158228      158229      158230      158231      158232      158233      158234      158235      158236      158237
         158238      158239      158240      158241      158242      158243      158244      158245      158246      158247
         158248      158249      158250      158251      158252      158253      158254      158255      158256      158257
         158258      158259      158260      158261      158262      158263      158264      158265      158266      158267
         158268      158269      158270      158271      158272      158273      158274      158275      158276      158277
         158278      158279      158280      158281      158282      158283      158284      158285      158286      158287
         158288      158289      158290      158291      158292      158293      158294      158295      158296      158297
         158298      158299      158300      158301      158302      158303      158304      158305      158306      158307
         158308      158309      158310      158311      158312      158313      158314      158315      158316      158317
         158318      158319      158320      158321      158322      158323      158324      158325      158326      158327
         158328      158329      158330      158331      158332      158333      158334      158335      158336      158337
         158338      158339      158340      158341      158342      158343      158344      158345      158346      158347
         158348      158349      158350      158351      158352      158353      158354      158355      158356      158357
         158358      158359      158360      158361      158362      158363      158364      158365      158366      158367
         158368      158369      158370      158371      158372      158373      158374      158375      158376      158377
         158378      158379      158380      158381      158382      158383      158384      158385      158386      158387
         158388      158389      158390      158391      158392      158393      158394      158395      158396      158397
         158398      158399      158400      158401      158402      158403      158404      158405      158406      158407
         158408      158409      158410      158411      158412      158413      158414      158415      158416      158417
         158418      158419      158420      158421      158422      158423      158424      158425      158426      158427
         158428      158429      158430      158431      158432      158433      158434      158435      158436      158437

