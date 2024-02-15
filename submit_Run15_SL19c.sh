#!/bin/bash

#max. line is 52694 in Run15 trans file list SL19c for st_ssdmb
#sed -n '1,52694 p' ./picoLists/picoList_Run15_ssdmb_trans.list > picoList_submit.list

#max. line is 17624 in Run15 long file list SL19c for st_ssdmb
#sed -n '1,17624 p' ./picoLists/picoList_Run15_ssdmb_long.list > picoList_submit.list

#max. line is 55808 in Run15 long2 file list SL19c for st_ssdmb, this is 92% of full available statistics
sed -n '1,55808 p' ./picoLists/picoList_Run15_ssdmb_long2_92_percent.list > picoList_submit.list

#max. line is 687 in Run15 long3 file list SL19c
#sed -n '1,687 p' ./picoLists/picoList_Run15_ssdmb_long3.list > picoList_submit.list

path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )

#trigger = 0 - MB
trigger=0

echo executing submitPicoHFMaker.csh f0r picoList_test.list inside $path

starver SL19c

#re-compile under correct library
./compile_SL19c.csh

#for list generated directly by sed ad the top of this macro
csh starSubmit/submitPicoHFMaker_Run15_SL19c.csh $path picoList_submit.list $trigger

