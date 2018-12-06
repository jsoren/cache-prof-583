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
#include "llvm/Analysis/LoopInfo.h"

#include <map>
#include <fstream>

using namespace llvm;

#define DEBUG_TYPE "hw1"

namespace {
    // HW1 - The first implementation, without getAnalysisUsage.
    struct HW1 : public ModulePass {
        static char ID; // Pass identification, replacement for typeid
        HW1() : ModulePass(ID) {}

        // Get unique indexes for every load and store. These should match the indexes from the profiler
        std::map<Instruction*, int> memoryIndexes;
        int memIdx = 0;

        // Get hit ratio for each mem access id
        std::map<int, double> hitratio;

        bool runOnModule(Module &M) override
        {
            // Read data from hitmiss file
            std::ifstream reader("hitmiss.txt");
            std::string tmp;
            int id, hits, misses;
            // Get rid of header
            reader >> tmp >> tmp >> tmp;
            while (reader >> id >> hits >> misses)
            {
                hitratio[id] = (double) hits / (double) (hits + misses);
            }
            reader.close();

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

                LoopInfo& LI = getAnalysis<LoopInfoWrapperPass>(*f).getLoopInfo();

                for (Loop* L : LI)
                {
                    for (BasicBlock* bb : L->getBlocks())
                    {
                        for(BasicBlock::iterator i_iter = bb->begin(); i_iter != bb->end(); ++i_iter) {
                            Instruction *I = &(*i_iter);

                            if (I->getOpcode() == Instruction::Load)
                            {
                                errs() << "Found Load\n";
                                int memID = memoryIndexes.at(I);
                                double ratio = hitratio.at(memID);
                                errs() << memID << " " << ratio << "\n";
                            }

                            if (I->getOpcode() == Instruction::Store)
                            {
                                errs() << "Found Store\n";
                                int memID = memoryIndexes.at(I);
                                double ratio = hitratio.at(memID);
                                errs() << memID << " " << ratio << "\n";
                            }
                        }
                    } 
                }

                
            }

            return false;
        }

        void getAnalysisUsage(AnalysisUsage &AU) const {
            AU.addRequired<BranchProbabilityInfoWrapperPass>();
            AU.addRequired<BlockFrequencyInfoWrapperPass>();
            AU.addRequired<LoopInfoWrapperPass>();
        }
    };

}

char HW1::ID = 0;
static RegisterPass<HW1> X("hw1", "HW1 Pass");
