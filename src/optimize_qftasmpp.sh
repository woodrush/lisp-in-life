#!/bin/bash
set -e

if [ "$#" -ne 2 ]; then
  echo "Usage: ./optimize_qftasmpp.sh [input file] [output_file]" >&2
  exit 1
fi

input_file=$1
output_file=$2
tmp_file=/tmp/tmp.qftasmpp

echo "Running qftasmpp compiler optimizations on ${input_file}..."

wc -l $input_file
cp $input_file $tmp_file
while true; do
  python ./src/qftasmopt.py $tmp_file > $output_file
  wc -l $output_file
  if [ $(diff $tmp_file $output_file | wc -l) -ne 0 ]; then
      mv $output_file $tmp_file
      continue
  else
      echo "Files ${tmp_file} and ${output_file} match."
      break
  fi
done

rm $tmp_file

echo "Done."
