#!/bin/tcsh

#starver SL21d
starver SL20c

# -- set maker mode
#    0 - kAnalyze, 
#    1 - kWrite
#    2 - kRead
#    3 - kQA
set makerMode=0

./compile_SL20c.csh

root4star -l -b -q './StRoot/macros/loadSharedHFLibraries.C'

root4star -l -b -q './StRoot/macros/runPicoLambdaAnaMaker.C("./picoLists/local_testList_SL20c.list","output_test",'$makerMode',"./picoLists/BadRunList_MB.list","picoHFtree","/star/u/vanekjan/pwg/vanekjan/myLambdaAnalysis/pp510GeV_Run17/input_local/SL20c/",0, "SL20c")'
