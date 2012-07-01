//===- Transforms/Instrumentation.h - Instrumentation passes ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines constructor functions for instrumentation passes.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_INSTRUMENTATION_H
#define LLVM_TRANSFORMS_INSTRUMENTATION_H
//---OPENCOR--- BEGIN
#include "llvmglobal.h"
//---OPENCOR--- END

namespace llvm {

class ModulePass;
class FunctionPass;

// Insert edge profiling instrumentation
ModulePass *createEdgeProfilerPass();

// Insert optimal edge profiling instrumentation
ModulePass *createOptimalEdgeProfilerPass();

// Insert path profiling instrumentation
ModulePass *createPathProfilerPass();

// Insert GCOV profiling instrumentation
/*---OPENCOR---
ModulePass *createGCOVProfilerPass(bool EmitNotes = true, bool EmitData = true,
*/
//---OPENCOR--- BEGIN
ModulePass LLVM_EXPORT *createGCOVProfilerPass(bool EmitNotes = true, bool EmitData = true,
//---OPENCOR--- END
                                   bool Use402Format = false,
                                   bool UseExtraChecksum = false);

// Insert AddressSanitizer (address sanity checking) instrumentation
/*---OPENCOR---
ModulePass *createAddressSanitizerPass();
*/
//---OPENCOR--- BEGIN
ModulePass LLVM_EXPORT *createAddressSanitizerPass();
//---OPENCOR--- END
// Insert ThreadSanitizer (race detection) instrumentation
/*---OPENCOR---
FunctionPass *createThreadSanitizerPass();
*/
//---OPENCOR--- BEGIN
FunctionPass LLVM_EXPORT *createThreadSanitizerPass();
//---OPENCOR--- END

} // End llvm namespace

#endif
