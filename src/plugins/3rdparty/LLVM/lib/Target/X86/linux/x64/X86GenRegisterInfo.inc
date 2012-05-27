//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// Target Register Enum Values
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//


#ifdef GET_REGINFO_ENUM
#undef GET_REGINFO_ENUM
namespace llvm {

class MCRegisterClass;
extern const MCRegisterClass X86MCRegisterClasses[];

namespace X86 {
enum {
  NoRegister,
  AH = 1,
  AL = 2,
  AX = 3,
  BH = 4,
  BL = 5,
  BP = 6,
  BPL = 7,
  BX = 8,
  CH = 9,
  CL = 10,
  CR0 = 11,
  CR1 = 12,
  CR2 = 13,
  CR3 = 14,
  CR4 = 15,
  CR5 = 16,
  CR6 = 17,
  CR7 = 18,
  CR8 = 19,
  CR9 = 20,
  CR10 = 21,
  CR11 = 22,
  CR12 = 23,
  CR13 = 24,
  CR14 = 25,
  CR15 = 26,
  CS = 27,
  CX = 28,
  DH = 29,
  DI = 30,
  DIL = 31,
  DL = 32,
  DR0 = 33,
  DR1 = 34,
  DR2 = 35,
  DR3 = 36,
  DR4 = 37,
  DR5 = 38,
  DR6 = 39,
  DR7 = 40,
  DS = 41,
  DX = 42,
  EAX = 43,
  EBP = 44,
  EBX = 45,
  ECX = 46,
  EDI = 47,
  EDX = 48,
  EFLAGS = 49,
  EIP = 50,
  EIZ = 51,
  ES = 52,
  ESI = 53,
  ESP = 54,
  FP0 = 55,
  FP1 = 56,
  FP2 = 57,
  FP3 = 58,
  FP4 = 59,
  FP5 = 60,
  FP6 = 61,
  FS = 62,
  GS = 63,
  IP = 64,
  MM0 = 65,
  MM1 = 66,
  MM2 = 67,
  MM3 = 68,
  MM4 = 69,
  MM5 = 70,
  MM6 = 71,
  MM7 = 72,
  R8 = 73,
  R8B = 74,
  R8D = 75,
  R8W = 76,
  R9 = 77,
  R9B = 78,
  R9D = 79,
  R9W = 80,
  R10 = 81,
  R10B = 82,
  R10D = 83,
  R10W = 84,
  R11 = 85,
  R11B = 86,
  R11D = 87,
  R11W = 88,
  R12 = 89,
  R12B = 90,
  R12D = 91,
  R12W = 92,
  R13 = 93,
  R13B = 94,
  R13D = 95,
  R13W = 96,
  R14 = 97,
  R14B = 98,
  R14D = 99,
  R14W = 100,
  R15 = 101,
  R15B = 102,
  R15D = 103,
  R15W = 104,
  RAX = 105,
  RBP = 106,
  RBX = 107,
  RCX = 108,
  RDI = 109,
  RDX = 110,
  RIP = 111,
  RIZ = 112,
  RSI = 113,
  RSP = 114,
  SI = 115,
  SIL = 116,
  SP = 117,
  SPL = 118,
  SS = 119,
  ST0 = 120,
  ST1 = 121,
  ST2 = 122,
  ST3 = 123,
  ST4 = 124,
  ST5 = 125,
  ST6 = 126,
  ST7 = 127,
  XMM0 = 128,
  XMM1 = 129,
  XMM2 = 130,
  XMM3 = 131,
  XMM4 = 132,
  XMM5 = 133,
  XMM6 = 134,
  XMM7 = 135,
  XMM8 = 136,
  XMM9 = 137,
  XMM10 = 138,
  XMM11 = 139,
  XMM12 = 140,
  XMM13 = 141,
  XMM14 = 142,
  XMM15 = 143,
  YMM0 = 144,
  YMM1 = 145,
  YMM2 = 146,
  YMM3 = 147,
  YMM4 = 148,
  YMM5 = 149,
  YMM6 = 150,
  YMM7 = 151,
  YMM8 = 152,
  YMM9 = 153,
  YMM10 = 154,
  YMM11 = 155,
  YMM12 = 156,
  YMM13 = 157,
  YMM14 = 158,
  YMM15 = 159,
  NUM_TARGET_REGS 	// 160
};
}

// Register classes
namespace X86 {
enum {
  GR8RegClassID = 0,
  GR64RegClassID = 1,
  GR16RegClassID = 2,
  FR32RegClassID = 3,
  GR32RegClassID = 4,
  CONTROL_REGRegClassID = 5,
  FR64RegClassID = 6,
  GR64_with_sub_8bitRegClassID = 7,
  VR128RegClassID = 8,
  VR256RegClassID = 9,
  GR32_NOAXRegClassID = 10,
  GR32_NOSPRegClassID = 11,
  GR64_NOSPRegClassID = 12,
  GR64_with_sub_32bit_in_GR32_NOAXRegClassID = 13,
  GR32_NOAX_and_GR32_NOSPRegClassID = 14,
  GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClassID = 15,
  GR64_NOREXRegClassID = 16,
  GR64_TCRegClassID = 17,
  GR8_NOREXRegClassID = 18,
  GR16_NOREXRegClassID = 19,
  DEBUG_REGRegClassID = 20,
  GR32_NOREXRegClassID = 21,
  GR64_NOSP_and_GR64_TCRegClassID = 22,
  GR64_with_sub_16bit_in_GR16_NOREXRegClassID = 23,
  VR64RegClassID = 24,
  RSTRegClassID = 25,
  GR32_NOAX_and_GR32_NOREXRegClassID = 26,
  GR32_NOREX_NOSPRegClassID = 27,
  RFP32RegClassID = 28,
  RFP64RegClassID = 29,
  GR64_NOREX_NOSPRegClassID = 30,
  GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID = 31,
  GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID = 32,
  RFP80RegClassID = 33,
  SEGMENT_REGRegClassID = 34,
  GR32_NOAX_and_GR32_NOREX_NOSPRegClassID = 35,
  GR64_NOREX_and_GR64_TCRegClassID = 36,
  GR64_TCW64RegClassID = 37,
  GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClassID = 38,
  GR64_NOREX_NOSP_and_GR64_TCRegClassID = 39,
  GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID = 40,
  GR8_ABCD_HRegClassID = 41,
  GR8_ABCD_LRegClassID = 42,
  GR16_ABCDRegClassID = 43,
  GR32_ABCDRegClassID = 44,
  GR64_ABCDRegClassID = 45,
  GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID = 46,
  GR32_ABCD_and_GR32_NOAXRegClassID = 47,
  GR32_TCRegClassID = 48,
  GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClassID = 49,
  GR64_with_sub_32bit_in_GR32_TCRegClassID = 50,
  GR32_ADRegClassID = 51,
  GR32_NOAX_and_GR32_TCRegClassID = 52,
  GR64_with_sub_32bit_in_GR32_ADRegClassID = 53,
  GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClassID = 54,
  CCRRegClassID = 55,
  GR32_AD_and_GR32_NOAXRegClassID = 56,
  GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClassID = 57
  };
}

// Subregister indices
namespace X86 {
enum {
  NoSubRegister,
  sub_8bit,	// 1
  sub_8bit_hi,	// 2
  sub_16bit,	// 3
  sub_32bit,	// 4
  sub_sd,	// 5
  sub_ss,	// 6
  sub_xmm,	// 7
  NUM_TARGET_NAMED_SUBREGS
};
}
} // End llvm namespace 
#endif // GET_REGINFO_ENUM

//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// MC Register Information
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//


#ifdef GET_REGINFO_MC_DESC
#undef GET_REGINFO_MC_DESC
namespace llvm {

extern const uint16_t X86RegLists[] = {
  /* 0 */ X86::RAX, X86::EAX, X86::AX, X86::AL, X86::AH, 0,
  /* 6 */ X86::EAX, X86::RAX, X86::AX, X86::AL, X86::AH, 0,
  /* 12 */ X86::RBX, X86::EBX, X86::BX, X86::BL, X86::BH, 0,
  /* 18 */ X86::EBX, X86::RBX, X86::BX, X86::BL, X86::BH, 0,
  /* 24 */ X86::RBP, X86::EBP, X86::BP, X86::BPL, 0,
  /* 29 */ X86::EBP, X86::RBP, X86::BP, X86::BPL, 0,
  /* 34 */ X86::RCX, X86::ECX, X86::CX, X86::CL, X86::CH, 0,
  /* 40 */ X86::ECX, X86::RCX, X86::CX, X86::CL, X86::CH, 0,
  /* 46 */ X86::CR0, 0,
  /* 48 */ X86::CR1, 0,
  /* 50 */ X86::CR2, 0,
  /* 52 */ X86::CR3, 0,
  /* 54 */ X86::CR4, 0,
  /* 56 */ X86::CR5, 0,
  /* 58 */ X86::CR6, 0,
  /* 60 */ X86::CR7, 0,
  /* 62 */ X86::CR8, 0,
  /* 64 */ X86::CR9, 0,
  /* 66 */ X86::CR10, 0,
  /* 68 */ X86::CR11, 0,
  /* 70 */ X86::CR12, 0,
  /* 72 */ X86::CR13, 0,
  /* 74 */ X86::CR14, 0,
  /* 76 */ X86::CR15, 0,
  /* 78 */ X86::CS, 0,
  /* 80 */ X86::RDX, X86::EDX, X86::DX, X86::DL, X86::DH, 0,
  /* 86 */ X86::EDX, X86::RDX, X86::DX, X86::DL, X86::DH, 0,
  /* 92 */ X86::RDI, X86::EDI, X86::DI, X86::DIL, 0,
  /* 97 */ X86::EDI, X86::RDI, X86::DI, X86::DIL, 0,
  /* 102 */ X86::DR0, 0,
  /* 104 */ X86::DR1, 0,
  /* 106 */ X86::DR2, 0,
  /* 108 */ X86::DR3, 0,
  /* 110 */ X86::DR4, 0,
  /* 112 */ X86::DR5, 0,
  /* 114 */ X86::DR6, 0,
  /* 116 */ X86::DR7, 0,
  /* 118 */ X86::DS, 0,
  /* 120 */ X86::EFLAGS, 0,
  /* 122 */ X86::EIZ, 0,
  /* 124 */ X86::ES, 0,
  /* 126 */ X86::ST7, X86::FP0, 0,
  /* 129 */ X86::ST6, X86::FP1, 0,
  /* 132 */ X86::ST5, X86::FP2, 0,
  /* 135 */ X86::ST4, X86::FP3, 0,
  /* 138 */ X86::ST3, X86::FP4, 0,
  /* 141 */ X86::ST2, X86::FP5, 0,
  /* 144 */ X86::ST1, X86::FP6, 0,
  /* 147 */ X86::FS, 0,
  /* 149 */ X86::GS, 0,
  /* 151 */ X86::RIP, X86::EIP, X86::IP, 0,
  /* 155 */ X86::MM0, 0,
  /* 157 */ X86::MM1, 0,
  /* 159 */ X86::MM2, 0,
  /* 161 */ X86::MM3, 0,
  /* 163 */ X86::MM4, 0,
  /* 165 */ X86::MM5, 0,
  /* 167 */ X86::MM6, 0,
  /* 169 */ X86::MM7, 0,
  /* 171 */ X86::R8W, X86::R8B, X86::R8D, X86::R8, 0,
  /* 176 */ X86::R8B, X86::R8W, X86::R8D, X86::R8, 0,
  /* 181 */ X86::R8D, X86::R8, X86::R8W, X86::R8B, 0,
  /* 186 */ X86::R8, X86::R8D, X86::R8W, X86::R8B, 0,
  /* 191 */ X86::R9W, X86::R9B, X86::R9D, X86::R9, 0,
  /* 196 */ X86::R9B, X86::R9W, X86::R9D, X86::R9, 0,
  /* 201 */ X86::R9D, X86::R9, X86::R9W, X86::R9B, 0,
  /* 206 */ X86::R9, X86::R9D, X86::R9W, X86::R9B, 0,
  /* 211 */ X86::R10W, X86::R10B, X86::R10D, X86::R10, 0,
  /* 216 */ X86::R10B, X86::R10W, X86::R10D, X86::R10, 0,
  /* 221 */ X86::R10D, X86::R10, X86::R10W, X86::R10B, 0,
  /* 226 */ X86::R10, X86::R10D, X86::R10W, X86::R10B, 0,
  /* 231 */ X86::R11W, X86::R11B, X86::R11D, X86::R11, 0,
  /* 236 */ X86::R11B, X86::R11W, X86::R11D, X86::R11, 0,
  /* 241 */ X86::R11D, X86::R11, X86::R11W, X86::R11B, 0,
  /* 246 */ X86::R11, X86::R11D, X86::R11W, X86::R11B, 0,
  /* 251 */ X86::R12W, X86::R12B, X86::R12D, X86::R12, 0,
  /* 256 */ X86::R12B, X86::R12W, X86::R12D, X86::R12, 0,
  /* 261 */ X86::R12D, X86::R12, X86::R12W, X86::R12B, 0,
  /* 266 */ X86::R12, X86::R12D, X86::R12W, X86::R12B, 0,
  /* 271 */ X86::R13W, X86::R13B, X86::R13D, X86::R13, 0,
  /* 276 */ X86::R13B, X86::R13W, X86::R13D, X86::R13, 0,
  /* 281 */ X86::R13D, X86::R13, X86::R13W, X86::R13B, 0,
  /* 286 */ X86::R13, X86::R13D, X86::R13W, X86::R13B, 0,
  /* 291 */ X86::R14W, X86::R14B, X86::R14D, X86::R14, 0,
  /* 296 */ X86::R14B, X86::R14W, X86::R14D, X86::R14, 0,
  /* 301 */ X86::R14D, X86::R14, X86::R14W, X86::R14B, 0,
  /* 306 */ X86::R14, X86::R14D, X86::R14W, X86::R14B, 0,
  /* 311 */ X86::R15W, X86::R15B, X86::R15D, X86::R15, 0,
  /* 316 */ X86::R15B, X86::R15W, X86::R15D, X86::R15, 0,
  /* 321 */ X86::R15D, X86::R15, X86::R15W, X86::R15B, 0,
  /* 326 */ X86::R15, X86::R15D, X86::R15W, X86::R15B, 0,
  /* 331 */ X86::AX, X86::AH, X86::AL, X86::EAX, X86::RAX, 0,
  /* 337 */ X86::AH, X86::AX, X86::EAX, X86::RAX, 0,
  /* 342 */ X86::AL, X86::AX, X86::EAX, X86::RAX, 0,
  /* 347 */ X86::BPL, X86::BP, X86::EBP, X86::RBP, 0,
  /* 352 */ X86::BP, X86::BPL, X86::EBP, X86::RBP, 0,
  /* 357 */ X86::BX, X86::BH, X86::BL, X86::EBX, X86::RBX, 0,
  /* 363 */ X86::BH, X86::BX, X86::EBX, X86::RBX, 0,
  /* 368 */ X86::BL, X86::BX, X86::EBX, X86::RBX, 0,
  /* 373 */ X86::CX, X86::CH, X86::CL, X86::ECX, X86::RCX, 0,
  /* 379 */ X86::CH, X86::CX, X86::ECX, X86::RCX, 0,
  /* 384 */ X86::CL, X86::CX, X86::ECX, X86::RCX, 0,
  /* 389 */ X86::DIL, X86::DI, X86::EDI, X86::RDI, 0,
  /* 394 */ X86::DI, X86::DIL, X86::EDI, X86::RDI, 0,
  /* 399 */ X86::DX, X86::DH, X86::DL, X86::EDX, X86::RDX, 0,
  /* 405 */ X86::DH, X86::DX, X86::EDX, X86::RDX, 0,
  /* 410 */ X86::DL, X86::DX, X86::EDX, X86::RDX, 0,
  /* 415 */ X86::IP, X86::EIP, X86::RIP, 0,
  /* 419 */ X86::EIP, X86::IP, X86::RIP, 0,
  /* 423 */ X86::RIZ, 0,
  /* 425 */ X86::SIL, X86::SI, X86::ESI, X86::RSI, 0,
  /* 430 */ X86::SI, X86::SIL, X86::ESI, X86::RSI, 0,
  /* 435 */ X86::SPL, X86::SP, X86::ESP, X86::RSP, 0,
  /* 440 */ X86::SP, X86::SPL, X86::ESP, X86::RSP, 0,
  /* 445 */ X86::RSI, X86::ESI, X86::SI, X86::SIL, 0,
  /* 450 */ X86::ESI, X86::RSI, X86::SI, X86::SIL, 0,
  /* 455 */ X86::RSP, X86::ESP, X86::SP, X86::SPL, 0,
  /* 460 */ X86::ESP, X86::RSP, X86::SP, X86::SPL, 0,
  /* 465 */ X86::SS, 0,
  /* 467 */ X86::ST0, 0,
  /* 469 */ X86::FP6, X86::ST1, 0,
  /* 472 */ X86::FP5, X86::ST2, 0,
  /* 475 */ X86::FP4, X86::ST3, 0,
  /* 478 */ X86::FP3, X86::ST4, 0,
  /* 481 */ X86::FP2, X86::ST5, 0,
  /* 484 */ X86::FP1, X86::ST6, 0,
  /* 487 */ X86::FP0, X86::ST7, 0,
  /* 490 */ X86::YMM0, X86::XMM0, 0,
  /* 493 */ X86::YMM1, X86::XMM1, 0,
  /* 496 */ X86::YMM2, X86::XMM2, 0,
  /* 499 */ X86::YMM3, X86::XMM3, 0,
  /* 502 */ X86::YMM4, X86::XMM4, 0,
  /* 505 */ X86::YMM5, X86::XMM5, 0,
  /* 508 */ X86::YMM6, X86::XMM6, 0,
  /* 511 */ X86::YMM7, X86::XMM7, 0,
  /* 514 */ X86::YMM8, X86::XMM8, 0,
  /* 517 */ X86::YMM9, X86::XMM9, 0,
  /* 520 */ X86::YMM10, X86::XMM10, 0,
  /* 523 */ X86::YMM11, X86::XMM11, 0,
  /* 526 */ X86::YMM12, X86::XMM12, 0,
  /* 529 */ X86::YMM13, X86::XMM13, 0,
  /* 532 */ X86::YMM14, X86::XMM14, 0,
  /* 535 */ X86::YMM15, X86::XMM15, 0,
  /* 538 */ X86::XMM0, X86::YMM0, 0,
  /* 541 */ X86::XMM1, X86::YMM1, 0,
  /* 544 */ X86::XMM2, X86::YMM2, 0,
  /* 547 */ X86::XMM3, X86::YMM3, 0,
  /* 550 */ X86::XMM4, X86::YMM4, 0,
  /* 553 */ X86::XMM5, X86::YMM5, 0,
  /* 556 */ X86::XMM6, X86::YMM6, 0,
  /* 559 */ X86::XMM7, X86::YMM7, 0,
  /* 562 */ X86::XMM8, X86::YMM8, 0,
  /* 565 */ X86::XMM9, X86::YMM9, 0,
  /* 568 */ X86::XMM10, X86::YMM10, 0,
  /* 571 */ X86::XMM11, X86::YMM11, 0,
  /* 574 */ X86::XMM12, X86::YMM12, 0,
  /* 577 */ X86::XMM13, X86::YMM13, 0,
  /* 580 */ X86::XMM14, X86::YMM14, 0,
  /* 583 */ X86::XMM15, X86::YMM15, 0,
};

extern const MCRegisterDesc X86RegDesc[] = { // Descriptors
  { "NOREG", 0, 0, 0 },
  { "AH", 337, 5, 338 },
  { "AL", 342, 5, 338 },
  { "AX", 331, 3, 334 },
  { "BH", 363, 5, 364 },
  { "BL", 368, 5, 364 },
  { "BP", 352, 27, 349 },
  { "BPL", 347, 5, 348 },
  { "BX", 357, 15, 360 },
  { "CH", 379, 5, 380 },
  { "CL", 384, 5, 380 },
  { "CR0", 46, 5, 5 },
  { "CR1", 48, 5, 5 },
  { "CR2", 50, 5, 5 },
  { "CR3", 52, 5, 5 },
  { "CR4", 54, 5, 5 },
  { "CR5", 56, 5, 5 },
  { "CR6", 58, 5, 5 },
  { "CR7", 60, 5, 5 },
  { "CR8", 62, 5, 5 },
  { "CR9", 64, 5, 5 },
  { "CR10", 66, 5, 5 },
  { "CR11", 68, 5, 5 },
  { "CR12", 70, 5, 5 },
  { "CR13", 72, 5, 5 },
  { "CR14", 74, 5, 5 },
  { "CR15", 76, 5, 5 },
  { "CS", 78, 5, 5 },
  { "CX", 373, 37, 376 },
  { "DH", 405, 5, 406 },
  { "DI", 394, 95, 391 },
  { "DIL", 389, 5, 390 },
  { "DL", 410, 5, 406 },
  { "DR0", 102, 5, 5 },
  { "DR1", 104, 5, 5 },
  { "DR2", 106, 5, 5 },
  { "DR3", 108, 5, 5 },
  { "DR4", 110, 5, 5 },
  { "DR5", 112, 5, 5 },
  { "DR6", 114, 5, 5 },
  { "DR7", 116, 5, 5 },
  { "DS", 118, 5, 5 },
  { "DX", 399, 83, 402 },
  { "EAX", 6, 2, 335 },
  { "EBP", 29, 26, 350 },
  { "EBX", 18, 14, 361 },
  { "ECX", 40, 36, 377 },
  { "EDI", 97, 94, 392 },
  { "EDX", 86, 82, 403 },
  { "EFLAGS", 120, 5, 5 },
  { "EIP", 419, 153, 417 },
  { "EIZ", 122, 5, 5 },
  { "ES", 124, 5, 5 },
  { "ESI", 450, 447, 428 },
  { "ESP", 460, 457, 438 },
  { "FP0", 487, 5, 5 },
  { "FP1", 484, 5, 5 },
  { "FP2", 481, 5, 5 },
  { "FP3", 478, 5, 5 },
  { "FP4", 475, 5, 5 },
  { "FP5", 472, 5, 5 },
  { "FP6", 469, 5, 5 },
  { "FS", 147, 5, 5 },
  { "GS", 149, 5, 5 },
  { "IP", 415, 5, 416 },
  { "MM0", 155, 5, 5 },
  { "MM1", 157, 5, 5 },
  { "MM2", 159, 5, 5 },
  { "MM3", 161, 5, 5 },
  { "MM4", 163, 5, 5 },
  { "MM5", 165, 5, 5 },
  { "MM6", 167, 5, 5 },
  { "MM7", 169, 5, 5 },
  { "R8", 186, 187, 5 },
  { "R8B", 176, 5, 177 },
  { "R8D", 181, 183, 174 },
  { "R8W", 171, 184, 173 },
  { "R9", 206, 207, 5 },
  { "R9B", 196, 5, 197 },
  { "R9D", 201, 203, 194 },
  { "R9W", 191, 204, 193 },
  { "R10", 226, 227, 5 },
  { "R10B", 216, 5, 217 },
  { "R10D", 221, 223, 214 },
  { "R10W", 211, 224, 213 },
  { "R11", 246, 247, 5 },
  { "R11B", 236, 5, 237 },
  { "R11D", 241, 243, 234 },
  { "R11W", 231, 244, 233 },
  { "R12", 266, 267, 5 },
  { "R12B", 256, 5, 257 },
  { "R12D", 261, 263, 254 },
  { "R12W", 251, 264, 253 },
  { "R13", 286, 287, 5 },
  { "R13B", 276, 5, 277 },
  { "R13D", 281, 283, 274 },
  { "R13W", 271, 284, 273 },
  { "R14", 306, 307, 5 },
  { "R14B", 296, 5, 297 },
  { "R14D", 301, 303, 294 },
  { "R14W", 291, 304, 293 },
  { "R15", 326, 327, 5 },
  { "R15B", 316, 5, 317 },
  { "R15D", 321, 323, 314 },
  { "R15W", 311, 324, 313 },
  { "RAX", 0, 1, 5 },
  { "RBP", 24, 25, 5 },
  { "RBX", 12, 13, 5 },
  { "RCX", 34, 35, 5 },
  { "RDI", 92, 93, 5 },
  { "RDX", 80, 81, 5 },
  { "RIP", 151, 152, 5 },
  { "RIZ", 423, 5, 5 },
  { "RSI", 445, 446, 5 },
  { "RSP", 455, 456, 5 },
  { "SI", 430, 448, 427 },
  { "SIL", 425, 5, 426 },
  { "SP", 440, 458, 437 },
  { "SPL", 435, 5, 436 },
  { "SS", 465, 5, 5 },
  { "ST0", 467, 5, 5 },
  { "ST1", 144, 5, 5 },
  { "ST2", 141, 5, 5 },
  { "ST3", 138, 5, 5 },
  { "ST4", 135, 5, 5 },
  { "ST5", 132, 5, 5 },
  { "ST6", 129, 5, 5 },
  { "ST7", 126, 5, 5 },
  { "XMM0", 538, 5, 539 },
  { "XMM1", 541, 5, 542 },
  { "XMM2", 544, 5, 545 },
  { "XMM3", 547, 5, 548 },
  { "XMM4", 550, 5, 551 },
  { "XMM5", 553, 5, 554 },
  { "XMM6", 556, 5, 557 },
  { "XMM7", 559, 5, 560 },
  { "XMM8", 562, 5, 563 },
  { "XMM9", 565, 5, 566 },
  { "XMM10", 568, 5, 569 },
  { "XMM11", 571, 5, 572 },
  { "XMM12", 574, 5, 575 },
  { "XMM13", 577, 5, 578 },
  { "XMM14", 580, 5, 581 },
  { "XMM15", 583, 5, 584 },
  { "YMM0", 490, 491, 5 },
  { "YMM1", 493, 494, 5 },
  { "YMM2", 496, 497, 5 },
  { "YMM3", 499, 500, 5 },
  { "YMM4", 502, 503, 5 },
  { "YMM5", 505, 506, 5 },
  { "YMM6", 508, 509, 5 },
  { "YMM7", 511, 512, 5 },
  { "YMM8", 514, 515, 5 },
  { "YMM9", 517, 518, 5 },
  { "YMM10", 520, 521, 5 },
  { "YMM11", 523, 524, 5 },
  { "YMM12", 526, 527, 5 },
  { "YMM13", 529, 530, 5 },
  { "YMM14", 532, 533, 5 },
  { "YMM15", 535, 536, 5 },
};

namespace {     // Register classes...
  // GR8 Register Class...
  const uint16_t GR8[] = {
    X86::AL, X86::CL, X86::DL, X86::AH, X86::CH, X86::DH, X86::BL, X86::BH, X86::SIL, X86::DIL, X86::BPL, X86::SPL, X86::R8B, X86::R9B, X86::R10B, X86::R11B, X86::R14B, X86::R15B, X86::R12B, X86::R13B, 
  };

