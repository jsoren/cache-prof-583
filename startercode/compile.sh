clang -emit-llvm -c main.c -o main.bc
llvm-dis main.bc -o main.ll

