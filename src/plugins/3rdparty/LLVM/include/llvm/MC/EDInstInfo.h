//===-- llvm/MC/EDInstInfo.h - EDis instruction info ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef EDINSTINFO_H
#define EDINSTINFO_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
  
#define EDIS_MAX_OPERANDS 13
#define EDIS_MAX_SYNTAXES 2

struct EDInstInfo {
  uint8_t       instructionType;
  uint8_t       numOperands;
  uint8_t       operandTypes[EDIS_MAX_OPERANDS];
  uint8_t       operandFlags[EDIS_MAX_OPERANDS];
/*---OPENCOR---
  const signed char operandOrders[EDIS_MAX_SYNTAXES][EDIS_MAX_OPERANDS];
*/
//---OPENCOR--- BEGIN
// Note: this addresses the issue reported at:
//       https://connectppe.microsoft.com/VisualStudio/feedback/details/641144/llvmx86disassembler-fails-to-compile-properly-in-optimize-mode
  signed char operandOrders[EDIS_MAX_SYNTAXES][EDIS_MAX_OPERANDS];
//---OPENCOR--- END
};
  
} // namespace llvm

#endif
