#!/bin/bash

date=$(date +%m-%d_%H-%M-%S)
file_full="./data/page_owner_full_$date";
file_sort="./data/page_owner_sort_$date";
file_res="./data/page_owner_$date";

touch $file_full
touch $file_sort
touch $file_res

get_po=`sudo cat /sys/kernel/debug/page_owner > $file_full`
grep_po=`grep -v ^PFN $file_full > $file_sort`
sort_po=`page_owner_sort $file_sort $file_res`

echo $get_po
echo $grep_po
echo $sort_po
