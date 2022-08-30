#!/bin/tcsh

starver SL21d

root4star -l -b -q './StRoot/macros/loadSharedHFLibraries.C'

root4star -l -b -q './StRoot/macros/runPicoLambdaAnaMaker.C("./picoLists/local_testList.list","output_test",0,"./picoLists/BadRunList_MB.list","picoHFtree","/star/u/vanekjan/pwg/vanekjan/myLambdaAnalysis/pp510GeV_Run17/input_local/",0)'
