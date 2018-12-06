opt -load HW1PASS/LLVMHW1PASS.so -hw1 unroll.bc > print.bc
llvm-dis print.bc -o test.ll
lli print.bc > prof.txt