  // GR8 Bit set.
  const uint8_t GR8Bits[] = {
    0xb6, 0x06, 0x00, 0xa0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x00, 0x50, 
  };

  // GR64 Register Class...
  const uint16_t GR64[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R10, X86::R11, X86::RBX, X86::R14, X86::R15, X86::R12, X86::R13, X86::RBP, X86::RSP, X86::RIP, 
  };

  // GR64 Bit set.
  const uint8_t GR64Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0xfe, 0x06, 
  };

  // GR16 Register Class...
  const uint16_t GR16[] = {
    X86::AX, X86::CX, X86::DX, X86::SI, X86::DI, X86::BX, X86::BP, X86::SP, X86::R8W, X86::R9W, X86::R10W, X86::R11W, X86::R14W, X86::R15W, X86::R12W, X86::R13W, 
  };

  // GR16 Bit set.
  const uint8_t GR16Bits[] = {
    0x48, 0x01, 0x00, 0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x10, 0x11, 0x11, 0x11, 0x01, 0x28, 
  };

  // FR32 Register Class...
  const uint16_t FR32[] = {
    X86::XMM0, X86::XMM1, X86::XMM2, X86::XMM3, X86::XMM4, X86::XMM5, X86::XMM6, X86::XMM7, X86::XMM8, X86::XMM9, X86::XMM10, X86::XMM11, X86::XMM12, X86::XMM13, X86::XMM14, X86::XMM15, 
  };

  // FR32 Bit set.
  const uint8_t FR32Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
  };

  // GR32 Register Class...
  const uint16_t GR32[] = {
    X86::EAX, X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::ESP, X86::R8D, X86::R9D, X86::R10D, X86::R11D, X86::R14D, X86::R15D, X86::R12D, X86::R13D, 
  };

  // GR32 Bit set.
  const uint8_t GR32Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x61, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 
  };

  // CONTROL_REG Register Class...
  const uint16_t CONTROL_REG[] = {
    X86::CR0, X86::CR1, X86::CR2, X86::CR3, X86::CR4, X86::CR5, X86::CR6, X86::CR7, X86::CR8, X86::CR9, X86::CR10, X86::CR11, X86::CR12, X86::CR13, X86::CR14, X86::CR15, 
  };

  // CONTROL_REG Bit set.
  const uint8_t CONTROL_REGBits[] = {
    0x00, 0xf8, 0xff, 0x07, 
  };

  // FR64 Register Class...
  const uint16_t FR64[] = {
    X86::XMM0, X86::XMM1, X86::XMM2, X86::XMM3, X86::XMM4, X86::XMM5, X86::XMM6, X86::XMM7, X86::XMM8, X86::XMM9, X86::XMM10, X86::XMM11, X86::XMM12, X86::XMM13, X86::XMM14, X86::XMM15, 
  };

  // FR64 Bit set.
  const uint8_t FR64Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
  };

  // GR64_with_sub_8bit Register Class...
  const uint16_t GR64_with_sub_8bit[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R10, X86::R11, X86::RBX, X86::R14, X86::R15, X86::R12, X86::R13, X86::RBP, X86::RSP, 
  };

  // GR64_with_sub_8bit Bit set.
  const uint8_t GR64_with_sub_8bitBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x7e, 0x06, 
  };

  // VR128 Register Class...
  const uint16_t VR128[] = {
    X86::XMM0, X86::XMM1, X86::XMM2, X86::XMM3, X86::XMM4, X86::XMM5, X86::XMM6, X86::XMM7, X86::XMM8, X86::XMM9, X86::XMM10, X86::XMM11, X86::XMM12, X86::XMM13, X86::XMM14, X86::XMM15, 
  };

  // VR128 Bit set.
  const uint8_t VR128Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
  };

  // VR256 Register Class...
  const uint16_t VR256[] = {
    X86::YMM0, X86::YMM1, X86::YMM2, X86::YMM3, X86::YMM4, X86::YMM5, X86::YMM6, X86::YMM7, X86::YMM8, X86::YMM9, X86::YMM10, X86::YMM11, X86::YMM12, X86::YMM13, X86::YMM14, X86::YMM15, 
  };

  // VR256 Bit set.
  const uint8_t VR256Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
  };

  // GR32_NOAX Register Class...
  const uint16_t GR32_NOAX[] = {
    X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::ESP, X86::R8D, X86::R9D, X86::R10D, X86::R11D, X86::R14D, X86::R15D, X86::R12D, X86::R13D, 
  };

  // GR32_NOAX Bit set.
  const uint8_t GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x61, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 
  };

  // GR32_NOSP Register Class...
  const uint16_t GR32_NOSP[] = {
    X86::EAX, X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::R8D, X86::R9D, X86::R10D, X86::R11D, X86::R14D, X86::R15D, X86::R12D, X86::R13D, 
  };

  // GR32_NOSP Bit set.
  const uint8_t GR32_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x21, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 
  };

  // GR64_NOSP Register Class...
  const uint16_t GR64_NOSP[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R10, X86::R11, X86::RBX, X86::R14, X86::R15, X86::R12, X86::R13, X86::RBP, 
  };

  // GR64_NOSP Bit set.
  const uint8_t GR64_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x7e, 0x02, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_NOAX[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R10, X86::R11, X86::RBX, X86::R14, X86::R15, X86::R12, X86::R13, X86::RBP, X86::RSP, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x7c, 0x06, 
  };

  // GR32_NOAX_and_GR32_NOSP Register Class...
  const uint16_t GR32_NOAX_and_GR32_NOSP[] = {
    X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::R8D, X86::R9D, X86::R10D, X86::R11D, X86::R14D, X86::R15D, X86::R12D, X86::R13D, 
  };

  // GR32_NOAX_and_GR32_NOSP Bit set.
  const uint8_t GR32_NOAX_and_GR32_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x21, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R10, X86::R11, X86::RBX, X86::R14, X86::R15, X86::R12, X86::R13, X86::RBP, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x7c, 0x02, 
  };

  // GR64_NOREX Register Class...
  const uint16_t GR64_NOREX[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RBX, X86::RBP, X86::RSP, X86::RIP, 
  };

  // GR64_NOREX Bit set.
  const uint8_t GR64_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x06, 
  };

  // GR64_TC Register Class...
  const uint16_t GR64_TC[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R11, X86::RIP, 
  };

  // GR64_TC Bit set.
  const uint8_t GR64_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0xf2, 0x02, 
  };

  // GR8_NOREX Register Class...
  const uint16_t GR8_NOREX[] = {
    X86::AL, X86::CL, X86::DL, X86::AH, X86::CH, X86::DH, X86::BL, X86::BH, 
  };

  // GR8_NOREX Bit set.
  const uint8_t GR8_NOREXBits[] = {
    0x36, 0x06, 0x00, 0x20, 0x01, 
  };

  // GR16_NOREX Register Class...
  const uint16_t GR16_NOREX[] = {
    X86::AX, X86::CX, X86::DX, X86::SI, X86::DI, X86::BX, X86::BP, X86::SP, 
  };

  // GR16_NOREX Bit set.
  const uint8_t GR16_NOREXBits[] = {
    0x48, 0x01, 0x00, 0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 
  };

  // DEBUG_REG Register Class...
  const uint16_t DEBUG_REG[] = {
    X86::DR0, X86::DR1, X86::DR2, X86::DR3, X86::DR4, X86::DR5, X86::DR6, X86::DR7, 
  };

  // DEBUG_REG Bit set.
  const uint8_t DEBUG_REGBits[] = {
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 
  };

  // GR32_NOREX Register Class...
  const uint16_t GR32_NOREX[] = {
    X86::EAX, X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::ESP, 
  };

  // GR32_NOREX Bit set.
  const uint8_t GR32_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x61, 
  };

  // GR64_NOSP_and_GR64_TC Register Class...
  const uint16_t GR64_NOSP_and_GR64_TC[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R11, 
  };

  // GR64_NOSP_and_GR64_TC Bit set.
  const uint8_t GR64_NOSP_and_GR64_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0x72, 0x02, 
  };

  // GR64_with_sub_16bit_in_GR16_NOREX Register Class...
  const uint16_t GR64_with_sub_16bit_in_GR16_NOREX[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RBX, X86::RBP, X86::RSP, 
  };

  // GR64_with_sub_16bit_in_GR16_NOREX Bit set.
  const uint8_t GR64_with_sub_16bit_in_GR16_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x06, 
  };

  // VR64 Register Class...
  const uint16_t VR64[] = {
    X86::MM0, X86::MM1, X86::MM2, X86::MM3, X86::MM4, X86::MM5, X86::MM6, X86::MM7, 
  };

  // VR64 Bit set.
  const uint8_t VR64Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 
  };

  // RST Register Class...
  const uint16_t RST[] = {
    X86::ST0, X86::ST1, X86::ST2, X86::ST3, X86::ST4, X86::ST5, X86::ST6, X86::ST7, 
  };

  // RST Bit set.
  const uint8_t RSTBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
  };

  // GR32_NOAX_and_GR32_NOREX Register Class...
  const uint16_t GR32_NOAX_and_GR32_NOREX[] = {
    X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, X86::ESP, 
  };

  // GR32_NOAX_and_GR32_NOREX Bit set.
  const uint8_t GR32_NOAX_and_GR32_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x61, 
  };

  // GR32_NOREX_NOSP Register Class...
  const uint16_t GR32_NOREX_NOSP[] = {
    X86::EAX, X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, 
  };

  // GR32_NOREX_NOSP Bit set.
  const uint8_t GR32_NOREX_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x21, 
  };

  // RFP32 Register Class...
  const uint16_t RFP32[] = {
    X86::FP0, X86::FP1, X86::FP2, X86::FP3, X86::FP4, X86::FP5, X86::FP6, 
  };

  // RFP32 Bit set.
  const uint8_t RFP32Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 
  };

  // RFP64 Register Class...
  const uint16_t RFP64[] = {
    X86::FP0, X86::FP1, X86::FP2, X86::FP3, X86::FP4, X86::FP5, X86::FP6, 
  };

  // RFP64 Bit set.
  const uint8_t RFP64Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 
  };

  // GR64_NOREX_NOSP Register Class...
  const uint16_t GR64_NOREX_NOSP[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RBX, X86::RBP, 
  };

  // GR64_NOREX_NOSP Bit set.
  const uint8_t GR64_NOREX_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x02, 
  };

  // GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX Register Class...
  const uint16_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::R8, X86::R9, X86::R11, 
  };

  // GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX Bit set.
  const uint8_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0x70, 0x02, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RBX, X86::RBP, X86::RSP, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x06, 
  };

  // RFP80 Register Class...
  const uint16_t RFP80[] = {
    X86::FP0, X86::FP1, X86::FP2, X86::FP3, X86::FP4, X86::FP5, X86::FP6, 
  };

  // RFP80 Bit set.
  const uint8_t RFP80Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 
  };

  // SEGMENT_REG Register Class...
  const uint16_t SEGMENT_REG[] = {
    X86::CS, X86::DS, X86::SS, X86::ES, X86::FS, X86::GS, 
  };

  // SEGMENT_REG Bit set.
  const uint8_t SEGMENT_REGBits[] = {
    0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x10, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  };

  // GR32_NOAX_and_GR32_NOREX_NOSP Register Class...
  const uint16_t GR32_NOAX_and_GR32_NOREX_NOSP[] = {
    X86::ECX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, 
  };

  // GR32_NOAX_and_GR32_NOREX_NOSP Bit set.
  const uint8_t GR32_NOAX_and_GR32_NOREX_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x21, 
  };

  // GR64_NOREX_and_GR64_TC Register Class...
  const uint16_t GR64_NOREX_and_GR64_TC[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RIP, 
  };

  // GR64_NOREX_and_GR64_TC Bit set.
  const uint8_t GR64_NOREX_and_GR64_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x02, 
  };

  // GR64_TCW64 Register Class...
  const uint16_t GR64_TCW64[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::R8, X86::R9, X86::R11, 
  };

  // GR64_TCW64 Bit set.
  const uint8_t GR64_TCW64Bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0x52, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, X86::RBX, X86::RBP, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x02, 
  };

  // GR64_NOREX_NOSP_and_GR64_TC Register Class...
  const uint16_t GR64_NOREX_NOSP_and_GR64_TC[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RSI, X86::RDI, 
  };

  // GR64_NOREX_NOSP_and_GR64_TC Bit set.
  const uint8_t GR64_NOREX_NOSP_and_GR64_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x02, 
  };

  // GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX Register Class...
  const uint16_t GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX[] = {
    X86::RCX, X86::RDX, X86::R8, X86::R9, X86::R11, 
  };

  // GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX Bit set.
  const uint8_t GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x20, 0x00, 0x00, 0x50, 
  };

  // GR8_ABCD_H Register Class...
  const uint16_t GR8_ABCD_H[] = {
    X86::AH, X86::CH, X86::DH, X86::BH, 
  };

  // GR8_ABCD_H Bit set.
  const uint8_t GR8_ABCD_HBits[] = {
    0x12, 0x02, 0x00, 0x20, 
  };

  // GR8_ABCD_L Register Class...
  const uint16_t GR8_ABCD_L[] = {
    X86::AL, X86::CL, X86::DL, X86::BL, 
  };

  // GR8_ABCD_L Bit set.
  const uint8_t GR8_ABCD_LBits[] = {
    0x24, 0x04, 0x00, 0x00, 0x01, 
  };

  // GR16_ABCD Register Class...
  const uint16_t GR16_ABCD[] = {
    X86::AX, X86::CX, X86::DX, X86::BX, 
  };

  // GR16_ABCD Bit set.
  const uint8_t GR16_ABCDBits[] = {
    0x08, 0x01, 0x00, 0x10, 0x00, 0x04, 
  };

  // GR32_ABCD Register Class...
  const uint16_t GR32_ABCD[] = {
    X86::EAX, X86::ECX, X86::EDX, X86::EBX, 
  };

  // GR32_ABCD Bit set.
  const uint8_t GR32_ABCDBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 
  };

  // GR64_ABCD Register Class...
  const uint16_t GR64_ABCD[] = {
    X86::RAX, X86::RCX, X86::RDX, X86::RBX, 
  };

  // GR64_ABCD Bit set.
  const uint8_t GR64_ABCDBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5a, 
  };

  // GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Register Class...
  const uint16_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX[] = {
    X86::RCX, X86::RDX, X86::RSI, X86::RDI, 
  };

  // GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Bit set.
  const uint8_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x02, 
  };

  // GR32_ABCD_and_GR32_NOAX Register Class...
  const uint16_t GR32_ABCD_and_GR32_NOAX[] = {
    X86::ECX, X86::EDX, X86::EBX, 
  };

  // GR32_ABCD_and_GR32_NOAX Bit set.
  const uint8_t GR32_ABCD_and_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 
  };

  // GR32_TC Register Class...
  const uint16_t GR32_TC[] = {
    X86::EAX, X86::ECX, X86::EDX, 
  };

  // GR32_TC Bit set.
  const uint8_t GR32_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x01, 
  };

  // GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX[] = {
    X86::RCX, X86::RDX, X86::RBX, 
  };

  // GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 
  };

  // GR64_with_sub_32bit_in_GR32_TC Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_TC[] = {
    X86::RAX, X86::RCX, X86::RDX, 
  };

  // GR64_with_sub_32bit_in_GR32_TC Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
  };

  // GR32_AD Register Class...
  const uint16_t GR32_AD[] = {
    X86::EAX, X86::EDX, 
  };

  // GR32_AD Bit set.
  const uint8_t GR32_ADBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 
  };

  // GR32_NOAX_and_GR32_TC Register Class...
  const uint16_t GR32_NOAX_and_GR32_TC[] = {
    X86::ECX, X86::EDX, 
  };

  // GR32_NOAX_and_GR32_TC Bit set.
  const uint8_t GR32_NOAX_and_GR32_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 
  };

  // GR64_with_sub_32bit_in_GR32_AD Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_AD[] = {
    X86::RAX, X86::RDX, 
  };

  // GR64_with_sub_32bit_in_GR32_AD Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_ADBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC[] = {
    X86::RCX, X86::RDX, 
  };

  // GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 
  };

  // CCR Register Class...
  const uint16_t CCR[] = {
    X86::EFLAGS, 
  };

  // CCR Bit set.
  const uint8_t CCRBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
  };

  // GR32_AD_and_GR32_NOAX Register Class...
  const uint16_t GR32_AD_and_GR32_NOAX[] = {
    X86::EDX, 
  };

  // GR32_AD_and_GR32_NOAX Bit set.
  const uint8_t GR32_AD_and_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  };

  // GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX Register Class...
  const uint16_t GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX[] = {
    X86::RDX, 
  };

  // GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX Bit set.
  const uint8_t GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
  };

}

