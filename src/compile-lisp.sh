../elvm/out/8cc -S -DELVM -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir $1
echo "" > tmp.eir
cat ./src/memheader.eir >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing