//===- HW1.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "HW1" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Constant.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"

#include <map>

using namespace llvm;

#define DEBUG_TYPE "hw1"

namespace {
    // HW1 - The first implementation, without getAnalysisUsage.
    struct HW1 : public ModulePass {
        static char ID; // Pass identification, replacement for typeid
        HW1() : ModulePass(ID) {}

        // Every store and load instruction will be represented as a unique index.
        // The first will be instruction 0, the second will be instruction 1, and so on.
        std::map<Instruction*, int> memoryIndexes;
        int memIdx = 0;

        bool runOnModule(Module &M) override
        {
            // Get the functions that print out for every load and store
            Function* loadprint = M.getFunction("loadprint");
            Function* storeprint = M.getFunction("storeprint");

            // If we can't find them, then exit early
            if(!loadprint || !storeprint)
            {
                errs() << "Function not found\n";
                return false;
            }

            // Fill out the map of memoryindexes
            for (Module::iterator f = M.begin(); f != M.end(); ++f)
            {
                Function* F = &(*f);
                // We don't process any memory operations in any print functions
                if (F->getName() == "loadprint" || F->getName() == "storeprint" || F->getName() == "printf") continue;

                for (Function::iterator b = F->begin(); b != F->end(); b++) {
                    BasicBlock *bb = &(*b);
                    for(BasicBlock::iterator i_iter = bb->begin(); i_iter != bb->end(); ++i_iter) {
                        Instruction *I = &(*i_iter);

                        if (I->getOpcode() == Instruction::Load)
                        {
                            memoryIndexes[I] = memIdx++;
                        }

                        if (I->getOpcode() == Instruction::Store)
                        {
                            memoryIndexes[I] = memIdx++;
                        }
                    }

                }
            }

            // Loop through the entire module, and insert prints before every load and store
            for (Module::iterator f = M.begin(); f != M.end(); ++f)
            {
                Function* F = &(*f);
                // Do not insert extra prints in from of print operations
                if (F->getName() == "loadprint" || F->getName() == "storeprint" || F->getName() == "printf") continue;

                for (Function::iterator b = F->begin(); b != F->end(); b++) {
                    BasicBlock *bb = &(*b);
                    for(BasicBlock::iterator i_iter = bb->begin(); i_iter != bb->end(); ++i_iter) {
                        Instruction *I = &(*i_iter);

                        if (I->getOpcode() == Instruction::Load)
                        {
                            errs() << "Load found\n";
                            
                            // Create a call to loadprint.
                            // The first agrument will be the id we assigned to this load.
                            // The second argument will be the memory address of the load.
                            std::vector<Value*> args;
                            ConstantInt* idx = ConstantInt::get(Type::getInt32Ty(M.getContext()), memoryIndexes.at(I));
                            args.push_back(idx);
                            args.push_back(I->getOperand(0));
                            
                            IRBuilder<> builder(I);
                            builder.CreateCall(loadprint, args);
                        }

                        if (I->getOpcode() == Instruction::Store)
                        {
                            errs() << "Store found\n";

                            // Create a call to storeprint.
                            // The first agrument will be the id we assigned to this store.
                            // The second argument will be the memory address of the store.
                            std::vector<Value*> args;
                            ConstantInt* idx = ConstantInt::get(Type::getInt32Ty(M.getContext()), memoryIndexes.at(I));
                            args.push_back(idx);
                            args.push_back(I->getOperand(1));
                            
                            IRBuilder<> builder(I);
                            builder.CreateCall(storeprint, args);
                        }
                    }

                }

                return false;
            }
        }

        void getAnalysisUsage(AnalysisUsage &AU) const {
            AU.addRequired<BranchProbabilityInfoWrapperPass>();
            AU.addRequired<BlockFrequencyInfoWrapperPass>();
        }
    };

}

char HW1::ID = 0;
static RegisterPass<HW1> X("hw1", "HW1 Pass");
