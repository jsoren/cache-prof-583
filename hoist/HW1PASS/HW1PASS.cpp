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

        bool runOnModule(Module &M) override
        {
            // Get uniwue indexes for every load and store. These should match the indexes from the profiler
            std::map<Instruction*, int> memoryIndexes;
            int memIdx = 0;

            for (Module::iterator f = M.begin(); f != M.end(); ++f)
            {
                Function* F = &(*f);
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

            for (Module::iterator f = M.begin(); f != M.end(); ++f)
            {
                Function* F = &(*f);
                if (F->getName() == "loadprint" || F->getName() == "storeprint" || F->getName() == "printf") continue;

                for (Function::iterator b = F->begin(); b != F->end(); b++) {
                    BasicBlock *bb = &(*b);
                    for(BasicBlock::iterator i_iter = bb->begin(); i_iter != bb->end(); ++i_iter) {
                        Instruction *I = &(*i_iter);

                        // Hoist? do stuff?

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
