clang -emit-llvm -Xclang -disable-O0-optnone -c main.c -o main.bc
llvm-dis main.bc -o main.ll

