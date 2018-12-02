opt -load HW1PASS/LLVMHW1PASS.so -hw1 main.bc > new.bc
llvm-dis new.bc -o test.ll
lli new.bc > prof.txt

