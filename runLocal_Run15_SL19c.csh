#!/bin/tcsh

starver SL19c

./compile_SL19c.csh

root4star -l -b -q './StRoot/macros/loadSharedHFLibraries.C'

root4star -l -b -q './StRoot/macros/runPicoLambdaAnaMaker.C("./picoLists/local_testList_SL19c.list","output_test",0,"./picoLists/BadRunList_MB.list","picoHFtree","/star/u/vanekjan/pwg/vanekjan/myLambdaAnalysis/pp/input_local/SL19c/",0,"SL19c",0)'
