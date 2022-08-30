#!/bin/bash

#max. line is 22173 in current version of file list

sed -n '1,5000 p' ./picoLists/picoList_Run17.list > picoList_submit.list

#sort ./picoLists/sublists/picoList_test.list > ./picoLists/sublists/picoList_test_09.list

#rm ./picoLists/sublists/picoList_test.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )


echo executing submitPicoHFMaker.csh f0r picoList_test.list inside $path

starver SL21d

#for list generated directly by sed ad the top of this macro
csh starSubmit/submitPicoHFMaker.csh $path picoList_submit.list

#for pre-generated sublists
#csh starSubmit/submitPicoHFMaker.csh $path ./picoLists/sublists/picoList_test_08.list
