lisp_src=$1
memheader_eir=$2
QFTASM_RAMSTDIN_BUF_STARTPOSITION=$3
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=$4
outfile=$5

tmp_eir=./build/tmp.eir
tmp2_eir=./build/tmp2.eir

../elvm/out/8cc -S -DQFT -Dprecalculation_run -Isrc -o $tmp2_eir $lisp_src

cat $memheader_eir > $tmp_eir
echo "" >> $tmp_eir
cat $tmp2_eir >> $tmp_eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  $tmp_eir > $outfile
