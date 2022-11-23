#!/bin/bash

#max. line is 22173 in file list SL21d
#max. line is 444695 in file list SL20d
#max. line is 599253 in file list SL22b
#to test submit

sed -n '100000,100100 p' ./picoLists/picoList_Run17_SL22b.list > picoList_submit.list

#sort ./picoLists/sublists/picoList_test.list > ./picoLists/sublists/picoList_test_09.list

#rm ./picoLists/sublists/picoList_test.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )

#trigger = 1 - JP
trigger=1


echo executing submitPicoHFMaker_Run17_SL22b.csh f0r picoList_submit.list inside $path

starver SL22b

#re-compile under correct library
./compile_SL22b.csh

#for list generated directly by sed ad the top of this macro
#last argument (1) is to set trigger to JP
csh starSubmit/submitPicoHFMaker_Run17_SL22b.csh $path picoList_submit.list $trigger

#for pre-generated sublists
#csh starSubmit/submitPicoHFMaker.csh $path ./picoLists/sublists/picoList_test_08.list
