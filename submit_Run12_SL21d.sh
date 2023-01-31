#!/bin/bash

#max. line is 70025 in Run12 file list SL21d

sed -n '50001,70025 p' ./picoLists/picoList_Run12_SL21d.list > picoList_submit.list

#sort ./picoLists/sublists/picoList_test.list > ./picoLists/sublists/picoList_test_09.list

#rm ./picoLists/sublists/picoList_test.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )

#trigger = 0 - MB
trigger=0

echo executing submitPicoHFMaker.csh f0r picoList_test.list inside $path

starver SL21d

#re-compile under correct library
./compile_SL21d.csh

#for list generated directly by sed ad the top of this macro
csh starSubmit/submitPicoHFMaker_Run12_SL21d.csh $path picoList_submit.list $trigger

#for pre-generated sublists
#csh starSubmit/submitPicoHFMaker.csh $path ./picoLists/sublists/picoList_test_08.list
