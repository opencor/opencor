/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Subtarget Enumeration Source Fragment                                      *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_SUBTARGETINFO_ENUM
#undef GET_SUBTARGETINFO_ENUM
namespace llvm {
namespace X86 {
enum : uint64_t {
  Feature3DNow = 0,
  Feature3DNowA = 1,
  Feature64Bit = 2,
  FeatureADX = 3,
  FeatureAES = 4,
  FeatureAVX = 5,
  FeatureAVX2 = 6,
  FeatureAVX512 = 7,
  FeatureBMI = 8,
  FeatureBMI2 = 9,
  FeatureBWI = 10,
  FeatureCDI = 11,
  FeatureCMOV = 12,
  FeatureCMPXCHG16B = 13,
  FeatureCallRegIndirect = 14,
  FeatureDQI = 15,
  FeatureERI = 16,
  FeatureF16C = 17,
  FeatureFMA = 18,
  FeatureFMA4 = 19,
  FeatureFSGSBase = 20,
  FeatureFXSR = 21,
  FeatureHLE = 22,
  FeatureLAHFSAHF = 23,
  FeatureLEAForSP = 24,
  FeatureLEAUsesAG = 25,
  FeatureLZCNT = 26,
  FeatureMMX = 27,
  FeatureMOVBE = 28,
  FeatureMPX = 29,
  FeaturePCLMUL = 30,
  FeaturePFI = 31,
  FeaturePKU = 32,
  FeaturePOPCNT = 33,
  FeaturePRFCHW = 34,
  FeaturePadShortFunctions = 35,
  FeatureRDRAND = 36,
  FeatureRDSEED = 37,
  FeatureRTM = 38,
  FeatureSHA = 39,
  FeatureSSE1 = 40,
  FeatureSSE2 = 41,
  FeatureSSE3 = 42,
  FeatureSSE4A = 43,
  FeatureSSE41 = 44,
  FeatureSSE42 = 45,
  FeatureSSEUnalignedMem = 46,
  FeatureSSSE3 = 47,
  FeatureSlowBTMem = 48,
  FeatureSlowDivide32 = 49,
  FeatureSlowDivide64 = 50,
  FeatureSlowIncDec = 51,
  FeatureSlowLEA = 52,
  FeatureSlowSHLD = 53,
  FeatureSlowUAMem16 = 54,
  FeatureSlowUAMem32 = 55,
  FeatureSoftFloat = 56,
  FeatureTBM = 57,
  FeatureVLX = 58,
  FeatureXOP = 59,
  FeatureXSAVE = 60,
  FeatureXSAVEC = 61,
  FeatureXSAVEOPT = 62,
  FeatureXSAVES = 63,
  Mode16Bit = 64,
  Mode32Bit = 65,
  Mode64Bit = 66,
  ProcIntelAtom = 67,
  ProcIntelSLM = 68
};
}
} // end llvm namespace
#endif // GET_SUBTARGETINFO_ENUM


#ifdef GET_SUBTARGETINFO_MC_DESC
#undef GET_SUBTARGETINFO_MC_DESC
namespace llvm {
// Sorted (by key) array of values for CPU features.
extern const llvm::SubtargetFeatureKV X86FeatureKV[] = {
  { "16bit-mode", "16-bit mode (i8086)", { X86::Mode16Bit }, { } },
  { "32bit-mode", "32-bit mode (80386)", { X86::Mode32Bit }, { } },
  { "3dnow", "Enable 3DNow! instructions", { X86::Feature3DNow }, { X86::FeatureMMX } },
  { "3dnowa", "Enable 3DNow! Athlon instructions", { X86::Feature3DNowA }, { X86::Feature3DNow } },
  { "64bit", "Support 64-bit instructions", { X86::Feature64Bit }, { X86::FeatureCMOV } },
  { "64bit-mode", "64-bit mode (x86_64)", { X86::Mode64Bit }, { } },
  { "adx", "Support ADX instructions", { X86::FeatureADX }, { } },
  { "aes", "Enable AES instructions", { X86::FeatureAES }, { X86::FeatureSSE2 } },
  { "atom", "Intel Atom processors", { X86::ProcIntelAtom }, { } },
  { "avx", "Enable AVX instructions", { X86::FeatureAVX }, { X86::FeatureSSE42 } },
  { "avx2", "Enable AVX2 instructions", { X86::FeatureAVX2 }, { X86::FeatureAVX } },
  { "avx512bw", "Enable AVX-512 Byte and Word Instructions", { X86::FeatureBWI }, { X86::FeatureAVX512 } },
  { "avx512cd", "Enable AVX-512 Conflict Detection Instructions", { X86::FeatureCDI }, { X86::FeatureAVX512 } },
  { "avx512dq", "Enable AVX-512 Doubleword and Quadword Instructions", { X86::FeatureDQI }, { X86::FeatureAVX512 } },
  { "avx512er", "Enable AVX-512 Exponential and Reciprocal Instructions", { X86::FeatureERI }, { X86::FeatureAVX512 } },
  { "avx512f", "Enable AVX-512 instructions", { X86::FeatureAVX512 }, { X86::FeatureAVX2 } },
  { "avx512pf", "Enable AVX-512 PreFetch Instructions", { X86::FeaturePFI }, { X86::FeatureAVX512 } },
  { "avx512vl", "Enable AVX-512 Vector Length eXtensions", { X86::FeatureVLX }, { X86::FeatureAVX512 } },
  { "bmi", "Support BMI instructions", { X86::FeatureBMI }, { } },
  { "bmi2", "Support BMI2 instructions", { X86::FeatureBMI2 }, { } },
  { "call-reg-indirect", "Call register indirect", { X86::FeatureCallRegIndirect }, { } },
  { "cmov", "Enable conditional move instructions", { X86::FeatureCMOV }, { } },
  { "cx16", "64-bit with cmpxchg16b", { X86::FeatureCMPXCHG16B }, { X86::Feature64Bit } },
  { "f16c", "Support 16-bit floating point conversion instructions", { X86::FeatureF16C }, { X86::FeatureAVX } },
  { "fma", "Enable three-operand fused multiple-add", { X86::FeatureFMA }, { X86::FeatureAVX } },
  { "fma4", "Enable four-operand fused multiple-add", { X86::FeatureFMA4 }, { X86::FeatureAVX, X86::FeatureSSE4A } },
  { "fsgsbase", "Support FS/GS Base instructions", { X86::FeatureFSGSBase }, { } },
  { "fxsr", "Support fxsave/fxrestore instructions", { X86::FeatureFXSR }, { } },
  { "hle", "Support HLE", { X86::FeatureHLE }, { } },
  { "idivl-to-divb", "Use 8-bit divide for positive values less than 256", { X86::FeatureSlowDivide32 }, { } },
  { "idivq-to-divw", "Use 16-bit divide for positive values less than 65536", { X86::FeatureSlowDivide64 }, { } },
  { "lea-sp", "Use LEA for adjusting the stack pointer", { X86::FeatureLEAForSP }, { } },
  { "lea-uses-ag", "LEA instruction needs inputs at AG stage", { X86::FeatureLEAUsesAG }, { } },
  { "lzcnt", "Support LZCNT instruction", { X86::FeatureLZCNT }, { } },
  { "mmx", "Enable MMX instructions", { X86::FeatureMMX }, { } },
  { "movbe", "Support MOVBE instruction", { X86::FeatureMOVBE }, { } },
  { "mpx", "Support MPX instructions", { X86::FeatureMPX }, { } },
  { "pad-short-functions", "Pad short functions", { X86::FeaturePadShortFunctions }, { } },
  { "pclmul", "Enable packed carry-less multiplication instructions", { X86::FeaturePCLMUL }, { X86::FeatureSSE2 } },
  { "pku", "Enable protection keys", { X86::FeaturePKU }, { } },
  { "popcnt", "Support POPCNT instruction", { X86::FeaturePOPCNT }, { } },
  { "prfchw", "Support PRFCHW instructions", { X86::FeaturePRFCHW }, { } },
  { "rdrnd", "Support RDRAND instruction", { X86::FeatureRDRAND }, { } },
  { "rdseed", "Support RDSEED instruction", { X86::FeatureRDSEED }, { } },
  { "rtm", "Support RTM instructions", { X86::FeatureRTM }, { } },
  { "sahf", "Support LAHF and SAHF instructions", { X86::FeatureLAHFSAHF }, { } },
  { "sha", "Enable SHA instructions", { X86::FeatureSHA }, { X86::FeatureSSE2 } },
  { "slm", "Intel Silvermont processors", { X86::ProcIntelSLM }, { } },
  { "slow-bt-mem", "Bit testing of memory is slow", { X86::FeatureSlowBTMem }, { } },
  { "slow-incdec", "INC and DEC instructions are slower than ADD and SUB", { X86::FeatureSlowIncDec }, { } },
  { "slow-lea", "LEA instruction with certain arguments is slow", { X86::FeatureSlowLEA }, { } },
  { "slow-shld", "SHLD instruction is slow", { X86::FeatureSlowSHLD }, { } },
  { "slow-unaligned-mem-16", "Slow unaligned 16-byte memory access", { X86::FeatureSlowUAMem16 }, { } },
  { "slow-unaligned-mem-32", "Slow unaligned 32-byte memory access", { X86::FeatureSlowUAMem32 }, { } },
  { "soft-float", "Use software floating point features.", { X86::FeatureSoftFloat }, { } },
  { "sse", "Enable SSE instructions", { X86::FeatureSSE1 }, { X86::FeatureCMOV } },
  { "sse-unaligned-mem", "Allow unaligned memory operands with SSE instructions", { X86::FeatureSSEUnalignedMem }, { } },
  { "sse2", "Enable SSE2 instructions", { X86::FeatureSSE2 }, { X86::FeatureSSE1 } },
  { "sse3", "Enable SSE3 instructions", { X86::FeatureSSE3 }, { X86::FeatureSSE2 } },
  { "sse4.1", "Enable SSE 4.1 instructions", { X86::FeatureSSE41 }, { X86::FeatureSSSE3 } },
  { "sse4.2", "Enable SSE 4.2 instructions", { X86::FeatureSSE42 }, { X86::FeatureSSE41 } },
  { "sse4a", "Support SSE 4a instructions", { X86::FeatureSSE4A }, { X86::FeatureSSE3 } },
  { "ssse3", "Enable SSSE3 instructions", { X86::FeatureSSSE3 }, { X86::FeatureSSE3 } },
  { "tbm", "Enable TBM instructions", { X86::FeatureTBM }, { } },
  { "xop", "Enable XOP instructions", { X86::FeatureXOP }, { X86::FeatureFMA4 } },
  { "xsave", "Support xsave instructions", { X86::FeatureXSAVE }, { } },
  { "xsavec", "Support xsavec instructions", { X86::FeatureXSAVEC }, { } },
  { "xsaveopt", "Support xsaveopt instructions", { X86::FeatureXSAVEOPT }, { } },
  { "xsaves", "Support xsaves instructions", { X86::FeatureXSAVES }, { } }
};

// Sorted (by key) array of values for CPU subtype.
extern const llvm::SubtargetFeatureKV X86SubTypeKV[] = {
  { "amdfam10", "Select the amdfam10 processor", { X86::FeatureSSE4A, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "athlon", "Select the athlon processor", { X86::FeatureSlowUAMem16, X86::Feature3DNowA, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon-4", "Select the athlon-4 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE1, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon-fx", "Select the athlon-fx processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE2, X86::Feature3DNowA, X86::FeatureFXSR, X86::Feature64Bit, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon-mp", "Select the athlon-mp processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE1, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon-tbird", "Select the athlon-tbird processor", { X86::FeatureSlowUAMem16, X86::Feature3DNowA, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon-xp", "Select the athlon-xp processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE1, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon64", "Select the athlon64 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE2, X86::Feature3DNowA, X86::FeatureFXSR, X86::Feature64Bit, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "athlon64-sse3", "Select the athlon64-sse3 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE3, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "atom", "Select the atom processor", { X86::ProcIntelAtom, X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSSE3, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureMOVBE, X86::FeatureSlowBTMem, X86::FeatureLEAForSP, X86::FeatureSlowDivide32, X86::FeatureSlowDivide64, X86::FeatureCallRegIndirect, X86::FeatureLEAUsesAG, X86::FeaturePadShortFunctions, X86::FeatureLAHFSAHF }, { } },
  { "barcelona", "Select the barcelona processor", { X86::FeatureSSE4A, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "bdver1", "Select the bdver1 processor", { X86::FeatureXOP, X86::FeatureFMA4, X86::FeatureCMPXCHG16B, X86::FeatureAES, X86::FeaturePRFCHW, X86::FeaturePCLMUL, X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureSSE4A, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "bdver2", "Select the bdver2 processor", { X86::FeatureXOP, X86::FeatureFMA4, X86::FeatureCMPXCHG16B, X86::FeatureAES, X86::FeaturePRFCHW, X86::FeaturePCLMUL, X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureSSE4A, X86::FeatureF16C, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureBMI, X86::FeatureTBM, X86::FeatureFMA, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "bdver3", "Select the bdver3 processor", { X86::FeatureXOP, X86::FeatureFMA4, X86::FeatureCMPXCHG16B, X86::FeatureAES, X86::FeaturePRFCHW, X86::FeaturePCLMUL, X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureSSE4A, X86::FeatureF16C, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureBMI, X86::FeatureTBM, X86::FeatureFMA, X86::FeatureXSAVEOPT, X86::FeatureSlowSHLD, X86::FeatureFSGSBase, X86::FeatureLAHFSAHF }, { } },
  { "bdver4", "Select the bdver4 processor", { X86::FeatureMMX, X86::FeatureAVX2, X86::FeatureFXSR, X86::FeatureXOP, X86::FeatureFMA4, X86::FeatureCMPXCHG16B, X86::FeatureAES, X86::FeaturePRFCHW, X86::FeaturePCLMUL, X86::FeatureF16C, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureTBM, X86::FeatureFMA, X86::FeatureXSAVEOPT, X86::FeatureFSGSBase, X86::FeatureLAHFSAHF }, { } },
  { "bonnell", "Select the bonnell processor", { X86::ProcIntelAtom, X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSSE3, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureMOVBE, X86::FeatureSlowBTMem, X86::FeatureLEAForSP, X86::FeatureSlowDivide32, X86::FeatureSlowDivide64, X86::FeatureCallRegIndirect, X86::FeatureLEAUsesAG, X86::FeaturePadShortFunctions, X86::FeatureLAHFSAHF }, { } },
  { "broadwell", "Select the broadwell processor", { X86::FeatureMMX, X86::FeatureAVX2, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureADX, X86::FeatureRDSEED, X86::FeatureSlowIncDec, X86::FeatureLAHFSAHF }, { } },
  { "btver1", "Select the btver1 processor", { X86::FeatureMMX, X86::FeatureSSSE3, X86::FeatureSSE4A, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeaturePRFCHW, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "btver2", "Select the btver2 processor", { X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureSSE4A, X86::FeatureCMPXCHG16B, X86::FeaturePRFCHW, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureBMI, X86::FeatureF16C, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeaturePOPCNT, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureSlowSHLD, X86::FeatureLAHFSAHF }, { } },
  { "c3", "Select the c3 processor", { X86::FeatureSlowUAMem16, X86::Feature3DNow }, { } },
  { "c3-2", "Select the c3-2 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE1, X86::FeatureFXSR }, { } },
  { "core-avx-i", "Select the core-avx-i processor", { X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowUAMem32, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureLAHFSAHF }, { } },
  { "core-avx2", "Select the core-avx2 processor", { X86::FeatureMMX, X86::FeatureAVX2, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureRDRAND, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureSlowIncDec, X86::FeatureLAHFSAHF }, { } },
  { "core2", "Select the core2 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSSE3, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureLAHFSAHF }, { } },
  { "corei7", "Select the corei7 processor", { X86::FeatureMMX, X86::FeatureSSE42, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureLAHFSAHF }, { } },
  { "corei7-avx", "Select the corei7-avx processor", { X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowUAMem32, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureLAHFSAHF }, { } },
  { "generic", "Select the generic processor", { X86::FeatureSlowUAMem16 }, { } },
  { "geode", "Select the geode processor", { X86::FeatureSlowUAMem16, X86::Feature3DNowA }, { } },
  { "haswell", "Select the haswell processor", { X86::FeatureMMX, X86::FeatureAVX2, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureRDRAND, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureSlowIncDec, X86::FeatureLAHFSAHF }, { } },
  { "i386", "Select the i386 processor", { X86::FeatureSlowUAMem16 }, { } },
  { "i486", "Select the i486 processor", { X86::FeatureSlowUAMem16 }, { } },
  { "i586", "Select the i586 processor", { X86::FeatureSlowUAMem16 }, { } },
  { "i686", "Select the i686 processor", { X86::FeatureSlowUAMem16 }, { } },
  { "ivybridge", "Select the ivybridge processor", { X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowUAMem32, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureLAHFSAHF }, { } },
  { "k6", "Select the k6 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX }, { } },
  { "k6-2", "Select the k6-2 processor", { X86::FeatureSlowUAMem16, X86::Feature3DNow }, { } },
  { "k6-3", "Select the k6-3 processor", { X86::FeatureSlowUAMem16, X86::Feature3DNow }, { } },
  { "k8", "Select the k8 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE2, X86::Feature3DNowA, X86::FeatureFXSR, X86::Feature64Bit, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "k8-sse3", "Select the k8-sse3 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE3, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "knl", "Select the knl processor", { X86::FeatureMMX, X86::FeatureAVX512, X86::FeatureFXSR, X86::FeatureERI, X86::FeatureCDI, X86::FeaturePFI, X86::FeatureCMPXCHG16B, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureSlowIncDec, X86::FeatureMPX, X86::FeatureLAHFSAHF }, { } },
  { "nehalem", "Select the nehalem processor", { X86::FeatureMMX, X86::FeatureSSE42, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureLAHFSAHF }, { } },
  { "nocona", "Select the nocona processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE3, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem }, { } },
  { "opteron", "Select the opteron processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE2, X86::Feature3DNowA, X86::FeatureFXSR, X86::Feature64Bit, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "opteron-sse3", "Select the opteron-sse3 processor", { X86::FeatureSlowUAMem16, X86::FeatureSSE3, X86::Feature3DNowA, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowSHLD }, { } },
  { "penryn", "Select the penryn processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE41, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureLAHFSAHF }, { } },
  { "pentium", "Select the pentium processor", { X86::FeatureSlowUAMem16 }, { } },
  { "pentium-m", "Select the pentium-m processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE2, X86::FeatureFXSR, X86::FeatureSlowBTMem }, { } },
  { "pentium-mmx", "Select the pentium-mmx processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX }, { } },
  { "pentium2", "Select the pentium2 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureCMOV, X86::FeatureFXSR }, { } },
  { "pentium3", "Select the pentium3 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE1, X86::FeatureFXSR }, { } },
  { "pentium3m", "Select the pentium3m processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE1, X86::FeatureFXSR, X86::FeatureSlowBTMem }, { } },
  { "pentium4", "Select the pentium4 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE2, X86::FeatureFXSR }, { } },
  { "pentium4m", "Select the pentium4m processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE2, X86::FeatureFXSR, X86::FeatureSlowBTMem }, { } },
  { "pentiumpro", "Select the pentiumpro processor", { X86::FeatureSlowUAMem16, X86::FeatureCMOV }, { } },
  { "prescott", "Select the prescott processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE3, X86::FeatureFXSR, X86::FeatureSlowBTMem }, { } },
  { "sandybridge", "Select the sandybridge processor", { X86::FeatureMMX, X86::FeatureAVX, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeatureSlowUAMem32, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureLAHFSAHF }, { } },
  { "silvermont", "Select the silvermont processor", { X86::ProcIntelSLM, X86::FeatureMMX, X86::FeatureSSE42, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureMOVBE, X86::FeaturePOPCNT, X86::FeaturePCLMUL, X86::FeatureAES, X86::FeatureSlowDivide64, X86::FeatureCallRegIndirect, X86::FeaturePRFCHW, X86::FeatureSlowLEA, X86::FeatureSlowIncDec, X86::FeatureSlowBTMem, X86::FeatureLAHFSAHF }, { } },
  { "skx", "Select the skx processor", { X86::FeatureMMX, X86::FeatureAVX512, X86::FeatureFXSR, X86::FeatureCDI, X86::FeatureDQI, X86::FeatureBWI, X86::FeatureVLX, X86::FeaturePKU, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureADX, X86::FeatureRDSEED, X86::FeatureSlowIncDec, X86::FeatureMPX, X86::FeatureXSAVEC, X86::FeatureXSAVES, X86::FeatureLAHFSAHF }, { } },
  { "skylake", "Select the skylake processor", { X86::FeatureMMX, X86::FeatureAVX512, X86::FeatureFXSR, X86::FeatureCDI, X86::FeatureDQI, X86::FeatureBWI, X86::FeatureVLX, X86::FeaturePKU, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureXSAVE, X86::FeatureXSAVEOPT, X86::FeatureRDRAND, X86::FeatureF16C, X86::FeatureFSGSBase, X86::FeatureMOVBE, X86::FeatureLZCNT, X86::FeatureBMI, X86::FeatureBMI2, X86::FeatureFMA, X86::FeatureRTM, X86::FeatureHLE, X86::FeatureADX, X86::FeatureRDSEED, X86::FeatureSlowIncDec, X86::FeatureMPX, X86::FeatureXSAVEC, X86::FeatureXSAVES, X86::FeatureLAHFSAHF }, { } },
  { "slm", "Select the slm processor", { X86::ProcIntelSLM, X86::FeatureMMX, X86::FeatureSSE42, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureMOVBE, X86::FeaturePOPCNT, X86::FeaturePCLMUL, X86::FeatureAES, X86::FeatureSlowDivide64, X86::FeatureCallRegIndirect, X86::FeaturePRFCHW, X86::FeatureSlowLEA, X86::FeatureSlowIncDec, X86::FeatureSlowBTMem, X86::FeatureLAHFSAHF }, { } },
  { "westmere", "Select the westmere processor", { X86::FeatureMMX, X86::FeatureSSE42, X86::FeatureFXSR, X86::FeatureCMPXCHG16B, X86::FeatureSlowBTMem, X86::FeaturePOPCNT, X86::FeatureAES, X86::FeaturePCLMUL, X86::FeatureLAHFSAHF }, { } },
  { "winchip-c6", "Select the winchip-c6 processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX }, { } },
  { "winchip2", "Select the winchip2 processor", { X86::FeatureSlowUAMem16, X86::Feature3DNow }, { } },
  { "x86-64", "Select the x86-64 processor", { X86::FeatureMMX, X86::FeatureSSE2, X86::FeatureFXSR, X86::Feature64Bit, X86::FeatureSlowBTMem }, { } },
  { "yonah", "Select the yonah processor", { X86::FeatureSlowUAMem16, X86::FeatureMMX, X86::FeatureSSE3, X86::FeatureFXSR, X86::FeatureSlowBTMem }, { } }
};

#ifdef DBGFIELD
#error "<target>GenSubtargetInfo.inc requires a DBGFIELD macro"
#endif
#ifndef NDEBUG
#define DBGFIELD(x) x,
#else
#define DBGFIELD(x)
#endif

// Functional units for "AtomItineraries"
namespace AtomItinerariesFU {
  const unsigned Port0 = 1 << 0;
  const unsigned Port1 = 1 << 1;
}

extern const llvm::InstrStage X86Stages[] = {
  { 0, 0, 0, llvm::InstrStage::Required }, // No itinerary
  { 13, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 1
  { 7, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 2
  { 21, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 3
  { 1, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 4
  { 1, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 5
  { 6, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 6
  { 5, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 7-8
  { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 9
  { 23, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 10
  { 24, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 11
  { 11, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 12
  { 16, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 13
  { 9, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 14
  { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 15
  { 2, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 16
  { 15, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 17
  { 1, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 18-19
  { 4, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 20
  { 3, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 21
  { 14, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 22
  { 33, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 23
  { 22, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 24
  { 18, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 25
  { 10, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 26
  { 121, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 27
  { 8, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 28
  { 20, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 29
  { 50, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 30
  { 130, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 31
  { 68, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 32
  { 125, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 33
  { 70, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 34
  { 62, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 35
  { 34, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 36
  { 79, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 37
  { 32, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 38
  { 99, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 39
  { 39, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 40
  { 40, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 41
  { 31, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 42
  { 5, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 43
  { 25, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 44
  { 63, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 45
  { 183, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 46
  { 55, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 47
  { 71, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 48
  { 168, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 49
  { 46, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 50
  { 77, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 51
  { 174, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 52
  { 141, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 53
  { 140, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 54
  { 146, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 55
  { 147, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 56
  { 5, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 57
  { 12, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 58
  { 92, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 59
  { 94, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 60
  { 59, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 61
  { 127, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 62
  { 1003, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 63
  { 109, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 64
  { 54, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 65
  { 44, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 66
  { 64, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 67
  { 60, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 68
  { 69, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 69
  { 67, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 70
  { 17, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 71
  { 49, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 72
  { 83, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 73
  { 3, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 74
  { 4, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 75
  { 45, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 76
  { 26, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 77
  { 136, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 78
  { 123, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 79
  { 88, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 80
  { 72, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 81
  { 74, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 82
  { 29, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 83
  { 48, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 84
  { 78, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 85
  { 30, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 86
  { 75, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 87
  { 741, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 88
  { 96, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 89
  { 41, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 90
  { 51, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 91
  { 202, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 92
  { 0, 0, 0, llvm::InstrStage::Required } // End stages
};
extern const unsigned X86OperandCycles[] = {
  0, // No itinerary
  0 // End operand cycles
};
extern const unsigned X86ForwardingPaths[] = {
 0, // No itinerary
 0 // End bypass tables
};

static const llvm::InstrItinerary AtomItineraries[] = {
  { 0, 0, 0, 0, 0 }, // 0 NoInstrModel
  { 1, 1, 2, 0, 0 }, // 1 IIC_AAA_WriteMicrocoded
  { 1, 2, 3, 0, 0 }, // 2 IIC_AAD_WriteMicrocoded
  { 1, 3, 4, 0, 0 }, // 3 IIC_AAM_WriteMicrocoded
  { 1, 1, 2, 0, 0 }, // 4 IIC_AAS_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 5 IIC_BIN_CARRY_NONMEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 6 IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW
  { 1, 5, 6, 0, 0 }, // 7 IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 8 IIC_BIN_CARRY_MEM_WriteALULd
  { 1, 4, 5, 0, 0 }, // 9 IIC_BIN_NONMEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 10 IIC_BIN_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 11 WriteALU
  { 1, 5, 6, 0, 0 }, // 12 IIC_BIN_MEM_WriteALULd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 13 IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 14 IIC_SSE_ALU_F64P_RR_WriteFAdd
  { 1, 7, 9, 0, 0 }, // 15 IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd
  { 1, 9, 10, 0, 0 }, // 16 IIC_SSE_ALU_F32P_RR_WriteFAdd
  { 1, 7, 9, 0, 0 }, // 17 IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd
  { 1, 9, 10, 0, 0 }, // 18 IIC_SSE_ALU_F64S_RR_WriteFAdd
  { 1, 7, 9, 0, 0 }, // 19 IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd
  { 1, 9, 10, 0, 0 }, // 20 IIC_SSE_ALU_F32S_RR_WriteFAdd
  { 1, 6, 7, 0, 0 }, // 21 IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd
  { 1, 9, 10, 0, 0 }, // 22 IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 23 WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 24 WriteFAdd
  { 1, 5, 6, 0, 0 }, // 25 IIC_BIN_MEM_WriteALULd
  { 0, 0, 0, 0, 0 }, // 26 IIC_AES_WriteAESDecEncLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 27 IIC_AES_WriteAESDecEnc
  { 0, 0, 0, 0, 0 }, // 28 IIC_AES_WriteAESIMCLd
  { 0, 0, 0, 0, 0 }, // 29 IIC_AES_WriteAESIMC
  { 0, 0, 0, 0, 0 }, // 30 WriteAESKeyGenLd
  { 0, 0, 0, 0, 0 }, // 31 WriteAESKeyGen
  { 0, 0, 0, 0, 0 }, // 32 WriteVecLogicLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 33 WriteVecLogic
  { 1, 10, 11, 0, 0 }, // 34 IIC_ARPL_MEM_WriteSystem
  { 1, 11, 12, 0, 0 }, // 35 IIC_ARPL_REG_WriteSystem
  { 0, 0, 0, 0, 0 }, // 36 WriteZero
  { 1, 5, 6, 0, 0 }, // 37 IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 38 IIC_SSE_INTALU_P_RR_WriteFBlend
  { 1, 5, 6, 0, 0 }, // 39 IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 40 IIC_ALU_NONMEM_WriteFBlend
  { 1, 12, 13, 0, 0 }, // 41 IIC_BOUND_WriteSystem
  { 1, 13, 14, 0, 0 }, // 42 IIC_BIT_SCAN_MEM_WriteShiftLd
  { 1, 13, 14, 0, 0 }, // 43 IIC_BIT_SCAN_REG_WriteShift
  { 1, 5, 6, 0, 0 }, // 44 IIC_BSWAP_WriteALU
  { 1, 4, 5, 0, 0 }, // 45 IIC_BT_MI_WriteALU
  { 1, 14, 15, 0, 0 }, // 46 IIC_BT_MR_WriteALULd
  { 1, 15, 16, 0, 0 }, // 47 IIC_BT_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 48 IIC_BT_RR_WriteALU
  { 1, 16, 17, 0, 0 }, // 49 IIC_BTX_MI_WriteALULd_WriteRMW
  { 1, 12, 13, 0, 0 }, // 50 IIC_BTX_MR_WriteALULd_WriteRMW
  { 1, 15, 16, 0, 0 }, // 51 IIC_BTX_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 52 IIC_BTX_RR_WriteALU
  { 1, 17, 18, 0, 0 }, // 53 IIC_CALL_MEM_WriteJumpLd
  { 1, 18, 20, 0, 0 }, // 54 IIC_CALL_RI_WriteJump
  { 1, 17, 18, 0, 0 }, // 55 IIC_CALL_MEM_WriteJump
  { 0, 0, 0, 0, 0 }, // 56 WriteSystem
  { 1, 20, 21, 0, 0 }, // 57 IIC_CBW
  { 1, 4, 5, 0, 0 }, // 58 IIC_CLC_WriteALU
  { 1, 21, 22, 0, 0 }, // 59 IIC_CLD_WriteALU
  { 1, 5, 6, 0, 0 }, // 60 IIC_SSE_PREFETCH_WriteLoad
  { 1, 22, 23, 0, 0 }, // 61 IIC_CLI_WriteALU
  { 1, 23, 24, 0, 0 }, // 62 IIC_CLTS_WriteALU
  { 1, 4, 5, 0, 0 }, // 63 IIC_CMC_WriteALU
  { 1, 5, 6, 0, 0 }, // 64 IIC_CMOV16_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 65 IIC_CMOV16_RR_WriteALU
  { 1, 5, 6, 0, 0 }, // 66 IIC_CMOV32_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 67 IIC_CMOV32_RR_WriteALU
  { 1, 21, 22, 0, 0 }, // 68 IIC_CMPS_WriteMicrocoded
  { 1, 24, 25, 0, 0 }, // 69 IIC_CMPXCHG_16B_WriteALULd_WriteRMW
  { 1, 22, 23, 0, 0 }, // 70 IIC_CMPXCHG_MEM_WriteALULd_WriteRMW
  { 1, 17, 18, 0, 0 }, // 71 IIC_CMPXCHG_REG_WriteALU
  { 1, 25, 26, 0, 0 }, // 72 IIC_CMPXCHG_8B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 73 IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 74 IIC_CMPXCHG_REG8_WriteALU
  { 1, 26, 27, 0, 0 }, // 75 IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 76 IIC_SSE_COMIS_RR_WriteFAdd
  { 1, 14, 15, 0, 0 }, // 77 IIC_FCOMI_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 78 IIC_CPUID_WriteSystem
  { 0, 0, 0, 0, 0 }, // 79 IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 80 IIC_CRC32_REG_WriteFAdd
  { 1, 2, 3, 0, 0 }, // 81 IIC_SSE_CVT_PD_RR_WriteCvtI2FLd
  { 1, 28, 29, 0, 0 }, // 82 IIC_SSE_CVT_PD_RM_WriteCvtI2F
  { 1, 2, 3, 0, 0 }, // 83 IIC_SSE_CVT_PS_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 84 IIC_SSE_CVT_PS_RR_WriteCvtI2F
  { 1, 28, 29, 0, 0 }, // 85 IIC_SSE_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 86 IIC_SSE_CVT_PD_RR_WriteCvtF2I
  { 1, 28, 29, 0, 0 }, // 87 IIC_SSE_CVT_PD_RM_WriteCvtF2FLd
  { 1, 2, 3, 0, 0 }, // 88 IIC_SSE_CVT_PD_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 89 IIC_SSE_CVT_PS_RM_WriteCvtF2ILd
  { 1, 6, 7, 0, 0 }, // 90 IIC_SSE_CVT_PS_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 91 IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 92 IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I
  { 1, 2, 3, 0, 0 }, // 93 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd
  { 1, 6, 7, 0, 0 }, // 94 IIC_SSE_CVT_Scalar_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 95 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 96 IIC_SSE_CVT_Scalar_RR_WriteCvtI2F
  { 1, 26, 27, 0, 0 }, // 97 IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd
  { 1, 14, 15, 0, 0 }, // 98 IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 99 IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 100 IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I
  { 1, 25, 26, 0, 0 }, // 101 IIC_DAA_WriteMicrocoded
  { 1, 29, 30, 0, 0 }, // 102 IIC_DAS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 103 IIC_UNARY_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 104 IIC_UNARY_REG_WriteALU
  { 1, 30, 31, 0, 0 }, // 105 IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 106 IIC_DIV16_WriteIDiv
  { 1, 30, 31, 0, 0 }, // 107 IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 108 IIC_DIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 109 IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 110 IIC_DIV64_WriteIDiv
  { 1, 32, 33, 0, 0 }, // 111 IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 112 IIC_DIV8_REG_WriteIDiv
  { 1, 33, 34, 0, 0 }, // 113 IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd
  { 1, 33, 34, 0, 0 }, // 114 IIC_SSE_DIV_F64P_RR_WriteFDiv
  { 1, 34, 35, 0, 0 }, // 115 IIC_SSE_DIV_F32P_RR_WriteFDiv
  { 0, 0, 0, 0, 0 }, // 116 WriteFDivLd
  { 0, 0, 0, 0, 0 }, // 117 WriteFDiv
  { 1, 35, 36, 0, 0 }, // 118 IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 119 IIC_SSE_DIV_F64S_RR_WriteFDiv
  { 1, 36, 37, 0, 0 }, // 120 IIC_SSE_DIV_F32S_RR_WriteFDiv
  { 0, 0, 0, 0, 0 }, // 121 IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 122 IIC_SSE_DPPD_RR_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 123 IIC_SSE_DPPS_RR_WriteFAdd
  { 1, 37, 38, 0, 0 }, // 124 IIC_RET_WriteSystem
  { 1, 38, 39, 0, 0 }, // 125 IIC_ENTER_WriteMicrocoded
  { 0, 0, 0, 0, 0 }, // 126 IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 127 IIC_SSE_EXTRACTPS_RR_WriteFBlend
  { 1, 39, 40, 0, 0 }, // 128 IIC_F2XM1_WriteMicrocoded
  { 1, 40, 41, 0, 0 }, // 129 IIC_CALL_FAR_PTR_WriteJump
  { 1, 41, 42, 0, 0 }, // 130 IIC_CALL_FAR_MEM_WriteJumpLd
  { 1, 41, 42, 0, 0 }, // 131 IIC_CALL_FAR_MEM_WriteJump
  { 1, 42, 43, 0, 0 }, // 132 IIC_JMP_FAR_PTR_WriteJump
  { 1, 38, 39, 0, 0 }, // 133 IIC_JMP_FAR_MEM_WriteJumpLd
  { 1, 38, 39, 0, 0 }, // 134 IIC_JMP_FAR_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 135 IIC_FCOMPP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 136 IIC_FPSTP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 137 IIC_FFREE_WriteMicrocoded
  { 1, 43, 44, 0, 0 }, // 138 IIC_FLDCW_WriteLoad
  { 1, 26, 27, 0, 0 }, // 139 IIC_FLDL_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 140 IIC_FNCLEX_WriteMicrocoded
  { 1, 45, 46, 0, 0 }, // 141 IIC_FNINIT_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 142 IIC_FNOP_WriteMicrocoded
  { 1, 28, 29, 0, 0 }, // 143 IIC_FNSTCW_WriteALU
  { 1, 26, 27, 0, 0 }, // 144 IIC_FNSTSW_WriteALU
  { 1, 46, 47, 0, 0 }, // 145 IIC_FPATAN_WriteMicrocoded
  { 1, 47, 48, 0, 0 }, // 146 IIC_FPREM_WriteMicrocoded
  { 1, 48, 49, 0, 0 }, // 147 IIC_FPREM1_WriteMicrocoded
  { 1, 49, 50, 0, 0 }, // 148 IIC_FPTAN_WriteMicrocoded
  { 1, 50, 51, 0, 0 }, // 149 IIC_FRNDINT_WriteMicrocoded
  { 1, 51, 52, 0, 0 }, // 150 IIC_FSCALE_WriteMicrocoded
  { 1, 52, 53, 0, 0 }, // 151 IIC_FSINCOS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 152 IIC_FXAM_WriteMicrocoded
  { 1, 53, 54, 0, 0 }, // 153 IIC_FXRSTOR_WriteMicrocoded
  { 1, 54, 55, 0, 0 }, // 154 IIC_FXSAVE_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 155 IIC_FXTRACT_WriteMicrocoded
  { 1, 55, 56, 0, 0 }, // 156 IIC_FYL2X_WriteMicrocoded
  { 1, 56, 57, 0, 0 }, // 157 IIC_FYL2XP1_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 158 IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 159 IIC_SSE_BIT_P_RR_WriteFAdd
  { 1, 5, 6, 0, 0 }, // 160 IIC_SSE_MOVA_P_RM_WriteLoad
  { 1, 14, 15, 0, 0 }, // 161 IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd
  { 1, 28, 29, 0, 0 }, // 162 IIC_SSE_HADDSUB_RR_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 163 IIC_HLT_WriteSystem
  { 1, 35, 36, 0, 0 }, // 164 IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 165 IIC_IDIV16_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 166 IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 167 IIC_IDIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 168 IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 169 IIC_IDIV64_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 170 IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 171 IIC_IDIV8_WriteIDiv
  { 1, 7, 9, 0, 0 }, // 172 IIC_FILD_WriteLoad
  { 1, 28, 29, 0, 0 }, // 173 IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 174 IIC_IMUL16_RR_WriteIMul
  { 1, 2, 3, 0, 0 }, // 175 IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 176 IIC_IMUL16_RMI_WriteIMulLd
  { 1, 6, 7, 0, 0 }, // 177 IIC_IMUL16_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 178 IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 179 IIC_IMUL32_RR_WriteIMul
  { 1, 57, 58, 0, 0 }, // 180 IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 181 IIC_IMUL32_RMI_WriteIMulLd
  { 1, 57, 58, 0, 0 }, // 182 IIC_IMUL32_RRI_WriteIMul
  { 1, 58, 59, 0, 0 }, // 183 IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 184 IIC_IMUL64_RR_WriteIMul
  { 1, 58, 59, 0, 0 }, // 185 IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd
  { 1, 22, 23, 0, 0 }, // 186 IIC_IMUL64_RMI_WriteIMulLd
  { 1, 22, 23, 0, 0 }, // 187 IIC_IMUL64_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 188 IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 189 IIC_IMUL8_WriteIMul
  { 1, 59, 60, 0, 0 }, // 190 IIC_IN_RI_WriteSystem
  { 1, 60, 61, 0, 0 }, // 191 IIC_IN_RR_WriteSystem
  { 1, 61, 62, 0, 0 }, // 192 IIC_INS_WriteSystem
  { 0, 0, 0, 0, 0 }, // 193 IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 194 IIC_SSE_INSERTPS_RR_WriteFShuffle
  { 1, 62, 63, 0, 0 }, // 195 IIC_INT_WriteSystem
  { 1, 31, 32, 0, 0 }, // 196 IIC_INT3_WriteSystem
  { 1, 63, 64, 0, 0 }, // 197 IIC_INVD_WriteSystem
  { 1, 48, 49, 0, 0 }, // 198 IIC_INVLPG_WriteSystem
  { 0, 0, 0, 0, 0 }, // 199 WriteJumpLd
  { 1, 64, 65, 0, 0 }, // 200 IIC_IRET_WriteJumpLd
  { 1, 16, 17, 0, 0 }, // 201 IIC_FST_WriteStore
  { 1, 6, 7, 0, 0 }, // 202 IIC_FIST_WriteStore
  { 1, 2, 3, 0, 0 }, // 203 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 204 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 205 WriteLoad
  { 1, 15, 16, 0, 0 }, // 206 IIC_Jcc_WriteJump
  { 1, 20, 21, 0, 0 }, // 207 IIC_JCXZ_WriteJump
  { 1, 16, 17, 0, 0 }, // 208 IIC_JMP_MEM_WriteJumpLd
  { 1, 15, 16, 0, 0 }, // 209 IIC_JMP_REG_WriteJump
  { 1, 15, 16, 0, 0 }, // 210 IIC_JMP_REL_WriteJump
  { 1, 4, 5, 0, 0 }, // 211 IIC_AHF_WriteALU
  { 1, 30, 31, 0, 0 }, // 212 IIC_LAR_RM_WriteSystem
  { 1, 65, 66, 0, 0 }, // 213 IIC_LAR_RR_WriteSystem
  { 1, 22, 23, 0, 0 }, // 214 IIC_CMPX_LOCK_WriteALULd_WriteRMW
  { 1, 24, 25, 0, 0 }, // 215 IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 216 IIC_CMPX_LOCK_8_WriteALULd_WriteRMW
  { 1, 25, 26, 0, 0 }, // 217 IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW
  { 1, 21, 22, 0, 0 }, // 218 IIC_SSE_LDDQU_WriteLoad
  { 1, 43, 44, 0, 0 }, // 219 IIC_SSE_LDMXCSR_WriteLoad
  { 1, 26, 27, 0, 0 }, // 220 IIC_LXS_WriteSystem
  { 1, 4, 5, 0, 0 }, // 221 IIC_FLDZ_WriteZero
  { 1, 6, 7, 0, 0 }, // 222 IIC_FIST_WriteZero
  { 1, 5, 6, 0, 0 }, // 223 IIC_FLD_WriteLoad
  { 1, 20, 21, 0, 0 }, // 224 IIC_FLD80_WriteLoad
  { 1, 5, 6, 0, 0 }, // 225 IIC_FLD_WriteMove
  { 1, 16, 17, 0, 0 }, // 226 IIC_LEA_16_WriteLEA
  { 1, 15, 16, 0, 0 }, // 227 IIC_LEA_WriteLEA
  { 1, 16, 17, 0, 0 }, // 228 IIC_LEAVE_WriteALU
  { 1, 4, 5, 0, 0 }, // 229 IIC_SSE_LFENCE_WriteFence
  { 1, 66, 67, 0, 0 }, // 230 IIC_LGDT_WriteSystem
  { 1, 66, 67, 0, 0 }, // 231 IIC_LIDT_WriteSystem
  { 1, 67, 68, 0, 0 }, // 232 IIC_LLDT_MEM_WriteSystem
  { 1, 68, 69, 0, 0 }, // 233 IIC_LLDT_REG_WriteSystem
  { 1, 69, 70, 0, 0 }, // 234 IIC_LMSW_REG_WriteSystem
  { 1, 70, 71, 0, 0 }, // 235 IIC_LMSW_MEM_WriteSystem
  { 1, 5, 6, 0, 0 }, // 236 IIC_ALU_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 237 IIC_ALU_NONMEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 238 IIC_LODS_WriteMicrocoded
  { 1, 25, 26, 0, 0 }, // 239 IIC_LOOP_WriteJump
  { 1, 28, 29, 0, 0 }, // 240 IIC_LOOPE_WriteJump
  { 1, 71, 72, 0, 0 }, // 241 IIC_LOOPNE_WriteJump
  { 1, 37, 38, 0, 0 }, // 242 IIC_RET_WriteJumpLd
  { 1, 50, 51, 0, 0 }, // 243 IIC_LSL_RM_WriteSystem
  { 1, 72, 73, 0, 0 }, // 244 IIC_LSL_RR_WriteSystem
  { 1, 73, 74, 0, 0 }, // 245 IIC_LTR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 246 IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 247 IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 248 IIC_SSE_MASKMOV_WriteStore
  { 1, 5, 6, 0, 0 }, // 249 IIC_SSE_MFENCE_WriteFence
  { 1, 28, 29, 0, 0 }, // 250 IIC_MMX_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 251 IIC_MMX_CVT_PD_RR_WriteCvtF2I
  { 0, 0, 0, 0, 0 }, // 252 WriteCvtI2FLd
  { 0, 0, 0, 0, 0 }, // 253 WriteCvtI2F
  { 1, 7, 9, 0, 0 }, // 254 IIC_MMX_CVT_PS_RM_WriteCvtF2ILd
  { 1, 9, 10, 0, 0 }, // 255 IIC_MMX_CVT_PS_RR_WriteCvtF2I
  { 1, 43, 44, 0, 0 }, // 256 IIC_MMX_EMMS
  { 1, 5, 6, 0, 0 }, // 257 IIC_MMX_MASKMOV_WriteShuffle
  { 1, 74, 75, 0, 0 }, // 258 IIC_MMX_MOV_REG_MM_WriteStore
  { 1, 74, 75, 0, 0 }, // 259 IIC_MMX_MOV_REG_MM_WriteMove
  { 1, 5, 6, 0, 0 }, // 260 IIC_MMX_MOV_MM_RM_WriteStore
  { 1, 5, 6, 0, 0 }, // 261 IIC_MMX_MOV_MM_RM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 262 IIC_MMX_MOV_MM_RM_WriteMove
  { 1, 5, 6, 0, 0 }, // 263 IIC_MMX_MOVQ_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 264 IIC_MMX_MOVQ_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 265 IIC_MMX_MOVQ_RM_WriteStore
  { 0, 0, 0, 0, 0 }, // 266 WriteMove
  { 1, 5, 6, 0, 0 }, // 267 IIC_MMX_ALU_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 268 IIC_MMX_ALU_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 269 IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 270 IIC_MMX_PCK_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 271 IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 272 IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 273 IIC_MMX_ALUQ_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 274 WriteShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 275 WriteShuffle
  { 1, 5, 6, 0, 0 }, // 276 IIC_MMX_ALU_RM_WriteVecLogicLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 277 IIC_MMX_ALU_RR_WriteVecLogic
  { 1, 4, 5, 0, 0 }, // 278 IIC_MMX_MISC_FUNC_REG_WriteVecIMulLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 279 IIC_MMX_MISC_FUNC_MEM_WriteVecIMul
  { 1, 20, 21, 0, 0 }, // 280 IIC_MMX_PEXTR_WriteShuffle
  { 1, 6, 7, 0, 0 }, // 281 IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 43, 44, 0, 0 }, // 282 IIC_MMX_PHADDSUBW_RR_WriteVecALU
  { 1, 20, 21, 0, 0 }, // 283 IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 284 IIC_MMX_PHADDSUBD_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 285 IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 286 IIC_MMX_PINSRW_WriteShuffle
  { 1, 75, 76, 0, 0 }, // 287 IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd
  { 1, 75, 76, 0, 0 }, // 288 IIC_MMX_PMUL_WriteVecIMul
  { 1, 20, 21, 0, 0 }, // 289 IIC_MMX_PSADBW_WriteVecIMulLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 290 IIC_MMX_PSADBW_WriteVecIMul
  { 1, 5, 6, 0, 0 }, // 291 IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 292 IIC_MMX_PSHUF_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 293 IIC_MMX_PSHUF_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 294 IIC_MMX_SHIFT_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 295 IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 296 IIC_MMX_SHIFT_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 297 IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 298 IIC_MMX_UNPCK_H_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 299 IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 300 IIC_MMX_UNPCK_L_WriteShuffle
  { 1, 76, 77, 0, 0 }, // 301 IIC_SSE_MONITOR_WriteSystem
  { 1, 5, 6, 0, 0 }, // 302 IIC_MOV_MEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 303 IIC_MOV_MEM_WriteStore
  { 1, 16, 17, 0, 0 }, // 304 IIC_MOV_MEM_SR_WriteMove
  { 1, 4, 5, 0, 0 }, // 305 IIC_MOV_WriteMove
  { 1, 5, 6, 0, 0 }, // 306 IIC_MOV_MEM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 307 IIC_MOV_REG_SR_WriteMove
  { 1, 77, 78, 0, 0 }, // 308 IIC_MOV_SR_MEM_WriteMove
  { 1, 3, 4, 0, 0 }, // 309 IIC_MOV_SR_REG_WriteMove
  { 1, 78, 79, 0, 0 }, // 310 IIC_MOV_CR_REG_WriteSystem
  { 1, 79, 80, 0, 0 }, // 311 IIC_MOV_DR_REG_WriteSystem
  { 1, 4, 5, 0, 0 }, // 312 IIC_ALU_NONMEM_WriteZero
  { 1, 58, 59, 0, 0 }, // 313 IIC_MOV_REG_CR_WriteSystem
  { 1, 80, 81, 0, 0 }, // 314 IIC_MOV_REG_DR_WriteSystem
  { 1, 5, 6, 0, 0 }, // 315 IIC_SSE_MOVDQ_WriteLoad
  { 1, 5, 6, 0, 0 }, // 316 IIC_SSE_MOVDQ_WriteMove
  { 1, 5, 6, 0, 0 }, // 317 IIC_SSE_MOVA_P_MR_WriteStore
  { 1, 4, 5, 0, 0 }, // 318 IIC_SSE_MOVA_P_RR_WriteFShuffle
  { 1, 5, 6, 0, 0 }, // 319 IIC_MOVBE_WriteStore
  { 1, 5, 6, 0, 0 }, // 320 IIC_MOVBE_WriteALULd
  { 1, 5, 6, 0, 0 }, // 321 IIC_SSE_MOV_LH_WriteLoad
  { 1, 5, 6, 0, 0 }, // 322 IIC_SSE_MOV_LH_WriteFShuffle
  { 1, 4, 5, 0, 0 }, // 323 IIC_SSE_MOVA_P_RR_WriteMove
  { 1, 16, 17, 0, 0 }, // 324 IIC_SSE_MOVU_P_MR_WriteStore
  { 1, 21, 22, 0, 0 }, // 325 IIC_SSE_MOVU_P_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 326 IIC_SSE_MOVU_P_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 327 IIC_SSE_MOV_LH_WriteStore
  { 1, 5, 6, 0, 0 }, // 328 IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd
  { 1, 74, 75, 0, 0 }, // 329 IIC_SSE_MOVMSK_WriteVecLogic
  { 1, 5, 6, 0, 0 }, // 330 IIC_SSE_MOVNT_WriteStore
  { 1, 5, 6, 0, 0 }, // 331 IIC_SSE_MOVDQ_WriteStore
  { 1, 74, 75, 0, 0 }, // 332 IIC_SSE_MOVD_ToGP_WriteMove
  { 1, 4, 5, 0, 0 }, // 333 IIC_SSE_MOVQ_RR_WriteVecLogic
  { 1, 21, 22, 0, 0 }, // 334 IIC_MOVS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 335 IIC_SSE_MOV_S_MR_WriteStore
  { 1, 5, 6, 0, 0 }, // 336 IIC_SSE_MOV_S_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 337 IIC_SSE_MOV_S_RR_WriteFShuffle
  { 1, 21, 22, 0, 0 }, // 338 IIC_MOVSX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 339 IIC_MOVSX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 340 IIC_MOVSX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 341 IIC_MOVSX_WriteALU
  { 1, 4, 5, 0, 0 }, // 342 IIC_SSE_MOVU_P_RR_WriteFShuffle
  { 1, 5, 6, 0, 0 }, // 343 IIC_SSE_MOVDQ_WriteVecLogicLd
  { 1, 21, 22, 0, 0 }, // 344 IIC_MOVZX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 345 IIC_MOVZX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 346 IIC_MOVZX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 347 IIC_MOVZX_WriteALU
  { 0, 0, 0, 0, 0 }, // 348 IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 349 IIC_SSE_MPSADBW_RR_WriteMPSAD
  { 1, 28, 29, 0, 0 }, // 350 IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 351 IIC_MUL16_REG_WriteIMul
  { 1, 2, 3, 0, 0 }, // 352 IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 353 IIC_MUL32_REG_WriteIMul
  { 1, 58, 59, 0, 0 }, // 354 IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 355 IIC_MUL64_WriteIMul
  { 1, 2, 3, 0, 0 }, // 356 IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 357 IIC_MUL8_WriteIMul
  { 1, 26, 27, 0, 0 }, // 358 IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 359 IIC_SSE_MUL_F64P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 360 IIC_SSE_MUL_F32P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 361 IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 362 IIC_SSE_MUL_F64S_RR_WriteFMul
  { 1, 75, 76, 0, 0 }, // 363 IIC_SSE_MUL_F32S_RR_WriteFMul
  { 1, 2, 3, 0, 0 }, // 364 IIC_MUL8_WriteIMulLd_WriteIMulH
  { 1, 2, 3, 0, 0 }, // 365 IIC_MUL8_WriteIMul_WriteIMulH
  { 0, 0, 0, 0, 0 }, // 366 WriteFMulLd
  { 0, 0, 0, 0, 0 }, // 367 WriteFMul
  { 1, 50, 51, 0, 0 }, // 368 IIC_SSE_MWAIT_WriteSystem
  { 1, 4, 5, 0, 0 }, // 369 IIC_NOP_WriteZero
  { 1, 81, 82, 0, 0 }, // 370 IIC_OUT_IR_WriteSystem
  { 1, 32, 33, 0, 0 }, // 371 IIC_OUT_RR_WriteSystem
  { 1, 82, 83, 0, 0 }, // 372 IIC_OUTS_WriteSystem
  { 1, 5, 6, 0, 0 }, // 373 IIC_SSE_PABS_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 374 IIC_SSE_PABS_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 375 IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 376 IIC_SSE_INTALU_P_RR_WriteVecALU
  { 1, 21, 22, 0, 0 }, // 377 IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 378 IIC_SSE_INTALUQ_P_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 379 IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 380 IIC_SSE_PALIGNRR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 381 IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 382 IIC_SSE_BIT_P_RR_WriteVecLogic
  { 1, 71, 72, 0, 0 }, // 383 IIC_SSE_PAUSE_WriteNop
  { 1, 5, 6, 0, 0 }, // 384 IIC_ALU_MEM_WriteVarBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 385 IIC_ALU_NONMEM_WriteVarBlend
  { 1, 5, 6, 0, 0 }, // 386 IIC_SSE_INTALU_P_RM_WriteBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 387 IIC_SSE_INTALU_P_RR_WriteBlend
  { 0, 0, 0, 0, 0 }, // 388 IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 389 IIC_SSE_PCLMULQDQ_RR_WriteCLMul
  { 0, 0, 0, 0, 0 }, // 390 WriteVecALULd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 391 WriteVecALU
  { 0, 0, 0, 0, 0 }, // 392 WritePCmpEStrILd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 393 WritePCmpEStrI
  { 0, 0, 0, 0, 0 }, // 394 WritePCmpEStrMLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 395 WritePCmpEStrM
  { 0, 0, 0, 0, 0 }, // 396 WritePCmpIStrILd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 397 WritePCmpIStrI
  { 0, 0, 0, 0, 0 }, // 398 WritePCmpIStrMLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 399 WritePCmpIStrM
  { 0, 0, 0, 0, 0 }, // 400 WriteShuffleLd_WriteRMW
  { 1, 20, 21, 0, 0 }, // 401 IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 402 IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 403 IIC_SSE_PHADDSUBD_RR_WriteVecALU
  { 1, 28, 29, 0, 0 }, // 404 IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 405 IIC_SSE_PHADDSUBW_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 406 WriteVecIMulLd
  { 0, 0, 0, 0, 0 }, // 407 WriteVecIMul
  { 1, 5, 6, 0, 0 }, // 408 IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 409 IIC_SSE_PINSRW_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 410 WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 411 IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 412 IIC_SSE_PMADD_WriteVecIMul
  { 1, 5, 6, 0, 0 }, // 413 IIC_SSE_INTALU_P_RM_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 414 IIC_SSE_INTALU_P_RR_WriteShuffle
  { 1, 57, 58, 0, 0 }, // 415 IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 416 IIC_SSE_INTMUL_P_RR_WriteVecIMul
  { 1, 16, 17, 0, 0 }, // 417 IIC_POP_REG16_WriteLoad
  { 1, 21, 22, 0, 0 }, // 418 IIC_POP_MEM_WriteLoad
  { 1, 18, 20, 0, 0 }, // 419 IIC_POP_REG_WriteLoad
  { 1, 14, 15, 0, 0 }, // 420 IIC_POP_A_WriteLoad
  { 0, 0, 0, 0, 0 }, // 421 IIC_SSE_POPCNT_RM_WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 422 IIC_SSE_POPCNT_RR_WriteFAdd
  { 1, 83, 84, 0, 0 }, // 423 IIC_POP_SR_WriteSystem
  { 1, 38, 39, 0, 0 }, // 424 IIC_POP_F_WriteLoad
  { 1, 77, 78, 0, 0 }, // 425 IIC_POP_FD_WriteLoad
  { 1, 84, 85, 0, 0 }, // 426 IIC_POP_SR_SS_WriteSystem
  { 1, 43, 44, 0, 0 }, // 427 IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 428 IIC_SSE_PSHUFB_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 429 IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 430 IIC_SSE_PSHUF_RI_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 431 IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 432 IIC_SSE_PSIGN_RR_WriteVecALU
  { 1, 4, 5, 0, 0 }, // 433 IIC_SSE_INTSHDQ_P_RI_WriteVecShift
  { 1, 4, 5, 0, 0 }, // 434 IIC_SSE_INTSH_P_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 435 IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 436 IIC_SSE_INTSH_P_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 437 IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 438 IIC_SSE_UNPCK_WriteShuffle
  { 1, 18, 20, 0, 0 }, // 439 IIC_PUSH_IMM_WriteStore
  { 1, 18, 20, 0, 0 }, // 440 IIC_PUSH_REG_WriteStore
  { 1, 16, 17, 0, 0 }, // 441 IIC_PUSH_MEM_WriteRMW
  { 1, 28, 29, 0, 0 }, // 442 IIC_PUSH_A_WriteStore
  { 1, 16, 17, 0, 0 }, // 443 IIC_PUSH_SR_WriteSystem
  { 1, 16, 17, 0, 0 }, // 444 IIC_PUSH_CS_WriteSystem
  { 1, 14, 15, 0, 0 }, // 445 IIC_PUSH_F_WriteStore
  { 1, 5, 6, 0, 0 }, // 446 IIC_SR_WriteShiftLd_WriteRMW
  { 1, 5, 6, 0, 0 }, // 447 IIC_SR_WriteShift
  { 1, 26, 27, 0, 0 }, // 448 IIC_SSE_RCPP_RM_WriteFRcpLd
  { 1, 14, 15, 0, 0 }, // 449 IIC_SSE_RCPP_RR_WriteFRcp
  { 1, 75, 76, 0, 0 }, // 450 IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 451 WriteFRcpLd_ReadAfterLd
  { 1, 75, 76, 0, 0 }, // 452 IIC_SSE_RCPS_RR_WriteFRcp
  { 0, 0, 0, 0, 0 }, // 453 WriteRMW
  { 1, 85, 86, 0, 0 }, // 454 IIC_RDMSR_WriteSystem
  { 1, 50, 51, 0, 0 }, // 455 IIC_RDPMC_WriteSystem
  { 1, 86, 87, 0, 0 }, // 456 IIC_RDTSC_WriteSystem
  { 1, 87, 88, 0, 0 }, // 457 IIC_REP_MOVS_WriteMicrocoded
  { 1, 82, 83, 0, 0 }, // 458 IIC_REP_STOS_WriteMicrocoded
  { 1, 18, 20, 0, 0 }, // 459 IIC_RET_IMM_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 460 WriteShiftLd
  { 0, 0, 0, 0, 0 }, // 461 WriteShift
  { 0, 0, 0, 0, 0 }, // 462 IIC_SSE_ROUNDPS_REG_WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 463 IIC_SSE_ROUNDPS_REG_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 464 IIC_SSE_ROUNDPS_MEM_WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 465 WriteFAddLd_ReadAfterLd
  { 1, 88, 89, 0, 0 }, // 466 IIC_RSM_WriteSystem
  { 1, 26, 27, 0, 0 }, // 467 IIC_SSE_RSQRTPS_RM_WriteFRsqrtLd
  { 1, 14, 15, 0, 0 }, // 468 IIC_SSE_RSQRTPS_RR_WriteFRsqrt
  { 1, 75, 76, 0, 0 }, // 469 IIC_SSE_RSQRTSS_RM_WriteFRsqrtLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 470 WriteFRsqrtLd_ReadAfterLd
  { 1, 75, 76, 0, 0 }, // 471 IIC_SSE_RSQRTSS_RR_WriteFRsqrt
  { 0, 0, 0, 0, 0 }, // 472 WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 473 IIC_SCAS_WriteMicrocoded
  { 1, 16, 17, 0, 0 }, // 474 IIC_SET_M_WriteALU_WriteStore
  { 1, 4, 5, 0, 0 }, // 475 IIC_SET_R_WriteALU
  { 1, 5, 6, 0, 0 }, // 476 IIC_SSE_SFENCE_WriteFence
  { 1, 20, 21, 0, 0 }, // 477 IIC_SGDT_WriteSystem
  { 1, 6, 7, 0, 0 }, // 478 IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 479 IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 480 IIC_SHD16_REG_CL_WriteShift
  { 1, 6, 7, 0, 0 }, // 481 IIC_SHD16_REG_IM_WriteShift
  { 1, 20, 21, 0, 0 }, // 482 IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 20, 21, 0, 0 }, // 483 IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 484 IIC_SHD32_REG_CL_WriteShift
  { 1, 16, 17, 0, 0 }, // 485 IIC_SHD32_REG_IM_WriteShift
  { 1, 14, 15, 0, 0 }, // 486 IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 487 IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 28, 29, 0, 0 }, // 488 IIC_SHD64_REG_CL_WriteShift
  { 1, 14, 15, 0, 0 }, // 489 IIC_SHD64_REG_IM_WriteShift
  { 1, 5, 6, 0, 0 }, // 490 IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 491 IIC_SSE_SHUFP_WriteFShuffle
  { 1, 20, 21, 0, 0 }, // 492 IIC_SIDT_WriteSystem
  { 1, 21, 22, 0, 0 }, // 493 IIC_SLDT_WriteSystem
  { 1, 14, 15, 0, 0 }, // 494 IIC_SMSW_WriteSystem
  { 1, 33, 34, 0, 0 }, // 495 IIC_SSE_SQRTPD_RM_WriteFSqrtLd
  { 1, 33, 34, 0, 0 }, // 496 IIC_SSE_SQRTPD_RR_WriteFSqrt
  { 1, 34, 35, 0, 0 }, // 497 IIC_SSE_SQRTPS_RM_WriteFSqrtLd
  { 1, 34, 35, 0, 0 }, // 498 IIC_SSE_SQRTPS_RR_WriteFSqrt
  { 1, 35, 36, 0, 0 }, // 499 IIC_SSE_SQRTSD_RM_WriteFSqrtLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 500 WriteFSqrtLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 501 IIC_SSE_SQRTSD_RR_WriteFSqrt
  { 1, 36, 37, 0, 0 }, // 502 IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd
  { 1, 36, 37, 0, 0 }, // 503 IIC_SSE_SQRTSS_RR_WriteFSqrt
  { 0, 0, 0, 0, 0 }, // 504 WriteFSqrt
  { 1, 4, 5, 0, 0 }, // 505 IIC_STC_WriteALU
  { 1, 3, 4, 0, 0 }, // 506 IIC_STD_WriteALU
  { 1, 71, 72, 0, 0 }, // 507 IIC_STI_WriteALU
  { 1, 17, 18, 0, 0 }, // 508 IIC_SSE_STMXCSR_WriteStore
  { 1, 4, 5, 0, 0 }, // 509 IIC_STOS_WriteMicrocoded
  { 1, 21, 22, 0, 0 }, // 510 IIC_STR_WriteSystem
  { 1, 43, 44, 0, 0 }, // 511 IIC_FST80_WriteStore
  { 1, 16, 17, 0, 0 }, // 512 IIC_FST_WriteMove
  { 1, 24, 25, 0, 0 }, // 513 IIC_SWAPGS_WriteSystem
  { 1, 89, 90, 0, 0 }, // 514 IIC_SYSCALL_WriteSystem
  { 1, 80, 81, 0, 0 }, // 515 IIC_SYS_ENTER_EXIT_WriteSystem
  { 1, 15, 16, 0, 0 }, // 516 IIC_JMP_REL_WriteJumpLd
  { 1, 16, 17, 0, 0 }, // 517 IIC_JMP_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 518 IIC_JMP_REG_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 519 WriteJump
  { 1, 14, 15, 0, 0 }, // 520 IIC_FUCOMI_WriteFAdd
  { 1, 15, 16, 0, 0 }, // 521 IIC_FUCOM_WriteFAdd
  { 1, 5, 6, 0, 0 }, // 522 IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 523 IIC_SSE_UNPCK_WriteFShuffle
  { 1, 9, 10, 0, 0 }, // 524 IIC_SSE_ALU_F64S_RR
  { 1, 9, 10, 0, 0 }, // 525 IIC_SSE_ALU_F32S_RR
  { 0, 0, 0, 0, 0 }, // 526 WriteFVarBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 527 WriteFVarBlend
  { 0, 0, 0, 0, 0 }, // 528 WriteFShuffleLd
  { 0, 0, 0, 0, 0 }, // 529 WriteFShuffle256
  { 0, 0, 0, 0, 0 }, // 530 WriteFShuffle
  { 1, 7, 9, 0, 0 }, // 531 IIC_SSE_ALU_F32P_RM
  { 0, 0, 0, 0, 0 }, // 532 WriteCvtF2ILd
  { 0, 0, 0, 0, 0 }, // 533 WriteCvtF2I
  { 0, 0, 0, 0, 0 }, // 534 WriteCvtF2FLd
  { 0, 0, 0, 0, 0 }, // 535 WriteCvtF2F
  { 0, 0, 0, 0, 0 }, // 536 WriteCvtF2FLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 537 WriteCvtF2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 538 WriteCvtI2FLd_ReadAfterLd
  { 1, 90, 91, 0, 0 }, // 539 IIC_VERR_WriteSystem
  { 1, 91, 92, 0, 0 }, // 540 IIC_VERW_REG_WriteSystem
  { 1, 30, 31, 0, 0 }, // 541 IIC_VERW_MEM_WriteSystem
  { 0, 0, 0, 0, 0 }, // 542 WriteStore
  { 0, 0, 0, 0, 0 }, // 543 WriteShuffle256
  { 1, 5, 6, 0, 0 }, // 544 IIC_ALU_MEM_WriteFBlendLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 545 WriteFShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 546 WriteShuffle256Ld_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 547 IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 548 IIC_ALU_NONMEM_WriteFShuffle
  { 1, 5, 6, 0, 0 }, // 549 IIC_SSE_MOVDQ
  { 1, 5, 6, 0, 0 }, // 550 IIC_SSE_MOV_LH
  { 1, 74, 75, 0, 0 }, // 551 IIC_SSE_MOVD_ToGP
  { 1, 5, 6, 0, 0 }, // 552 IIC_SSE_MOV_S_MR
  { 1, 5, 6, 0, 0 }, // 553 IIC_SSE_MOV_S_RM
  { 1, 4, 5, 0, 0 }, // 554 IIC_SSE_MOV_S_RR
  { 1, 4, 5, 0, 0 }, // 555 IIC_SSE_MOVQ_RR
  { 1, 5, 6, 0, 0 }, // 556 IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 557 IIC_ALU_NONMEM_WriteMPSAD
  { 0, 0, 0, 0, 0 }, // 558 WriteVecALULd
  { 0, 0, 0, 0, 0 }, // 559 WriteBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 560 WriteBlend
  { 0, 0, 0, 0, 0 }, // 561 WriteVarBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 562 WriteVarBlend
  { 1, 5, 6, 0, 0 }, // 563 IIC_ALU_MEM_WriteBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 564 IIC_ALU_NONMEM_WriteBlend
  { 0, 0, 0, 0, 0 }, // 565 WriteCLMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 566 WriteCLMul
  { 1, 9, 10, 0, 0 }, // 567 IIC_SSE_ALU_F32P_RR
  { 0, 0, 0, 0, 0 }, // 568 WriteFShuffle256Ld_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 569 IIC_ALU_MEM_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 570 IIC_ALU_NONMEM_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 571 WriteVarVecShift_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 572 WriteVarVecShift
  { 1, 5, 6, 0, 0 }, // 573 IIC_SSE_PSHUF_MI_WriteShuffleLd
  { 0, 0, 0, 0, 0 }, // 574 WriteVecShift
  { 0, 0, 0, 0, 0 }, // 575 WriteVarVecShiftLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 576 WriteFRcpLd
  { 0, 0, 0, 0, 0 }, // 577 WriteFRsqrtLd
  { 0, 0, 0, 0, 0 }, // 578 WriteFSqrtLd
  { 1, 4, 5, 0, 0 }, // 579 IIC_WAIT_WriteMicrocoded
  { 1, 92, 93, 0, 0 }, // 580 IIC_WRMSR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 581 IIC_XADD_MEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 582 IIC_XADD_REG_WriteALU
  { 1, 16, 17, 0, 0 }, // 583 IIC_XCHG_REG_WriteALU
  { 1, 21, 22, 0, 0 }, // 584 IIC_XCHG_MEM_WriteALULd_WriteRMW
  { 1, 18, 20, 0, 0 }, // 585 IIC_FXCH_WriteMove
  { 1, 6, 7, 0, 0 }, // 586 IIC_XLAT_WriteLoad
  { 1, 5, 6, 0, 0 }, // 587 MOV16rm
  { 1, 5, 6, 0, 0 }, // 588 MOVSX32rm16_MOVSX32rm8
  { 1, 5, 6, 0, 0 }, // 589 MOVZX32rm16_MOVZX32rm8
  { 1, 4, 5, 0, 0 }, // 590 CMOVA16rr_CMOVAE16rr_CMOVB16rr_CMOVBE16rr_CMOVE16rr_CMOVG16rr_CMOVGE16rr_CMOVL16rr_CMOVLE16rr_CMOVNE16rr_CMOVNO16rr_CMOVNP16rr_CMOVNS16rr_CMOVO16rr_CMOVP16rr_CMOVS16rr
  { 1, 4, 5, 0, 0 }, // 591 CMOVA32rr_CMOVA64rr_CMOVAE32rr_CMOVAE64rr_CMOVB32rr_CMOVB64rr_CMOVBE32rr_CMOVBE64rr_CMOVE32rr_CMOVE64rr_CMOVG32rr_CMOVG64rr_CMOVGE32rr_CMOVGE64rr_CMOVL32rr_CMOVL64rr_CMOVLE32rr_CMOVLE64rr_CMOVNE32rr_CMOVNE64rr_CMOVNO32rr_CMOVNO64rr_CMOVNP32rr_CMOVNP64rr_CMOVNS32rr_CMOVNS64rr_CMOVO32rr_CMOVO64rr_CMOVP32rr_CMOVP64rr_CMOVS32rr_CMOVS64rr
  { 1, 5, 6, 0, 0 }, // 592 CMOVA16rm_CMOVAE16rm_CMOVB16rm_CMOVBE16rm_CMOVE16rm_CMOVG16rm_CMOVGE16rm_CMOVL16rm_CMOVLE16rm_CMOVNE16rm_CMOVNO16rm_CMOVNP16rm_CMOVNS16rm_CMOVO16rm_CMOVP16rm_CMOVS16rm
  { 1, 5, 6, 0, 0 }, // 593 CMOVA32rm_CMOVA64rm_CMOVAE32rm_CMOVAE64rm_CMOVB32rm_CMOVB64rm_CMOVBE32rm_CMOVBE64rm_CMOVE32rm_CMOVE64rm_CMOVG32rm_CMOVG64rm_CMOVGE32rm_CMOVGE64rm_CMOVL32rm_CMOVL64rm_CMOVLE32rm_CMOVLE64rm_CMOVNE32rm_CMOVNE64rm_CMOVNO32rm_CMOVNO64rm_CMOVNP32rm_CMOVNP64rm_CMOVNS32rm_CMOVNS64rm_CMOVO32rm_CMOVO64rm_CMOVP32rm_CMOVP64rm_CMOVS32rm_CMOVS64rm
  { 1, 16, 17, 0, 0 }, // 594 XCHG16ar_XCHG16rr_XCHG32ar_XCHG32ar64_XCHG32rr_XCHG64ar_XCHG64rr_XCHG8rr
  { 1, 21, 22, 0, 0 }, // 595 XCHG16rm_XCHG32rm_XCHG64rm_XCHG8rm
  { 1, 6, 7, 0, 0 }, // 596 XLAT
  { 1, 16, 17, 0, 0 }, // 597 PUSH16rmm_PUSH32rmm
  { 1, 14, 15, 0, 0 }, // 598 PUSHF16_PUSHF32
  { 1, 28, 29, 0, 0 }, // 599 PUSHA16_PUSHA32
  { 1, 21, 22, 0, 0 }, // 600 POP16rmm_POP32rmm
  { 1, 38, 39, 0, 0 }, // 601 POPF16
  { 1, 77, 78, 0, 0 }, // 602 POPF32
  { 1, 14, 15, 0, 0 }, // 603 POPA16_POPA32
  { 1, 4, 5, 0, 0 }, // 604 LAHF_SAHF
  { 1, 5, 6, 0, 0 }, // 605 BSWAP32r
  { 1, 5, 6, 0, 0 }, // 606 BSWAP64r
  { 1, 5, 6, 0, 0 }, // 607 MOVBE16rm_MOVBE64rm
  { 1, 5, 6, 0, 0 }, // 608 MOVBE32rm
  { 1, 5, 6, 0, 0 }, // 609 MOVBE16mr
  { 1, 5, 6, 0, 0 }, // 610 MOVBE32mr
  { 1, 5, 6, 0, 0 }, // 611 MOVBE64mr
  { 1, 5, 6, 0, 0 }, // 612 ADD16mi_ADD16mi8_ADD16mr_ADD32mi_ADD32mi8_ADD32mr_ADD64mi32_ADD64mi8_ADD64mr_ADD8mi_ADD8mi8_ADD8mr_SUB16mi_SUB16mi8_SUB16mr_SUB32mi_SUB32mi8_SUB32mr_SUB64mi32_SUB64mi8_SUB64mr_SUB8mi_SUB8mi8_SUB8mr
  { 1, 4, 5, 0, 0 }, // 613 ADC16ri_ADC16ri8_ADC16rr_ADC16rr_REV_ADC32ri_ADC32ri8_ADC32rr_ADC32rr_REV_ADC64ri32_ADC64ri8_ADC64rr_ADC64rr_REV_ADC8ri_ADC8ri8_ADC8rr_ADC8rr_REV_SBB16ri_SBB16ri8_SBB16rr_SBB16rr_REV_SBB32ri_SBB32ri8_SBB32rr_SBB32rr_REV_SBB64ri32_SBB64ri8_SBB64rr_SBB64rr_REV_SBB8ri_SBB8ri8_SBB8rr_SBB8rr_REV
  { 1, 5, 6, 0, 0 }, // 614 ADC16rm_ADC32rm_ADC64rm_ADC8rm_SBB16rm_SBB32rm_SBB64rm_SBB8rm
  { 1, 5, 6, 0, 0 }, // 615 ADC16mi_ADC16mi8_ADC16mr_ADC32mi_ADC32mi8_ADC32mr_ADC64mi32_ADC64mi8_ADC64mr_ADC8mi_ADC8mi8_ADC8mr_SBB16mi_SBB16mi8_SBB16mr_SBB32mi_SBB32mi8_SBB32mr_SBB64mi32_SBB64mi8_SBB64mr_SBB8mi_SBB8mi8_SBB8mr
  { 1, 5, 6, 0, 0 }, // 616 DEC16m_DEC32m_DEC64m_DEC8m_INC16m_INC32m_INC64m_INC8m_NEG16m_NEG32m_NEG64m_NEG8m_NOT16m_NOT32m_NOT64m_NOT8m
  { 1, 6, 7, 0, 0 }, // 617 IMUL16r_IMUL16rr
  { 1, 2, 3, 0, 0 }, // 618 IMUL16rm
  { 1, 2, 3, 0, 0 }, // 619 IMUL16rmi_IMUL16rmi8
  { 1, 6, 7, 0, 0 }, // 620 IMUL16rri_IMUL16rri8
  { 1, 2, 3, 0, 0 }, // 621 MUL16r
  { 1, 28, 29, 0, 0 }, // 622 IMUL16m
  { 1, 28, 29, 0, 0 }, // 623 MUL16m
  { 1, 57, 58, 0, 0 }, // 624 IMUL32r_IMUL32rr
  { 1, 57, 58, 0, 0 }, // 625 IMUL32rm
  { 1, 57, 58, 0, 0 }, // 626 IMUL32rmi_IMUL32rmi8
  { 1, 57, 58, 0, 0 }, // 627 IMUL32rri_IMUL32rri8
  { 1, 6, 7, 0, 0 }, // 628 MUL32r
  { 1, 2, 3, 0, 0 }, // 629 IMUL32m
  { 1, 2, 3, 0, 0 }, // 630 MUL32m
  { 1, 58, 59, 0, 0 }, // 631 IMUL64r_IMUL64rr
  { 1, 58, 59, 0, 0 }, // 632 IMUL64rm
  { 1, 22, 23, 0, 0 }, // 633 IMUL64rmi32_IMUL64rmi8
  { 1, 22, 23, 0, 0 }, // 634 IMUL64rri32_IMUL64rri8
  { 1, 58, 59, 0, 0 }, // 635 MUL64r
  { 1, 58, 59, 0, 0 }, // 636 IMUL64m
  { 1, 58, 59, 0, 0 }, // 637 MUL64m
  { 1, 2, 3, 0, 0 }, // 638 MULX32rr
  { 1, 2, 3, 0, 0 }, // 639 MULX32rm
  { 1, 2, 3, 0, 0 }, // 640 MULX64rr
  { 1, 2, 3, 0, 0 }, // 641 MULX64rm
  { 1, 30, 31, 0, 0 }, // 642 DIV8r
  { 1, 30, 31, 0, 0 }, // 643 DIV16r
  { 1, 30, 31, 0, 0 }, // 644 DIV32r
  { 1, 31, 32, 0, 0 }, // 645 DIV64r
  { 1, 35, 36, 0, 0 }, // 646 IDIV8r
  { 1, 35, 36, 0, 0 }, // 647 IDIV16r
  { 1, 35, 36, 0, 0 }, // 648 IDIV32r
  { 1, 31, 32, 0, 0 }, // 649 IDIV64r
  { 1, 5, 6, 0, 0 }, // 650 AND16mi_AND16mi8_AND16mr_AND32mi_AND32mi8_AND32mr_AND64mi32_AND64mi8_AND64mr_AND8mi_AND8mi8_AND8mr_OR16mi_OR16mi8_OR16mr_OR32mi_OR32mi8_OR32mr_OR64mi32_OR64mi8_OR64mr_OR8mi_OR8mi8_OR8mr_XOR16mi_XOR16mi8_XOR16mr_XOR32mi_XOR32mi8_XOR32mr_XOR64mi32_XOR64mi8_XOR64mr_XOR8mi_XOR8mi8_XOR8mr
  { 1, 5, 6, 0, 0 }, // 651 OR32mrLocked
  { 1, 5, 6, 0, 0 }, // 652 SAR16m1_SAR16mi_SAR32m1_SAR32mi_SAR64m1_SAR64mi_SAR8m1_SAR8mi_SHL16m1_SHL16mi_SHL32m1_SHL32mi_SHL64m1_SHL64mi_SHL8m1_SHL8mi_SHR16m1_SHR16mi_SHR32m1_SHR32mi_SHR64m1_SHR64mi_SHR8m1_SHR8mi
  { 1, 5, 6, 0, 0 }, // 653 SAR16rCL_SAR32rCL_SAR64rCL_SAR8rCL_SHL16rCL_SHL32rCL_SHL64rCL_SHL8rCL_SHR16rCL_SHR32rCL_SHR64rCL_SHR8rCL
  { 1, 5, 6, 0, 0 }, // 654 SAR16mCL_SAR32mCL_SAR64mCL_SAR8mCL_SHL16mCL_SHL32mCL_SHL64mCL_SHL8mCL_SHR16mCL_SHR32mCL_SHR64mCL_SHR8mCL
  { 1, 5, 6, 0, 0 }, // 655 ROL16r1_ROL32r1_ROL64r1_ROL8r1_ROR16r1_ROR32r1_ROR64r1_ROR8r1
  { 1, 5, 6, 0, 0 }, // 656 ROL16mi_ROL32mi_ROL64mi_ROL8mi_ROR16mi_ROR32mi_ROR64mi_ROR8mi
  { 1, 5, 6, 0, 0 }, // 657 ROL16rCL_ROL32rCL_ROL64rCL_ROL8rCL_ROR16rCL_ROR32rCL_ROR64rCL_ROR8rCL
  { 1, 5, 6, 0, 0 }, // 658 ROL16mCL_ROL32mCL_ROL64mCL_ROL8mCL_ROR16mCL_ROR32mCL_ROR64mCL_ROR8mCL
  { 1, 5, 6, 0, 0 }, // 659 RCL16r1_RCL32r1_RCL64r1_RCL8r1_RCR16r1_RCR32r1_RCR64r1_RCR8r1
  { 1, 5, 6, 0, 0 }, // 660 RCL16m1_RCL32m1_RCL64m1_RCL8m1_RCR16m1_RCR32m1_RCR64m1_RCR8m1
  { 1, 5, 6, 0, 0 }, // 661 RCL16rCL_RCL16ri_RCL32rCL_RCL32ri_RCL64rCL_RCL64ri_RCL8rCL_RCL8ri_RCR16rCL_RCR16ri_RCR32rCL_RCR32ri_RCR64rCL_RCR64ri_RCR8rCL_RCR8ri
  { 1, 5, 6, 0, 0 }, // 662 RCL16mCL_RCL16mi_RCL32mCL_RCL32mi_RCL64mCL_RCL64mi_RCL8mCL_RCL8mi_RCR16mCL_RCR16mi_RCR32mCL_RCR32mi_RCR64mCL_RCR64mi_RCR8mCL_RCR8mi
  { 1, 6, 7, 0, 0 }, // 663 SHLD16rri8_SHRD16rri8
  { 1, 16, 17, 0, 0 }, // 664 SHLD32rri8_SHRD32rri8
  { 1, 14, 15, 0, 0 }, // 665 SHLD64rri8_SHRD64rri8
  { 1, 6, 7, 0, 0 }, // 666 SHLD16mri8_SHRD16mri8
  { 1, 20, 21, 0, 0 }, // 667 SHLD32mri8_SHRD32mri8
  { 1, 14, 15, 0, 0 }, // 668 SHLD64mri8_SHRD64mri8
  { 1, 6, 7, 0, 0 }, // 669 SHLD16rrCL
  { 1, 16, 17, 0, 0 }, // 670 SHLD32rrCL
  { 1, 28, 29, 0, 0 }, // 671 SHLD64rrCL
  { 1, 6, 7, 0, 0 }, // 672 SHRD16rrCL
  { 1, 16, 17, 0, 0 }, // 673 SHRD32rrCL
  { 1, 28, 29, 0, 0 }, // 674 SHRD64rrCL
  { 1, 6, 7, 0, 0 }, // 675 SHLD16mrCL_SHRD16mrCL
  { 1, 20, 21, 0, 0 }, // 676 SHLD32mrCL_SHRD32mrCL
  { 1, 14, 15, 0, 0 }, // 677 SHLD64mrCL_SHRD64mrCL
  { 1, 15, 16, 0, 0 }, // 678 BT16ri8_BT32ri8_BT64ri8
  { 1, 15, 16, 0, 0 }, // 679 BT16rr_BT32rr_BT64rr
  { 1, 14, 15, 0, 0 }, // 680 BT16mr_BT32mr_BT64mr
  { 1, 4, 5, 0, 0 }, // 681 BT16mi8_BT32mi8_BT64mi8
  { 1, 15, 16, 0, 0 }, // 682 BTC16ri8_BTC32ri8_BTC64ri8_BTR16ri8_BTR32ri8_BTR64ri8_BTS16ri8_BTS32ri8_BTS64ri8
  { 1, 15, 16, 0, 0 }, // 683 BTC16rr_BTC32rr_BTC64rr_BTR16rr_BTR32rr_BTS16rr_BTS32rr_BTS64rr
  { 0, 0, 0, 0, 0 }, // 684 BTR64rr
  { 1, 12, 13, 0, 0 }, // 685 BTC16mr_BTC32mr_BTC64mr_BTR16mr_BTR32mr_BTR64mr_BTS16mr_BTS32mr_BTS64mr
  { 1, 16, 17, 0, 0 }, // 686 BTC16mi8_BTC32mi8_BTC64mi8_BTR16mi8_BTR32mi8_BTR64mi8_BTS16mi8_BTS32mi8_BTS64mi8
  { 1, 13, 14, 0, 0 }, // 687 BSF16rr_BSF32rr_BSF64rr_BSR16rr_BSR32rr_BSR64rr
  { 1, 13, 14, 0, 0 }, // 688 BSF16rm_BSF32rm_BSF64rm_BSR16rm_BSR32rm_BSR64rm
  { 1, 4, 5, 0, 0 }, // 689 SETAEr_SETAr_SETBEr_SETBr_SETEr_SETGEr_SETGr_SETLEr_SETLr_SETNEr_SETNOr_SETNPr_SETNSr_SETOr_SETPr_SETSr
  { 1, 16, 17, 0, 0 }, // 690 SETAEm_SETAm_SETBEm_SETBm_SETEm_SETGEm_SETGm_SETLEm_SETLm_SETNEm_SETNOm_SETNPm_SETNSm_SETOm_SETPm_SETSm
  { 1, 21, 22, 0, 0 }, // 691 CLD
  { 1, 3, 4, 0, 0 }, // 692 STD
  { 0, 0, 0, 0, 0 }, // 693 TZCNT16rr_TZCNT32rr_TZCNT64rr
  { 0, 0, 0, 0, 0 }, // 694 TZCNT16rm_TZCNT32rm_TZCNT64rm
  { 1, 4, 5, 0, 0 }, // 695 ANDN32rr_ANDN64rr
  { 1, 5, 6, 0, 0 }, // 696 ANDN32rm_ANDN64rm
  { 0, 0, 0, 0, 0 }, // 697 BLSI32rr_BLSI64rr_BLSMSK32rr_BLSMSK64rr_BLSR32rr_BLSR64rr
  { 0, 0, 0, 0, 0 }, // 698 BLSI32rm_BLSI64rm_BLSMSK32rm_BLSMSK64rm_BLSR32rm_BLSR64rm
  { 0, 0, 0, 0, 0 }, // 699 BEXTR32rr_BEXTR64rr
  { 0, 0, 0, 0, 0 }, // 700 BEXTR32rm_BEXTR64rm
  { 0, 0, 0, 0, 0 }, // 701 BZHI32rr_BZHI64rr
  { 0, 0, 0, 0, 0 }, // 702 BZHI32rm_BZHI64rm
  { 0, 0, 0, 0, 0 }, // 703 PDEP32rr_PDEP64rr_PEXT32rr_PEXT64rr
  { 0, 0, 0, 0, 0 }, // 704 PDEP32rm_PDEP64rm_PEXT32rm_PEXT64rm
  { 1, 20, 21, 0, 0 }, // 705 JCXZ_JRCXZ
  { 1, 25, 26, 0, 0 }, // 706 LOOP
  { 1, 28, 29, 0, 0 }, // 707 LOOPE
  { 1, 71, 72, 0, 0 }, // 708 LOOPNE
  { 1, 18, 20, 0, 0 }, // 709 CALL16r_CALL32r
  { 1, 17, 18, 0, 0 }, // 710 CALL16m_CALL32m
  { 1, 37, 38, 0, 0 }, // 711 LRETL_LRETQ_LRETW_RETL_RETQ_RETW
  { 1, 37, 38, 0, 0 }, // 712 LRETIL_LRETIQ_LRETIW
  { 1, 18, 20, 0, 0 }, // 713 RETIL_RETIQ_RETIW
  { 1, 12, 13, 0, 0 }, // 714 BOUNDS16rm_BOUNDS32rm
  { 0, 0, 0, 0, 0 }, // 715 INTO
  { 1, 16, 17, 0, 0 }, // 716 LODSB_LODSW
  { 1, 16, 17, 0, 0 }, // 717 LODSL_LODSQ
  { 1, 4, 5, 0, 0 }, // 718 STOSB_STOSL_STOSQ_STOSW
  { 1, 21, 22, 0, 0 }, // 719 MOVSB_MOVSL_MOVSQ_MOVSW
  { 1, 5, 6, 0, 0 }, // 720 MOVSLDUPrm
  { 1, 5, 6, 0, 0 }, // 721 MOVSLDUPrr
  { 1, 16, 17, 0, 0 }, // 722 SCASB_SCASL_SCASQ_SCASW
  { 1, 21, 22, 0, 0 }, // 723 CMPSB_CMPSL_CMPSQ_CMPSW
  { 1, 21, 22, 0, 0 }, // 724 XADD16rm_XADD32rm_XADD64rm_XADD8rm
  { 1, 22, 23, 0, 0 }, // 725 CMPXCHG16rm_CMPXCHG32rm_CMPXCHG64rm
  { 1, 6, 7, 0, 0 }, // 726 CMPXCHG8rm
  { 1, 25, 26, 0, 0 }, // 727 CMPXCHG8B
  { 1, 24, 25, 0, 0 }, // 728 CMPXCHG16B
  { 1, 71, 72, 0, 0 }, // 729 PAUSE
  { 1, 16, 17, 0, 0 }, // 730 LEAVE_LEAVE64
  { 0, 0, 0, 0, 0 }, // 731 XGETBV
  { 1, 86, 87, 0, 0 }, // 732 RDTSC
  { 0, 0, 0, 0, 0 }, // 733 RDTSCP
  { 1, 50, 51, 0, 0 }, // 734 RDPMC
  { 0, 0, 0, 0, 0 }, // 735 RDRAND16r_RDRAND32r_RDRAND64r
  { 1, 5, 6, 0, 0 }, // 736 LD_Frr
  { 1, 20, 21, 0, 0 }, // 737 LD_F80m
  { 0, 0, 0, 0, 0 }, // 738 FBLDm
  { 1, 16, 17, 0, 0 }, // 739 ST_FPrr_ST_Frr
  { 1, 43, 44, 0, 0 }, // 740 ST_FP80m
  { 0, 0, 0, 0, 0 }, // 741 FBSTPm
  { 1, 18, 20, 0, 0 }, // 742 XCH_F
  { 1, 7, 9, 0, 0 }, // 743 ILD_F16m_ILD_F32m_ILD_F64m
  { 1, 6, 7, 0, 0 }, // 744 IST_F16m_IST_F32m_IST_FP16m_IST_FP32m
  { 1, 4, 5, 0, 0 }, // 745 LD_F0
  { 1, 6, 7, 0, 0 }, // 746 LD_F1
  { 1, 26, 27, 0, 0 }, // 747 FLDPI
  { 0, 0, 0, 0, 0 }, // 748 CMOVBE_F_CMOVBE_Fp32_CMOVBE_Fp64_CMOVBE_Fp80_CMOVB_F_CMOVB_Fp32_CMOVB_Fp64_CMOVB_Fp80_CMOVNBE_F_CMOVNBE_Fp32_CMOVNBE_Fp64_CMOVNBE_Fp80_CMOVNB_F_CMOVNB_Fp32_CMOVNB_Fp64_CMOVNB_Fp80_CMOVNE_F_CMOVNE_Fp32_CMOVNE_Fp64_CMOVNE_Fp80_CMOVNP_F_CMOVNP_Fp32_CMOVNP_Fp64_CMOVNP_Fp80_CMOVP_F_CMOVP_Fp32_CMOVP_Fp64_CMOVP_Fp80
  { 1, 26, 27, 0, 0 }, // 749 FNSTSW16r
  { 0, 0, 0, 0, 0 }, // 750 FNSTSWm
  { 1, 43, 44, 0, 0 }, // 751 FLDCW16m
  { 1, 28, 29, 0, 0 }, // 752 FNSTCW16m
  { 1, 4, 5, 0, 0 }, // 753 FDECSTP_FINCSTP
  { 1, 4, 5, 0, 0 }, // 754 FFREE
  { 0, 0, 0, 0, 0 }, // 755 FSAVEm
  { 0, 0, 0, 0, 0 }, // 756 FRSTORm
  { 0, 0, 0, 0, 0 }, // 757 ABS_F_ABS_Fp32_ABS_Fp64_ABS_Fp80
  { 0, 0, 0, 0, 0 }, // 758 CHS_F_CHS_Fp32_CHS_Fp64_CHS_Fp80
  { 0, 0, 0, 0, 0 }, // 759 COMP_FST0r_COM_FST0r
  { 1, 15, 16, 0, 0 }, // 760 UCOM_FPr_UCOM_Fr
  { 0, 0, 0, 0, 0 }, // 761 FCOM32m_FCOM64m_FCOMP32m_FCOMP64m
  { 1, 15, 16, 0, 0 }, // 762 FCOMPP
  { 1, 15, 16, 0, 0 }, // 763 UCOM_FPPr
  { 1, 14, 15, 0, 0 }, // 764 COM_FIPr_COM_FIr
  { 1, 14, 15, 0, 0 }, // 765 UCOM_FIPr_UCOM_FIr
  { 0, 0, 0, 0, 0 }, // 766 FICOM16m_FICOM32m_FICOMP16m_FICOMP32m
  { 0, 0, 0, 0, 0 }, // 767 TST_F_TST_Fp32_TST_Fp64_TST_Fp80
  { 1, 5, 6, 0, 0 }, // 768 FXAM
  { 1, 47, 48, 0, 0 }, // 769 FPREM
  { 1, 48, 49, 0, 0 }, // 770 FPREM1
  { 1, 50, 51, 0, 0 }, // 771 FRNDINT
  { 1, 51, 52, 0, 0 }, // 772 FSCALE
  { 1, 44, 45, 0, 0 }, // 773 FXTRACT
  { 1, 4, 5, 0, 0 }, // 774 FNOP
  { 1, 4, 5, 0, 0 }, // 775 WAIT
  { 1, 44, 45, 0, 0 }, // 776 FNCLEX
  { 1, 45, 46, 0, 0 }, // 777 FNINIT
  { 1, 74, 75, 0, 0 }, // 778 MMX_MOVD64from64rr_MMX_MOVD64grr
  { 1, 74, 75, 0, 0 }, // 779 MOVPDI2DIrr_VMOVPDI2DIrr
  { 1, 5, 6, 0, 0 }, // 780 MMX_MOVD64rr_MMX_MOVD64to64rr
  { 1, 5, 6, 0, 0 }, // 781 MOVDI2PDIrr_VMOVDI2PDIrr
  { 1, 74, 75, 0, 0 }, // 782 VMOVPQIto64rr
  { 1, 5, 6, 0, 0 }, // 783 VMOV64toPQIrr
  { 1, 4, 5, 0, 0 }, // 784 MMX_MOVQ64rr_MMX_MOVQ64rr_REV
  { 1, 4, 5, 0, 0 }, // 785 MOVDQArr_MOVDQArr_REV_VMOVDQAYrr_VMOVDQAYrr_REV_VMOVDQArr_VMOVDQArr_REV
  { 1, 4, 5, 0, 0 }, // 786 MOVDQUrr_MOVDQUrr_REV_VMOVDQUYrr_VMOVDQUYrr_REV_VMOVDQUrr_VMOVDQUrr_REV
  { 1, 4, 5, 0, 0 }, // 787 MMX_MOVDQ2Qrr
  { 0, 0, 0, 0, 0 }, // 788 MMX_MOVQ2DQrr
  { 1, 4, 5, 0, 0 }, // 789 MMX_PACKSSDWirr_MMX_PACKSSWBirr_MMX_PACKUSWBirr
  { 1, 5, 6, 0, 0 }, // 790 MMX_PACKSSDWirm_MMX_PACKSSWBirm_MMX_PACKUSWBirm
  { 1, 4, 5, 0, 0 }, // 791 VPMOVSXBQYrr_VPMOVSXBWYrr_VPMOVSXDQYrr_VPMOVZXBQYrr_VPMOVZXBWYrr_VPMOVZXDQYrr
  { 1, 4, 5, 0, 0 }, // 792 PBLENDWrri
  { 1, 4, 5, 0, 0 }, // 793 VPBLENDWYrri_VPBLENDWrri
  { 1, 5, 6, 0, 0 }, // 794 PBLENDWrmi
  { 1, 5, 6, 0, 0 }, // 795 VPBLENDWYrmi_VPBLENDWrmi
  { 0, 0, 0, 0, 0 }, // 796 VPBLENDDYrri_VPBLENDDrri
  { 0, 0, 0, 0, 0 }, // 797 VPBLENDDYrmi_VPBLENDDrmi
  { 1, 5, 6, 0, 0 }, // 798 MMX_MASKMOVQ_MMX_MASKMOVQ64
  { 1, 16, 17, 0, 0 }, // 799 MASKMOVDQU_MASKMOVDQU64_VMASKMOVDQU_VMASKMOVDQU64
  { 0, 0, 0, 0, 0 }, // 800 VPMASKMOVDYrm_VPMASKMOVDrm_VPMASKMOVQYrm_VPMASKMOVQrm
  { 0, 0, 0, 0, 0 }, // 801 VPMASKMOVDYmr_VPMASKMOVDmr_VPMASKMOVQYmr_VPMASKMOVQmr
  { 0, 0, 0, 0, 0 }, // 802 MMX_PMOVMSKBrr
  { 1, 74, 75, 0, 0 }, // 803 PMOVMSKBrr_VPMOVMSKBrr
  { 0, 0, 0, 0, 0 }, // 804 VPMOVMSKBYrr
  { 1, 20, 21, 0, 0 }, // 805 MMX_PEXTRWirri
  { 0, 0, 0, 0, 0 }, // 806 PEXTRBrr_PEXTRDrr_PEXTRQrr_PEXTRWrr_REV
  { 0, 0, 0, 0, 0 }, // 807 PEXTRBmr_PEXTRDmr_PEXTRQmr_PEXTRWmr
  { 0, 0, 0, 0, 0 }, // 808 VPBROADCASTBrm_VPBROADCASTWrm
  { 0, 0, 0, 0, 0 }, // 809 VPBROADCASTBYrm_VPBROADCASTWYrm
  { 0, 0, 0, 0, 0 }, // 810 VPGATHERDDrm
  { 0, 0, 0, 0, 0 }, // 811 VPGATHERDDYrm
  { 0, 0, 0, 0, 0 }, // 812 VPGATHERQDrm
  { 0, 0, 0, 0, 0 }, // 813 VPGATHERQDYrm
  { 0, 0, 0, 0, 0 }, // 814 VPGATHERDQrm
  { 0, 0, 0, 0, 0 }, // 815 VPGATHERDQYrm
  { 0, 0, 0, 0, 0 }, // 816 VPGATHERQQrm
  { 0, 0, 0, 0, 0 }, // 817 VPGATHERQQYrm
  { 1, 43, 44, 0, 0 }, // 818 MMX_PHADDSWrr64_MMX_PHADDWrr64_MMX_PHSUBSWrr64_MMX_PHSUBWrr64
  { 1, 21, 22, 0, 0 }, // 819 MMX_PHADDrr64_MMX_PHSUBDrr64
  { 1, 21, 22, 0, 0 }, // 820 PHADDDrr_PHSUBDrr_VPHADDDrr_VPHSUBDrr
  { 0, 0, 0, 0, 0 }, // 821 PHADDSWrr128_PHSUBSWrr128_VPHADDSWrr128_VPHADDSWrr256_VPHSUBSWrr128_VPHSUBSWrr256
  { 1, 2, 3, 0, 0 }, // 822 PHADDWrr_PHSUBWrr_VPHADDDYrr_VPHADDWYrr_VPHADDWrr_VPHSUBDYrr_VPHSUBWYrr_VPHSUBWrr
  { 1, 6, 7, 0, 0 }, // 823 MMX_PHADDSWrm64_MMX_PHADDWrm64_MMX_PHSUBSWrm64_MMX_PHSUBWrm64
  { 1, 20, 21, 0, 0 }, // 824 MMX_PHADDrm64_MMX_PHSUBDrm64
  { 1, 20, 21, 0, 0 }, // 825 PHADDDrm_PHSUBDrm_VPHADDDrm_VPHSUBDrm
  { 0, 0, 0, 0, 0 }, // 826 PHADDSWrm128_PHSUBSWrm128_VPHADDSWrm128_VPHADDSWrm256_VPHSUBSWrm128_VPHSUBSWrm256
  { 1, 28, 29, 0, 0 }, // 827 PHADDWrm_PHSUBWrm_VPHADDDYrm_VPHADDWYrm_VPHADDWrm_VPHSUBDYrm_VPHSUBWYrm_VPHSUBWrm
  { 0, 0, 0, 0, 0 }, // 828 PCMPGTQrr_VPCMPGTQYrr_VPCMPGTQrr
  { 0, 0, 0, 0, 0 }, // 829 PCMPGTQrm_VPCMPGTQYrm_VPCMPGTQrm
  { 0, 0, 0, 0, 0 }, // 830 PMULLDrr_VPMULLDYrr_VPMULLDrr
  { 0, 0, 0, 0, 0 }, // 831 PMULLDrm_VPMULLDYrm_VPMULLDrm
  { 0, 0, 0, 0, 0 }, // 832 PTESTrr_VPTESTYrr_VPTESTrr
  { 0, 0, 0, 0, 0 }, // 833 PTESTrm_VPTESTYrm_VPTESTrm
  { 1, 16, 17, 0, 0 }, // 834 PSLLDrr_PSLLQrr_PSLLWrr_PSRADrr_PSRAWrr_PSRLDrr_PSRLQrr_PSRLWrr_VPSLLDYrr_VPSLLDrr_VPSLLQYrr_VPSLLQrr_VPSLLWYrr_VPSLLWrr_VPSRADYrr_VPSRADrr_VPSRAWYrr_VPSRAWrr_VPSRLDYrr_VPSRLDrr_VPSRLQYrr_VPSRLQrr_VPSRLWYrr_VPSRLWrr
  { 1, 4, 5, 0, 0 }, // 835 PSLLDQri_PSRLDQri
  { 0, 0, 0, 0, 0 }, // 836 VPSLLDQYri_VPSLLDQri_VPSRLDQYri_VPSRLDQri
  { 1, 43, 44, 0, 0 }, // 837 MMX_EMMS
  { 1, 74, 75, 0, 0 }, // 838 MOVMSKPDrr_MOVMSKPSrr_VMOVMSKPDrr_VMOVMSKPSrr
  { 1, 74, 75, 0, 0 }, // 839 VMOVMSKPDYrr_VMOVMSKPSYrr
  { 0, 0, 0, 0, 0 }, // 840 VPERM2F128rr
  { 0, 0, 0, 0, 0 }, // 841 VPERM2F128rm
  { 1, 4, 5, 0, 0 }, // 842 BLENDVPDrr0_BLENDVPSrr0
  { 1, 5, 6, 0, 0 }, // 843 BLENDVPDrm0_BLENDVPSrm0
  { 0, 0, 0, 0, 0 }, // 844 VBROADCASTF128
  { 0, 0, 0, 0, 0 }, // 845 EXTRACTPSrr
  { 1, 4, 5, 0, 0 }, // 846 VEXTRACTPSrr
  { 0, 0, 0, 0, 0 }, // 847 EXTRACTPSmr
  { 1, 5, 6, 0, 0 }, // 848 VEXTRACTPSmr
  { 0, 0, 0, 0, 0 }, // 849 VEXTRACTF128rr
  { 0, 0, 0, 0, 0 }, // 850 VEXTRACTF128mr
  { 0, 0, 0, 0, 0 }, // 851 VINSERTF128rr
  { 0, 0, 0, 0, 0 }, // 852 VINSERTF128rm
  { 0, 0, 0, 0, 0 }, // 853 VMASKMOVPDYrm_VMASKMOVPDrm_VMASKMOVPSYrm_VMASKMOVPSrm
  { 0, 0, 0, 0, 0 }, // 854 VMASKMOVPDmr_VMASKMOVPSmr
  { 0, 0, 0, 0, 0 }, // 855 VMASKMOVPDYmr_VMASKMOVPSYmr
  { 0, 0, 0, 0, 0 }, // 856 VGATHERDPSrm
  { 0, 0, 0, 0, 0 }, // 857 VGATHERDPSYrm
  { 0, 0, 0, 0, 0 }, // 858 VGATHERQPSrm
  { 0, 0, 0, 0, 0 }, // 859 VGATHERQPSYrm
  { 0, 0, 0, 0, 0 }, // 860 VGATHERDPDrm
  { 0, 0, 0, 0, 0 }, // 861 VGATHERDPDYrm
  { 0, 0, 0, 0, 0 }, // 862 VGATHERQPDrm
  { 0, 0, 0, 0, 0 }, // 863 VGATHERQPDYrm
  { 1, 2, 3, 0, 0 }, // 864 CVTPD2PSrr_VCVTPD2PSrr
  { 1, 28, 29, 0, 0 }, // 865 CVTPD2PSrm_VCVTPD2PSXrm
  { 1, 2, 3, 0, 0 }, // 866 VCVTPD2PSYrr
  { 1, 28, 29, 0, 0 }, // 867 VCVTPD2PSYrm
  { 1, 6, 7, 0, 0 }, // 868 CVTSD2SSrr_Int_CVTSD2SSrr_Int_VCVTSD2SSrr_VCVTSD2SSrr
  { 1, 2, 3, 0, 0 }, // 869 CVTSD2SSrm
  { 1, 2, 3, 0, 0 }, // 870 Int_CVTSD2SSrm_Int_VCVTSD2SSrm_VCVTSD2SSrm
  { 1, 2, 3, 0, 0 }, // 871 CVTPS2PDrr_VCVTPS2PDrr
  { 1, 28, 29, 0, 0 }, // 872 CVTPS2PDrm_VCVTPS2PDYrm_VCVTPS2PDrm
  { 1, 2, 3, 0, 0 }, // 873 VCVTPS2PDYrr
  { 1, 6, 7, 0, 0 }, // 874 CVTSS2SDrr_Int_CVTSS2SDrr_Int_VCVTSS2SDrr_VCVTSS2SDrr
  { 1, 2, 3, 0, 0 }, // 875 CVTSS2SDrm
  { 1, 2, 3, 0, 0 }, // 876 Int_CVTSS2SDrm_Int_VCVTSS2SDrm_VCVTSS2SDrm
  { 1, 28, 29, 0, 0 }, // 877 CVTDQ2PDrr
  { 0, 0, 0, 0, 0 }, // 878 VCVTDQ2PDrr
  { 0, 0, 0, 0, 0 }, // 879 VCVTDQ2PDYrr
  { 1, 2, 3, 0, 0 }, // 880 CVTPD2DQrr_CVTTPD2DQrr_VCVTTPD2DQrr
  { 0, 0, 0, 0, 0 }, // 881 VCVTPD2DQrr
  { 1, 28, 29, 0, 0 }, // 882 CVTPD2DQrm_CVTTPD2DQrm
  { 0, 0, 0, 0, 0 }, // 883 VCVTPD2DQYrr
  { 1, 2, 3, 0, 0 }, // 884 VCVTTPD2DQYrr
  { 0, 0, 0, 0, 0 }, // 885 VCVTPD2DQYrm
  { 1, 28, 29, 0, 0 }, // 886 VCVTTPD2DQYrm
  { 1, 9, 10, 0, 0 }, // 887 MMX_CVTPS2PIirr_MMX_CVTTPS2PIirr
  { 1, 2, 3, 0, 0 }, // 888 MMX_CVTPI2PDirr
  { 1, 2, 3, 0, 0 }, // 889 MMX_CVTPD2PIirr_MMX_CVTTPD2PIirr
  { 1, 6, 7, 0, 0 }, // 890 CVTSI2SS64rr_CVTSI2SSrr_Int_CVTSI2SS64rr_Int_CVTSI2SSrr_Int_VCVTSI2SS64rr_Int_VCVTSI2SSrr
  { 0, 0, 0, 0, 0 }, // 891 VCVTSI2SS64rr_VCVTSI2SSrr
  { 1, 14, 15, 0, 0 }, // 892 CVTSS2SI64rr_CVTTSS2SI64rr_Int_CVTTSS2SI64rr_Int_VCVTTSS2SI64rr_VCVTSS2SI64rr_VCVTTSS2SI64rr
  { 1, 28, 29, 0, 0 }, // 893 CVTSS2SIrr_CVTTSS2SIrr_Int_CVTTSS2SIrr_Int_VCVTTSS2SIrr_VCVTSS2SIrr_VCVTTSS2SIrr
  { 1, 26, 27, 0, 0 }, // 894 CVTSS2SI64rm_CVTTSS2SI64rm_Int_CVTTSS2SI64rm_Int_VCVTTSS2SI64rm_VCVTSS2SI64rm_VCVTTSS2SI64rm
  { 1, 14, 15, 0, 0 }, // 895 CVTSS2SIrm_CVTTSS2SIrm_Int_CVTTSS2SIrm_Int_VCVTTSS2SIrm_VCVTSS2SIrm_VCVTTSS2SIrm
  { 1, 28, 29, 0, 0 }, // 896 CVTSD2SI64rr_CVTSD2SIrr_CVTTSD2SI64rr_CVTTSD2SIrr_Int_CVTTSD2SI64rr_Int_CVTTSD2SIrr_Int_VCVTTSD2SI64rr_Int_VCVTTSD2SIrr_VCVTSD2SI64rr_VCVTSD2SIrr_VCVTTSD2SI64rr_VCVTTSD2SIrr
  { 1, 14, 15, 0, 0 }, // 897 CVTSD2SI64rm_CVTSD2SIrm_CVTTSD2SI64rm_CVTTSD2SIrm_Int_CVTTSD2SI64rm_Int_CVTTSD2SIrm_Int_VCVTTSD2SI64rm_Int_VCVTTSD2SIrm_VCVTSD2SI64rm_VCVTSD2SIrm_VCVTTSD2SI64rm_VCVTTSD2SIrm
  { 0, 0, 0, 0, 0 }, // 898 VCVTPS2PHYrr_VCVTPS2PHrr
  { 0, 0, 0, 0, 0 }, // 899 VCVTPS2PHYmr_VCVTPS2PHmr
  { 0, 0, 0, 0, 0 }, // 900 VCVTPH2PSYrr_VCVTPH2PSrr
  { 1, 28, 29, 0, 0 }, // 901 HADDPDrr_HADDPSrr_HSUBPDrr_HSUBPSrr_VHADDPDYrr_VHADDPDrr_VHADDPSYrr_VHADDPSrr_VHSUBPDYrr_VHSUBPDrr_VHSUBPSYrr_VHSUBPSrr
  { 1, 14, 15, 0, 0 }, // 902 HADDPDrm_HADDPSrm_HSUBPDrm_HSUBPSrm_VHADDPDYrm_VHADDPDrm_VHADDPSYrm_VHADDPSrm_VHSUBPDYrm_VHSUBPDrm_VHSUBPSYrm_VHSUBPSrm
  { 1, 14, 15, 0, 0 }, // 903 MULPDrr_VMULPDrr
  { 1, 57, 58, 0, 0 }, // 904 MULPSrr_VMULPSrr
  { 1, 57, 58, 0, 0 }, // 905 MULSDrr_MULSDrr_Int_VMULSDrr_VMULSDrr_Int
  { 1, 75, 76, 0, 0 }, // 906 MULSSrr_MULSSrr_Int_VMULSSrr_VMULSSrr_Int
  { 1, 26, 27, 0, 0 }, // 907 MULPDrm_MULPSrm_VMULPDrm_VMULPSrm
  { 1, 57, 58, 0, 0 }, // 908 MULSDrm_MULSDrm_Int_MULSSrm_MULSSrm_Int_VMULSDrm_VMULSDrm_Int_VMULSSrm_VMULSSrm_Int
  { 1, 34, 35, 0, 0 }, // 909 VDIVPSYrr
  { 1, 33, 34, 0, 0 }, // 910 VDIVPSYrm
  { 1, 33, 34, 0, 0 }, // 911 VDIVPDYrr
  { 1, 33, 34, 0, 0 }, // 912 VDIVPDYrm
  { 1, 14, 15, 0, 0 }, // 913 VRCPPSYr
  { 1, 26, 27, 0, 0 }, // 914 VRCPPSYm
  { 0, 0, 0, 0, 0 }, // 915 ROUNDPDr_ROUNDPSr_VROUNDPDr_VROUNDPSr_VROUNDYPDr_VROUNDYPSr
  { 0, 0, 0, 0, 0 }, // 916 ROUNDSDr_ROUNDSDr_Int_ROUNDSSr_ROUNDSSr_Int_VROUNDSDr_VROUNDSDr_Int_VROUNDSSr_VROUNDSSr_Int
  { 0, 0, 0, 0, 0 }, // 917 ROUNDPDm_VROUNDPDm_VROUNDYPDm
  { 0, 0, 0, 0, 0 }, // 918 ROUNDPSm_VROUNDPSm_VROUNDYPSm
  { 0, 0, 0, 0, 0 }, // 919 ROUNDSDm_ROUNDSSm_VROUNDSDm_VROUNDSSm
  { 0, 0, 0, 0, 0 }, // 920 DPPSrri_VDPPSYrri_VDPPSrri
  { 0, 0, 0, 0, 0 }, // 921 DPPSrmi_VDPPSYrmi_VDPPSrmi
  { 0, 0, 0, 0, 0 }, // 922 DPPDrri
  { 0, 0, 0, 0, 0 }, // 923 VDPPDrri
  { 0, 0, 0, 0, 0 }, // 924 DPPDrmi_VDPPDrmi
  { 0, 0, 0, 0, 0 }, // 925 VFMADDPD4rr_VFMADDPD4rrY_VFMADDPD4rrY_REV_VFMADDPD4rr_REV_VFMADDPDr132r_VFMADDPDr132rY_VFMADDPDr213r_VFMADDPDr213rY_VFMADDPDr231r_VFMADDPDr231rY_VFMADDPS4rr_VFMADDPS4rrY_VFMADDPS4rrY_REV_VFMADDPS4rr_REV_VFMADDPSr132r_VFMADDPSr132rY_VFMADDPSr213r_VFMADDPSr213rY_VFMADDPSr231r_VFMADDPSr231rY_VFMADDSD4rr_VFMADDSD4rr_Int_VFMADDSD4rr_REV_VFMADDSDr132r_VFMADDSDr132r_Int_VFMADDSDr213r_VFMADDSDr213r_Int_VFMADDSDr231r_VFMADDSDr231r_Int_VFMADDSS4rr_VFMADDSS4rr_Int_VFMADDSS4rr_REV_VFMADDSSr132r_VFMADDSSr132r_Int_VFMADDSSr213r_VFMADDSSr213r_Int_VFMADDSSr231r_VFMADDSSr231r_Int_VFMADDSUBPDr132r_VFMADDSUBPDr132rY_VFMADDSUBPDr213r_VFMADDSUBPDr213rY_VFMADDSUBPDr231r_VFMADDSUBPDr231rY_VFMADDSUBPSr132r_VFMADDSUBPSr132rY_VFMADDSUBPSr213r_VFMADDSUBPSr213rY_VFMADDSUBPSr231r_VFMADDSUBPSr231rY_VFMSUBADDPDr132r_VFMSUBADDPDr132rY_VFMSUBADDPDr213r_VFMSUBADDPDr213rY_VFMSUBADDPDr231r_VFMSUBADDPDr231rY_VFMSUBADDPSr132r_VFMSUBADDPSr132rY_VFMSUBADDPSr213r_VFMSUBADDPSr213rY_VFMSUBADDPSr231r_VFMSUBADDPSr231rY_VFMSUBPD4rr_VFMSUBPD4rrY_VFMSUBPD4rrY_REV_VFMSUBPD4rr_REV_VFMSUBPDr132r_VFMSUBPDr132rY_VFMSUBPDr213r_VFMSUBPDr213rY_VFMSUBPDr231r_VFMSUBPDr231rY_VFMSUBPS4rr_VFMSUBPS4rrY_VFMSUBPS4rrY_REV_VFMSUBPS4rr_REV_VFMSUBPSr132r_VFMSUBPSr132rY_VFMSUBPSr213r_VFMSUBPSr213rY_VFMSUBPSr231r_VFMSUBPSr231rY_VFMSUBSD4rr_VFMSUBSD4rr_Int_VFMSUBSD4rr_REV_VFMSUBSDr132r_VFMSUBSDr132r_Int_VFMSUBSDr213r_VFMSUBSDr213r_Int_VFMSUBSDr231r_VFMSUBSDr231r_Int_VFMSUBSS4rr_VFMSUBSS4rr_Int_VFMSUBSS4rr_REV_VFMSUBSSr132r_VFMSUBSSr132r_Int_VFMSUBSSr213r_VFMSUBSSr213r_Int_VFMSUBSSr231r_VFMSUBSSr231r_Int_VFNMADDPD4rr_VFNMADDPD4rrY_VFNMADDPD4rrY_REV_VFNMADDPD4rr_REV_VFNMADDPDr132r_VFNMADDPDr132rY_VFNMADDPDr213r_VFNMADDPDr213rY_VFNMADDPDr231r_VFNMADDPDr231rY_VFNMADDPS4rr_VFNMADDPS4rrY_VFNMADDPS4rrY_REV_VFNMADDPS4rr_REV_VFNMADDPSr132r_VFNMADDPSr132rY_VFNMADDPSr213r_VFNMADDPSr213rY_VFNMADDPSr231r_VFNMADDPSr231rY_VFNMADDSD4rr_VFNMADDSD4rr_Int_VFNMADDSD4rr_REV_VFNMADDSDr132r_VFNMADDSDr132r_Int_VFNMADDSDr213r_VFNMADDSDr213r_Int_VFNMADDSDr231r_VFNMADDSDr231r_Int_VFNMADDSS4rr_VFNMADDSS4rr_Int_VFNMADDSS4rr_REV_VFNMADDSSr132r_VFNMADDSSr132r_Int_VFNMADDSSr213r_VFNMADDSSr213r_Int_VFNMADDSSr231r_VFNMADDSSr231r_Int_VFNMSUBPD4rr_VFNMSUBPD4rrY_VFNMSUBPD4rrY_REV_VFNMSUBPD4rr_REV_VFNMSUBPDr132r_VFNMSUBPDr132rY_VFNMSUBPDr213r_VFNMSUBPDr213rY_VFNMSUBPDr231r_VFNMSUBPDr231rY_VFNMSUBPS4rr_VFNMSUBPS4rrY_VFNMSUBPS4rrY_REV_VFNMSUBPS4rr_REV_VFNMSUBPSr132r_VFNMSUBPSr132rY_VFNMSUBPSr213r_VFNMSUBPSr213rY_VFNMSUBPSr231r_VFNMSUBPSr231rY_VFNMSUBSD4rr_VFNMSUBSD4rr_Int_VFNMSUBSD4rr_REV_VFNMSUBSDr132r_VFNMSUBSDr132r_Int_VFNMSUBSDr213r_VFNMSUBSDr213r_Int_VFNMSUBSDr231r_VFNMSUBSDr231r_Int_VFNMSUBSS4rr_VFNMSUBSS4rr_Int_VFNMSUBSS4rr_REV_VFNMSUBSSr132r_VFNMSUBSSr132r_Int_VFNMSUBSSr213r_VFNMSUBSSr213r_Int_VFNMSUBSSr231r_VFNMSUBSSr231r_Int
  { 0, 0, 0, 0, 0 }, // 926 VFMADDPD4mr_VFMADDPD4mrY_VFMADDPD4rm_VFMADDPD4rmY_VFMADDPDr132m_VFMADDPDr132mY_VFMADDPDr213m_VFMADDPDr213mY_VFMADDPDr231m_VFMADDPDr231mY_VFMADDPS4mr_VFMADDPS4mrY_VFMADDPS4rm_VFMADDPS4rmY_VFMADDPSr132m_VFMADDPSr132mY_VFMADDPSr213m_VFMADDPSr213mY_VFMADDPSr231m_VFMADDPSr231mY_VFMADDSD4mr_VFMADDSD4mr_Int_VFMADDSD4rm_VFMADDSD4rm_Int_VFMADDSDr132m_VFMADDSDr132m_Int_VFMADDSDr213m_VFMADDSDr213m_Int_VFMADDSDr231m_VFMADDSDr231m_Int_VFMADDSS4mr_VFMADDSS4mr_Int_VFMADDSS4rm_VFMADDSS4rm_Int_VFMADDSSr132m_VFMADDSSr132m_Int_VFMADDSSr213m_VFMADDSSr213m_Int_VFMADDSSr231m_VFMADDSSr231m_Int_VFMADDSUBPDr132m_VFMADDSUBPDr132mY_VFMADDSUBPDr213m_VFMADDSUBPDr213mY_VFMADDSUBPDr231m_VFMADDSUBPDr231mY_VFMADDSUBPSr132m_VFMADDSUBPSr132mY_VFMADDSUBPSr213m_VFMADDSUBPSr213mY_VFMADDSUBPSr231m_VFMADDSUBPSr231mY_VFMSUBADDPDr132m_VFMSUBADDPDr132mY_VFMSUBADDPDr213m_VFMSUBADDPDr213mY_VFMSUBADDPDr231m_VFMSUBADDPDr231mY_VFMSUBADDPSr132m_VFMSUBADDPSr132mY_VFMSUBADDPSr213m_VFMSUBADDPSr213mY_VFMSUBADDPSr231m_VFMSUBADDPSr231mY_VFMSUBPD4mr_VFMSUBPD4mrY_VFMSUBPD4rm_VFMSUBPD4rmY_VFMSUBPDr132m_VFMSUBPDr132mY_VFMSUBPDr213m_VFMSUBPDr213mY_VFMSUBPDr231m_VFMSUBPDr231mY_VFMSUBPS4mr_VFMSUBPS4mrY_VFMSUBPS4rm_VFMSUBPS4rmY_VFMSUBPSr132m_VFMSUBPSr132mY_VFMSUBPSr213m_VFMSUBPSr213mY_VFMSUBPSr231m_VFMSUBPSr231mY_VFMSUBSD4mr_VFMSUBSD4mr_Int_VFMSUBSD4rm_VFMSUBSD4rm_Int_VFMSUBSDr132m_VFMSUBSDr132m_Int_VFMSUBSDr213m_VFMSUBSDr213m_Int_VFMSUBSDr231m_VFMSUBSDr231m_Int_VFMSUBSS4mr_VFMSUBSS4mr_Int_VFMSUBSS4rm_VFMSUBSS4rm_Int_VFMSUBSSr132m_VFMSUBSSr132m_Int_VFMSUBSSr213m_VFMSUBSSr213m_Int_VFMSUBSSr231m_VFMSUBSSr231m_Int_VFNMADDPD4mr_VFNMADDPD4mrY_VFNMADDPD4rm_VFNMADDPD4rmY_VFNMADDPDr132m_VFNMADDPDr132mY_VFNMADDPDr213m_VFNMADDPDr213mY_VFNMADDPDr231m_VFNMADDPDr231mY_VFNMADDPS4mr_VFNMADDPS4mrY_VFNMADDPS4rm_VFNMADDPS4rmY_VFNMADDPSr132m_VFNMADDPSr132mY_VFNMADDPSr213m_VFNMADDPSr213mY_VFNMADDPSr231m_VFNMADDPSr231mY_VFNMADDSD4mr_VFNMADDSD4mr_Int_VFNMADDSD4rm_VFNMADDSD4rm_Int_VFNMADDSDr132m_VFNMADDSDr132m_Int_VFNMADDSDr213m_VFNMADDSDr213m_Int_VFNMADDSDr231m_VFNMADDSDr231m_Int_VFNMADDSS4mr_VFNMADDSS4mr_Int_VFNMADDSS4rm_VFNMADDSS4rm_Int_VFNMADDSSr132m_VFNMADDSSr132m_Int_VFNMADDSSr213m_VFNMADDSSr213m_Int_VFNMADDSSr231m_VFNMADDSSr231m_Int_VFNMSUBPD4mr_VFNMSUBPD4mrY_VFNMSUBPD4rm_VFNMSUBPD4rmY_VFNMSUBPDr132m_VFNMSUBPDr132mY_VFNMSUBPDr213m_VFNMSUBPDr213mY_VFNMSUBPDr231m_VFNMSUBPDr231mY_VFNMSUBPS4mr_VFNMSUBPS4mrY_VFNMSUBPS4rm_VFNMSUBPS4rmY_VFNMSUBPSr132m_VFNMSUBPSr132mY_VFNMSUBPSr213m_VFNMSUBPSr213mY_VFNMSUBPSr231m_VFNMSUBPSr231mY_VFNMSUBSD4mr_VFNMSUBSD4mr_Int_VFNMSUBSD4rm_VFNMSUBSD4rm_Int_VFNMSUBSDr132m_VFNMSUBSDr132m_Int_VFNMSUBSDr213m_VFNMSUBSDr213m_Int_VFNMSUBSDr231m_VFNMSUBSDr231m_Int_VFNMSUBSS4mr_VFNMSUBSS4mr_Int_VFNMSUBSS4rm_VFNMSUBSS4rm_Int_VFNMSUBSSr132m_VFNMSUBSSr132m_Int_VFNMSUBSSr213m_VFNMSUBSSr213m_Int_VFNMSUBSSr231m_VFNMSUBSSr231m_Int
  { 1, 34, 35, 0, 0 }, // 927 VSQRTPSYr
  { 1, 34, 35, 0, 0 }, // 928 VSQRTPSYm
  { 1, 33, 34, 0, 0 }, // 929 VSQRTPDYr
  { 1, 33, 34, 0, 0 }, // 930 VSQRTPDYm
  { 1, 14, 15, 0, 0 }, // 931 RSQRTPSr_VRSQRTPSr
  { 1, 75, 76, 0, 0 }, // 932 RSQRTSSr_VRSQRTSSr
  { 0, 0, 0, 0, 0 }, // 933 RSQRTSSr_Int
  { 0, 0, 0, 0, 0 }, // 934 VRSQRTSSr_Int
  { 1, 26, 27, 0, 0 }, // 935 RSQRTPSm_VRSQRTPSm
  { 1, 75, 76, 0, 0 }, // 936 RSQRTSSm_VRSQRTSSm
  { 0, 0, 0, 0, 0 }, // 937 RSQRTSSm_Int_VRSQRTSSm_Int
  { 1, 14, 15, 0, 0 }, // 938 VRSQRTPSYr
  { 1, 26, 27, 0, 0 }, // 939 VRSQRTPSYm
  { 0, 0, 0, 0, 0 }, // 940 ANDNPDrr_ANDNPSrr_ANDPDrr_ANDPSrr_ORPDrr_ORPSrr_VANDNPDYrr_VANDNPDrr_VANDNPSYrr_VANDNPSrr_VANDPDYrr_VANDPDrr_VANDPSYrr_VANDPSrr_VORPDYrr_VORPDrr_VORPSYrr_VORPSrr_VXORPDYrr_VXORPDrr_VXORPSYrr_VXORPSrr_XORPDrr_XORPSrr
  { 0, 0, 0, 0, 0 }, // 941 ANDNPDrm_ANDNPSrm_ANDPDrm_ANDPSrm_ORPDrm_ORPSrm_VANDNPDYrm_VANDNPDrm_VANDNPSYrm_VANDNPSrm_VANDPDYrm_VANDPDrm_VANDPSYrm_VANDPSrm_VORPDYrm_VORPDrm_VORPSYrm_VORPSrm_VXORPDYrm_VXORPDrm_VXORPSYrm_VXORPSrm_XORPDrm_XORPSrm
  { 0, 0, 0, 0, 0 }, // 942 VZEROUPPER
  { 0, 0, 0, 0, 0 }, // 943 VZEROALL
  { 1, 43, 44, 0, 0 }, // 944 LDMXCSR_VLDMXCSR
  { 1, 17, 18, 0, 0 }, // 945 STMXCSR_VSTMXCSR
  { 0, ~0U, ~0U, ~0U, ~0U } // end marker
};

// ===============================================================
// Data tables for the new per-operand machine model.

// {ProcResourceIdx, Cycles}
extern const llvm::MCWriteProcResEntry X86WriteProcResTable[] = {
  { 0,  0}, // Invalid
  {18,  1}, // #1
  {19,  1}, // #2
  { 4,  1}, // #3
  { 8,  1}, // #4
  {13,  1}, // #5
  {17,  1}, // #6
  {18,  1}, // #7
  {19,  3}, // #8
  {13,  1}, // #9
  {17,  1}, // #10
  {18,  1}, // #11
  {19,  2}, // #12
  { 3,  1}, // #13
  { 7,  1}, // #14
  {11,  1}, // #15
  {12,  1}, // #16
  {13,  1}, // #17
  {15,  1}, // #18
  {17,  1}, // #19
  {18,  1}, // #20
  {19,  2}, // #21
  { 3,  1}, // #22
  { 7,  1}, // #23
  {11,  1}, // #24
  {12,  1}, // #25
  {15,  1}, // #26
  {18,  1}, // #27
  {19,  1}, // #28
  { 5,  1}, // #29
  { 9,  1}, // #30
  {11,  1}, // #31
  {13,  1}, // #32
  {14,  1}, // #33
  {15,  1}, // #34
  {16,  1}, // #35
  {17,  1}, // #36
  {18,  1}, // #37
  {19,  2}, // #38
  { 5,  1}, // #39
  { 9,  1}, // #40
  {11,  1}, // #41
  {14,  1}, // #42
  {15,  1}, // #43
  {16,  1}, // #44
  {18,  1}, // #45
  {19,  1}, // #46
  { 5,  2}, // #47
  { 9,  2}, // #48
  {11,  2}, // #49
  {13,  1}, // #50
  {14,  2}, // #51
  {15,  2}, // #52
  {16,  2}, // #53
  {17,  1}, // #54
  {18,  2}, // #55
  {19,  3}, // #56
  { 5,  2}, // #57
  { 9,  2}, // #58
  {11,  2}, // #59
  {14,  2}, // #60
  {15,  2}, // #61
  {16,  2}, // #62
  {18,  2}, // #63
  {19,  2}, // #64
  { 2,  2}, // #65
  { 5,  7}, // #66
  { 7,  2}, // #67
  { 8,  2}, // #68
  { 9,  9}, // #69
  {10,  2}, // #70
  {11,  7}, // #71
  {13,  1}, // #72
  {14,  7}, // #73
  {15,  9}, // #74
  {16,  9}, // #75
  {17,  1}, // #76
  {18,  9}, // #77
  {19, 10}, // #78
  { 2,  2}, // #79
  { 5,  8}, // #80
  { 7,  2}, // #81
  { 8,  2}, // #82
  { 9, 10}, // #83
  {10,  2}, // #84
  {11,  8}, // #85
  {14,  8}, // #86
  {15, 10}, // #87
  {16, 10}, // #88
  {18, 10}, // #89
  {19, 10}, // #90
  {10,  1}, // #91
  {13,  1}, // #92
  {16,  1}, // #93
  {17,  1}, // #94
  {18,  1}, // #95
  {19,  2}, // #96
  {10,  1}, // #97
  {16,  1}, // #98
  {18,  1}, // #99
  {19,  1}, // #100
  {13,  1}, // #101
  {17,  1}, // #102
  {19,  1}, // #103
  { 1, 10}, // #104
  { 2,  1}, // #105
  { 7,  1}, // #106
  { 8,  1}, // #107
  { 9,  1}, // #108
  {10,  1}, // #109
  {13,  1}, // #110
  {15,  1}, // #111
  {16,  1}, // #112
  {17,  1}, // #113
  {18,  1}, // #114
  {19,  2}, // #115
  { 1, 10}, // #116
  { 2,  1}, // #117
  { 7,  1}, // #118
  { 8,  1}, // #119
  { 9,  1}, // #120
  {10,  1}, // #121
  {15,  1}, // #122
  {16,  1}, // #123
  {18,  1}, // #124
  {19,  1}, // #125
  { 4,  1}, // #126
  { 8,  1}, // #127
  {13,  1}, // #128
  {15,  1}, // #129
  {17,  1}, // #130
  {18,  1}, // #131
  {19,  3}, // #132
  { 4,  1}, // #133
  { 8,  1}, // #134
  {17,  1}, // #135
  {19,  2}, // #136
  {11,  1}, // #137
  {15,  1}, // #138
  {18,  1}, // #139
  {19,  1}, // #140
  { 4,  1}, // #141
  { 8,  1}, // #142
  {13,  1}, // #143
  {17,  1}, // #144
  {19,  2}, // #145
  {11,  1}, // #146
  {13,  1}, // #147
  {15,  1}, // #148
  {17,  1}, // #149
  {18,  1}, // #150
  {19,  2}, // #151
  { 2,  1}, // #152
  { 5,  2}, // #153
  { 7,  1}, // #154
  { 8,  1}, // #155
  { 9,  3}, // #156
  {10,  1}, // #157
  {11,  2}, // #158
  {13,  1}, // #159
  {14,  2}, // #160
  {15,  3}, // #161
  {16,  3}, // #162
  {17,  1}, // #163
  {18,  3}, // #164
  {19,  4}, // #165
  { 2,  1}, // #166
  { 5,  2}, // #167
  { 7,  1}, // #168
  { 8,  1}, // #169
  { 9,  3}, // #170
  {10,  1}, // #171
  {11,  2}, // #172
  {14,  2}, // #173
  {15,  3}, // #174
  {16,  3}, // #175
  {18,  3}, // #176
  {19,  3}, // #177
  { 2,  2}, // #178
  { 5,  1}, // #179
  { 7,  2}, // #180
  { 8,  2}, // #181
  { 9,  3}, // #182
  {10,  2}, // #183
  {11,  1}, // #184
  {13,  1}, // #185
  {14,  1}, // #186
  {15,  3}, // #187
  {16,  3}, // #188
  {17,  1}, // #189
  {18,  3}, // #190
  {19,  4}, // #191
  { 2,  2}, // #192
  { 5,  1}, // #193
  { 7,  2}, // #194
  { 8,  2}, // #195
  { 9,  3}, // #196
  {10,  2}, // #197
  {11,  1}, // #198
  {14,  1}, // #199
  {15,  3}, // #200
  {16,  3}, // #201
  {18,  3}, // #202
  {19,  3}, // #203
  { 2,  3}, // #204
  { 5,  2}, // #205
  { 7,  3}, // #206
  { 8,  3}, // #207
  { 9,  5}, // #208
  {10,  3}, // #209
  {11,  2}, // #210
  {12,  2}, // #211
  {13,  1}, // #212
  {14,  2}, // #213
  {15,  5}, // #214
  {16,  5}, // #215
  {17,  1}, // #216
  {18,  7}, // #217
  {19,  8}, // #218
  { 9,  6}, // #219
  {12,  2}, // #220
  {15,  6}, // #221
  {16,  6}, // #222
  {18,  8}, // #223
  {19,  8}, // #224
  { 9,  6}, // #225
  {12,  2}, // #226
  {13,  1}, // #227
  {15,  6}, // #228
  {16,  6}, // #229
  {17,  1}, // #230
  {18,  8}, // #231
  {19,  9}, // #232
  { 2,  3}, // #233
  { 5,  4}, // #234
  { 7,  3}, // #235
  { 8,  3}, // #236
  { 9,  7}, // #237
  {10,  3}, // #238
  {11,  4}, // #239
  {12,  2}, // #240
  {14,  4}, // #241
  {15,  7}, // #242
  {16,  7}, // #243
  {18,  9}, // #244
  {19,  9}, // #245
  { 2,  3}, // #246
  { 7,  3}, // #247
  { 8,  3}, // #248
  { 9,  3}, // #249
  {10,  3}, // #250
  {13,  1}, // #251
  {15,  3}, // #252
  {16,  3}, // #253
  {17,  1}, // #254
  {18,  3}, // #255
  {19,  4}, // #256
  { 2,  3}, // #257
  { 7,  3}, // #258
  { 8,  3}, // #259
  { 9,  3}, // #260
  {10,  3}, // #261
  {15,  3}, // #262
  {16,  3}, // #263
  {18,  3}, // #264
  {19,  3}, // #265
  { 4,  1}, // #266
  { 5,  1}, // #267
  { 8,  1}, // #268
  { 9,  1}, // #269
  {11,  1}, // #270
  {13,  1}, // #271
  {14,  1}, // #272
  {15,  1}, // #273
  {16,  1}, // #274
  {17,  1}, // #275
  {18,  1}, // #276
  {19,  3}, // #277
  { 4,  1}, // #278
  { 8,  1}, // #279
  {19,  1}, // #280
  { 4,  1}, // #281
  { 8,  1}, // #282
  {10,  1}, // #283
  {13,  1}, // #284
  {16,  1}, // #285
  {17,  1}, // #286
  {18,  1}, // #287
  {19,  3}, // #288
  { 4,  1}, // #289
  { 8,  1}, // #290
  {17,  1}, // #291
  {18,  1}, // #292
  {19,  3}, // #293
  { 3,  1}, // #294
  { 4,  1}, // #295
  { 7,  1}, // #296
  { 8,  1}, // #297
  {11,  1}, // #298
  {12,  1}, // #299
  {13,  1}, // #300
  {15,  1}, // #301
  {17,  1}, // #302
  {18,  1}, // #303
  {19,  3}, // #304
  {13,  1}, // #305
  {17,  1}, // #306
  {18,  2}, // #307
  {19,  3}, // #308
  { 4,  1}, // #309
  { 8,  1}, // #310
  {17,  2}, // #311
  {19,  3}, // #312
  { 3,  1}, // #313
  { 4,  1}, // #314
  { 7,  1}, // #315
  { 8,  1}, // #316
  {10,  1}, // #317
  {11,  1}, // #318
  {12,  1}, // #319
  {15,  1}, // #320
  {16,  1}, // #321
  {17,  1}, // #322
  {18,  2}, // #323
  {19,  4}, // #324
  {10,  1}, // #325
  {11,  1}, // #326
  {15,  1}, // #327
  {16,  1}, // #328
  {18,  2}, // #329
  {19,  2}, // #330
  { 4,  1}, // #331
  { 8,  1}, // #332
  {10,  1}, // #333
  {16,  1}, // #334
  {17,  1}, // #335
  {18,  1}, // #336
  {19,  3}, // #337
  { 4,  1}, // #338
  { 8,  1}, // #339
  {11,  1}, // #340
  {15,  1}, // #341
  {17,  1}, // #342
  {18,  1}, // #343
  {19,  3}, // #344
  { 4,  1}, // #345
  { 8,  1}, // #346
  {10,  1}, // #347
  {11,  1}, // #348
  {15,  1}, // #349
  {16,  1}, // #350
  {17,  1}, // #351
  {18,  2}, // #352
  {19,  4}, // #353
  { 4,  1}, // #354
  { 8,  1}, // #355
  {17,  2}, // #356
  {18,  2}, // #357
  {19,  5}, // #358
  { 4,  1}, // #359
  { 8,  1}, // #360
  {17,  2}, // #361
  {18,  3}, // #362
  {19,  6}, // #363
  { 4,  1}, // #364
  { 8,  1}, // #365
  {17,  2}, // #366
  {18,  1}, // #367
  {19,  4}, // #368
  { 3,  1}, // #369
  { 7,  1}, // #370
  {11,  1}, // #371
  {12,  1}, // #372
  {15,  1}, // #373
  {18,  2}, // #374
  {19,  2}, // #375
  { 3,  1}, // #376
  { 7,  1}, // #377
  {11,  1}, // #378
  {12,  1}, // #379
  {13,  1}, // #380
  {15,  1}, // #381
  {17,  1}, // #382
  {18,  2}, // #383
  {19,  3}, // #384
  { 3,  1}, // #385
  { 6,  1}, // #386
  { 7,  1}, // #387
  {10,  1}, // #388
  {11,  1}, // #389
  {12,  2}, // #390
  {14,  1}, // #391
  {15,  1}, // #392
  {16,  1}, // #393
  {18,  2}, // #394
  {19,  2}, // #395
  { 3,  1}, // #396
  { 6,  1}, // #397
  { 7,  1}, // #398
  {10,  1}, // #399
  {11,  1}, // #400
  {12,  2}, // #401
  {13,  1}, // #402
  {14,  1}, // #403
  {15,  1}, // #404
  {16,  1}, // #405
  {17,  1}, // #406
  {18,  2}, // #407
  {19,  3}, // #408
  { 3,  1}, // #409
  { 7,  1}, // #410
  {11,  1}, // #411
  {12,  1}, // #412
  {15,  1}, // #413
  {16,  2}, // #414
  {18,  3}, // #415
  {19,  3}, // #416
  { 3,  1}, // #417
  { 7,  1}, // #418
  {11,  1}, // #419
  {12,  1}, // #420
  {13,  1}, // #421
  {15,  1}, // #422
  {16,  2}, // #423
  {17,  1}, // #424
  {18,  3}, // #425
  {19,  4}, // #426
  { 2,  1}, // #427
  { 3,  1}, // #428
  { 5,  1}, // #429
  { 6,  1}, // #430
  { 7,  2}, // #431
  { 8,  1}, // #432
  { 9,  2}, // #433
  {10,  2}, // #434
  {11,  2}, // #435
  {12,  2}, // #436
  {14,  2}, // #437
  {15,  3}, // #438
  {16,  3}, // #439
  {18,  4}, // #440
  {19,  4}, // #441
  { 4,  1}, // #442
  { 8,  1}, // #443
  {10,  2}, // #444
  {16,  2}, // #445
  {17,  1}, // #446
  {18,  2}, // #447
  {19,  4}, // #448
  {10,  3}, // #449
  {16,  3}, // #450
  {18,  3}, // #451
  {19,  3}, // #452
  { 4,  1}, // #453
  { 8,  1}, // #454
  {10,  3}, // #455
  {13,  2}, // #456
  {16,  3}, // #457
  {17,  2}, // #458
  {18,  3}, // #459
  {19,  6}, // #460
  {10,  2}, // #461
  {16,  2}, // #462
  {18,  2}, // #463
  {19,  2}, // #464
  { 4,  1}, // #465
  { 8,  1}, // #466
  {10,  2}, // #467
  {16,  2}, // #468
  {17,  2}, // #469
  {18,  2}, // #470
  {19,  5}, // #471
  {10,  2}, // #472
  {16,  2}, // #473
  {18,  3}, // #474
  {19,  3}, // #475
  { 6,  1}, // #476
  {10,  1}, // #477
  {11,  1}, // #478
  {12,  1}, // #479
  {14,  1}, // #480
  {15,  1}, // #481
  {16,  1}, // #482
  {18,  2}, // #483
  {19,  2}, // #484
  { 6,  1}, // #485
  {10,  1}, // #486
  {12,  1}, // #487
  {14,  1}, // #488
  {16,  1}, // #489
  {18,  2}, // #490
  {19,  2}, // #491
  { 4,  1}, // #492
  { 6,  1}, // #493
  { 8,  1}, // #494
  {10,  1}, // #495
  {12,  1}, // #496
  {14,  1}, // #497
  {16,  1}, // #498
  {17,  1}, // #499
  {18,  1}, // #500
  {19,  3}, // #501
  { 4,  1}, // #502
  { 6,  1}, // #503
  { 8,  1}, // #504
  {10,  1}, // #505
  {12,  1}, // #506
  {14,  1}, // #507
  {16,  1}, // #508
  {17,  2}, // #509
  {18,  1}, // #510
  {19,  4}, // #511
  { 6,  1}, // #512
  {10,  1}, // #513
  {12,  1}, // #514
  {14,  1}, // #515
  {16,  1}, // #516
  {17,  1}, // #517
  {18,  1}, // #518
  {19,  2}, // #519
  { 6,  2}, // #520
  {10,  2}, // #521
  {12,  2}, // #522
  {13,  1}, // #523
  {14,  2}, // #524
  {15,  1}, // #525
  {16,  2}, // #526
  {17,  1}, // #527
  {18,  3}, // #528
  {19,  4}, // #529
  { 4,  1}, // #530
  { 8,  1}, // #531
  {13,  2}, // #532
  {17,  2}, // #533
  {18,  2}, // #534
  {19,  5}, // #535
  {13,  2}, // #536
  {17,  2}, // #537
  {18,  3}, // #538
  {19,  5}, // #539
  { 6,  3}, // #540
  { 9,  1}, // #541
  {10,  3}, // #542
  {12,  3}, // #543
  {14,  3}, // #544
  {15,  1}, // #545
  {16,  4}, // #546
  {18,  4}, // #547
  {19,  4}, // #548
  {13,  1}, // #549
  {15, 16}, // #550
  {17,  1}, // #551
  {18, 16}, // #552
  {19, 17}, // #553
  { 7,  1}, // #554
  {15,  1}, // #555
  {18,  1}, // #556
  {19,  1}, // #557
  { 7,  2}, // #558
  {13,  2}, // #559
  {15,  2}, // #560
  {17,  2}, // #561
  {18,  2}, // #562
  {19,  4}, // #563
  { 4,  2}, // #564
  { 8,  2}, // #565
  {13,  2}, // #566
  {17,  2}, // #567
  {18,  3}, // #568
  {19,  7}, // #569
  { 7,  1}, // #570
  {13,  1}, // #571
  {15,  1}, // #572
  {17,  1}, // #573
  {18,  1}, // #574
  {19,  2}, // #575
  { 2,  1}, // #576
  { 7,  1}, // #577
  { 8,  1}, // #578
  { 9,  1}, // #579
  {10,  1}, // #580
  {15,  1}, // #581
  {16,  1}, // #582
  {18,  2}, // #583
  {19,  2}, // #584
  { 2,  1}, // #585
  { 4,  1}, // #586
  { 7,  1}, // #587
  { 8,  2}, // #588
  { 9,  1}, // #589
  {10,  1}, // #590
  {15,  1}, // #591
  {16,  1}, // #592
  {17,  1}, // #593
  {18,  1}, // #594
  {19,  3}, // #595
  { 6,  1}, // #596
  { 7,  1}, // #597
  {10,  1}, // #598
  {12,  1}, // #599
  {13,  1}, // #600
  {14,  1}, // #601
  {15,  1}, // #602
  {16,  1}, // #603
  {17,  1}, // #604
  {18,  2}, // #605
  {19,  3}, // #606
  { 3,  2}, // #607
  { 7,  2}, // #608
  {11,  2}, // #609
  {12,  2}, // #610
  {13,  1}, // #611
  {15,  2}, // #612
  {17,  1}, // #613
  {18,  2}, // #614
  {19,  3}, // #615
  { 3,  2}, // #616
  { 7,  2}, // #617
  {11,  2}, // #618
  {12,  2}, // #619
  {15,  2}, // #620
  {18,  2}, // #621
  {19,  2}, // #622
  {18,  5}, // #623
  {19,  5}, // #624
  { 5,  1}, // #625
  { 7,  1}, // #626
  { 9,  1}, // #627
  {11,  1}, // #628
  {14,  1}, // #629
  {15,  2}, // #630
  {16,  1}, // #631
  {18,  2}, // #632
  {19,  2}, // #633
  { 5,  3}, // #634
  { 9,  3}, // #635
  {11,  3}, // #636
  {14,  3}, // #637
  {15,  3}, // #638
  {16,  3}, // #639
  {18,  3}, // #640
  {19,  3}, // #641
  { 5,  3}, // #642
  { 9,  3}, // #643
  {11,  3}, // #644
  {13,  1}, // #645
  {14,  3}, // #646
  {15,  3}, // #647
  {16,  3}, // #648
  {17,  1}, // #649
  {18,  3}, // #650
  {19,  4}, // #651
  { 2,  1}, // #652
  { 4,  1}, // #653
  { 7,  1}, // #654
  { 8,  2}, // #655
  { 9,  1}, // #656
  {10,  1}, // #657
  {13,  2}, // #658
  {15,  1}, // #659
  {16,  1}, // #660
  {17,  2}, // #661
  {18,  1}, // #662
  {19,  4}, // #663
  { 8,  4}, // #664
  {13,  4}, // #665
  {14,  2}, // #666
  {16,  2}, // #667
  {17,  4}, // #668
  {18,  2}, // #669
  {19, 10}, // #670
  { 2,  1}, // #671
  { 3,  1}, // #672
  { 4,  1}, // #673
  { 7,  2}, // #674
  { 8,  2}, // #675
  { 9,  1}, // #676
  {10,  1}, // #677
  {11,  1}, // #678
  {12,  1}, // #679
  {13,  1}, // #680
  {15,  2}, // #681
  {16,  1}, // #682
  {17,  1}, // #683
  {18,  2}, // #684
  {19,  4}, // #685
  { 2,  1}, // #686
  { 5,  1}, // #687
  { 7,  1}, // #688
  { 8,  1}, // #689
  { 9,  2}, // #690
  {10,  1}, // #691
  {11,  1}, // #692
  {14,  1}, // #693
  {15,  2}, // #694
  {16,  2}, // #695
  {18,  2}, // #696
  {19,  2}, // #697
  { 5,  1}, // #698
  { 7,  1}, // #699
  { 9,  1}, // #700
  {11,  1}, // #701
  {13,  1}, // #702
  {14,  1}, // #703
  {15,  2}, // #704
  {16,  1}, // #705
  {17,  1}, // #706
  {18,  2}, // #707
  {19,  3}, // #708
  { 3,  1}, // #709
  { 5,  2}, // #710
  { 7,  1}, // #711
  { 9,  2}, // #712
  {11,  3}, // #713
  {12,  1}, // #714
  {14,  2}, // #715
  {15,  3}, // #716
  {16,  2}, // #717
  {18,  3}, // #718
  {19,  3}, // #719
  { 3,  1}, // #720
  { 5,  2}, // #721
  { 7,  1}, // #722
  { 9,  2}, // #723
  {11,  3}, // #724
  {12,  1}, // #725
  {13,  1}, // #726
  {14,  2}, // #727
  {15,  3}, // #728
  {16,  2}, // #729
  {17,  1}, // #730
  {18,  3}, // #731
  {19,  4}, // #732
  { 2,  2}, // #733
  { 7,  2}, // #734
  { 8,  2}, // #735
  { 9,  2}, // #736
  {10,  2}, // #737
  {15,  2}, // #738
  {16,  2}, // #739
  {18,  2}, // #740
  {19,  2}, // #741
  { 2,  2}, // #742
  { 7,  2}, // #743
  { 8,  2}, // #744
  { 9,  2}, // #745
  {10,  2}, // #746
  {13,  1}, // #747
  {15,  2}, // #748
  {16,  2}, // #749
  {17,  1}, // #750
  {18,  2}, // #751
  {19,  3}, // #752
  { 2,  1}, // #753
  { 5,  1}, // #754
  { 7,  1}, // #755
  { 8,  1}, // #756
  { 9,  2}, // #757
  {10,  1}, // #758
  {11,  1}, // #759
  {13,  1}, // #760
  {14,  1}, // #761
  {15,  2}, // #762
  {16,  2}, // #763
  {17,  1}, // #764
  {18,  2}, // #765
  {19,  3}, // #766
  { 3,  1}, // #767
  { 5,  1}, // #768
  { 7,  1}, // #769
  { 9,  1}, // #770
  {11,  2}, // #771
  {12,  1}, // #772
  {14,  1}, // #773
  {15,  2}, // #774
  {16,  1}, // #775
  {18,  2}, // #776
  {19,  2}, // #777
  { 3,  1}, // #778
  { 5,  1}, // #779
  { 7,  1}, // #780
  { 9,  1}, // #781
  {11,  2}, // #782
  {12,  1}, // #783
  {13,  1}, // #784
  {14,  1}, // #785
  {15,  2}, // #786
  {16,  1}, // #787
  {17,  1}, // #788
  {18,  2}, // #789
  {19,  3}, // #790
  { 2,  1}, // #791
  { 3,  1}, // #792
  { 7,  2}, // #793
  { 8,  1}, // #794
  { 9,  1}, // #795
  {10,  1}, // #796
  {11,  1}, // #797
  {12,  1}, // #798
  {15,  2}, // #799
  {16,  1}, // #800
  {18,  2}, // #801
  {19,  2}, // #802
  { 2,  1}, // #803
  { 3,  1}, // #804
  { 7,  2}, // #805
  { 8,  1}, // #806
  { 9,  1}, // #807
  {10,  1}, // #808
  {11,  1}, // #809
  {12,  1}, // #810
  {13,  1}, // #811
  {15,  2}, // #812
  {16,  1}, // #813
  {17,  1}, // #814
  {18,  2}, // #815
  {19,  3}, // #816
  { 3,  1}, // #817
  { 4,  1}, // #818
  { 5,  1}, // #819
  { 7,  1}, // #820
  { 8,  1}, // #821
  { 9,  1}, // #822
  {11,  2}, // #823
  {12,  1}, // #824
  {13,  1}, // #825
  {14,  1}, // #826
  {15,  2}, // #827
  {16,  1}, // #828
  {17,  1}, // #829
  {18,  2}, // #830
  {19,  4}, // #831
  { 2,  2}, // #832
  { 7,  2}, // #833
  { 8,  2}, // #834
  { 9,  2}, // #835
  {10,  2}, // #836
  {11,  1}, // #837
  {15,  3}, // #838
  {16,  2}, // #839
  {18,  3}, // #840
  {19,  3}, // #841
  { 2,  2}, // #842
  { 7,  2}, // #843
  { 8,  2}, // #844
  { 9,  2}, // #845
  {10,  2}, // #846
  {11,  1}, // #847
  {13,  1}, // #848
  {15,  3}, // #849
  {16,  2}, // #850
  {17,  1}, // #851
  {18,  3}, // #852
  {19,  4}, // #853
  { 2,  2}, // #854
  { 3,  1}, // #855
  { 5,  1}, // #856
  { 7,  3}, // #857
  { 8,  2}, // #858
  { 9,  3}, // #859
  {10,  2}, // #860
  {11,  2}, // #861
  {12,  1}, // #862
  {14,  1}, // #863
  {15,  4}, // #864
  {16,  3}, // #865
  {18,  4}, // #866
  {19,  4}, // #867
  { 2,  2}, // #868
  { 3,  1}, // #869
  { 5,  1}, // #870
  { 6,  1}, // #871
  { 7,  3}, // #872
  { 8,  2}, // #873
  { 9,  3}, // #874
  {10,  3}, // #875
  {11,  2}, // #876
  {12,  2}, // #877
  {13,  1}, // #878
  {14,  2}, // #879
  {15,  4}, // #880
  {16,  4}, // #881
  {17,  1}, // #882
  {18,  5}, // #883
  {19,  6}, // #884
  { 2,  1}, // #885
  { 3,  1}, // #886
  { 5,  1}, // #887
  { 7,  2}, // #888
  { 8,  1}, // #889
  { 9,  2}, // #890
  {10,  1}, // #891
  {11,  2}, // #892
  {12,  1}, // #893
  {14,  1}, // #894
  {15,  3}, // #895
  {16,  2}, // #896
  {18,  3}, // #897
  {19,  3}, // #898
  { 2,  1}, // #899
  { 3,  1}, // #900
  { 5,  1}, // #901
  { 7,  2}, // #902
  { 8,  1}, // #903
  { 9,  2}, // #904
  {10,  1}, // #905
  {11,  2}, // #906
  {12,  1}, // #907
  {13,  1}, // #908
  {14,  1}, // #909
  {15,  3}, // #910
  {16,  2}, // #911
  {17,  1}, // #912
  {18,  3}, // #913
  {19,  4}, // #914
  { 2,  1}, // #915
  { 6,  1}, // #916
  { 7,  1}, // #917
  { 8,  1}, // #918
  { 9,  1}, // #919
  {10,  2}, // #920
  {12,  1}, // #921
  {13,  1}, // #922
  {14,  1}, // #923
  {15,  1}, // #924
  {16,  2}, // #925
  {17,  1}, // #926
  {18,  2}, // #927
  {19,  3}, // #928
  { 2,  1}, // #929
  { 4,  1}, // #930
  { 6,  1}, // #931
  { 7,  1}, // #932
  { 8,  2}, // #933
  { 9,  1}, // #934
  {10,  2}, // #935
  {12,  1}, // #936
  {14,  1}, // #937
  {15,  1}, // #938
  {16,  2}, // #939
  {17,  1}, // #940
  {18,  2}, // #941
  {19,  4}, // #942
  { 2,  1}, // #943
  { 3,  3}, // #944
  { 9,  1}, // #945
  {10,  2}, // #946
  {11,  1}, // #947
  { 2,  1}, // #948
  { 3,  2}, // #949
  { 9,  1}, // #950
  {10,  1}, // #951
  { 3,  2}, // #952
  { 6,  1}, // #953
  { 8,  1}, // #954
  { 9,  1}, // #955
  {10,  1}, // #956
  { 3,  1}, // #957
  { 6,  1}, // #958
  { 8,  1}, // #959
  { 3,  2}, // #960
  { 8,  1}, // #961
  { 9,  1}, // #962
  {10,  1}, // #963
  {12,  1}, // #964
  { 3,  1}, // #965
  { 8,  1}, // #966
  {12,  1}, // #967
  { 3,  1}, // #968
  { 9,  1}, // #969
  {10,  1}, // #970
  {12,  1}, // #971
  { 3,  2}, // #972
  { 7,  1}, // #973
  { 8,  1}, // #974
  { 9,  1}, // #975
  {10,  1}, // #976
  { 3,  1}, // #977
  { 7,  1}, // #978
  { 8,  1}, // #979
  { 1,  1}, // #980
  { 2,  1}, // #981
  { 3,  2}, // #982
  { 4, 25}, // #983
  { 9,  1}, // #984
  {10,  1}, // #985
  { 1,  1}, // #986
  { 2,  1}, // #987
  { 3,  1}, // #988
  { 4, 25}, // #989
  { 3,  2}, // #990
  { 5, 19}, // #991
  { 7,  1}, // #992
  { 8,  1}, // #993
  { 9,  1}, // #994
  {10,  1}, // #995
  { 3,  3}, // #996
  { 8,  1}, // #997
  { 9,  1}, // #998
  {10,  2}, // #999
  {11,  1}, // #1000
  { 1,  1}, // #1001
  { 2,  1}, // #1002
  { 3,  2}, // #1003
  { 9,  1}, // #1004
  {10,  1}, // #1005
  { 3,  1}, // #1006
  {10,  1}, // #1007
  {11,  1}, // #1008
  { 3,  3}, // #1009
  { 6,  2}, // #1010
  { 8,  2}, // #1011
  { 9,  1}, // #1012
  {10,  1}, // #1013
  { 3,  2}, // #1014
  { 6,  2}, // #1015
  { 8,  2}, // #1016
  { 1,  5}, // #1017
  { 2,  5}, // #1018
  { 3,  6}, // #1019
  { 9,  1}, // #1020
  {10,  1}, // #1021
  { 1,  5}, // #1022
  { 2,  5}, // #1023
  { 3,  5}, // #1024
  { 3,  3}, // #1025
  { 8,  2}, // #1026
  { 9,  1}, // #1027
  {10,  1}, // #1028
  { 3,  2}, // #1029
  { 8,  2}, // #1030
  { 3,  6}, // #1031
  { 8,  5}, // #1032
  { 9,  1}, // #1033
  {10,  1}, // #1034
  { 3,  5}, // #1035
  { 8,  5}, // #1036
  { 2,  1}, // #1037
  { 3,  2}, // #1038
  {10,  1}, // #1039
  {11,  1}, // #1040
  { 3,  2}, // #1041
  { 5, 21}, // #1042
  { 7,  1}, // #1043
  { 8,  1}, // #1044
  { 9,  1}, // #1045
  {10,  1}, // #1046
  { 3,  3}, // #1047
  { 7,  1}, // #1048
  { 8,  1}, // #1049
  { 9,  1}, // #1050
  {10,  2}, // #1051
  {11,  1}, // #1052
  { 4,  1}, // #1053
  { 8,  1}, // #1054
  { 9,  1}, // #1055
  {10,  3}, // #1056
  { 3,  1}, // #1057
  { 7,  1}, // #1058
  { 8,  1}, // #1059
  { 9,  1}, // #1060
  {10,  2}, // #1061
  { 3,  1}, // #1062
  { 7,  1}, // #1063
  { 9,  1}, // #1064
  {10,  1}, // #1065
  { 8,  1}, // #1066
  { 9,  2}, // #1067
  {10,  3}, // #1068
  { 8,  1}, // #1069
  { 9, 10}, // #1070
  {10, 11}, // #1071
  { 9, 11}, // #1072
  {10, 11}, // #1073
  { 6,  1}, // #1074
  { 8,  1}, // #1075
  { 9,  1}, // #1076
  {10,  2}, // #1077
  { 6,  1}, // #1078
  { 9,  1}, // #1079
  {10,  1}, // #1080
  { 5,  1}, // #1081
  { 6,  1}, // #1082
  { 7,  1}, // #1083
  { 8,  1}, // #1084
  { 9,  1}, // #1085
  {10,  2}, // #1086
  { 5,  1}, // #1087
  { 6,  1}, // #1088
  { 7,  1}, // #1089
  { 9,  1}, // #1090
  {10,  1}, // #1091
  { 1, 10}, // #1092
  { 2,  1}, // #1093
  { 6,  1}, // #1094
  { 8,  1}, // #1095
  { 9,  1}, // #1096
  {10,  2}, // #1097
  { 1, 10}, // #1098
  { 2,  1}, // #1099
  { 6,  1}, // #1100
  { 9,  1}, // #1101
  {10,  1}, // #1102
  { 4,  1}, // #1103
  { 6,  1}, // #1104
  { 8,  1}, // #1105
  { 9,  1}, // #1106
  {10,  3}, // #1107
  { 2,  1}, // #1108
  { 3,  1}, // #1109
  { 5,  1}, // #1110
  { 6,  2}, // #1111
  { 7,  2}, // #1112
  { 8,  1}, // #1113
  { 9,  3}, // #1114
  {10,  4}, // #1115
  { 2,  1}, // #1116
  { 3,  1}, // #1117
  { 5,  1}, // #1118
  { 6,  2}, // #1119
  { 7,  2}, // #1120
  { 9,  3}, // #1121
  {10,  3}, // #1122
  { 3,  1}, // #1123
  { 5,  1}, // #1124
  { 6,  1}, // #1125
  { 7,  2}, // #1126
  { 8,  1}, // #1127
  { 9,  2}, // #1128
  {10,  3}, // #1129
  { 3,  1}, // #1130
  { 5,  1}, // #1131
  { 6,  1}, // #1132
  { 7,  2}, // #1133
  { 9,  2}, // #1134
  {10,  2}, // #1135
  { 8,  1}, // #1136
  { 9, 17}, // #1137
  {10, 18}, // #1138
  { 9, 18}, // #1139
  {10, 18}, // #1140
  { 8,  1}, // #1141
  { 9,  7}, // #1142
  {10,  8}, // #1143
  { 9,  8}, // #1144
  {10,  8}, // #1145
  { 4,  1}, // #1146
  { 7,  1}, // #1147
  { 8,  1}, // #1148
  { 9,  1}, // #1149
  {10,  3}, // #1150
  { 4,  1}, // #1151
  {10,  1}, // #1152
  { 2,  1}, // #1153
  { 5,  1}, // #1154
  { 6,  2}, // #1155
  { 7,  1}, // #1156
  { 8,  1}, // #1157
  { 9,  2}, // #1158
  {10,  3}, // #1159
  { 2,  1}, // #1160
  { 5,  1}, // #1161
  { 6,  2}, // #1162
  { 7,  1}, // #1163
  { 9,  2}, // #1164
  {10,  2}, // #1165
  { 3,  1}, // #1166
  { 4,  1}, // #1167
  { 7,  1}, // #1168
  { 8,  1}, // #1169
  { 9,  1}, // #1170
  {10,  3}, // #1171
  { 1,  1}, // #1172
  { 3,  1}, // #1173
  { 2,  1}, // #1174
  { 3,  1}, // #1175
  { 7,  1}, // #1176
  { 1,  5}, // #1177
  { 3,  5}, // #1178
  { 7,  1}, // #1179
  { 1,  1}, // #1180
  { 3,  1}, // #1181
  { 7,  1}, // #1182
  { 6,  1}, // #1183
  { 7,  1}, // #1184
  { 8, 25}, // #1185
  { 6,  1}, // #1186
  { 8, 25}, // #1187
  { 1,  1}, // #1188
  { 3,  1}, // #1189
  { 7,  1}, // #1190
  { 9, 34}, // #1191
  { 1,  1}, // #1192
  { 3,  1}, // #1193
  { 9, 34}, // #1194
  { 1,  1}, // #1195
  { 3,  1}, // #1196
  { 7,  2}, // #1197
  { 1,  1}, // #1198
  { 3,  1}, // #1199
  { 7,  1}, // #1200
  {10,  2}, // #1201
  { 1,  1}, // #1202
  { 3,  1}, // #1203
  {10,  2}, // #1204
  { 1,  1}, // #1205
  { 3,  1}, // #1206
  { 5,  1}, // #1207
  { 6,  1}, // #1208
  { 7,  1}, // #1209
  { 1, 10}, // #1210
  { 3, 10}, // #1211
  { 7,  1}, // #1212
  { 1, 21}, // #1213
  { 3, 21}, // #1214
  { 7,  1}, // #1215
  { 1, 17}, // #1216
  { 3, 17}, // #1217
  { 7,  1}, // #1218
  { 1, 13}, // #1219
  { 3, 13}, // #1220
  { 7,  1}, // #1221
  { 4,  1}, // #1222
  { 6,  1}, // #1223
  { 7,  2} // #1224
}; // X86WriteProcResTable

// {Cycles, WriteResourceID}
extern const llvm::MCWriteLatencyEntry X86WriteLatencyTable[] = {
  { 0,  0}, // Invalid
  {100,  0}, // #1 WriteMicrocoded_WriteSystem
  { 1,  0}, // #2 WriteALU_WriteVecLogic_WriteZero_WriteFBlend_WriteShift_WriteJump_WriteFShuffle_WriteStore_WriteMove_WriteLEA_WriteFence_WriteShuffle_WriteVecALU_WriteVecShift_WriteNop_WriteBlend_WriteRMW_Write2P237_P4_WritePushF_WritePushA_WritePopF_WritePopA_WriteP06_WriteBSwap32_WriteBSwap64_WriteMoveBE32rm_WriteMoveBE16mr_WriteMoveBE32mr_WriteMoveBE64mr_Write2P0156_2P237_P4_Write3P0156_2P237_P4_WriteP0156_2P237_P4_WriteShiftRMW_WriteShiftClLdRMW_Write2P06_WriteRotateRMW_WriteRotateRMWCL_WriteRCm1_WriteRCmi_WriteShDmr_WriteShDmrCL_WriteBTmr_WriteBTRSCmr_WriteSetCCm_WriteCldStd_WriteP15_WriteJCXZ_WriteLOOP_WriteCALLr_WriteCALLm_WriteRET_WriteRETI_WriteBOUND_WriteINTO_Write2P0156_P23_WriteP0156_P23_WriteSTOS_WriteXADD_WriteCMPXCHG_WriteCMPXCHG8B_WriteCMPXCHG16B_WritePAUSE_WriteXGETBV_WriteRDTSC_WriteRDPMC_WriteRDRAND_WriteST_FP80m_WriteFBSTP_WriteFNSTSW_WriteFNSTCW_WriteFNSAVE_WriteFRSTOR_WriteP1_P23_Write2P1_P23_Write5P0156_WriteFNINIT_WriteP5_WriteP01_P5_WritePBLENDWr_WriteVPBLENDDr_WritePEXTRm_WriteVPGATHERDD128_WriteVPGATHERDD256_WriteVPGATHERQD128_WriteVPGATHERQD256_WriteVPGATHERDQ128_WriteVPGATHERDQ256_WriteVPGATHERQQ128_WriteVPGATHERQQ256_WriteEXTRACTPSr_WriteVGATHERDPS128_WriteVGATHERDPS256_WriteVGATHERQPS128_WriteVGATHERQPS256_WriteVGATHERDPD128_WriteVGATHERDPD256_WriteVGATHERQPD128_WriteVGATHERQPD256_WriteVZEROUPPER_WriteVZEROALL_WriteFShuffle256_WriteShuffle256_WriteVarVecShift_WriteVarBlend_WriteFVarBlend
  { 5,  0}, // #3 WriteALULd_WriteVecLogicLd_WriteFBlendLd_WriteShiftLd_WriteJumpLd_WriteFShuffleLd_WriteVecALULd_WriteShuffleLd_WriteVecIMul_WriteVecShiftLd_WriteFMul_WriteBlendLd_WriteFRcp_WriteFRsqrt_WriteVPBROADCAST128Ld_WritePCMPGTQr_WritePCMPGTQm_WriteCVTPD2PSYrr_WriteCVTPS2PDrm_WriteVCVTPS2PDYrr_WriteCVTSS2SDrm_WriteHADDSUBPr_WriteMULr_WriteFMADDr_WriteRSQRTr_WriteP5Ld_WriteLoad_WriteShuffle256Ld_WriteFShuffle256Ld_WriteVarVecShiftLd
  { 1,  0}, // #4 WriteRMW
  { 7,  0}, // #5 WriteFAddLd_WriteAESDecEncLd_WriteCvtF2ILd_WriteCvtF2FLd_WriteIMulLd_WriteCLMulLd_WriteShuffle256Ld_WriteFShuffle256Ld_WriteXLAT_WriteMul64Ld_WriteP1_Lat3Ld_WriteFIST_WriteFLDCW_WriteVPBROADCAST256Ld_WriteVRCPPSr_WriteRSQRTPSYr_WriteSTMXCSR_WriteAESIMCLd_WriteAESKeyGenLd_WriteVecIMulLd_WriteFMulLd_WriteVarBlendLd_WritePCmpIStrM_WriteFRcpLd_WriteFRsqrtLd_WriteFVarBlendLd_WriteCvtI2FLd_WriteMPSAD
  { 3,  0}, // #6 WriteFAdd_WriteCvtF2I_WriteCvtF2F_WriteIMul_WriteIMulH_WriteFShuffle256_WriteShuffle256_WriteMul64_WriteShDrr_WriteShlDCL_WriteP1_Lat3_WriteVPMOVSX_WritePMOVMSKB_WritePHADDSUBr_WriteMOVMSKPr_WriteAESDecEnc_WriteCvtI2F_WriteMPSAD_WritePCmpIStrILd_WriteLoad
  {14,  0}, // #7 WriteAESIMCLd_WriteMASKMOVDQU_WriteVMASKMOVPYmr_WriteDPPSr_WriteCLMulLd
  {10,  0}, // #8 WriteAESKeyGenLd_WritePCmpEStrMLd_WritePCmpIStrMLd_WritePMULLDr_WritePMULLDm_WriteP1_P5_Lat6Ld_WriteROUNDm_WriteMPSADLd_WriteCLMulLd
  { 4,  0}, // #9 WriteLoad_WriteCvtI2F_WriteMul16_WriteMul32_WriteMulX32_WriteMulX64_WriteShrDCL_WriteP15Ld_WriteMOVS_WriteCMPS_WriteLD_F80m_WriteMMXPACKSSrm_WritePBLENDWm_WriteVPBLENDDm_WriteVPMASKMOVr_WriteEXTRACTPSm_WriteVEXTRACTF128m_WriteVMASKMOVPrm_WriteP1_P5_Lat4_WriteP0_P1_Lat4_WriteALULd_WriteShiftLd_WriteJumpLd_WritePCmpEStrILd_WriteVecLogicLd_WriteFBlendLd_WriteCvtF2I_WriteCvtF2F_WriteFShuffleLd_WriteVecALULd_WriteShuffleLd_WriteVecIMul_WriteVecShiftLd_WriteVarBlendLd_WriteBlendLd_WriteFVarBlendLd_WriteShuffle256Ld_WriteFShuffle256Ld_WriteVarVecShiftLd
  { 8,  0}, // #10 WriteCvtI2FLd_WriteMul16Ld_WriteMul32Ld_WriteMulX32Ld_WriteMulX64Ld_WriteP1_P5_Lat4Ld_WriteP0_P1_Lat4Ld_WriteFAddLd_WriteAESDecEncLd_WriteCvtF2ILd_WriteCvtF2FLd_WriteMPSADLd_WriteAESIMCLd_WriteAESKeyGenLd_WriteFMulLd_WriteFRcpLd_WriteFRsqrtLd
  {29,  0}, // #11 WriteIDivLd
  {25,  0}, // #12 WriteIDiv
  {16,  0}, // #13 WriteFDivLd
  {12,  0}, // #14 WriteFDiv_WritePCmpIStrMLd
  { 9,  0}, // #15 WriteVecIMulLd_WriteFMulLd_WriteFRcpLd_WriteFRsqrtLd_WriteCVTPD2PSYrm_WriteHADDSUBPm_WriteMULm_WriteDPPDr_WriteFMADDm_WriteRSQRTm
  { 6,  0}, // #16 WriteMPSADLd_WriteVarBlendLd_WriteFVarBlendLd_WriteVarVecShiftLd_Write2P0156_Lat2Ld_WriteRCri_WriteFILD_WriteFNSTSWm_WritePHADDSUBm_WriteP1_P5_Lat6_WriteROUNDr_WriteLDMXCSR_WriteVecLogicLd_WriteFBlendLd_WriteIMulLd_WriteFShuffleLd_WriteVecALULd_WriteShuffleLd_WriteVecShiftLd_WriteBlendLd_WritePCmpIStrI_WriteShuffle256Ld_WriteFShuffle256Ld_WriteFAddLd
  { 3,  0}, // #17 WriteIMul
  { 3,  0}, // #18 WriteIMulH
  { 2,  0}, // #19 WriteVarBlend_WriteFVarBlend_WriteVarVecShift_Write2P0156_Lat2_WriteXCHG_Write3P06_Lat2_WriteRCr1_WriteFCMOVcc_WriteMMXPACKSSrr_WritePEXTRr_WritePTESTr_WritePShift_WriteVMOVMSKPYr_WriteCVTPS2PDrr_WriteCVTSS2SDrr_WriteAESIMC_WriteAESKeyGen_WriteVecIMul_WriteFMul_WriteCLMul_WriteFRcp_WriteFRsqrt
  {11,  0}, // #20 WritePCmpEStrILd_WritePCmpIStrILd_WriteFRNDINT_WriteVRCPPSm_WriteRSQRTPSYm_WritePCmpEStrMLd_WritePCmpIStrMLd
  { 1,  0}, // #21 WriteALU
  { 1,  0}, // #22 WriteStore
  {19,  0}, // #23 WriteFSqrtLd_WriteFPREM_WriteVDIVPSYrr_WriteVSQRTPSYr_WriteFDiv
  {15,  0}, // #24 WriteFSqrt_WriteFXTRACT
  { 7,  0}, // #25 WriteCvtF2FLd
  { 1,  0}, // #26 WriteRMW
  {21,  0}, // #27 WriteXCHGrm_WriteFSqrt_WritePCmpEStrILd
  {22,  0}, // #28 WriteDiv8_WriteDiv32_WriteIDiv32
  {23,  0}, // #29 WriteDiv16_WriteIDiv8_WriteIDiv16_WriteVDIVPSYrm_WriteVSQRTPSYm
  {32,  0}, // #30 WriteDiv64_WriteVSQRTPDYm
  {39,  0}, // #31 WriteIDiv64
  {47,  0}, // #32 WriteFBLD
  {75,  0}, // #33 WriteFSCALE
  {13,  0}, // #34 WriteMASKMOVQ_WriteVPMASKMOVm_WriteEMMS_WriteVMASKMOVPmr_WriteDPPDm_WritePCmpEStrI_WritePCmpEStrM_WritePCmpIStrMLd
  { 8,  0}, // #35 WriteP1_P5_Lat4Ld_WriteCvtF2FLd
  { 1,  0}, // #36 WriteRMW
  {27,  0}, // #37 WriteVDIVPDYrr
  {31,  0}, // #38 WriteVDIVPDYrm
  {18,  0}, // #39 WriteDPPSm_WritePCmpEStrILd_WritePCmpEStrMLd_WriteFSqrtLd
  {28,  0}, // #40 WriteVSQRTPDYr
  { 4,  0}, // #41 WriteALULd_WriteShiftLd_WriteFBlendLd_WriteShuffleLd
  { 1,  0}, // #42 WriteRMW
  {41,  0}, // #43 WriteIDivLd
  {24,  0}, // #44 WriteFDivLd
  { 6,  0}, // #45 WriteFBlendLd_WriteShuffleLd_WriteIMulLd
  { 1,  0}, // #46 WriteRMW_WriteIMulH
  { 6,  0}, // #47 WriteIMulLd
  { 6,  0}, // #48 WriteIMulH
  { 3,  0}, // #49 WriteIMul
  { 6,  0}, // #50 WriteIMulH
  {26,  0}, // #51 WriteFSqrtLd
  {37,  0}, // #52 WriteFDivLd
  {34,  0}, // #53 WriteFDiv
  { 3,  0}, // #54 WriteIMul
  { 1,  0}, // #55 WriteIMulH
  {17,  0} // #56 WritePCmpEStrMLd_WritePCmpIStrILd
}; // X86WriteLatencyTable

// {UseIdx, WriteResourceID, Cycles}
extern const llvm::MCReadAdvanceEntry X86ReadAdvanceTable[] = {
  {0,  0,  0}, // Invalid
  {0,  0,  4}, // #1
  {5,  0,  4}, // #2
  {6,  0,  4}, // #3
  {0,  0,  3}, // #4
  {5,  0,  3}, // #5
  {6,  0,  3} // #6
}; // X86ReadAdvanceTable

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc HaswellModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0,  9, 4,  3, 1,  1, 1}, // #7
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0,  1, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0,  9, 4,  3, 1,  1, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 13, 9,  5, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 22, 7,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 29, 10,  5, 1,  1, 1}, // #26
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 39, 8,  5, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 47, 10,  7, 1,  0, 0}, // #28
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 57, 8,  7, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 65, 14,  8, 1,  0, 0}, // #30
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 79, 12,  8, 1,  0, 0}, // #31
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 17, 5,  3, 1,  1, 1}, // #32
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 26, 3,  2, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #35
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 17, 5,  3, 1,  1, 1}, // #37
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 17, 5,  3, 1,  1, 1}, // #39
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #55
  {DBGFIELD("WriteSystem")        1, 0, 0,  1, 2,  1, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #57
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  9, 4,  3, 1,  1, 1}, // #64
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  9, 4,  3, 1,  1, 1}, // #66
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #67
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #72
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #73
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #75
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #76
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #78
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #79
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 13, 9, 10, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 22, 7,  9, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 13, 9, 10, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 22, 7,  9, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 13, 9, 10, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 22, 7,  9, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #98
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #99
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #100
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #102
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #103
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #105
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #107
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #109
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #110
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #111
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 105, 11, 13, 1,  1, 1}, // #113
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 117, 9, 14, 1,  0, 0}, // #114
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 117, 9, 14, 1,  0, 0}, // #115
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 105, 11, 13, 1,  0, 0}, // #116
  {DBGFIELD("WriteFDiv")          1, 0, 0, 117, 9, 14, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 105, 11, 13, 1,  1, 1}, // #118
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 117, 9, 14, 1,  0, 0}, // #119
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 117, 9, 14, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #121
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #122
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #123
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 126, 7,  3, 2,  0, 0}, // #126
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #127
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #130
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #133
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #158
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #161
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #162
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #164
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #166
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #168
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 104, 12, 11, 1,  2, 2}, // #170
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 116, 10, 12, 1,  0, 0}, // #171
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #173
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #175
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #178
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #180
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #182
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #183
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #184
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #185
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #188
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #193
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 91, 6,  3, 1,  0, 0}, // #199
  {DBGFIELD("IIC_IRET_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #200
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #201
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #202
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #203
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 13, 9, 10, 1,  1, 1}, // #204
  {DBGFIELD("WriteLoad")          1, 0, 0, 101, 3,  9, 1,  0, 0}, // #205
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 97, 4,  2, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #208
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #212
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #215
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #216
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #218
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 101, 3,  9, 1,  0, 0}, // #223
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #224
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 137, 4,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 141, 5,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #235
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #236
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #237
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #241
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #244
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #245
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #246
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #247
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 141, 5,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #251
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 13, 9, 10, 1,  0, 0}, // #252
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 22, 7,  9, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #265
  {DBGFIELD("WriteMove")          1, 0, 0,  1, 2,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 146, 6,  3, 1,  0, 0}, // #267
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #271
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #272
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #273
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #274
  {DBGFIELD("WriteShuffle")       1, 0, 0, 39, 8,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 17, 5,  3, 1,  1, 1}, // #276
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecLogic") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #278
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecIMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #281
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #283
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #285
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #287
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #289
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #291
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 29, 10,  3, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 105, 11,  3, 1,  1, 1}, // #295
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #297
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #299
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #306
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #310
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #311
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #313
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #320
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #325
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #328
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #330
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #332
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #335
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #336
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #340
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #342
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 17, 5,  3, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0,  9, 4,  3, 1,  0, 0}, // #346
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #347
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 152, 14, 16, 1,  1, 1}, // #348
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 166, 12, 16, 1,  0, 0}, // #349
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #350
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #351
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #352
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #353
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #354
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  2, 2}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #358
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #361
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #363
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 13, 9,  5, 2,  0, 0}, // #364
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 22, 7, 17, 2,  0, 0}, // #365
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 105, 11, 15, 1,  0, 0}, // #366
  {DBGFIELD("WriteFMul")          1, 0, 0, 117, 9,  3, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #368
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #370
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #371
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 146, 6,  3, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #375
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #377
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #378
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #379
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #380
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 17, 5,  3, 1,  1, 1}, // #381
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #383
  {DBGFIELD("IIC_ALU_MEM_WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 47, 10, 16, 1,  1, 1}, // #384
  {DBGFIELD("IIC_ALU_NONMEM_WriteVarBlend") 1, 0, 0, 57, 8, 19, 1,  0, 0}, // #385
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #386
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteBlend") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 178, 14,  5, 1,  1, 1}, // #388
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 192, 12,  5, 1,  0, 0}, // #389
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #390
  {DBGFIELD("WriteVecALU")        1, 0, 0, 137, 4,  2, 1,  0, 0}, // #391
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 204, 15, 20, 1,  1, 1}, // #392
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 219, 6, 20, 1,  0, 0}, // #393
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 225, 8,  8, 1,  1, 1}, // #394
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 233, 13,  8, 1,  0, 0}, // #395
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 246, 11, 20, 1,  1, 1}, // #396
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 257, 9, 20, 1,  0, 0}, // #397
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 246, 11,  8, 1,  1, 1}, // #398
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 257, 9,  8, 1,  0, 0}, // #399
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 266, 12,  3, 2,  0, 0}, // #400
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #401
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #402
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #404
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #405
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 105, 11, 15, 1,  0, 0}, // #406
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 117, 9,  3, 1,  0, 0}, // #407
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #408
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #409
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #410
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #411
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 29, 10,  3, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #414
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #415
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #418
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #419
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #421
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #422
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #423
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #424
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #425
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #427
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #429
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #431
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 105, 11,  3, 1,  1, 1}, // #435
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #437
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #438
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_PUSH_MEM_WriteRMW") 1, 0, 0, 278, 3,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #442
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #443
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #444
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 97, 4,  2, 1,  0, 0}, // #447
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 105, 11, 15, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #450
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #451
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #452
  {DBGFIELD("WriteRMW")           1, 0, 0, 278, 3,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #455
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #456
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #458
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #459
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 91, 6,  3, 1,  0, 0}, // #460
  {DBGFIELD("WriteShift")         1, 0, 0, 97, 4,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #463
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #464
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #465
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SSE_RSQRTPS_RM_WriteFRsqrtLd") 1, 0, 0, 105, 11, 15, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_RSQRTPS_RR_WriteFRsqrt") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SSE_RSQRTSS_RM_WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #469
  {DBGFIELD("WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 105, 11, 15, 1,  1, 1}, // #470
  {DBGFIELD("IIC_SSE_RSQRTSS_RR_WriteFRsqrt") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #471
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 91, 6,  3, 1,  2, 1}, // #472
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 289, 5, 21, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #475
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 141, 5,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #482
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #483
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #486
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 281, 8,  3, 2,  0, 0}, // #487
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #490
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #494
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 105, 11, 23, 1,  0, 0}, // #495
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 117, 9, 24, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 105, 11, 23, 1,  0, 0}, // #497
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 117, 9, 24, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 105, 11, 23, 1,  1, 1}, // #499
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 105, 11, 23, 1,  1, 1}, // #500
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 117, 9, 24, 1,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 105, 11, 23, 1,  1, 1}, // #502
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 117, 9, 24, 1,  0, 0}, // #503
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 117, 9, 24, 1,  0, 0}, // #504
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #505
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #507
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #509
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #510
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 133, 4,  2, 1,  0, 0}, // #511
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #512
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #513
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #514
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #515
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #516
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #517
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #518
  {DBGFIELD("WriteJump")          1, 0, 0, 97, 4,  2, 1,  0, 0}, // #519
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #520
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #522
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_SSE_ALU_F64S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #524
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #525
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 47, 10, 16, 1,  1, 1}, // #526
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 57, 8, 19, 1,  0, 0}, // #527
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 29, 10,  3, 1,  0, 0}, // #528
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 39, 8,  6, 1,  0, 0}, // #529
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 39, 8,  2, 1,  0, 0}, // #530
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #531
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 13, 9,  5, 1,  0, 0}, // #532
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 22, 7,  6, 1,  0, 0}, // #533
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 13, 9,  5, 1,  0, 0}, // #534
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 22, 7,  6, 1,  0, 0}, // #535
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 294, 11, 25, 2,  0, 0}, // #536
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 13, 9,  5, 1,  1, 1}, // #537
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 13, 9, 10, 1,  1, 1}, // #538
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #539
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #540
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #541
  {DBGFIELD("WriteStore")         1, 0, 0, 133, 4,  2, 1,  0, 0}, // #542
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 39, 8,  6, 1,  0, 0}, // #543
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 126, 7,  3, 2,  0, 0}, // #544
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #545
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 29, 10,  5, 1,  1, 1}, // #546
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 29, 10,  3, 1,  1, 1}, // #547
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #548
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #549
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #550
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 152, 14, 16, 1,  1, 1}, // #556
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 166, 12, 16, 1,  0, 0}, // #557
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 146, 6,  3, 1,  0, 0}, // #558
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #559
  {DBGFIELD("WriteBlend")         1, 0, 0, 137, 4,  2, 1,  0, 0}, // #560
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 47, 10, 16, 1,  1, 1}, // #561
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 57, 8, 19, 1,  0, 0}, // #562
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 146, 6,  3, 1,  1, 1}, // #563
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #564
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 178, 14,  5, 1,  1, 1}, // #565
  {DBGFIELD("WriteCLMul")         1, 0, 0, 192, 12,  5, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 29, 10,  5, 1,  1, 1}, // #568
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 29, 10,  3, 1,  0, 0}, // #569
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #570
  {DBGFIELD("WriteVarVecShift_ReadAfterLd") 1, 0, 0, 192, 12, 19, 1,  1, 1}, // #571
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 192, 12, 19, 1,  0, 0}, // #572
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 29, 10,  3, 1,  0, 0}, // #573
  {DBGFIELD("WriteVecShift")      1, 0, 0, 117, 9,  2, 1,  0, 0}, // #574
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 178, 14, 16, 1,  1, 1}, // #575
  {DBGFIELD("WriteFRcpLd")        1, 0, 0, 105, 11, 15, 1,  0, 0}, // #576
  {DBGFIELD("WriteFRsqrtLd")      1, 0, 0, 105, 11, 15, 1,  0, 0}, // #577
  {DBGFIELD("WriteFSqrtLd")       1, 0, 0, 105, 11, 23, 1,  0, 0}, // #578
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #579
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #580
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #581
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #582
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #583
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 6,  3, 2,  0, 0}, // #584
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #585
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #586
  {DBGFIELD("MOV16rm")            1, 0, 0,  9, 4,  3, 1,  0, 0}, // #587
  {DBGFIELD("MOVSX32rm16_MOVSX32rm8") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #588
  {DBGFIELD("MOVZX32rm16_MOVZX32rm8") 1, 0, 0, 101, 3,  9, 1,  0, 0}, // #589
  {DBGFIELD("CMOVA16rr_CMOVAE16rr_CMOVB16rr_CMOVBE16rr_CMOVE16rr_CMOVG16rr_CMOVGE16rr_CMOVL16rr_CMOVLE16rr_CMOVNE16rr_CMOVNO16rr_CMOVNP16rr_CMOVNS16rr_CMOVO16rr_CMOVP16rr_CMOVS16rr") 1, 0, 0, 63, 2, 19, 1,  0, 0}, // #590
  {DBGFIELD("CMOVA32rr_CMOVA64rr_CMOVAE32rr_CMOVAE64rr_CMOVB32rr_CMOVB64rr_CMOVBE32rr_CMOVBE64rr_CMOVE32rr_CMOVE64rr_CMOVG32rr_CMOVG64rr_CMOVGE32rr_CMOVGE64rr_CMOVL32rr_CMOVL64rr_CMOVLE32rr_CMOVLE64rr_CMOVNE32rr_CMOVNE64rr_CMOVNO32rr_CMOVNO64rr_CMOVNP32rr_CMOVNP64rr_CMOVNS32rr_CMOVNS64rr_CMOVO32rr_CMOVO64rr_CMOVP32rr_CMOVP64rr_CMOVS32rr_CMOVS64rr") 1, 0, 0, 63, 2, 19, 1,  0, 0}, // #591
  {DBGFIELD("CMOVA16rm_CMOVAE16rm_CMOVB16rm_CMOVBE16rm_CMOVE16rm_CMOVG16rm_CMOVGE16rm_CMOVL16rm_CMOVLE16rm_CMOVNE16rm_CMOVNO16rm_CMOVNP16rm_CMOVNS16rm_CMOVO16rm_CMOVP16rm_CMOVS16rm") 1, 0, 0, 305, 4, 16, 1,  1, 1}, // #592
  {DBGFIELD("CMOVA32rm_CMOVA64rm_CMOVAE32rm_CMOVAE64rm_CMOVB32rm_CMOVB64rm_CMOVBE32rm_CMOVBE64rm_CMOVE32rm_CMOVE64rm_CMOVG32rm_CMOVG64rm_CMOVGE32rm_CMOVGE64rm_CMOVL32rm_CMOVL64rm_CMOVLE32rm_CMOVLE64rm_CMOVNE32rm_CMOVNE64rm_CMOVNO32rm_CMOVNO64rm_CMOVNP32rm_CMOVNP64rm_CMOVNS32rm_CMOVNS64rm_CMOVO32rm_CMOVO64rm_CMOVP32rm_CMOVP64rm_CMOVS32rm_CMOVS64rm") 1, 0, 0, 305, 4, 16, 1,  1, 1}, // #593
  {DBGFIELD("XCHG16ar_XCHG16rr_XCHG32ar_XCHG32ar64_XCHG32rr_XCHG64ar_XCHG64rr_XCHG8rr") 1, 0, 0, 176, 2, 19, 1,  0, 0}, // #594
  {DBGFIELD("XCHG16rm_XCHG32rm_XCHG64rm_XCHG8rm") 8, 0, 0,  0, 0, 27, 1,  0, 0}, // #595
  {DBGFIELD("XLAT")               3, 0, 0,  0, 0,  5, 1,  0, 0}, // #596
  {DBGFIELD("PUSH16rmm_PUSH32rmm") 1, 0, 0, 309, 4,  2, 1,  0, 0}, // #597
  {DBGFIELD("PUSHF16_PUSHF32")    4, 0, 0, 313, 12,  2, 1,  0, 0}, // #598
  {DBGFIELD("PUSHA16_PUSHA32")    19, 0, 0,  0, 0,  2, 1,  0, 0}, // #599
  {DBGFIELD("POP16rmm_POP32rmm")  1, 0, 0, 309, 4,  2, 1,  0, 0}, // #600
  {DBGFIELD("POPF16")             9, 0, 0,  0, 0,  2, 1,  0, 0}, // #601
  {DBGFIELD("POPF32")             9, 0, 0,  0, 0,  2, 1,  0, 0}, // #602
  {DBGFIELD("POPA16_POPA32")      18, 0, 0,  0, 0,  2, 1,  0, 0}, // #603
  {DBGFIELD("LAHF_SAHF")          1, 0, 0, 97, 4,  2, 1,  0, 0}, // #604
  {DBGFIELD("BSWAP32r")           1, 0, 0, 137, 4,  2, 1,  0, 0}, // #605
  {DBGFIELD("BSWAP64r")           2, 0, 0, 325, 6,  2, 1,  0, 0}, // #606
  {DBGFIELD("MOVBE16rm_MOVBE64rm") 1, 0, 0, 305, 4, 16, 1,  0, 0}, // #607
  {DBGFIELD("MOVBE32rm")          2, 0, 0, 146, 6,  2, 1,  0, 0}, // #608
  {DBGFIELD("MOVBE16mr")          3, 0, 0, 331, 7,  2, 1,  0, 0}, // #609
  {DBGFIELD("MOVBE32mr")          3, 0, 0, 338, 7,  2, 1,  0, 0}, // #610
  {DBGFIELD("MOVBE64mr")          4, 0, 0, 345, 9,  2, 1,  0, 0}, // #611
  {DBGFIELD("ADD16mi_ADD16mi8_ADD16mr_ADD32mi_ADD32mi8_ADD32mr_ADD64mi32_ADD64mi8_ADD64mr_ADD8mi_ADD8mi8_ADD8mr_SUB16mi_SUB16mi8_SUB16mr_SUB32mi_SUB32mi8_SUB32mr_SUB64mi32_SUB64mi8_SUB64mr_SUB8mi_SUB8mi8_SUB8mr") 1, 0, 0, 354, 5,  2, 1,  0, 0}, // #612
  {DBGFIELD("ADC16ri_ADC16ri8_ADC16rr_ADC16rr_REV_ADC32ri_ADC32ri8_ADC32rr_ADC32rr_REV_ADC64ri32_ADC64ri8_ADC64rr_ADC64rr_REV_ADC8ri_ADC8ri8_ADC8rr_ADC8rr_REV_SBB16ri_SBB16ri8_SBB16rr_SBB16rr_REV_SBB32ri_SBB32ri8_SBB32rr_SBB32rr_REV_SBB64ri32_SBB64ri8_SBB64rr_SBB64rr_REV_SBB8ri_SBB8ri8_SBB8rr_SBB8rr_REV") 1, 0, 0, 63, 2, 19, 1,  0, 0}, // #613
  {DBGFIELD("ADC16rm_ADC32rm_ADC64rm_ADC8rm_SBB16rm_SBB32rm_SBB64rm_SBB8rm") 1, 0, 0, 305, 4, 16, 1,  1, 1}, // #614
  {DBGFIELD("ADC16mi_ADC16mi8_ADC16mr_ADC32mi_ADC32mi8_ADC32mr_ADC64mi32_ADC64mi8_ADC64mr_ADC8mi_ADC8mi8_ADC8mr_SBB16mi_SBB16mi8_SBB16mr_SBB32mi_SBB32mi8_SBB32mr_SBB64mi32_SBB64mi8_SBB64mr_SBB8mi_SBB8mi8_SBB8mr") 1, 0, 0, 359, 5,  2, 1,  0, 0}, // #615
  {DBGFIELD("DEC16m_DEC32m_DEC64m_DEC8m_INC16m_INC32m_INC64m_INC8m_NEG16m_NEG32m_NEG64m_NEG8m_NOT16m_NOT32m_NOT64m_NOT8m") 1, 0, 0, 364, 5,  2, 1,  0, 0}, // #616
  {DBGFIELD("IMUL16r_IMUL16rr")   4, 0, 0, 369, 7,  9, 1,  0, 0}, // #617
  {DBGFIELD("IMUL16rm")           4, 0, 0, 369, 7,  9, 1,  0, 0}, // #618
  {DBGFIELD("IMUL16rmi_IMUL16rmi8") 4, 0, 0, 369, 7,  9, 1,  0, 0}, // #619
  {DBGFIELD("IMUL16rri_IMUL16rri8") 4, 0, 0, 369, 7,  9, 1,  0, 0}, // #620
  {DBGFIELD("MUL16r")             4, 0, 0, 369, 7,  9, 1,  0, 0}, // #621
  {DBGFIELD("IMUL16m")            5, 0, 0, 376, 9, 10, 1,  0, 0}, // #622
  {DBGFIELD("MUL16m")             5, 0, 0, 376, 9, 10, 1,  0, 0}, // #623
  {DBGFIELD("IMUL32r_IMUL32rr")   3, 0, 0, 369, 7,  9, 1,  0, 0}, // #624
  {DBGFIELD("IMUL32rm")           3, 0, 0, 369, 7,  9, 1,  0, 0}, // #625
  {DBGFIELD("IMUL32rmi_IMUL32rmi8") 3, 0, 0, 369, 7,  9, 1,  0, 0}, // #626
  {DBGFIELD("IMUL32rri_IMUL32rri8") 3, 0, 0, 369, 7,  9, 1,  0, 0}, // #627
  {DBGFIELD("MUL32r")             3, 0, 0, 369, 7,  9, 1,  0, 0}, // #628
  {DBGFIELD("IMUL32m")            4, 0, 0, 376, 9, 10, 1,  0, 0}, // #629
  {DBGFIELD("MUL32m")             4, 0, 0, 376, 9, 10, 1,  0, 0}, // #630
  {DBGFIELD("IMUL64r_IMUL64rr")   2, 0, 0, 385, 11,  6, 1,  0, 0}, // #631
  {DBGFIELD("IMUL64rm")           2, 0, 0, 385, 11,  6, 1,  0, 0}, // #632
  {DBGFIELD("IMUL64rmi32_IMUL64rmi8") 2, 0, 0, 385, 11,  6, 1,  0, 0}, // #633
  {DBGFIELD("IMUL64rri32_IMUL64rri8") 2, 0, 0, 385, 11,  6, 1,  0, 0}, // #634
  {DBGFIELD("MUL64r")             2, 0, 0, 385, 11,  6, 1,  0, 0}, // #635
  {DBGFIELD("IMUL64m")            3, 0, 0, 396, 13,  5, 1,  0, 0}, // #636
  {DBGFIELD("MUL64m")             3, 0, 0, 396, 13,  5, 1,  0, 0}, // #637
  {DBGFIELD("MULX32rr")           3, 0, 0, 409, 8,  9, 1,  0, 0}, // #638
  {DBGFIELD("MULX32rm")           4, 0, 0, 417, 10, 10, 1,  0, 0}, // #639
  {DBGFIELD("MULX64rr")           2, 0, 0, 385, 11,  9, 1,  0, 0}, // #640
  {DBGFIELD("MULX64rm")           3, 0, 0, 396, 13, 10, 1,  0, 0}, // #641
  {DBGFIELD("DIV8r")              9, 0, 0, 427, 15, 28, 1,  0, 0}, // #642
  {DBGFIELD("DIV16r")             10, 0, 0, 427, 15, 29, 1,  0, 0}, // #643
  {DBGFIELD("DIV32r")             10, 0, 0, 427, 15, 28, 1,  0, 0}, // #644
  {DBGFIELD("DIV64r")             36, 0, 0, 427, 15, 30, 1,  0, 0}, // #645
  {DBGFIELD("IDIV8r")             9, 0, 0, 427, 15, 29, 1,  0, 0}, // #646
  {DBGFIELD("IDIV16r")            10, 0, 0, 427, 15, 29, 1,  0, 0}, // #647
  {DBGFIELD("IDIV32r")            9, 0, 0, 427, 15, 28, 1,  0, 0}, // #648
  {DBGFIELD("IDIV64r")            59, 0, 0, 427, 15, 31, 1,  0, 0}, // #649
  {DBGFIELD("AND16mi_AND16mi8_AND16mr_AND32mi_AND32mi8_AND32mr_AND64mi32_AND64mi8_AND64mr_AND8mi_AND8mi8_AND8mr_OR16mi_OR16mi8_OR16mr_OR32mi_OR32mi8_OR32mr_OR64mi32_OR64mi8_OR64mr_OR8mi_OR8mi8_OR8mr_XOR16mi_XOR16mi8_XOR16mr_XOR32mi_XOR32mi8_XOR32mr_XOR64mi32_XOR64mi8_XOR64mr_XOR8mi_XOR8mi8_XOR8mr") 1, 0, 0, 354, 5,  2, 1,  0, 0}, // #650
  {DBGFIELD("OR32mrLocked")       1, 0, 0, 354, 5,  2, 1,  0, 0}, // #651
  {DBGFIELD("SAR16m1_SAR16mi_SAR32m1_SAR32mi_SAR64m1_SAR64mi_SAR8m1_SAR8mi_SHL16m1_SHL16mi_SHL32m1_SHL32mi_SHL64m1_SHL64mi_SHL8m1_SHL8mi_SHR16m1_SHR16mi_SHR32m1_SHR32mi_SHR64m1_SHR64mi_SHR8m1_SHR8mi") 4, 0, 0, 442, 7,  2, 1,  0, 0}, // #652
  {DBGFIELD("SAR16rCL_SAR32rCL_SAR64rCL_SAR8rCL_SHL16rCL_SHL32rCL_SHL64rCL_SHL8rCL_SHR16rCL_SHR32rCL_SHR64rCL_SHR8rCL") 3, 0, 0, 449, 4, 19, 1,  0, 0}, // #653
  {DBGFIELD("SAR16mCL_SAR32mCL_SAR64mCL_SAR8mCL_SHL16mCL_SHL32mCL_SHL64mCL_SHL8mCL_SHR16mCL_SHR32mCL_SHR64mCL_SHR8mCL") 6, 0, 0, 453, 8,  2, 1,  0, 0}, // #654
  {DBGFIELD("ROL16r1_ROL32r1_ROL64r1_ROL8r1_ROR16r1_ROR32r1_ROR64r1_ROR8r1") 2, 0, 0, 461, 4,  2, 1,  0, 0}, // #655
  {DBGFIELD("ROL16mi_ROL32mi_ROL64mi_ROL8mi_ROR16mi_ROR32mi_ROR64mi_ROR8mi") 5, 0, 0, 465, 7,  2, 1,  0, 0}, // #656
  {DBGFIELD("ROL16rCL_ROL32rCL_ROL64rCL_ROL8rCL_ROR16rCL_ROR32rCL_ROR64rCL_ROR8rCL") 3, 0, 0, 449, 4, 19, 1,  0, 0}, // #657
  {DBGFIELD("ROL16mCL_ROL32mCL_ROL64mCL_ROL8mCL_ROR16mCL_ROR32mCL_ROR64mCL_ROR8mCL") 6, 0, 0,  0, 0,  2, 1,  0, 0}, // #658
  {DBGFIELD("RCL16r1_RCL32r1_RCL64r1_RCL8r1_RCR16r1_RCR32r1_RCR64r1_RCR8r1") 3, 0, 0, 472, 4, 19, 1,  0, 0}, // #659
  {DBGFIELD("RCL16m1_RCL32m1_RCL64m1_RCL8m1_RCR16m1_RCR32m1_RCR64m1_RCR8m1") 6, 0, 0,  0, 0,  2, 1,  0, 0}, // #660
  {DBGFIELD("RCL16rCL_RCL16ri_RCL32rCL_RCL32ri_RCL64rCL_RCL64ri_RCL8rCL_RCL8ri_RCR16rCL_RCR16ri_RCR32rCL_RCR32ri_RCR64rCL_RCR64ri_RCR8rCL_RCR8ri") 8, 0, 0,  1, 2, 16, 1,  0, 0}, // #661
  {DBGFIELD("RCL16mCL_RCL16mi_RCL32mCL_RCL32mi_RCL64mCL_RCL64mi_RCL8mCL_RCL8mi_RCR16mCL_RCR16mi_RCR32mCL_RCR32mi_RCR64mCL_RCR64mi_RCR8mCL_RCR8mi") 11, 0, 0,  0, 0,  2, 1,  0, 0}, // #662
  {DBGFIELD("SHLD16rri8_SHRD16rri8") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #663
  {DBGFIELD("SHLD32rri8_SHRD32rri8") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #664
  {DBGFIELD("SHLD64rri8_SHRD64rri8") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #665
  {DBGFIELD("SHLD16mri8_SHRD16mri8") 5, 0, 0,  0, 0,  2, 1,  0, 0}, // #666
  {DBGFIELD("SHLD32mri8_SHRD32mri8") 5, 0, 0,  0, 0,  2, 1,  0, 0}, // #667
  {DBGFIELD("SHLD64mri8_SHRD64mri8") 5, 0, 0,  0, 0,  2, 1,  0, 0}, // #668
  {DBGFIELD("SHLD16rrCL")         4, 0, 0,  1, 2,  6, 1,  0, 0}, // #669
  {DBGFIELD("SHLD32rrCL")         4, 0, 0,  1, 2,  6, 1,  0, 0}, // #670
  {DBGFIELD("SHLD64rrCL")         4, 0, 0,  1, 2,  6, 1,  0, 0}, // #671
  {DBGFIELD("SHRD16rrCL")         4, 0, 0,  1, 2,  9, 1,  0, 0}, // #672
  {DBGFIELD("SHRD32rrCL")         4, 0, 0,  1, 2,  9, 1,  0, 0}, // #673
  {DBGFIELD("SHRD64rrCL")         4, 0, 0,  1, 2,  9, 1,  0, 0}, // #674
  {DBGFIELD("SHLD16mrCL_SHRD16mrCL") 7, 0, 0,  0, 0,  2, 1,  0, 0}, // #675
  {DBGFIELD("SHLD32mrCL_SHRD32mrCL") 7, 0, 0,  0, 0,  2, 1,  0, 0}, // #676
  {DBGFIELD("SHLD64mrCL_SHRD64mrCL") 7, 0, 0,  0, 0,  2, 1,  0, 0}, // #677
  {DBGFIELD("BT16ri8_BT32ri8_BT64ri8") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #678
  {DBGFIELD("BT16rr_BT32rr_BT64rr") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #679
  {DBGFIELD("BT16mr_BT32mr_BT64mr") 10, 0, 0,  0, 0,  2, 1,  0, 0}, // #680
  {DBGFIELD("BT16mi8_BT32mi8_BT64mi8") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #681
  {DBGFIELD("BTC16ri8_BTC32ri8_BTC64ri8_BTR16ri8_BTR32ri8_BTR64ri8_BTS16ri8_BTS32ri8_BTS64ri8") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #682
  {DBGFIELD("BTC16rr_BTC32rr_BTC64rr_BTR16rr_BTR32rr_BTS16rr_BTS32rr_BTS64rr") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #683
  {DBGFIELD("BTR64rr")            1, 0, 0, 97, 4,  2, 1,  0, 0}, // #684
  {DBGFIELD("BTC16mr_BTC32mr_BTC64mr_BTR16mr_BTR32mr_BTR64mr_BTS16mr_BTS32mr_BTS64mr") 11, 0, 0,  0, 0,  2, 1,  0, 0}, // #685
  {DBGFIELD("BTC16mi8_BTC32mi8_BTC64mi8_BTR16mi8_BTR32mi8_BTR64mi8_BTS16mi8_BTS32mi8_BTS64mi8") 1, 0, 0, 91, 6,  3, 1,  0, 0}, // #686
  {DBGFIELD("BSF16rr_BSF32rr_BSF64rr_BSR16rr_BSR32rr_BSR64rr") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #687
  {DBGFIELD("BSF16rm_BSF32rm_BSF64rm_BSR16rm_BSR32rm_BSR64rm") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #688
  {DBGFIELD("SETAEr_SETAr_SETBEr_SETBr_SETEr_SETGEr_SETGr_SETLEr_SETLr_SETNEr_SETNOr_SETNPr_SETNSr_SETOr_SETPr_SETSr") 1, 0, 0, 97, 4,  2, 1,  0, 0}, // #689
  {DBGFIELD("SETAEm_SETAm_SETBEm_SETBm_SETEm_SETGEm_SETGm_SETLEm_SETLm_SETNEm_SETNOm_SETNPm_SETNSm_SETOm_SETPm_SETSm") 3, 0, 0, 331, 7,  2, 1,  0, 0}, // #690
  {DBGFIELD("CLD")                3, 0, 0, 476, 9,  2, 1,  0, 0}, // #691
  {DBGFIELD("STD")                3, 0, 0, 476, 9,  2, 1,  0, 0}, // #692
  {DBGFIELD("TZCNT16rr_TZCNT32rr_TZCNT64rr") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #693
  {DBGFIELD("TZCNT16rm_TZCNT32rm_TZCNT64rm") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #694
  {DBGFIELD("ANDN32rr_ANDN64rr")  1, 0, 0, 137, 4,  2, 1,  0, 0}, // #695
  {DBGFIELD("ANDN32rm_ANDN64rm")  1, 0, 0, 146, 6,  9, 1,  0, 0}, // #696
  {DBGFIELD("BLSI32rr_BLSI64rr_BLSMSK32rr_BLSMSK64rr_BLSR32rr_BLSR64rr") 1, 0, 0, 137, 4,  2, 1,  0, 0}, // #697
  {DBGFIELD("BLSI32rm_BLSI64rm_BLSMSK32rm_BLSMSK64rm_BLSR32rm_BLSR64rm") 1, 0, 0, 146, 6,  9, 1,  0, 0}, // #698
  {DBGFIELD("BEXTR32rr_BEXTR64rr") 1, 0, 0, 63, 2, 19, 1,  0, 0}, // #699
  {DBGFIELD("BEXTR32rm_BEXTR64rm") 1, 0, 0, 305, 4, 16, 1,  0, 0}, // #700
  {DBGFIELD("BZHI32rr_BZHI64rr")  1, 0, 0, 137, 4,  2, 1,  0, 0}, // #701
  {DBGFIELD("BZHI32rm_BZHI64rm")  1, 0, 0, 146, 6,  9, 1,  0, 0}, // #702
  {DBGFIELD("PDEP32rr_PDEP64rr_PEXT32rr_PEXT64rr") 1, 0, 0, 22, 7,  6, 1,  0, 0}, // #703
  {DBGFIELD("PDEP32rm_PDEP64rm_PEXT32rm_PEXT64rm") 1, 0, 0, 13, 9,  5, 1,  0, 0}, // #704
  {DBGFIELD("JCXZ_JRCXZ")         2, 0, 0, 485, 7,  2, 1,  0, 0}, // #705
  {DBGFIELD("LOOP")               7, 0, 0,  0, 0,  2, 1,  0, 0}, // #706
  {DBGFIELD("LOOPE")              7, 0, 0,  0, 0,  2, 1,  0, 0}, // #707
  {DBGFIELD("LOOPNE")             7, 0, 0,  0, 0,  2, 1,  0, 0}, // #708
  {DBGFIELD("CALL16r_CALL32r")    3, 0, 0, 492, 10,  2, 1,  0, 0}, // #709
  {DBGFIELD("CALL16m_CALL32m")    4, 0, 0, 502, 10,  2, 1,  0, 0}, // #710
  {DBGFIELD("LRETL_LRETQ_LRETW_RETL_RETQ_RETW") 2, 0, 0, 512, 8,  2, 1,  0, 0}, // #711
  {DBGFIELD("LRETIL_LRETIQ_LRETIW") 4, 0, 0, 520, 10,  2, 1,  0, 0}, // #712
  {DBGFIELD("RETIL_RETIQ_RETIW")  4, 0, 0, 520, 10,  2, 1,  0, 0}, // #713
  {DBGFIELD("BOUNDS16rm_BOUNDS32rm") 15, 0, 0,  0, 0,  2, 1,  0, 0}, // #714
  {DBGFIELD("INTO")               4, 0, 0,  0, 0,  2, 1,  0, 0}, // #715
  {DBGFIELD("LODSB_LODSW")        3, 0, 0, 305, 4,  2, 1,  0, 0}, // #716
  {DBGFIELD("LODSL_LODSQ")        2, 0, 0,  9, 4,  2, 1,  0, 0}, // #717
  {DBGFIELD("STOSB_STOSL_STOSQ_STOSW") 3, 0, 0,  3, 6,  2, 1,  0, 0}, // #718
  {DBGFIELD("MOVSB_MOVSL_MOVSQ_MOVSW") 5, 0, 0, 530, 6,  9, 1,  0, 0}, // #719
  {DBGFIELD("MOVSLDUPrm")         5, 0, 0, 530, 6,  9, 1,  0, 0}, // #720
  {DBGFIELD("MOVSLDUPrr")         5, 0, 0, 530, 6,  9, 1,  0, 0}, // #721
  {DBGFIELD("SCASB_SCASL_SCASQ_SCASW") 3, 0, 0, 305, 4,  2, 1,  0, 0}, // #722
  {DBGFIELD("CMPSB_CMPSL_CMPSQ_CMPSW") 5, 0, 0, 536, 4,  9, 1,  0, 0}, // #723
  {DBGFIELD("XADD16rm_XADD32rm_XADD64rm_XADD8rm") 5, 0, 0,  0, 0,  2, 1,  0, 0}, // #724
  {DBGFIELD("CMPXCHG16rm_CMPXCHG32rm_CMPXCHG64rm") 6, 0, 0,  0, 0,  2, 1,  0, 0}, // #725
  {DBGFIELD("CMPXCHG8rm")         6, 0, 0,  0, 0,  2, 1,  0, 0}, // #726
  {DBGFIELD("CMPXCHG8B")          15, 0, 0,  0, 0,  2, 1,  0, 0}, // #727
  {DBGFIELD("CMPXCHG16B")         22, 0, 0,  0, 0,  2, 1,  0, 0}, // #728
  {DBGFIELD("PAUSE")              5, 0, 0, 540, 9,  2, 1,  0, 0}, // #729
  {DBGFIELD("LEAVE_LEAVE64")      3, 0, 0, 305, 4,  2, 1,  0, 0}, // #730
  {DBGFIELD("XGETBV")             8, 0, 0,  0, 0,  2, 1,  0, 0}, // #731
  {DBGFIELD("RDTSC")              15, 0, 0,  0, 0,  2, 1,  0, 0}, // #732
  {DBGFIELD("RDTSCP")             15, 0, 0,  0, 0,  2, 1,  0, 0}, // #733
  {DBGFIELD("RDPMC")              34, 0, 0,  0, 0,  2, 1,  0, 0}, // #734
  {DBGFIELD("RDRAND16r_RDRAND32r_RDRAND64r") 17, 0, 0, 549, 5,  2, 1,  0, 0}, // #735
  {DBGFIELD("LD_Frr")             1, 0, 0, 554, 4,  2, 1,  0, 0}, // #736
  {DBGFIELD("LD_F80m")            4, 0, 0, 558, 6,  9, 1,  0, 0}, // #737
  {DBGFIELD("FBLDm")              43, 0, 0,  0, 0, 32, 1,  0, 0}, // #738
  {DBGFIELD("ST_FPrr_ST_Frr")     1, 0, 0, 554, 4,  2, 1,  0, 0}, // #739
  {DBGFIELD("ST_FP80m")           7, 0, 0, 564, 6,  2, 1,  0, 0}, // #740
  {DBGFIELD("FBSTPm")             226, 0, 0,  0, 0,  2, 1,  0, 0}, // #741
  {DBGFIELD("XCH_F")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #742
  {DBGFIELD("ILD_F16m_ILD_F32m_ILD_F64m") 2, 0, 0, 570, 6, 16, 1,  0, 0}, // #743
  {DBGFIELD("IST_F16m_IST_F32m_IST_FP16m_IST_FP32m") 3, 0, 0, 294, 11,  5, 1,  0, 0}, // #744
  {DBGFIELD("LD_F0")              1, 0, 0, 554, 4,  2, 1,  0, 0}, // #745
  {DBGFIELD("LD_F1")              2, 0, 0, 554, 4,  2, 1,  0, 0}, // #746
  {DBGFIELD("FLDPI")              2, 0, 0, 554, 4,  2, 1,  0, 0}, // #747
  {DBGFIELD("CMOVBE_F_CMOVBE_Fp32_CMOVBE_Fp64_CMOVBE_Fp80_CMOVB_F_CMOVB_Fp32_CMOVB_Fp64_CMOVB_Fp80_CMOVNBE_F_CMOVNBE_Fp32_CMOVNBE_Fp64_CMOVNBE_Fp80_CMOVNB_F_CMOVNB_Fp32_CMOVNB_Fp64_CMOVNB_Fp80_CMOVNE_F_CMOVNE_Fp32_CMOVNE_Fp64_CMOVNE_Fp80_CMOVNP_F_CMOVNP_Fp32_CMOVNP_Fp64_CMOVNP_Fp80_CMOVP_F_CMOVP_Fp32_CMOVP_Fp64_CMOVP_Fp80") 3, 0, 0, 192, 12, 19, 1,  0, 0}, // #748
  {DBGFIELD("FNSTSW16r")          2, 0, 0, 576, 9,  2, 1,  0, 0}, // #749
  {DBGFIELD("FNSTSWm")            3, 0, 0, 585, 11, 16, 1,  0, 0}, // #750
  {DBGFIELD("FLDCW16m")           3, 0, 0, 596, 11,  5, 1,  0, 0}, // #751
  {DBGFIELD("FNSTCW16m")          3, 0, 0, 492, 10,  2, 1,  0, 0}, // #752
  {DBGFIELD("FDECSTP_FINCSTP")    1, 0, 0, 554, 4,  2, 1,  0, 0}, // #753
  {DBGFIELD("FFREE")              1, 0, 0, 554, 4,  2, 1,  0, 0}, // #754
  {DBGFIELD("FSAVEm")             147, 0, 0,  0, 0,  2, 1,  0, 0}, // #755
  {DBGFIELD("FRSTORm")            90, 0, 0,  0, 0,  2, 1,  0, 0}, // #756
  {DBGFIELD("ABS_F_ABS_Fp32_ABS_Fp64_ABS_Fp80") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #757
  {DBGFIELD("CHS_F_CHS_Fp32_CHS_Fp64_CHS_Fp80") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #758
  {DBGFIELD("COMP_FST0r_COM_FST0r") 1, 0, 0, 22, 7,  2, 1,  0, 0}, // #759
  {DBGFIELD("UCOM_FPr_UCOM_Fr")   1, 0, 0, 22, 7,  2, 1,  0, 0}, // #760
  {DBGFIELD("FCOM32m_FCOM64m_FCOMP32m_FCOMP64m") 2, 0, 0, 13, 9,  2, 1,  0, 0}, // #761
  {DBGFIELD("FCOMPP")             2, 0, 0, 554, 4,  2, 1,  0, 0}, // #762
  {DBGFIELD("UCOM_FPPr")          2, 0, 0, 554, 4,  2, 1,  0, 0}, // #763
  {DBGFIELD("COM_FIPr_COM_FIr")   3, 0, 0, 554, 4,  2, 1,  0, 0}, // #764
  {DBGFIELD("UCOM_FIPr_UCOM_FIr") 3, 0, 0, 554, 4,  2, 1,  0, 0}, // #765
  {DBGFIELD("FICOM16m_FICOM32m_FICOMP16m_FICOMP32m") 3, 0, 0, 607, 9,  2, 1,  0, 0}, // #766
  {DBGFIELD("TST_F_TST_Fp32_TST_Fp64_TST_Fp80") 1, 0, 0, 22, 7,  2, 1,  0, 0}, // #767
  {DBGFIELD("FXAM")               2, 0, 0, 616, 7,  2, 1,  0, 0}, // #768
  {DBGFIELD("FPREM")              28, 0, 0,  0, 0, 23, 1,  0, 0}, // #769
  {DBGFIELD("FPREM1")             28, 0, 0,  0, 0, 23, 1,  0, 0}, // #770
  {DBGFIELD("FRNDINT")            17, 0, 0,  0, 0, 20, 1,  0, 0}, // #771
  {DBGFIELD("FSCALE")             50, 0, 0,  0, 0, 33, 1,  0, 0}, // #772
  {DBGFIELD("FXTRACT")            17, 0, 0,  0, 0, 24, 1,  0, 0}, // #773
  {DBGFIELD("FNOP")               1, 0, 0, 554, 4,  2, 1,  0, 0}, // #774
  {DBGFIELD("WAIT")               2, 0, 0, 554, 4,  2, 1,  0, 0}, // #775
  {DBGFIELD("FNCLEX")             5, 0, 0, 623, 2,  2, 1,  0, 0}, // #776
  {DBGFIELD("FNINIT")             26, 0, 0,  0, 0,  2, 1,  0, 0}, // #777
  {DBGFIELD("MMX_MOVD64from64rr_MMX_MOVD64grr") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #778
  {DBGFIELD("MOVPDI2DIrr_VMOVPDI2DIrr") 1, 0, 0, 117, 9,  2, 1,  0, 0}, // #779
  {DBGFIELD("MMX_MOVD64rr_MMX_MOVD64to64rr") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #780
  {DBGFIELD("MOVDI2PDIrr_VMOVDI2PDIrr") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #781
  {DBGFIELD("VMOVPQIto64rr")      1, 0, 0, 117, 9,  2, 1,  0, 0}, // #782
  {DBGFIELD("VMOV64toPQIrr")      1, 0, 0, 39, 8,  2, 1,  0, 0}, // #783
  {DBGFIELD("MMX_MOVQ64rr_MMX_MOVQ64rr_REV") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #784
  {DBGFIELD("MOVDQArr_MOVDQArr_REV_VMOVDQAYrr_VMOVDQAYrr_REV_VMOVDQArr_VMOVDQArr_REV") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #785
  {DBGFIELD("MOVDQUrr_MOVDQUrr_REV_VMOVDQUYrr_VMOVDQUYrr_REV_VMOVDQUrr_VMOVDQUrr_REV") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #786
  {DBGFIELD("MMX_MOVDQ2Qrr")      2, 0, 0, 625, 9,  2, 1,  0, 0}, // #787
  {DBGFIELD("MMX_MOVQ2DQrr")      1, 0, 0, 26, 3,  2, 1,  0, 0}, // #788
  {DBGFIELD("MMX_PACKSSDWirr_MMX_PACKSSWBirr_MMX_PACKUSWBirr") 3, 0, 0, 634, 8, 19, 1,  0, 0}, // #789
  {DBGFIELD("MMX_PACKSSDWirm_MMX_PACKSSWBirm_MMX_PACKUSWBirm") 3, 0, 0, 642, 10,  9, 1,  0, 0}, // #790
  {DBGFIELD("VPMOVSXBQYrr_VPMOVSXBWYrr_VPMOVSXDQYrr_VPMOVZXBQYrr_VPMOVZXBWYrr_VPMOVZXDQYrr") 1, 0, 0, 39, 8,  6, 1,  0, 0}, // #791
  {DBGFIELD("PBLENDWrri")         1, 0, 0, 39, 8,  2, 1,  0, 0}, // #792
  {DBGFIELD("VPBLENDWYrri_VPBLENDWrri") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #793
  {DBGFIELD("PBLENDWrmi")         2, 0, 0, 29, 10,  9, 1,  1, 1}, // #794
  {DBGFIELD("VPBLENDWYrmi_VPBLENDWrmi") 2, 0, 0, 29, 10,  9, 1,  1, 1}, // #795
  {DBGFIELD("VPBLENDDYrri_VPBLENDDrri") 1, 0, 0, 26, 3,  2, 1,  0, 0}, // #796
  {DBGFIELD("VPBLENDDYrmi_VPBLENDDrmi") 2, 0, 0, 17, 5,  9, 1,  1, 1}, // #797
  {DBGFIELD("MMX_MASKMOVQ_MMX_MASKMOVQ64") 4, 0, 0, 652, 12, 34, 1,  0, 0}, // #798
  {DBGFIELD("MASKMOVDQU_MASKMOVDQU64_VMASKMOVDQU_VMASKMOVDQU64") 10, 0, 0, 664, 7,  7, 1,  0, 0}, // #799
  {DBGFIELD("VPMASKMOVDYrm_VPMASKMOVDrm_VPMASKMOVQYrm_VPMASKMOVQrm") 3, 0, 0, 47, 10,  9, 1,  1, 1}, // #800
  {DBGFIELD("VPMASKMOVDYmr_VPMASKMOVDmr_VPMASKMOVQYmr_VPMASKMOVQmr") 4, 0, 0, 671, 15, 34, 1,  0, 0}, // #801
  {DBGFIELD("MMX_PMOVMSKBrr")     1, 0, 0, 117, 9,  6, 1,  0, 0}, // #802
  {DBGFIELD("PMOVMSKBrr_VPMOVMSKBrr") 1, 0, 0, 117, 9,  6, 1,  0, 0}, // #803
  {DBGFIELD("VPMOVMSKBYrr")       1, 0, 0, 117, 9,  6, 1,  0, 0}, // #804
  {DBGFIELD("MMX_PEXTRWirri")     2, 0, 0, 686, 12, 19, 1,  0, 0}, // #805
  {DBGFIELD("PEXTRBrr_PEXTRDrr_PEXTRQrr_PEXTRWrr_REV") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #806
  {DBGFIELD("PEXTRBmr_PEXTRDmr_PEXTRQmr_PEXTRWmr") 3, 0, 0, 266, 12,  2, 1,  0, 0}, // #807
  {DBGFIELD("VPBROADCASTBrm_VPBROADCASTWrm") 3, 0, 0, 698, 11,  3, 1,  1, 1}, // #808
  {DBGFIELD("VPBROADCASTBYrm_VPBROADCASTWYrm") 3, 0, 0, 698, 11,  5, 1,  1, 1}, // #809
  {DBGFIELD("VPGATHERDDrm")       20, 0, 0,  0, 0,  2, 1,  1, 1}, // #810
  {DBGFIELD("VPGATHERDDYrm")      34, 0, 0,  0, 0,  2, 1,  1, 1}, // #811
  {DBGFIELD("VPGATHERQDrm")       15, 0, 0,  0, 0,  2, 1,  1, 1}, // #812
  {DBGFIELD("VPGATHERQDYrm")      22, 0, 0,  0, 0,  2, 1,  1, 1}, // #813
  {DBGFIELD("VPGATHERDQrm")       12, 0, 0,  0, 0,  2, 1,  1, 1}, // #814
  {DBGFIELD("VPGATHERDQYrm")      20, 0, 0,  0, 0,  2, 1,  1, 1}, // #815
  {DBGFIELD("VPGATHERQQrm")       14, 0, 0,  0, 0,  2, 1,  1, 1}, // #816
  {DBGFIELD("VPGATHERQQYrm")      22, 0, 0,  0, 0,  2, 1,  1, 1}, // #817
  {DBGFIELD("MMX_PHADDSWrr64_MMX_PHADDWrr64_MMX_PHSUBSWrr64_MMX_PHSUBWrr64") 3, 0, 0, 709, 11,  6, 1,  0, 0}, // #818
  {DBGFIELD("MMX_PHADDrr64_MMX_PHSUBDrr64") 3, 0, 0, 709, 11,  6, 1,  0, 0}, // #819
  {DBGFIELD("PHADDDrr_PHSUBDrr_VPHADDDrr_VPHSUBDrr") 3, 0, 0, 709, 11,  6, 1,  0, 0}, // #820
  {DBGFIELD("PHADDSWrr128_PHSUBSWrr128_VPHADDSWrr128_VPHADDSWrr256_VPHSUBSWrr128_VPHSUBSWrr256") 3, 0, 0, 709, 11,  6, 1,  0, 0}, // #821
  {DBGFIELD("PHADDWrr_PHSUBWrr_VPHADDDYrr_VPHADDWYrr_VPHADDWrr_VPHSUBDYrr_VPHSUBWYrr_VPHSUBWrr") 3, 0, 0, 709, 11,  6, 1,  0, 0}, // #822
  {DBGFIELD("MMX_PHADDSWrm64_MMX_PHADDWrm64_MMX_PHSUBSWrm64_MMX_PHSUBWrm64") 3, 0, 0, 720, 13, 16, 1,  1, 1}, // #823
  {DBGFIELD("MMX_PHADDrm64_MMX_PHSUBDrm64") 3, 0, 0, 720, 13, 16, 1,  1, 1}, // #824
  {DBGFIELD("PHADDDrm_PHSUBDrm_VPHADDDrm_VPHSUBDrm") 3, 0, 0, 720, 13, 16, 1,  1, 1}, // #825
  {DBGFIELD("PHADDSWrm128_PHSUBSWrm128_VPHADDSWrm128_VPHADDSWrm256_VPHSUBSWrm128_VPHSUBSWrm256") 3, 0, 0, 720, 13, 16, 1,  1, 1}, // #826
  {DBGFIELD("PHADDWrm_PHSUBWrm_VPHADDDYrm_VPHADDWYrm_VPHADDWrm_VPHSUBDYrm_VPHSUBWYrm_VPHSUBWrm") 3, 0, 0, 720, 13, 16, 1,  1, 1}, // #827
  {DBGFIELD("PCMPGTQrr_VPCMPGTQYrr_VPCMPGTQrr") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #828
  {DBGFIELD("PCMPGTQrm_VPCMPGTQYrm_VPCMPGTQrm") 2, 0, 0, 105, 11,  3, 1,  1, 1}, // #829
  {DBGFIELD("PMULLDrr_VPMULLDYrr_VPMULLDrr") 2, 0, 0, 733, 9,  8, 1,  0, 0}, // #830
  {DBGFIELD("PMULLDrm_VPMULLDYrm_VPMULLDrm") 3, 0, 0, 742, 11,  8, 1,  1, 1}, // #831
  {DBGFIELD("PTESTrr_VPTESTYrr_VPTESTrr") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #832
  {DBGFIELD("PTESTrm_VPTESTYrm_VPTESTrm") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #833
  {DBGFIELD("PSLLDrr_PSLLQrr_PSLLWrr_PSRADrr_PSRAWrr_PSRLDrr_PSRLQrr_PSRLWrr_VPSLLDYrr_VPSLLDrr_VPSLLQYrr_VPSLLQrr_VPSLLWYrr_VPSLLWrr_VPSRADYrr_VPSRADrr_VPSRAWYrr_VPSRAWrr_VPSRLDYrr_VPSRLDrr_VPSRLQYrr_VPSRLQrr_VPSRLWYrr_VPSRLWrr") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #834
  {DBGFIELD("PSLLDQri_PSRLDQri")  1, 0, 0, 39, 8,  2, 1,  0, 0}, // #835
  {DBGFIELD("VPSLLDQYri_VPSLLDQri_VPSRLDQYri_VPSRLDQri") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #836
  {DBGFIELD("MMX_EMMS")           31, 0, 0,  0, 0, 34, 1,  0, 0}, // #837
  {DBGFIELD("MOVMSKPDrr_MOVMSKPSrr_VMOVMSKPDrr_VMOVMSKPSrr") 1, 0, 0, 117, 9,  6, 1,  0, 0}, // #838
  {DBGFIELD("VMOVMSKPDYrr_VMOVMSKPSYrr") 1, 0, 0, 117, 9, 19, 1,  0, 0}, // #839
  {DBGFIELD("VPERM2F128rr")       1, 0, 0, 39, 8,  6, 1,  0, 0}, // #840
  {DBGFIELD("VPERM2F128rm")       1, 0, 0, 29, 10,  5, 1,  1, 1}, // #841
  {DBGFIELD("BLENDVPDrr0_BLENDVPSrr0") 1, 0, 0, 57, 8, 19, 1,  0, 0}, // #842
  {DBGFIELD("BLENDVPDrm0_BLENDVPSrm0") 1, 0, 0, 47, 10, 16, 1,  1, 1}, // #843
  {DBGFIELD("VBROADCASTF128")     1, 0, 0, 101, 3,  9, 1,  0, 0}, // #844
  {DBGFIELD("EXTRACTPSrr")        2, 0, 0, 686, 12,  2, 1,  0, 0}, // #845
  {DBGFIELD("VEXTRACTPSrr")       2, 0, 0, 686, 12,  2, 1,  0, 0}, // #846
  {DBGFIELD("EXTRACTPSmr")        3, 0, 0, 753, 14,  9, 1,  0, 0}, // #847
  {DBGFIELD("VEXTRACTPSmr")       3, 0, 0, 753, 14,  9, 1,  0, 0}, // #848
  {DBGFIELD("VEXTRACTF128rr")     1, 0, 0, 39, 8,  6, 1,  0, 0}, // #849
  {DBGFIELD("VEXTRACTF128mr")     2, 0, 0, 141, 5,  9, 1,  0, 0}, // #850
  {DBGFIELD("VINSERTF128rr")      1, 0, 0, 39, 8,  6, 1,  0, 0}, // #851
  {DBGFIELD("VINSERTF128rm")      1, 0, 0, 39, 8,  6, 1,  1, 1}, // #852
  {DBGFIELD("VMASKMOVPDYrm_VMASKMOVPDrm_VMASKMOVPSYrm_VMASKMOVPSrm") 3, 0, 0, 47, 10,  9, 1,  0, 0}, // #853
  {DBGFIELD("VMASKMOVPDmr_VMASKMOVPSmr") 4, 0, 0, 671, 15, 34, 1,  0, 0}, // #854
  {DBGFIELD("VMASKMOVPDYmr_VMASKMOVPSYmr") 4, 0, 0, 671, 15,  7, 1,  0, 0}, // #855
  {DBGFIELD("VGATHERDPSrm")       20, 0, 0,  0, 0,  2, 1,  1, 1}, // #856
  {DBGFIELD("VGATHERDPSYrm")      34, 0, 0,  0, 0,  2, 1,  1, 1}, // #857
  {DBGFIELD("VGATHERQPSrm")       15, 0, 0,  0, 0,  2, 1,  1, 1}, // #858
  {DBGFIELD("VGATHERQPSYrm")      22, 0, 0,  0, 0,  2, 1,  1, 1}, // #859
  {DBGFIELD("VGATHERDPDrm")       12, 0, 0,  0, 0,  2, 1,  1, 1}, // #860
  {DBGFIELD("VGATHERDPDYrm")      20, 0, 0,  0, 0,  2, 1,  1, 1}, // #861
  {DBGFIELD("VGATHERQPDrm")       14, 0, 0,  0, 0,  2, 1,  1, 1}, // #862
  {DBGFIELD("VGATHERQPDYrm")      22, 0, 0,  0, 0,  2, 1,  1, 1}, // #863
  {DBGFIELD("CVTPD2PSrr_VCVTPD2PSrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #864
  {DBGFIELD("CVTPD2PSrm_VCVTPD2PSXrm") 3, 0, 0, 778, 13, 10, 1,  0, 0}, // #865
  {DBGFIELD("VCVTPD2PSYrr")       2, 0, 0, 767, 11,  3, 1,  0, 0}, // #866
  {DBGFIELD("VCVTPD2PSYrm")       3, 0, 0, 778, 13, 15, 1,  0, 0}, // #867
  {DBGFIELD("CVTSD2SSrr_Int_CVTSD2SSrr_Int_VCVTSD2SSrr_VCVTSD2SSrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #868
  {DBGFIELD("CVTSD2SSrm")         3, 0, 0, 778, 13, 10, 1,  0, 0}, // #869
  {DBGFIELD("Int_CVTSD2SSrm_Int_VCVTSD2SSrm_VCVTSD2SSrm") 3, 0, 0, 778, 13, 10, 1,  0, 0}, // #870
  {DBGFIELD("CVTPS2PDrr_VCVTPS2PDrr") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #871
  {DBGFIELD("CVTPS2PDrm_VCVTPS2PDYrm_VCVTPS2PDrm") 2, 0, 0, 105, 11,  3, 1,  0, 0}, // #872
  {DBGFIELD("VCVTPS2PDYrr")       2, 0, 0, 686, 12,  3, 1,  0, 0}, // #873
  {DBGFIELD("CVTSS2SDrr_Int_CVTSS2SDrr_Int_VCVTSS2SDrr_VCVTSS2SDrr") 2, 0, 0, 686, 12, 19, 1,  0, 0}, // #874
  {DBGFIELD("CVTSS2SDrm")         2, 0, 0, 105, 11,  3, 1,  0, 0}, // #875
  {DBGFIELD("Int_CVTSS2SDrm_Int_VCVTSS2SDrm_VCVTSS2SDrm") 2, 0, 0, 105, 11,  3, 1,  0, 0}, // #876
  {DBGFIELD("CVTDQ2PDrr")         2, 0, 0, 767, 11,  9, 1,  0, 0}, // #877
  {DBGFIELD("VCVTDQ2PDrr")        2, 0, 0, 767, 11,  9, 1,  0, 0}, // #878
  {DBGFIELD("VCVTDQ2PDYrr")       2, 0, 0, 767, 11, 16, 1,  0, 0}, // #879
  {DBGFIELD("CVTPD2DQrr_CVTTPD2DQrr_VCVTTPD2DQrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #880
  {DBGFIELD("VCVTPD2DQrr")        2, 0, 0, 767, 11,  9, 1,  0, 0}, // #881
  {DBGFIELD("CVTPD2DQrm_CVTTPD2DQrm") 3, 0, 0, 778, 13, 10, 1,  0, 0}, // #882
  {DBGFIELD("VCVTPD2DQYrr")       2, 0, 0, 767, 11, 16, 1,  0, 0}, // #883
  {DBGFIELD("VCVTTPD2DQYrr")      2, 0, 0, 767, 11, 16, 1,  0, 0}, // #884
  {DBGFIELD("VCVTPD2DQYrm")       3, 0, 0, 778, 13,  8, 1,  0, 0}, // #885
  {DBGFIELD("VCVTTPD2DQYrm")      3, 0, 0, 778, 13,  8, 1,  0, 0}, // #886
  {DBGFIELD("MMX_CVTPS2PIirr_MMX_CVTTPS2PIirr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #887
  {DBGFIELD("MMX_CVTPI2PDirr")    2, 0, 0, 767, 11,  9, 1,  0, 0}, // #888
  {DBGFIELD("MMX_CVTPD2PIirr_MMX_CVTTPD2PIirr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #889
  {DBGFIELD("CVTSI2SS64rr_CVTSI2SSrr_Int_CVTSI2SS64rr_Int_CVTSI2SSrr_Int_VCVTSI2SS64rr_Int_VCVTSI2SSrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #890
  {DBGFIELD("VCVTSI2SS64rr_VCVTSI2SSrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #891
  {DBGFIELD("CVTSS2SI64rr_CVTTSS2SI64rr_Int_CVTTSS2SI64rr_Int_VCVTTSS2SI64rr_VCVTSS2SI64rr_VCVTTSS2SI64rr") 2, 0, 0, 791, 12,  9, 1,  0, 0}, // #892
  {DBGFIELD("CVTSS2SIrr_CVTTSS2SIrr_Int_CVTTSS2SIrr_Int_VCVTTSS2SIrr_VCVTSS2SIrr_VCVTTSS2SIrr") 2, 0, 0, 791, 12,  9, 1,  0, 0}, // #893
  {DBGFIELD("CVTSS2SI64rm_CVTTSS2SI64rm_Int_CVTTSS2SI64rm_Int_VCVTTSS2SI64rm_VCVTSS2SI64rm_VCVTTSS2SI64rm") 3, 0, 0, 803, 14, 10, 1,  0, 0}, // #894
  {DBGFIELD("CVTSS2SIrm_CVTTSS2SIrm_Int_CVTTSS2SIrm_Int_VCVTTSS2SIrm_VCVTSS2SIrm_VCVTTSS2SIrm") 3, 0, 0, 803, 14, 10, 1,  0, 0}, // #895
  {DBGFIELD("CVTSD2SI64rr_CVTSD2SIrr_CVTTSD2SI64rr_CVTTSD2SIrr_Int_CVTTSD2SI64rr_Int_CVTTSD2SIrr_Int_VCVTTSD2SI64rr_Int_VCVTTSD2SIrr_VCVTSD2SI64rr_VCVTSD2SIrr_VCVTTSD2SI64rr_VCVTTSD2SIrr") 2, 0, 0, 791, 12,  9, 1,  0, 0}, // #896
  {DBGFIELD("CVTSD2SI64rm_CVTSD2SIrm_CVTTSD2SI64rm_CVTTSD2SIrm_Int_CVTTSD2SI64rm_Int_CVTTSD2SIrm_Int_VCVTTSD2SI64rm_Int_VCVTTSD2SIrm_VCVTSD2SI64rm_VCVTSD2SIrm_VCVTTSD2SI64rm_VCVTTSD2SIrm") 3, 0, 0, 803, 14, 10, 1,  0, 0}, // #897
  {DBGFIELD("VCVTPS2PHYrr_VCVTPS2PHrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #898
  {DBGFIELD("VCVTPS2PHYmr_VCVTPS2PHmr") 4, 0, 0, 817, 15, 35, 2,  0, 0}, // #899
  {DBGFIELD("VCVTPH2PSYrr_VCVTPH2PSrr") 2, 0, 0, 767, 11,  9, 1,  0, 0}, // #900
  {DBGFIELD("HADDPDrr_HADDPSrr_HSUBPDrr_HSUBPSrr_VHADDPDYrr_VHADDPDrr_VHADDPSYrr_VHADDPSrr_VHSUBPDYrr_VHSUBPDrr_VHSUBPSYrr_VHSUBPSrr") 3, 0, 0, 709, 11,  3, 1,  0, 0}, // #901
  {DBGFIELD("HADDPDrm_HADDPSrm_HSUBPDrm_HSUBPSrm_VHADDPDYrm_VHADDPDrm_VHADDPSYrm_VHADDPSrm_VHSUBPDYrm_VHSUBPDrm_VHSUBPSYrm_VHSUBPSrm") 4, 0, 0, 720, 13, 15, 1,  0, 0}, // #902
  {DBGFIELD("MULPDrr_VMULPDrr")   1, 0, 0, 554, 4,  3, 1,  0, 0}, // #903
  {DBGFIELD("MULPSrr_VMULPSrr")   1, 0, 0, 554, 4,  3, 1,  0, 0}, // #904
  {DBGFIELD("MULSDrr_MULSDrr_Int_VMULSDrr_VMULSDrr_Int") 1, 0, 0, 554, 4,  3, 1,  0, 0}, // #905
  {DBGFIELD("MULSSrr_MULSSrr_Int_VMULSSrr_VMULSSrr_Int") 1, 0, 0, 554, 4,  3, 1,  0, 0}, // #906
  {DBGFIELD("MULPDrm_MULPSrm_VMULPDrm_VMULPSrm") 2, 0, 0, 570, 6, 15, 1,  0, 0}, // #907
  {DBGFIELD("MULSDrm_MULSDrm_Int_MULSSrm_MULSSrm_Int_VMULSDrm_VMULSDrm_Int_VMULSSrm_VMULSSrm_Int") 2, 0, 0, 570, 6, 15, 1,  0, 0}, // #908
  {DBGFIELD("VDIVPSYrr")          3, 0, 0, 832, 10, 23, 1,  0, 0}, // #909
  {DBGFIELD("VDIVPSYrm")          4, 0, 0, 842, 12, 29, 1,  1, 1}, // #910
  {DBGFIELD("VDIVPDYrr")          3, 0, 0, 832, 10, 37, 1,  0, 0}, // #911
  {DBGFIELD("VDIVPDYrm")          4, 0, 0, 842, 12, 38, 1,  1, 1}, // #912
  {DBGFIELD("VRCPPSYr")           3, 0, 0, 832, 10,  5, 1,  0, 0}, // #913
  {DBGFIELD("VRCPPSYm")           4, 0, 0, 842, 12, 20, 1,  0, 0}, // #914
  {DBGFIELD("ROUNDPDr_ROUNDPSr_VROUNDPDr_VROUNDPSr_VROUNDYPDr_VROUNDYPSr") 2, 0, 0, 616, 7, 16, 1,  0, 0}, // #915
  {DBGFIELD("ROUNDSDr_ROUNDSDr_Int_ROUNDSSr_ROUNDSSr_Int_VROUNDSDr_VROUNDSDr_Int_VROUNDSSr_VROUNDSSr_Int") 2, 0, 0, 616, 7, 16, 1,  0, 0}, // #916
  {DBGFIELD("ROUNDPDm_VROUNDPDm_VROUNDYPDm") 3, 0, 0, 607, 9,  8, 1,  0, 0}, // #917
  {DBGFIELD("ROUNDPSm_VROUNDPSm_VROUNDYPSm") 3, 0, 0, 607, 9,  8, 1,  0, 0}, // #918
  {DBGFIELD("ROUNDSDm_ROUNDSSm_VROUNDSDm_VROUNDSSm") 3, 0, 0, 607, 9,  8, 1,  0, 0}, // #919
  {DBGFIELD("DPPSrri_VDPPSYrri_VDPPSrri") 4, 0, 0, 854, 14,  7, 1,  0, 0}, // #920
  {DBGFIELD("DPPSrmi_VDPPSYrmi_VDPPSrmi") 6, 0, 0, 868, 17, 39, 1,  1, 1}, // #921
  {DBGFIELD("DPPDrri")            3, 0, 0, 885, 14, 15, 1,  0, 0}, // #922
  {DBGFIELD("VDPPDrri")           3, 0, 0, 885, 14, 15, 1,  0, 0}, // #923
  {DBGFIELD("DPPDrmi_VDPPDrmi")   4, 0, 0, 899, 16, 34, 1,  0, 0}, // #924
  {DBGFIELD("VFMADDPD4rr_VFMADDPD4rrY_VFMADDPD4rrY_REV_VFMADDPD4rr_REV_VFMADDPDr132r_VFMADDPDr132rY_VFMADDPDr213r_VFMADDPDr213rY_VFMADDPDr231r_VFMADDPDr231rY_VFMADDPS4rr_VFMADDPS4rrY_VFMADDPS4rrY_REV_VFMADDPS4rr_REV_VFMADDPSr132r_VFMADDPSr132rY_VFMADDPSr213r_VFMADDPSr213rY_VFMADDPSr231r_VFMADDPSr231rY_VFMADDSD4rr_VFMADDSD4rr_Int_VFMADDSD4rr_REV_VFMADDSDr132r_VFMADDSDr132r_Int_VFMADDSDr213r_VFMADDSDr213r_Int_VFMADDSDr231r_VFMADDSDr231r_Int_VFMADDSS4rr_VFMADDSS4rr_Int_VFMADDSS4rr_REV_VFMADDSSr132r_VFMADDSSr132r_Int_VFMADDSSr213r_VFMADDSSr213r_Int_VFMADDSSr231r_VFMADDSSr231r_Int_VFMADDSUBPDr132r_VFMADDSUBPDr132rY_VFMADDSUBPDr213r_VFMADDSUBPDr213rY_VFMADDSUBPDr231r_VFMADDSUBPDr231rY_VFMADDSUBPSr132r_VFMADDSUBPSr132rY_VFMADDSUBPSr213r_VFMADDSUBPSr213rY_VFMADDSUBPSr231r_VFMADDSUBPSr231rY_VFMSUBADDPDr132r_VFMSUBADDPDr132rY_VFMSUBADDPDr213r_VFMSUBADDPDr213rY_VFMSUBADDPDr231r_VFMSUBADDPDr231rY_VFMSUBADDPSr132r_VFMSUBADDPSr132rY_VFMSUBADDPSr213r_VFMSUBADDPSr213rY_VFMSUBADDPSr231r_VFMSUBADDPSr231rY_VFMSUBPD4rr_VFMSUBPD4rrY_VFMSUBPD4rrY_REV_VFMSUBPD4rr_REV_VFMSUBPDr132r_VFMSUBPDr132rY_VFMSUBPDr213r_VFMSUBPDr213rY_VFMSUBPDr231r_VFMSUBPDr231rY_VFMSUBPS4rr_VFMSUBPS4rrY_VFMSUBPS4rrY_REV_VFMSUBPS4rr_REV_VFMSUBPSr132r_VFMSUBPSr132rY_VFMSUBPSr213r_VFMSUBPSr213rY_VFMSUBPSr231r_VFMSUBPSr231rY_VFMSUBSD4rr_VFMSUBSD4rr_Int_VFMSUBSD4rr_REV_VFMSUBSDr132r_VFMSUBSDr132r_Int_VFMSUBSDr213r_VFMSUBSDr213r_Int_VFMSUBSDr231r_VFMSUBSDr231r_Int_VFMSUBSS4rr_VFMSUBSS4rr_Int_VFMSUBSS4rr_REV_VFMSUBSSr132r_VFMSUBSSr132r_Int_VFMSUBSSr213r_VFMSUBSSr213r_Int_VFMSUBSSr231r_VFMSUBSSr231r_Int_VFNMADDPD4rr_VFNMADDPD4rrY_VFNMADDPD4rrY_REV_VFNMADDPD4rr_REV_VFNMADDPDr132r_VFNMADDPDr132rY_VFNMADDPDr213r_VFNMADDPDr213rY_VFNMADDPDr231r_VFNMADDPDr231rY_VFNMADDPS4rr_VFNMADDPS4rrY_VFNMADDPS4rrY_REV_VFNMADDPS4rr_REV_VFNMADDPSr132r_VFNMADDPSr132rY_VFNMADDPSr213r_VFNMADDPSr213rY_VFNMADDPSr231r_VFNMADDPSr231rY_VFNMADDSD4rr_VFNMADDSD4rr_Int_VFNMADDSD4rr_REV_VFNMADDSDr132r_VFNMADDSDr132r_Int_VFNMADDSDr213r_VFNMADDSDr213r_Int_VFNMADDSDr231r_VFNMADDSDr231r_Int_VFNMADDSS4rr_VFNMADDSS4rr_Int_VFNMADDSS4rr_REV_VFNMADDSSr132r_VFNMADDSSr132r_Int_VFNMADDSSr213r_VFNMADDSSr213r_Int_VFNMADDSSr231r_VFNMADDSSr231r_Int_VFNMSUBPD4rr_VFNMSUBPD4rrY_VFNMSUBPD4rrY_REV_VFNMSUBPD4rr_REV_VFNMSUBPDr132r_VFNMSUBPDr132rY_VFNMSUBPDr213r_VFNMSUBPDr213rY_VFNMSUBPDr231r_VFNMSUBPDr231rY_VFNMSUBPS4rr_VFNMSUBPS4rrY_VFNMSUBPS4rrY_REV_VFNMSUBPS4rr_REV_VFNMSUBPSr132r_VFNMSUBPSr132rY_VFNMSUBPSr213r_VFNMSUBPSr213rY_VFNMSUBPSr231r_VFNMSUBPSr231rY_VFNMSUBSD4rr_VFNMSUBSD4rr_Int_VFNMSUBSD4rr_REV_VFNMSUBSDr132r_VFNMSUBSDr132r_Int_VFNMSUBSDr213r_VFNMSUBSDr213r_Int_VFNMSUBSDr231r_VFNMSUBSDr231r_Int_VFNMSUBSS4rr_VFNMSUBSS4rr_Int_VFNMSUBSS4rr_REV_VFNMSUBSSr132r_VFNMSUBSSr132r_Int_VFNMSUBSSr213r_VFNMSUBSSr213r_Int_VFNMSUBSSr231r_VFNMSUBSSr231r_Int") 1, 0, 0, 554, 4,  3, 1,  0, 0}, // #925
  {DBGFIELD("VFMADDPD4mr_VFMADDPD4mrY_VFMADDPD4rm_VFMADDPD4rmY_VFMADDPDr132m_VFMADDPDr132mY_VFMADDPDr213m_VFMADDPDr213mY_VFMADDPDr231m_VFMADDPDr231mY_VFMADDPS4mr_VFMADDPS4mrY_VFMADDPS4rm_VFMADDPS4rmY_VFMADDPSr132m_VFMADDPSr132mY_VFMADDPSr213m_VFMADDPSr213mY_VFMADDPSr231m_VFMADDPSr231mY_VFMADDSD4mr_VFMADDSD4mr_Int_VFMADDSD4rm_VFMADDSD4rm_Int_VFMADDSDr132m_VFMADDSDr132m_Int_VFMADDSDr213m_VFMADDSDr213m_Int_VFMADDSDr231m_VFMADDSDr231m_Int_VFMADDSS4mr_VFMADDSS4mr_Int_VFMADDSS4rm_VFMADDSS4rm_Int_VFMADDSSr132m_VFMADDSSr132m_Int_VFMADDSSr213m_VFMADDSSr213m_Int_VFMADDSSr231m_VFMADDSSr231m_Int_VFMADDSUBPDr132m_VFMADDSUBPDr132mY_VFMADDSUBPDr213m_VFMADDSUBPDr213mY_VFMADDSUBPDr231m_VFMADDSUBPDr231mY_VFMADDSUBPSr132m_VFMADDSUBPSr132mY_VFMADDSUBPSr213m_VFMADDSUBPSr213mY_VFMADDSUBPSr231m_VFMADDSUBPSr231mY_VFMSUBADDPDr132m_VFMSUBADDPDr132mY_VFMSUBADDPDr213m_VFMSUBADDPDr213mY_VFMSUBADDPDr231m_VFMSUBADDPDr231mY_VFMSUBADDPSr132m_VFMSUBADDPSr132mY_VFMSUBADDPSr213m_VFMSUBADDPSr213mY_VFMSUBADDPSr231m_VFMSUBADDPSr231mY_VFMSUBPD4mr_VFMSUBPD4mrY_VFMSUBPD4rm_VFMSUBPD4rmY_VFMSUBPDr132m_VFMSUBPDr132mY_VFMSUBPDr213m_VFMSUBPDr213mY_VFMSUBPDr231m_VFMSUBPDr231mY_VFMSUBPS4mr_VFMSUBPS4mrY_VFMSUBPS4rm_VFMSUBPS4rmY_VFMSUBPSr132m_VFMSUBPSr132mY_VFMSUBPSr213m_VFMSUBPSr213mY_VFMSUBPSr231m_VFMSUBPSr231mY_VFMSUBSD4mr_VFMSUBSD4mr_Int_VFMSUBSD4rm_VFMSUBSD4rm_Int_VFMSUBSDr132m_VFMSUBSDr132m_Int_VFMSUBSDr213m_VFMSUBSDr213m_Int_VFMSUBSDr231m_VFMSUBSDr231m_Int_VFMSUBSS4mr_VFMSUBSS4mr_Int_VFMSUBSS4rm_VFMSUBSS4rm_Int_VFMSUBSSr132m_VFMSUBSSr132m_Int_VFMSUBSSr213m_VFMSUBSSr213m_Int_VFMSUBSSr231m_VFMSUBSSr231m_Int_VFNMADDPD4mr_VFNMADDPD4mrY_VFNMADDPD4rm_VFNMADDPD4rmY_VFNMADDPDr132m_VFNMADDPDr132mY_VFNMADDPDr213m_VFNMADDPDr213mY_VFNMADDPDr231m_VFNMADDPDr231mY_VFNMADDPS4mr_VFNMADDPS4mrY_VFNMADDPS4rm_VFNMADDPS4rmY_VFNMADDPSr132m_VFNMADDPSr132mY_VFNMADDPSr213m_VFNMADDPSr213mY_VFNMADDPSr231m_VFNMADDPSr231mY_VFNMADDSD4mr_VFNMADDSD4mr_Int_VFNMADDSD4rm_VFNMADDSD4rm_Int_VFNMADDSDr132m_VFNMADDSDr132m_Int_VFNMADDSDr213m_VFNMADDSDr213m_Int_VFNMADDSDr231m_VFNMADDSDr231m_Int_VFNMADDSS4mr_VFNMADDSS4mr_Int_VFNMADDSS4rm_VFNMADDSS4rm_Int_VFNMADDSSr132m_VFNMADDSSr132m_Int_VFNMADDSSr213m_VFNMADDSSr213m_Int_VFNMADDSSr231m_VFNMADDSSr231m_Int_VFNMSUBPD4mr_VFNMSUBPD4mrY_VFNMSUBPD4rm_VFNMSUBPD4rmY_VFNMSUBPDr132m_VFNMSUBPDr132mY_VFNMSUBPDr213m_VFNMSUBPDr213mY_VFNMSUBPDr231m_VFNMSUBPDr231mY_VFNMSUBPS4mr_VFNMSUBPS4mrY_VFNMSUBPS4rm_VFNMSUBPS4rmY_VFNMSUBPSr132m_VFNMSUBPSr132mY_VFNMSUBPSr213m_VFNMSUBPSr213mY_VFNMSUBPSr231m_VFNMSUBPSr231mY_VFNMSUBSD4mr_VFNMSUBSD4mr_Int_VFNMSUBSD4rm_VFNMSUBSD4rm_Int_VFNMSUBSDr132m_VFNMSUBSDr132m_Int_VFNMSUBSDr213m_VFNMSUBSDr213m_Int_VFNMSUBSDr231m_VFNMSUBSDr231m_Int_VFNMSUBSS4mr_VFNMSUBSS4mr_Int_VFNMSUBSS4rm_VFNMSUBSS4rm_Int_VFNMSUBSSr132m_VFNMSUBSSr132m_Int_VFNMSUBSSr213m_VFNMSUBSSr213m_Int_VFNMSUBSSr231m_VFNMSUBSSr231m_Int") 2, 0, 0, 570, 6, 15, 1,  0, 0}, // #926
  {DBGFIELD("VSQRTPSYr")          3, 0, 0, 832, 10, 23, 1,  0, 0}, // #927
  {DBGFIELD("VSQRTPSYm")          4, 0, 0, 842, 12, 29, 1,  0, 0}, // #928
  {DBGFIELD("VSQRTPDYr")          3, 0, 0, 832, 10, 40, 1,  0, 0}, // #929
  {DBGFIELD("VSQRTPDYm")          4, 0, 0, 842, 12, 30, 1,  0, 0}, // #930
  {DBGFIELD("RSQRTPSr_VRSQRTPSr") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #931
  {DBGFIELD("RSQRTSSr_VRSQRTSSr") 1, 0, 0, 117, 9,  3, 1,  0, 0}, // #932
  {DBGFIELD("RSQRTSSr_Int")       1, 0, 0, 117, 9,  3, 1,  0, 0}, // #933
  {DBGFIELD("VRSQRTSSr_Int")      1, 0, 0, 117, 9,  3, 1,  0, 0}, // #934
  {DBGFIELD("RSQRTPSm_VRSQRTPSm") 2, 0, 0, 105, 11, 15, 1,  0, 0}, // #935
  {DBGFIELD("RSQRTSSm_VRSQRTSSm") 2, 0, 0, 105, 11, 15, 1,  0, 0}, // #936
  {DBGFIELD("RSQRTSSm_Int_VRSQRTSSm_Int") 2, 0, 0, 105, 11, 15, 1,  0, 0}, // #937
  {DBGFIELD("VRSQRTPSYr")         3, 0, 0, 832, 10,  5, 1,  0, 0}, // #938
  {DBGFIELD("VRSQRTPSYm")         4, 0, 0, 842, 12, 20, 1,  0, 0}, // #939
  {DBGFIELD("ANDNPDrr_ANDNPSrr_ANDPDrr_ANDPSrr_ORPDrr_ORPSrr_VANDNPDYrr_VANDNPDrr_VANDNPSYrr_VANDNPSrr_VANDPDYrr_VANDPDrr_VANDPSYrr_VANDPSrr_VORPDYrr_VORPDrr_VORPSYrr_VORPSrr_VXORPDYrr_VXORPDrr_VXORPSYrr_VXORPSrr_XORPDrr_XORPSrr") 1, 0, 0, 39, 8,  2, 1,  0, 0}, // #940
  {DBGFIELD("ANDNPDrm_ANDNPSrm_ANDPDrm_ANDPSrm_ORPDrm_ORPSrm_VANDNPDYrm_VANDNPDrm_VANDNPSYrm_VANDNPSrm_VANDPDYrm_VANDPDrm_VANDPSYrm_VANDPSrm_VORPDYrm_VORPDrm_VORPSYrm_VORPSrm_VXORPDYrm_VXORPDrm_VXORPSYrm_VXORPSrm_XORPDrm_XORPSrm") 2, 0, 0, 29, 10,  3, 1,  1, 1}, // #941
  {DBGFIELD("VZEROUPPER")         4, 0, 0,  0, 0,  2, 1,  0, 0}, // #942
  {DBGFIELD("VZEROALL")           12, 0, 0,  0, 0,  2, 1,  0, 0}, // #943
  {DBGFIELD("LDMXCSR_VLDMXCSR")   3, 0, 0, 915, 14, 16, 1,  0, 0}, // #944
  {DBGFIELD("STMXCSR_VSTMXCSR")   4, 0, 0, 929, 14,  5, 1,  0, 0} // #945
}; // HaswellModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc BtVer2ModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #7
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 427, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 952, 5, 10, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 957, 3,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 960, 5, 10, 1,  4, 1}, // #26
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 965, 3,  6, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 968, 4,  5, 1,  0, 0}, // #28
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 16, 1, 19, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 968, 4,  5, 1,  0, 0}, // #30
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 16, 1, 19, 1,  0, 0}, // #31
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #32
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 965, 2,  2, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #34
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #35
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #37
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #39
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #55
  {DBGFIELD("WriteSystem")        1, 0, 0, 13, 1,  1, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #57
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 427, 2,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #64
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #66
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #67
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #72
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #73
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #75
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #76
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #78
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #79
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #98
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #99
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #100
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #102
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #103
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #105
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #107
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #109
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #110
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #111
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 990, 6, 44, 1,  4, 1}, // #113
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 990, 6, 23, 1,  0, 0}, // #114
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 990, 6, 23, 1,  0, 0}, // #115
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 990, 6, 44, 1,  0, 0}, // #116
  {DBGFIELD("WriteFDiv")          1, 0, 0, 990, 6, 23, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 990, 6, 44, 1,  4, 1}, // #118
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 990, 6, 23, 1,  0, 0}, // #119
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 990, 6, 23, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #121
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #122
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #123
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 996, 5, 45, 2,  0, 0}, // #126
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #127
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #130
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #133
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #158
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #161
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #162
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #164
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #166
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #168
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 980, 6, 43, 1,  5, 2}, // #170
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 986, 4, 12, 1,  0, 0}, // #171
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #173
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #175
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #178
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #180
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #182
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #183
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #184
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #185
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #188
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #193
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #198
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 948, 4,  9, 1,  0, 0}, // #199
  {DBGFIELD("IIC_IRET_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #200
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #201
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #202
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #203
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #204
  {DBGFIELD("WriteLoad")          1, 0, 0, 968, 3,  3, 1,  0, 0}, // #205
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 427, 2,  2, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #208
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #212
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #215
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #216
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #218
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 968, 3,  3, 1,  0, 0}, // #223
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #224
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 13, 1,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #235
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #236
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #237
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #241
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #244
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #245
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #246
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #247
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #251
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 972, 5, 10, 1,  0, 0}, // #252
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 977, 3,  6, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #265
  {DBGFIELD("WriteMove")          1, 0, 0, 13, 1,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #267
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #269
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #271
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #272
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #273
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #274
  {DBGFIELD("WriteShuffle")       1, 0, 0, 965, 2,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #276
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecLogic") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #278
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecIMul") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #281
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #283
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #285
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #287
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #289
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMul") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #291
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #295
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #297
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #299
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 13, 1,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #306
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #310
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #311
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #313
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #320
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #325
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #328
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #330
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #332
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #335
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #336
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #340
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #342
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #346
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #347
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1009, 5, 10, 1,  4, 1}, // #348
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 1014, 3,  6, 1,  0, 0}, // #349
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #350
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #351
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #352
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #353
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #354
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #358
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #361
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #363
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 1017, 5, 47, 2,  0, 0}, // #364
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 1022, 3, 49, 2,  0, 0}, // #365
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 972, 5,  5, 1,  0, 0}, // #366
  {DBGFIELD("WriteFMul")          1, 0, 0, 977, 3, 19, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #368
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #370
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #371
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #375
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #377
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #378
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #379
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #380
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #381
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #383
  {DBGFIELD("IIC_ALU_MEM_WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1025, 4,  5, 1,  4, 1}, // #384
  {DBGFIELD("IIC_ALU_NONMEM_WriteVarBlend") 1, 0, 0, 1029, 2, 19, 1,  0, 0}, // #385
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #386
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteBlend") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 968, 4,  5, 1,  4, 1}, // #388
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 16, 1, 19, 1,  0, 0}, // #389
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #390
  {DBGFIELD("WriteVecALU")        1, 0, 0, 965, 2,  2, 1,  0, 0}, // #391
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 1031, 4, 39, 1,  4, 1}, // #392
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 1035, 2, 34, 1,  0, 0}, // #393
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 1031, 4, 39, 1,  4, 1}, // #394
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 1035, 2, 34, 1,  0, 0}, // #395
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 1025, 4, 20, 1,  4, 1}, // #396
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 1029, 2, 16, 1,  0, 0}, // #397
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 1025, 4, 14, 1,  4, 1}, // #398
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 1029, 2,  5, 1,  0, 0}, // #399
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 996, 5, 45, 2,  0, 0}, // #400
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #401
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #402
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #404
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #405
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 952, 5,  5, 1,  0, 0}, // #406
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 957, 3, 19, 1,  0, 0}, // #407
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #408
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #409
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #410
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #411
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #414
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #415
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #418
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #419
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 952, 5, 10, 1,  0, 0}, // #421
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #422
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #423
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #424
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #425
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #427
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #429
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #431
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #435
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #437
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #438
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_PUSH_MEM_WriteRMW") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #442
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #443
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #444
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 427, 2,  2, 1,  0, 0}, // #447
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 972, 5,  5, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #450
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #451
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #452
  {DBGFIELD("WriteRMW")           1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #455
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #456
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #458
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #459
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 948, 4,  9, 1,  0, 0}, // #460
  {DBGFIELD("WriteShift")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 952, 5, 10, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #463
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 952, 5, 10, 1,  0, 0}, // #464
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #465
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SSE_RSQRTPS_RM_WriteFRsqrtLd") 1, 0, 0, 972, 5,  5, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_RSQRTPS_RR_WriteFRsqrt") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SSE_RSQRTSS_RM_WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #469
  {DBGFIELD("WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #470
  {DBGFIELD("IIC_SSE_RSQRTSS_RR_WriteFRsqrt") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #471
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 948, 4,  9, 1,  5, 1}, // #472
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 1037, 4, 21, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #475
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #482
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #483
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #486
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #487
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #490
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #494
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 1041, 6, 51, 1,  0, 0}, // #495
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 1041, 6, 51, 1,  0, 0}, // #497
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1041, 6, 51, 1,  4, 1}, // #499
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1041, 6, 51, 1,  4, 1}, // #500
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1041, 6, 51, 1,  4, 1}, // #502
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #503
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #504
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #505
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 427, 2,  2, 1,  0, 0}, // #507
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #509
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #510
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #511
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 13, 1,  2, 1,  0, 0}, // #512
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #513
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #514
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #515
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #516
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #517
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #518
  {DBGFIELD("WriteJump")          1, 0, 0, 427, 2,  2, 1,  0, 0}, // #519
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #520
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #522
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_SSE_ALU_F64S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #524
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #525
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 1025, 4,  5, 1,  4, 1}, // #526
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 1029, 2, 19, 1,  0, 0}, // #527
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 960, 4, 16, 1,  0, 0}, // #528
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 965, 2,  2, 1,  0, 0}, // #529
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 965, 2,  2, 1,  0, 0}, // #530
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #531
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 972, 5, 10, 1,  0, 0}, // #532
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 977, 3,  6, 1,  0, 0}, // #533
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 972, 5, 10, 1,  0, 0}, // #534
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 977, 3,  6, 1,  0, 0}, // #535
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 1047, 6, 35, 2,  0, 0}, // #536
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #537
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #538
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #539
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #540
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #541
  {DBGFIELD("WriteStore")         1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #542
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 965, 2,  2, 1,  0, 0}, // #543
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 996, 5, 45, 2,  0, 0}, // #544
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #545
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #546
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #547
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #548
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #549
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #550
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1009, 5, 10, 1,  4, 1}, // #556
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 1014, 3,  6, 1,  0, 0}, // #557
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 960, 4, 16, 1,  0, 0}, // #558
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #559
  {DBGFIELD("WriteBlend")         1, 0, 0, 965, 2,  2, 1,  0, 0}, // #560
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1025, 4,  5, 1,  4, 1}, // #561
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 1029, 2, 19, 1,  0, 0}, // #562
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #563
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #564
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 968, 4,  5, 1,  4, 1}, // #565
  {DBGFIELD("WriteCLMul")         1, 0, 0, 16, 1, 19, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #568
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #569
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #570
  {DBGFIELD("WriteVarVecShift_ReadAfterLd") 1, 0, 0, 965, 2,  2, 1,  4, 1}, // #571
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 965, 2,  2, 1,  0, 0}, // #572
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 960, 4, 16, 1,  0, 0}, // #573
  {DBGFIELD("WriteVecShift")      1, 0, 0, 965, 2,  2, 1,  0, 0}, // #574
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #575
  {DBGFIELD("WriteFRcpLd")        1, 0, 0, 972, 5,  5, 1,  0, 0}, // #576
  {DBGFIELD("WriteFRsqrtLd")      1, 0, 0, 972, 5,  5, 1,  0, 0}, // #577
  {DBGFIELD("WriteFSqrtLd")       1, 0, 0, 1041, 6, 51, 1,  0, 0}, // #578
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #579
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #580
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #581
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #582
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #583
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #584
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #585
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #586
  {DBGFIELD("MOV16rm")            1, 0, 0, 968, 3,  3, 1,  0, 0}, // #587
  {DBGFIELD("MOVSX32rm16_MOVSX32rm8") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #588
  {DBGFIELD("MOVZX32rm16_MOVZX32rm8") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #589
  {DBGFIELD("CMOVA16rr_CMOVAE16rr_CMOVB16rr_CMOVBE16rr_CMOVE16rr_CMOVG16rr_CMOVGE16rr_CMOVL16rr_CMOVLE16rr_CMOVNE16rr_CMOVNO16rr_CMOVNP16rr_CMOVNS16rr_CMOVO16rr_CMOVP16rr_CMOVS16rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #590
  {DBGFIELD("CMOVA32rr_CMOVA64rr_CMOVAE32rr_CMOVAE64rr_CMOVB32rr_CMOVB64rr_CMOVBE32rr_CMOVBE64rr_CMOVE32rr_CMOVE64rr_CMOVG32rr_CMOVG64rr_CMOVGE32rr_CMOVGE64rr_CMOVL32rr_CMOVL64rr_CMOVLE32rr_CMOVLE64rr_CMOVNE32rr_CMOVNE64rr_CMOVNO32rr_CMOVNO64rr_CMOVNP32rr_CMOVNP64rr_CMOVNS32rr_CMOVNS64rr_CMOVO32rr_CMOVO64rr_CMOVP32rr_CMOVP64rr_CMOVS32rr_CMOVS64rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #591
  {DBGFIELD("CMOVA16rm_CMOVAE16rm_CMOVB16rm_CMOVBE16rm_CMOVE16rm_CMOVG16rm_CMOVGE16rm_CMOVL16rm_CMOVLE16rm_CMOVNE16rm_CMOVNO16rm_CMOVNP16rm_CMOVNS16rm_CMOVO16rm_CMOVP16rm_CMOVS16rm") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #592
  {DBGFIELD("CMOVA32rm_CMOVA64rm_CMOVAE32rm_CMOVAE64rm_CMOVB32rm_CMOVB64rm_CMOVBE32rm_CMOVBE64rm_CMOVE32rm_CMOVE64rm_CMOVG32rm_CMOVG64rm_CMOVGE32rm_CMOVGE64rm_CMOVL32rm_CMOVL64rm_CMOVLE32rm_CMOVLE64rm_CMOVNE32rm_CMOVNE64rm_CMOVNO32rm_CMOVNO64rm_CMOVNP32rm_CMOVNP64rm_CMOVNS32rm_CMOVNS64rm_CMOVO32rm_CMOVO64rm_CMOVP32rm_CMOVP64rm_CMOVS32rm_CMOVS64rm") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #593
  {DBGFIELD("XCHG16ar_XCHG16rr_XCHG32ar_XCHG32ar64_XCHG32rr_XCHG64ar_XCHG64rr_XCHG8rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #594
  {DBGFIELD("XCHG16rm_XCHG32rm_XCHG64rm_XCHG8rm") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #595
  {DBGFIELD("XLAT")               1, 0, 0, 968, 3,  3, 1,  0, 0}, // #596
  {DBGFIELD("PUSH16rmm_PUSH32rmm") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #597
  {DBGFIELD("PUSHF16_PUSHF32")    1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #598
  {DBGFIELD("PUSHA16_PUSHA32")    1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #599
  {DBGFIELD("POP16rmm_POP32rmm")  1, 0, 0, 968, 3,  3, 1,  0, 0}, // #600
  {DBGFIELD("POPF16")             1, 0, 0, 968, 3,  3, 1,  0, 0}, // #601
  {DBGFIELD("POPF32")             1, 0, 0, 968, 3,  3, 1,  0, 0}, // #602
  {DBGFIELD("POPA16_POPA32")      1, 0, 0, 968, 3,  3, 1,  0, 0}, // #603
  {DBGFIELD("LAHF_SAHF")          1, 0, 0, 427, 2,  2, 1,  0, 0}, // #604
  {DBGFIELD("BSWAP32r")           1, 0, 0, 427, 2,  2, 1,  0, 0}, // #605
  {DBGFIELD("BSWAP64r")           1, 0, 0, 427, 2,  2, 1,  0, 0}, // #606
  {DBGFIELD("MOVBE16rm_MOVBE64rm") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #607
  {DBGFIELD("MOVBE32rm")          1, 0, 0, 948, 4,  9, 1,  0, 0}, // #608
  {DBGFIELD("MOVBE16mr")          1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #609
  {DBGFIELD("MOVBE32mr")          1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #610
  {DBGFIELD("MOVBE64mr")          1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #611
  {DBGFIELD("ADD16mi_ADD16mi8_ADD16mr_ADD32mi_ADD32mi8_ADD32mr_ADD64mi32_ADD64mi8_ADD64mr_ADD8mi_ADD8mi8_ADD8mr_SUB16mi_SUB16mi8_SUB16mr_SUB32mi_SUB32mi8_SUB32mr_SUB64mi32_SUB64mi8_SUB64mr_SUB8mi_SUB8mi8_SUB8mr") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #612
  {DBGFIELD("ADC16ri_ADC16ri8_ADC16rr_ADC16rr_REV_ADC32ri_ADC32ri8_ADC32rr_ADC32rr_REV_ADC64ri32_ADC64ri8_ADC64rr_ADC64rr_REV_ADC8ri_ADC8ri8_ADC8rr_ADC8rr_REV_SBB16ri_SBB16ri8_SBB16rr_SBB16rr_REV_SBB32ri_SBB32ri8_SBB32rr_SBB32rr_REV_SBB64ri32_SBB64ri8_SBB64rr_SBB64rr_REV_SBB8ri_SBB8ri8_SBB8rr_SBB8rr_REV") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #613
  {DBGFIELD("ADC16rm_ADC32rm_ADC64rm_ADC8rm_SBB16rm_SBB32rm_SBB64rm_SBB8rm") 1, 0, 0, 948, 4,  9, 1,  4, 1}, // #614
  {DBGFIELD("ADC16mi_ADC16mi8_ADC16mr_ADC32mi_ADC32mi8_ADC32mr_ADC64mi32_ADC64mi8_ADC64mr_ADC8mi_ADC8mi8_ADC8mr_SBB16mi_SBB16mi8_SBB16mr_SBB32mi_SBB32mi8_SBB32mr_SBB64mi32_SBB64mi8_SBB64mr_SBB8mi_SBB8mi8_SBB8mr") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #615
  {DBGFIELD("DEC16m_DEC32m_DEC64m_DEC8m_INC16m_INC32m_INC64m_INC8m_NEG16m_NEG32m_NEG64m_NEG8m_NOT16m_NOT32m_NOT64m_NOT8m") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #616
  {DBGFIELD("IMUL16r_IMUL16rr")   1, 0, 0, 986, 3,  6, 1,  0, 0}, // #617
  {DBGFIELD("IMUL16rm")           1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #618
  {DBGFIELD("IMUL16rmi_IMUL16rmi8") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #619
  {DBGFIELD("IMUL16rri_IMUL16rri8") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #620
  {DBGFIELD("MUL16r")             1, 0, 0, 986, 3,  6, 1,  0, 0}, // #621
  {DBGFIELD("IMUL16m")            1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #622
  {DBGFIELD("MUL16m")             1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #623
  {DBGFIELD("IMUL32r_IMUL32rr")   1, 0, 0, 986, 3,  6, 1,  0, 0}, // #624
  {DBGFIELD("IMUL32rm")           1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #625
  {DBGFIELD("IMUL32rmi_IMUL32rmi8") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #626
  {DBGFIELD("IMUL32rri_IMUL32rri8") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #627
  {DBGFIELD("MUL32r")             1, 0, 0, 986, 3,  6, 1,  0, 0}, // #628
  {DBGFIELD("IMUL32m")            1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #629
  {DBGFIELD("MUL32m")             1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #630
  {DBGFIELD("IMUL64r_IMUL64rr")   1, 0, 0, 986, 3,  6, 1,  0, 0}, // #631
  {DBGFIELD("IMUL64rm")           1, 0, 0, 1001, 5, 16, 1,  4, 1}, // #632
  {DBGFIELD("IMUL64rmi32_IMUL64rmi8") 1, 0, 0, 1001, 5, 16, 1,  0, 0}, // #633
  {DBGFIELD("IMUL64rri32_IMUL64rri8") 1, 0, 0, 986, 3,  6, 1,  0, 0}, // #634
  {DBGFIELD("MUL64r")             1, 0, 0, 986, 3,  6, 1,  0, 0}, // #635
  {DBGFIELD("IMUL64m")            1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #636
  {DBGFIELD("MUL64m")             1, 0, 0, 1001, 5, 16, 1,  5, 2}, // #637
  {DBGFIELD("MULX32rr")           2, 0, 0, 1022, 3, 49, 2,  0, 0}, // #638
  {DBGFIELD("MULX32rm")           2, 0, 0, 1017, 5, 47, 2,  0, 0}, // #639
  {DBGFIELD("MULX64rr")           2, 0, 0, 1022, 3, 49, 2,  0, 0}, // #640
  {DBGFIELD("MULX64rm")           2, 0, 0, 1017, 5, 47, 2,  0, 0}, // #641
  {DBGFIELD("DIV8r")              1, 0, 0, 986, 4, 12, 1,  0, 0}, // #642
  {DBGFIELD("DIV16r")             1, 0, 0, 986, 4, 12, 1,  0, 0}, // #643
  {DBGFIELD("DIV32r")             1, 0, 0, 986, 4, 12, 1,  0, 0}, // #644
  {DBGFIELD("DIV64r")             1, 0, 0, 986, 4, 12, 1,  0, 0}, // #645
  {DBGFIELD("IDIV8r")             1, 0, 0, 986, 4, 12, 1,  0, 0}, // #646
  {DBGFIELD("IDIV16r")            1, 0, 0, 986, 4, 12, 1,  0, 0}, // #647
  {DBGFIELD("IDIV32r")            1, 0, 0, 986, 4, 12, 1,  0, 0}, // #648
  {DBGFIELD("IDIV64r")            1, 0, 0, 986, 4, 12, 1,  0, 0}, // #649
  {DBGFIELD("AND16mi_AND16mi8_AND16mr_AND32mi_AND32mi8_AND32mr_AND64mi32_AND64mi8_AND64mr_AND8mi_AND8mi8_AND8mr_OR16mi_OR16mi8_OR16mr_OR32mi_OR32mi8_OR32mr_OR64mi32_OR64mi8_OR64mr_OR8mi_OR8mi8_OR8mr_XOR16mi_XOR16mi8_XOR16mr_XOR32mi_XOR32mi8_XOR32mr_XOR64mi32_XOR64mi8_XOR64mr_XOR8mi_XOR8mi8_XOR8mr") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #650
  {DBGFIELD("OR32mrLocked")       2, 0, 0, 943, 5, 41, 2,  0, 0}, // #651
  {DBGFIELD("SAR16m1_SAR16mi_SAR32m1_SAR32mi_SAR64m1_SAR64mi_SAR8m1_SAR8mi_SHL16m1_SHL16mi_SHL32m1_SHL32mi_SHL64m1_SHL64mi_SHL8m1_SHL8mi_SHR16m1_SHR16mi_SHR32m1_SHR32mi_SHR64m1_SHR64mi_SHR8m1_SHR8mi") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #652
  {DBGFIELD("SAR16rCL_SAR32rCL_SAR64rCL_SAR8rCL_SHL16rCL_SHL32rCL_SHL64rCL_SHL8rCL_SHR16rCL_SHR32rCL_SHR64rCL_SHR8rCL") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #653
  {DBGFIELD("SAR16mCL_SAR32mCL_SAR64mCL_SAR8mCL_SHL16mCL_SHL32mCL_SHL64mCL_SHL8mCL_SHR16mCL_SHR32mCL_SHR64mCL_SHR8mCL") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #654
  {DBGFIELD("ROL16r1_ROL32r1_ROL64r1_ROL8r1_ROR16r1_ROR32r1_ROR64r1_ROR8r1") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #655
  {DBGFIELD("ROL16mi_ROL32mi_ROL64mi_ROL8mi_ROR16mi_ROR32mi_ROR64mi_ROR8mi") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #656
  {DBGFIELD("ROL16rCL_ROL32rCL_ROL64rCL_ROL8rCL_ROR16rCL_ROR32rCL_ROR64rCL_ROR8rCL") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #657
  {DBGFIELD("ROL16mCL_ROL32mCL_ROL64mCL_ROL8mCL_ROR16mCL_ROR32mCL_ROR64mCL_ROR8mCL") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #658
  {DBGFIELD("RCL16r1_RCL32r1_RCL64r1_RCL8r1_RCR16r1_RCR32r1_RCR64r1_RCR8r1") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #659
  {DBGFIELD("RCL16m1_RCL32m1_RCL64m1_RCL8m1_RCR16m1_RCR32m1_RCR64m1_RCR8m1") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #660
  {DBGFIELD("RCL16rCL_RCL16ri_RCL32rCL_RCL32ri_RCL64rCL_RCL64ri_RCL8rCL_RCL8ri_RCR16rCL_RCR16ri_RCR32rCL_RCR32ri_RCR64rCL_RCR64ri_RCR8rCL_RCR8ri") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #661
  {DBGFIELD("RCL16mCL_RCL16mi_RCL32mCL_RCL32mi_RCL64mCL_RCL64mi_RCL8mCL_RCL8mi_RCR16mCL_RCR16mi_RCR32mCL_RCR32mi_RCR64mCL_RCR64mi_RCR8mCL_RCR8mi") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #662
  {DBGFIELD("SHLD16rri8_SHRD16rri8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #663
  {DBGFIELD("SHLD32rri8_SHRD32rri8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #664
  {DBGFIELD("SHLD64rri8_SHRD64rri8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #665
  {DBGFIELD("SHLD16mri8_SHRD16mri8") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #666
  {DBGFIELD("SHLD32mri8_SHRD32mri8") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #667
  {DBGFIELD("SHLD64mri8_SHRD64mri8") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #668
  {DBGFIELD("SHLD16rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #669
  {DBGFIELD("SHLD32rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #670
  {DBGFIELD("SHLD64rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #671
  {DBGFIELD("SHRD16rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #672
  {DBGFIELD("SHRD32rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #673
  {DBGFIELD("SHRD64rrCL")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #674
  {DBGFIELD("SHLD16mrCL_SHRD16mrCL") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #675
  {DBGFIELD("SHLD32mrCL_SHRD32mrCL") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #676
  {DBGFIELD("SHLD64mrCL_SHRD64mrCL") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #677
  {DBGFIELD("BT16ri8_BT32ri8_BT64ri8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #678
  {DBGFIELD("BT16rr_BT32rr_BT64rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #679
  {DBGFIELD("BT16mr_BT32mr_BT64mr") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #680
  {DBGFIELD("BT16mi8_BT32mi8_BT64mi8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #681
  {DBGFIELD("BTC16ri8_BTC32ri8_BTC64ri8_BTR16ri8_BTR32ri8_BTR64ri8_BTS16ri8_BTS32ri8_BTS64ri8") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #682
  {DBGFIELD("BTC16rr_BTC32rr_BTC64rr_BTR16rr_BTR32rr_BTS16rr_BTS32rr_BTS64rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #683
  {DBGFIELD("BTR64rr")            1, 0, 0, 427, 2,  2, 1,  0, 0}, // #684
  {DBGFIELD("BTC16mr_BTC32mr_BTC64mr_BTR16mr_BTR32mr_BTR64mr_BTS16mr_BTS32mr_BTS64mr") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #685
  {DBGFIELD("BTC16mi8_BTC32mi8_BTC64mi8_BTR16mi8_BTR32mi8_BTR64mi8_BTS16mi8_BTS32mi8_BTS64mi8") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #686
  {DBGFIELD("BSF16rr_BSF32rr_BSF64rr_BSR16rr_BSR32rr_BSR64rr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #687
  {DBGFIELD("BSF16rm_BSF32rm_BSF64rm_BSR16rm_BSR32rm_BSR64rm") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #688
  {DBGFIELD("SETAEr_SETAr_SETBEr_SETBr_SETEr_SETGEr_SETGr_SETLEr_SETLr_SETNEr_SETNOr_SETNPr_SETNSr_SETOr_SETPr_SETSr") 1, 0, 0, 427, 2,  2, 1,  0, 0}, // #689
  {DBGFIELD("SETAEm_SETAm_SETBEm_SETBm_SETEm_SETGEm_SETGm_SETLEm_SETLm_SETNEm_SETNOm_SETNPm_SETNSm_SETOm_SETPm_SETSm") 2, 0, 0, 1037, 4, 21, 2,  0, 0}, // #690
  {DBGFIELD("CLD")                1, 0, 0, 427, 2,  2, 1,  0, 0}, // #691
  {DBGFIELD("STD")                1, 0, 0, 427, 2,  2, 1,  0, 0}, // #692
  {DBGFIELD("TZCNT16rr_TZCNT32rr_TZCNT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #693
  {DBGFIELD("TZCNT16rm_TZCNT32rm_TZCNT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #694
  {DBGFIELD("ANDN32rr_ANDN64rr")  1, 0, 0, 427, 2,  2, 1,  0, 0}, // #695
  {DBGFIELD("ANDN32rm_ANDN64rm")  1, 0, 0, 948, 4,  9, 1,  4, 1}, // #696
  {DBGFIELD("BLSI32rr_BLSI64rr_BLSMSK32rr_BLSMSK64rr_BLSR32rr_BLSR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #697
  {DBGFIELD("BLSI32rm_BLSI64rm_BLSMSK32rm_BLSMSK64rm_BLSR32rm_BLSR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #698
  {DBGFIELD("BEXTR32rr_BEXTR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #699
  {DBGFIELD("BEXTR32rm_BEXTR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #700
  {DBGFIELD("BZHI32rr_BZHI64rr")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #701
  {DBGFIELD("BZHI32rm_BZHI64rm")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #702
  {DBGFIELD("PDEP32rr_PDEP64rr_PEXT32rr_PEXT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #703
  {DBGFIELD("PDEP32rm_PDEP64rm_PEXT32rm_PEXT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #704
  {DBGFIELD("JCXZ_JRCXZ")         1, 0, 0, 427, 2,  2, 1,  0, 0}, // #705
  {DBGFIELD("LOOP")               1, 0, 0, 427, 2,  2, 1,  0, 0}, // #706
  {DBGFIELD("LOOPE")              1, 0, 0, 427, 2,  2, 1,  0, 0}, // #707
  {DBGFIELD("LOOPNE")             1, 0, 0, 427, 2,  2, 1,  0, 0}, // #708
  {DBGFIELD("CALL16r_CALL32r")    1, 0, 0, 427, 2,  2, 1,  0, 0}, // #709
  {DBGFIELD("CALL16m_CALL32m")    1, 0, 0, 948, 4,  9, 1,  0, 0}, // #710
  {DBGFIELD("LRETL_LRETQ_LRETW_RETL_RETQ_RETW") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #711
  {DBGFIELD("LRETIL_LRETIQ_LRETIW") 1, 0, 0, 948, 4,  9, 1,  0, 0}, // #712
  {DBGFIELD("RETIL_RETIQ_RETIW")  1, 0, 0, 948, 4,  9, 1,  0, 0}, // #713
  {DBGFIELD("BOUNDS16rm_BOUNDS32rm") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #714
  {DBGFIELD("INTO")               1, 0, 0, 13, 1,  1, 1,  0, 0}, // #715
  {DBGFIELD("LODSB_LODSW")        1, 0, 0, 13, 1,  1, 1,  0, 0}, // #716
  {DBGFIELD("LODSL_LODSQ")        1, 0, 0, 13, 1,  1, 1,  0, 0}, // #717
  {DBGFIELD("STOSB_STOSL_STOSQ_STOSW") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #718
  {DBGFIELD("MOVSB_MOVSL_MOVSQ_MOVSW") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #719
  {DBGFIELD("MOVSLDUPrm")         1, 0, 0, 968, 3,  3, 1,  0, 0}, // #720
  {DBGFIELD("MOVSLDUPrr")         1, 0, 0, 965, 2,  2, 1,  0, 0}, // #721
  {DBGFIELD("SCASB_SCASL_SCASQ_SCASW") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #722
  {DBGFIELD("CMPSB_CMPSL_CMPSQ_CMPSW") 1, 0, 0, 13, 1,  1, 1,  0, 0}, // #723
  {DBGFIELD("XADD16rm_XADD32rm_XADD64rm_XADD8rm") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #724
  {DBGFIELD("CMPXCHG16rm_CMPXCHG32rm_CMPXCHG64rm") 2, 0, 0, 943, 5, 41, 2,  0, 0}, // #725
  {DBGFIELD("CMPXCHG8rm")         2, 0, 0, 943, 5, 41, 2,  0, 0}, // #726
  {DBGFIELD("CMPXCHG8B")          2, 0, 0, 943, 5, 41, 2,  0, 0}, // #727
  {DBGFIELD("CMPXCHG16B")         2, 0, 0, 943, 5, 41, 2,  0, 0}, // #728
  {DBGFIELD("PAUSE")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #729
  {DBGFIELD("LEAVE_LEAVE64")      1, 0, 0, 427, 2,  2, 1,  0, 0}, // #730
  {DBGFIELD("XGETBV")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #731
  {DBGFIELD("RDTSC")              1, 0, 0, 13, 1,  1, 1,  0, 0}, // #732
  {DBGFIELD("RDTSCP")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #733
  {DBGFIELD("RDPMC")              1, 0, 0, 13, 1,  1, 1,  0, 0}, // #734
  {DBGFIELD("RDRAND16r_RDRAND32r_RDRAND64r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #735
  {DBGFIELD("LD_Frr")             1, 0, 0, 13, 1,  2, 1,  0, 0}, // #736
  {DBGFIELD("LD_F80m")            1, 0, 0, 968, 3,  3, 1,  0, 0}, // #737
  {DBGFIELD("FBLDm")              0, 0, 0,  0, 0,  0, 0,  0, 0}, // #738
  {DBGFIELD("ST_FPrr_ST_Frr")     1, 0, 0, 13, 1,  2, 1,  0, 0}, // #739
  {DBGFIELD("ST_FP80m")           1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #740
  {DBGFIELD("FBSTPm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #741
  {DBGFIELD("XCH_F")              1, 0, 0, 13, 1,  2, 1,  0, 0}, // #742
  {DBGFIELD("ILD_F16m_ILD_F32m_ILD_F64m") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #743
  {DBGFIELD("IST_F16m_IST_F32m_IST_FP16m_IST_FP32m") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #744
  {DBGFIELD("LD_F0")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #745
  {DBGFIELD("LD_F1")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #746
  {DBGFIELD("FLDPI")              1, 0, 0, 13, 1,  1, 1,  0, 0}, // #747
  {DBGFIELD("CMOVBE_F_CMOVBE_Fp32_CMOVBE_Fp64_CMOVBE_Fp80_CMOVB_F_CMOVB_Fp32_CMOVB_Fp64_CMOVB_Fp80_CMOVNBE_F_CMOVNBE_Fp32_CMOVNBE_Fp64_CMOVNBE_Fp80_CMOVNB_F_CMOVNB_Fp32_CMOVNB_Fp64_CMOVNB_Fp80_CMOVNE_F_CMOVNE_Fp32_CMOVNE_Fp64_CMOVNE_Fp80_CMOVNP_F_CMOVNP_Fp32_CMOVNP_Fp64_CMOVNP_Fp80_CMOVP_F_CMOVP_Fp32_CMOVP_Fp64_CMOVP_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #748
  {DBGFIELD("FNSTSW16r")          1, 0, 0, 427, 2,  2, 1,  0, 0}, // #749
  {DBGFIELD("FNSTSWm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #750
  {DBGFIELD("FLDCW16m")           1, 0, 0, 968, 3,  3, 1,  0, 0}, // #751
  {DBGFIELD("FNSTCW16m")          1, 0, 0, 427, 2,  2, 1,  0, 0}, // #752
  {DBGFIELD("FDECSTP_FINCSTP")    1, 0, 0, 13, 1,  1, 1,  0, 0}, // #753
  {DBGFIELD("FFREE")              1, 0, 0, 13, 1,  1, 1,  0, 0}, // #754
  {DBGFIELD("FSAVEm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #755
  {DBGFIELD("FRSTORm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #756
  {DBGFIELD("ABS_F_ABS_Fp32_ABS_Fp64_ABS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #757
  {DBGFIELD("CHS_F_CHS_Fp32_CHS_Fp64_CHS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #758
  {DBGFIELD("COMP_FST0r_COM_FST0r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #759
  {DBGFIELD("UCOM_FPr_UCOM_Fr")   1, 0, 0, 957, 3,  6, 1,  0, 0}, // #760
  {DBGFIELD("FCOM32m_FCOM64m_FCOMP32m_FCOMP64m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #761
  {DBGFIELD("FCOMPP")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #762
  {DBGFIELD("UCOM_FPPr")          1, 0, 0, 957, 3,  6, 1,  0, 0}, // #763
  {DBGFIELD("COM_FIPr_COM_FIr")   1, 0, 0, 957, 3,  6, 1,  0, 0}, // #764
  {DBGFIELD("UCOM_FIPr_UCOM_FIr") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #765
  {DBGFIELD("FICOM16m_FICOM32m_FICOMP16m_FICOMP32m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #766
  {DBGFIELD("TST_F_TST_Fp32_TST_Fp64_TST_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #767
  {DBGFIELD("FXAM")               1, 0, 0, 13, 1,  1, 1,  0, 0}, // #768
  {DBGFIELD("FPREM")              1, 0, 0, 13, 1,  1, 1,  0, 0}, // #769
  {DBGFIELD("FPREM1")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #770
  {DBGFIELD("FRNDINT")            1, 0, 0, 13, 1,  1, 1,  0, 0}, // #771
  {DBGFIELD("FSCALE")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #772
  {DBGFIELD("FXTRACT")            1, 0, 0, 13, 1,  1, 1,  0, 0}, // #773
  {DBGFIELD("FNOP")               1, 0, 0, 13, 1,  1, 1,  0, 0}, // #774
  {DBGFIELD("WAIT")               1, 0, 0, 13, 1,  1, 1,  0, 0}, // #775
  {DBGFIELD("FNCLEX")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #776
  {DBGFIELD("FNINIT")             1, 0, 0, 13, 1,  1, 1,  0, 0}, // #777
  {DBGFIELD("MMX_MOVD64from64rr_MMX_MOVD64grr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #778
  {DBGFIELD("MOVPDI2DIrr_VMOVPDI2DIrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #779
  {DBGFIELD("MMX_MOVD64rr_MMX_MOVD64to64rr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #780
  {DBGFIELD("MOVDI2PDIrr_VMOVDI2PDIrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #781
  {DBGFIELD("VMOVPQIto64rr")      1, 0, 0, 13, 1,  2, 1,  0, 0}, // #782
  {DBGFIELD("VMOV64toPQIrr")      1, 0, 0, 13, 1,  2, 1,  0, 0}, // #783
  {DBGFIELD("MMX_MOVQ64rr_MMX_MOVQ64rr_REV") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #784
  {DBGFIELD("MOVDQArr_MOVDQArr_REV_VMOVDQAYrr_VMOVDQAYrr_REV_VMOVDQArr_VMOVDQArr_REV") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #785
  {DBGFIELD("MOVDQUrr_MOVDQUrr_REV_VMOVDQUYrr_VMOVDQUYrr_REV_VMOVDQUrr_VMOVDQUrr_REV") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #786
  {DBGFIELD("MMX_MOVDQ2Qrr")      1, 0, 0, 13, 1,  2, 1,  0, 0}, // #787
  {DBGFIELD("MMX_MOVQ2DQrr")      1, 0, 0, 13, 1,  2, 1,  0, 0}, // #788
  {DBGFIELD("MMX_PACKSSDWirr_MMX_PACKSSWBirr_MMX_PACKUSWBirr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #789
  {DBGFIELD("MMX_PACKSSDWirm_MMX_PACKSSWBirm_MMX_PACKUSWBirm") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #790
  {DBGFIELD("VPMOVSXBQYrr_VPMOVSXBWYrr_VPMOVSXDQYrr_VPMOVZXBQYrr_VPMOVZXBWYrr_VPMOVZXDQYrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #791
  {DBGFIELD("PBLENDWrri")         1, 0, 0, 965, 2,  2, 1,  0, 0}, // #792
  {DBGFIELD("VPBLENDWYrri_VPBLENDWrri") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #793
  {DBGFIELD("PBLENDWrmi")         1, 0, 0, 960, 4, 16, 1,  4, 1}, // #794
  {DBGFIELD("VPBLENDWYrmi_VPBLENDWrmi") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #795
  {DBGFIELD("VPBLENDDYrri_VPBLENDDrri") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #796
  {DBGFIELD("VPBLENDDYrmi_VPBLENDDrmi") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #797
  {DBGFIELD("MMX_MASKMOVQ_MMX_MASKMOVQ64") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #798
  {DBGFIELD("MASKMOVDQU_MASKMOVDQU64_VMASKMOVDQU_VMASKMOVDQU64") 1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #799
  {DBGFIELD("VPMASKMOVDYrm_VPMASKMOVDrm_VPMASKMOVQYrm_VPMASKMOVQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #800
  {DBGFIELD("VPMASKMOVDYmr_VPMASKMOVDmr_VPMASKMOVQYmr_VPMASKMOVQmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #801
  {DBGFIELD("MMX_PMOVMSKBrr")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #802
  {DBGFIELD("PMOVMSKBrr_VPMOVMSKBrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #803
  {DBGFIELD("VPMOVMSKBYrr")       1, 0, 0, 965, 2,  2, 1,  0, 0}, // #804
  {DBGFIELD("MMX_PEXTRWirri")     1, 0, 0, 965, 2,  2, 1,  0, 0}, // #805
  {DBGFIELD("PEXTRBrr_PEXTRDrr_PEXTRQrr_PEXTRWrr_REV") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #806
  {DBGFIELD("PEXTRBmr_PEXTRDmr_PEXTRQmr_PEXTRWmr") 2, 0, 0, 996, 5, 45, 2,  0, 0}, // #807
  {DBGFIELD("VPBROADCASTBrm_VPBROADCASTWrm") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #808
  {DBGFIELD("VPBROADCASTBYrm_VPBROADCASTWYrm") 1, 0, 0, 968, 3,  3, 1,  0, 0}, // #809
  {DBGFIELD("VPGATHERDDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #810
  {DBGFIELD("VPGATHERDDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #811
  {DBGFIELD("VPGATHERQDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #812
  {DBGFIELD("VPGATHERQDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #813
  {DBGFIELD("VPGATHERDQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #814
  {DBGFIELD("VPGATHERDQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #815
  {DBGFIELD("VPGATHERQQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #816
  {DBGFIELD("VPGATHERQQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #817
  {DBGFIELD("MMX_PHADDSWrr64_MMX_PHADDWrr64_MMX_PHSUBSWrr64_MMX_PHSUBWrr64") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #818
  {DBGFIELD("MMX_PHADDrr64_MMX_PHSUBDrr64") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #819
  {DBGFIELD("PHADDDrr_PHSUBDrr_VPHADDDrr_VPHSUBDrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #820
  {DBGFIELD("PHADDSWrr128_PHSUBSWrr128_VPHADDSWrr128_VPHADDSWrr256_VPHSUBSWrr128_VPHSUBSWrr256") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #821
  {DBGFIELD("PHADDWrr_PHSUBWrr_VPHADDDYrr_VPHADDWYrr_VPHADDWrr_VPHSUBDYrr_VPHSUBWYrr_VPHSUBWrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #822
  {DBGFIELD("MMX_PHADDSWrm64_MMX_PHADDWrm64_MMX_PHSUBSWrm64_MMX_PHSUBWrm64") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #823
  {DBGFIELD("MMX_PHADDrm64_MMX_PHSUBDrm64") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #824
  {DBGFIELD("PHADDDrm_PHSUBDrm_VPHADDDrm_VPHSUBDrm") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #825
  {DBGFIELD("PHADDSWrm128_PHSUBSWrm128_VPHADDSWrm128_VPHADDSWrm256_VPHSUBSWrm128_VPHSUBSWrm256") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #826
  {DBGFIELD("PHADDWrm_PHSUBWrm_VPHADDDYrm_VPHADDWYrm_VPHADDWrm_VPHSUBDYrm_VPHSUBWYrm_VPHSUBWrm") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #827
  {DBGFIELD("PCMPGTQrr_VPCMPGTQYrr_VPCMPGTQrr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #828
  {DBGFIELD("PCMPGTQrm_VPCMPGTQYrm_VPCMPGTQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #829
  {DBGFIELD("PMULLDrr_VPMULLDYrr_VPMULLDrr") 1, 0, 0, 957, 3, 19, 1,  0, 0}, // #830
  {DBGFIELD("PMULLDrm_VPMULLDYrm_VPMULLDrm") 1, 0, 0, 952, 5,  5, 1,  4, 1}, // #831
  {DBGFIELD("PTESTrr_VPTESTYrr_VPTESTrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #832
  {DBGFIELD("PTESTrm_VPTESTYrm_VPTESTrm") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #833
  {DBGFIELD("PSLLDrr_PSLLQrr_PSLLWrr_PSRADrr_PSRAWrr_PSRLDrr_PSRLQrr_PSRLWrr_VPSLLDYrr_VPSLLDrr_VPSLLQYrr_VPSLLQrr_VPSLLWYrr_VPSLLWrr_VPSRADYrr_VPSRADrr_VPSRAWYrr_VPSRAWrr_VPSRLDYrr_VPSRLDrr_VPSRLQYrr_VPSRLQrr_VPSRLWYrr_VPSRLWrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #834
  {DBGFIELD("PSLLDQri_PSRLDQri")  1, 0, 0, 965, 2,  2, 1,  0, 0}, // #835
  {DBGFIELD("VPSLLDQYri_VPSLLDQri_VPSRLDQYri_VPSRLDQri") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #836
  {DBGFIELD("MMX_EMMS")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #837
  {DBGFIELD("MOVMSKPDrr_MOVMSKPSrr_VMOVMSKPDrr_VMOVMSKPSrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #838
  {DBGFIELD("VMOVMSKPDYrr_VMOVMSKPSYrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #839
  {DBGFIELD("VPERM2F128rr")       1, 0, 0, 965, 2,  2, 1,  0, 0}, // #840
  {DBGFIELD("VPERM2F128rm")       1, 0, 0, 960, 4, 16, 1,  4, 1}, // #841
  {DBGFIELD("BLENDVPDrr0_BLENDVPSrr0") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #842
  {DBGFIELD("BLENDVPDrm0_BLENDVPSrm0") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #843
  {DBGFIELD("VBROADCASTF128")     1, 0, 0, 960, 4, 16, 1,  0, 0}, // #844
  {DBGFIELD("EXTRACTPSrr")        1, 0, 0, 965, 2,  2, 1,  0, 0}, // #845
  {DBGFIELD("VEXTRACTPSrr")       1, 0, 0, 965, 2,  2, 1,  0, 0}, // #846
  {DBGFIELD("EXTRACTPSmr")        2, 0, 0, 996, 5, 45, 2,  0, 0}, // #847
  {DBGFIELD("VEXTRACTPSmr")       2, 0, 0, 996, 5, 45, 2,  0, 0}, // #848
  {DBGFIELD("VEXTRACTF128rr")     1, 0, 0, 965, 2,  2, 1,  0, 0}, // #849
  {DBGFIELD("VEXTRACTF128mr")     1, 0, 0, 1006, 3,  2, 1,  0, 0}, // #850
  {DBGFIELD("VINSERTF128rr")      1, 0, 0, 965, 2,  2, 1,  0, 0}, // #851
  {DBGFIELD("VINSERTF128rm")      1, 0, 0, 960, 4, 16, 1,  4, 1}, // #852
  {DBGFIELD("VMASKMOVPDYrm_VMASKMOVPDrm_VMASKMOVPSYrm_VMASKMOVPSrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #853
  {DBGFIELD("VMASKMOVPDmr_VMASKMOVPSmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #854
  {DBGFIELD("VMASKMOVPDYmr_VMASKMOVPSYmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #855
  {DBGFIELD("VGATHERDPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #856
  {DBGFIELD("VGATHERDPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #857
  {DBGFIELD("VGATHERQPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #858
  {DBGFIELD("VGATHERQPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #859
  {DBGFIELD("VGATHERDPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #860
  {DBGFIELD("VGATHERDPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #861
  {DBGFIELD("VGATHERQPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #862
  {DBGFIELD("VGATHERQPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #863
  {DBGFIELD("CVTPD2PSrr_VCVTPD2PSrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #864
  {DBGFIELD("CVTPD2PSrm_VCVTPD2PSXrm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #865
  {DBGFIELD("VCVTPD2PSYrr")       1, 0, 0, 977, 3,  6, 1,  0, 0}, // #866
  {DBGFIELD("VCVTPD2PSYrm")       1, 0, 0, 972, 5, 10, 1,  0, 0}, // #867
  {DBGFIELD("CVTSD2SSrr_Int_CVTSD2SSrr_Int_VCVTSD2SSrr_VCVTSD2SSrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #868
  {DBGFIELD("CVTSD2SSrm")         1, 0, 0, 972, 5, 10, 1,  0, 0}, // #869
  {DBGFIELD("Int_CVTSD2SSrm_Int_VCVTSD2SSrm_VCVTSD2SSrm") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #870
  {DBGFIELD("CVTPS2PDrr_VCVTPS2PDrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #871
  {DBGFIELD("CVTPS2PDrm_VCVTPS2PDYrm_VCVTPS2PDrm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #872
  {DBGFIELD("VCVTPS2PDYrr")       1, 0, 0, 977, 3,  6, 1,  0, 0}, // #873
  {DBGFIELD("CVTSS2SDrr_Int_CVTSS2SDrr_Int_VCVTSS2SDrr_VCVTSS2SDrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #874
  {DBGFIELD("CVTSS2SDrm")         1, 0, 0, 972, 5, 10, 1,  0, 0}, // #875
  {DBGFIELD("Int_CVTSS2SDrm_Int_VCVTSS2SDrm_VCVTSS2SDrm") 1, 0, 0, 972, 5, 10, 1,  4, 1}, // #876
  {DBGFIELD("CVTDQ2PDrr")         1, 0, 0, 977, 3,  6, 1,  0, 0}, // #877
  {DBGFIELD("VCVTDQ2PDrr")        1, 0, 0, 977, 3,  6, 1,  0, 0}, // #878
  {DBGFIELD("VCVTDQ2PDYrr")       1, 0, 0, 977, 3,  6, 1,  0, 0}, // #879
  {DBGFIELD("CVTPD2DQrr_CVTTPD2DQrr_VCVTTPD2DQrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #880
  {DBGFIELD("VCVTPD2DQrr")        1, 0, 0, 977, 3,  6, 1,  0, 0}, // #881
  {DBGFIELD("CVTPD2DQrm_CVTTPD2DQrm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #882
  {DBGFIELD("VCVTPD2DQYrr")       1, 0, 0, 977, 3,  6, 1,  0, 0}, // #883
  {DBGFIELD("VCVTTPD2DQYrr")      1, 0, 0, 977, 3,  6, 1,  0, 0}, // #884
  {DBGFIELD("VCVTPD2DQYrm")       1, 0, 0, 972, 5, 10, 1,  0, 0}, // #885
  {DBGFIELD("VCVTTPD2DQYrm")      1, 0, 0, 972, 5, 10, 1,  0, 0}, // #886
  {DBGFIELD("MMX_CVTPS2PIirr_MMX_CVTTPS2PIirr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #887
  {DBGFIELD("MMX_CVTPI2PDirr")    1, 0, 0, 977, 3,  6, 1,  0, 0}, // #888
  {DBGFIELD("MMX_CVTPD2PIirr_MMX_CVTTPD2PIirr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #889
  {DBGFIELD("CVTSI2SS64rr_CVTSI2SSrr_Int_CVTSI2SS64rr_Int_CVTSI2SSrr_Int_VCVTSI2SS64rr_Int_VCVTSI2SSrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #890
  {DBGFIELD("VCVTSI2SS64rr_VCVTSI2SSrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #891
  {DBGFIELD("CVTSS2SI64rr_CVTTSS2SI64rr_Int_CVTTSS2SI64rr_Int_VCVTTSS2SI64rr_VCVTSS2SI64rr_VCVTTSS2SI64rr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #892
  {DBGFIELD("CVTSS2SIrr_CVTTSS2SIrr_Int_CVTTSS2SIrr_Int_VCVTTSS2SIrr_VCVTSS2SIrr_VCVTTSS2SIrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #893
  {DBGFIELD("CVTSS2SI64rm_CVTTSS2SI64rm_Int_CVTTSS2SI64rm_Int_VCVTTSS2SI64rm_VCVTSS2SI64rm_VCVTTSS2SI64rm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #894
  {DBGFIELD("CVTSS2SIrm_CVTTSS2SIrm_Int_CVTTSS2SIrm_Int_VCVTTSS2SIrm_VCVTSS2SIrm_VCVTTSS2SIrm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #895
  {DBGFIELD("CVTSD2SI64rr_CVTSD2SIrr_CVTTSD2SI64rr_CVTTSD2SIrr_Int_CVTTSD2SI64rr_Int_CVTTSD2SIrr_Int_VCVTTSD2SI64rr_Int_VCVTTSD2SIrr_VCVTSD2SI64rr_VCVTSD2SIrr_VCVTTSD2SI64rr_VCVTTSD2SIrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #896
  {DBGFIELD("CVTSD2SI64rm_CVTSD2SIrm_CVTTSD2SI64rm_CVTTSD2SIrm_Int_CVTTSD2SI64rm_Int_CVTTSD2SIrm_Int_VCVTTSD2SI64rm_Int_VCVTTSD2SIrm_VCVTSD2SI64rm_VCVTSD2SIrm_VCVTTSD2SI64rm_VCVTTSD2SIrm") 1, 0, 0, 972, 5, 10, 1,  0, 0}, // #897
  {DBGFIELD("VCVTPS2PHYrr_VCVTPS2PHrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #898
  {DBGFIELD("VCVTPS2PHYmr_VCVTPS2PHmr") 2, 0, 0, 1047, 6, 35, 2,  0, 0}, // #899
  {DBGFIELD("VCVTPH2PSYrr_VCVTPH2PSrr") 1, 0, 0, 977, 3,  6, 1,  0, 0}, // #900
  {DBGFIELD("HADDPDrr_HADDPSrr_HSUBPDrr_HSUBPSrr_VHADDPDYrr_VHADDPDrr_VHADDPSYrr_VHADDPSrr_VHSUBPDYrr_VHSUBPDrr_VHSUBPSYrr_VHSUBPSrr") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #901
  {DBGFIELD("HADDPDrm_HADDPSrm_HSUBPDrm_HSUBPSrm_VHADDPDYrm_VHADDPDrm_VHADDPSYrm_VHADDPSrm_VHSUBPDYrm_VHSUBPDrm_VHSUBPSYrm_VHSUBPSrm") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #902
  {DBGFIELD("MULPDrr_VMULPDrr")   1, 0, 0, 977, 3, 19, 1,  0, 0}, // #903
  {DBGFIELD("MULPSrr_VMULPSrr")   1, 0, 0, 977, 3, 19, 1,  0, 0}, // #904
  {DBGFIELD("MULSDrr_MULSDrr_Int_VMULSDrr_VMULSDrr_Int") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #905
  {DBGFIELD("MULSSrr_MULSSrr_Int_VMULSSrr_VMULSSrr_Int") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #906
  {DBGFIELD("MULPDrm_MULPSrm_VMULPDrm_VMULPSrm") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #907
  {DBGFIELD("MULSDrm_MULSDrm_Int_MULSSrm_MULSSrm_Int_VMULSDrm_VMULSDrm_Int_VMULSSrm_VMULSSrm_Int") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #908
  {DBGFIELD("VDIVPSYrr")          1, 0, 0, 990, 6, 23, 1,  0, 0}, // #909
  {DBGFIELD("VDIVPSYrm")          1, 0, 0, 990, 6, 44, 1,  4, 1}, // #910
  {DBGFIELD("VDIVPDYrr")          1, 0, 0, 990, 6, 23, 1,  0, 0}, // #911
  {DBGFIELD("VDIVPDYrm")          1, 0, 0, 990, 6, 44, 1,  4, 1}, // #912
  {DBGFIELD("VRCPPSYr")           1, 0, 0, 977, 3, 19, 1,  0, 0}, // #913
  {DBGFIELD("VRCPPSYm")           1, 0, 0, 972, 5,  5, 1,  0, 0}, // #914
  {DBGFIELD("ROUNDPDr_ROUNDPSr_VROUNDPDr_VROUNDPSr_VROUNDYPDr_VROUNDYPSr") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #915
  {DBGFIELD("ROUNDSDr_ROUNDSDr_Int_ROUNDSSr_ROUNDSSr_Int_VROUNDSDr_VROUNDSDr_Int_VROUNDSSr_VROUNDSSr_Int") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #916
  {DBGFIELD("ROUNDPDm_VROUNDPDm_VROUNDYPDm") 1, 0, 0, 952, 5, 10, 1,  0, 0}, // #917
  {DBGFIELD("ROUNDPSm_VROUNDPSm_VROUNDYPSm") 1, 0, 0, 952, 5, 10, 1,  0, 0}, // #918
  {DBGFIELD("ROUNDSDm_ROUNDSSm_VROUNDSDm_VROUNDSSm") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #919
  {DBGFIELD("DPPSrri_VDPPSYrri_VDPPSrri") 1, 0, 0, 957, 3,  6, 1,  0, 0}, // #920
  {DBGFIELD("DPPSrmi_VDPPSYrmi_VDPPSrmi") 1, 0, 0, 952, 5, 10, 1,  4, 1}, // #921
  {DBGFIELD("DPPDrri")            1, 0, 0, 957, 3,  6, 1,  0, 0}, // #922
  {DBGFIELD("VDPPDrri")           1, 0, 0, 957, 3,  6, 1,  0, 0}, // #923
  {DBGFIELD("DPPDrmi_VDPPDrmi")   1, 0, 0, 952, 5, 10, 1,  4, 1}, // #924
  {DBGFIELD("VFMADDPD4rr_VFMADDPD4rrY_VFMADDPD4rrY_REV_VFMADDPD4rr_REV_VFMADDPDr132r_VFMADDPDr132rY_VFMADDPDr213r_VFMADDPDr213rY_VFMADDPDr231r_VFMADDPDr231rY_VFMADDPS4rr_VFMADDPS4rrY_VFMADDPS4rrY_REV_VFMADDPS4rr_REV_VFMADDPSr132r_VFMADDPSr132rY_VFMADDPSr213r_VFMADDPSr213rY_VFMADDPSr231r_VFMADDPSr231rY_VFMADDSD4rr_VFMADDSD4rr_Int_VFMADDSD4rr_REV_VFMADDSDr132r_VFMADDSDr132r_Int_VFMADDSDr213r_VFMADDSDr213r_Int_VFMADDSDr231r_VFMADDSDr231r_Int_VFMADDSS4rr_VFMADDSS4rr_Int_VFMADDSS4rr_REV_VFMADDSSr132r_VFMADDSSr132r_Int_VFMADDSSr213r_VFMADDSSr213r_Int_VFMADDSSr231r_VFMADDSSr231r_Int_VFMADDSUBPDr132r_VFMADDSUBPDr132rY_VFMADDSUBPDr213r_VFMADDSUBPDr213rY_VFMADDSUBPDr231r_VFMADDSUBPDr231rY_VFMADDSUBPSr132r_VFMADDSUBPSr132rY_VFMADDSUBPSr213r_VFMADDSUBPSr213rY_VFMADDSUBPSr231r_VFMADDSUBPSr231rY_VFMSUBADDPDr132r_VFMSUBADDPDr132rY_VFMSUBADDPDr213r_VFMSUBADDPDr213rY_VFMSUBADDPDr231r_VFMSUBADDPDr231rY_VFMSUBADDPSr132r_VFMSUBADDPSr132rY_VFMSUBADDPSr213r_VFMSUBADDPSr213rY_VFMSUBADDPSr231r_VFMSUBADDPSr231rY_VFMSUBPD4rr_VFMSUBPD4rrY_VFMSUBPD4rrY_REV_VFMSUBPD4rr_REV_VFMSUBPDr132r_VFMSUBPDr132rY_VFMSUBPDr213r_VFMSUBPDr213rY_VFMSUBPDr231r_VFMSUBPDr231rY_VFMSUBPS4rr_VFMSUBPS4rrY_VFMSUBPS4rrY_REV_VFMSUBPS4rr_REV_VFMSUBPSr132r_VFMSUBPSr132rY_VFMSUBPSr213r_VFMSUBPSr213rY_VFMSUBPSr231r_VFMSUBPSr231rY_VFMSUBSD4rr_VFMSUBSD4rr_Int_VFMSUBSD4rr_REV_VFMSUBSDr132r_VFMSUBSDr132r_Int_VFMSUBSDr213r_VFMSUBSDr213r_Int_VFMSUBSDr231r_VFMSUBSDr231r_Int_VFMSUBSS4rr_VFMSUBSS4rr_Int_VFMSUBSS4rr_REV_VFMSUBSSr132r_VFMSUBSSr132r_Int_VFMSUBSSr213r_VFMSUBSSr213r_Int_VFMSUBSSr231r_VFMSUBSSr231r_Int_VFNMADDPD4rr_VFNMADDPD4rrY_VFNMADDPD4rrY_REV_VFNMADDPD4rr_REV_VFNMADDPDr132r_VFNMADDPDr132rY_VFNMADDPDr213r_VFNMADDPDr213rY_VFNMADDPDr231r_VFNMADDPDr231rY_VFNMADDPS4rr_VFNMADDPS4rrY_VFNMADDPS4rrY_REV_VFNMADDPS4rr_REV_VFNMADDPSr132r_VFNMADDPSr132rY_VFNMADDPSr213r_VFNMADDPSr213rY_VFNMADDPSr231r_VFNMADDPSr231rY_VFNMADDSD4rr_VFNMADDSD4rr_Int_VFNMADDSD4rr_REV_VFNMADDSDr132r_VFNMADDSDr132r_Int_VFNMADDSDr213r_VFNMADDSDr213r_Int_VFNMADDSDr231r_VFNMADDSDr231r_Int_VFNMADDSS4rr_VFNMADDSS4rr_Int_VFNMADDSS4rr_REV_VFNMADDSSr132r_VFNMADDSSr132r_Int_VFNMADDSSr213r_VFNMADDSSr213r_Int_VFNMADDSSr231r_VFNMADDSSr231r_Int_VFNMSUBPD4rr_VFNMSUBPD4rrY_VFNMSUBPD4rrY_REV_VFNMSUBPD4rr_REV_VFNMSUBPDr132r_VFNMSUBPDr132rY_VFNMSUBPDr213r_VFNMSUBPDr213rY_VFNMSUBPDr231r_VFNMSUBPDr231rY_VFNMSUBPS4rr_VFNMSUBPS4rrY_VFNMSUBPS4rrY_REV_VFNMSUBPS4rr_REV_VFNMSUBPSr132r_VFNMSUBPSr132rY_VFNMSUBPSr213r_VFNMSUBPSr213rY_VFNMSUBPSr231r_VFNMSUBPSr231rY_VFNMSUBSD4rr_VFNMSUBSD4rr_Int_VFNMSUBSD4rr_REV_VFNMSUBSDr132r_VFNMSUBSDr132r_Int_VFNMSUBSDr213r_VFNMSUBSDr213r_Int_VFNMSUBSDr231r_VFNMSUBSDr231r_Int_VFNMSUBSS4rr_VFNMSUBSS4rr_Int_VFNMSUBSS4rr_REV_VFNMSUBSSr132r_VFNMSUBSSr132r_Int_VFNMSUBSSr213r_VFNMSUBSSr213r_Int_VFNMSUBSSr231r_VFNMSUBSSr231r_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #925
  {DBGFIELD("VFMADDPD4mr_VFMADDPD4mrY_VFMADDPD4rm_VFMADDPD4rmY_VFMADDPDr132m_VFMADDPDr132mY_VFMADDPDr213m_VFMADDPDr213mY_VFMADDPDr231m_VFMADDPDr231mY_VFMADDPS4mr_VFMADDPS4mrY_VFMADDPS4rm_VFMADDPS4rmY_VFMADDPSr132m_VFMADDPSr132mY_VFMADDPSr213m_VFMADDPSr213mY_VFMADDPSr231m_VFMADDPSr231mY_VFMADDSD4mr_VFMADDSD4mr_Int_VFMADDSD4rm_VFMADDSD4rm_Int_VFMADDSDr132m_VFMADDSDr132m_Int_VFMADDSDr213m_VFMADDSDr213m_Int_VFMADDSDr231m_VFMADDSDr231m_Int_VFMADDSS4mr_VFMADDSS4mr_Int_VFMADDSS4rm_VFMADDSS4rm_Int_VFMADDSSr132m_VFMADDSSr132m_Int_VFMADDSSr213m_VFMADDSSr213m_Int_VFMADDSSr231m_VFMADDSSr231m_Int_VFMADDSUBPDr132m_VFMADDSUBPDr132mY_VFMADDSUBPDr213m_VFMADDSUBPDr213mY_VFMADDSUBPDr231m_VFMADDSUBPDr231mY_VFMADDSUBPSr132m_VFMADDSUBPSr132mY_VFMADDSUBPSr213m_VFMADDSUBPSr213mY_VFMADDSUBPSr231m_VFMADDSUBPSr231mY_VFMSUBADDPDr132m_VFMSUBADDPDr132mY_VFMSUBADDPDr213m_VFMSUBADDPDr213mY_VFMSUBADDPDr231m_VFMSUBADDPDr231mY_VFMSUBADDPSr132m_VFMSUBADDPSr132mY_VFMSUBADDPSr213m_VFMSUBADDPSr213mY_VFMSUBADDPSr231m_VFMSUBADDPSr231mY_VFMSUBPD4mr_VFMSUBPD4mrY_VFMSUBPD4rm_VFMSUBPD4rmY_VFMSUBPDr132m_VFMSUBPDr132mY_VFMSUBPDr213m_VFMSUBPDr213mY_VFMSUBPDr231m_VFMSUBPDr231mY_VFMSUBPS4mr_VFMSUBPS4mrY_VFMSUBPS4rm_VFMSUBPS4rmY_VFMSUBPSr132m_VFMSUBPSr132mY_VFMSUBPSr213m_VFMSUBPSr213mY_VFMSUBPSr231m_VFMSUBPSr231mY_VFMSUBSD4mr_VFMSUBSD4mr_Int_VFMSUBSD4rm_VFMSUBSD4rm_Int_VFMSUBSDr132m_VFMSUBSDr132m_Int_VFMSUBSDr213m_VFMSUBSDr213m_Int_VFMSUBSDr231m_VFMSUBSDr231m_Int_VFMSUBSS4mr_VFMSUBSS4mr_Int_VFMSUBSS4rm_VFMSUBSS4rm_Int_VFMSUBSSr132m_VFMSUBSSr132m_Int_VFMSUBSSr213m_VFMSUBSSr213m_Int_VFMSUBSSr231m_VFMSUBSSr231m_Int_VFNMADDPD4mr_VFNMADDPD4mrY_VFNMADDPD4rm_VFNMADDPD4rmY_VFNMADDPDr132m_VFNMADDPDr132mY_VFNMADDPDr213m_VFNMADDPDr213mY_VFNMADDPDr231m_VFNMADDPDr231mY_VFNMADDPS4mr_VFNMADDPS4mrY_VFNMADDPS4rm_VFNMADDPS4rmY_VFNMADDPSr132m_VFNMADDPSr132mY_VFNMADDPSr213m_VFNMADDPSr213mY_VFNMADDPSr231m_VFNMADDPSr231mY_VFNMADDSD4mr_VFNMADDSD4mr_Int_VFNMADDSD4rm_VFNMADDSD4rm_Int_VFNMADDSDr132m_VFNMADDSDr132m_Int_VFNMADDSDr213m_VFNMADDSDr213m_Int_VFNMADDSDr231m_VFNMADDSDr231m_Int_VFNMADDSS4mr_VFNMADDSS4mr_Int_VFNMADDSS4rm_VFNMADDSS4rm_Int_VFNMADDSSr132m_VFNMADDSSr132m_Int_VFNMADDSSr213m_VFNMADDSSr213m_Int_VFNMADDSSr231m_VFNMADDSSr231m_Int_VFNMSUBPD4mr_VFNMSUBPD4mrY_VFNMSUBPD4rm_VFNMSUBPD4rmY_VFNMSUBPDr132m_VFNMSUBPDr132mY_VFNMSUBPDr213m_VFNMSUBPDr213mY_VFNMSUBPDr231m_VFNMSUBPDr231mY_VFNMSUBPS4mr_VFNMSUBPS4mrY_VFNMSUBPS4rm_VFNMSUBPS4rmY_VFNMSUBPSr132m_VFNMSUBPSr132mY_VFNMSUBPSr213m_VFNMSUBPSr213mY_VFNMSUBPSr231m_VFNMSUBPSr231mY_VFNMSUBSD4mr_VFNMSUBSD4mr_Int_VFNMSUBSD4rm_VFNMSUBSD4rm_Int_VFNMSUBSDr132m_VFNMSUBSDr132m_Int_VFNMSUBSDr213m_VFNMSUBSDr213m_Int_VFNMSUBSDr231m_VFNMSUBSDr231m_Int_VFNMSUBSS4mr_VFNMSUBSS4mr_Int_VFNMSUBSS4rm_VFNMSUBSS4rm_Int_VFNMSUBSSr132m_VFNMSUBSSr132m_Int_VFNMSUBSSr213m_VFNMSUBSSr213m_Int_VFNMSUBSSr231m_VFNMSUBSSr231m_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #926
  {DBGFIELD("VSQRTPSYr")          1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #927
  {DBGFIELD("VSQRTPSYm")          1, 0, 0, 1041, 6, 51, 1,  0, 0}, // #928
  {DBGFIELD("VSQRTPDYr")          1, 0, 0, 1041, 6, 27, 1,  0, 0}, // #929
  {DBGFIELD("VSQRTPDYm")          1, 0, 0, 1041, 6, 51, 1,  0, 0}, // #930
  {DBGFIELD("RSQRTPSr_VRSQRTPSr") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #931
  {DBGFIELD("RSQRTSSr_VRSQRTSSr") 1, 0, 0, 977, 3, 19, 1,  0, 0}, // #932
  {DBGFIELD("RSQRTSSr_Int")       1, 0, 0, 972, 5,  5, 1,  4, 1}, // #933
  {DBGFIELD("VRSQRTSSr_Int")      1, 0, 0, 972, 5,  5, 1,  0, 0}, // #934
  {DBGFIELD("RSQRTPSm_VRSQRTPSm") 1, 0, 0, 972, 5,  5, 1,  0, 0}, // #935
  {DBGFIELD("RSQRTSSm_VRSQRTSSm") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #936
  {DBGFIELD("RSQRTSSm_Int_VRSQRTSSm_Int") 1, 0, 0, 972, 5,  5, 1,  4, 1}, // #937
  {DBGFIELD("VRSQRTPSYr")         1, 0, 0, 977, 3, 19, 1,  0, 0}, // #938
  {DBGFIELD("VRSQRTPSYm")         1, 0, 0, 972, 5,  5, 1,  0, 0}, // #939
  {DBGFIELD("ANDNPDrr_ANDNPSrr_ANDPDrr_ANDPSrr_ORPDrr_ORPSrr_VANDNPDYrr_VANDNPDrr_VANDNPSYrr_VANDNPSrr_VANDPDYrr_VANDPDrr_VANDPSYrr_VANDPSrr_VORPDYrr_VORPDrr_VORPSYrr_VORPSrr_VXORPDYrr_VXORPDrr_VXORPSYrr_VXORPSrr_XORPDrr_XORPSrr") 1, 0, 0, 965, 2,  2, 1,  0, 0}, // #940
  {DBGFIELD("ANDNPDrm_ANDNPSrm_ANDPDrm_ANDPSrm_ORPDrm_ORPSrm_VANDNPDYrm_VANDNPDrm_VANDNPSYrm_VANDNPSrm_VANDPDYrm_VANDPDrm_VANDPSYrm_VANDPSrm_VORPDYrm_VORPDrm_VORPSYrm_VORPSrm_VXORPDYrm_VXORPDrm_VXORPSYrm_VXORPSrm_XORPDrm_XORPSrm") 1, 0, 0, 960, 4, 16, 1,  4, 1}, // #941
  {DBGFIELD("VZEROUPPER")         0, 0, 0,  0, 0,  0, 0,  0, 0}, // #942
  {DBGFIELD("VZEROALL")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #943
  {DBGFIELD("LDMXCSR_VLDMXCSR")   1, 0, 0, 968, 3,  3, 1,  0, 0}, // #944
  {DBGFIELD("STMXCSR_VSTMXCSR")   1, 0, 0, 1006, 3,  2, 1,  0, 0} // #945
}; // BtVer2ModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc SandyBridgeModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #7
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 108, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 1066, 3, 10, 1,  1, 1}, // #26
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 433, 2, 10, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 1066, 3, 10, 1,  0, 0}, // #28
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 433, 2, 10, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 1069, 3, 10, 1,  0, 0}, // #30
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 1072, 2, 10, 1,  0, 0}, // #31
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #32
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #35
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 1074, 4,  3, 1,  1, 1}, // #37
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 1074, 4,  3, 1,  1, 1}, // #39
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 1074, 4,  3, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #55
  {DBGFIELD("WriteSystem")        1, 0, 0, 108, 2,  1, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #57
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 108, 2,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #64
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #66
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #67
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #72
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #73
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #75
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #76
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #78
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #79
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 1057, 5, 10, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 1057, 5, 10, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 1057, 5, 10, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #98
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #99
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #100
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #102
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #103
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #105
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #107
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #109
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #110
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #111
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 1093, 5, 13, 1,  1, 1}, // #113
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #114
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #115
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 1093, 5, 13, 1,  0, 0}, // #116
  {DBGFIELD("WriteFDiv")          1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 1093, 5, 13, 1,  1, 1}, // #118
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #119
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #121
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #122
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #123
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #126
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #127
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #130
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #133
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #158
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #161
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #162
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #164
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #166
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #168
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1092, 6, 11, 1,  2, 2}, // #170
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #171
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #173
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #175
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #178
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #180
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #182
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #183
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #184
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #185
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #188
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #193
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #199
  {DBGFIELD("IIC_IRET_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #200
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #201
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #202
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #203
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 1057, 5, 10, 1,  1, 1}, // #204
  {DBGFIELD("WriteLoad")          1, 0, 0, 282, 2,  9, 1,  0, 0}, // #205
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #208
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #212
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #215
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #216
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #218
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 282, 2,  9, 1,  0, 0}, // #223
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #224
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 108, 2,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #235
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #236
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #237
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #241
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #244
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #245
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #246
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #247
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #251
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 1057, 5, 10, 1,  0, 0}, // #252
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #265
  {DBGFIELD("WriteMove")          1, 0, 0, 108, 2,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #267
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #271
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #272
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #273
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #274
  {DBGFIELD("WriteShuffle")       1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #276
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecLogic") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #278
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecIMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #281
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #283
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #285
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #287
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #289
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #291
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 1074, 4,  3, 1,  1, 1}, // #295
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #297
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #299
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 108, 2,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #306
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #310
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #311
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #313
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #320
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #325
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #328
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #330
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #332
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #335
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #336
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #340
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #342
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #346
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #347
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1108, 8, 16, 1,  1, 1}, // #348
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 1116, 7, 16, 1,  0, 0}, // #349
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #350
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #351
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #352
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #353
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #354
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #358
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #361
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #363
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 1057, 5,  5, 2,  0, 0}, // #364
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 1062, 4, 17, 2,  0, 0}, // #365
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #366
  {DBGFIELD("WriteFMul")          1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #368
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #370
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #371
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #375
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #377
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #378
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #379
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #380
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #381
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #383
  {DBGFIELD("IIC_ALU_MEM_WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1123, 7, 16, 1,  1, 1}, // #384
  {DBGFIELD("IIC_ALU_NONMEM_WriteVarBlend") 1, 0, 0, 1130, 6, 19, 1,  0, 0}, // #385
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #386
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteBlend") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 1136, 3,  7, 1,  1, 1}, // #388
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 1139, 2,  7, 1,  0, 0}, // #389
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #390
  {DBGFIELD("WriteVecALU")        1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #391
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 1141, 3,  9, 1,  1, 1}, // #392
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 1144, 2,  9, 1,  0, 0}, // #393
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 1141, 3, 20, 1,  1, 1}, // #394
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 1144, 2, 20, 1,  0, 0}, // #395
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 1113, 3,  6, 1,  1, 1}, // #396
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 249, 2,  6, 1,  0, 0}, // #397
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 1113, 3, 20, 1,  1, 1}, // #398
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 249, 2, 20, 1,  0, 0}, // #399
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 1146, 5,  3, 2,  0, 0}, // #400
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #401
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #402
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #404
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #405
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #406
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #407
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #408
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #409
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #410
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #411
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #414
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #415
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #418
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #419
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #421
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #422
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #423
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #424
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #425
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #427
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #429
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #431
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 1074, 4,  3, 1,  1, 1}, // #435
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #437
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #438
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_PUSH_MEM_WriteRMW") 1, 0, 0, 1151, 2,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #442
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #443
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #444
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #447
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #450
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #451
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #452
  {DBGFIELD("WriteRMW")           1, 0, 0, 1151, 2,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #455
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #456
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #458
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #459
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 1074, 4,  3, 1,  0, 0}, // #460
  {DBGFIELD("WriteShift")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #463
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #464
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #465
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SSE_RSQRTPS_RM_WriteFRsqrtLd") 1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_RSQRTPS_RR_WriteFRsqrt") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SSE_RSQRTSS_RM_WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #469
  {DBGFIELD("WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #470
  {DBGFIELD("IIC_SSE_RSQRTSS_RR_WriteFRsqrt") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #471
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 1074, 4,  3, 1,  2, 1}, // #472
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 1053, 4, 21, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #475
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #482
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #483
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #486
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #487
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #490
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #494
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 1093, 5, 23, 1,  0, 0}, // #495
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 1093, 5, 23, 1,  0, 0}, // #497
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1093, 5, 23, 1,  1, 1}, // #499
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1093, 5, 23, 1,  1, 1}, // #500
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1093, 5, 23, 1,  1, 1}, // #502
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #503
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #504
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #505
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 108, 2,  2, 1,  0, 0}, // #507
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #509
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #510
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #511
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 108, 2,  2, 1,  0, 0}, // #512
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #513
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #514
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #515
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #516
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #517
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #518
  {DBGFIELD("WriteJump")          1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #519
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #520
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #522
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_SSE_ALU_F64S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #524
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #525
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 1153, 7, 16, 1,  1, 1}, // #526
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 1160, 6, 19, 1,  0, 0}, // #527
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #528
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 1099, 4,  2, 1,  0, 0}, // #529
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #530
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #531
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #532
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #533
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #534
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #535
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 1166, 6, 25, 2,  0, 0}, // #536
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #537
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 1057, 5, 10, 1,  1, 1}, // #538
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #539
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #540
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #541
  {DBGFIELD("WriteStore")         1, 0, 0, 442, 3,  2, 1,  0, 0}, // #542
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 1099, 4,  2, 1,  0, 0}, // #543
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #544
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #545
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 1093, 5,  3, 1,  1, 1}, // #546
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #547
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #548
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #549
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #550
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1108, 8, 16, 1,  1, 1}, // #556
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 1116, 7, 16, 1,  0, 0}, // #557
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 917, 4,  3, 1,  0, 0}, // #558
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #559
  {DBGFIELD("WriteBlend")         1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #560
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1123, 7, 16, 1,  1, 1}, // #561
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 1130, 6, 19, 1,  0, 0}, // #562
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #563
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #564
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 1136, 3,  7, 1,  1, 1}, // #565
  {DBGFIELD("WriteCLMul")         1, 0, 0, 1139, 2,  7, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 1093, 5,  3, 1,  1, 1}, // #568
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #569
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #570
  {DBGFIELD("WriteVarVecShift_ReadAfterLd") 1, 0, 0, 1099, 4,  2, 1,  1, 1}, // #571
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 1099, 4,  2, 1,  0, 0}, // #572
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 917, 4,  3, 1,  0, 0}, // #573
  {DBGFIELD("WriteVecShift")      1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #574
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 1093, 5,  3, 1,  1, 1}, // #575
  {DBGFIELD("WriteFRcpLd")        1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #576
  {DBGFIELD("WriteFRsqrtLd")      1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #577
  {DBGFIELD("WriteFSqrtLd")       1, 0, 0, 1093, 5, 23, 1,  0, 0}, // #578
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #579
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #580
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #581
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #582
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #583
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #584
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #585
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #586
  {DBGFIELD("MOV16rm")            1, 0, 0, 282, 2,  9, 1,  0, 0}, // #587
  {DBGFIELD("MOVSX32rm16_MOVSX32rm8") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #588
  {DBGFIELD("MOVZX32rm16_MOVZX32rm8") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #589
  {DBGFIELD("CMOVA16rr_CMOVAE16rr_CMOVB16rr_CMOVBE16rr_CMOVE16rr_CMOVG16rr_CMOVGE16rr_CMOVL16rr_CMOVLE16rr_CMOVNE16rr_CMOVNO16rr_CMOVNP16rr_CMOVNS16rr_CMOVO16rr_CMOVP16rr_CMOVS16rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #590
  {DBGFIELD("CMOVA32rr_CMOVA64rr_CMOVAE32rr_CMOVAE64rr_CMOVB32rr_CMOVB64rr_CMOVBE32rr_CMOVBE64rr_CMOVE32rr_CMOVE64rr_CMOVG32rr_CMOVG64rr_CMOVGE32rr_CMOVGE64rr_CMOVL32rr_CMOVL64rr_CMOVLE32rr_CMOVLE64rr_CMOVNE32rr_CMOVNE64rr_CMOVNO32rr_CMOVNO64rr_CMOVNP32rr_CMOVNP64rr_CMOVNS32rr_CMOVNS64rr_CMOVO32rr_CMOVO64rr_CMOVP32rr_CMOVP64rr_CMOVS32rr_CMOVS64rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #591
  {DBGFIELD("CMOVA16rm_CMOVAE16rm_CMOVB16rm_CMOVBE16rm_CMOVE16rm_CMOVG16rm_CMOVGE16rm_CMOVL16rm_CMOVLE16rm_CMOVNE16rm_CMOVNO16rm_CMOVNP16rm_CMOVNS16rm_CMOVO16rm_CMOVP16rm_CMOVS16rm") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #592
  {DBGFIELD("CMOVA32rm_CMOVA64rm_CMOVAE32rm_CMOVAE64rm_CMOVB32rm_CMOVB64rm_CMOVBE32rm_CMOVBE64rm_CMOVE32rm_CMOVE64rm_CMOVG32rm_CMOVG64rm_CMOVGE32rm_CMOVGE64rm_CMOVL32rm_CMOVL64rm_CMOVLE32rm_CMOVLE64rm_CMOVNE32rm_CMOVNE64rm_CMOVNO32rm_CMOVNO64rm_CMOVNP32rm_CMOVNP64rm_CMOVNS32rm_CMOVNS64rm_CMOVO32rm_CMOVO64rm_CMOVP32rm_CMOVP64rm_CMOVS32rm_CMOVS64rm") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #593
  {DBGFIELD("XCHG16ar_XCHG16rr_XCHG32ar_XCHG32ar64_XCHG32rr_XCHG64ar_XCHG64rr_XCHG8rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #594
  {DBGFIELD("XCHG16rm_XCHG32rm_XCHG64rm_XCHG8rm") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #595
  {DBGFIELD("XLAT")               1, 0, 0, 282, 2,  9, 1,  0, 0}, // #596
  {DBGFIELD("PUSH16rmm_PUSH32rmm") 1, 0, 0, 1151, 2,  2, 1,  0, 0}, // #597
  {DBGFIELD("PUSHF16_PUSHF32")    1, 0, 0, 442, 3,  2, 1,  0, 0}, // #598
  {DBGFIELD("PUSHA16_PUSHA32")    1, 0, 0, 442, 3,  2, 1,  0, 0}, // #599
  {DBGFIELD("POP16rmm_POP32rmm")  1, 0, 0, 282, 2,  9, 1,  0, 0}, // #600
  {DBGFIELD("POPF16")             1, 0, 0, 282, 2,  9, 1,  0, 0}, // #601
  {DBGFIELD("POPF32")             1, 0, 0, 282, 2,  9, 1,  0, 0}, // #602
  {DBGFIELD("POPA16_POPA32")      1, 0, 0, 282, 2,  9, 1,  0, 0}, // #603
  {DBGFIELD("LAHF_SAHF")          1, 0, 0, 108, 2,  2, 1,  0, 0}, // #604
  {DBGFIELD("BSWAP32r")           1, 0, 0, 108, 2,  2, 1,  0, 0}, // #605
  {DBGFIELD("BSWAP64r")           1, 0, 0, 108, 2,  2, 1,  0, 0}, // #606
  {DBGFIELD("MOVBE16rm_MOVBE64rm") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #607
  {DBGFIELD("MOVBE32rm")          1, 0, 0, 918, 3,  3, 1,  0, 0}, // #608
  {DBGFIELD("MOVBE16mr")          1, 0, 0, 442, 3,  2, 1,  0, 0}, // #609
  {DBGFIELD("MOVBE32mr")          1, 0, 0, 442, 3,  2, 1,  0, 0}, // #610
  {DBGFIELD("MOVBE64mr")          1, 0, 0, 442, 3,  2, 1,  0, 0}, // #611
  {DBGFIELD("ADD16mi_ADD16mi8_ADD16mr_ADD32mi_ADD32mi8_ADD32mr_ADD64mi32_ADD64mi8_ADD64mr_ADD8mi_ADD8mi8_ADD8mr_SUB16mi_SUB16mi8_SUB16mr_SUB32mi_SUB32mi8_SUB32mr_SUB64mi32_SUB64mi8_SUB64mr_SUB8mi_SUB8mi8_SUB8mr") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #612
  {DBGFIELD("ADC16ri_ADC16ri8_ADC16rr_ADC16rr_REV_ADC32ri_ADC32ri8_ADC32rr_ADC32rr_REV_ADC64ri32_ADC64ri8_ADC64rr_ADC64rr_REV_ADC8ri_ADC8ri8_ADC8rr_ADC8rr_REV_SBB16ri_SBB16ri8_SBB16rr_SBB16rr_REV_SBB32ri_SBB32ri8_SBB32rr_SBB32rr_REV_SBB64ri32_SBB64ri8_SBB64rr_SBB64rr_REV_SBB8ri_SBB8ri8_SBB8rr_SBB8rr_REV") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #613
  {DBGFIELD("ADC16rm_ADC32rm_ADC64rm_ADC8rm_SBB16rm_SBB32rm_SBB64rm_SBB8rm") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #614
  {DBGFIELD("ADC16mi_ADC16mi8_ADC16mr_ADC32mi_ADC32mi8_ADC32mr_ADC64mi32_ADC64mi8_ADC64mr_ADC8mi_ADC8mi8_ADC8mr_SBB16mi_SBB16mi8_SBB16mr_SBB32mi_SBB32mi8_SBB32mr_SBB64mi32_SBB64mi8_SBB64mr_SBB8mi_SBB8mi8_SBB8mr") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #615
  {DBGFIELD("DEC16m_DEC32m_DEC64m_DEC8m_INC16m_INC32m_INC64m_INC8m_NEG16m_NEG32m_NEG64m_NEG8m_NOT16m_NOT32m_NOT64m_NOT8m") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #616
  {DBGFIELD("IMUL16r_IMUL16rr")   1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #617
  {DBGFIELD("IMUL16rm")           1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #618
  {DBGFIELD("IMUL16rmi_IMUL16rmi8") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #619
  {DBGFIELD("IMUL16rri_IMUL16rri8") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #620
  {DBGFIELD("MUL16r")             1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #621
  {DBGFIELD("IMUL16m")            1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #622
  {DBGFIELD("MUL16m")             1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #623
  {DBGFIELD("IMUL32r_IMUL32rr")   1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #624
  {DBGFIELD("IMUL32rm")           1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #625
  {DBGFIELD("IMUL32rmi_IMUL32rmi8") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #626
  {DBGFIELD("IMUL32rri_IMUL32rri8") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #627
  {DBGFIELD("MUL32r")             1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #628
  {DBGFIELD("IMUL32m")            1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #629
  {DBGFIELD("MUL32m")             1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #630
  {DBGFIELD("IMUL64r_IMUL64rr")   1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #631
  {DBGFIELD("IMUL64rm")           1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #632
  {DBGFIELD("IMUL64rmi32_IMUL64rmi8") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #633
  {DBGFIELD("IMUL64rri32_IMUL64rri8") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #634
  {DBGFIELD("MUL64r")             1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #635
  {DBGFIELD("IMUL64m")            1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #636
  {DBGFIELD("MUL64m")             1, 0, 0, 1057, 5,  5, 1,  2, 2}, // #637
  {DBGFIELD("MULX32rr")           2, 0, 0, 1062, 4, 17, 2,  0, 0}, // #638
  {DBGFIELD("MULX32rm")           2, 0, 0, 1057, 5,  5, 2,  0, 0}, // #639
  {DBGFIELD("MULX64rr")           2, 0, 0, 1062, 4, 17, 2,  0, 0}, // #640
  {DBGFIELD("MULX64rm")           2, 0, 0, 1057, 5,  5, 2,  0, 0}, // #641
  {DBGFIELD("DIV8r")              1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #642
  {DBGFIELD("DIV16r")             1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #643
  {DBGFIELD("DIV32r")             1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #644
  {DBGFIELD("DIV64r")             1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #645
  {DBGFIELD("IDIV8r")             1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #646
  {DBGFIELD("IDIV16r")            1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #647
  {DBGFIELD("IDIV32r")            1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #648
  {DBGFIELD("IDIV64r")            1, 0, 0, 1098, 5, 12, 1,  0, 0}, // #649
  {DBGFIELD("AND16mi_AND16mi8_AND16mr_AND32mi_AND32mi8_AND32mr_AND64mi32_AND64mi8_AND64mr_AND8mi_AND8mi8_AND8mr_OR16mi_OR16mi8_OR16mr_OR32mi_OR32mi8_OR32mr_OR64mi32_OR64mi8_OR64mr_OR8mi_OR8mi8_OR8mr_XOR16mi_XOR16mi8_XOR16mr_XOR32mi_XOR32mi8_XOR32mr_XOR64mi32_XOR64mi8_XOR64mr_XOR8mi_XOR8mi8_XOR8mr") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #650
  {DBGFIELD("OR32mrLocked")       2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #651
  {DBGFIELD("SAR16m1_SAR16mi_SAR32m1_SAR32mi_SAR64m1_SAR64mi_SAR8m1_SAR8mi_SHL16m1_SHL16mi_SHL32m1_SHL32mi_SHL64m1_SHL64mi_SHL8m1_SHL8mi_SHR16m1_SHR16mi_SHR32m1_SHR32mi_SHR64m1_SHR64mi_SHR8m1_SHR8mi") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #652
  {DBGFIELD("SAR16rCL_SAR32rCL_SAR64rCL_SAR8rCL_SHL16rCL_SHL32rCL_SHL64rCL_SHL8rCL_SHR16rCL_SHR32rCL_SHR64rCL_SHR8rCL") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #653
  {DBGFIELD("SAR16mCL_SAR32mCL_SAR64mCL_SAR8mCL_SHL16mCL_SHL32mCL_SHL64mCL_SHL8mCL_SHR16mCL_SHR32mCL_SHR64mCL_SHR8mCL") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #654
  {DBGFIELD("ROL16r1_ROL32r1_ROL64r1_ROL8r1_ROR16r1_ROR32r1_ROR64r1_ROR8r1") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #655
  {DBGFIELD("ROL16mi_ROL32mi_ROL64mi_ROL8mi_ROR16mi_ROR32mi_ROR64mi_ROR8mi") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #656
  {DBGFIELD("ROL16rCL_ROL32rCL_ROL64rCL_ROL8rCL_ROR16rCL_ROR32rCL_ROR64rCL_ROR8rCL") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #657
  {DBGFIELD("ROL16mCL_ROL32mCL_ROL64mCL_ROL8mCL_ROR16mCL_ROR32mCL_ROR64mCL_ROR8mCL") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #658
  {DBGFIELD("RCL16r1_RCL32r1_RCL64r1_RCL8r1_RCR16r1_RCR32r1_RCR64r1_RCR8r1") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #659
  {DBGFIELD("RCL16m1_RCL32m1_RCL64m1_RCL8m1_RCR16m1_RCR32m1_RCR64m1_RCR8m1") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #660
  {DBGFIELD("RCL16rCL_RCL16ri_RCL32rCL_RCL32ri_RCL64rCL_RCL64ri_RCL8rCL_RCL8ri_RCR16rCL_RCR16ri_RCR32rCL_RCR32ri_RCR64rCL_RCR64ri_RCR8rCL_RCR8ri") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #661
  {DBGFIELD("RCL16mCL_RCL16mi_RCL32mCL_RCL32mi_RCL64mCL_RCL64mi_RCL8mCL_RCL8mi_RCR16mCL_RCR16mi_RCR32mCL_RCR32mi_RCR64mCL_RCR64mi_RCR8mCL_RCR8mi") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #662
  {DBGFIELD("SHLD16rri8_SHRD16rri8") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #663
  {DBGFIELD("SHLD32rri8_SHRD32rri8") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #664
  {DBGFIELD("SHLD64rri8_SHRD64rri8") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #665
  {DBGFIELD("SHLD16mri8_SHRD16mri8") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #666
  {DBGFIELD("SHLD32mri8_SHRD32mri8") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #667
  {DBGFIELD("SHLD64mri8_SHRD64mri8") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #668
  {DBGFIELD("SHLD16rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #669
  {DBGFIELD("SHLD32rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #670
  {DBGFIELD("SHLD64rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #671
  {DBGFIELD("SHRD16rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #672
  {DBGFIELD("SHRD32rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #673
  {DBGFIELD("SHRD64rrCL")         1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #674
  {DBGFIELD("SHLD16mrCL_SHRD16mrCL") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #675
  {DBGFIELD("SHLD32mrCL_SHRD32mrCL") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #676
  {DBGFIELD("SHLD64mrCL_SHRD64mrCL") 2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #677
  {DBGFIELD("BT16ri8_BT32ri8_BT64ri8") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #678
  {DBGFIELD("BT16rr_BT32rr_BT64rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #679
  {DBGFIELD("BT16mr_BT32mr_BT64mr") 1, 0, 0, 918, 3,  3, 1,  0, 0}, // #680
  {DBGFIELD("BT16mi8_BT32mi8_BT64mi8") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #681
  {DBGFIELD("BTC16ri8_BTC32ri8_BTC64ri8_BTR16ri8_BTR32ri8_BTR64ri8_BTS16ri8_BTS32ri8_BTS64ri8") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #682
  {DBGFIELD("BTC16rr_BTC32rr_BTC64rr_BTR16rr_BTR32rr_BTS16rr_BTS32rr_BTS64rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #683
  {DBGFIELD("BTR64rr")            1, 0, 0, 108, 2,  2, 1,  0, 0}, // #684
  {DBGFIELD("BTC16mr_BTC32mr_BTC64mr_BTR16mr_BTR32mr_BTR64mr_BTS16mr_BTS32mr_BTS64mr") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #685
  {DBGFIELD("BTC16mi8_BTC32mi8_BTC64mi8_BTR16mi8_BTR32mi8_BTR64mi8_BTS16mi8_BTS32mi8_BTS64mi8") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #686
  {DBGFIELD("BSF16rr_BSF32rr_BSF64rr_BSR16rr_BSR32rr_BSR64rr") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #687
  {DBGFIELD("BSF16rm_BSF32rm_BSF64rm_BSR16rm_BSR32rm_BSR64rm") 1, 0, 0, 1074, 4,  3, 1,  0, 0}, // #688
  {DBGFIELD("SETAEr_SETAr_SETBEr_SETBr_SETEr_SETGEr_SETGr_SETLEr_SETLr_SETNEr_SETNOr_SETNPr_SETNSr_SETOr_SETPr_SETSr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #689
  {DBGFIELD("SETAEm_SETAm_SETBEm_SETBm_SETEm_SETGEm_SETGm_SETLEm_SETLm_SETNEm_SETNOm_SETNPm_SETNSm_SETOm_SETPm_SETSm") 2, 0, 0, 1053, 4, 21, 2,  0, 0}, // #690
  {DBGFIELD("CLD")                1, 0, 0, 108, 2,  2, 1,  0, 0}, // #691
  {DBGFIELD("STD")                1, 0, 0, 108, 2,  2, 1,  0, 0}, // #692
  {DBGFIELD("TZCNT16rr_TZCNT32rr_TZCNT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #693
  {DBGFIELD("TZCNT16rm_TZCNT32rm_TZCNT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #694
  {DBGFIELD("ANDN32rr_ANDN64rr")  1, 0, 0, 108, 2,  2, 1,  0, 0}, // #695
  {DBGFIELD("ANDN32rm_ANDN64rm")  1, 0, 0, 918, 3,  3, 1,  1, 1}, // #696
  {DBGFIELD("BLSI32rr_BLSI64rr_BLSMSK32rr_BLSMSK64rr_BLSR32rr_BLSR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #697
  {DBGFIELD("BLSI32rm_BLSI64rm_BLSMSK32rm_BLSMSK64rm_BLSR32rm_BLSR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #698
  {DBGFIELD("BEXTR32rr_BEXTR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #699
  {DBGFIELD("BEXTR32rm_BEXTR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #700
  {DBGFIELD("BZHI32rr_BZHI64rr")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #701
  {DBGFIELD("BZHI32rm_BZHI64rm")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #702
  {DBGFIELD("PDEP32rr_PDEP64rr_PEXT32rr_PEXT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #703
  {DBGFIELD("PDEP32rm_PDEP64rm_PEXT32rm_PEXT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #704
  {DBGFIELD("JCXZ_JRCXZ")         1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #705
  {DBGFIELD("LOOP")               1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #706
  {DBGFIELD("LOOPE")              1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #707
  {DBGFIELD("LOOPNE")             1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #708
  {DBGFIELD("CALL16r_CALL32r")    1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #709
  {DBGFIELD("CALL16m_CALL32m")    1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #710
  {DBGFIELD("LRETL_LRETQ_LRETW_RETL_RETQ_RETW") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #711
  {DBGFIELD("LRETIL_LRETIQ_LRETIW") 1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #712
  {DBGFIELD("RETIL_RETIQ_RETIW")  1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #713
  {DBGFIELD("BOUNDS16rm_BOUNDS32rm") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #714
  {DBGFIELD("INTO")               1, 0, 0, 108, 2,  1, 1,  0, 0}, // #715
  {DBGFIELD("LODSB_LODSW")        1, 0, 0, 108, 2,  1, 1,  0, 0}, // #716
  {DBGFIELD("LODSL_LODSQ")        1, 0, 0, 108, 2,  1, 1,  0, 0}, // #717
  {DBGFIELD("STOSB_STOSL_STOSQ_STOSW") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #718
  {DBGFIELD("MOVSB_MOVSL_MOVSQ_MOVSW") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #719
  {DBGFIELD("MOVSLDUPrm")         1, 0, 0, 282, 2,  9, 1,  0, 0}, // #720
  {DBGFIELD("MOVSLDUPrr")         1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #721
  {DBGFIELD("SCASB_SCASL_SCASQ_SCASW") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #722
  {DBGFIELD("CMPSB_CMPSL_CMPSQ_CMPSW") 1, 0, 0, 108, 2,  1, 1,  0, 0}, // #723
  {DBGFIELD("XADD16rm_XADD32rm_XADD64rm_XADD8rm") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #724
  {DBGFIELD("CMPXCHG16rm_CMPXCHG32rm_CMPXCHG64rm") 2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #725
  {DBGFIELD("CMPXCHG8rm")         2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #726
  {DBGFIELD("CMPXCHG8B")          2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #727
  {DBGFIELD("CMPXCHG16B")         2, 0, 0, 1053, 4,  3, 2,  0, 0}, // #728
  {DBGFIELD("PAUSE")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #729
  {DBGFIELD("LEAVE_LEAVE64")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #730
  {DBGFIELD("XGETBV")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #731
  {DBGFIELD("RDTSC")              1, 0, 0, 108, 2,  1, 1,  0, 0}, // #732
  {DBGFIELD("RDTSCP")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #733
  {DBGFIELD("RDPMC")              1, 0, 0, 108, 2,  1, 1,  0, 0}, // #734
  {DBGFIELD("RDRAND16r_RDRAND32r_RDRAND64r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #735
  {DBGFIELD("LD_Frr")             1, 0, 0, 108, 2,  2, 1,  0, 0}, // #736
  {DBGFIELD("LD_F80m")            1, 0, 0, 282, 2,  9, 1,  0, 0}, // #737
  {DBGFIELD("FBLDm")              0, 0, 0,  0, 0,  0, 0,  0, 0}, // #738
  {DBGFIELD("ST_FPrr_ST_Frr")     1, 0, 0, 108, 2,  2, 1,  0, 0}, // #739
  {DBGFIELD("ST_FP80m")           1, 0, 0, 442, 3,  2, 1,  0, 0}, // #740
  {DBGFIELD("FBSTPm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #741
  {DBGFIELD("XCH_F")              1, 0, 0, 108, 2,  2, 1,  0, 0}, // #742
  {DBGFIELD("ILD_F16m_ILD_F32m_ILD_F64m") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #743
  {DBGFIELD("IST_F16m_IST_F32m_IST_FP16m_IST_FP32m") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #744
  {DBGFIELD("LD_F0")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #745
  {DBGFIELD("LD_F1")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #746
  {DBGFIELD("FLDPI")              1, 0, 0, 108, 2,  1, 1,  0, 0}, // #747
  {DBGFIELD("CMOVBE_F_CMOVBE_Fp32_CMOVBE_Fp64_CMOVBE_Fp80_CMOVB_F_CMOVB_Fp32_CMOVB_Fp64_CMOVB_Fp80_CMOVNBE_F_CMOVNBE_Fp32_CMOVNBE_Fp64_CMOVNBE_Fp80_CMOVNB_F_CMOVNB_Fp32_CMOVNB_Fp64_CMOVNB_Fp80_CMOVNE_F_CMOVNE_Fp32_CMOVNE_Fp64_CMOVNE_Fp80_CMOVNP_F_CMOVNP_Fp32_CMOVNP_Fp64_CMOVNP_Fp80_CMOVP_F_CMOVP_Fp32_CMOVP_Fp64_CMOVP_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #748
  {DBGFIELD("FNSTSW16r")          1, 0, 0, 108, 2,  2, 1,  0, 0}, // #749
  {DBGFIELD("FNSTSWm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #750
  {DBGFIELD("FLDCW16m")           1, 0, 0, 282, 2,  9, 1,  0, 0}, // #751
  {DBGFIELD("FNSTCW16m")          1, 0, 0, 108, 2,  2, 1,  0, 0}, // #752
  {DBGFIELD("FDECSTP_FINCSTP")    1, 0, 0, 108, 2,  1, 1,  0, 0}, // #753
  {DBGFIELD("FFREE")              1, 0, 0, 108, 2,  1, 1,  0, 0}, // #754
  {DBGFIELD("FSAVEm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #755
  {DBGFIELD("FRSTORm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #756
  {DBGFIELD("ABS_F_ABS_Fp32_ABS_Fp64_ABS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #757
  {DBGFIELD("CHS_F_CHS_Fp32_CHS_Fp64_CHS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #758
  {DBGFIELD("COMP_FST0r_COM_FST0r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #759
  {DBGFIELD("UCOM_FPr_UCOM_Fr")   1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #760
  {DBGFIELD("FCOM32m_FCOM64m_FCOMP32m_FCOMP64m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #761
  {DBGFIELD("FCOMPP")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #762
  {DBGFIELD("UCOM_FPPr")          1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #763
  {DBGFIELD("COM_FIPr_COM_FIr")   1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #764
  {DBGFIELD("UCOM_FIPr_UCOM_FIr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #765
  {DBGFIELD("FICOM16m_FICOM32m_FICOMP16m_FICOMP32m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #766
  {DBGFIELD("TST_F_TST_Fp32_TST_Fp64_TST_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #767
  {DBGFIELD("FXAM")               1, 0, 0, 108, 2,  1, 1,  0, 0}, // #768
  {DBGFIELD("FPREM")              1, 0, 0, 108, 2,  1, 1,  0, 0}, // #769
  {DBGFIELD("FPREM1")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #770
  {DBGFIELD("FRNDINT")            1, 0, 0, 108, 2,  1, 1,  0, 0}, // #771
  {DBGFIELD("FSCALE")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #772
  {DBGFIELD("FXTRACT")            1, 0, 0, 108, 2,  1, 1,  0, 0}, // #773
  {DBGFIELD("FNOP")               1, 0, 0, 108, 2,  1, 1,  0, 0}, // #774
  {DBGFIELD("WAIT")               1, 0, 0, 108, 2,  1, 1,  0, 0}, // #775
  {DBGFIELD("FNCLEX")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #776
  {DBGFIELD("FNINIT")             1, 0, 0, 108, 2,  1, 1,  0, 0}, // #777
  {DBGFIELD("MMX_MOVD64from64rr_MMX_MOVD64grr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #778
  {DBGFIELD("MOVPDI2DIrr_VMOVPDI2DIrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #779
  {DBGFIELD("MMX_MOVD64rr_MMX_MOVD64to64rr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #780
  {DBGFIELD("MOVDI2PDIrr_VMOVDI2PDIrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #781
  {DBGFIELD("VMOVPQIto64rr")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #782
  {DBGFIELD("VMOV64toPQIrr")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #783
  {DBGFIELD("MMX_MOVQ64rr_MMX_MOVQ64rr_REV") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #784
  {DBGFIELD("MOVDQArr_MOVDQArr_REV_VMOVDQAYrr_VMOVDQAYrr_REV_VMOVDQArr_VMOVDQArr_REV") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #785
  {DBGFIELD("MOVDQUrr_MOVDQUrr_REV_VMOVDQUYrr_VMOVDQUYrr_REV_VMOVDQUrr_VMOVDQUrr_REV") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #786
  {DBGFIELD("MMX_MOVDQ2Qrr")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #787
  {DBGFIELD("MMX_MOVQ2DQrr")      1, 0, 0, 108, 2,  2, 1,  0, 0}, // #788
  {DBGFIELD("MMX_PACKSSDWirr_MMX_PACKSSWBirr_MMX_PACKUSWBirr") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #789
  {DBGFIELD("MMX_PACKSSDWirm_MMX_PACKSSWBirm_MMX_PACKUSWBirm") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #790
  {DBGFIELD("VPMOVSXBQYrr_VPMOVSXBWYrr_VPMOVSXDQYrr_VPMOVZXBQYrr_VPMOVZXBWYrr_VPMOVZXDQYrr") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #791
  {DBGFIELD("PBLENDWrri")         1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #792
  {DBGFIELD("VPBLENDWYrri_VPBLENDWrri") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #793
  {DBGFIELD("PBLENDWrmi")         1, 0, 0, 917, 4,  3, 1,  1, 1}, // #794
  {DBGFIELD("VPBLENDWYrmi_VPBLENDWrmi") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #795
  {DBGFIELD("VPBLENDDYrri_VPBLENDDrri") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #796
  {DBGFIELD("VPBLENDDYrmi_VPBLENDDrmi") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #797
  {DBGFIELD("MMX_MASKMOVQ_MMX_MASKMOVQ64") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #798
  {DBGFIELD("MASKMOVDQU_MASKMOVDQU64_VMASKMOVDQU_VMASKMOVDQU64") 1, 0, 0, 442, 3,  2, 1,  0, 0}, // #799
  {DBGFIELD("VPMASKMOVDYrm_VPMASKMOVDrm_VPMASKMOVQYrm_VPMASKMOVQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #800
  {DBGFIELD("VPMASKMOVDYmr_VPMASKMOVDmr_VPMASKMOVQYmr_VPMASKMOVQmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #801
  {DBGFIELD("MMX_PMOVMSKBrr")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #802
  {DBGFIELD("PMOVMSKBrr_VPMOVMSKBrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #803
  {DBGFIELD("VPMOVMSKBYrr")       1, 0, 0, 108, 2,  2, 1,  0, 0}, // #804
  {DBGFIELD("MMX_PEXTRWirri")     1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #805
  {DBGFIELD("PEXTRBrr_PEXTRDrr_PEXTRQrr_PEXTRWrr_REV") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #806
  {DBGFIELD("PEXTRBmr_PEXTRDmr_PEXTRQmr_PEXTRWmr") 2, 0, 0, 1146, 5,  3, 2,  0, 0}, // #807
  {DBGFIELD("VPBROADCASTBrm_VPBROADCASTWrm") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #808
  {DBGFIELD("VPBROADCASTBYrm_VPBROADCASTWYrm") 1, 0, 0, 282, 2,  9, 1,  0, 0}, // #809
  {DBGFIELD("VPGATHERDDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #810
  {DBGFIELD("VPGATHERDDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #811
  {DBGFIELD("VPGATHERQDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #812
  {DBGFIELD("VPGATHERQDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #813
  {DBGFIELD("VPGATHERDQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #814
  {DBGFIELD("VPGATHERDQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #815
  {DBGFIELD("VPGATHERQQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #816
  {DBGFIELD("VPGATHERQQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #817
  {DBGFIELD("MMX_PHADDSWrr64_MMX_PHADDWrr64_MMX_PHSUBSWrr64_MMX_PHSUBWrr64") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #818
  {DBGFIELD("MMX_PHADDrr64_MMX_PHSUBDrr64") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #819
  {DBGFIELD("PHADDDrr_PHSUBDrr_VPHADDDrr_VPHSUBDrr") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #820
  {DBGFIELD("PHADDSWrr128_PHSUBSWrr128_VPHADDSWrr128_VPHADDSWrr256_VPHSUBSWrr128_VPHSUBSWrr256") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #821
  {DBGFIELD("PHADDWrr_PHSUBWrr_VPHADDDYrr_VPHADDWYrr_VPHADDWrr_VPHSUBDYrr_VPHSUBWYrr_VPHSUBWrr") 1, 0, 0, 1063, 3,  2, 1,  0, 0}, // #822
  {DBGFIELD("MMX_PHADDSWrm64_MMX_PHADDWrm64_MMX_PHSUBSWrm64_MMX_PHSUBWrm64") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #823
  {DBGFIELD("MMX_PHADDrm64_MMX_PHSUBDrm64") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #824
  {DBGFIELD("PHADDDrm_PHSUBDrm_VPHADDDrm_VPHSUBDrm") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #825
  {DBGFIELD("PHADDSWrm128_PHSUBSWrm128_VPHADDSWrm128_VPHADDSWrm256_VPHSUBSWrm128_VPHSUBSWrm256") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #826
  {DBGFIELD("PHADDWrm_PHSUBWrm_VPHADDDYrm_VPHADDWYrm_VPHADDWrm_VPHSUBDYrm_VPHSUBWYrm_VPHSUBWrm") 1, 0, 0, 917, 4,  3, 1,  1, 1}, // #827
  {DBGFIELD("PCMPGTQrr_VPCMPGTQYrr_VPCMPGTQrr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #828
  {DBGFIELD("PCMPGTQrm_VPCMPGTQYrm_VPCMPGTQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #829
  {DBGFIELD("PMULLDrr_VPMULLDYrr_VPMULLDrr") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #830
  {DBGFIELD("PMULLDrm_VPMULLDYrm_VPMULLDrm") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #831
  {DBGFIELD("PTESTrr_VPTESTYrr_VPTESTrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #832
  {DBGFIELD("PTESTrm_VPTESTYrm_VPTESTrm") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #833
  {DBGFIELD("PSLLDrr_PSLLQrr_PSLLWrr_PSRADrr_PSRAWrr_PSRLDrr_PSRLQrr_PSRLWrr_VPSLLDYrr_VPSLLDrr_VPSLLQYrr_VPSLLQrr_VPSLLWYrr_VPSLLWrr_VPSRADYrr_VPSRADrr_VPSRAWYrr_VPSRAWrr_VPSRLDYrr_VPSRLDrr_VPSRLQYrr_VPSRLQrr_VPSRLWYrr_VPSRLWrr") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #834
  {DBGFIELD("PSLLDQri_PSRLDQri")  1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #835
  {DBGFIELD("VPSLLDQYri_VPSLLDQri_VPSRLDQYri_VPSRLDQri") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #836
  {DBGFIELD("MMX_EMMS")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #837
  {DBGFIELD("MOVMSKPDrr_MOVMSKPSrr_VMOVMSKPDrr_VMOVMSKPSrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #838
  {DBGFIELD("VMOVMSKPDYrr_VMOVMSKPSYrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #839
  {DBGFIELD("VPERM2F128rr")       1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #840
  {DBGFIELD("VPERM2F128rm")       1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #841
  {DBGFIELD("BLENDVPDrr0_BLENDVPSrr0") 1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #842
  {DBGFIELD("BLENDVPDrm0_BLENDVPSrm0") 1, 0, 0, 1074, 4,  3, 1,  1, 1}, // #843
  {DBGFIELD("VBROADCASTF128")     1, 0, 0, 1081, 6,  3, 1,  0, 0}, // #844
  {DBGFIELD("EXTRACTPSrr")        1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #845
  {DBGFIELD("VEXTRACTPSrr")       1, 0, 0, 1078, 3,  2, 1,  0, 0}, // #846
  {DBGFIELD("EXTRACTPSmr")        2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #847
  {DBGFIELD("VEXTRACTPSmr")       2, 0, 0, 1103, 5,  3, 2,  0, 0}, // #848
  {DBGFIELD("VEXTRACTF128rr")     1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #849
  {DBGFIELD("VEXTRACTF128mr")     1, 0, 0, 442, 3,  2, 1,  0, 0}, // #850
  {DBGFIELD("VINSERTF128rr")      1, 0, 0, 1087, 5,  2, 1,  0, 0}, // #851
  {DBGFIELD("VINSERTF128rm")      1, 0, 0, 1081, 6,  3, 1,  1, 1}, // #852
  {DBGFIELD("VMASKMOVPDYrm_VMASKMOVPDrm_VMASKMOVPSYrm_VMASKMOVPSrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #853
  {DBGFIELD("VMASKMOVPDmr_VMASKMOVPSmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #854
  {DBGFIELD("VMASKMOVPDYmr_VMASKMOVPSYmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #855
  {DBGFIELD("VGATHERDPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #856
  {DBGFIELD("VGATHERDPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #857
  {DBGFIELD("VGATHERQPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #858
  {DBGFIELD("VGATHERQPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #859
  {DBGFIELD("VGATHERDPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #860
  {DBGFIELD("VGATHERDPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #861
  {DBGFIELD("VGATHERQPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #862
  {DBGFIELD("VGATHERQPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #863
  {DBGFIELD("CVTPD2PSrr_VCVTPD2PSrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #864
  {DBGFIELD("CVTPD2PSrm_VCVTPD2PSXrm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #865
  {DBGFIELD("VCVTPD2PSYrr")       1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #866
  {DBGFIELD("VCVTPD2PSYrm")       1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #867
  {DBGFIELD("CVTSD2SSrr_Int_CVTSD2SSrr_Int_VCVTSD2SSrr_VCVTSD2SSrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #868
  {DBGFIELD("CVTSD2SSrm")         1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #869
  {DBGFIELD("Int_CVTSD2SSrm_Int_VCVTSD2SSrm_VCVTSD2SSrm") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #870
  {DBGFIELD("CVTPS2PDrr_VCVTPS2PDrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #871
  {DBGFIELD("CVTPS2PDrm_VCVTPS2PDYrm_VCVTPS2PDrm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #872
  {DBGFIELD("VCVTPS2PDYrr")       1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #873
  {DBGFIELD("CVTSS2SDrr_Int_CVTSS2SDrr_Int_VCVTSS2SDrr_VCVTSS2SDrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #874
  {DBGFIELD("CVTSS2SDrm")         1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #875
  {DBGFIELD("Int_CVTSS2SDrm_Int_VCVTSS2SDrm_VCVTSS2SDrm") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #876
  {DBGFIELD("CVTDQ2PDrr")         1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #877
  {DBGFIELD("VCVTDQ2PDrr")        1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #878
  {DBGFIELD("VCVTDQ2PDYrr")       1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #879
  {DBGFIELD("CVTPD2DQrr_CVTTPD2DQrr_VCVTTPD2DQrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #880
  {DBGFIELD("VCVTPD2DQrr")        1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #881
  {DBGFIELD("CVTPD2DQrm_CVTTPD2DQrm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #882
  {DBGFIELD("VCVTPD2DQYrr")       1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #883
  {DBGFIELD("VCVTTPD2DQYrr")      1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #884
  {DBGFIELD("VCVTPD2DQYrm")       1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #885
  {DBGFIELD("VCVTTPD2DQYrm")      1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #886
  {DBGFIELD("MMX_CVTPS2PIirr_MMX_CVTTPS2PIirr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #887
  {DBGFIELD("MMX_CVTPI2PDirr")    1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #888
  {DBGFIELD("MMX_CVTPD2PIirr_MMX_CVTTPD2PIirr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #889
  {DBGFIELD("CVTSI2SS64rr_CVTSI2SSrr_Int_CVTSI2SS64rr_Int_CVTSI2SSrr_Int_VCVTSI2SS64rr_Int_VCVTSI2SSrr") 1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #890
  {DBGFIELD("VCVTSI2SS64rr_VCVTSI2SSrr") 1, 0, 0, 1062, 4,  9, 1,  0, 0}, // #891
  {DBGFIELD("CVTSS2SI64rr_CVTTSS2SI64rr_Int_CVTTSS2SI64rr_Int_VCVTTSS2SI64rr_VCVTSS2SI64rr_VCVTTSS2SI64rr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #892
  {DBGFIELD("CVTSS2SIrr_CVTTSS2SIrr_Int_CVTTSS2SIrr_Int_VCVTTSS2SIrr_VCVTSS2SIrr_VCVTTSS2SIrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #893
  {DBGFIELD("CVTSS2SI64rm_CVTTSS2SI64rm_Int_CVTTSS2SI64rm_Int_VCVTTSS2SI64rm_VCVTSS2SI64rm_VCVTTSS2SI64rm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #894
  {DBGFIELD("CVTSS2SIrm_CVTTSS2SIrm_Int_CVTTSS2SIrm_Int_VCVTTSS2SIrm_VCVTSS2SIrm_VCVTTSS2SIrm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #895
  {DBGFIELD("CVTSD2SI64rr_CVTSD2SIrr_CVTTSD2SI64rr_CVTTSD2SIrr_Int_CVTTSD2SI64rr_Int_CVTTSD2SIrr_Int_VCVTTSD2SI64rr_Int_VCVTTSD2SIrr_VCVTSD2SI64rr_VCVTSD2SIrr_VCVTTSD2SI64rr_VCVTTSD2SIrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #896
  {DBGFIELD("CVTSD2SI64rm_CVTSD2SIrm_CVTTSD2SI64rm_CVTTSD2SIrm_Int_CVTTSD2SI64rm_Int_CVTTSD2SIrm_Int_VCVTTSD2SI64rm_Int_VCVTTSD2SIrm_VCVTSD2SI64rm_VCVTSD2SIrm_VCVTTSD2SI64rm_VCVTTSD2SIrm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #897
  {DBGFIELD("VCVTPS2PHYrr_VCVTPS2PHrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #898
  {DBGFIELD("VCVTPS2PHYmr_VCVTPS2PHmr") 2, 0, 0, 1166, 6, 25, 2,  0, 0}, // #899
  {DBGFIELD("VCVTPH2PSYrr_VCVTPH2PSrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #900
  {DBGFIELD("HADDPDrr_HADDPSrr_HSUBPDrr_HSUBPSrr_VHADDPDYrr_VHADDPDrr_VHADDPSYrr_VHADDPSrr_VHSUBPDYrr_VHSUBPDrr_VHSUBPSYrr_VHSUBPSrr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #901
  {DBGFIELD("HADDPDrm_HADDPSrm_HSUBPDrm_HSUBPSrm_VHADDPDYrm_VHADDPDrm_VHADDPSYrm_VHADDPSrm_VHSUBPDYrm_VHSUBPDrm_VHSUBPSYrm_VHSUBPSrm") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #902
  {DBGFIELD("MULPDrr_VMULPDrr")   1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #903
  {DBGFIELD("MULPSrr_VMULPSrr")   1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #904
  {DBGFIELD("MULSDrr_MULSDrr_Int_VMULSDrr_VMULSDrr_Int") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #905
  {DBGFIELD("MULSSrr_MULSSrr_Int_VMULSSrr_VMULSSrr_Int") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #906
  {DBGFIELD("MULPDrm_MULPSrm_VMULPDrm_VMULPSrm") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #907
  {DBGFIELD("MULSDrm_MULSDrm_Int_MULSSrm_MULSSrm_Int_VMULSDrm_VMULSDrm_Int_VMULSSrm_VMULSSrm_Int") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #908
  {DBGFIELD("VDIVPSYrr")          1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #909
  {DBGFIELD("VDIVPSYrm")          1, 0, 0, 1093, 5, 13, 1,  1, 1}, // #910
  {DBGFIELD("VDIVPDYrr")          1, 0, 0, 1099, 4, 14, 1,  0, 0}, // #911
  {DBGFIELD("VDIVPDYrm")          1, 0, 0, 1093, 5, 13, 1,  1, 1}, // #912
  {DBGFIELD("VRCPPSYr")           1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #913
  {DBGFIELD("VRCPPSYm")           1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #914
  {DBGFIELD("ROUNDPDr_ROUNDPSr_VROUNDPDr_VROUNDPSr_VROUNDYPDr_VROUNDYPSr") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #915
  {DBGFIELD("ROUNDSDr_ROUNDSDr_Int_ROUNDSSr_ROUNDSSr_Int_VROUNDSDr_VROUNDSDr_Int_VROUNDSSr_VROUNDSSr_Int") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #916
  {DBGFIELD("ROUNDPDm_VROUNDPDm_VROUNDYPDm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #917
  {DBGFIELD("ROUNDPSm_VROUNDPSm_VROUNDYPSm") 1, 0, 0, 1057, 5,  5, 1,  0, 0}, // #918
  {DBGFIELD("ROUNDSDm_ROUNDSSm_VROUNDSDm_VROUNDSSm") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #919
  {DBGFIELD("DPPSrri_VDPPSYrri_VDPPSrri") 1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #920
  {DBGFIELD("DPPSrmi_VDPPSYrmi_VDPPSrmi") 1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #921
  {DBGFIELD("DPPDrri")            1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #922
  {DBGFIELD("VDPPDrri")           1, 0, 0, 1062, 4,  6, 1,  0, 0}, // #923
  {DBGFIELD("DPPDrmi_VDPPDrmi")   1, 0, 0, 1057, 5,  5, 1,  1, 1}, // #924
  {DBGFIELD("VFMADDPD4rr_VFMADDPD4rrY_VFMADDPD4rrY_REV_VFMADDPD4rr_REV_VFMADDPDr132r_VFMADDPDr132rY_VFMADDPDr213r_VFMADDPDr213rY_VFMADDPDr231r_VFMADDPDr231rY_VFMADDPS4rr_VFMADDPS4rrY_VFMADDPS4rrY_REV_VFMADDPS4rr_REV_VFMADDPSr132r_VFMADDPSr132rY_VFMADDPSr213r_VFMADDPSr213rY_VFMADDPSr231r_VFMADDPSr231rY_VFMADDSD4rr_VFMADDSD4rr_Int_VFMADDSD4rr_REV_VFMADDSDr132r_VFMADDSDr132r_Int_VFMADDSDr213r_VFMADDSDr213r_Int_VFMADDSDr231r_VFMADDSDr231r_Int_VFMADDSS4rr_VFMADDSS4rr_Int_VFMADDSS4rr_REV_VFMADDSSr132r_VFMADDSSr132r_Int_VFMADDSSr213r_VFMADDSSr213r_Int_VFMADDSSr231r_VFMADDSSr231r_Int_VFMADDSUBPDr132r_VFMADDSUBPDr132rY_VFMADDSUBPDr213r_VFMADDSUBPDr213rY_VFMADDSUBPDr231r_VFMADDSUBPDr231rY_VFMADDSUBPSr132r_VFMADDSUBPSr132rY_VFMADDSUBPSr213r_VFMADDSUBPSr213rY_VFMADDSUBPSr231r_VFMADDSUBPSr231rY_VFMSUBADDPDr132r_VFMSUBADDPDr132rY_VFMSUBADDPDr213r_VFMSUBADDPDr213rY_VFMSUBADDPDr231r_VFMSUBADDPDr231rY_VFMSUBADDPSr132r_VFMSUBADDPSr132rY_VFMSUBADDPSr213r_VFMSUBADDPSr213rY_VFMSUBADDPSr231r_VFMSUBADDPSr231rY_VFMSUBPD4rr_VFMSUBPD4rrY_VFMSUBPD4rrY_REV_VFMSUBPD4rr_REV_VFMSUBPDr132r_VFMSUBPDr132rY_VFMSUBPDr213r_VFMSUBPDr213rY_VFMSUBPDr231r_VFMSUBPDr231rY_VFMSUBPS4rr_VFMSUBPS4rrY_VFMSUBPS4rrY_REV_VFMSUBPS4rr_REV_VFMSUBPSr132r_VFMSUBPSr132rY_VFMSUBPSr213r_VFMSUBPSr213rY_VFMSUBPSr231r_VFMSUBPSr231rY_VFMSUBSD4rr_VFMSUBSD4rr_Int_VFMSUBSD4rr_REV_VFMSUBSDr132r_VFMSUBSDr132r_Int_VFMSUBSDr213r_VFMSUBSDr213r_Int_VFMSUBSDr231r_VFMSUBSDr231r_Int_VFMSUBSS4rr_VFMSUBSS4rr_Int_VFMSUBSS4rr_REV_VFMSUBSSr132r_VFMSUBSSr132r_Int_VFMSUBSSr213r_VFMSUBSSr213r_Int_VFMSUBSSr231r_VFMSUBSSr231r_Int_VFNMADDPD4rr_VFNMADDPD4rrY_VFNMADDPD4rrY_REV_VFNMADDPD4rr_REV_VFNMADDPDr132r_VFNMADDPDr132rY_VFNMADDPDr213r_VFNMADDPDr213rY_VFNMADDPDr231r_VFNMADDPDr231rY_VFNMADDPS4rr_VFNMADDPS4rrY_VFNMADDPS4rrY_REV_VFNMADDPS4rr_REV_VFNMADDPSr132r_VFNMADDPSr132rY_VFNMADDPSr213r_VFNMADDPSr213rY_VFNMADDPSr231r_VFNMADDPSr231rY_VFNMADDSD4rr_VFNMADDSD4rr_Int_VFNMADDSD4rr_REV_VFNMADDSDr132r_VFNMADDSDr132r_Int_VFNMADDSDr213r_VFNMADDSDr213r_Int_VFNMADDSDr231r_VFNMADDSDr231r_Int_VFNMADDSS4rr_VFNMADDSS4rr_Int_VFNMADDSS4rr_REV_VFNMADDSSr132r_VFNMADDSSr132r_Int_VFNMADDSSr213r_VFNMADDSSr213r_Int_VFNMADDSSr231r_VFNMADDSSr231r_Int_VFNMSUBPD4rr_VFNMSUBPD4rrY_VFNMSUBPD4rrY_REV_VFNMSUBPD4rr_REV_VFNMSUBPDr132r_VFNMSUBPDr132rY_VFNMSUBPDr213r_VFNMSUBPDr213rY_VFNMSUBPDr231r_VFNMSUBPDr231rY_VFNMSUBPS4rr_VFNMSUBPS4rrY_VFNMSUBPS4rrY_REV_VFNMSUBPS4rr_REV_VFNMSUBPSr132r_VFNMSUBPSr132rY_VFNMSUBPSr213r_VFNMSUBPSr213rY_VFNMSUBPSr231r_VFNMSUBPSr231rY_VFNMSUBSD4rr_VFNMSUBSD4rr_Int_VFNMSUBSD4rr_REV_VFNMSUBSDr132r_VFNMSUBSDr132r_Int_VFNMSUBSDr213r_VFNMSUBSDr213r_Int_VFNMSUBSDr231r_VFNMSUBSDr231r_Int_VFNMSUBSS4rr_VFNMSUBSS4rr_Int_VFNMSUBSS4rr_REV_VFNMSUBSSr132r_VFNMSUBSSr132r_Int_VFNMSUBSSr213r_VFNMSUBSSr213r_Int_VFNMSUBSSr231r_VFNMSUBSSr231r_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #925
  {DBGFIELD("VFMADDPD4mr_VFMADDPD4mrY_VFMADDPD4rm_VFMADDPD4rmY_VFMADDPDr132m_VFMADDPDr132mY_VFMADDPDr213m_VFMADDPDr213mY_VFMADDPDr231m_VFMADDPDr231mY_VFMADDPS4mr_VFMADDPS4mrY_VFMADDPS4rm_VFMADDPS4rmY_VFMADDPSr132m_VFMADDPSr132mY_VFMADDPSr213m_VFMADDPSr213mY_VFMADDPSr231m_VFMADDPSr231mY_VFMADDSD4mr_VFMADDSD4mr_Int_VFMADDSD4rm_VFMADDSD4rm_Int_VFMADDSDr132m_VFMADDSDr132m_Int_VFMADDSDr213m_VFMADDSDr213m_Int_VFMADDSDr231m_VFMADDSDr231m_Int_VFMADDSS4mr_VFMADDSS4mr_Int_VFMADDSS4rm_VFMADDSS4rm_Int_VFMADDSSr132m_VFMADDSSr132m_Int_VFMADDSSr213m_VFMADDSSr213m_Int_VFMADDSSr231m_VFMADDSSr231m_Int_VFMADDSUBPDr132m_VFMADDSUBPDr132mY_VFMADDSUBPDr213m_VFMADDSUBPDr213mY_VFMADDSUBPDr231m_VFMADDSUBPDr231mY_VFMADDSUBPSr132m_VFMADDSUBPSr132mY_VFMADDSUBPSr213m_VFMADDSUBPSr213mY_VFMADDSUBPSr231m_VFMADDSUBPSr231mY_VFMSUBADDPDr132m_VFMSUBADDPDr132mY_VFMSUBADDPDr213m_VFMSUBADDPDr213mY_VFMSUBADDPDr231m_VFMSUBADDPDr231mY_VFMSUBADDPSr132m_VFMSUBADDPSr132mY_VFMSUBADDPSr213m_VFMSUBADDPSr213mY_VFMSUBADDPSr231m_VFMSUBADDPSr231mY_VFMSUBPD4mr_VFMSUBPD4mrY_VFMSUBPD4rm_VFMSUBPD4rmY_VFMSUBPDr132m_VFMSUBPDr132mY_VFMSUBPDr213m_VFMSUBPDr213mY_VFMSUBPDr231m_VFMSUBPDr231mY_VFMSUBPS4mr_VFMSUBPS4mrY_VFMSUBPS4rm_VFMSUBPS4rmY_VFMSUBPSr132m_VFMSUBPSr132mY_VFMSUBPSr213m_VFMSUBPSr213mY_VFMSUBPSr231m_VFMSUBPSr231mY_VFMSUBSD4mr_VFMSUBSD4mr_Int_VFMSUBSD4rm_VFMSUBSD4rm_Int_VFMSUBSDr132m_VFMSUBSDr132m_Int_VFMSUBSDr213m_VFMSUBSDr213m_Int_VFMSUBSDr231m_VFMSUBSDr231m_Int_VFMSUBSS4mr_VFMSUBSS4mr_Int_VFMSUBSS4rm_VFMSUBSS4rm_Int_VFMSUBSSr132m_VFMSUBSSr132m_Int_VFMSUBSSr213m_VFMSUBSSr213m_Int_VFMSUBSSr231m_VFMSUBSSr231m_Int_VFNMADDPD4mr_VFNMADDPD4mrY_VFNMADDPD4rm_VFNMADDPD4rmY_VFNMADDPDr132m_VFNMADDPDr132mY_VFNMADDPDr213m_VFNMADDPDr213mY_VFNMADDPDr231m_VFNMADDPDr231mY_VFNMADDPS4mr_VFNMADDPS4mrY_VFNMADDPS4rm_VFNMADDPS4rmY_VFNMADDPSr132m_VFNMADDPSr132mY_VFNMADDPSr213m_VFNMADDPSr213mY_VFNMADDPSr231m_VFNMADDPSr231mY_VFNMADDSD4mr_VFNMADDSD4mr_Int_VFNMADDSD4rm_VFNMADDSD4rm_Int_VFNMADDSDr132m_VFNMADDSDr132m_Int_VFNMADDSDr213m_VFNMADDSDr213m_Int_VFNMADDSDr231m_VFNMADDSDr231m_Int_VFNMADDSS4mr_VFNMADDSS4mr_Int_VFNMADDSS4rm_VFNMADDSS4rm_Int_VFNMADDSSr132m_VFNMADDSSr132m_Int_VFNMADDSSr213m_VFNMADDSSr213m_Int_VFNMADDSSr231m_VFNMADDSSr231m_Int_VFNMSUBPD4mr_VFNMSUBPD4mrY_VFNMSUBPD4rm_VFNMSUBPD4rmY_VFNMSUBPDr132m_VFNMSUBPDr132mY_VFNMSUBPDr213m_VFNMSUBPDr213mY_VFNMSUBPDr231m_VFNMSUBPDr231mY_VFNMSUBPS4mr_VFNMSUBPS4mrY_VFNMSUBPS4rm_VFNMSUBPS4rmY_VFNMSUBPSr132m_VFNMSUBPSr132mY_VFNMSUBPSr213m_VFNMSUBPSr213mY_VFNMSUBPSr231m_VFNMSUBPSr231mY_VFNMSUBSD4mr_VFNMSUBSD4mr_Int_VFNMSUBSD4rm_VFNMSUBSD4rm_Int_VFNMSUBSDr132m_VFNMSUBSDr132m_Int_VFNMSUBSDr213m_VFNMSUBSDr213m_Int_VFNMSUBSDr231m_VFNMSUBSDr231m_Int_VFNMSUBSS4mr_VFNMSUBSS4mr_Int_VFNMSUBSS4rm_VFNMSUBSS4rm_Int_VFNMSUBSSr132m_VFNMSUBSSr132m_Int_VFNMSUBSSr213m_VFNMSUBSSr213m_Int_VFNMSUBSSr231m_VFNMSUBSSr231m_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #926
  {DBGFIELD("VSQRTPSYr")          1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #927
  {DBGFIELD("VSQRTPSYm")          1, 0, 0, 1093, 5, 23, 1,  0, 0}, // #928
  {DBGFIELD("VSQRTPDYr")          1, 0, 0, 1099, 4, 24, 1,  0, 0}, // #929
  {DBGFIELD("VSQRTPDYm")          1, 0, 0, 1093, 5, 23, 1,  0, 0}, // #930
  {DBGFIELD("RSQRTPSr_VRSQRTPSr") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #931
  {DBGFIELD("RSQRTSSr_VRSQRTSSr") 1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #932
  {DBGFIELD("RSQRTSSr_Int")       1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #933
  {DBGFIELD("VRSQRTSSr_Int")      1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #934
  {DBGFIELD("RSQRTPSm_VRSQRTPSm") 1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #935
  {DBGFIELD("RSQRTSSm_VRSQRTSSm") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #936
  {DBGFIELD("RSQRTSSm_Int_VRSQRTSSm_Int") 1, 0, 0, 1093, 5, 15, 1,  1, 1}, // #937
  {DBGFIELD("VRSQRTPSYr")         1, 0, 0, 1099, 4,  3, 1,  0, 0}, // #938
  {DBGFIELD("VRSQRTPSYm")         1, 0, 0, 1093, 5, 15, 1,  0, 0}, // #939
  {DBGFIELD("ANDNPDrr_ANDNPSrr_ANDPDrr_ANDPSrr_ORPDrr_ORPSrr_VANDNPDYrr_VANDNPDrr_VANDNPSYrr_VANDNPSrr_VANDPDYrr_VANDPDrr_VANDPSYrr_VANDPSrr_VORPDYrr_VORPDrr_VORPSYrr_VORPSrr_VXORPDYrr_VXORPDrr_VXORPSYrr_VXORPSrr_XORPDrr_XORPSrr") 1, 0, 0, 108, 2,  2, 1,  0, 0}, // #940
  {DBGFIELD("ANDNPDrm_ANDNPSrm_ANDPDrm_ANDPSrm_ORPDrm_ORPSrm_VANDNPDYrm_VANDNPDrm_VANDNPSYrm_VANDNPSrm_VANDPDYrm_VANDPDrm_VANDPSYrm_VANDPSrm_VORPDYrm_VORPDrm_VORPSYrm_VORPSrm_VXORPDYrm_VXORPDrm_VXORPSYrm_VXORPSrm_XORPDrm_XORPSrm") 1, 0, 0, 918, 3,  3, 1,  1, 1}, // #941
  {DBGFIELD("VZEROUPPER")         0, 0, 0,  0, 0,  0, 0,  0, 0}, // #942
  {DBGFIELD("VZEROALL")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #943
  {DBGFIELD("LDMXCSR_VLDMXCSR")   1, 0, 0, 282, 2,  9, 1,  0, 0}, // #944
  {DBGFIELD("STMXCSR_VSTMXCSR")   1, 0, 0, 442, 3,  2, 1,  0, 0} // #945
}; // SandyBridgeModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc SLMModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #7
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 386, 1,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 427, 2,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 1177, 3, 10, 1,  4, 1}, // #26
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 1177, 2, 10, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 1177, 3, 10, 1,  0, 0}, // #28
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 1177, 2, 10, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 1177, 3, 10, 1,  0, 0}, // #30
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 1177, 2, 10, 1,  0, 0}, // #31
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #32
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 13, 1,  2, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #35
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #37
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #39
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 930, 3,  9, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #55
  {DBGFIELD("WriteSystem")        1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #57
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 386, 1,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #64
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #66
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #67
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #72
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #73
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #75
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #76
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #78
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #79
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #98
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #99
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #100
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #102
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #103
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #105
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #107
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #109
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #110
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #111
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 1188, 4, 52, 1,  4, 1}, // #113
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #114
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #115
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 1188, 4, 52, 1,  0, 0}, // #116
  {DBGFIELD("WriteFDiv")          1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 1188, 4, 52, 1,  4, 1}, // #118
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #119
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #121
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #122
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #123
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #126
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #127
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #130
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #133
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #158
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #161
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #162
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #164
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #166
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #168
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1183, 3, 11, 1,  5, 2}, // #170
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #171
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #173
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #175
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #178
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #180
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #182
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #183
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #184
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #185
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #188
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #193
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #199
  {DBGFIELD("IIC_IRET_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #200
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #201
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #202
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #203
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #204
  {DBGFIELD("WriteLoad")          1, 0, 0, 14, 1,  6, 1,  0, 0}, // #205
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 429, 2,  2, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #208
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #212
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #215
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #216
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #218
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 14, 1,  6, 1,  0, 0}, // #223
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #224
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 386, 1,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 429, 2,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 14, 1,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #235
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #236
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #237
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #241
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #244
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #245
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #246
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #247
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 14, 1,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #251
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 13, 2,  5, 1,  0, 0}, // #252
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 13, 1,  9, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #265
  {DBGFIELD("WriteMove")          1, 0, 0, 386, 1,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 13, 2,  9, 1,  0, 0}, // #267
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #269
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #271
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #272
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #273
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #274
  {DBGFIELD("WriteShuffle")       1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #276
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecLogic") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #278
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecIMul") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #281
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #283
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #285
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #287
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #289
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecIMul") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #291
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #295
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #297
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #299
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 386, 1,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #306
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #310
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #311
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #313
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #320
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #325
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #328
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #330
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #332
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #335
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #336
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #340
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #342
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 13, 2,  9, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #346
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #347
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1180, 3,  8, 1,  4, 1}, // #348
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 1172, 2,  5, 1,  0, 0}, // #349
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #350
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #351
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #352
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #353
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #354
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 1198, 4, 10, 1,  4, 1}, // #358
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 1198, 4, 10, 1,  4, 1}, // #361
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #363
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 1205, 5, 45, 2,  0, 0}, // #364
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 1205, 4, 54, 2,  0, 0}, // #365
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 1198, 4, 10, 1,  0, 0}, // #366
  {DBGFIELD("WriteFMul")          1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #368
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #370
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #371
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 13, 2,  9, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #375
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #377
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #378
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #379
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #380
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #381
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #383
  {DBGFIELD("IIC_ALU_MEM_WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #384
  {DBGFIELD("IIC_ALU_NONMEM_WriteVarBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #385
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #386
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 1210, 3,  8, 1,  4, 1}, // #388
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 1210, 2,  8, 1,  0, 0}, // #389
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #390
  {DBGFIELD("WriteVecALU")        1, 0, 0, 13, 1,  2, 1,  0, 0}, // #391
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 1213, 3, 27, 1,  4, 1}, // #392
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 1213, 2, 27, 1,  0, 0}, // #393
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 1216, 3, 56, 1,  4, 1}, // #394
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 1216, 2, 56, 1,  0, 0}, // #395
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 1216, 3, 56, 1,  4, 1}, // #396
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 1216, 2, 56, 1,  0, 0}, // #397
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 1219, 3, 34, 1,  4, 1}, // #398
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 1219, 2, 34, 1,  0, 0}, // #399
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #400
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #401
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #402
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #404
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #405
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 1180, 3,  5, 1,  0, 0}, // #406
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #407
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #408
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #409
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #410
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #411
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #414
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #415
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #418
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #419
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #421
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #422
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #423
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #424
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #425
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #427
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #429
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #431
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #435
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #437
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #438
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_PUSH_MEM_WriteRMW") 1, 0, 0, 14, 1,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #442
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #443
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #444
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 492, 2,  2, 1,  0, 0}, // #447
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #450
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #451
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #452
  {DBGFIELD("WriteRMW")           1, 0, 0, 14, 1,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #455
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #456
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #458
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #459
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 930, 3,  9, 1,  0, 0}, // #460
  {DBGFIELD("WriteShift")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #463
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #464
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #465
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SSE_RSQRTPS_RM_WriteFRsqrtLd") 1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_RSQRTPS_RR_WriteFRsqrt") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SSE_RSQRTSS_RM_WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #469
  {DBGFIELD("WriteFRsqrtLd_ReadAfterLd") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #470
  {DBGFIELD("IIC_SSE_RSQRTSS_RR_WriteFRsqrt") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #471
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 930, 3,  9, 1,  5, 1}, // #472
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 1155, 2, 21, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #475
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 14, 1,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #482
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #483
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #486
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #487
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #490
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #494
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 1180, 3, 39, 1,  0, 0}, // #495
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 1180, 3, 39, 1,  0, 0}, // #497
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1180, 3, 39, 1,  4, 1}, // #499
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1180, 3, 39, 1,  4, 1}, // #500
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 1180, 3, 39, 1,  4, 1}, // #502
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #503
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #504
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #505
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 386, 1,  2, 1,  0, 0}, // #507
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #509
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #510
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #511
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 386, 1,  2, 1,  0, 0}, // #512
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #513
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #514
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #515
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #516
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 429, 2,  2, 1,  0, 0}, // #517
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #518
  {DBGFIELD("WriteJump")          1, 0, 0, 429, 2,  2, 1,  0, 0}, // #519
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #520
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #522
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_SSE_ALU_F64S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #524
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #525
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #526
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #527
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #528
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #529
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #530
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #531
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 13, 2,  5, 1,  0, 0}, // #532
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 13, 1,  9, 1,  0, 0}, // #533
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 13, 2,  5, 1,  0, 0}, // #534
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 13, 1,  9, 1,  0, 0}, // #535
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 792, 2, 25, 2,  0, 0}, // #536
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #537
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #538
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #539
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #540
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #541
  {DBGFIELD("WriteStore")         1, 0, 0, 386, 2,  2, 1,  0, 0}, // #542
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #543
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #544
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #545
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #546
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #547
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #548
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #549
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #550
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 1180, 3,  8, 1,  4, 1}, // #556
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 1172, 2,  5, 1,  0, 0}, // #557
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 13, 2,  9, 1,  0, 0}, // #558
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #559
  {DBGFIELD("WriteBlend")         1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #560
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #561
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #562
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #563
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #564
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 1210, 3,  8, 1,  4, 1}, // #565
  {DBGFIELD("WriteCLMul")         1, 0, 0, 1210, 2,  8, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #568
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #569
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #570
  {DBGFIELD("WriteVarVecShift_ReadAfterLd") 1, 0, 0, 1172, 2,  2, 1,  4, 1}, // #571
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #572
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #573
  {DBGFIELD("WriteVecShift")      1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #574
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #575
  {DBGFIELD("WriteFRcpLd")        1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #576
  {DBGFIELD("WriteFRsqrtLd")      1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #577
  {DBGFIELD("WriteFSqrtLd")       1, 0, 0, 1180, 3, 39, 1,  0, 0}, // #578
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #579
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #580
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #581
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #582
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #583
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #584
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #585
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #586
  {DBGFIELD("MOV16rm")            1, 0, 0, 14, 1,  6, 1,  0, 0}, // #587
  {DBGFIELD("MOVSX32rm16_MOVSX32rm8") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #588
  {DBGFIELD("MOVZX32rm16_MOVZX32rm8") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #589
  {DBGFIELD("CMOVA16rr_CMOVAE16rr_CMOVB16rr_CMOVBE16rr_CMOVE16rr_CMOVG16rr_CMOVGE16rr_CMOVL16rr_CMOVLE16rr_CMOVNE16rr_CMOVNO16rr_CMOVNP16rr_CMOVNS16rr_CMOVO16rr_CMOVP16rr_CMOVS16rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #590
  {DBGFIELD("CMOVA32rr_CMOVA64rr_CMOVAE32rr_CMOVAE64rr_CMOVB32rr_CMOVB64rr_CMOVBE32rr_CMOVBE64rr_CMOVE32rr_CMOVE64rr_CMOVG32rr_CMOVG64rr_CMOVGE32rr_CMOVGE64rr_CMOVL32rr_CMOVL64rr_CMOVLE32rr_CMOVLE64rr_CMOVNE32rr_CMOVNE64rr_CMOVNO32rr_CMOVNO64rr_CMOVNP32rr_CMOVNP64rr_CMOVNS32rr_CMOVNS64rr_CMOVO32rr_CMOVO64rr_CMOVP32rr_CMOVP64rr_CMOVS32rr_CMOVS64rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #591
  {DBGFIELD("CMOVA16rm_CMOVAE16rm_CMOVB16rm_CMOVBE16rm_CMOVE16rm_CMOVG16rm_CMOVGE16rm_CMOVL16rm_CMOVLE16rm_CMOVNE16rm_CMOVNO16rm_CMOVNP16rm_CMOVNS16rm_CMOVO16rm_CMOVP16rm_CMOVS16rm") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #592
  {DBGFIELD("CMOVA32rm_CMOVA64rm_CMOVAE32rm_CMOVAE64rm_CMOVB32rm_CMOVB64rm_CMOVBE32rm_CMOVBE64rm_CMOVE32rm_CMOVE64rm_CMOVG32rm_CMOVG64rm_CMOVGE32rm_CMOVGE64rm_CMOVL32rm_CMOVL64rm_CMOVLE32rm_CMOVLE64rm_CMOVNE32rm_CMOVNE64rm_CMOVNO32rm_CMOVNO64rm_CMOVNP32rm_CMOVNP64rm_CMOVNS32rm_CMOVNS64rm_CMOVO32rm_CMOVO64rm_CMOVP32rm_CMOVP64rm_CMOVS32rm_CMOVS64rm") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #593
  {DBGFIELD("XCHG16ar_XCHG16rr_XCHG32ar_XCHG32ar64_XCHG32rr_XCHG64ar_XCHG64rr_XCHG8rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #594
  {DBGFIELD("XCHG16rm_XCHG32rm_XCHG64rm_XCHG8rm") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #595
  {DBGFIELD("XLAT")               1, 0, 0, 14, 1,  6, 1,  0, 0}, // #596
  {DBGFIELD("PUSH16rmm_PUSH32rmm") 1, 0, 0, 14, 1,  2, 1,  0, 0}, // #597
  {DBGFIELD("PUSHF16_PUSHF32")    1, 0, 0, 386, 2,  2, 1,  0, 0}, // #598
  {DBGFIELD("PUSHA16_PUSHA32")    1, 0, 0, 386, 2,  2, 1,  0, 0}, // #599
  {DBGFIELD("POP16rmm_POP32rmm")  1, 0, 0, 14, 1,  6, 1,  0, 0}, // #600
  {DBGFIELD("POPF16")             1, 0, 0, 14, 1,  6, 1,  0, 0}, // #601
  {DBGFIELD("POPF32")             1, 0, 0, 14, 1,  6, 1,  0, 0}, // #602
  {DBGFIELD("POPA16_POPA32")      1, 0, 0, 14, 1,  6, 1,  0, 0}, // #603
  {DBGFIELD("LAHF_SAHF")          1, 0, 0, 386, 1,  2, 1,  0, 0}, // #604
  {DBGFIELD("BSWAP32r")           1, 0, 0, 386, 1,  2, 1,  0, 0}, // #605
  {DBGFIELD("BSWAP64r")           1, 0, 0, 386, 1,  2, 1,  0, 0}, // #606
  {DBGFIELD("MOVBE16rm_MOVBE64rm") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #607
  {DBGFIELD("MOVBE32rm")          1, 0, 0, 386, 2,  9, 1,  0, 0}, // #608
  {DBGFIELD("MOVBE16mr")          1, 0, 0, 386, 2,  2, 1,  0, 0}, // #609
  {DBGFIELD("MOVBE32mr")          1, 0, 0, 386, 2,  2, 1,  0, 0}, // #610
  {DBGFIELD("MOVBE64mr")          1, 0, 0, 386, 2,  2, 1,  0, 0}, // #611
  {DBGFIELD("ADD16mi_ADD16mi8_ADD16mr_ADD32mi_ADD32mi8_ADD32mr_ADD64mi32_ADD64mi8_ADD64mr_ADD8mi_ADD8mi8_ADD8mr_SUB16mi_SUB16mi8_SUB16mr_SUB32mi_SUB32mi8_SUB32mr_SUB64mi32_SUB64mi8_SUB64mr_SUB8mi_SUB8mi8_SUB8mr") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #612
  {DBGFIELD("ADC16ri_ADC16ri8_ADC16rr_ADC16rr_REV_ADC32ri_ADC32ri8_ADC32rr_ADC32rr_REV_ADC64ri32_ADC64ri8_ADC64rr_ADC64rr_REV_ADC8ri_ADC8ri8_ADC8rr_ADC8rr_REV_SBB16ri_SBB16ri8_SBB16rr_SBB16rr_REV_SBB32ri_SBB32ri8_SBB32rr_SBB32rr_REV_SBB64ri32_SBB64ri8_SBB64rr_SBB64rr_REV_SBB8ri_SBB8ri8_SBB8rr_SBB8rr_REV") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #613
  {DBGFIELD("ADC16rm_ADC32rm_ADC64rm_ADC8rm_SBB16rm_SBB32rm_SBB64rm_SBB8rm") 1, 0, 0, 386, 2,  9, 1,  4, 1}, // #614
  {DBGFIELD("ADC16mi_ADC16mi8_ADC16mr_ADC32mi_ADC32mi8_ADC32mr_ADC64mi32_ADC64mi8_ADC64mr_ADC8mi_ADC8mi8_ADC8mr_SBB16mi_SBB16mi8_SBB16mr_SBB32mi_SBB32mi8_SBB32mr_SBB64mi32_SBB64mi8_SBB64mr_SBB8mi_SBB8mi8_SBB8mr") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #615
  {DBGFIELD("DEC16m_DEC32m_DEC64m_DEC8m_INC16m_INC32m_INC64m_INC8m_NEG16m_NEG32m_NEG64m_NEG8m_NOT16m_NOT32m_NOT64m_NOT8m") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #616
  {DBGFIELD("IMUL16r_IMUL16rr")   1, 0, 0, 429, 2,  6, 1,  0, 0}, // #617
  {DBGFIELD("IMUL16rm")           1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #618
  {DBGFIELD("IMUL16rmi_IMUL16rmi8") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #619
  {DBGFIELD("IMUL16rri_IMUL16rri8") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #620
  {DBGFIELD("MUL16r")             1, 0, 0, 429, 2,  6, 1,  0, 0}, // #621
  {DBGFIELD("IMUL16m")            1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #622
  {DBGFIELD("MUL16m")             1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #623
  {DBGFIELD("IMUL32r_IMUL32rr")   1, 0, 0, 429, 2,  6, 1,  0, 0}, // #624
  {DBGFIELD("IMUL32rm")           1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #625
  {DBGFIELD("IMUL32rmi_IMUL32rmi8") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #626
  {DBGFIELD("IMUL32rri_IMUL32rri8") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #627
  {DBGFIELD("MUL32r")             1, 0, 0, 429, 2,  6, 1,  0, 0}, // #628
  {DBGFIELD("IMUL32m")            1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #629
  {DBGFIELD("MUL32m")             1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #630
  {DBGFIELD("IMUL64r_IMUL64rr")   1, 0, 0, 429, 2,  6, 1,  0, 0}, // #631
  {DBGFIELD("IMUL64rm")           1, 0, 0, 1081, 3, 16, 1,  4, 1}, // #632
  {DBGFIELD("IMUL64rmi32_IMUL64rmi8") 1, 0, 0, 1081, 3, 16, 1,  0, 0}, // #633
  {DBGFIELD("IMUL64rri32_IMUL64rri8") 1, 0, 0, 429, 2,  6, 1,  0, 0}, // #634
  {DBGFIELD("MUL64r")             1, 0, 0, 429, 2,  6, 1,  0, 0}, // #635
  {DBGFIELD("IMUL64m")            1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #636
  {DBGFIELD("MUL64m")             1, 0, 0, 1081, 3, 16, 1,  5, 2}, // #637
  {DBGFIELD("MULX32rr")           2, 0, 0, 1205, 4, 54, 2,  0, 0}, // #638
  {DBGFIELD("MULX32rm")           2, 0, 0, 1205, 5, 45, 2,  0, 0}, // #639
  {DBGFIELD("MULX64rr")           2, 0, 0, 1205, 4, 54, 2,  0, 0}, // #640
  {DBGFIELD("MULX64rm")           2, 0, 0, 1205, 5, 45, 2,  0, 0}, // #641
  {DBGFIELD("DIV8r")              1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #642
  {DBGFIELD("DIV16r")             1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #643
  {DBGFIELD("DIV32r")             1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #644
  {DBGFIELD("DIV64r")             1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #645
  {DBGFIELD("IDIV8r")             1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #646
  {DBGFIELD("IDIV16r")            1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #647
  {DBGFIELD("IDIV32r")            1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #648
  {DBGFIELD("IDIV64r")            1, 0, 0, 1186, 2, 12, 1,  0, 0}, // #649
  {DBGFIELD("AND16mi_AND16mi8_AND16mr_AND32mi_AND32mi8_AND32mr_AND64mi32_AND64mi8_AND64mr_AND8mi_AND8mi8_AND8mr_OR16mi_OR16mi8_OR16mr_OR32mi_OR32mi8_OR32mr_OR64mi32_OR64mi8_OR64mr_OR8mi_OR8mi8_OR8mr_XOR16mi_XOR16mi8_XOR16mr_XOR32mi_XOR32mi8_XOR32mr_XOR64mi32_XOR64mi8_XOR64mr_XOR8mi_XOR8mi8_XOR8mr") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #650
  {DBGFIELD("OR32mrLocked")       2, 0, 0, 430, 2, 41, 2,  0, 0}, // #651
  {DBGFIELD("SAR16m1_SAR16mi_SAR32m1_SAR32mi_SAR64m1_SAR64mi_SAR8m1_SAR8mi_SHL16m1_SHL16mi_SHL32m1_SHL32mi_SHL64m1_SHL64mi_SHL8m1_SHL8mi_SHR16m1_SHR16mi_SHR32m1_SHR32mi_SHR64m1_SHR64mi_SHR8m1_SHR8mi") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #652
  {DBGFIELD("SAR16rCL_SAR32rCL_SAR64rCL_SAR8rCL_SHL16rCL_SHL32rCL_SHL64rCL_SHL8rCL_SHR16rCL_SHR32rCL_SHR64rCL_SHR8rCL") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #653
  {DBGFIELD("SAR16mCL_SAR32mCL_SAR64mCL_SAR8mCL_SHL16mCL_SHL32mCL_SHL64mCL_SHL8mCL_SHR16mCL_SHR32mCL_SHR64mCL_SHR8mCL") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #654
  {DBGFIELD("ROL16r1_ROL32r1_ROL64r1_ROL8r1_ROR16r1_ROR32r1_ROR64r1_ROR8r1") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #655
  {DBGFIELD("ROL16mi_ROL32mi_ROL64mi_ROL8mi_ROR16mi_ROR32mi_ROR64mi_ROR8mi") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #656
  {DBGFIELD("ROL16rCL_ROL32rCL_ROL64rCL_ROL8rCL_ROR16rCL_ROR32rCL_ROR64rCL_ROR8rCL") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #657
  {DBGFIELD("ROL16mCL_ROL32mCL_ROL64mCL_ROL8mCL_ROR16mCL_ROR32mCL_ROR64mCL_ROR8mCL") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #658
  {DBGFIELD("RCL16r1_RCL32r1_RCL64r1_RCL8r1_RCR16r1_RCR32r1_RCR64r1_RCR8r1") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #659
  {DBGFIELD("RCL16m1_RCL32m1_RCL64m1_RCL8m1_RCR16m1_RCR32m1_RCR64m1_RCR8m1") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #660
  {DBGFIELD("RCL16rCL_RCL16ri_RCL32rCL_RCL32ri_RCL64rCL_RCL64ri_RCL8rCL_RCL8ri_RCR16rCL_RCR16ri_RCR32rCL_RCR32ri_RCR64rCL_RCR64ri_RCR8rCL_RCR8ri") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #661
  {DBGFIELD("RCL16mCL_RCL16mi_RCL32mCL_RCL32mi_RCL64mCL_RCL64mi_RCL8mCL_RCL8mi_RCR16mCL_RCR16mi_RCR32mCL_RCR32mi_RCR64mCL_RCR64mi_RCR8mCL_RCR8mi") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #662
  {DBGFIELD("SHLD16rri8_SHRD16rri8") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #663
  {DBGFIELD("SHLD32rri8_SHRD32rri8") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #664
  {DBGFIELD("SHLD64rri8_SHRD64rri8") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #665
  {DBGFIELD("SHLD16mri8_SHRD16mri8") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #666
  {DBGFIELD("SHLD32mri8_SHRD32mri8") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #667
  {DBGFIELD("SHLD64mri8_SHRD64mri8") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #668
  {DBGFIELD("SHLD16rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #669
  {DBGFIELD("SHLD32rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #670
  {DBGFIELD("SHLD64rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #671
  {DBGFIELD("SHRD16rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #672
  {DBGFIELD("SHRD32rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #673
  {DBGFIELD("SHRD64rrCL")         1, 0, 0, 492, 2,  2, 1,  0, 0}, // #674
  {DBGFIELD("SHLD16mrCL_SHRD16mrCL") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #675
  {DBGFIELD("SHLD32mrCL_SHRD32mrCL") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #676
  {DBGFIELD("SHLD64mrCL_SHRD64mrCL") 2, 0, 0, 1222, 3, 41, 2,  0, 0}, // #677
  {DBGFIELD("BT16ri8_BT32ri8_BT64ri8") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #678
  {DBGFIELD("BT16rr_BT32rr_BT64rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #679
  {DBGFIELD("BT16mr_BT32mr_BT64mr") 1, 0, 0, 386, 2,  9, 1,  0, 0}, // #680
  {DBGFIELD("BT16mi8_BT32mi8_BT64mi8") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #681
  {DBGFIELD("BTC16ri8_BTC32ri8_BTC64ri8_BTR16ri8_BTR32ri8_BTR64ri8_BTS16ri8_BTS32ri8_BTS64ri8") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #682
  {DBGFIELD("BTC16rr_BTC32rr_BTC64rr_BTR16rr_BTR32rr_BTS16rr_BTS32rr_BTS64rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #683
  {DBGFIELD("BTR64rr")            1, 0, 0, 386, 1,  2, 1,  0, 0}, // #684
  {DBGFIELD("BTC16mr_BTC32mr_BTC64mr_BTR16mr_BTR32mr_BTR64mr_BTS16mr_BTS32mr_BTS64mr") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #685
  {DBGFIELD("BTC16mi8_BTC32mi8_BTC64mi8_BTR16mi8_BTR32mi8_BTR64mi8_BTS16mi8_BTS32mi8_BTS64mi8") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #686
  {DBGFIELD("BSF16rr_BSF32rr_BSF64rr_BSR16rr_BSR32rr_BSR64rr") 1, 0, 0, 492, 2,  2, 1,  0, 0}, // #687
  {DBGFIELD("BSF16rm_BSF32rm_BSF64rm_BSR16rm_BSR32rm_BSR64rm") 1, 0, 0, 930, 3,  9, 1,  0, 0}, // #688
  {DBGFIELD("SETAEr_SETAr_SETBEr_SETBr_SETEr_SETGEr_SETGr_SETLEr_SETLr_SETNEr_SETNOr_SETNPr_SETNSr_SETOr_SETPr_SETSr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #689
  {DBGFIELD("SETAEm_SETAm_SETBEm_SETBm_SETEm_SETGEm_SETGm_SETLEm_SETLm_SETNEm_SETNOm_SETNPm_SETNSm_SETOm_SETPm_SETSm") 2, 0, 0, 1155, 2, 21, 2,  0, 0}, // #690
  {DBGFIELD("CLD")                1, 0, 0, 386, 1,  2, 1,  0, 0}, // #691
  {DBGFIELD("STD")                1, 0, 0, 386, 1,  2, 1,  0, 0}, // #692
  {DBGFIELD("TZCNT16rr_TZCNT32rr_TZCNT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #693
  {DBGFIELD("TZCNT16rm_TZCNT32rm_TZCNT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #694
  {DBGFIELD("ANDN32rr_ANDN64rr")  1, 0, 0, 386, 1,  2, 1,  0, 0}, // #695
  {DBGFIELD("ANDN32rm_ANDN64rm")  1, 0, 0, 386, 2,  9, 1,  4, 1}, // #696
  {DBGFIELD("BLSI32rr_BLSI64rr_BLSMSK32rr_BLSMSK64rr_BLSR32rr_BLSR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #697
  {DBGFIELD("BLSI32rm_BLSI64rm_BLSMSK32rm_BLSMSK64rm_BLSR32rm_BLSR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #698
  {DBGFIELD("BEXTR32rr_BEXTR64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #699
  {DBGFIELD("BEXTR32rm_BEXTR64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #700
  {DBGFIELD("BZHI32rr_BZHI64rr")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #701
  {DBGFIELD("BZHI32rm_BZHI64rm")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #702
  {DBGFIELD("PDEP32rr_PDEP64rr_PEXT32rr_PEXT64rr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #703
  {DBGFIELD("PDEP32rm_PDEP64rm_PEXT32rm_PEXT64rm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #704
  {DBGFIELD("JCXZ_JRCXZ")         1, 0, 0, 429, 2,  2, 1,  0, 0}, // #705
  {DBGFIELD("LOOP")               1, 0, 0, 429, 2,  2, 1,  0, 0}, // #706
  {DBGFIELD("LOOPE")              1, 0, 0, 429, 2,  2, 1,  0, 0}, // #707
  {DBGFIELD("LOOPNE")             1, 0, 0, 429, 2,  2, 1,  0, 0}, // #708
  {DBGFIELD("CALL16r_CALL32r")    1, 0, 0, 429, 2,  2, 1,  0, 0}, // #709
  {DBGFIELD("CALL16m_CALL32m")    1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #710
  {DBGFIELD("LRETL_LRETQ_LRETW_RETL_RETQ_RETW") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #711
  {DBGFIELD("LRETIL_LRETIQ_LRETIW") 1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #712
  {DBGFIELD("RETIL_RETIQ_RETIW")  1, 0, 0, 1081, 3,  9, 1,  0, 0}, // #713
  {DBGFIELD("BOUNDS16rm_BOUNDS32rm") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #714
  {DBGFIELD("INTO")               1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #715
  {DBGFIELD("LODSB_LODSW")        1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #716
  {DBGFIELD("LODSL_LODSQ")        1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #717
  {DBGFIELD("STOSB_STOSL_STOSQ_STOSW") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #718
  {DBGFIELD("MOVSB_MOVSL_MOVSQ_MOVSW") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #719
  {DBGFIELD("MOVSLDUPrm")         1, 0, 0, 14, 1,  6, 1,  0, 0}, // #720
  {DBGFIELD("MOVSLDUPrr")         1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #721
  {DBGFIELD("SCASB_SCASL_SCASQ_SCASW") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #722
  {DBGFIELD("CMPSB_CMPSL_CMPSQ_CMPSW") 1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #723
  {DBGFIELD("XADD16rm_XADD32rm_XADD64rm_XADD8rm") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #724
  {DBGFIELD("CMPXCHG16rm_CMPXCHG32rm_CMPXCHG64rm") 2, 0, 0, 430, 2, 41, 2,  0, 0}, // #725
  {DBGFIELD("CMPXCHG8rm")         2, 0, 0, 430, 2, 41, 2,  0, 0}, // #726
  {DBGFIELD("CMPXCHG8B")          2, 0, 0, 430, 2, 41, 2,  0, 0}, // #727
  {DBGFIELD("CMPXCHG16B")         2, 0, 0, 430, 2, 41, 2,  0, 0}, // #728
  {DBGFIELD("PAUSE")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #729
  {DBGFIELD("LEAVE_LEAVE64")      1, 0, 0, 386, 1,  2, 1,  0, 0}, // #730
  {DBGFIELD("XGETBV")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #731
  {DBGFIELD("RDTSC")              1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #732
  {DBGFIELD("RDTSCP")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #733
  {DBGFIELD("RDPMC")              1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #734
  {DBGFIELD("RDRAND16r_RDRAND32r_RDRAND64r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #735
  {DBGFIELD("LD_Frr")             1, 0, 0, 386, 1,  2, 1,  0, 0}, // #736
  {DBGFIELD("LD_F80m")            1, 0, 0, 14, 1,  6, 1,  0, 0}, // #737
  {DBGFIELD("FBLDm")              0, 0, 0,  0, 0,  0, 0,  0, 0}, // #738
  {DBGFIELD("ST_FPrr_ST_Frr")     1, 0, 0, 386, 1,  2, 1,  0, 0}, // #739
  {DBGFIELD("ST_FP80m")           1, 0, 0, 386, 2,  2, 1,  0, 0}, // #740
  {DBGFIELD("FBSTPm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #741
  {DBGFIELD("XCH_F")              1, 0, 0, 386, 1,  2, 1,  0, 0}, // #742
  {DBGFIELD("ILD_F16m_ILD_F32m_ILD_F64m") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #743
  {DBGFIELD("IST_F16m_IST_F32m_IST_FP16m_IST_FP32m") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #744
  {DBGFIELD("LD_F0")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #745
  {DBGFIELD("LD_F1")              1, 0, 0,  0, 0,  2, 1,  0, 0}, // #746
  {DBGFIELD("FLDPI")              1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #747
  {DBGFIELD("CMOVBE_F_CMOVBE_Fp32_CMOVBE_Fp64_CMOVBE_Fp80_CMOVB_F_CMOVB_Fp32_CMOVB_Fp64_CMOVB_Fp80_CMOVNBE_F_CMOVNBE_Fp32_CMOVNBE_Fp64_CMOVNBE_Fp80_CMOVNB_F_CMOVNB_Fp32_CMOVNB_Fp64_CMOVNB_Fp80_CMOVNE_F_CMOVNE_Fp32_CMOVNE_Fp64_CMOVNE_Fp80_CMOVNP_F_CMOVNP_Fp32_CMOVNP_Fp64_CMOVNP_Fp80_CMOVP_F_CMOVP_Fp32_CMOVP_Fp64_CMOVP_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #748
  {DBGFIELD("FNSTSW16r")          1, 0, 0, 386, 1,  2, 1,  0, 0}, // #749
  {DBGFIELD("FNSTSWm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #750
  {DBGFIELD("FLDCW16m")           1, 0, 0, 14, 1,  6, 1,  0, 0}, // #751
  {DBGFIELD("FNSTCW16m")          1, 0, 0, 386, 1,  2, 1,  0, 0}, // #752
  {DBGFIELD("FDECSTP_FINCSTP")    1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #753
  {DBGFIELD("FFREE")              1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #754
  {DBGFIELD("FSAVEm")             0, 0, 0,  0, 0,  0, 0,  0, 0}, // #755
  {DBGFIELD("FRSTORm")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #756
  {DBGFIELD("ABS_F_ABS_Fp32_ABS_Fp64_ABS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #757
  {DBGFIELD("CHS_F_CHS_Fp32_CHS_Fp64_CHS_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #758
  {DBGFIELD("COMP_FST0r_COM_FST0r") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #759
  {DBGFIELD("UCOM_FPr_UCOM_Fr")   1, 0, 0, 427, 2,  6, 1,  0, 0}, // #760
  {DBGFIELD("FCOM32m_FCOM64m_FCOMP32m_FCOMP64m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #761
  {DBGFIELD("FCOMPP")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #762
  {DBGFIELD("UCOM_FPPr")          1, 0, 0, 427, 2,  6, 1,  0, 0}, // #763
  {DBGFIELD("COM_FIPr_COM_FIr")   1, 0, 0, 427, 2,  6, 1,  0, 0}, // #764
  {DBGFIELD("UCOM_FIPr_UCOM_FIr") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #765
  {DBGFIELD("FICOM16m_FICOM32m_FICOMP16m_FICOMP32m") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #766
  {DBGFIELD("TST_F_TST_Fp32_TST_Fp64_TST_Fp80") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #767
  {DBGFIELD("FXAM")               1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #768
  {DBGFIELD("FPREM")              1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #769
  {DBGFIELD("FPREM1")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #770
  {DBGFIELD("FRNDINT")            1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #771
  {DBGFIELD("FSCALE")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #772
  {DBGFIELD("FXTRACT")            1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #773
  {DBGFIELD("FNOP")               1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #774
  {DBGFIELD("WAIT")               1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #775
  {DBGFIELD("FNCLEX")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #776
  {DBGFIELD("FNINIT")             1, 0, 0, 1172, 2,  1, 1,  0, 0}, // #777
  {DBGFIELD("MMX_MOVD64from64rr_MMX_MOVD64grr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #778
  {DBGFIELD("MOVPDI2DIrr_VMOVPDI2DIrr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #779
  {DBGFIELD("MMX_MOVD64rr_MMX_MOVD64to64rr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #780
  {DBGFIELD("MOVDI2PDIrr_VMOVDI2PDIrr") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #781
  {DBGFIELD("VMOVPQIto64rr")      1, 0, 0, 386, 1,  2, 1,  0, 0}, // #782
  {DBGFIELD("VMOV64toPQIrr")      1, 0, 0, 386, 1,  2, 1,  0, 0}, // #783
  {DBGFIELD("MMX_MOVQ64rr_MMX_MOVQ64rr_REV") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #784
  {DBGFIELD("MOVDQArr_MOVDQArr_REV_VMOVDQAYrr_VMOVDQAYrr_REV_VMOVDQArr_VMOVDQArr_REV") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #785
  {DBGFIELD("MOVDQUrr_MOVDQUrr_REV_VMOVDQUYrr_VMOVDQUYrr_REV_VMOVDQUrr_VMOVDQUrr_REV") 1, 0, 0, 386, 1,  2, 1,  0, 0}, // #786
  {DBGFIELD("MMX_MOVDQ2Qrr")      1, 0, 0, 386, 1,  2, 1,  0, 0}, // #787
  {DBGFIELD("MMX_MOVQ2DQrr")      1, 0, 0, 386, 1,  2, 1,  0, 0}, // #788
  {DBGFIELD("MMX_PACKSSDWirr_MMX_PACKSSWBirr_MMX_PACKUSWBirr") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #789
  {DBGFIELD("MMX_PACKSSDWirm_MMX_PACKSSWBirm_MMX_PACKUSWBirm") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #790
  {DBGFIELD("VPMOVSXBQYrr_VPMOVSXBWYrr_VPMOVSXDQYrr_VPMOVZXBQYrr_VPMOVZXBWYrr_VPMOVZXDQYrr") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #791
  {DBGFIELD("PBLENDWrri")         1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #792
  {DBGFIELD("VPBLENDWYrri_VPBLENDWrri") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #793
  {DBGFIELD("PBLENDWrmi")         1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #794
  {DBGFIELD("VPBLENDWYrmi_VPBLENDWrmi") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #795
  {DBGFIELD("VPBLENDDYrri_VPBLENDDrri") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #796
  {DBGFIELD("VPBLENDDYrmi_VPBLENDDrmi") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #797
  {DBGFIELD("MMX_MASKMOVQ_MMX_MASKMOVQ64") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #798
  {DBGFIELD("MASKMOVDQU_MASKMOVDQU64_VMASKMOVDQU_VMASKMOVDQU64") 1, 0, 0, 386, 2,  2, 1,  0, 0}, // #799
  {DBGFIELD("VPMASKMOVDYrm_VPMASKMOVDrm_VPMASKMOVQYrm_VPMASKMOVQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #800
  {DBGFIELD("VPMASKMOVDYmr_VPMASKMOVDmr_VPMASKMOVQYmr_VPMASKMOVQmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #801
  {DBGFIELD("MMX_PMOVMSKBrr")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #802
  {DBGFIELD("PMOVMSKBrr_VPMOVMSKBrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #803
  {DBGFIELD("VPMOVMSKBYrr")       1, 0, 0, 13, 1,  2, 1,  0, 0}, // #804
  {DBGFIELD("MMX_PEXTRWirri")     1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #805
  {DBGFIELD("PEXTRBrr_PEXTRDrr_PEXTRQrr_PEXTRWrr_REV") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #806
  {DBGFIELD("PEXTRBmr_PEXTRDmr_PEXTRQmr_PEXTRWmr") 2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #807
  {DBGFIELD("VPBROADCASTBrm_VPBROADCASTWrm") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #808
  {DBGFIELD("VPBROADCASTBYrm_VPBROADCASTWYrm") 1, 0, 0, 14, 1,  6, 1,  0, 0}, // #809
  {DBGFIELD("VPGATHERDDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #810
  {DBGFIELD("VPGATHERDDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #811
  {DBGFIELD("VPGATHERQDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #812
  {DBGFIELD("VPGATHERQDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #813
  {DBGFIELD("VPGATHERDQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #814
  {DBGFIELD("VPGATHERDQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #815
  {DBGFIELD("VPGATHERQQrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #816
  {DBGFIELD("VPGATHERQQYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #817
  {DBGFIELD("MMX_PHADDSWrr64_MMX_PHADDWrr64_MMX_PHSUBSWrr64_MMX_PHSUBWrr64") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #818
  {DBGFIELD("MMX_PHADDrr64_MMX_PHSUBDrr64") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #819
  {DBGFIELD("PHADDDrr_PHSUBDrr_VPHADDDrr_VPHSUBDrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #820
  {DBGFIELD("PHADDSWrr128_PHSUBSWrr128_VPHADDSWrr128_VPHADDSWrr256_VPHSUBSWrr128_VPHSUBSWrr256") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #821
  {DBGFIELD("PHADDWrr_PHSUBWrr_VPHADDDYrr_VPHADDWYrr_VPHADDWrr_VPHSUBDYrr_VPHSUBWYrr_VPHSUBWrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #822
  {DBGFIELD("MMX_PHADDSWrm64_MMX_PHADDWrm64_MMX_PHSUBSWrm64_MMX_PHSUBWrm64") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #823
  {DBGFIELD("MMX_PHADDrm64_MMX_PHSUBDrm64") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #824
  {DBGFIELD("PHADDDrm_PHSUBDrm_VPHADDDrm_VPHSUBDrm") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #825
  {DBGFIELD("PHADDSWrm128_PHSUBSWrm128_VPHADDSWrm128_VPHADDSWrm256_VPHSUBSWrm128_VPHSUBSWrm256") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #826
  {DBGFIELD("PHADDWrm_PHSUBWrm_VPHADDDYrm_VPHADDWYrm_VPHADDWrm_VPHSUBDYrm_VPHSUBWYrm_VPHSUBWrm") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #827
  {DBGFIELD("PCMPGTQrr_VPCMPGTQYrr_VPCMPGTQrr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #828
  {DBGFIELD("PCMPGTQrm_VPCMPGTQYrm_VPCMPGTQrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #829
  {DBGFIELD("PMULLDrr_VPMULLDYrr_VPMULLDrr") 1, 0, 0, 1172, 2,  9, 1,  0, 0}, // #830
  {DBGFIELD("PMULLDrm_VPMULLDYrm_VPMULLDrm") 1, 0, 0, 1180, 3,  5, 1,  4, 1}, // #831
  {DBGFIELD("PTESTrr_VPTESTYrr_VPTESTrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #832
  {DBGFIELD("PTESTrm_VPTESTYrm_VPTESTrm") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #833
  {DBGFIELD("PSLLDrr_PSLLQrr_PSLLWrr_PSRADrr_PSRAWrr_PSRLDrr_PSRLQrr_PSRLWrr_VPSLLDYrr_VPSLLDrr_VPSLLQYrr_VPSLLQrr_VPSLLWYrr_VPSLLWrr_VPSRADYrr_VPSRADrr_VPSRAWYrr_VPSRAWrr_VPSRLDYrr_VPSRLDrr_VPSRLQYrr_VPSRLQrr_VPSRLWYrr_VPSRLWrr") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #834
  {DBGFIELD("PSLLDQri_PSRLDQri")  1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #835
  {DBGFIELD("VPSLLDQYri_VPSLLDQri_VPSRLDQYri_VPSRLDQri") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #836
  {DBGFIELD("MMX_EMMS")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #837
  {DBGFIELD("MOVMSKPDrr_MOVMSKPSrr_VMOVMSKPDrr_VMOVMSKPSrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #838
  {DBGFIELD("VMOVMSKPDYrr_VMOVMSKPSYrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #839
  {DBGFIELD("VPERM2F128rr")       1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #840
  {DBGFIELD("VPERM2F128rm")       1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #841
  {DBGFIELD("BLENDVPDrr0_BLENDVPSrr0") 1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #842
  {DBGFIELD("BLENDVPDrm0_BLENDVPSrm0") 1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #843
  {DBGFIELD("VBROADCASTF128")     1, 0, 0, 1180, 3,  9, 1,  0, 0}, // #844
  {DBGFIELD("EXTRACTPSrr")        1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #845
  {DBGFIELD("VEXTRACTPSrr")       1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #846
  {DBGFIELD("EXTRACTPSmr")        2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #847
  {DBGFIELD("VEXTRACTPSmr")       2, 0, 0, 1195, 3, 41, 2,  0, 0}, // #848
  {DBGFIELD("VEXTRACTF128rr")     1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #849
  {DBGFIELD("VEXTRACTF128mr")     1, 0, 0, 386, 2,  2, 1,  0, 0}, // #850
  {DBGFIELD("VINSERTF128rr")      1, 0, 0, 1172, 2,  2, 1,  0, 0}, // #851
  {DBGFIELD("VINSERTF128rm")      1, 0, 0, 1180, 3,  9, 1,  4, 1}, // #852
  {DBGFIELD("VMASKMOVPDYrm_VMASKMOVPDrm_VMASKMOVPSYrm_VMASKMOVPSrm") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #853
  {DBGFIELD("VMASKMOVPDmr_VMASKMOVPSmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #854
  {DBGFIELD("VMASKMOVPDYmr_VMASKMOVPSYmr") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #855
  {DBGFIELD("VGATHERDPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #856
  {DBGFIELD("VGATHERDPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #857
  {DBGFIELD("VGATHERQPSrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #858
  {DBGFIELD("VGATHERQPSYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #859
  {DBGFIELD("VGATHERDPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #860
  {DBGFIELD("VGATHERDPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #861
  {DBGFIELD("VGATHERQPDrm")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #862
  {DBGFIELD("VGATHERQPDYrm")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #863
  {DBGFIELD("CVTPD2PSrr_VCVTPD2PSrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #864
  {DBGFIELD("CVTPD2PSrm_VCVTPD2PSXrm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #865
  {DBGFIELD("VCVTPD2PSYrr")       1, 0, 0, 13, 1,  9, 1,  0, 0}, // #866
  {DBGFIELD("VCVTPD2PSYrm")       1, 0, 0, 13, 2,  5, 1,  0, 0}, // #867
  {DBGFIELD("CVTSD2SSrr_Int_CVTSD2SSrr_Int_VCVTSD2SSrr_VCVTSD2SSrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #868
  {DBGFIELD("CVTSD2SSrm")         1, 0, 0, 13, 2,  5, 1,  0, 0}, // #869
  {DBGFIELD("Int_CVTSD2SSrm_Int_VCVTSD2SSrm_VCVTSD2SSrm") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #870
  {DBGFIELD("CVTPS2PDrr_VCVTPS2PDrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #871
  {DBGFIELD("CVTPS2PDrm_VCVTPS2PDYrm_VCVTPS2PDrm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #872
  {DBGFIELD("VCVTPS2PDYrr")       1, 0, 0, 13, 1,  9, 1,  0, 0}, // #873
  {DBGFIELD("CVTSS2SDrr_Int_CVTSS2SDrr_Int_VCVTSS2SDrr_VCVTSS2SDrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #874
  {DBGFIELD("CVTSS2SDrm")         1, 0, 0, 13, 2,  5, 1,  0, 0}, // #875
  {DBGFIELD("Int_CVTSS2SDrm_Int_VCVTSS2SDrm_VCVTSS2SDrm") 1, 0, 0, 13, 2,  5, 1,  4, 1}, // #876
  {DBGFIELD("CVTDQ2PDrr")         1, 0, 0, 13, 1,  9, 1,  0, 0}, // #877
  {DBGFIELD("VCVTDQ2PDrr")        1, 0, 0, 13, 1,  9, 1,  0, 0}, // #878
  {DBGFIELD("VCVTDQ2PDYrr")       1, 0, 0, 13, 1,  9, 1,  0, 0}, // #879
  {DBGFIELD("CVTPD2DQrr_CVTTPD2DQrr_VCVTTPD2DQrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #880
  {DBGFIELD("VCVTPD2DQrr")        1, 0, 0, 13, 1,  9, 1,  0, 0}, // #881
  {DBGFIELD("CVTPD2DQrm_CVTTPD2DQrm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #882
  {DBGFIELD("VCVTPD2DQYrr")       1, 0, 0, 13, 1,  9, 1,  0, 0}, // #883
  {DBGFIELD("VCVTTPD2DQYrr")      1, 0, 0, 13, 1,  9, 1,  0, 0}, // #884
  {DBGFIELD("VCVTPD2DQYrm")       1, 0, 0, 13, 2,  5, 1,  0, 0}, // #885
  {DBGFIELD("VCVTTPD2DQYrm")      1, 0, 0, 13, 2,  5, 1,  0, 0}, // #886
  {DBGFIELD("MMX_CVTPS2PIirr_MMX_CVTTPS2PIirr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #887
  {DBGFIELD("MMX_CVTPI2PDirr")    1, 0, 0, 13, 1,  9, 1,  0, 0}, // #888
  {DBGFIELD("MMX_CVTPD2PIirr_MMX_CVTTPD2PIirr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #889
  {DBGFIELD("CVTSI2SS64rr_CVTSI2SSrr_Int_CVTSI2SS64rr_Int_CVTSI2SSrr_Int_VCVTSI2SS64rr_Int_VCVTSI2SSrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #890
  {DBGFIELD("VCVTSI2SS64rr_VCVTSI2SSrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #891
  {DBGFIELD("CVTSS2SI64rr_CVTTSS2SI64rr_Int_CVTTSS2SI64rr_Int_VCVTTSS2SI64rr_VCVTSS2SI64rr_VCVTTSS2SI64rr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #892
  {DBGFIELD("CVTSS2SIrr_CVTTSS2SIrr_Int_CVTTSS2SIrr_Int_VCVTTSS2SIrr_VCVTSS2SIrr_VCVTTSS2SIrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #893
  {DBGFIELD("CVTSS2SI64rm_CVTTSS2SI64rm_Int_CVTTSS2SI64rm_Int_VCVTTSS2SI64rm_VCVTSS2SI64rm_VCVTTSS2SI64rm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #894
  {DBGFIELD("CVTSS2SIrm_CVTTSS2SIrm_Int_CVTTSS2SIrm_Int_VCVTTSS2SIrm_VCVTSS2SIrm_VCVTTSS2SIrm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #895
  {DBGFIELD("CVTSD2SI64rr_CVTSD2SIrr_CVTTSD2SI64rr_CVTTSD2SIrr_Int_CVTTSD2SI64rr_Int_CVTTSD2SIrr_Int_VCVTTSD2SI64rr_Int_VCVTTSD2SIrr_VCVTSD2SI64rr_VCVTSD2SIrr_VCVTTSD2SI64rr_VCVTTSD2SIrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #896
  {DBGFIELD("CVTSD2SI64rm_CVTSD2SIrm_CVTTSD2SI64rm_CVTTSD2SIrm_Int_CVTTSD2SI64rm_Int_CVTTSD2SIrm_Int_VCVTTSD2SI64rm_Int_VCVTTSD2SIrm_VCVTSD2SI64rm_VCVTSD2SIrm_VCVTTSD2SI64rm_VCVTTSD2SIrm") 1, 0, 0, 13, 2,  5, 1,  0, 0}, // #897
  {DBGFIELD("VCVTPS2PHYrr_VCVTPS2PHrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #898
  {DBGFIELD("VCVTPS2PHYmr_VCVTPS2PHmr") 2, 0, 0, 792, 2, 25, 2,  0, 0}, // #899
  {DBGFIELD("VCVTPH2PSYrr_VCVTPH2PSrr") 1, 0, 0, 13, 1,  9, 1,  0, 0}, // #900
  {DBGFIELD("HADDPDrr_HADDPSrr_HSUBPDrr_HSUBPSrr_VHADDPDYrr_VHADDPDrr_VHADDPSYrr_VHADDPSrr_VHSUBPDYrr_VHSUBPDrr_VHSUBPSYrr_VHSUBPSrr") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #901
  {DBGFIELD("HADDPDrm_HADDPSrm_HSUBPDrm_HSUBPSrm_VHADDPDYrm_VHADDPDrm_VHADDPSYrm_VHADDPSrm_VHSUBPDYrm_VHSUBPDrm_VHSUBPSYrm_VHSUBPSrm") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #902
  {DBGFIELD("MULPDrr_VMULPDrr")   1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #903
  {DBGFIELD("MULPSrr_VMULPSrr")   1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #904
  {DBGFIELD("MULSDrr_MULSDrr_Int_VMULSDrr_VMULSDrr_Int") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #905
  {DBGFIELD("MULSSrr_MULSSrr_Int_VMULSSrr_VMULSSrr_Int") 1, 0, 0, 1202, 3,  3, 1,  0, 0}, // #906
  {DBGFIELD("MULPDrm_MULPSrm_VMULPDrm_VMULPSrm") 1, 0, 0, 1198, 4, 10, 1,  4, 1}, // #907
  {DBGFIELD("MULSDrm_MULSDrm_Int_MULSSrm_MULSSrm_Int_VMULSDrm_VMULSDrm_Int_VMULSSrm_VMULSSrm_Int") 1, 0, 0, 1198, 4, 10, 1,  4, 1}, // #908
  {DBGFIELD("VDIVPSYrr")          1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #909
  {DBGFIELD("VDIVPSYrm")          1, 0, 0, 1188, 4, 52, 1,  4, 1}, // #910
  {DBGFIELD("VDIVPDYrr")          1, 0, 0, 1192, 3, 53, 1,  0, 0}, // #911
  {DBGFIELD("VDIVPDYrm")          1, 0, 0, 1188, 4, 52, 1,  4, 1}, // #912
  {DBGFIELD("VRCPPSYr")           1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #913
  {DBGFIELD("VRCPPSYm")           1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #914
  {DBGFIELD("ROUNDPDr_ROUNDPSr_VROUNDPDr_VROUNDPSr_VROUNDYPDr_VROUNDYPSr") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #915
  {DBGFIELD("ROUNDSDr_ROUNDSDr_Int_ROUNDSSr_ROUNDSSr_Int_VROUNDSDr_VROUNDSDr_Int_VROUNDSSr_VROUNDSSr_Int") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #916
  {DBGFIELD("ROUNDPDm_VROUNDPDm_VROUNDYPDm") 1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #917
  {DBGFIELD("ROUNDPSm_VROUNDPSm_VROUNDYPSm") 1, 0, 0, 1174, 3, 16, 1,  0, 0}, // #918
  {DBGFIELD("ROUNDSDm_ROUNDSSm_VROUNDSDm_VROUNDSSm") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #919
  {DBGFIELD("DPPSrri_VDPPSYrri_VDPPSrri") 1, 0, 0, 427, 2,  6, 1,  0, 0}, // #920
  {DBGFIELD("DPPSrmi_VDPPSYrmi_VDPPSrmi") 1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #921
  {DBGFIELD("DPPDrri")            1, 0, 0, 427, 2,  6, 1,  0, 0}, // #922
  {DBGFIELD("VDPPDrri")           1, 0, 0, 427, 2,  6, 1,  0, 0}, // #923
  {DBGFIELD("DPPDrmi_VDPPDrmi")   1, 0, 0, 1174, 3, 16, 1,  4, 1}, // #924
  {DBGFIELD("VFMADDPD4rr_VFMADDPD4rrY_VFMADDPD4rrY_REV_VFMADDPD4rr_REV_VFMADDPDr132r_VFMADDPDr132rY_VFMADDPDr213r_VFMADDPDr213rY_VFMADDPDr231r_VFMADDPDr231rY_VFMADDPS4rr_VFMADDPS4rrY_VFMADDPS4rrY_REV_VFMADDPS4rr_REV_VFMADDPSr132r_VFMADDPSr132rY_VFMADDPSr213r_VFMADDPSr213rY_VFMADDPSr231r_VFMADDPSr231rY_VFMADDSD4rr_VFMADDSD4rr_Int_VFMADDSD4rr_REV_VFMADDSDr132r_VFMADDSDr132r_Int_VFMADDSDr213r_VFMADDSDr213r_Int_VFMADDSDr231r_VFMADDSDr231r_Int_VFMADDSS4rr_VFMADDSS4rr_Int_VFMADDSS4rr_REV_VFMADDSSr132r_VFMADDSSr132r_Int_VFMADDSSr213r_VFMADDSSr213r_Int_VFMADDSSr231r_VFMADDSSr231r_Int_VFMADDSUBPDr132r_VFMADDSUBPDr132rY_VFMADDSUBPDr213r_VFMADDSUBPDr213rY_VFMADDSUBPDr231r_VFMADDSUBPDr231rY_VFMADDSUBPSr132r_VFMADDSUBPSr132rY_VFMADDSUBPSr213r_VFMADDSUBPSr213rY_VFMADDSUBPSr231r_VFMADDSUBPSr231rY_VFMSUBADDPDr132r_VFMSUBADDPDr132rY_VFMSUBADDPDr213r_VFMSUBADDPDr213rY_VFMSUBADDPDr231r_VFMSUBADDPDr231rY_VFMSUBADDPSr132r_VFMSUBADDPSr132rY_VFMSUBADDPSr213r_VFMSUBADDPSr213rY_VFMSUBADDPSr231r_VFMSUBADDPSr231rY_VFMSUBPD4rr_VFMSUBPD4rrY_VFMSUBPD4rrY_REV_VFMSUBPD4rr_REV_VFMSUBPDr132r_VFMSUBPDr132rY_VFMSUBPDr213r_VFMSUBPDr213rY_VFMSUBPDr231r_VFMSUBPDr231rY_VFMSUBPS4rr_VFMSUBPS4rrY_VFMSUBPS4rrY_REV_VFMSUBPS4rr_REV_VFMSUBPSr132r_VFMSUBPSr132rY_VFMSUBPSr213r_VFMSUBPSr213rY_VFMSUBPSr231r_VFMSUBPSr231rY_VFMSUBSD4rr_VFMSUBSD4rr_Int_VFMSUBSD4rr_REV_VFMSUBSDr132r_VFMSUBSDr132r_Int_VFMSUBSDr213r_VFMSUBSDr213r_Int_VFMSUBSDr231r_VFMSUBSDr231r_Int_VFMSUBSS4rr_VFMSUBSS4rr_Int_VFMSUBSS4rr_REV_VFMSUBSSr132r_VFMSUBSSr132r_Int_VFMSUBSSr213r_VFMSUBSSr213r_Int_VFMSUBSSr231r_VFMSUBSSr231r_Int_VFNMADDPD4rr_VFNMADDPD4rrY_VFNMADDPD4rrY_REV_VFNMADDPD4rr_REV_VFNMADDPDr132r_VFNMADDPDr132rY_VFNMADDPDr213r_VFNMADDPDr213rY_VFNMADDPDr231r_VFNMADDPDr231rY_VFNMADDPS4rr_VFNMADDPS4rrY_VFNMADDPS4rrY_REV_VFNMADDPS4rr_REV_VFNMADDPSr132r_VFNMADDPSr132rY_VFNMADDPSr213r_VFNMADDPSr213rY_VFNMADDPSr231r_VFNMADDPSr231rY_VFNMADDSD4rr_VFNMADDSD4rr_Int_VFNMADDSD4rr_REV_VFNMADDSDr132r_VFNMADDSDr132r_Int_VFNMADDSDr213r_VFNMADDSDr213r_Int_VFNMADDSDr231r_VFNMADDSDr231r_Int_VFNMADDSS4rr_VFNMADDSS4rr_Int_VFNMADDSS4rr_REV_VFNMADDSSr132r_VFNMADDSSr132r_Int_VFNMADDSSr213r_VFNMADDSSr213r_Int_VFNMADDSSr231r_VFNMADDSSr231r_Int_VFNMSUBPD4rr_VFNMSUBPD4rrY_VFNMSUBPD4rrY_REV_VFNMSUBPD4rr_REV_VFNMSUBPDr132r_VFNMSUBPDr132rY_VFNMSUBPDr213r_VFNMSUBPDr213rY_VFNMSUBPDr231r_VFNMSUBPDr231rY_VFNMSUBPS4rr_VFNMSUBPS4rrY_VFNMSUBPS4rrY_REV_VFNMSUBPS4rr_REV_VFNMSUBPSr132r_VFNMSUBPSr132rY_VFNMSUBPSr213r_VFNMSUBPSr213rY_VFNMSUBPSr231r_VFNMSUBPSr231rY_VFNMSUBSD4rr_VFNMSUBSD4rr_Int_VFNMSUBSD4rr_REV_VFNMSUBSDr132r_VFNMSUBSDr132r_Int_VFNMSUBSDr213r_VFNMSUBSDr213r_Int_VFNMSUBSDr231r_VFNMSUBSDr231r_Int_VFNMSUBSS4rr_VFNMSUBSS4rr_Int_VFNMSUBSS4rr_REV_VFNMSUBSSr132r_VFNMSUBSSr132r_Int_VFNMSUBSSr213r_VFNMSUBSSr213r_Int_VFNMSUBSSr231r_VFNMSUBSSr231r_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #925
  {DBGFIELD("VFMADDPD4mr_VFMADDPD4mrY_VFMADDPD4rm_VFMADDPD4rmY_VFMADDPDr132m_VFMADDPDr132mY_VFMADDPDr213m_VFMADDPDr213mY_VFMADDPDr231m_VFMADDPDr231mY_VFMADDPS4mr_VFMADDPS4mrY_VFMADDPS4rm_VFMADDPS4rmY_VFMADDPSr132m_VFMADDPSr132mY_VFMADDPSr213m_VFMADDPSr213mY_VFMADDPSr231m_VFMADDPSr231mY_VFMADDSD4mr_VFMADDSD4mr_Int_VFMADDSD4rm_VFMADDSD4rm_Int_VFMADDSDr132m_VFMADDSDr132m_Int_VFMADDSDr213m_VFMADDSDr213m_Int_VFMADDSDr231m_VFMADDSDr231m_Int_VFMADDSS4mr_VFMADDSS4mr_Int_VFMADDSS4rm_VFMADDSS4rm_Int_VFMADDSSr132m_VFMADDSSr132m_Int_VFMADDSSr213m_VFMADDSSr213m_Int_VFMADDSSr231m_VFMADDSSr231m_Int_VFMADDSUBPDr132m_VFMADDSUBPDr132mY_VFMADDSUBPDr213m_VFMADDSUBPDr213mY_VFMADDSUBPDr231m_VFMADDSUBPDr231mY_VFMADDSUBPSr132m_VFMADDSUBPSr132mY_VFMADDSUBPSr213m_VFMADDSUBPSr213mY_VFMADDSUBPSr231m_VFMADDSUBPSr231mY_VFMSUBADDPDr132m_VFMSUBADDPDr132mY_VFMSUBADDPDr213m_VFMSUBADDPDr213mY_VFMSUBADDPDr231m_VFMSUBADDPDr231mY_VFMSUBADDPSr132m_VFMSUBADDPSr132mY_VFMSUBADDPSr213m_VFMSUBADDPSr213mY_VFMSUBADDPSr231m_VFMSUBADDPSr231mY_VFMSUBPD4mr_VFMSUBPD4mrY_VFMSUBPD4rm_VFMSUBPD4rmY_VFMSUBPDr132m_VFMSUBPDr132mY_VFMSUBPDr213m_VFMSUBPDr213mY_VFMSUBPDr231m_VFMSUBPDr231mY_VFMSUBPS4mr_VFMSUBPS4mrY_VFMSUBPS4rm_VFMSUBPS4rmY_VFMSUBPSr132m_VFMSUBPSr132mY_VFMSUBPSr213m_VFMSUBPSr213mY_VFMSUBPSr231m_VFMSUBPSr231mY_VFMSUBSD4mr_VFMSUBSD4mr_Int_VFMSUBSD4rm_VFMSUBSD4rm_Int_VFMSUBSDr132m_VFMSUBSDr132m_Int_VFMSUBSDr213m_VFMSUBSDr213m_Int_VFMSUBSDr231m_VFMSUBSDr231m_Int_VFMSUBSS4mr_VFMSUBSS4mr_Int_VFMSUBSS4rm_VFMSUBSS4rm_Int_VFMSUBSSr132m_VFMSUBSSr132m_Int_VFMSUBSSr213m_VFMSUBSSr213m_Int_VFMSUBSSr231m_VFMSUBSSr231m_Int_VFNMADDPD4mr_VFNMADDPD4mrY_VFNMADDPD4rm_VFNMADDPD4rmY_VFNMADDPDr132m_VFNMADDPDr132mY_VFNMADDPDr213m_VFNMADDPDr213mY_VFNMADDPDr231m_VFNMADDPDr231mY_VFNMADDPS4mr_VFNMADDPS4mrY_VFNMADDPS4rm_VFNMADDPS4rmY_VFNMADDPSr132m_VFNMADDPSr132mY_VFNMADDPSr213m_VFNMADDPSr213mY_VFNMADDPSr231m_VFNMADDPSr231mY_VFNMADDSD4mr_VFNMADDSD4mr_Int_VFNMADDSD4rm_VFNMADDSD4rm_Int_VFNMADDSDr132m_VFNMADDSDr132m_Int_VFNMADDSDr213m_VFNMADDSDr213m_Int_VFNMADDSDr231m_VFNMADDSDr231m_Int_VFNMADDSS4mr_VFNMADDSS4mr_Int_VFNMADDSS4rm_VFNMADDSS4rm_Int_VFNMADDSSr132m_VFNMADDSSr132m_Int_VFNMADDSSr213m_VFNMADDSSr213m_Int_VFNMADDSSr231m_VFNMADDSSr231m_Int_VFNMSUBPD4mr_VFNMSUBPD4mrY_VFNMSUBPD4rm_VFNMSUBPD4rmY_VFNMSUBPDr132m_VFNMSUBPDr132mY_VFNMSUBPDr213m_VFNMSUBPDr213mY_VFNMSUBPDr231m_VFNMSUBPDr231mY_VFNMSUBPS4mr_VFNMSUBPS4mrY_VFNMSUBPS4rm_VFNMSUBPS4rmY_VFNMSUBPSr132m_VFNMSUBPSr132mY_VFNMSUBPSr213m_VFNMSUBPSr213mY_VFNMSUBPSr231m_VFNMSUBPSr231mY_VFNMSUBSD4mr_VFNMSUBSD4mr_Int_VFNMSUBSD4rm_VFNMSUBSD4rm_Int_VFNMSUBSDr132m_VFNMSUBSDr132m_Int_VFNMSUBSDr213m_VFNMSUBSDr213m_Int_VFNMSUBSDr231m_VFNMSUBSDr231m_Int_VFNMSUBSS4mr_VFNMSUBSS4mr_Int_VFNMSUBSS4rm_VFNMSUBSS4rm_Int_VFNMSUBSSr132m_VFNMSUBSSr132m_Int_VFNMSUBSSr213m_VFNMSUBSSr213m_Int_VFNMSUBSSr231m_VFNMSUBSSr231m_Int") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #926
  {DBGFIELD("VSQRTPSYr")          1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #927
  {DBGFIELD("VSQRTPSYm")          1, 0, 0, 1180, 3, 39, 1,  0, 0}, // #928
  {DBGFIELD("VSQRTPDYr")          1, 0, 0, 1172, 2, 24, 1,  0, 0}, // #929
  {DBGFIELD("VSQRTPDYm")          1, 0, 0, 1180, 3, 39, 1,  0, 0}, // #930
  {DBGFIELD("RSQRTPSr_VRSQRTPSr") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #931
  {DBGFIELD("RSQRTSSr_VRSQRTSSr") 1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #932
  {DBGFIELD("RSQRTSSr_Int")       1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #933
  {DBGFIELD("VRSQRTSSr_Int")      1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #934
  {DBGFIELD("RSQRTPSm_VRSQRTPSm") 1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #935
  {DBGFIELD("RSQRTSSm_VRSQRTSSm") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #936
  {DBGFIELD("RSQRTSSm_Int_VRSQRTSSm_Int") 1, 0, 0, 1180, 3, 10, 1,  4, 1}, // #937
  {DBGFIELD("VRSQRTPSYr")         1, 0, 0, 1172, 2,  3, 1,  0, 0}, // #938
  {DBGFIELD("VRSQRTPSYm")         1, 0, 0, 1180, 3, 10, 1,  0, 0}, // #939
  {DBGFIELD("ANDNPDrr_ANDNPSrr_ANDPDrr_ANDPSrr_ORPDrr_ORPSrr_VANDNPDYrr_VANDNPDrr_VANDNPSYrr_VANDNPSrr_VANDPDYrr_VANDPDrr_VANDPSYrr_VANDPSrr_VORPDYrr_VORPDrr_VORPSYrr_VORPSrr_VXORPDYrr_VXORPDrr_VXORPSYrr_VXORPSrr_XORPDrr_XORPSrr") 1, 0, 0, 13, 1,  2, 1,  0, 0}, // #940
  {DBGFIELD("ANDNPDrm_ANDNPSrm_ANDPDrm_ANDPSrm_ORPDrm_ORPSrm_VANDNPDYrm_VANDNPDrm_VANDNPSYrm_VANDNPSrm_VANDPDYrm_VANDPDrm_VANDPSYrm_VANDPSrm_VORPDYrm_VORPDrm_VORPSYrm_VORPSrm_VXORPDYrm_VXORPDrm_VXORPSYrm_VXORPSrm_XORPDrm_XORPSrm") 1, 0, 0, 13, 2,  9, 1,  4, 1}, // #941
  {DBGFIELD("VZEROUPPER")         0, 0, 0,  0, 0,  0, 0,  0, 0}, // #942
  {DBGFIELD("VZEROALL")           0, 0, 0,  0, 0,  0, 0,  0, 0}, // #943
  {DBGFIELD("LDMXCSR_VLDMXCSR")   1, 0, 0, 14, 1,  6, 1,  0, 0}, // #944
  {DBGFIELD("STMXCSR_VSTMXCSR")   1, 0, 0, 386, 2,  2, 1,  0, 0} // #945
}; // SLMModelSchedClasses

static const llvm::MCSchedModel NoSchedModel = {
  MCSchedModel::DefaultIssueWidth,
  MCSchedModel::DefaultMicroOpBufferSize,
  MCSchedModel::DefaultLoopMicroOpBufferSize,
  MCSchedModel::DefaultLoadLatency,
  MCSchedModel::DefaultHighLatency,
  MCSchedModel::DefaultMispredictPenalty,
  0, // PostRAScheduler
  1, // CompleteModel
  0, // Processor ID
  nullptr, nullptr, 0, 0, // No instruction-level machine model.
  nullptr}; // No Itinerary

static const llvm::MCSchedModel GenericModel = {
  4, // IssueWidth
  32, // MicroOpBufferSize
  MCSchedModel::DefaultLoopMicroOpBufferSize,
  4, // LoadLatency
  10, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  0, // PostRAScheduler
  1, // CompleteModel
  1, // Processor ID
  nullptr, nullptr, 0, 0, // No instruction-level machine model.
  nullptr}; // No Itinerary

static const llvm::MCSchedModel AtomModel = {
  2, // IssueWidth
  0, // MicroOpBufferSize
  10, // LoopMicroOpBufferSize
  3, // LoadLatency
  30, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // PostRAScheduler
  1, // CompleteModel
  2, // Processor ID
  nullptr, nullptr, 0, 0, // No instruction-level machine model.
  AtomItineraries};

// {Name, NumUnits, SuperIdx, IsBuffered}
static const llvm::MCProcResourceDesc HaswellModelProcResources[] = {
  {DBGFIELD("InvalidUnit")     0, 0, 0},
  {DBGFIELD("HWDivider")       1, 0, -1}, // #1
  {DBGFIELD("HWPort0")         1, 0, -1}, // #2
  {DBGFIELD("HWPort1")         1, 0, -1}, // #3
  {DBGFIELD("HWPort4")         1, 0, -1}, // #4
  {DBGFIELD("HWPort5")         1, 0, -1}, // #5
  {DBGFIELD("HWPort6")         1, 0, -1}, // #6
  {DBGFIELD("HWPort01")        2, 0, -1}, // #7
  {DBGFIELD("HWPort04")        2, 0, -1}, // #8
  {DBGFIELD("HWPort05")        2, 0, -1}, // #9
  {DBGFIELD("HWPort06")        2, 0, -1}, // #10
  {DBGFIELD("HWPort15")        2, 0, -1}, // #11
  {DBGFIELD("HWPort16")        2, 0, -1}, // #12
  {DBGFIELD("HWPort23")        2, 0, -1}, // #13
  {DBGFIELD("HWPort56")        2, 0, -1}, // #14
  {DBGFIELD("HWPort015")       3, 0, -1}, // #15
  {DBGFIELD("HWPort056")       3, 0, -1}, // #16
  {DBGFIELD("HWPort237")       3, 0, -1}, // #17
  {DBGFIELD("HWPort0156")      4, 0, -1}, // #18
  {DBGFIELD("HWPortAny")       8, 0, 60}  // #19
};

static const llvm::MCSchedModel HaswellModel = {
  4, // IssueWidth
  192, // MicroOpBufferSize
  50, // LoopMicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // PostRAScheduler
  0, // CompleteModel
  3, // Processor ID
  HaswellModelProcResources,
  HaswellModelSchedClasses,
  20,
  946,
  nullptr}; // No Itinerary

// {Name, NumUnits, SuperIdx, IsBuffered}
static const llvm::MCProcResourceDesc BtVer2ModelProcResources[] = {
  {DBGFIELD("InvalidUnit")     0, 0, 0},
  {DBGFIELD("JALU1")           1, 0, -1}, // #1
  {DBGFIELD("JALU01")          2, 0, 20}, // #2
  {DBGFIELD("JAny")            6, 0, -1}, // #3
  {DBGFIELD("JDiv")            1, 0, -1}, // #4
  {DBGFIELD("JFPM")            1, 0, -1}, // #5
  {DBGFIELD("JFPU0")           1, 0, -1}, // #6
  {DBGFIELD("JFPU1")           1, 0, -1}, // #7
  {DBGFIELD("JFPU01")          2, 0, 18}, // #8
  {DBGFIELD("JLAGU")           1, 0, -1}, // #9
  {DBGFIELD("JLSAGU")          2, 0, 12}, // #10
  {DBGFIELD("JSAGU")           1, 0, -1}, // #11
  {DBGFIELD("JVIMUL")          1, 0, -1}  // #12
};

static const llvm::MCSchedModel BtVer2Model = {
  2, // IssueWidth
  64, // MicroOpBufferSize
  MCSchedModel::DefaultLoopMicroOpBufferSize,
  5, // LoadLatency
  25, // HighLatency
  14, // MispredictPenalty
  1, // PostRAScheduler
  0, // CompleteModel
  4, // Processor ID
  BtVer2ModelProcResources,
  BtVer2ModelSchedClasses,
  13,
  946,
  nullptr}; // No Itinerary

// {Name, NumUnits, SuperIdx, IsBuffered}
static const llvm::MCProcResourceDesc SandyBridgeModelProcResources[] = {
  {DBGFIELD("InvalidUnit")     0, 0, 0},
  {DBGFIELD("SBDivider")       1, 0, -1}, // #1
  {DBGFIELD("SBPort0")         1, 0, -1}, // #2
  {DBGFIELD("SBPort1")         1, 0, -1}, // #3
  {DBGFIELD("SBPort4")         1, 0, -1}, // #4
  {DBGFIELD("SBPort5")         1, 0, -1}, // #5
  {DBGFIELD("SBPort05")        2, 0, -1}, // #6
  {DBGFIELD("SBPort15")        2, 0, -1}, // #7
  {DBGFIELD("SBPort23")        2, 0, -1}, // #8
  {DBGFIELD("SBPort015")       3, 0, -1}, // #9
  {DBGFIELD("SBPortAny")       6, 0, 54}  // #10
};

static const llvm::MCSchedModel SandyBridgeModel = {
  4, // IssueWidth
  168, // MicroOpBufferSize
  28, // LoopMicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // PostRAScheduler
  0, // CompleteModel
  5, // Processor ID
  SandyBridgeModelProcResources,
  SandyBridgeModelSchedClasses,
  11,
  946,
  nullptr}; // No Itinerary

// {Name, NumUnits, SuperIdx, IsBuffered}
static const llvm::MCProcResourceDesc SLMModelProcResources[] = {
  {DBGFIELD("InvalidUnit")     0, 0, 0},
  {DBGFIELD("FPC_RSV0")        1, 0, 1}, // #1
  {DBGFIELD("FPC_RSV1")        1, 0, 1}, // #2
  {DBGFIELD("FPC_RSV01")       2, 0, -1}, // #3
  {DBGFIELD("IEC_RSV0")        1, 0, -1}, // #4
  {DBGFIELD("IEC_RSV1")        1, 0, -1}, // #5
  {DBGFIELD("IEC_RSV01")       2, 0, -1}, // #6
  {DBGFIELD("MEC_RSV")         1, 0, -1}, // #7
  {DBGFIELD("SMDivider")       1, 0, -1}, // #8
  {DBGFIELD("SMFPDivider")     1, 0, -1}, // #9
  {DBGFIELD("SMFPMultiplier")  1, 0, -1}  // #10
};

static const llvm::MCSchedModel SLMModel = {
  2, // IssueWidth
  32, // MicroOpBufferSize
  10, // LoopMicroOpBufferSize
  3, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  10, // MispredictPenalty
  1, // PostRAScheduler
  0, // CompleteModel
  6, // Processor ID
  SLMModelProcResources,
  SLMModelSchedClasses,
  11,
  946,
  nullptr}; // No Itinerary

// Sorted (by key) array of itineraries for CPU subtype.
extern const llvm::SubtargetInfoKV X86ProcSchedKV[] = {
  { "amdfam10", (const void *)&GenericModel },
  { "athlon", (const void *)&GenericModel },
  { "athlon-4", (const void *)&GenericModel },
  { "athlon-fx", (const void *)&GenericModel },
  { "athlon-mp", (const void *)&GenericModel },
  { "athlon-tbird", (const void *)&GenericModel },
  { "athlon-xp", (const void *)&GenericModel },
  { "athlon64", (const void *)&GenericModel },
  { "athlon64-sse3", (const void *)&GenericModel },
  { "atom", (const void *)&AtomModel },
  { "barcelona", (const void *)&GenericModel },
  { "bdver1", (const void *)&GenericModel },
  { "bdver2", (const void *)&GenericModel },
  { "bdver3", (const void *)&GenericModel },
  { "bdver4", (const void *)&GenericModel },
  { "bonnell", (const void *)&AtomModel },
  { "broadwell", (const void *)&HaswellModel },
  { "btver1", (const void *)&GenericModel },
  { "btver2", (const void *)&BtVer2Model },
  { "c3", (const void *)&GenericModel },
  { "c3-2", (const void *)&GenericModel },
  { "core-avx-i", (const void *)&SandyBridgeModel },
  { "core-avx2", (const void *)&HaswellModel },
  { "core2", (const void *)&SandyBridgeModel },
  { "corei7", (const void *)&SandyBridgeModel },
  { "corei7-avx", (const void *)&SandyBridgeModel },
  { "generic", (const void *)&GenericModel },
  { "geode", (const void *)&GenericModel },
  { "haswell", (const void *)&HaswellModel },
  { "i386", (const void *)&GenericModel },
  { "i486", (const void *)&GenericModel },
  { "i586", (const void *)&GenericModel },
  { "i686", (const void *)&GenericModel },
  { "ivybridge", (const void *)&SandyBridgeModel },
  { "k6", (const void *)&GenericModel },
  { "k6-2", (const void *)&GenericModel },
  { "k6-3", (const void *)&GenericModel },
  { "k8", (const void *)&GenericModel },
  { "k8-sse3", (const void *)&GenericModel },
  { "knl", (const void *)&HaswellModel },
  { "nehalem", (const void *)&SandyBridgeModel },
  { "nocona", (const void *)&GenericModel },
  { "opteron", (const void *)&GenericModel },
  { "opteron-sse3", (const void *)&GenericModel },
  { "penryn", (const void *)&SandyBridgeModel },
  { "pentium", (const void *)&GenericModel },
  { "pentium-m", (const void *)&GenericModel },
  { "pentium-mmx", (const void *)&GenericModel },
  { "pentium2", (const void *)&GenericModel },
  { "pentium3", (const void *)&GenericModel },
  { "pentium3m", (const void *)&GenericModel },
  { "pentium4", (const void *)&GenericModel },
  { "pentium4m", (const void *)&GenericModel },
  { "pentiumpro", (const void *)&GenericModel },
  { "prescott", (const void *)&GenericModel },
  { "sandybridge", (const void *)&SandyBridgeModel },
  { "silvermont", (const void *)&SLMModel },
  { "skx", (const void *)&HaswellModel },
  { "skylake", (const void *)&HaswellModel },
  { "slm", (const void *)&SLMModel },
  { "westmere", (const void *)&SandyBridgeModel },
  { "winchip-c6", (const void *)&GenericModel },
  { "winchip2", (const void *)&GenericModel },
  { "x86-64", (const void *)&SandyBridgeModel },
  { "yonah", (const void *)&SandyBridgeModel }
};
#undef DBGFIELD
static inline MCSubtargetInfo *createX86MCSubtargetInfoImpl(const Triple &TT, StringRef CPU, StringRef FS) {
  return new MCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV,
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable,
                      X86Stages, X86OperandCycles, X86ForwardingPaths);
}

} // end llvm namespace
#endif // GET_SUBTARGETINFO_MC_DESC


#ifdef GET_SUBTARGETINFO_TARGET_DESC
#undef GET_SUBTARGETINFO_TARGET_DESC
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
// ParseSubtargetFeatures - Parses features string setting specified
// subtarget options.
void llvm::X86Subtarget::ParseSubtargetFeatures(StringRef CPU, StringRef FS) {
  DEBUG(dbgs() << "\nFeatures:" << FS);
  DEBUG(dbgs() << "\nCPU:" << CPU << "\n\n");
  InitMCProcessorInfo(CPU, FS);
  const FeatureBitset& Bits = getFeatureBits();
  if (Bits[X86::Feature3DNow] && X863DNowLevel < ThreeDNow) X863DNowLevel = ThreeDNow;
  if (Bits[X86::Feature3DNowA] && X863DNowLevel < ThreeDNowA) X863DNowLevel = ThreeDNowA;
  if (Bits[X86::Feature64Bit]) HasX86_64 = true;
  if (Bits[X86::FeatureADX]) HasADX = true;
  if (Bits[X86::FeatureAES]) HasAES = true;
  if (Bits[X86::FeatureAVX] && X86SSELevel < AVX) X86SSELevel = AVX;
  if (Bits[X86::FeatureAVX2] && X86SSELevel < AVX2) X86SSELevel = AVX2;
  if (Bits[X86::FeatureAVX512] && X86SSELevel < AVX512F) X86SSELevel = AVX512F;
  if (Bits[X86::FeatureBMI]) HasBMI = true;
  if (Bits[X86::FeatureBMI2]) HasBMI2 = true;
  if (Bits[X86::FeatureBWI]) HasBWI = true;
  if (Bits[X86::FeatureCDI]) HasCDI = true;
  if (Bits[X86::FeatureCMOV]) HasCMov = true;
  if (Bits[X86::FeatureCMPXCHG16B]) HasCmpxchg16b = true;
  if (Bits[X86::FeatureCallRegIndirect]) CallRegIndirect = true;
  if (Bits[X86::FeatureDQI]) HasDQI = true;
  if (Bits[X86::FeatureERI]) HasERI = true;
  if (Bits[X86::FeatureF16C]) HasF16C = true;
  if (Bits[X86::FeatureFMA]) HasFMA = true;
  if (Bits[X86::FeatureFMA4]) HasFMA4 = true;
  if (Bits[X86::FeatureFSGSBase]) HasFSGSBase = true;
  if (Bits[X86::FeatureFXSR]) HasFXSR = true;
  if (Bits[X86::FeatureHLE]) HasHLE = true;
  if (Bits[X86::FeatureLAHFSAHF]) HasLAHFSAHF = true;
  if (Bits[X86::FeatureLEAForSP]) UseLeaForSP = true;
  if (Bits[X86::FeatureLEAUsesAG]) LEAUsesAG = true;
  if (Bits[X86::FeatureLZCNT]) HasLZCNT = true;
  if (Bits[X86::FeatureMMX] && X863DNowLevel < MMX) X863DNowLevel = MMX;
  if (Bits[X86::FeatureMOVBE]) HasMOVBE = true;
  if (Bits[X86::FeatureMPX]) HasMPX = true;
  if (Bits[X86::FeaturePCLMUL]) HasPCLMUL = true;
  if (Bits[X86::FeaturePFI]) HasPFI = true;
  if (Bits[X86::FeaturePKU]) HasPKU = true;
  if (Bits[X86::FeaturePOPCNT]) HasPOPCNT = true;
  if (Bits[X86::FeaturePRFCHW]) HasPRFCHW = true;
  if (Bits[X86::FeaturePadShortFunctions]) PadShortFunctions = true;
  if (Bits[X86::FeatureRDRAND]) HasRDRAND = true;
  if (Bits[X86::FeatureRDSEED]) HasRDSEED = true;
  if (Bits[X86::FeatureRTM]) HasRTM = true;
  if (Bits[X86::FeatureSHA]) HasSHA = true;
  if (Bits[X86::FeatureSSE1] && X86SSELevel < SSE1) X86SSELevel = SSE1;
  if (Bits[X86::FeatureSSE2] && X86SSELevel < SSE2) X86SSELevel = SSE2;
  if (Bits[X86::FeatureSSE3] && X86SSELevel < SSE3) X86SSELevel = SSE3;
  if (Bits[X86::FeatureSSE4A]) HasSSE4A = true;
  if (Bits[X86::FeatureSSE41] && X86SSELevel < SSE41) X86SSELevel = SSE41;
  if (Bits[X86::FeatureSSE42] && X86SSELevel < SSE42) X86SSELevel = SSE42;
  if (Bits[X86::FeatureSSEUnalignedMem]) HasSSEUnalignedMem = true;
  if (Bits[X86::FeatureSSSE3] && X86SSELevel < SSSE3) X86SSELevel = SSSE3;
  if (Bits[X86::FeatureSlowBTMem]) IsBTMemSlow = true;
  if (Bits[X86::FeatureSlowDivide32]) HasSlowDivide32 = true;
  if (Bits[X86::FeatureSlowDivide64]) HasSlowDivide64 = true;
  if (Bits[X86::FeatureSlowIncDec]) SlowIncDec = true;
  if (Bits[X86::FeatureSlowLEA]) SlowLEA = true;
  if (Bits[X86::FeatureSlowSHLD]) IsSHLDSlow = true;
  if (Bits[X86::FeatureSlowUAMem16]) IsUAMem16Slow = true;
  if (Bits[X86::FeatureSlowUAMem32]) IsUAMem32Slow = true;
  if (Bits[X86::FeatureSoftFloat]) UseSoftFloat = true;
  if (Bits[X86::FeatureTBM]) HasTBM = true;
  if (Bits[X86::FeatureVLX]) HasVLX = true;
  if (Bits[X86::FeatureXOP]) HasXOP = true;
  if (Bits[X86::FeatureXSAVE]) HasXSAVE = true;
  if (Bits[X86::FeatureXSAVEC]) HasXSAVEC = true;
  if (Bits[X86::FeatureXSAVEOPT]) HasXSAVEOPT = true;
  if (Bits[X86::FeatureXSAVES]) HasXSAVES = true;
  if (Bits[X86::Mode16Bit]) In16BitMode = true;
  if (Bits[X86::Mode32Bit]) In32BitMode = true;
  if (Bits[X86::Mode64Bit]) In64BitMode = true;
  if (Bits[X86::ProcIntelAtom] && X86ProcFamily < IntelAtom) X86ProcFamily = IntelAtom;
  if (Bits[X86::ProcIntelSLM] && X86ProcFamily < IntelSLM) X86ProcFamily = IntelSLM;
}
#endif // GET_SUBTARGETINFO_TARGET_DESC


#ifdef GET_SUBTARGETINFO_HEADER
#undef GET_SUBTARGETINFO_HEADER
namespace llvm {
class DFAPacketizer;
struct X86GenSubtargetInfo : public TargetSubtargetInfo {
  explicit X86GenSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS);
public:
  unsigned resolveSchedClass(unsigned SchedClass,  const MachineInstr *DefMI, const TargetSchedModel *SchedModel) const override;
  DFAPacketizer *createDFAPacketizer(const InstrItineraryData *IID) const;
};
} // end llvm namespace
#endif // GET_SUBTARGETINFO_HEADER


#ifdef GET_SUBTARGETINFO_CTOR
#undef GET_SUBTARGETINFO_CTOR
#include "llvm/CodeGen/TargetSchedule.h"
namespace llvm {
extern const llvm::SubtargetFeatureKV X86FeatureKV[];
extern const llvm::SubtargetFeatureKV X86SubTypeKV[];
extern const llvm::SubtargetInfoKV X86ProcSchedKV[];
extern const llvm::MCWriteProcResEntry X86WriteProcResTable[];
extern const llvm::MCWriteLatencyEntry X86WriteLatencyTable[];
extern const llvm::MCReadAdvanceEntry X86ReadAdvanceTable[];
extern const llvm::InstrStage X86Stages[];
extern const unsigned X86OperandCycles[];
extern const unsigned X86ForwardingPaths[];
X86GenSubtargetInfo::X86GenSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS)
  : TargetSubtargetInfo(TT, CPU, FS, makeArrayRef(X86FeatureKV, 69), makeArrayRef(X86SubTypeKV, 65),
                        X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable,
                        X86Stages, X86OperandCycles, X86ForwardingPaths) {}

unsigned X86GenSubtargetInfo
::resolveSchedClass(unsigned SchedClass, const MachineInstr *MI, const TargetSchedModel *SchedModel) const {
  report_fatal_error("Expected a variant SchedClass");
} // X86GenSubtargetInfo::resolveSchedClass
} // end llvm namespace
#endif // GET_SUBTARGETINFO_CTOR

