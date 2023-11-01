#!/bin/bash

#max. line is 22173 in file list SL21d
#max. line is 444695 in file list SL20d
#max. line is 599253 in file list SL22b
#max. line is 651714 in file list SL22b new
#to test submit

sed -n '100000,101000 p' ./picoLists/picoList_Run17_SL22b_new.list > picoList_submit.list

#sort ./picoLists/sublists/picoList_test.list > ./picoLists/sublists/picoList_test_09.list

#rm ./picoLists/sublists/picoList_test.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )

#trigger = 0 - MB
trigger=0

echo executing submitPicoHFMaker.csh f0r picoList_test.list inside $path

starver SL22b

#re-compile under correct library
./compile_SL22b.csh

#for list generated directly by sed ad the top of this macro
csh starSubmit/submitPicoHFMaker_Run17_SL22b.csh $path picoList_submit.list $trigger

#for pre-generated sublists
#csh starSubmit/submitPicoHFMaker.csh $path ./picoLists/sublists/picoList_test_08.list
