opt -mem2reg -simplifycfg -loops -lcssa -loop-simplify -loop-rotate -loop-unroll -unroll-count=2 -unroll-allow-partial main.bc -o unroll.bc
llvm-dis unroll.bc -o unroll.ll