extern const MCRegisterClass X86MCRegisterClasses[] = {
  { "GR8", GR8, GR8Bits, 20, sizeof(GR8Bits), X86::GR8RegClassID, 1, 1, 1, 1 },
  { "GR64", GR64, GR64Bits, 17, sizeof(GR64Bits), X86::GR64RegClassID, 8, 8, 1, 1 },
  { "GR16", GR16, GR16Bits, 16, sizeof(GR16Bits), X86::GR16RegClassID, 2, 2, 1, 1 },
  { "FR32", FR32, FR32Bits, 16, sizeof(FR32Bits), X86::FR32RegClassID, 4, 4, 1, 1 },
  { "GR32", GR32, GR32Bits, 16, sizeof(GR32Bits), X86::GR32RegClassID, 4, 4, 1, 1 },
  { "CONTROL_REG", CONTROL_REG, CONTROL_REGBits, 16, sizeof(CONTROL_REGBits), X86::CONTROL_REGRegClassID, 8, 8, 1, 1 },
  { "FR64", FR64, FR64Bits, 16, sizeof(FR64Bits), X86::FR64RegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_8bit", GR64_with_sub_8bit, GR64_with_sub_8bitBits, 16, sizeof(GR64_with_sub_8bitBits), X86::GR64_with_sub_8bitRegClassID, 8, 8, 1, 1 },
  { "VR128", VR128, VR128Bits, 16, sizeof(VR128Bits), X86::VR128RegClassID, 16, 16, 1, 1 },
  { "VR256", VR256, VR256Bits, 16, sizeof(VR256Bits), X86::VR256RegClassID, 32, 32, 1, 1 },
  { "GR32_NOAX", GR32_NOAX, GR32_NOAXBits, 15, sizeof(GR32_NOAXBits), X86::GR32_NOAXRegClassID, 4, 4, 1, 1 },
  { "GR32_NOSP", GR32_NOSP, GR32_NOSPBits, 15, sizeof(GR32_NOSPBits), X86::GR32_NOSPRegClassID, 4, 4, 1, 1 },
  { "GR64_NOSP", GR64_NOSP, GR64_NOSPBits, 15, sizeof(GR64_NOSPBits), X86::GR64_NOSPRegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_NOAX", GR64_with_sub_32bit_in_GR32_NOAX, GR64_with_sub_32bit_in_GR32_NOAXBits, 15, sizeof(GR64_with_sub_32bit_in_GR32_NOAXBits), X86::GR64_with_sub_32bit_in_GR32_NOAXRegClassID, 8, 8, 1, 1 },
  { "GR32_NOAX_and_GR32_NOSP", GR32_NOAX_and_GR32_NOSP, GR32_NOAX_and_GR32_NOSPBits, 14, sizeof(GR32_NOAX_and_GR32_NOSPBits), X86::GR32_NOAX_and_GR32_NOSPRegClassID, 4, 4, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP", GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP, GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPBits, 14, sizeof(GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPBits), X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClassID, 8, 8, 1, 1 },
  { "GR64_NOREX", GR64_NOREX, GR64_NOREXBits, 9, sizeof(GR64_NOREXBits), X86::GR64_NOREXRegClassID, 8, 8, 1, 1 },
  { "GR64_TC", GR64_TC, GR64_TCBits, 9, sizeof(GR64_TCBits), X86::GR64_TCRegClassID, 8, 8, 1, 1 },
  { "GR8_NOREX", GR8_NOREX, GR8_NOREXBits, 8, sizeof(GR8_NOREXBits), X86::GR8_NOREXRegClassID, 1, 1, 1, 1 },
  { "GR16_NOREX", GR16_NOREX, GR16_NOREXBits, 8, sizeof(GR16_NOREXBits), X86::GR16_NOREXRegClassID, 2, 2, 1, 1 },
  { "DEBUG_REG", DEBUG_REG, DEBUG_REGBits, 8, sizeof(DEBUG_REGBits), X86::DEBUG_REGRegClassID, 4, 4, 1, 1 },
  { "GR32_NOREX", GR32_NOREX, GR32_NOREXBits, 8, sizeof(GR32_NOREXBits), X86::GR32_NOREXRegClassID, 4, 4, 1, 1 },
  { "GR64_NOSP_and_GR64_TC", GR64_NOSP_and_GR64_TC, GR64_NOSP_and_GR64_TCBits, 8, sizeof(GR64_NOSP_and_GR64_TCBits), X86::GR64_NOSP_and_GR64_TCRegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_16bit_in_GR16_NOREX", GR64_with_sub_16bit_in_GR16_NOREX, GR64_with_sub_16bit_in_GR16_NOREXBits, 8, sizeof(GR64_with_sub_16bit_in_GR16_NOREXBits), X86::GR64_with_sub_16bit_in_GR16_NOREXRegClassID, 8, 8, 1, 1 },
  { "VR64", VR64, VR64Bits, 8, sizeof(VR64Bits), X86::VR64RegClassID, 8, 8, 1, 1 },
  { "RST", RST, RSTBits, 8, sizeof(RSTBits), X86::RSTRegClassID, 10, 4, 1, 0 },
  { "GR32_NOAX_and_GR32_NOREX", GR32_NOAX_and_GR32_NOREX, GR32_NOAX_and_GR32_NOREXBits, 7, sizeof(GR32_NOAX_and_GR32_NOREXBits), X86::GR32_NOAX_and_GR32_NOREXRegClassID, 4, 4, 1, 1 },
  { "GR32_NOREX_NOSP", GR32_NOREX_NOSP, GR32_NOREX_NOSPBits, 7, sizeof(GR32_NOREX_NOSPBits), X86::GR32_NOREX_NOSPRegClassID, 4, 4, 1, 1 },
  { "RFP32", RFP32, RFP32Bits, 7, sizeof(RFP32Bits), X86::RFP32RegClassID, 4, 4, 1, 1 },
  { "RFP64", RFP64, RFP64Bits, 7, sizeof(RFP64Bits), X86::RFP64RegClassID, 8, 4, 1, 1 },
  { "GR64_NOREX_NOSP", GR64_NOREX_NOSP, GR64_NOREX_NOSPBits, 7, sizeof(GR64_NOREX_NOSPBits), X86::GR64_NOREX_NOSPRegClassID, 8, 8, 1, 1 },
  { "GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX", GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX, GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXBits, 7, sizeof(GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXBits), X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX", GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX, GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits, 7, sizeof(GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits), X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID, 8, 8, 1, 1 },
  { "RFP80", RFP80, RFP80Bits, 7, sizeof(RFP80Bits), X86::RFP80RegClassID, 10, 4, 1, 1 },
  { "SEGMENT_REG", SEGMENT_REG, SEGMENT_REGBits, 6, sizeof(SEGMENT_REGBits), X86::SEGMENT_REGRegClassID, 2, 2, 1, 1 },
  { "GR32_NOAX_and_GR32_NOREX_NOSP", GR32_NOAX_and_GR32_NOREX_NOSP, GR32_NOAX_and_GR32_NOREX_NOSPBits, 6, sizeof(GR32_NOAX_and_GR32_NOREX_NOSPBits), X86::GR32_NOAX_and_GR32_NOREX_NOSPRegClassID, 4, 4, 1, 1 },
  { "GR64_NOREX_and_GR64_TC", GR64_NOREX_and_GR64_TC, GR64_NOREX_and_GR64_TCBits, 6, sizeof(GR64_NOREX_and_GR64_TCBits), X86::GR64_NOREX_and_GR64_TCRegClassID, 8, 8, 1, 1 },
  { "GR64_TCW64", GR64_TCW64, GR64_TCW64Bits, 6, sizeof(GR64_TCW64Bits), X86::GR64_TCW64RegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP", GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP, GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPBits, 6, sizeof(GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPBits), X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClassID, 8, 8, 1, 1 },
  { "GR64_NOREX_NOSP_and_GR64_TC", GR64_NOREX_NOSP_and_GR64_TC, GR64_NOREX_NOSP_and_GR64_TCBits, 5, sizeof(GR64_NOREX_NOSP_and_GR64_TCBits), X86::GR64_NOREX_NOSP_and_GR64_TCRegClassID, 8, 8, 1, 1 },
  { "GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX", GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX, GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXBits, 5, sizeof(GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXBits), X86::GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID, 8, 8, 1, 1 },
  { "GR8_ABCD_H", GR8_ABCD_H, GR8_ABCD_HBits, 4, sizeof(GR8_ABCD_HBits), X86::GR8_ABCD_HRegClassID, 1, 1, 1, 1 },
  { "GR8_ABCD_L", GR8_ABCD_L, GR8_ABCD_LBits, 4, sizeof(GR8_ABCD_LBits), X86::GR8_ABCD_LRegClassID, 1, 1, 1, 1 },
  { "GR16_ABCD", GR16_ABCD, GR16_ABCDBits, 4, sizeof(GR16_ABCDBits), X86::GR16_ABCDRegClassID, 2, 2, 1, 1 },
  { "GR32_ABCD", GR32_ABCD, GR32_ABCDBits, 4, sizeof(GR32_ABCDBits), X86::GR32_ABCDRegClassID, 4, 4, 1, 1 },
  { "GR64_ABCD", GR64_ABCD, GR64_ABCDBits, 4, sizeof(GR64_ABCDBits), X86::GR64_ABCDRegClassID, 8, 8, 1, 1 },
  { "GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX", GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX, GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits, 4, sizeof(GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXBits), X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID, 8, 8, 1, 1 },
  { "GR32_ABCD_and_GR32_NOAX", GR32_ABCD_and_GR32_NOAX, GR32_ABCD_and_GR32_NOAXBits, 3, sizeof(GR32_ABCD_and_GR32_NOAXBits), X86::GR32_ABCD_and_GR32_NOAXRegClassID, 4, 4, 1, 1 },
  { "GR32_TC", GR32_TC, GR32_TCBits, 3, sizeof(GR32_TCBits), X86::GR32_TCRegClassID, 4, 4, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX", GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX, GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXBits, 3, sizeof(GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXBits), X86::GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_TC", GR64_with_sub_32bit_in_GR32_TC, GR64_with_sub_32bit_in_GR32_TCBits, 3, sizeof(GR64_with_sub_32bit_in_GR32_TCBits), X86::GR64_with_sub_32bit_in_GR32_TCRegClassID, 8, 8, 1, 1 },
  { "GR32_AD", GR32_AD, GR32_ADBits, 2, sizeof(GR32_ADBits), X86::GR32_ADRegClassID, 4, 4, 1, 1 },
  { "GR32_NOAX_and_GR32_TC", GR32_NOAX_and_GR32_TC, GR32_NOAX_and_GR32_TCBits, 2, sizeof(GR32_NOAX_and_GR32_TCBits), X86::GR32_NOAX_and_GR32_TCRegClassID, 4, 4, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_AD", GR64_with_sub_32bit_in_GR32_AD, GR64_with_sub_32bit_in_GR32_ADBits, 2, sizeof(GR64_with_sub_32bit_in_GR32_ADBits), X86::GR64_with_sub_32bit_in_GR32_ADRegClassID, 8, 8, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC", GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC, GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCBits, 2, sizeof(GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCBits), X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClassID, 8, 8, 1, 1 },
  { "CCR", CCR, CCRBits, 1, sizeof(CCRBits), X86::CCRRegClassID, 4, 4, -1, 0 },
  { "GR32_AD_and_GR32_NOAX", GR32_AD_and_GR32_NOAX, GR32_AD_and_GR32_NOAXBits, 1, sizeof(GR32_AD_and_GR32_NOAXBits), X86::GR32_AD_and_GR32_NOAXRegClassID, 4, 4, 1, 1 },
  { "GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX", GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX, GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXBits, 1, sizeof(GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXBits), X86::GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClassID, 8, 8, 1, 1 },
};

const uint16_t X86SubRegTable[][7] = {
  /* AH */
  {0},
  /* AL */
  {0},
  /* AX */
  {X86::AL, X86::AH, 0, 0, 0, 0, 0},
  /* BH */
  {0},
  /* BL */
  {0},
  /* BP */
  {X86::BPL, 0, 0, 0, 0, 0, 0},
  /* BPL */
  {0},
  /* BX */
  {X86::BL, X86::BH, 0, 0, 0, 0, 0},
  /* CH */
  {0},
  /* CL */
  {0},
  /* CR0 */
  {0},
  /* CR1 */
  {0},
  /* CR2 */
  {0},
  /* CR3 */
  {0},
  /* CR4 */
  {0},
  /* CR5 */
  {0},
  /* CR6 */
  {0},
  /* CR7 */
  {0},
  /* CR8 */
  {0},
  /* CR9 */
  {0},
  /* CR10 */
  {0},
  /* CR11 */
  {0},
  /* CR12 */
  {0},
  /* CR13 */
  {0},
  /* CR14 */
  {0},
  /* CR15 */
  {0},
  /* CS */
  {0},
  /* CX */
  {X86::CL, X86::CH, 0, 0, 0, 0, 0},
  /* DH */
  {0},
  /* DI */
  {X86::DIL, 0, 0, 0, 0, 0, 0},
  /* DIL */
  {0},
  /* DL */
  {0},
  /* DR0 */
  {0},
  /* DR1 */
  {0},
  /* DR2 */
  {0},
  /* DR3 */
  {0},
  /* DR4 */
  {0},
  /* DR5 */
  {0},
  /* DR6 */
  {0},
  /* DR7 */
  {0},
  /* DS */
  {0},
  /* DX */
  {X86::DL, X86::DH, 0, 0, 0, 0, 0},
  /* EAX */
  {X86::AL, X86::AH, X86::AX, 0, 0, 0, 0},
  /* EBP */
  {X86::BPL, 0, X86::BP, 0, 0, 0, 0},
  /* EBX */
  {X86::BL, X86::BH, X86::BX, 0, 0, 0, 0},
  /* ECX */
  {X86::CL, X86::CH, X86::CX, 0, 0, 0, 0},
  /* EDI */
  {X86::DIL, 0, X86::DI, 0, 0, 0, 0},
  /* EDX */
  {X86::DL, X86::DH, X86::DX, 0, 0, 0, 0},
  /* EFLAGS */
  {0},
  /* EIP */
  {0, 0, X86::IP, 0, 0, 0, 0},
  /* EIZ */
  {0},
  /* ES */
  {0},
  /* ESI */
  {X86::SIL, 0, X86::SI, 0, 0, 0, 0},
  /* ESP */
  {X86::SPL, 0, X86::SP, 0, 0, 0, 0},
  /* FP0 */
  {0},
  /* FP1 */
  {0},
  /* FP2 */
  {0},
  /* FP3 */
  {0},
  /* FP4 */
  {0},
  /* FP5 */
  {0},
  /* FP6 */
  {0},
  /* FS */
  {0},
  /* GS */
  {0},
  /* IP */
  {0},
  /* MM0 */
  {0},
  /* MM1 */
  {0},
  /* MM2 */
  {0},
  /* MM3 */
  {0},
  /* MM4 */
  {0},
  /* MM5 */
  {0},
  /* MM6 */
  {0},
  /* MM7 */
  {0},
  /* R8 */
  {X86::R8B, 0, X86::R8W, X86::R8D, 0, 0, 0},
  /* R8B */
  {0},
  /* R8D */
  {X86::R8B, 0, X86::R8W, 0, 0, 0, 0},
  /* R8W */
  {X86::R8B, 0, 0, 0, 0, 0, 0},
  /* R9 */
  {X86::R9B, 0, X86::R9W, X86::R9D, 0, 0, 0},
  /* R9B */
  {0},
  /* R9D */
  {X86::R9B, 0, X86::R9W, 0, 0, 0, 0},
  /* R9W */
  {X86::R9B, 0, 0, 0, 0, 0, 0},
  /* R10 */
  {X86::R10B, 0, X86::R10W, X86::R10D, 0, 0, 0},
  /* R10B */
  {0},
  /* R10D */
  {X86::R10B, 0, X86::R10W, 0, 0, 0, 0},
  /* R10W */
  {X86::R10B, 0, 0, 0, 0, 0, 0},
  /* R11 */
  {X86::R11B, 0, X86::R11W, X86::R11D, 0, 0, 0},
  /* R11B */
  {0},
  /* R11D */
  {X86::R11B, 0, X86::R11W, 0, 0, 0, 0},
  /* R11W */
  {X86::R11B, 0, 0, 0, 0, 0, 0},
  /* R12 */
  {X86::R12B, 0, X86::R12W, X86::R12D, 0, 0, 0},
  /* R12B */
  {0},
  /* R12D */
  {X86::R12B, 0, X86::R12W, 0, 0, 0, 0},
  /* R12W */
  {X86::R12B, 0, 0, 0, 0, 0, 0},
  /* R13 */
  {X86::R13B, 0, X86::R13W, X86::R13D, 0, 0, 0},
  /* R13B */
  {0},
  /* R13D */
  {X86::R13B, 0, X86::R13W, 0, 0, 0, 0},
  /* R13W */
  {X86::R13B, 0, 0, 0, 0, 0, 0},
  /* R14 */
  {X86::R14B, 0, X86::R14W, X86::R14D, 0, 0, 0},
  /* R14B */
  {0},
  /* R14D */
  {X86::R14B, 0, X86::R14W, 0, 0, 0, 0},
  /* R14W */
  {X86::R14B, 0, 0, 0, 0, 0, 0},
  /* R15 */
  {X86::R15B, 0, X86::R15W, X86::R15D, 0, 0, 0},
  /* R15B */
  {0},
  /* R15D */
  {X86::R15B, 0, X86::R15W, 0, 0, 0, 0},
  /* R15W */
  {X86::R15B, 0, 0, 0, 0, 0, 0},
  /* RAX */
  {X86::AL, X86::AH, X86::AX, X86::EAX, 0, 0, 0},
  /* RBP */
  {X86::BPL, 0, X86::BP, X86::EBP, 0, 0, 0},
  /* RBX */
  {X86::BL, X86::BH, X86::BX, X86::EBX, 0, 0, 0},
  /* RCX */
  {X86::CL, X86::CH, X86::CX, X86::ECX, 0, 0, 0},
  /* RDI */
  {X86::DIL, 0, X86::DI, X86::EDI, 0, 0, 0},
  /* RDX */
  {X86::DL, X86::DH, X86::DX, X86::EDX, 0, 0, 0},
  /* RIP */
  {0, 0, X86::IP, X86::EIP, 0, 0, 0},
  /* RIZ */
  {0},
  /* RSI */
  {X86::SIL, 0, X86::SI, X86::ESI, 0, 0, 0},
  /* RSP */
  {X86::SPL, 0, X86::SP, X86::ESP, 0, 0, 0},
  /* SI */
  {X86::SIL, 0, 0, 0, 0, 0, 0},
  /* SIL */
  {0},
  /* SP */
  {X86::SPL, 0, 0, 0, 0, 0, 0},
  /* SPL */
  {0},
  /* SS */
  {0},
  /* ST0 */
  {0},
  /* ST1 */
  {0},
  /* ST2 */
  {0},
  /* ST3 */
  {0},
  /* ST4 */
  {0},
  /* ST5 */
  {0},
  /* ST6 */
  {0},
  /* ST7 */
  {0},
  /* XMM0 */
  {0, 0, 0, 0, X86::XMM0, X86::XMM0, 0},
  /* XMM1 */
  {0, 0, 0, 0, X86::XMM1, X86::XMM1, 0},
  /* XMM2 */
  {0, 0, 0, 0, X86::XMM2, X86::XMM2, 0},
  /* XMM3 */
  {0, 0, 0, 0, X86::XMM3, X86::XMM3, 0},
  /* XMM4 */
  {0, 0, 0, 0, X86::XMM4, X86::XMM4, 0},
  /* XMM5 */
  {0, 0, 0, 0, X86::XMM5, X86::XMM5, 0},
  /* XMM6 */
  {0, 0, 0, 0, X86::XMM6, X86::XMM6, 0},
  /* XMM7 */
  {0, 0, 0, 0, X86::XMM7, X86::XMM7, 0},
  /* XMM8 */
  {0, 0, 0, 0, X86::XMM8, X86::XMM8, 0},
  /* XMM9 */
  {0, 0, 0, 0, X86::XMM9, X86::XMM9, 0},
  /* XMM10 */
  {0, 0, 0, 0, X86::XMM10, X86::XMM10, 0},
  /* XMM11 */
  {0, 0, 0, 0, X86::XMM11, X86::XMM11, 0},
  /* XMM12 */
  {0, 0, 0, 0, X86::XMM12, X86::XMM12, 0},
  /* XMM13 */
  {0, 0, 0, 0, X86::XMM13, X86::XMM13, 0},
  /* XMM14 */
  {0, 0, 0, 0, X86::XMM14, X86::XMM14, 0},
  /* XMM15 */
  {0, 0, 0, 0, X86::XMM15, X86::XMM15, 0},
  /* YMM0 */
  {0, 0, 0, 0, X86::XMM0, X86::XMM0, X86::XMM0},
  /* YMM1 */
  {0, 0, 0, 0, X86::XMM1, X86::XMM1, X86::XMM1},
  /* YMM2 */
  {0, 0, 0, 0, X86::XMM2, X86::XMM2, X86::XMM2},
  /* YMM3 */
  {0, 0, 0, 0, X86::XMM3, X86::XMM3, X86::XMM3},
  /* YMM4 */
  {0, 0, 0, 0, X86::XMM4, X86::XMM4, X86::XMM4},
  /* YMM5 */
  {0, 0, 0, 0, X86::XMM5, X86::XMM5, X86::XMM5},
  /* YMM6 */
  {0, 0, 0, 0, X86::XMM6, X86::XMM6, X86::XMM6},
  /* YMM7 */
  {0, 0, 0, 0, X86::XMM7, X86::XMM7, X86::XMM7},
  /* YMM8 */
  {0, 0, 0, 0, X86::XMM8, X86::XMM8, X86::XMM8},
  /* YMM9 */
  {0, 0, 0, 0, X86::XMM9, X86::XMM9, X86::XMM9},
  /* YMM10 */
  {0, 0, 0, 0, X86::XMM10, X86::XMM10, X86::XMM10},
  /* YMM11 */
  {0, 0, 0, 0, X86::XMM11, X86::XMM11, X86::XMM11},
  /* YMM12 */
  {0, 0, 0, 0, X86::XMM12, X86::XMM12, X86::XMM12},
  /* YMM13 */
  {0, 0, 0, 0, X86::XMM13, X86::XMM13, X86::XMM13},
  /* YMM14 */
  {0, 0, 0, 0, X86::XMM14, X86::XMM14, X86::XMM14},
  /* YMM15 */
  {0, 0, 0, 0, X86::XMM15, X86::XMM15, X86::XMM15},
};

const uint16_t *getX86SubRegTable() {
  return (const uint16_t *)X86SubRegTable;
}

// X86 Dwarf<->LLVM register mappings.
extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour0Dwarf2L[] = {
  { 0U, X86::RAX },
  { 1U, X86::RDX },
  { 2U, X86::RCX },
  { 3U, X86::RBX },
  { 4U, X86::RSI },
  { 5U, X86::RDI },
  { 6U, X86::RBP },
  { 7U, X86::RSP },
  { 8U, X86::R8 },
  { 9U, X86::R9 },
  { 10U, X86::R10 },
  { 11U, X86::R11 },
  { 12U, X86::R12 },
  { 13U, X86::R13 },
  { 14U, X86::R14 },
  { 15U, X86::R15 },
  { 16U, X86::RIP },
  { 17U, X86::XMM0 },
  { 18U, X86::XMM1 },
  { 19U, X86::XMM2 },
  { 20U, X86::XMM3 },
  { 21U, X86::XMM4 },
  { 22U, X86::XMM5 },
  { 23U, X86::XMM6 },
  { 24U, X86::XMM7 },
  { 25U, X86::XMM8 },
  { 26U, X86::XMM9 },
  { 27U, X86::XMM10 },
  { 28U, X86::XMM11 },
  { 29U, X86::XMM12 },
  { 30U, X86::XMM13 },
  { 31U, X86::XMM14 },
  { 32U, X86::XMM15 },
  { 33U, X86::ST0 },
  { 34U, X86::ST1 },
  { 35U, X86::ST2 },
  { 36U, X86::ST3 },
  { 37U, X86::ST4 },
  { 38U, X86::ST5 },
  { 39U, X86::ST6 },
  { 40U, X86::ST7 },
  { 41U, X86::MM0 },
  { 42U, X86::MM1 },
  { 43U, X86::MM2 },
  { 44U, X86::MM3 },
  { 45U, X86::MM4 },
  { 46U, X86::MM5 },
  { 47U, X86::MM6 },
  { 48U, X86::MM7 },
};
extern const unsigned X86DwarfFlavour0Dwarf2LSize = sizeof(X86DwarfFlavour0Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour1Dwarf2L[] = {
  { 0U, X86::EAX },
  { 1U, X86::ECX },
  { 2U, X86::EDX },
  { 3U, X86::EBX },
  { 4U, X86::EBP },
  { 5U, X86::ESP },
  { 6U, X86::ESI },
  { 7U, X86::EDI },
  { 8U, X86::EIP },
  { 12U, X86::ST0 },
  { 13U, X86::ST1 },
  { 14U, X86::ST2 },
  { 15U, X86::ST3 },
  { 16U, X86::ST4 },
  { 17U, X86::ST5 },
  { 18U, X86::ST6 },
  { 19U, X86::ST7 },
  { 21U, X86::XMM0 },
  { 22U, X86::XMM1 },
  { 23U, X86::XMM2 },
  { 24U, X86::XMM3 },
  { 25U, X86::XMM4 },
  { 26U, X86::XMM5 },
  { 27U, X86::XMM6 },
  { 28U, X86::XMM7 },
  { 29U, X86::MM0 },
  { 30U, X86::MM1 },
  { 31U, X86::MM2 },
  { 32U, X86::MM3 },
  { 33U, X86::MM4 },
  { 34U, X86::MM5 },
  { 35U, X86::MM6 },
  { 36U, X86::MM7 },
};
extern const unsigned X86DwarfFlavour1Dwarf2LSize = sizeof(X86DwarfFlavour1Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour2Dwarf2L[] = {
  { 0U, X86::EAX },
  { 1U, X86::ECX },
  { 2U, X86::EDX },
  { 3U, X86::EBX },
  { 4U, X86::ESP },
  { 5U, X86::EBP },
  { 6U, X86::ESI },
  { 7U, X86::EDI },
  { 8U, X86::EIP },
  { 11U, X86::ST0 },
  { 12U, X86::ST1 },
  { 13U, X86::ST2 },
  { 14U, X86::ST3 },
  { 15U, X86::ST4 },
  { 16U, X86::ST5 },
  { 17U, X86::ST6 },
  { 18U, X86::ST7 },
  { 21U, X86::XMM0 },
  { 22U, X86::XMM1 },
  { 23U, X86::XMM2 },
  { 24U, X86::XMM3 },
  { 25U, X86::XMM4 },
  { 26U, X86::XMM5 },
  { 27U, X86::XMM6 },
  { 28U, X86::XMM7 },
  { 29U, X86::MM0 },
  { 30U, X86::MM1 },
  { 31U, X86::MM2 },
  { 32U, X86::MM3 },
  { 33U, X86::MM4 },
  { 34U, X86::MM5 },
  { 35U, X86::MM6 },
  { 36U, X86::MM7 },
};
extern const unsigned X86DwarfFlavour2Dwarf2LSize = sizeof(X86DwarfFlavour2Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour0Dwarf2L[] = {
  { 0U, X86::RAX },
  { 1U, X86::RDX },
  { 2U, X86::RCX },
  { 3U, X86::RBX },
  { 4U, X86::RSI },
  { 5U, X86::RDI },
  { 6U, X86::RBP },
  { 7U, X86::RSP },
  { 8U, X86::R8 },
  { 9U, X86::R9 },
  { 10U, X86::R10 },
  { 11U, X86::R11 },
  { 12U, X86::R12 },
  { 13U, X86::R13 },
  { 14U, X86::R14 },
  { 15U, X86::R15 },
  { 16U, X86::RIP },
  { 17U, X86::XMM0 },
  { 18U, X86::XMM1 },
  { 19U, X86::XMM2 },
  { 20U, X86::XMM3 },
  { 21U, X86::XMM4 },
  { 22U, X86::XMM5 },
  { 23U, X86::XMM6 },
  { 24U, X86::XMM7 },
  { 25U, X86::XMM8 },
  { 26U, X86::XMM9 },
  { 27U, X86::XMM10 },
  { 28U, X86::XMM11 },
  { 29U, X86::XMM12 },
  { 30U, X86::XMM13 },
  { 31U, X86::XMM14 },
  { 32U, X86::XMM15 },
  { 33U, X86::ST0 },
  { 34U, X86::ST1 },
  { 35U, X86::ST2 },
  { 36U, X86::ST3 },
  { 37U, X86::ST4 },
  { 38U, X86::ST5 },
  { 39U, X86::ST6 },
  { 40U, X86::ST7 },
  { 41U, X86::MM0 },
  { 42U, X86::MM1 },
  { 43U, X86::MM2 },
  { 44U, X86::MM3 },
  { 45U, X86::MM4 },
  { 46U, X86::MM5 },
  { 47U, X86::MM6 },
  { 48U, X86::MM7 },
};
extern const unsigned X86EHFlavour0Dwarf2LSize = sizeof(X86EHFlavour0Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour1Dwarf2L[] = {
  { 0U, X86::EAX },
  { 1U, X86::ECX },
  { 2U, X86::EDX },
  { 3U, X86::EBX },
  { 4U, X86::EBP },
  { 5U, X86::ESP },
  { 6U, X86::ESI },
  { 7U, X86::EDI },
  { 8U, X86::EIP },
  { 12U, X86::ST0 },
  { 13U, X86::ST1 },
  { 14U, X86::ST2 },
  { 15U, X86::ST3 },
  { 16U, X86::ST4 },
  { 17U, X86::ST5 },
  { 18U, X86::ST6 },
  { 19U, X86::ST7 },
  { 21U, X86::XMM0 },
  { 22U, X86::XMM1 },
  { 23U, X86::XMM2 },
  { 24U, X86::XMM3 },
  { 25U, X86::XMM4 },
  { 26U, X86::XMM5 },
  { 27U, X86::XMM6 },
  { 28U, X86::XMM7 },
  { 29U, X86::MM0 },
  { 30U, X86::MM1 },
  { 31U, X86::MM2 },
  { 32U, X86::MM3 },
  { 33U, X86::MM4 },
  { 34U, X86::MM5 },
  { 35U, X86::MM6 },
  { 36U, X86::MM7 },
};
extern const unsigned X86EHFlavour1Dwarf2LSize = sizeof(X86EHFlavour1Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour2Dwarf2L[] = {
  { 0U, X86::EAX },
  { 1U, X86::ECX },
  { 2U, X86::EDX },
  { 3U, X86::EBX },
  { 4U, X86::ESP },
  { 5U, X86::EBP },
  { 6U, X86::ESI },
  { 7U, X86::EDI },
  { 8U, X86::EIP },
  { 11U, X86::ST0 },
  { 12U, X86::ST1 },
  { 13U, X86::ST2 },
  { 14U, X86::ST3 },
  { 15U, X86::ST4 },
  { 16U, X86::ST5 },
  { 17U, X86::ST6 },
  { 18U, X86::ST7 },
  { 21U, X86::XMM0 },
  { 22U, X86::XMM1 },
  { 23U, X86::XMM2 },
  { 24U, X86::XMM3 },
  { 25U, X86::XMM4 },
  { 26U, X86::XMM5 },
  { 27U, X86::XMM6 },
  { 28U, X86::XMM7 },
  { 29U, X86::MM0 },
  { 30U, X86::MM1 },
  { 31U, X86::MM2 },
  { 32U, X86::MM3 },
  { 33U, X86::MM4 },
  { 34U, X86::MM5 },
  { 35U, X86::MM6 },
  { 36U, X86::MM7 },
};
extern const unsigned X86EHFlavour2Dwarf2LSize = sizeof(X86EHFlavour2Dwarf2L)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour0L2Dwarf[] = {
  { X86::EAX, -2U },
  { X86::EBP, -2U },
  { X86::EBX, -2U },
  { X86::ECX, -2U },
  { X86::EDI, -2U },
  { X86::EDX, -2U },
  { X86::EIP, -2U },
  { X86::ESI, -2U },
  { X86::ESP, -2U },
  { X86::MM0, 41U },
  { X86::MM1, 42U },
  { X86::MM2, 43U },
  { X86::MM3, 44U },
  { X86::MM4, 45U },
  { X86::MM5, 46U },
  { X86::MM6, 47U },
  { X86::MM7, 48U },
  { X86::R8, 8U },
  { X86::R9, 9U },
  { X86::R10, 10U },
  { X86::R11, 11U },
  { X86::R12, 12U },
  { X86::R13, 13U },
  { X86::R14, 14U },
  { X86::R15, 15U },
  { X86::RAX, 0U },
  { X86::RBP, 6U },
  { X86::RBX, 3U },
  { X86::RCX, 2U },
  { X86::RDI, 5U },
  { X86::RDX, 1U },
  { X86::RIP, 16U },
  { X86::RSI, 4U },
  { X86::RSP, 7U },
  { X86::ST0, 33U },
  { X86::ST1, 34U },
  { X86::ST2, 35U },
  { X86::ST3, 36U },
  { X86::ST4, 37U },
  { X86::ST5, 38U },
  { X86::ST6, 39U },
  { X86::ST7, 40U },
  { X86::XMM0, 17U },
  { X86::XMM1, 18U },
  { X86::XMM2, 19U },
  { X86::XMM3, 20U },
  { X86::XMM4, 21U },
  { X86::XMM5, 22U },
  { X86::XMM6, 23U },
  { X86::XMM7, 24U },
  { X86::XMM8, 25U },
  { X86::XMM9, 26U },
  { X86::XMM10, 27U },
  { X86::XMM11, 28U },
  { X86::XMM12, 29U },
  { X86::XMM13, 30U },
  { X86::XMM14, 31U },
  { X86::XMM15, 32U },
  { X86::YMM0, 17U },
  { X86::YMM1, 18U },
  { X86::YMM2, 19U },
  { X86::YMM3, 20U },
  { X86::YMM4, 21U },
  { X86::YMM5, 22U },
  { X86::YMM6, 23U },
  { X86::YMM7, 24U },
  { X86::YMM8, 25U },
  { X86::YMM9, 26U },
  { X86::YMM10, 27U },
  { X86::YMM11, 28U },
  { X86::YMM12, 29U },
  { X86::YMM13, 30U },
  { X86::YMM14, 31U },
  { X86::YMM15, 32U },
};
extern const unsigned X86DwarfFlavour0L2DwarfSize = sizeof(X86DwarfFlavour0L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour1L2Dwarf[] = {
  { X86::EAX, 0U },
  { X86::EBP, 4U },
  { X86::EBX, 3U },
  { X86::ECX, 1U },
  { X86::EDI, 7U },
  { X86::EDX, 2U },
  { X86::EIP, 8U },
  { X86::ESI, 6U },
  { X86::ESP, 5U },
  { X86::MM0, 29U },
  { X86::MM1, 30U },
  { X86::MM2, 31U },
  { X86::MM3, 32U },
  { X86::MM4, 33U },
  { X86::MM5, 34U },
  { X86::MM6, 35U },
  { X86::MM7, 36U },
  { X86::R8, -2U },
  { X86::R9, -2U },
  { X86::R10, -2U },
  { X86::R11, -2U },
  { X86::R12, -2U },
  { X86::R13, -2U },
  { X86::R14, -2U },
  { X86::R15, -2U },
  { X86::RAX, -2U },
  { X86::RBP, -2U },
  { X86::RBX, -2U },
  { X86::RCX, -2U },
  { X86::RDI, -2U },
  { X86::RDX, -2U },
  { X86::RIP, -2U },
  { X86::RSI, -2U },
  { X86::RSP, -2U },
  { X86::ST0, 12U },
  { X86::ST1, 13U },
  { X86::ST2, 14U },
  { X86::ST3, 15U },
  { X86::ST4, 16U },
  { X86::ST5, 17U },
  { X86::ST6, 18U },
  { X86::ST7, 19U },
  { X86::XMM0, 21U },
  { X86::XMM1, 22U },
  { X86::XMM2, 23U },
  { X86::XMM3, 24U },
  { X86::XMM4, 25U },
  { X86::XMM5, 26U },
  { X86::XMM6, 27U },
  { X86::XMM7, 28U },
  { X86::XMM8, -2U },
  { X86::XMM9, -2U },
  { X86::XMM10, -2U },
  { X86::XMM11, -2U },
  { X86::XMM12, -2U },
  { X86::XMM13, -2U },
  { X86::XMM14, -2U },
  { X86::XMM15, -2U },
  { X86::YMM0, 21U },
  { X86::YMM1, 22U },
  { X86::YMM2, 23U },
  { X86::YMM3, 24U },
  { X86::YMM4, 25U },
  { X86::YMM5, 26U },
  { X86::YMM6, 27U },
  { X86::YMM7, 28U },
  { X86::YMM8, -2U },
  { X86::YMM9, -2U },
  { X86::YMM10, -2U },
  { X86::YMM11, -2U },
  { X86::YMM12, -2U },
  { X86::YMM13, -2U },
  { X86::YMM14, -2U },
  { X86::YMM15, -2U },
};
extern const unsigned X86DwarfFlavour1L2DwarfSize = sizeof(X86DwarfFlavour1L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour2L2Dwarf[] = {
  { X86::EAX, 0U },
  { X86::EBP, 5U },
  { X86::EBX, 3U },
  { X86::ECX, 1U },
  { X86::EDI, 7U },
  { X86::EDX, 2U },
  { X86::EIP, 8U },
  { X86::ESI, 6U },
  { X86::ESP, 4U },
  { X86::MM0, 29U },
  { X86::MM1, 30U },
  { X86::MM2, 31U },
  { X86::MM3, 32U },
  { X86::MM4, 33U },
  { X86::MM5, 34U },
  { X86::MM6, 35U },
  { X86::MM7, 36U },
  { X86::R8, -2U },
  { X86::R9, -2U },
  { X86::R10, -2U },
  { X86::R11, -2U },
  { X86::R12, -2U },
  { X86::R13, -2U },
  { X86::R14, -2U },
  { X86::R15, -2U },
  { X86::RAX, -2U },
  { X86::RBP, -2U },
  { X86::RBX, -2U },
  { X86::RCX, -2U },
  { X86::RDI, -2U },
  { X86::RDX, -2U },
  { X86::RIP, -2U },
  { X86::RSI, -2U },
  { X86::RSP, -2U },
  { X86::ST0, 11U },
  { X86::ST1, 12U },
  { X86::ST2, 13U },
  { X86::ST3, 14U },
  { X86::ST4, 15U },
  { X86::ST5, 16U },
  { X86::ST6, 17U },
  { X86::ST7, 18U },
  { X86::XMM0, 21U },
  { X86::XMM1, 22U },
  { X86::XMM2, 23U },
  { X86::XMM3, 24U },
  { X86::XMM4, 25U },
  { X86::XMM5, 26U },
  { X86::XMM6, 27U },
  { X86::XMM7, 28U },
  { X86::XMM8, -2U },
  { X86::XMM9, -2U },
  { X86::XMM10, -2U },
  { X86::XMM11, -2U },
  { X86::XMM12, -2U },
  { X86::XMM13, -2U },
  { X86::XMM14, -2U },
  { X86::XMM15, -2U },
  { X86::YMM0, 21U },
  { X86::YMM1, 22U },
  { X86::YMM2, 23U },
  { X86::YMM3, 24U },
  { X86::YMM4, 25U },
  { X86::YMM5, 26U },
  { X86::YMM6, 27U },
  { X86::YMM7, 28U },
  { X86::YMM8, -2U },
  { X86::YMM9, -2U },
  { X86::YMM10, -2U },
  { X86::YMM11, -2U },
  { X86::YMM12, -2U },
  { X86::YMM13, -2U },
  { X86::YMM14, -2U },
  { X86::YMM15, -2U },
};
extern const unsigned X86DwarfFlavour2L2DwarfSize = sizeof(X86DwarfFlavour2L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour0L2Dwarf[] = {
  { X86::EAX, -2U },
  { X86::EBP, -2U },
  { X86::EBX, -2U },
  { X86::ECX, -2U },
  { X86::EDI, -2U },
  { X86::EDX, -2U },
  { X86::EIP, -2U },
  { X86::ESI, -2U },
  { X86::ESP, -2U },
  { X86::MM0, 41U },
  { X86::MM1, 42U },
  { X86::MM2, 43U },
  { X86::MM3, 44U },
  { X86::MM4, 45U },
  { X86::MM5, 46U },
  { X86::MM6, 47U },
  { X86::MM7, 48U },
  { X86::R8, 8U },
  { X86::R9, 9U },
  { X86::R10, 10U },
  { X86::R11, 11U },
  { X86::R12, 12U },
  { X86::R13, 13U },
  { X86::R14, 14U },
  { X86::R15, 15U },
  { X86::RAX, 0U },
  { X86::RBP, 6U },
  { X86::RBX, 3U },
  { X86::RCX, 2U },
  { X86::RDI, 5U },
  { X86::RDX, 1U },
  { X86::RIP, 16U },
  { X86::RSI, 4U },
  { X86::RSP, 7U },
  { X86::ST0, 33U },
  { X86::ST1, 34U },
  { X86::ST2, 35U },
  { X86::ST3, 36U },
  { X86::ST4, 37U },
  { X86::ST5, 38U },
  { X86::ST6, 39U },
  { X86::ST7, 40U },
  { X86::XMM0, 17U },
  { X86::XMM1, 18U },
  { X86::XMM2, 19U },
  { X86::XMM3, 20U },
  { X86::XMM4, 21U },
  { X86::XMM5, 22U },
  { X86::XMM6, 23U },
  { X86::XMM7, 24U },
  { X86::XMM8, 25U },
  { X86::XMM9, 26U },
  { X86::XMM10, 27U },
  { X86::XMM11, 28U },
  { X86::XMM12, 29U },
  { X86::XMM13, 30U },
  { X86::XMM14, 31U },
  { X86::XMM15, 32U },
  { X86::YMM0, 17U },
  { X86::YMM1, 18U },
  { X86::YMM2, 19U },
  { X86::YMM3, 20U },
  { X86::YMM4, 21U },
  { X86::YMM5, 22U },
  { X86::YMM6, 23U },
  { X86::YMM7, 24U },
  { X86::YMM8, 25U },
  { X86::YMM9, 26U },
  { X86::YMM10, 27U },
  { X86::YMM11, 28U },
  { X86::YMM12, 29U },
  { X86::YMM13, 30U },
  { X86::YMM14, 31U },
  { X86::YMM15, 32U },
};
extern const unsigned X86EHFlavour0L2DwarfSize = sizeof(X86EHFlavour0L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour1L2Dwarf[] = {
  { X86::EAX, 0U },
  { X86::EBP, 4U },
  { X86::EBX, 3U },
  { X86::ECX, 1U },
  { X86::EDI, 7U },
  { X86::EDX, 2U },
  { X86::EIP, 8U },
  { X86::ESI, 6U },
  { X86::ESP, 5U },
  { X86::MM0, 29U },
  { X86::MM1, 30U },
  { X86::MM2, 31U },
  { X86::MM3, 32U },
  { X86::MM4, 33U },
  { X86::MM5, 34U },
  { X86::MM6, 35U },
  { X86::MM7, 36U },
  { X86::R8, -2U },
  { X86::R9, -2U },
  { X86::R10, -2U },
  { X86::R11, -2U },
  { X86::R12, -2U },
  { X86::R13, -2U },
  { X86::R14, -2U },
  { X86::R15, -2U },
  { X86::RAX, -2U },
  { X86::RBP, -2U },
  { X86::RBX, -2U },
  { X86::RCX, -2U },
  { X86::RDI, -2U },
  { X86::RDX, -2U },
  { X86::RIP, -2U },
  { X86::RSI, -2U },
  { X86::RSP, -2U },
  { X86::ST0, 12U },
  { X86::ST1, 13U },
  { X86::ST2, 14U },
  { X86::ST3, 15U },
  { X86::ST4, 16U },
  { X86::ST5, 17U },
  { X86::ST6, 18U },
  { X86::ST7, 19U },
  { X86::XMM0, 21U },
  { X86::XMM1, 22U },
  { X86::XMM2, 23U },
  { X86::XMM3, 24U },
  { X86::XMM4, 25U },
  { X86::XMM5, 26U },
  { X86::XMM6, 27U },
  { X86::XMM7, 28U },
  { X86::XMM8, -2U },
  { X86::XMM9, -2U },
  { X86::XMM10, -2U },
  { X86::XMM11, -2U },
  { X86::XMM12, -2U },
  { X86::XMM13, -2U },
  { X86::XMM14, -2U },
  { X86::XMM15, -2U },
  { X86::YMM0, 21U },
  { X86::YMM1, 22U },
  { X86::YMM2, 23U },
  { X86::YMM3, 24U },
  { X86::YMM4, 25U },
  { X86::YMM5, 26U },
  { X86::YMM6, 27U },
  { X86::YMM7, 28U },
  { X86::YMM8, -2U },
  { X86::YMM9, -2U },
  { X86::YMM10, -2U },
  { X86::YMM11, -2U },
  { X86::YMM12, -2U },
  { X86::YMM13, -2U },
  { X86::YMM14, -2U },
  { X86::YMM15, -2U },
};
extern const unsigned X86EHFlavour1L2DwarfSize = sizeof(X86EHFlavour1L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour2L2Dwarf[] = {
  { X86::EAX, 0U },
  { X86::EBP, 5U },
  { X86::EBX, 3U },
  { X86::ECX, 1U },
  { X86::EDI, 7U },
  { X86::EDX, 2U },
  { X86::EIP, 8U },
  { X86::ESI, 6U },
  { X86::ESP, 4U },
  { X86::MM0, 29U },
  { X86::MM1, 30U },
  { X86::MM2, 31U },
  { X86::MM3, 32U },
  { X86::MM4, 33U },
  { X86::MM5, 34U },
  { X86::MM6, 35U },
  { X86::MM7, 36U },
  { X86::R8, -2U },
  { X86::R9, -2U },
  { X86::R10, -2U },
  { X86::R11, -2U },
  { X86::R12, -2U },
  { X86::R13, -2U },
  { X86::R14, -2U },
  { X86::R15, -2U },
  { X86::RAX, -2U },
  { X86::RBP, -2U },
  { X86::RBX, -2U },
  { X86::RCX, -2U },
  { X86::RDI, -2U },
  { X86::RDX, -2U },
  { X86::RIP, -2U },
  { X86::RSI, -2U },
  { X86::RSP, -2U },
  { X86::ST0, 11U },
  { X86::ST1, 12U },
  { X86::ST2, 13U },
  { X86::ST3, 14U },
  { X86::ST4, 15U },
  { X86::ST5, 16U },
  { X86::ST6, 17U },
  { X86::ST7, 18U },
  { X86::XMM0, 21U },
  { X86::XMM1, 22U },
  { X86::XMM2, 23U },
  { X86::XMM3, 24U },
  { X86::XMM4, 25U },
  { X86::XMM5, 26U },
  { X86::XMM6, 27U },
  { X86::XMM7, 28U },
  { X86::XMM8, -2U },
  { X86::XMM9, -2U },
  { X86::XMM10, -2U },
  { X86::XMM11, -2U },
  { X86::XMM12, -2U },
  { X86::XMM13, -2U },
  { X86::XMM14, -2U },
  { X86::XMM15, -2U },
  { X86::YMM0, 21U },
  { X86::YMM1, 22U },
  { X86::YMM2, 23U },
  { X86::YMM3, 24U },
  { X86::YMM4, 25U },
  { X86::YMM5, 26U },
  { X86::YMM6, 27U },
  { X86::YMM7, 28U },
  { X86::YMM8, -2U },
  { X86::YMM9, -2U },
  { X86::YMM10, -2U },
  { X86::YMM11, -2U },
  { X86::YMM12, -2U },
  { X86::YMM13, -2U },
  { X86::YMM14, -2U },
  { X86::YMM15, -2U },
};
extern const unsigned X86EHFlavour2L2DwarfSize = sizeof(X86EHFlavour2L2Dwarf)/sizeof(MCRegisterInfo::DwarfLLVMRegPair);

static inline void InitX86MCRegisterInfo(MCRegisterInfo *RI, unsigned RA, unsigned DwarfFlavour = 0, unsigned EHFlavour = 0) {
  RI->InitMCRegisterInfo(X86RegDesc, 160, RA, X86MCRegisterClasses, 58, X86RegLists, (uint16_t*)X86SubRegTable, 7);

  switch (DwarfFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    RI->mapDwarfRegsToLLVMRegs(X86DwarfFlavour0Dwarf2L, X86DwarfFlavour0Dwarf2LSize, false);
    break;
  case 1:
    RI->mapDwarfRegsToLLVMRegs(X86DwarfFlavour1Dwarf2L, X86DwarfFlavour1Dwarf2LSize, false);
    break;
  case 2:
    RI->mapDwarfRegsToLLVMRegs(X86DwarfFlavour2Dwarf2L, X86DwarfFlavour2Dwarf2LSize, false);
    break;
  }
  switch (EHFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    RI->mapDwarfRegsToLLVMRegs(X86EHFlavour0Dwarf2L, X86EHFlavour0Dwarf2LSize, true);
    break;
  case 1:
    RI->mapDwarfRegsToLLVMRegs(X86EHFlavour1Dwarf2L, X86EHFlavour1Dwarf2LSize, true);
    break;
  case 2:
    RI->mapDwarfRegsToLLVMRegs(X86EHFlavour2Dwarf2L, X86EHFlavour2Dwarf2LSize, true);
    break;
  }
  switch (DwarfFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    RI->mapLLVMRegsToDwarfRegs(X86DwarfFlavour0L2Dwarf, X86DwarfFlavour0L2DwarfSize, false);
    break;
  case 1:
    RI->mapLLVMRegsToDwarfRegs(X86DwarfFlavour1L2Dwarf, X86DwarfFlavour1L2DwarfSize, false);
    break;
  case 2:
    RI->mapLLVMRegsToDwarfRegs(X86DwarfFlavour2L2Dwarf, X86DwarfFlavour2L2DwarfSize, false);
    break;
  }
  switch (EHFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    RI->mapLLVMRegsToDwarfRegs(X86EHFlavour0L2Dwarf, X86EHFlavour0L2DwarfSize, true);
    break;
  case 1:
    RI->mapLLVMRegsToDwarfRegs(X86EHFlavour1L2Dwarf, X86EHFlavour1L2DwarfSize, true);
    break;
  case 2:
    RI->mapLLVMRegsToDwarfRegs(X86EHFlavour2L2Dwarf, X86EHFlavour2L2DwarfSize, true);
    break;
  }
}

} // End llvm namespace 
#endif // GET_REGINFO_MC_DESC

//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// Register Information Header Fragment
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//


#ifdef GET_REGINFO_HEADER
#undef GET_REGINFO_HEADER
#include "llvm/Target/TargetRegisterInfo.h"

namespace llvm {

struct X86GenRegisterInfo : public TargetRegisterInfo {
  explicit X86GenRegisterInfo(unsigned RA, unsigned D = 0, unsigned E = 0);
  virtual bool needsStackRealignment(const MachineFunction &) const
     { return false; }
  unsigned composeSubRegIndices(unsigned, unsigned) const;
  const TargetRegisterClass *getSubClassWithSubReg(const TargetRegisterClass*, unsigned) const;
  const TargetRegisterClass *getMatchingSuperRegClass(const TargetRegisterClass*, const TargetRegisterClass*, unsigned) const;
  const RegClassWeight &getRegClassWeight(const TargetRegisterClass *RC) const;
  unsigned getNumRegPressureSets() const;
  unsigned getRegPressureSetLimit(unsigned Idx) const;
  const int *getRegClassPressureSets(const TargetRegisterClass *RC) const;
};

namespace X86 { // Register classes
  extern const TargetRegisterClass GR8RegClass;
  static const TargetRegisterClass * const GR8RegisterClass = &GR8RegClass;
  extern const TargetRegisterClass GR64RegClass;
  static const TargetRegisterClass * const GR64RegisterClass = &GR64RegClass;
  extern const TargetRegisterClass GR16RegClass;
  static const TargetRegisterClass * const GR16RegisterClass = &GR16RegClass;
  extern const TargetRegisterClass FR32RegClass;
  static const TargetRegisterClass * const FR32RegisterClass = &FR32RegClass;
  extern const TargetRegisterClass GR32RegClass;
  static const TargetRegisterClass * const GR32RegisterClass = &GR32RegClass;
  extern const TargetRegisterClass CONTROL_REGRegClass;
  static const TargetRegisterClass * const CONTROL_REGRegisterClass = &CONTROL_REGRegClass;
  extern const TargetRegisterClass FR64RegClass;
  static const TargetRegisterClass * const FR64RegisterClass = &FR64RegClass;
  extern const TargetRegisterClass GR64_with_sub_8bitRegClass;
  static const TargetRegisterClass * const GR64_with_sub_8bitRegisterClass = &GR64_with_sub_8bitRegClass;
  extern const TargetRegisterClass VR128RegClass;
  static const TargetRegisterClass * const VR128RegisterClass = &VR128RegClass;
  extern const TargetRegisterClass VR256RegClass;
  static const TargetRegisterClass * const VR256RegisterClass = &VR256RegClass;
  extern const TargetRegisterClass GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR32_NOAXRegisterClass = &GR32_NOAXRegClass;
  extern const TargetRegisterClass GR32_NOSPRegClass;
  static const TargetRegisterClass * const GR32_NOSPRegisterClass = &GR32_NOSPRegClass;
  extern const TargetRegisterClass GR64_NOSPRegClass;
  static const TargetRegisterClass * const GR64_NOSPRegisterClass = &GR64_NOSPRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_NOAXRegisterClass = &GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOSPRegClass;
  static const TargetRegisterClass * const GR32_NOAX_and_GR32_NOSPRegisterClass = &GR32_NOAX_and_GR32_NOSPRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegisterClass = &GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass;
  extern const TargetRegisterClass GR64_NOREXRegClass;
  static const TargetRegisterClass * const GR64_NOREXRegisterClass = &GR64_NOREXRegClass;
  extern const TargetRegisterClass GR64_TCRegClass;
  static const TargetRegisterClass * const GR64_TCRegisterClass = &GR64_TCRegClass;
  extern const TargetRegisterClass GR8_NOREXRegClass;
  static const TargetRegisterClass * const GR8_NOREXRegisterClass = &GR8_NOREXRegClass;
  extern const TargetRegisterClass GR16_NOREXRegClass;
  static const TargetRegisterClass * const GR16_NOREXRegisterClass = &GR16_NOREXRegClass;
  extern const TargetRegisterClass DEBUG_REGRegClass;
  static const TargetRegisterClass * const DEBUG_REGRegisterClass = &DEBUG_REGRegClass;
  extern const TargetRegisterClass GR32_NOREXRegClass;
  static const TargetRegisterClass * const GR32_NOREXRegisterClass = &GR32_NOREXRegClass;
  extern const TargetRegisterClass GR64_NOSP_and_GR64_TCRegClass;
  static const TargetRegisterClass * const GR64_NOSP_and_GR64_TCRegisterClass = &GR64_NOSP_and_GR64_TCRegClass;
  extern const TargetRegisterClass GR64_with_sub_16bit_in_GR16_NOREXRegClass;
  static const TargetRegisterClass * const GR64_with_sub_16bit_in_GR16_NOREXRegisterClass = &GR64_with_sub_16bit_in_GR16_NOREXRegClass;
  extern const TargetRegisterClass VR64RegClass;
  static const TargetRegisterClass * const VR64RegisterClass = &VR64RegClass;
  extern const TargetRegisterClass RSTRegClass;
  static const TargetRegisterClass * const RSTRegisterClass = &RSTRegClass;
  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOREXRegClass;
  static const TargetRegisterClass * const GR32_NOAX_and_GR32_NOREXRegisterClass = &GR32_NOAX_and_GR32_NOREXRegClass;
  extern const TargetRegisterClass GR32_NOREX_NOSPRegClass;
  static const TargetRegisterClass * const GR32_NOREX_NOSPRegisterClass = &GR32_NOREX_NOSPRegClass;
  extern const TargetRegisterClass RFP32RegClass;
  static const TargetRegisterClass * const RFP32RegisterClass = &RFP32RegClass;
  extern const TargetRegisterClass RFP64RegClass;
  static const TargetRegisterClass * const RFP64RegisterClass = &RFP64RegClass;
  extern const TargetRegisterClass GR64_NOREX_NOSPRegClass;
  static const TargetRegisterClass * const GR64_NOREX_NOSPRegisterClass = &GR64_NOREX_NOSPRegClass;
  extern const TargetRegisterClass GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegisterClass = &GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegisterClass = &GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass;
  extern const TargetRegisterClass RFP80RegClass;
  static const TargetRegisterClass * const RFP80RegisterClass = &RFP80RegClass;
  extern const TargetRegisterClass SEGMENT_REGRegClass;
  static const TargetRegisterClass * const SEGMENT_REGRegisterClass = &SEGMENT_REGRegClass;
  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOREX_NOSPRegClass;
  static const TargetRegisterClass * const GR32_NOAX_and_GR32_NOREX_NOSPRegisterClass = &GR32_NOAX_and_GR32_NOREX_NOSPRegClass;
  extern const TargetRegisterClass GR64_NOREX_and_GR64_TCRegClass;
  static const TargetRegisterClass * const GR64_NOREX_and_GR64_TCRegisterClass = &GR64_NOREX_and_GR64_TCRegClass;
  extern const TargetRegisterClass GR64_TCW64RegClass;
  static const TargetRegisterClass * const GR64_TCW64RegisterClass = &GR64_TCW64RegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegisterClass = &GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass;
  extern const TargetRegisterClass GR64_NOREX_NOSP_and_GR64_TCRegClass;
  static const TargetRegisterClass * const GR64_NOREX_NOSP_and_GR64_TCRegisterClass = &GR64_NOREX_NOSP_and_GR64_TCRegClass;
  extern const TargetRegisterClass GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegisterClass = &GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR8_ABCD_HRegClass;
  static const TargetRegisterClass * const GR8_ABCD_HRegisterClass = &GR8_ABCD_HRegClass;
  extern const TargetRegisterClass GR8_ABCD_LRegClass;
  static const TargetRegisterClass * const GR8_ABCD_LRegisterClass = &GR8_ABCD_LRegClass;
  extern const TargetRegisterClass GR16_ABCDRegClass;
  static const TargetRegisterClass * const GR16_ABCDRegisterClass = &GR16_ABCDRegClass;
  extern const TargetRegisterClass GR32_ABCDRegClass;
  static const TargetRegisterClass * const GR32_ABCDRegisterClass = &GR32_ABCDRegClass;
  extern const TargetRegisterClass GR64_ABCDRegClass;
  static const TargetRegisterClass * const GR64_ABCDRegisterClass = &GR64_ABCDRegClass;
  extern const TargetRegisterClass GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass;
  static const TargetRegisterClass * const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegisterClass = &GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass;
  extern const TargetRegisterClass GR32_ABCD_and_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR32_ABCD_and_GR32_NOAXRegisterClass = &GR32_ABCD_and_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR32_TCRegClass;
  static const TargetRegisterClass * const GR32_TCRegisterClass = &GR32_TCRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegisterClass = &GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_TCRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_TCRegisterClass = &GR64_with_sub_32bit_in_GR32_TCRegClass;
  extern const TargetRegisterClass GR32_ADRegClass;
  static const TargetRegisterClass * const GR32_ADRegisterClass = &GR32_ADRegClass;
  extern const TargetRegisterClass GR32_NOAX_and_GR32_TCRegClass;
  static const TargetRegisterClass * const GR32_NOAX_and_GR32_TCRegisterClass = &GR32_NOAX_and_GR32_TCRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_ADRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_ADRegisterClass = &GR64_with_sub_32bit_in_GR32_ADRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegisterClass = &GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClass;
  extern const TargetRegisterClass CCRRegClass;
  static const TargetRegisterClass * const CCRRegisterClass = &CCRRegClass;
  extern const TargetRegisterClass GR32_AD_and_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR32_AD_and_GR32_NOAXRegisterClass = &GR32_AD_and_GR32_NOAXRegClass;
  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClass;
  static const TargetRegisterClass * const GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegisterClass = &GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClass;
} // end of namespace X86

} // End llvm namespace 
#endif // GET_REGINFO_HEADER

//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// Target Register and Register Classes Information
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//


#ifdef GET_REGINFO_TARGET_DESC
#undef GET_REGINFO_TARGET_DESC
namespace llvm {

extern const MCRegisterClass X86MCRegisterClasses[];

static const MVT::SimpleValueType VTLists[] = {
  /* 0 */ MVT::i8, MVT::Other,
  /* 2 */ MVT::i16, MVT::Other,
  /* 4 */ MVT::i32, MVT::Other,
  /* 6 */ MVT::i64, MVT::Other,
  /* 8 */ MVT::f80, MVT::f64, MVT::f32, MVT::Other,
  /* 12 */ MVT::f64, MVT::Other,
  /* 14 */ MVT::f80, MVT::Other,
  /* 16 */ MVT::v16i8, MVT::v8i16, MVT::v4i32, MVT::v2i64, MVT::v4f32, MVT::v2f64, MVT::Other,
  /* 23 */ MVT::v32i8, MVT::v16i16, MVT::v8i32, MVT::v4i64, MVT::v8f32, MVT::v4f64, MVT::Other,
  /* 30 */ MVT::x86mmx, MVT::Other,
};

static const TargetRegisterClass *const NullRegClasses[] = { NULL };

// GR8 Super-register Classes...
static const TargetRegisterClass *const GR8SuperRegClasses[] = {
  &X86::GR64RegClass, &X86::GR16RegClass, &X86::GR32RegClass, &X86::GR32_NOAXRegClass, &X86::GR32_NOSPRegClass, &X86::GR64_NOSPRegClass, &X86::GR64_TCRegClass, &X86::GR32_TCRegClass, NULL
};

// GR64 Super-register Classes...
static const TargetRegisterClass *const GR64SuperRegClasses[] = {
  NULL
};

// GR16 Super-register Classes...
static const TargetRegisterClass *const GR16SuperRegClasses[] = {
  &X86::GR64RegClass, &X86::GR32RegClass, &X86::GR32_NOAXRegClass, &X86::GR32_NOSPRegClass, &X86::GR64_NOSPRegClass, &X86::GR64_TCRegClass, &X86::GR32_TCRegClass, NULL
};

// FR32 Super-register Classes...
static const TargetRegisterClass *const FR32SuperRegClasses[] = {
  &X86::VR128RegClass, &X86::VR256RegClass, NULL
};

// GR32 Super-register Classes...
static const TargetRegisterClass *const GR32SuperRegClasses[] = {
  &X86::GR64RegClass, NULL
};

// CONTROL_REG Super-register Classes...
static const TargetRegisterClass *const CONTROL_REGSuperRegClasses[] = {
  NULL
};

// FR64 Super-register Classes...
static const TargetRegisterClass *const FR64SuperRegClasses[] = {
  &X86::VR128RegClass, &X86::VR256RegClass, NULL
};

// GR64_with_sub_8bit Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_8bitSuperRegClasses[] = {
  NULL
};

// VR128 Super-register Classes...
static const TargetRegisterClass *const VR128SuperRegClasses[] = {
  &X86::VR256RegClass, NULL
};

// VR256 Super-register Classes...
static const TargetRegisterClass *const VR256SuperRegClasses[] = {
  NULL
};

// GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR32_NOSP Super-register Classes...
static const TargetRegisterClass *const GR32_NOSPSuperRegClasses[] = {
  &X86::GR64_NOSPRegClass, NULL
};

// GR64_NOSP Super-register Classes...
static const TargetRegisterClass *const GR64_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR32_NOAX_and_GR32_NOSP Super-register Classes...
static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_NOREX Super-register Classes...
static const TargetRegisterClass *const GR64_NOREXSuperRegClasses[] = {
  NULL
};

// GR64_TC Super-register Classes...
static const TargetRegisterClass *const GR64_TCSuperRegClasses[] = {
  NULL
};

// GR8_NOREX Super-register Classes...
static const TargetRegisterClass *const GR8_NOREXSuperRegClasses[] = {
  &X86::GR64_NOREXRegClass, &X86::GR16_NOREXRegClass, &X86::GR32_NOREXRegClass, &X86::GR32_NOREX_NOSPRegClass, &X86::GR64_NOREX_NOSPRegClass, NULL
};

// GR16_NOREX Super-register Classes...
static const TargetRegisterClass *const GR16_NOREXSuperRegClasses[] = {
  &X86::GR64_NOREXRegClass, &X86::GR32_NOREXRegClass, &X86::GR32_NOREX_NOSPRegClass, &X86::GR64_NOREX_NOSPRegClass, NULL
};

// DEBUG_REG Super-register Classes...
static const TargetRegisterClass *const DEBUG_REGSuperRegClasses[] = {
  NULL
};

// GR32_NOREX Super-register Classes...
static const TargetRegisterClass *const GR32_NOREXSuperRegClasses[] = {
  &X86::GR64_NOREXRegClass, NULL
};

// GR64_NOSP_and_GR64_TC Super-register Classes...
static const TargetRegisterClass *const GR64_NOSP_and_GR64_TCSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_16bit_in_GR16_NOREX Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_16bit_in_GR16_NOREXSuperRegClasses[] = {
  NULL
};

// VR64 Super-register Classes...
static const TargetRegisterClass *const VR64SuperRegClasses[] = {
  NULL
};

// RST Super-register Classes...
static const TargetRegisterClass *const RSTSuperRegClasses[] = {
  NULL
};

// GR32_NOAX_and_GR32_NOREX Super-register Classes...
static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOREXSuperRegClasses[] = {
  NULL
};

// GR32_NOREX_NOSP Super-register Classes...
static const TargetRegisterClass *const GR32_NOREX_NOSPSuperRegClasses[] = {
  &X86::GR64_NOREX_NOSPRegClass, NULL
};

// RFP32 Super-register Classes...
static const TargetRegisterClass *const RFP32SuperRegClasses[] = {
  NULL
};

// RFP64 Super-register Classes...
static const TargetRegisterClass *const RFP64SuperRegClasses[] = {
  NULL
};

// GR64_NOREX_NOSP Super-register Classes...
static const TargetRegisterClass *const GR64_NOREX_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperRegClasses[] = {
  NULL
};

// RFP80 Super-register Classes...
static const TargetRegisterClass *const RFP80SuperRegClasses[] = {
  NULL
};

// SEGMENT_REG Super-register Classes...
static const TargetRegisterClass *const SEGMENT_REGSuperRegClasses[] = {
  NULL
};

// GR32_NOAX_and_GR32_NOREX_NOSP Super-register Classes...
static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOREX_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_NOREX_and_GR64_TC Super-register Classes...
static const TargetRegisterClass *const GR64_NOREX_and_GR64_TCSuperRegClasses[] = {
  NULL
};

// GR64_TCW64 Super-register Classes...
static const TargetRegisterClass *const GR64_TCW64SuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSuperRegClasses[] = {
  NULL
};

// GR64_NOREX_NOSP_and_GR64_TC Super-register Classes...
static const TargetRegisterClass *const GR64_NOREX_NOSP_and_GR64_TCSuperRegClasses[] = {
  NULL
};

// GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR8_ABCD_H Super-register Classes...
static const TargetRegisterClass *const GR8_ABCD_HSuperRegClasses[] = {
  &X86::GR16_ABCDRegClass, &X86::GR32_ABCDRegClass, &X86::GR64_ABCDRegClass, &X86::GR32_ADRegClass, NULL
};

// GR8_ABCD_L Super-register Classes...
static const TargetRegisterClass *const GR8_ABCD_LSuperRegClasses[] = {
  &X86::GR16_ABCDRegClass, &X86::GR32_ABCDRegClass, &X86::GR64_ABCDRegClass, &X86::GR32_ADRegClass, NULL
};

// GR16_ABCD Super-register Classes...
static const TargetRegisterClass *const GR16_ABCDSuperRegClasses[] = {
  &X86::GR32_ABCDRegClass, &X86::GR64_ABCDRegClass, &X86::GR32_ADRegClass, NULL
};

// GR32_ABCD Super-register Classes...
static const TargetRegisterClass *const GR32_ABCDSuperRegClasses[] = {
  &X86::GR64_ABCDRegClass, NULL
};

// GR64_ABCD Super-register Classes...
static const TargetRegisterClass *const GR64_ABCDSuperRegClasses[] = {
  NULL
};

// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX Super-register Classes...
static const TargetRegisterClass *const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperRegClasses[] = {
  NULL
};

// GR32_ABCD_and_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR32_ABCD_and_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR32_TC Super-register Classes...
static const TargetRegisterClass *const GR32_TCSuperRegClasses[] = {
  &X86::GR64_TCRegClass, NULL
};

// GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_TC Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_TCSuperRegClasses[] = {
  NULL
};

// GR32_AD Super-register Classes...
static const TargetRegisterClass *const GR32_ADSuperRegClasses[] = {
  NULL
};

// GR32_NOAX_and_GR32_TC Super-register Classes...
static const TargetRegisterClass *const GR32_NOAX_and_GR32_TCSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_AD Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_ADSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSuperRegClasses[] = {
  NULL
};

// CCR Super-register Classes...
static const TargetRegisterClass *const CCRSuperRegClasses[] = {
  NULL
};

// GR32_AD_and_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR32_AD_and_GR32_NOAXSuperRegClasses[] = {
  NULL
};

// GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX Super-register Classes...
static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSuperRegClasses[] = {
  NULL
};

static const uint32_t GR8SubclassMask[] = {
  0x00040001, 0x00000600, 
};

static const uint32_t GR64SubclassMask[] = {
  0xc0c3b082, 0x026661f1, 
};

static const uint32_t GR16SubclassMask[] = {
  0x00080004, 0x00000800, 
};

static const uint32_t FR32SubclassMask[] = {
  0x00000148, 0x00000000, 
};

static const uint32_t GR32SubclassMask[] = {
  0x0c204c10, 0x01199008, 
};

static const uint32_t CONTROL_REGSubclassMask[] = {
  0x00000020, 0x00000000, 
};

static const uint32_t FR64SubclassMask[] = {
  0x00000140, 0x00000000, 
};

static const uint32_t GR64_with_sub_8bitSubclassMask[] = {
  0xc0c0b080, 0x026661e1, 
};

static const uint32_t VR128SubclassMask[] = {
  0x00000100, 0x00000000, 
};

static const uint32_t VR256SubclassMask[] = {
  0x00000200, 0x00000000, 
};

static const uint32_t GR32_NOAXSubclassMask[] = {
  0x04004400, 0x01108008, 
};

static const uint32_t GR32_NOSPSubclassMask[] = {
  0x08004800, 0x01199008, 
};

static const uint32_t GR64_NOSPSubclassMask[] = {
  0xc0409000, 0x026661e0, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_NOAXSubclassMask[] = {
  0x8000a000, 0x02424141, 
};

static const uint32_t GR32_NOAX_and_GR32_NOSPSubclassMask[] = {
  0x00004000, 0x01108008, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSubclassMask[] = {
  0x80008000, 0x02424140, 
};

static const uint32_t GR64_NOREXSubclassMask[] = {
  0x40810000, 0x026660d1, 
};

static const uint32_t GR64_TCSubclassMask[] = {
  0x80420000, 0x026441b0, 
};

static const uint32_t GR8_NOREXSubclassMask[] = {
  0x00040000, 0x00000600, 
};

static const uint32_t GR16_NOREXSubclassMask[] = {
  0x00080000, 0x00000800, 
};

static const uint32_t DEBUG_REGSubclassMask[] = {
  0x00100000, 0x00000000, 
};

static const uint32_t GR32_NOREXSubclassMask[] = {
  0x0c200000, 0x01199008, 
};

static const uint32_t GR64_NOSP_and_GR64_TCSubclassMask[] = {
  0x80400000, 0x026441a0, 
};

static const uint32_t GR64_with_sub_16bit_in_GR16_NOREXSubclassMask[] = {
  0x40800000, 0x026660c1, 
};

static const uint32_t VR64SubclassMask[] = {
  0x01000000, 0x00000000, 
};

static const uint32_t RSTSubclassMask[] = {
  0x02000000, 0x00000000, 
};

static const uint32_t GR32_NOAX_and_GR32_NOREXSubclassMask[] = {
  0x04000000, 0x01108008, 
};

static const uint32_t GR32_NOREX_NOSPSubclassMask[] = {
  0x08000000, 0x01199008, 
};

static const uint32_t RFP32SubclassMask[] = {
  0x30000000, 0x00000002, 
};

static const uint32_t RFP64SubclassMask[] = {
  0x20000000, 0x00000002, 
};

static const uint32_t GR64_NOREX_NOSPSubclassMask[] = {
  0x40000000, 0x026660c0, 
};

static const uint32_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSubclassMask[] = {
  0x80000000, 0x02404100, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSubclassMask[] = {
  0x00000000, 0x02424041, 
};

static const uint32_t RFP80SubclassMask[] = {
  0x00000000, 0x00000002, 
};

static const uint32_t SEGMENT_REGSubclassMask[] = {
  0x00000000, 0x00000004, 
};

static const uint32_t GR32_NOAX_and_GR32_NOREX_NOSPSubclassMask[] = {
  0x00000000, 0x01108008, 
};

static const uint32_t GR64_NOREX_and_GR64_TCSubclassMask[] = {
  0x00000000, 0x02644090, 
};

static const uint32_t GR64_TCW64SubclassMask[] = {
  0x00000000, 0x02640120, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSubclassMask[] = {
  0x00000000, 0x02424040, 
};

static const uint32_t GR64_NOREX_NOSP_and_GR64_TCSubclassMask[] = {
  0x00000000, 0x02644080, 
};

static const uint32_t GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSubclassMask[] = {
  0x00000000, 0x02400100, 
};

static const uint32_t GR8_ABCD_HSubclassMask[] = {
  0x00000000, 0x00000200, 
};

static const uint32_t GR8_ABCD_LSubclassMask[] = {
  0x00000000, 0x00000400, 
};

static const uint32_t GR16_ABCDSubclassMask[] = {
  0x00000000, 0x00000800, 
};

static const uint32_t GR32_ABCDSubclassMask[] = {
  0x00000000, 0x01199000, 
};

static const uint32_t GR64_ABCDSubclassMask[] = {
  0x00000000, 0x02662000, 
};

static const uint32_t GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSubclassMask[] = {
  0x00000000, 0x02404000, 
};

static const uint32_t GR32_ABCD_and_GR32_NOAXSubclassMask[] = {
  0x00000000, 0x01108000, 
};

static const uint32_t GR32_TCSubclassMask[] = {
  0x00000000, 0x01190000, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSubclassMask[] = {
  0x00000000, 0x02420000, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_TCSubclassMask[] = {
  0x00000000, 0x02640000, 
};

static const uint32_t GR32_ADSubclassMask[] = {
  0x00000000, 0x01080000, 
};

static const uint32_t GR32_NOAX_and_GR32_TCSubclassMask[] = {
  0x00000000, 0x01100000, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_ADSubclassMask[] = {
  0x00000000, 0x02200000, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSubclassMask[] = {
  0x00000000, 0x02400000, 
};

static const uint32_t CCRSubclassMask[] = {
  0x00000000, 0x00800000, 
};

static const uint32_t GR32_AD_and_GR32_NOAXSubclassMask[] = {
  0x00000000, 0x01000000, 
};

static const uint32_t GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSubclassMask[] = {
  0x00000000, 0x02000000, 
};

static const TargetRegisterClass *const FR64Superclasses[] = {
  &X86::FR32RegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_8bitSuperclasses[] = {
  &X86::GR64RegClass,
  NULL
};

static const TargetRegisterClass *const VR128Superclasses[] = {
  &X86::FR32RegClass,
  &X86::FR64RegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOAXSuperclasses[] = {
  &X86::GR32RegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOSPSuperclasses[] = {
  &X86::GR32RegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOSPSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOSPSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOSPRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOREXSuperclasses[] = {
  &X86::GR64RegClass,
  NULL
};

static const TargetRegisterClass *const GR64_TCSuperclasses[] = {
  &X86::GR64RegClass,
  NULL
};

static const TargetRegisterClass *const GR8_NOREXSuperclasses[] = {
  &X86::GR8RegClass,
  NULL
};

static const TargetRegisterClass *const GR16_NOREXSuperclasses[] = {
  &X86::GR16RegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOREXSuperclasses[] = {
  &X86::GR32RegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOSP_and_GR64_TCSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_16bit_in_GR16_NOREXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOREXSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOREX_NOSPSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const RFP64Superclasses[] = {
  &X86::RFP32RegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOREX_NOSPSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const RFP80Superclasses[] = {
  &X86::RFP32RegClass,
  &X86::RFP64RegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOAX_and_GR32_NOREX_NOSPSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOREX_and_GR64_TCSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_TCW64Superclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_NOREX_NOSP_and_GR64_TCSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_TCW64RegClass,
  NULL
};

static const TargetRegisterClass *const GR8_ABCD_HSuperclasses[] = {
  &X86::GR8RegClass,
  &X86::GR8_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR8_ABCD_LSuperclasses[] = {
  &X86::GR8RegClass,
  &X86::GR8_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR16_ABCDSuperclasses[] = {
  &X86::GR16RegClass,
  &X86::GR16_NOREXRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_ABCDSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_ABCDSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_ABCD_and_GR32_NOAXSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR32_ABCDRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_TCSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  &X86::GR32_ABCDRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR64_ABCDRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_TCSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  &X86::GR64_TCW64RegClass,
  &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
  &X86::GR64_ABCDRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_ADSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  &X86::GR32_ABCDRegClass,
  &X86::GR32_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_NOAX_and_GR32_TCSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR32_ABCDRegClass,
  &X86::GR32_ABCD_and_GR32_NOAXRegClass,
  &X86::GR32_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_ADSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  &X86::GR64_TCW64RegClass,
  &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
  &X86::GR64_ABCDRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  &X86::GR64_TCW64RegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
  &X86::GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_ABCDRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR32_AD_and_GR32_NOAXSuperclasses[] = {
  &X86::GR32RegClass,
  &X86::GR32_NOAXRegClass,
  &X86::GR32_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR32_NOREXRegClass,
  &X86::GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR32_NOREX_NOSPRegClass,
  &X86::GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR32_ABCDRegClass,
  &X86::GR32_ABCD_and_GR32_NOAXRegClass,
  &X86::GR32_TCRegClass,
  &X86::GR32_ADRegClass,
  &X86::GR32_NOAX_and_GR32_TCRegClass,
  NULL
};

static const TargetRegisterClass *const GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSuperclasses[] = {
  &X86::GR64RegClass,
  &X86::GR64_with_sub_8bitRegClass,
  &X86::GR64_NOSPRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
  &X86::GR64_NOREXRegClass,
  &X86::GR64_TCRegClass,
  &X86::GR64_NOSP_and_GR64_TCRegClass,
  &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
  &X86::GR64_NOREX_NOSPRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_NOREX_and_GR64_TCRegClass,
  &X86::GR64_TCW64RegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
  &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
  &X86::GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
  &X86::GR64_ABCDRegClass,
  &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_TCRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_ADRegClass,
  &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClass,
  NULL
};


static inline unsigned GR8AltOrderSelect(const MachineFunction &MF) {
    return MF.getTarget().getSubtarget<X86Subtarget>().is64Bit();
  }

static ArrayRef<uint16_t> GR8GetRawAllocationOrder(const MachineFunction &MF) {
  static const uint16_t AltOrder1[] = { X86::AL, X86::CL, X86::DL, X86::BL, X86::SIL, X86::DIL, X86::BPL, X86::SPL, X86::R8B, X86::R9B, X86::R10B, X86::R11B, X86::R14B, X86::R15B, X86::R12B, X86::R13B };
  const MCRegisterClass &MCR = X86MCRegisterClasses[X86::GR8RegClassID];
  const ArrayRef<uint16_t> Order[] = {
    makeArrayRef(MCR.begin(), MCR.getNumRegs()),
    makeArrayRef(AltOrder1)
  };
  const unsigned Select = GR8AltOrderSelect(MF);
  assert(Select < 2);
  return Order[Select];
}

static inline unsigned GR8_NOREXAltOrderSelect(const MachineFunction &MF) {
    return MF.getTarget().getSubtarget<X86Subtarget>().is64Bit();
  }

static ArrayRef<uint16_t> GR8_NOREXGetRawAllocationOrder(const MachineFunction &MF) {
  static const uint16_t AltOrder1[] = { X86::AL, X86::CL, X86::DL, X86::BL };
  const MCRegisterClass &MCR = X86MCRegisterClasses[X86::GR8_NOREXRegClassID];
  const ArrayRef<uint16_t> Order[] = {
    makeArrayRef(MCR.begin(), MCR.getNumRegs()),
    makeArrayRef(AltOrder1)
  };
  const unsigned Select = GR8_NOREXAltOrderSelect(MF);
  assert(Select < 2);
  return Order[Select];
}
namespace X86 {   // Register class instances
  extern const TargetRegisterClass GR8RegClass = {
    &X86MCRegisterClasses[GR8RegClassID],
    VTLists + 0,
    GR8SubclassMask,
    NullRegClasses,
    GR8SuperRegClasses,
    GR8GetRawAllocationOrder
  };

  extern const TargetRegisterClass GR64RegClass = {
    &X86MCRegisterClasses[GR64RegClassID],
    VTLists + 6,
    GR64SubclassMask,
    NullRegClasses,
    GR64SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR16RegClass = {
    &X86MCRegisterClasses[GR16RegClassID],
    VTLists + 2,
    GR16SubclassMask,
    NullRegClasses,
    GR16SuperRegClasses,
    0
  };

  extern const TargetRegisterClass FR32RegClass = {
    &X86MCRegisterClasses[FR32RegClassID],
    VTLists + 10,
    FR32SubclassMask,
    NullRegClasses,
    FR32SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32RegClass = {
    &X86MCRegisterClasses[GR32RegClassID],
    VTLists + 4,
    GR32SubclassMask,
    NullRegClasses,
    GR32SuperRegClasses,
    0
  };

  extern const TargetRegisterClass CONTROL_REGRegClass = {
    &X86MCRegisterClasses[CONTROL_REGRegClassID],
    VTLists + 6,
    CONTROL_REGSubclassMask,
    NullRegClasses,
    CONTROL_REGSuperRegClasses,
    0
  };

  extern const TargetRegisterClass FR64RegClass = {
    &X86MCRegisterClasses[FR64RegClassID],
    VTLists + 12,
    FR64SubclassMask,
    FR64Superclasses,
    FR64SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_8bitRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_8bitRegClassID],
    VTLists + 6,
    GR64_with_sub_8bitSubclassMask,
    GR64_with_sub_8bitSuperclasses,
    GR64_with_sub_8bitSuperRegClasses,
    0
  };

  extern const TargetRegisterClass VR128RegClass = {
    &X86MCRegisterClasses[VR128RegClassID],
    VTLists + 16,
    VR128SubclassMask,
    VR128Superclasses,
    VR128SuperRegClasses,
    0
  };

  extern const TargetRegisterClass VR256RegClass = {
    &X86MCRegisterClasses[VR256RegClassID],
    VTLists + 23,
    VR256SubclassMask,
    NullRegClasses,
    VR256SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR32_NOAXRegClassID],
    VTLists + 4,
    GR32_NOAXSubclassMask,
    GR32_NOAXSuperclasses,
    GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOSPRegClass = {
    &X86MCRegisterClasses[GR32_NOSPRegClassID],
    VTLists + 4,
    GR32_NOSPSubclassMask,
    GR32_NOSPSuperclasses,
    GR32_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOSPRegClass = {
    &X86MCRegisterClasses[GR64_NOSPRegClassID],
    VTLists + 6,
    GR64_NOSPSubclassMask,
    GR64_NOSPSuperclasses,
    GR64_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_NOAXRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_NOAXSubclassMask,
    GR64_with_sub_32bit_in_GR32_NOAXSuperclasses,
    GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOSPRegClass = {
    &X86MCRegisterClasses[GR32_NOAX_and_GR32_NOSPRegClassID],
    VTLists + 4,
    GR32_NOAX_and_GR32_NOSPSubclassMask,
    GR32_NOAX_and_GR32_NOSPSuperclasses,
    GR32_NOAX_and_GR32_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSubclassMask,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSuperclasses,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOREXRegClass = {
    &X86MCRegisterClasses[GR64_NOREXRegClassID],
    VTLists + 6,
    GR64_NOREXSubclassMask,
    GR64_NOREXSuperclasses,
    GR64_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_TCRegClass = {
    &X86MCRegisterClasses[GR64_TCRegClassID],
    VTLists + 6,
    GR64_TCSubclassMask,
    GR64_TCSuperclasses,
    GR64_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR8_NOREXRegClass = {
    &X86MCRegisterClasses[GR8_NOREXRegClassID],
    VTLists + 0,
    GR8_NOREXSubclassMask,
    GR8_NOREXSuperclasses,
    GR8_NOREXSuperRegClasses,
    GR8_NOREXGetRawAllocationOrder
  };

  extern const TargetRegisterClass GR16_NOREXRegClass = {
    &X86MCRegisterClasses[GR16_NOREXRegClassID],
    VTLists + 2,
    GR16_NOREXSubclassMask,
    GR16_NOREXSuperclasses,
    GR16_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass DEBUG_REGRegClass = {
    &X86MCRegisterClasses[DEBUG_REGRegClassID],
    VTLists + 4,
    DEBUG_REGSubclassMask,
    NullRegClasses,
    DEBUG_REGSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOREXRegClass = {
    &X86MCRegisterClasses[GR32_NOREXRegClassID],
    VTLists + 4,
    GR32_NOREXSubclassMask,
    GR32_NOREXSuperclasses,
    GR32_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOSP_and_GR64_TCRegClass = {
    &X86MCRegisterClasses[GR64_NOSP_and_GR64_TCRegClassID],
    VTLists + 6,
    GR64_NOSP_and_GR64_TCSubclassMask,
    GR64_NOSP_and_GR64_TCSuperclasses,
    GR64_NOSP_and_GR64_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_16bit_in_GR16_NOREXRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_16bit_in_GR16_NOREXRegClassID],
    VTLists + 6,
    GR64_with_sub_16bit_in_GR16_NOREXSubclassMask,
    GR64_with_sub_16bit_in_GR16_NOREXSuperclasses,
    GR64_with_sub_16bit_in_GR16_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass VR64RegClass = {
    &X86MCRegisterClasses[VR64RegClassID],
    VTLists + 30,
    VR64SubclassMask,
    NullRegClasses,
    VR64SuperRegClasses,
    0
  };

  extern const TargetRegisterClass RSTRegClass = {
    &X86MCRegisterClasses[RSTRegClassID],
    VTLists + 8,
    RSTSubclassMask,
    NullRegClasses,
    RSTSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOREXRegClass = {
    &X86MCRegisterClasses[GR32_NOAX_and_GR32_NOREXRegClassID],
    VTLists + 4,
    GR32_NOAX_and_GR32_NOREXSubclassMask,
    GR32_NOAX_and_GR32_NOREXSuperclasses,
    GR32_NOAX_and_GR32_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOREX_NOSPRegClass = {
    &X86MCRegisterClasses[GR32_NOREX_NOSPRegClassID],
    VTLists + 4,
    GR32_NOREX_NOSPSubclassMask,
    GR32_NOREX_NOSPSuperclasses,
    GR32_NOREX_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass RFP32RegClass = {
    &X86MCRegisterClasses[RFP32RegClassID],
    VTLists + 10,
    RFP32SubclassMask,
    NullRegClasses,
    RFP32SuperRegClasses,
    0
  };

  extern const TargetRegisterClass RFP64RegClass = {
    &X86MCRegisterClasses[RFP64RegClassID],
    VTLists + 12,
    RFP64SubclassMask,
    RFP64Superclasses,
    RFP64SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOREX_NOSPRegClass = {
    &X86MCRegisterClasses[GR64_NOREX_NOSPRegClassID],
    VTLists + 6,
    GR64_NOREX_NOSPSubclassMask,
    GR64_NOREX_NOSPSuperclasses,
    GR64_NOREX_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID],
    VTLists + 6,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSubclassMask,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSuperclasses,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSubclassMask,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperclasses,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass RFP80RegClass = {
    &X86MCRegisterClasses[RFP80RegClassID],
    VTLists + 14,
    RFP80SubclassMask,
    RFP80Superclasses,
    RFP80SuperRegClasses,
    0
  };

  extern const TargetRegisterClass SEGMENT_REGRegClass = {
    &X86MCRegisterClasses[SEGMENT_REGRegClassID],
    VTLists + 2,
    SEGMENT_REGSubclassMask,
    NullRegClasses,
    SEGMENT_REGSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOAX_and_GR32_NOREX_NOSPRegClass = {
    &X86MCRegisterClasses[GR32_NOAX_and_GR32_NOREX_NOSPRegClassID],
    VTLists + 4,
    GR32_NOAX_and_GR32_NOREX_NOSPSubclassMask,
    GR32_NOAX_and_GR32_NOREX_NOSPSuperclasses,
    GR32_NOAX_and_GR32_NOREX_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOREX_and_GR64_TCRegClass = {
    &X86MCRegisterClasses[GR64_NOREX_and_GR64_TCRegClassID],
    VTLists + 6,
    GR64_NOREX_and_GR64_TCSubclassMask,
    GR64_NOREX_and_GR64_TCSuperclasses,
    GR64_NOREX_and_GR64_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_TCW64RegClass = {
    &X86MCRegisterClasses[GR64_TCW64RegClassID],
    VTLists + 6,
    GR64_TCW64SubclassMask,
    GR64_TCW64Superclasses,
    GR64_TCW64SuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSubclassMask,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSuperclasses,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_NOREX_NOSP_and_GR64_TCRegClass = {
    &X86MCRegisterClasses[GR64_NOREX_NOSP_and_GR64_TCRegClassID],
    VTLists + 6,
    GR64_NOREX_NOSP_and_GR64_TCSubclassMask,
    GR64_NOREX_NOSP_and_GR64_TCSuperclasses,
    GR64_NOREX_NOSP_and_GR64_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClassID],
    VTLists + 6,
    GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSubclassMask,
    GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSuperclasses,
    GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR8_ABCD_HRegClass = {
    &X86MCRegisterClasses[GR8_ABCD_HRegClassID],
    VTLists + 0,
    GR8_ABCD_HSubclassMask,
    GR8_ABCD_HSuperclasses,
    GR8_ABCD_HSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR8_ABCD_LRegClass = {
    &X86MCRegisterClasses[GR8_ABCD_LRegClassID],
    VTLists + 0,
    GR8_ABCD_LSubclassMask,
    GR8_ABCD_LSuperclasses,
    GR8_ABCD_LSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR16_ABCDRegClass = {
    &X86MCRegisterClasses[GR16_ABCDRegClassID],
    VTLists + 2,
    GR16_ABCDSubclassMask,
    GR16_ABCDSuperclasses,
    GR16_ABCDSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_ABCDRegClass = {
    &X86MCRegisterClasses[GR32_ABCDRegClassID],
    VTLists + 4,
    GR32_ABCDSubclassMask,
    GR32_ABCDSuperclasses,
    GR32_ABCDSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_ABCDRegClass = {
    &X86MCRegisterClasses[GR64_ABCDRegClassID],
    VTLists + 6,
    GR64_ABCDSubclassMask,
    GR64_ABCDSuperclasses,
    GR64_ABCDSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass = {
    &X86MCRegisterClasses[GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClassID],
    VTLists + 6,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSubclassMask,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperclasses,
    GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_ABCD_and_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR32_ABCD_and_GR32_NOAXRegClassID],
    VTLists + 4,
    GR32_ABCD_and_GR32_NOAXSubclassMask,
    GR32_ABCD_and_GR32_NOAXSuperclasses,
    GR32_ABCD_and_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_TCRegClass = {
    &X86MCRegisterClasses[GR32_TCRegClassID],
    VTLists + 4,
    GR32_TCSubclassMask,
    GR32_TCSuperclasses,
    GR32_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSubclassMask,
    GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSuperclasses,
    GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_TCRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_TCRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_TCSubclassMask,
    GR64_with_sub_32bit_in_GR32_TCSuperclasses,
    GR64_with_sub_32bit_in_GR32_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_ADRegClass = {
    &X86MCRegisterClasses[GR32_ADRegClassID],
    VTLists + 4,
    GR32_ADSubclassMask,
    GR32_ADSuperclasses,
    GR32_ADSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_NOAX_and_GR32_TCRegClass = {
    &X86MCRegisterClasses[GR32_NOAX_and_GR32_TCRegClassID],
    VTLists + 4,
    GR32_NOAX_and_GR32_TCSubclassMask,
    GR32_NOAX_and_GR32_TCSuperclasses,
    GR32_NOAX_and_GR32_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_ADRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_ADRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_ADSubclassMask,
    GR64_with_sub_32bit_in_GR32_ADSuperclasses,
    GR64_with_sub_32bit_in_GR32_ADSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSubclassMask,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSuperclasses,
    GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCSuperRegClasses,
    0
  };

  extern const TargetRegisterClass CCRRegClass = {
    &X86MCRegisterClasses[CCRRegClassID],
    VTLists + 4,
    CCRSubclassMask,
    NullRegClasses,
    CCRSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR32_AD_and_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR32_AD_and_GR32_NOAXRegClassID],
    VTLists + 4,
    GR32_AD_and_GR32_NOAXSubclassMask,
    GR32_AD_and_GR32_NOAXSuperclasses,
    GR32_AD_and_GR32_NOAXSuperRegClasses,
    0
  };

  extern const TargetRegisterClass GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClass = {
    &X86MCRegisterClasses[GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClassID],
    VTLists + 6,
    GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSubclassMask,
    GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSuperclasses,
    GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXSuperRegClasses,
    0
  };

}

namespace {
  const TargetRegisterClass* const RegisterClasses[] = {
    &X86::GR8RegClass,
    &X86::GR64RegClass,
    &X86::GR16RegClass,
    &X86::FR32RegClass,
    &X86::GR32RegClass,
    &X86::CONTROL_REGRegClass,
    &X86::FR64RegClass,
    &X86::GR64_with_sub_8bitRegClass,
    &X86::VR128RegClass,
    &X86::VR256RegClass,
    &X86::GR32_NOAXRegClass,
    &X86::GR32_NOSPRegClass,
    &X86::GR64_NOSPRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_NOAXRegClass,
    &X86::GR32_NOAX_and_GR32_NOSPRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSPRegClass,
    &X86::GR64_NOREXRegClass,
    &X86::GR64_TCRegClass,
    &X86::GR8_NOREXRegClass,
    &X86::GR16_NOREXRegClass,
    &X86::DEBUG_REGRegClass,
    &X86::GR32_NOREXRegClass,
    &X86::GR64_NOSP_and_GR64_TCRegClass,
    &X86::GR64_with_sub_16bit_in_GR16_NOREXRegClass,
    &X86::VR64RegClass,
    &X86::RSTRegClass,
    &X86::GR32_NOAX_and_GR32_NOREXRegClass,
    &X86::GR32_NOREX_NOSPRegClass,
    &X86::RFP32RegClass,
    &X86::RFP64RegClass,
    &X86::GR64_NOREX_NOSPRegClass,
    &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
    &X86::RFP80RegClass,
    &X86::SEGMENT_REGRegClass,
    &X86::GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
    &X86::GR64_NOREX_and_GR64_TCRegClass,
    &X86::GR64_TCW64RegClass,
    &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSPRegClass,
    &X86::GR64_NOREX_NOSP_and_GR64_TCRegClass,
    &X86::GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAXRegClass,
    &X86::GR8_ABCD_HRegClass,
    &X86::GR8_ABCD_LRegClass,
    &X86::GR16_ABCDRegClass,
    &X86::GR32_ABCDRegClass,
    &X86::GR64_ABCDRegClass,
    &X86::GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREXRegClass,
    &X86::GR32_ABCD_and_GR32_NOAXRegClass,
    &X86::GR32_TCRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAXRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_TCRegClass,
    &X86::GR32_ADRegClass,
    &X86::GR32_NOAX_and_GR32_TCRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_ADRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TCRegClass,
    &X86::CCRRegClass,
    &X86::GR32_AD_and_GR32_NOAXRegClass,
    &X86::GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAXRegClass,
  };
}

static const TargetRegisterInfoDesc X86RegInfoDesc[] = { // Extra Descriptors
  { 0, 0 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 0 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 0, 0 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 0, 1 },
  { 1, 1 },
  { 0, 1 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 1, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
  { 0, 1 },
};

static const char *const X86SubRegIndexTable[] = { "sub_8bit", "sub_8bit_hi", "sub_16bit", "sub_32bit", "sub_sd", "sub_ss", "sub_xmm" };


unsigned X86GenRegisterInfo::composeSubRegIndices(unsigned IdxA, unsigned IdxB) const {
  switch (IdxA) {
  default:
    return IdxB;
  }
}

const TargetRegisterClass *X86GenRegisterInfo::getSubClassWithSubReg(const TargetRegisterClass *RC, unsigned Idx) const {
  static const uint8_t Table[58][7] = {
    {	// GR8
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64
      8,	// sub_8bit -> GR64_with_sub_8bit
      46,	// sub_8bit_hi -> GR64_ABCD
      2,	// sub_16bit -> GR64
      2,	// sub_32bit -> GR64
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR16
      3,	// sub_8bit -> GR16
      44,	// sub_8bit_hi -> GR16_ABCD
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// FR32
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      4,	// sub_sd -> FR32
      4,	// sub_ss -> FR32
      0,	// sub_xmm
    },
    {	// GR32
      5,	// sub_8bit -> GR32
      45,	// sub_8bit_hi -> GR32_ABCD
      5,	// sub_16bit -> GR32
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// CONTROL_REG
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// FR64
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      7,	// sub_sd -> FR64
      7,	// sub_ss -> FR64
      0,	// sub_xmm
    },
    {	// GR64_with_sub_8bit
      8,	// sub_8bit -> GR64_with_sub_8bit
      46,	// sub_8bit_hi -> GR64_ABCD
      8,	// sub_16bit -> GR64_with_sub_8bit
      8,	// sub_32bit -> GR64_with_sub_8bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// VR128
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      9,	// sub_sd -> VR128
      9,	// sub_ss -> VR128
      0,	// sub_xmm
    },
    {	// VR256
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      10,	// sub_sd -> VR256
      10,	// sub_ss -> VR256
      10,	// sub_xmm -> VR256
    },
    {	// GR32_NOAX
      11,	// sub_8bit -> GR32_NOAX
      48,	// sub_8bit_hi -> GR32_ABCD_and_GR32_NOAX
      11,	// sub_16bit -> GR32_NOAX
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOSP
      12,	// sub_8bit -> GR32_NOSP
      45,	// sub_8bit_hi -> GR32_ABCD
      12,	// sub_16bit -> GR32_NOSP
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOSP
      13,	// sub_8bit -> GR64_NOSP
      46,	// sub_8bit_hi -> GR64_ABCD
      13,	// sub_16bit -> GR64_NOSP
      13,	// sub_32bit -> GR64_NOSP
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX
      14,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_NOAX
      50,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      14,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_NOAX
      14,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_NOAX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOSP
      15,	// sub_8bit -> GR32_NOAX_and_GR32_NOSP
      48,	// sub_8bit_hi -> GR32_ABCD_and_GR32_NOAX
      15,	// sub_16bit -> GR32_NOAX_and_GR32_NOSP
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
      16,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
      50,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      16,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
      16,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOREX
      24,	// sub_8bit -> GR64_with_sub_16bit_in_GR16_NOREX
      46,	// sub_8bit_hi -> GR64_ABCD
      17,	// sub_16bit -> GR64_NOREX
      17,	// sub_32bit -> GR64_NOREX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_TC
      23,	// sub_8bit -> GR64_NOSP_and_GR64_TC
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      18,	// sub_16bit -> GR64_TC
      18,	// sub_32bit -> GR64_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR8_NOREX
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR16_NOREX
      20,	// sub_8bit -> GR16_NOREX
      44,	// sub_8bit_hi -> GR16_ABCD
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// DEBUG_REG
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOREX
      22,	// sub_8bit -> GR32_NOREX
      45,	// sub_8bit_hi -> GR32_ABCD
      22,	// sub_16bit -> GR32_NOREX
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOSP_and_GR64_TC
      23,	// sub_8bit -> GR64_NOSP_and_GR64_TC
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      23,	// sub_16bit -> GR64_NOSP_and_GR64_TC
      23,	// sub_32bit -> GR64_NOSP_and_GR64_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_16bit_in_GR16_NOREX
      24,	// sub_8bit -> GR64_with_sub_16bit_in_GR16_NOREX
      46,	// sub_8bit_hi -> GR64_ABCD
      24,	// sub_16bit -> GR64_with_sub_16bit_in_GR16_NOREX
      24,	// sub_32bit -> GR64_with_sub_16bit_in_GR16_NOREX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// VR64
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// RST
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOREX
      27,	// sub_8bit -> GR32_NOAX_and_GR32_NOREX
      48,	// sub_8bit_hi -> GR32_ABCD_and_GR32_NOAX
      27,	// sub_16bit -> GR32_NOAX_and_GR32_NOREX
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOREX_NOSP
      28,	// sub_8bit -> GR32_NOREX_NOSP
      45,	// sub_8bit_hi -> GR32_ABCD
      28,	// sub_16bit -> GR32_NOREX_NOSP
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// RFP32
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// RFP64
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOREX_NOSP
      31,	// sub_8bit -> GR64_NOREX_NOSP
      46,	// sub_8bit_hi -> GR64_ABCD
      31,	// sub_16bit -> GR64_NOREX_NOSP
      31,	// sub_32bit -> GR64_NOREX_NOSP
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
      32,	// sub_8bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
      55,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      32,	// sub_16bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
      32,	// sub_32bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      33,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      50,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      33,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      33,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// RFP80
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// SEGMENT_REG
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOREX_NOSP
      36,	// sub_8bit -> GR32_NOAX_and_GR32_NOREX_NOSP
      48,	// sub_8bit_hi -> GR32_ABCD_and_GR32_NOAX
      36,	// sub_16bit -> GR32_NOAX_and_GR32_NOREX_NOSP
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOREX_and_GR64_TC
      40,	// sub_8bit -> GR64_NOREX_NOSP_and_GR64_TC
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      37,	// sub_16bit -> GR64_NOREX_and_GR64_TC
      37,	// sub_32bit -> GR64_NOREX_and_GR64_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_TCW64
      38,	// sub_8bit -> GR64_TCW64
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      38,	// sub_16bit -> GR64_TCW64
      38,	// sub_32bit -> GR64_TCW64
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
      39,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
      50,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      39,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
      39,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_NOREX_NOSP_and_GR64_TC
      40,	// sub_8bit -> GR64_NOREX_NOSP_and_GR64_TC
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      40,	// sub_16bit -> GR64_NOREX_NOSP_and_GR64_TC
      40,	// sub_32bit -> GR64_NOREX_NOSP_and_GR64_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
      41,	// sub_8bit -> GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
      55,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      41,	// sub_16bit -> GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
      41,	// sub_32bit -> GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR8_ABCD_H
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR8_ABCD_L
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR16_ABCD
      44,	// sub_8bit -> GR16_ABCD
      44,	// sub_8bit_hi -> GR16_ABCD
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_ABCD
      45,	// sub_8bit -> GR32_ABCD
      45,	// sub_8bit_hi -> GR32_ABCD
      45,	// sub_16bit -> GR32_ABCD
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_ABCD
      46,	// sub_8bit -> GR64_ABCD
      46,	// sub_8bit_hi -> GR64_ABCD
      46,	// sub_16bit -> GR64_ABCD
      46,	// sub_32bit -> GR64_ABCD
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      47,	// sub_8bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      55,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      47,	// sub_16bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      47,	// sub_32bit -> GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_ABCD_and_GR32_NOAX
      48,	// sub_8bit -> GR32_ABCD_and_GR32_NOAX
      48,	// sub_8bit_hi -> GR32_ABCD_and_GR32_NOAX
      48,	// sub_16bit -> GR32_ABCD_and_GR32_NOAX
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_TC
      49,	// sub_8bit -> GR32_TC
      49,	// sub_8bit_hi -> GR32_TC
      49,	// sub_16bit -> GR32_TC
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      50,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      50,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      50,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      50,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_TC
      51,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_TC
      51,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_TC
      51,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_TC
      51,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_AD
      52,	// sub_8bit -> GR32_AD
      52,	// sub_8bit_hi -> GR32_AD
      52,	// sub_16bit -> GR32_AD
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_TC
      53,	// sub_8bit -> GR32_NOAX_and_GR32_TC
      53,	// sub_8bit_hi -> GR32_NOAX_and_GR32_TC
      53,	// sub_16bit -> GR32_NOAX_and_GR32_TC
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_AD
      54,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_AD
      54,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_AD
      54,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_AD
      54,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_AD
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      55,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      55,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      55,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      55,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// CCR
      0,	// sub_8bit
      0,	// sub_8bit_hi
      0,	// sub_16bit
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR32_AD_and_GR32_NOAX
      57,	// sub_8bit -> GR32_AD_and_GR32_NOAX
      57,	// sub_8bit_hi -> GR32_AD_and_GR32_NOAX
      57,	// sub_16bit -> GR32_AD_and_GR32_NOAX
      0,	// sub_32bit
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      58,	// sub_8bit -> GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      58,	// sub_8bit_hi -> GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      58,	// sub_16bit -> GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      58,	// sub_32bit -> GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      0,	// sub_sd
      0,	// sub_ss
      0,	// sub_xmm
    },
  };
  assert(RC && "Missing regclass");
  if (!Idx) return RC;
  --Idx;
  assert(Idx < 7 && "Bad subreg");
  unsigned TV = Table[RC->getID()][Idx];
  return TV ? getRegClass(TV - 1) : 0;
}

const TargetRegisterClass *X86GenRegisterInfo::getMatchingSuperRegClass(const TargetRegisterClass *A, const TargetRegisterClass *B, unsigned Idx) const {
  static const uint32_t Table[58][7][2] = {
    {	// GR8
      { 0xcce8fc94, 0x037ff9e9, },	// sub_8bit
      { 0x00000000, 0x037fb800, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR16
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0xcce0fc90, 0x037ff1e9, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// FR32
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000348, 0x00000000, },	// sub_sd
      { 0x00000348, 0x00000000, },	// sub_ss
      { 0x00000200, 0x00000000, },	// sub_xmm
    },
    {	// GR32
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0xc0c0b080, 0x026661e1, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// CONTROL_REG
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// FR64
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000348, 0x00000000, },	// sub_sd
      { 0x00000348, 0x00000000, },	// sub_ss
      { 0x00000200, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// VR128
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000348, 0x00000000, },	// sub_sd
      { 0x00000348, 0x00000000, },	// sub_ss
      { 0x00000200, 0x00000000, },	// sub_xmm
    },
    {	// VR256
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x8000a000, 0x02424141, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0xc0409000, 0x026661e0, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x80008000, 0x02424140, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR8_NOREX
      { 0x00000000, 0x037fb800, },	// sub_8bit
      { 0x00000000, 0x037fb800, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR16_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x4ca00000, 0x037ff0c9, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// DEBUG_REG
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x40800000, 0x026660c1, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOSP_and_GR64_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_16bit_in_GR16_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// VR64
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// RST
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02424041, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOREX_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x40000000, 0x026660c0, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// RFP32
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// RFP64
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOREX_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// RFP80
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// SEGMENT_REG
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_NOREX_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02424040, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOREX_and_GR64_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_TCW64
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_NOREX_NOSP_and_GR64_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR8_ABCD_H
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x037fb800, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR8_ABCD_L
      { 0x00000000, 0x037fb800, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR16_ABCD
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x037fb000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_ABCD
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02662000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_ABCD
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_ABCD_and_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02420000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02640000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_AD
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02200000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_NOAX_and_GR32_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02400000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_AD
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// CCR
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR32_AD_and_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x02000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
    {	// GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
      { 0x00000000, 0x00000000, },	// sub_8bit
      { 0x00000000, 0x00000000, },	// sub_8bit_hi
      { 0x00000000, 0x00000000, },	// sub_16bit
      { 0x00000000, 0x00000000, },	// sub_32bit
      { 0x00000000, 0x00000000, },	// sub_sd
      { 0x00000000, 0x00000000, },	// sub_ss
      { 0x00000000, 0x00000000, },	// sub_xmm
    },
  };
  assert(A && B && "Missing regclass");
  --Idx;
  assert(Idx < 7 && "Bad subreg");
  const uint32_t *TV = Table[B->getID()][Idx];
  const uint32_t *SC = A->getSubClassMask();
  for (unsigned i = 0; i != 2; ++i)
    if (unsigned Common = TV[i] & SC[i])
      return getRegClass(32*i + CountTrailingZeros_32(Common));
  return 0;
}

/// Get the weight in units of pressure for this register class.
const RegClassWeight &X86GenRegisterInfo::
getRegClassWeight(const TargetRegisterClass *RC) const {
  static const RegClassWeight RCWeightTable[] = {
    {1, 20},  	// GR8
    {2, 34},  	// GR64
    {2, 32},  	// GR16
    {1, 16},  	// FR32
    {2, 32},  	// GR32
    {1, 16},  	// CONTROL_REG
    {1, 16},  	// FR64
    {2, 32},  	// GR64_with_sub_8bit
    {1, 16},  	// VR128
    {1, 16},  	// VR256
    {2, 30},  	// GR32_NOAX
    {2, 30},  	// GR32_NOSP
    {2, 30},  	// GR64_NOSP
    {2, 30},  	// GR64_with_sub_32bit_in_GR32_NOAX
    {2, 28},  	// GR32_NOAX_and_GR32_NOSP
    {2, 28},  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
    {2, 18},  	// GR64_NOREX
    {2, 18},  	// GR64_TC
    {1, 8},  	// GR8_NOREX
    {2, 16},  	// GR16_NOREX
    {1, 8},  	// DEBUG_REG
    {2, 16},  	// GR32_NOREX
    {2, 16},  	// GR64_NOSP_and_GR64_TC
    {2, 16},  	// GR64_with_sub_16bit_in_GR16_NOREX
    {1, 8},  	// VR64
    {0, 0},  	// RST
    {2, 14},  	// GR32_NOAX_and_GR32_NOREX
    {2, 14},  	// GR32_NOREX_NOSP
    {1, 7},  	// RFP32
    {1, 7},  	// RFP64
    {2, 14},  	// GR64_NOREX_NOSP
    {2, 14},  	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
    {2, 14},  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
    {1, 7},  	// RFP80
    {1, 6},  	// SEGMENT_REG
    {2, 12},  	// GR32_NOAX_and_GR32_NOREX_NOSP
    {2, 12},  	// GR64_NOREX_and_GR64_TC
    {2, 12},  	// GR64_TCW64
    {2, 12},  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
    {2, 10},  	// GR64_NOREX_NOSP_and_GR64_TC
    {2, 10},  	// GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
    {1, 4},  	// GR8_ABCD_H
    {1, 4},  	// GR8_ABCD_L
    {2, 8},  	// GR16_ABCD
    {2, 8},  	// GR32_ABCD
    {2, 8},  	// GR64_ABCD
    {2, 8},  	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
    {2, 6},  	// GR32_ABCD_and_GR32_NOAX
    {2, 6},  	// GR32_TC
    {2, 6},  	// GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
    {2, 6},  	// GR64_with_sub_32bit_in_GR32_TC
    {2, 4},  	// GR32_AD
    {2, 4},  	// GR32_NOAX_and_GR32_TC
    {2, 4},  	// GR64_with_sub_32bit_in_GR32_AD
    {2, 4},  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
    {0, 0},  	// CCR
    {2, 2},  	// GR32_AD_and_GR32_NOAX
    {2, 2},  	// GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
    {0, 0} };
  return RCWeightTable[RC->getID()];
}


// Get the number of dimensions of register pressure.
unsigned X86GenRegisterInfo::getNumRegPressureSets() const {
  return 19;
}

// Get the register unit pressure limit for this dimension.
// This limit must be adjusted dynamically for reserved registers.
unsigned X86GenRegisterInfo::
getRegPressureSetLimit(unsigned Idx) const {
  static const unsigned PressureLimitTable[] = {
    20,  	// 0: GR8
    34,  	// 1: GR64
    16,  	// 2: FR32
    16,  	// 3: CONTROL_REG
    18,  	// 4: GR64_NOREX
    8,  	// 5: GR8_NOREX
    8,  	// 6: DEBUG_REG
    8,  	// 7: VR64
    7,  	// 8: RFP32
    6,  	// 9: SEGMENT_REG
    4,  	// 10: GR8_ABCD_H
    4,  	// 11: GR8_ABCD_L
    26,  	// 12: GR8+GR64_NOREX
    27,  	// 13: GR8+GR64_TC
    23,  	// 14: GR8+GR64_TCW64
    24,  	// 15: GR64_NOREX+GR64_TC
    20,  	// 16: GR64_TC+GR8_NOREX
    14,  	// 17: GR8_NOREX+GR64_NOREX_and_GR64_TC
    14,  	// 18: GR8_NOREX+GR64_TCW64
    0 };
  return PressureLimitTable[Idx];
}

/// Get the dimensions of register pressure impacted by this register class.
/// Returns a -1 terminated array of pressure set IDs
const int* X86GenRegisterInfo::
getRegClassPressureSets(const TargetRegisterClass *RC) const {
  static const int RCSetsTable[] = {
    0,  1,  12,  13,  14,  -1,  	// GR8
    1,  -1,  	// GR64
    1,  -1,  	// GR16
    2,  -1,  	// FR32
    1,  -1,  	// GR32
    3,  -1,  	// CONTROL_REG
    2,  -1,  	// FR64
    1,  -1,  	// GR64_with_sub_8bit
    2,  -1,  	// VR128
    2,  -1,  	// VR256
    1,  -1,  	// GR32_NOAX
    1,  -1,  	// GR32_NOSP
    1,  -1,  	// GR64_NOSP
    1,  -1,  	// GR64_with_sub_32bit_in_GR32_NOAX
    1,  -1,  	// GR32_NOAX_and_GR32_NOSP
    1,  -1,  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOSP
    1,  4,  12,  15,  -1,  	// GR64_NOREX
    1,  13,  15,  16,  -1,  	// GR64_TC
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR8_NOREX
    1,  4,  12,  15,  -1,  	// GR16_NOREX
    6,  -1,  	// DEBUG_REG
    1,  4,  12,  15,  -1,  	// GR32_NOREX
    1,  13,  15,  16,  -1,  	// GR64_NOSP_and_GR64_TC
    1,  4,  12,  15,  -1,  	// GR64_with_sub_16bit_in_GR16_NOREX
    7,  -1,  	// VR64
    -1,  	// RST
    1,  4,  12,  15,  -1,  	// GR32_NOAX_and_GR32_NOREX
    1,  4,  12,  15,  -1,  	// GR32_NOREX_NOSP
    8,  -1,  	// RFP32
    8,  -1,  	// RFP64
    1,  4,  12,  15,  -1,  	// GR64_NOREX_NOSP
    1,  13,  15,  16,  -1,  	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX
    1,  4,  12,  15,  -1,  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
    8,  -1,  	// RFP80
    9,  -1,  	// SEGMENT_REG
    1,  4,  12,  15,  -1,  	// GR32_NOAX_and_GR32_NOREX_NOSP
    1,  4,  12,  13,  15,  16,  17,  -1,  	// GR64_NOREX_and_GR64_TC
    1,  13,  14,  15,  16,  18,  -1,  	// GR64_TCW64
    1,  4,  12,  15,  -1,  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX_NOSP
    1,  4,  12,  13,  15,  16,  17,  -1,  	// GR64_NOREX_NOSP_and_GR64_TC
    1,  13,  14,  15,  16,  18,  -1,  	// GR64_TCW64_and_GR64_with_sub_32bit_in_GR32_NOAX
    0,  1,  4,  5,  10,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR8_ABCD_H
    0,  1,  4,  5,  11,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR8_ABCD_L
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR16_ABCD
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_ABCD
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_ABCD
    1,  4,  12,  13,  15,  16,  17,  -1,  	// GR64_TC_and_GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_NOREX
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_ABCD_and_GR32_NOAX
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_TC
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_with_sub_32bit_in_GR32_ABCD_and_GR32_NOAX
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_with_sub_32bit_in_GR32_TC
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_AD
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_NOAX_and_GR32_TC
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_with_sub_32bit_in_GR32_AD
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_with_sub_32bit_in_GR32_NOAX_and_GR32_TC
    -1,  	// CCR
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR32_AD_and_GR32_NOAX
    0,  1,  4,  5,  12,  13,  14,  15,  16,  17,  18,  -1,  	// GR64_with_sub_32bit_in_GR32_AD_and_GR32_NOAX
    -1 };
  static const unsigned RCSetStartTable[] = {
    0,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,41,46,58,63,65,70,75,80,82,83,88,93,95,97,102,107,112,114,116,121,129,136,141,149,156,169,182,194,206,218,226,238,250,262,274,286,298,310,322,323,335,0 };
  unsigned SetListStart = RCSetStartTable[RC->getID()];
  return &RCSetsTable[SetListStart];
}

extern const MCRegisterDesc X86RegDesc[];
extern const uint16_t X86RegLists[];
extern const uint16_t *getX86SubRegTable();
// X86 Dwarf<->LLVM register mappings.
extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour0Dwarf2L[];
extern const unsigned X86DwarfFlavour0Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour1Dwarf2L[];
extern const unsigned X86DwarfFlavour1Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour2Dwarf2L[];
extern const unsigned X86DwarfFlavour2Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour0Dwarf2L[];
extern const unsigned X86EHFlavour0Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour1Dwarf2L[];
extern const unsigned X86EHFlavour1Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour2Dwarf2L[];
extern const unsigned X86EHFlavour2Dwarf2LSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour0L2Dwarf[];
extern const unsigned X86DwarfFlavour0L2DwarfSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour1L2Dwarf[];
extern const unsigned X86DwarfFlavour1L2DwarfSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86DwarfFlavour2L2Dwarf[];
extern const unsigned X86DwarfFlavour2L2DwarfSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour0L2Dwarf[];
extern const unsigned X86EHFlavour0L2DwarfSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour1L2Dwarf[];
extern const unsigned X86EHFlavour1L2DwarfSize;

extern const MCRegisterInfo::DwarfLLVMRegPair X86EHFlavour2L2Dwarf[];
extern const unsigned X86EHFlavour2L2DwarfSize;

X86GenRegisterInfo::
X86GenRegisterInfo(unsigned RA, unsigned DwarfFlavour, unsigned EHFlavour)
  : TargetRegisterInfo(X86RegInfoDesc, RegisterClasses, RegisterClasses+58,
             X86SubRegIndexTable) {
  InitMCRegisterInfo(X86RegDesc, 160, RA,
                     X86MCRegisterClasses, 58,
                     X86RegLists,
                     getX86SubRegTable(), 7);

  switch (DwarfFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    mapDwarfRegsToLLVMRegs(X86DwarfFlavour0Dwarf2L, X86DwarfFlavour0Dwarf2LSize, false);
    break;
  case 1:
    mapDwarfRegsToLLVMRegs(X86DwarfFlavour1Dwarf2L, X86DwarfFlavour1Dwarf2LSize, false);
    break;
  case 2:
    mapDwarfRegsToLLVMRegs(X86DwarfFlavour2Dwarf2L, X86DwarfFlavour2Dwarf2LSize, false);
    break;
  }
  switch (EHFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    mapDwarfRegsToLLVMRegs(X86EHFlavour0Dwarf2L, X86EHFlavour0Dwarf2LSize, true);
    break;
  case 1:
    mapDwarfRegsToLLVMRegs(X86EHFlavour1Dwarf2L, X86EHFlavour1Dwarf2LSize, true);
    break;
  case 2:
    mapDwarfRegsToLLVMRegs(X86EHFlavour2Dwarf2L, X86EHFlavour2Dwarf2LSize, true);
    break;
  }
  switch (DwarfFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    mapLLVMRegsToDwarfRegs(X86DwarfFlavour0L2Dwarf, X86DwarfFlavour0L2DwarfSize, false);
    break;
  case 1:
    mapLLVMRegsToDwarfRegs(X86DwarfFlavour1L2Dwarf, X86DwarfFlavour1L2DwarfSize, false);
    break;
  case 2:
    mapLLVMRegsToDwarfRegs(X86DwarfFlavour2L2Dwarf, X86DwarfFlavour2L2DwarfSize, false);
    break;
  }
  switch (EHFlavour) {
  default:
    llvm_unreachable("Unknown DWARF flavour");
  case 0:
    mapLLVMRegsToDwarfRegs(X86EHFlavour0L2Dwarf, X86EHFlavour0L2DwarfSize, true);
    break;
  case 1:
    mapLLVMRegsToDwarfRegs(X86EHFlavour1L2Dwarf, X86EHFlavour1L2DwarfSize, true);
    break;
  case 2:
    mapLLVMRegsToDwarfRegs(X86EHFlavour2L2Dwarf, X86EHFlavour2L2DwarfSize, true);
    break;
  }
}

static const uint16_t CSR_32_SaveList[] = { X86::ESI, X86::EDI, X86::EBX, X86::EBP, 0 };
static const uint32_t CSR_32_RegMask[] = { 0xc00001f0, 0x0020b000, 0x00000000, 0x00180000, 0x00000000, };
static const uint16_t CSR_32EHRet_SaveList[] = { X86::EAX, X86::EDX, X86::ESI, X86::EDI, X86::EBX, X86::EBP, 0 };
static const uint32_t CSR_32EHRet_RegMask[] = { 0xe00001fe, 0x0021bc01, 0x00000000, 0x00180000, 0x00000000, };
static const uint16_t CSR_64_SaveList[] = { X86::RBX, X86::R12, X86::R13, X86::R14, X86::R15, X86::RBP, 0 };
static const uint32_t CSR_64_RegMask[] = { 0x000001f0, 0x00003000, 0xfe000000, 0x00000dff, 0x00000000, };
static const uint16_t CSR_64EHRet_SaveList[] = { X86::RAX, X86::RDX, X86::RBX, X86::R12, X86::R13, X86::R14, X86::R15, X86::RBP, 0 };
static const uint32_t CSR_64EHRet_RegMask[] = { 0x200001fe, 0x00013c01, 0xfe000000, 0x00004fff, 0x00000000, };
static const uint16_t CSR_Ghc_SaveList[] = { 0 };
static const uint32_t CSR_Ghc_RegMask[] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, };
static const uint16_t CSR_Win64_SaveList[] = { X86::RBX, X86::RBP, X86::RDI, X86::RSI, X86::R12, X86::R13, X86::R14, X86::R15, X86::XMM6, X86::XMM7, X86::XMM8, X86::XMM9, X86::XMM10, X86::XMM11, X86::XMM12, X86::XMM13, X86::XMM14, X86::XMM15, 0 };
static const uint32_t CSR_Win64_RegMask[] = { 0xc00001f0, 0x0020b000, 0xfe000000, 0x001a2dff, 0x0000ffc0, };


} // End llvm namespace 
#endif // GET_REGINFO_TARGET_DESC

