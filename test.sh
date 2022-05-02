#!/bin/bash
function get_char()
{
  SAVEDSTTY=`stty -g`
  stty -echo
  stty cbreak
  dd if=/dev/tty bs=1 count=1 2> /dev/null
  stty -raw
  stty echo
  stty $SAVEDSTTY
}

enable_pause=1

function pause()
{
  if [ "x$1" != "x" ]; then
    echo $1
  fi
  if [ $enable_pause -eq 1 ]; then
    echo "Press any key to continue!"
    char=`get_char`
  fi
}

#获取文件夹下所有文件
#
unit_test="./test"
test_check="./test_check.py"
runner="/home/os/Documents/lang-exer/LightC/cmake-build-debug/LightC"

files=$(ls $unit_test)
for sfile in ${files}
do
    echo "start test:" $unit_test/$sfile
    output_file=$(mktemp)
    $runner $unit_test/$sfile ./output > $output_file 2>&1
    ans=$?
    #echo $ans
    output=$(< $output_file)
    rm -f $output_file
    python3 $test_check $sfile $ans "$output"
    ans=$?
    if [ $ans -eq 0 ] ; then
      echo "pass test"
    else
      echo "failed test:"
      echo $output
      if [ $ans -eq 1 ] ; then
        echo "input file's name invalid"
      elif [ $ans -eq 2 ] ; then
        echo "err code not match"
      elif [ $ans -eq 3 ] ; then
        echo "err line not match"
      fi
      pause
    fi


done
echo "finish all tests"