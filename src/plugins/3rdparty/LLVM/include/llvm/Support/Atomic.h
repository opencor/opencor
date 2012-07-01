//===- llvm/Support/Atomic.h - Atomic Operations -----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the llvm::sys atomic operations.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SYSTEM_ATOMIC_H
#define LLVM_SYSTEM_ATOMIC_H

#include "llvm/Support/DataTypes.h"
//---OPENCOR--- BEGIN
#include "llvmglobal.h"
//---OPENCOR--- END

namespace llvm {
  namespace sys {
/*---OPENCOR---
    void MemoryFence();
*/
//---OPENCOR--- BEGIN
    void LLVM_EXPORT MemoryFence();
//---OPENCOR--- END

#ifdef _MSC_VER
    typedef long cas_flag;
#else
    typedef uint32_t cas_flag;
#endif
    cas_flag CompareAndSwap(volatile cas_flag* ptr,
                            cas_flag new_value,
                            cas_flag old_value);
/*---OPENCOR---
    cas_flag AtomicIncrement(volatile cas_flag* ptr);
    cas_flag AtomicDecrement(volatile cas_flag* ptr);
*/
//---OPENCOR--- BEGIN
    cas_flag LLVM_EXPORT AtomicIncrement(volatile cas_flag* ptr);
    cas_flag LLVM_EXPORT AtomicDecrement(volatile cas_flag* ptr);
//---OPENCOR--- END
    cas_flag AtomicAdd(volatile cas_flag* ptr, cas_flag val);
    cas_flag AtomicMul(volatile cas_flag* ptr, cas_flag val);
    cas_flag AtomicDiv(volatile cas_flag* ptr, cas_flag val);
  }
}

#endif
