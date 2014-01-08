/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|*Subtarget Enumeration Source Fragment                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_SUBTARGETINFO_ENUM
#undef GET_SUBTARGETINFO_ENUM
namespace llvm {
namespace X86 {
  const uint64_t Feature3DNow = 1ULL << 0;
  const uint64_t Feature3DNowA = 1ULL << 1;
  const uint64_t Feature64Bit = 1ULL << 2;
  const uint64_t FeatureADX = 1ULL << 3;
  const uint64_t FeatureAES = 1ULL << 4;
  const uint64_t FeatureAVX = 1ULL << 5;
  const uint64_t FeatureAVX2 = 1ULL << 6;
  const uint64_t FeatureAVX512 = 1ULL << 7;
  const uint64_t FeatureBMI = 1ULL << 8;
  const uint64_t FeatureBMI2 = 1ULL << 9;
  const uint64_t FeatureCDI = 1ULL << 10;
  const uint64_t FeatureCMOV = 1ULL << 11;
  const uint64_t FeatureCMPXCHG16B = 1ULL << 12;
  const uint64_t FeatureCallRegIndirect = 1ULL << 13;
  const uint64_t FeatureERI = 1ULL << 14;
  const uint64_t FeatureF16C = 1ULL << 15;
  const uint64_t FeatureFMA = 1ULL << 16;
  const uint64_t FeatureFMA4 = 1ULL << 17;
  const uint64_t FeatureFSGSBase = 1ULL << 18;
  const uint64_t FeatureFastUAMem = 1ULL << 19;
  const uint64_t FeatureHLE = 1ULL << 20;
  const uint64_t FeatureLEAUsesAG = 1ULL << 21;
  const uint64_t FeatureLZCNT = 1ULL << 22;
  const uint64_t FeatureLeaForSP = 1ULL << 23;
  const uint64_t FeatureMMX = 1ULL << 24;
  const uint64_t FeatureMOVBE = 1ULL << 25;
  const uint64_t FeaturePCLMUL = 1ULL << 26;
  const uint64_t FeaturePFI = 1ULL << 27;
  const uint64_t FeaturePOPCNT = 1ULL << 28;
  const uint64_t FeaturePRFCHW = 1ULL << 29;
  const uint64_t FeaturePadShortFunctions = 1ULL << 30;
  const uint64_t FeatureRDRAND = 1ULL << 31;
  const uint64_t FeatureRDSEED = 1ULL << 32;
  const uint64_t FeatureRTM = 1ULL << 33;
  const uint64_t FeatureSHA = 1ULL << 34;
  const uint64_t FeatureSSE1 = 1ULL << 35;
  const uint64_t FeatureSSE2 = 1ULL << 36;
  const uint64_t FeatureSSE3 = 1ULL << 37;
  const uint64_t FeatureSSE4A = 1ULL << 38;
  const uint64_t FeatureSSE41 = 1ULL << 39;
  const uint64_t FeatureSSE42 = 1ULL << 40;
  const uint64_t FeatureSSSE3 = 1ULL << 41;
  const uint64_t FeatureSlowBTMem = 1ULL << 42;
  const uint64_t FeatureSlowDivide = 1ULL << 43;
  const uint64_t FeatureTBM = 1ULL << 44;
  const uint64_t FeatureVectorUAMem = 1ULL << 45;
  const uint64_t FeatureXOP = 1ULL << 46;
  const uint64_t Mode64Bit = 1ULL << 47;
  const uint64_t ProcIntelAtom = 1ULL << 48;
  const uint64_t ProcIntelSLM = 1ULL << 49;
}
} // End llvm namespace 
#endif // GET_SUBTARGETINFO_ENUM


#ifdef GET_SUBTARGETINFO_MC_DESC
#undef GET_SUBTARGETINFO_MC_DESC
namespace llvm {
// Sorted (by key) array of values for CPU features.
extern const llvm::SubtargetFeatureKV X86FeatureKV[] = {
  { "3dnow", "Enable 3DNow! instructions", X86::Feature3DNow, X86::FeatureMMX },
  { "3dnowa", "Enable 3DNow! Athlon instructions", X86::Feature3DNowA, X86::Feature3DNow },
  { "64bit", "Support 64-bit instructions", X86::Feature64Bit, X86::FeatureCMOV },
  { "64bit-mode", "64-bit mode (x86_64)", X86::Mode64Bit, 0ULL },
  { "adx", "Support ADX instructions", X86::FeatureADX, 0ULL },
  { "aes", "Enable AES instructions", X86::FeatureAES, X86::FeatureSSE2 },
  { "atom", "Intel Atom processors", X86::ProcIntelAtom, 0ULL },
  { "avx", "Enable AVX instructions", X86::FeatureAVX, X86::FeatureSSE42 },
  { "avx2", "Enable AVX2 instructions", X86::FeatureAVX2, X86::FeatureAVX },
  { "avx512cd", "Enable AVX-512 Conflict Detection Instructions", X86::FeatureCDI, X86::FeatureAVX512 },
  { "avx512er", "Enable AVX-512 Exponential and Reciprocal Instructions", X86::FeatureERI, X86::FeatureAVX512 },
  { "avx512f", "Enable AVX-512 instructions", X86::FeatureAVX512, X86::FeatureAVX2 },
  { "avx512pf", "Enable AVX-512 PreFetch Instructions", X86::FeaturePFI, X86::FeatureAVX512 },
  { "bmi", "Support BMI instructions", X86::FeatureBMI, 0ULL },
  { "bmi2", "Support BMI2 instructions", X86::FeatureBMI2, 0ULL },
  { "call-reg-indirect", "Call register indirect", X86::FeatureCallRegIndirect, 0ULL },
  { "cmov", "Enable conditional move instructions", X86::FeatureCMOV, 0ULL },
  { "cx16", "64-bit with cmpxchg16b", X86::FeatureCMPXCHG16B, X86::Feature64Bit },
  { "f16c", "Support 16-bit floating point conversion instructions", X86::FeatureF16C, X86::FeatureAVX },
  { "fast-unaligned-mem", "Fast unaligned memory access", X86::FeatureFastUAMem, 0ULL },
  { "fma", "Enable three-operand fused multiple-add", X86::FeatureFMA, X86::FeatureAVX },
  { "fma4", "Enable four-operand fused multiple-add", X86::FeatureFMA4, X86::FeatureAVX | X86::FeatureSSE4A },
  { "fsgsbase", "Support FS/GS Base instructions", X86::FeatureFSGSBase, 0ULL },
  { "hle", "Support HLE", X86::FeatureHLE, 0ULL },
  { "idiv-to-divb", "Use small divide for positive values less than 256", X86::FeatureSlowDivide, 0ULL },
  { "lea-sp", "Use LEA for adjusting the stack pointer", X86::FeatureLeaForSP, 0ULL },
  { "lea-uses-ag", "LEA instruction needs inputs at AG stage", X86::FeatureLEAUsesAG, 0ULL },
  { "lzcnt", "Support LZCNT instruction", X86::FeatureLZCNT, 0ULL },
  { "mmx", "Enable MMX instructions", X86::FeatureMMX, 0ULL },
  { "movbe", "Support MOVBE instruction", X86::FeatureMOVBE, 0ULL },
  { "pad-short-functions", "Pad short functions", X86::FeaturePadShortFunctions, 0ULL },
  { "pclmul", "Enable packed carry-less multiplication instructions", X86::FeaturePCLMUL, X86::FeatureSSE2 },
  { "popcnt", "Support POPCNT instruction", X86::FeaturePOPCNT, 0ULL },
  { "prfchw", "Support PRFCHW instructions", X86::FeaturePRFCHW, 0ULL },
  { "rdrnd", "Support RDRAND instruction", X86::FeatureRDRAND, 0ULL },
  { "rdseed", "Support RDSEED instruction", X86::FeatureRDSEED, 0ULL },
  { "rtm", "Support RTM instructions", X86::FeatureRTM, 0ULL },
  { "sha", "Enable SHA instructions", X86::FeatureSHA, X86::FeatureSSE2 },
  { "slm", "Intel Silvermont processors", X86::ProcIntelSLM, 0ULL },
  { "slow-bt-mem", "Bit testing of memory is slow", X86::FeatureSlowBTMem, 0ULL },
  { "sse", "Enable SSE instructions", X86::FeatureSSE1, X86::FeatureMMX | X86::FeatureCMOV },
  { "sse2", "Enable SSE2 instructions", X86::FeatureSSE2, X86::FeatureSSE1 },
  { "sse3", "Enable SSE3 instructions", X86::FeatureSSE3, X86::FeatureSSE2 },
  { "sse4.1", "Enable SSE 4.1 instructions", X86::FeatureSSE41, X86::FeatureSSSE3 },
  { "sse4.2", "Enable SSE 4.2 instructions", X86::FeatureSSE42, X86::FeatureSSE41 },
  { "sse4a", "Support SSE 4a instructions", X86::FeatureSSE4A, X86::FeatureSSE3 },
  { "ssse3", "Enable SSSE3 instructions", X86::FeatureSSSE3, X86::FeatureSSE3 },
  { "tbm", "Enable TBM instructions", X86::FeatureTBM, 0ULL },
  { "vector-unaligned-mem", "Allow unaligned memory operands on vector/SIMD instructions", X86::FeatureVectorUAMem, 0ULL },
  { "xop", "Enable XOP instructions", X86::FeatureXOP, X86::FeatureFMA4 }
};

// Sorted (by key) array of values for CPU subtype.
extern const llvm::SubtargetFeatureKV X86SubTypeKV[] = {
  { "amdfam10", "Select the amdfam10 processor", X86::FeatureSSE4A | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowBTMem, 0ULL },
  { "athlon", "Select the athlon processor", X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-4", "Select the athlon-4 processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-fx", "Select the athlon-fx processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-mp", "Select the athlon-mp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-tbird", "Select the athlon-tbird processor", X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-xp", "Select the athlon-xp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon64", "Select the athlon64 processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "athlon64-sse3", "Select the athlon64-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "atom", "Select the atom processor", X86::ProcIntelAtom | X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeatureSlowBTMem | X86::FeatureLeaForSP | X86::FeatureSlowDivide | X86::FeatureCallRegIndirect | X86::FeatureLEAUsesAG | X86::FeaturePadShortFunctions, 0ULL },
  { "bdver1", "Select the bdver1 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "bdver2", "Select the bdver2 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureTBM | X86::FeatureFMA, 0ULL },
  { "bdver3", "Select the bdver3 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureTBM | X86::FeatureFMA | X86::FeatureFSGSBase, 0ULL },
  { "btver1", "Select the btver1 processor", X86::FeatureSSSE3 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeaturePRFCHW | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "btver2", "Select the btver2 processor", X86::FeatureAVX | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeaturePRFCHW | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureBMI | X86::FeatureF16C | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "c3", "Select the c3 processor", X86::Feature3DNow, 0ULL },
  { "c3-2", "Select the c3-2 processor", X86::FeatureSSE1, 0ULL },
  { "core-avx-i", "Select the core-avx-i processor", X86::FeatureAVX | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase, 0ULL },
  { "core-avx2", "Select the core-avx2 processor", X86::FeatureAVX2 | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureFMA | X86::FeatureRTM | X86::FeatureHLE, 0ULL },
  { "core2", "Select the core2 processor", X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "corei7", "Select the corei7 processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES, 0ULL },
  { "corei7-avx", "Select the corei7-avx processor", X86::FeatureAVX | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL, 0ULL },
  { "generic", "Select the generic processor", 0ULL, 0ULL },
  { "geode", "Select the geode processor", X86::Feature3DNowA, 0ULL },
  { "i386", "Select the i386 processor", 0ULL, 0ULL },
  { "i486", "Select the i486 processor", 0ULL, 0ULL },
  { "i586", "Select the i586 processor", 0ULL, 0ULL },
  { "i686", "Select the i686 processor", 0ULL, 0ULL },
  { "k6", "Select the k6 processor", X86::FeatureMMX, 0ULL },
  { "k6-2", "Select the k6-2 processor", X86::Feature3DNow, 0ULL },
  { "k6-3", "Select the k6-3 processor", X86::Feature3DNow, 0ULL },
  { "k8", "Select the k8 processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "k8-sse3", "Select the k8-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "knl", "Select the knl processor", X86::FeatureAVX512 | X86::FeatureERI | X86::FeatureCDI | X86::FeaturePFI | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureFMA | X86::FeatureRTM | X86::FeatureHLE, 0ULL },
  { "nehalem", "Select the nehalem processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT, 0ULL },
  { "nocona", "Select the nocona processor", X86::FeatureSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "opteron", "Select the opteron processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "opteron-sse3", "Select the opteron-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "penryn", "Select the penryn processor", X86::FeatureSSE41 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "pentium", "Select the pentium processor", 0ULL, 0ULL },
  { "pentium-m", "Select the pentium-m processor", X86::FeatureSSE2 | X86::FeatureSlowBTMem, 0ULL },
  { "pentium-mmx", "Select the pentium-mmx processor", X86::FeatureMMX, 0ULL },
  { "pentium2", "Select the pentium2 processor", X86::FeatureMMX | X86::FeatureCMOV, 0ULL },
  { "pentium3", "Select the pentium3 processor", X86::FeatureSSE1, 0ULL },
  { "pentium3m", "Select the pentium3m processor", X86::FeatureSSE1 | X86::FeatureSlowBTMem, 0ULL },
  { "pentium4", "Select the pentium4 processor", X86::FeatureSSE2, 0ULL },
  { "pentium4m", "Select the pentium4m processor", X86::FeatureSSE2 | X86::FeatureSlowBTMem, 0ULL },
  { "pentiumpro", "Select the pentiumpro processor", X86::FeatureCMOV, 0ULL },
  { "prescott", "Select the prescott processor", X86::FeatureSSE3 | X86::FeatureSlowBTMem, 0ULL },
  { "slm", "Select the slm processor", X86::ProcIntelSLM | X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeaturePOPCNT | X86::FeaturePCLMUL | X86::FeatureAES | X86::FeatureCallRegIndirect | X86::FeaturePRFCHW | X86::FeatureSlowBTMem, 0ULL },
  { "westmere", "Select the westmere processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL, 0ULL },
  { "winchip-c6", "Select the winchip-c6 processor", X86::FeatureMMX, 0ULL },
  { "winchip2", "Select the winchip2 processor", X86::Feature3DNow, 0ULL },
  { "x86-64", "Select the x86-64 processor", X86::FeatureSSE2 | X86::Feature64Bit | X86::FeatureSlowBTMem | X86::FeatureFastUAMem, 0ULL },
  { "yonah", "Select the yonah processor", X86::FeatureSSE3 | X86::FeatureSlowBTMem, 0ULL }
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

// Functional units for "SLMItineraries"
namespace SLMItinerariesFU {
  const unsigned IEC_RSV0 = 1 << 0;
  const unsigned IEC_RSV1 = 1 << 1;
  const unsigned FPC_RSV0 = 1 << 2;
  const unsigned FPC_RSV1 = 1 << 3;
  const unsigned MEC_RSV = 1 << 4;
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
  { 59, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 59
  { 92, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 60
  { 94, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 61
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
  { 1, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 93
  { 2, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 94
  { 2, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 2, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 95-96
  { 1, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 97-98
  { 4, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 4, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 99-100
  { 4, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 101
  { 3, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 3, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 102-103
  { 3, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 104
  { 8, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 105
  { 1, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 106-107
  { 1, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 108
  { 10, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 10, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 109-110
  { 10, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 111
  { 4, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 112
  { 6, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 113
  { 6, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 6, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 114-115
  { 60, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 116
  { 3, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 3, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 117-118
  { 3, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 119
  { 5, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 120
  { 5, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 121-122
  { 26, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 123
  { 38, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 124
  { 25, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 25, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 125-126
  { 25, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 127
  { 27, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 27, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 128-129
  { 27, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 130
  { 13, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 13, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 131-132
  { 13, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 133
  { 12, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 12, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 134-135
  { 12, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 136
  { 15, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 137
  { 88, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 138
  { 296, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 139
  { 281, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 140
  { 25, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 141
  { 74, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 142
  { 6, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 6, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 143-144
  { 6, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 145
  { 35, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 146
  { 49, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 147
  { 34, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 148
  { 4, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 4, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 149-150
  { 10, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 151
  { 1, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 152-153
  { 5, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 154-155
  { 5, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 156
  { 2, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 157
  { 2, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 2, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 158-159
  { 10, SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 10, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 160-161
  { 10, SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 162
  { 9, SLMItinerariesFU::FPC_RSV0 | SLMItinerariesFU::FPC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 9, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 163-164
  { 4, SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 4, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 165-166
  { 4, SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 167
  { 1, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 168-169
  { 1, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 170
  { 2, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 2, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 171-172
  { 2, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 173
  { 1, SLMItinerariesFU::IEC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 174-175
  { 9, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 9, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 176-177
  { 9, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 178
  { 4, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 4, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 179-180
  { 4, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 181
  { 30, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 182
  { 26, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 26, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 183-184
  { 26, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 185
  { 13, SLMItinerariesFU::FPC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 13, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 186-187
  { 13, SLMItinerariesFU::FPC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 188
  { 4, SLMItinerariesFU::IEC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 4, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 189-190
  { 2, SLMItinerariesFU::IEC_RSV0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 2, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 191-192
  { 4, SLMItinerariesFU::IEC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 193
  { 2, SLMItinerariesFU::IEC_RSV0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 194
  { 5, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, SLMItinerariesFU::MEC_RSV, -1, (llvm::InstrStage::ReservationKinds)0 }, // 195-196
  { 5, SLMItinerariesFU::IEC_RSV0 | SLMItinerariesFU::IEC_RSV1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 197
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

static const llvm::InstrItinerary *NoItineraries = 0;

static const llvm::InstrItinerary AtomItineraries[] = {
  { 0, 0, 0, 0, 0 }, // 0 NoInstrModel
  { 1, 1, 2, 0, 0 }, // 1 IIC_AAA_WriteMicrocoded
  { 1, 2, 3, 0, 0 }, // 2 IIC_AAD_WriteMicrocoded
  { 1, 3, 4, 0, 0 }, // 3 IIC_AAM_WriteMicrocoded
  { 1, 1, 2, 0, 0 }, // 4 IIC_AAS_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 5 IIC_BIN_CARRY_NONMEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 6 IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW
  { 1, 5, 6, 0, 0 }, // 7 IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 8 IIC_BIN_MEM_WriteALULd
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
  { 0, 0, 0, 0, 0 }, // 23 IIC_AES
  { 0, 0, 0, 0, 0 }, // 24 WriteVecLogicLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 25 WriteVecLogic
  { 1, 10, 11, 0, 0 }, // 26 IIC_ARPL_MEM_WriteSystem
  { 1, 11, 12, 0, 0 }, // 27 IIC_ARPL_REG_WriteSystem
  { 0, 0, 0, 0, 0 }, // 28 WriteZero
  { 1, 5, 6, 0, 0 }, // 29 IIC_SSE_INTALU_P_RM
  { 1, 4, 5, 0, 0 }, // 30 IIC_SSE_INTALU_P_RR
  { 1, 5, 6, 0, 0 }, // 31 IIC_ALU_MEM
  { 1, 4, 5, 0, 0 }, // 32 IIC_ALU_NONMEM
  { 1, 12, 13, 0, 0 }, // 33 IIC_BOUND_WriteSystem
  { 1, 13, 14, 0, 0 }, // 34 IIC_BIT_SCAN_MEM_WriteShiftLd
  { 1, 13, 14, 0, 0 }, // 35 IIC_BIT_SCAN_REG_WriteShift
  { 1, 5, 6, 0, 0 }, // 36 IIC_BSWAP_WriteALU
  { 1, 4, 5, 0, 0 }, // 37 IIC_BT_MI_WriteALU
  { 1, 14, 15, 0, 0 }, // 38 IIC_BT_MR_WriteALULd
  { 1, 15, 16, 0, 0 }, // 39 IIC_BT_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 40 IIC_BT_RR_WriteALU
  { 1, 16, 17, 0, 0 }, // 41 IIC_BTX_MI_WriteALULd_WriteRMW
  { 1, 12, 13, 0, 0 }, // 42 IIC_BTX_MR_WriteALULd_WriteRMW
  { 1, 15, 16, 0, 0 }, // 43 IIC_BTX_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 44 IIC_BTX_RR_WriteALU
  { 1, 17, 18, 0, 0 }, // 45 IIC_CALL_MEM_WriteJumpLd
  { 1, 18, 20, 0, 0 }, // 46 IIC_CALL_RI_WriteJump
  { 1, 17, 18, 0, 0 }, // 47 IIC_CALL_MEM_WriteJump
  { 1, 20, 21, 0, 0 }, // 48 IIC_CBW
  { 1, 4, 5, 0, 0 }, // 49 IIC_CLC_WriteALU
  { 1, 21, 22, 0, 0 }, // 50 IIC_CLD_WriteALU
  { 1, 5, 6, 0, 0 }, // 51 IIC_SSE_PREFETCH_WriteLoad
  { 1, 22, 23, 0, 0 }, // 52 IIC_CLI_WriteALU
  { 1, 23, 24, 0, 0 }, // 53 IIC_CLTS_WriteALU
  { 1, 4, 5, 0, 0 }, // 54 IIC_CMC_WriteALU
  { 1, 5, 6, 0, 0 }, // 55 IIC_CMOV16_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 56 IIC_CMOV16_RR_WriteALU
  { 1, 5, 6, 0, 0 }, // 57 IIC_CMOV32_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 58 IIC_CMOV32_RR_WriteALU
  { 1, 21, 22, 0, 0 }, // 59 IIC_CMPS_WriteMicrocoded
  { 1, 24, 25, 0, 0 }, // 60 IIC_CMPXCHG_16B_WriteALULd_WriteRMW
  { 1, 22, 23, 0, 0 }, // 61 IIC_CMPXCHG_MEM_WriteALULd_WriteRMW
  { 1, 17, 18, 0, 0 }, // 62 IIC_CMPXCHG_REG_WriteALU
  { 1, 25, 26, 0, 0 }, // 63 IIC_CMPXCHG_8B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 64 IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 65 IIC_CMPXCHG_REG8_WriteALU
  { 1, 26, 27, 0, 0 }, // 66 IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 67 IIC_SSE_COMIS_RR_WriteFAdd
  { 1, 14, 15, 0, 0 }, // 68 IIC_FCOMI_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 69 IIC_CPUID_WriteSystem
  { 0, 0, 0, 0, 0 }, // 70 IIC_CRC32_MEM
  { 0, 0, 0, 0, 0 }, // 71 IIC_CRC32_REG
  { 1, 2, 3, 0, 0 }, // 72 IIC_SSE_CVT_PD_RR_WriteCvtI2FLd
  { 1, 28, 29, 0, 0 }, // 73 IIC_SSE_CVT_PD_RM_WriteCvtI2F
  { 1, 2, 3, 0, 0 }, // 74 IIC_SSE_CVT_PS_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 75 IIC_SSE_CVT_PS_RR_WriteCvtI2F
  { 1, 28, 29, 0, 0 }, // 76 IIC_SSE_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 77 IIC_SSE_CVT_PD_RR_WriteCvtF2I
  { 1, 28, 29, 0, 0 }, // 78 IIC_SSE_CVT_PD_RM_WriteCvtF2FLd
  { 1, 2, 3, 0, 0 }, // 79 IIC_SSE_CVT_PD_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 80 IIC_SSE_CVT_PS_RM_WriteCvtF2ILd
  { 1, 6, 7, 0, 0 }, // 81 IIC_SSE_CVT_PS_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 82 IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 83 IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I
  { 1, 2, 3, 0, 0 }, // 84 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd
  { 1, 6, 7, 0, 0 }, // 85 IIC_SSE_CVT_Scalar_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 86 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 87 IIC_SSE_CVT_Scalar_RR_WriteCvtI2F
  { 1, 26, 27, 0, 0 }, // 88 IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd
  { 1, 14, 15, 0, 0 }, // 89 IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 90 IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 91 IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I
  { 1, 25, 26, 0, 0 }, // 92 IIC_DAA_WriteMicrocoded
  { 1, 29, 30, 0, 0 }, // 93 IIC_DAS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 94 IIC_UNARY_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 95 IIC_UNARY_REG_WriteALU
  { 1, 30, 31, 0, 0 }, // 96 IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 97 IIC_DIV16_WriteIDiv
  { 1, 30, 31, 0, 0 }, // 98 IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 99 IIC_DIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 100 IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 101 IIC_DIV64_WriteIDiv
  { 1, 32, 33, 0, 0 }, // 102 IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 103 IIC_DIV8_REG_WriteIDiv
  { 1, 33, 34, 0, 0 }, // 104 IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd
  { 1, 33, 34, 0, 0 }, // 105 IIC_SSE_DIV_F64P_RR_WriteFDiv
  { 1, 34, 35, 0, 0 }, // 106 IIC_SSE_DIV_F32P_RR_WriteFDiv
  { 1, 35, 36, 0, 0 }, // 107 IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 108 IIC_SSE_DIV_F64S_RR_WriteFDiv
  { 1, 36, 37, 0, 0 }, // 109 IIC_SSE_DIV_F32S_RR_WriteFDiv
  { 0, 0, 0, 0, 0 }, // 110 IIC_SSE_DPPD_RM
  { 0, 0, 0, 0, 0 }, // 111 IIC_SSE_DPPD_RR
  { 0, 0, 0, 0, 0 }, // 112 IIC_SSE_DPPS_RR
  { 1, 37, 38, 0, 0 }, // 113 IIC_RET_WriteSystem
  { 0, 0, 0, 0, 0 }, // 114 WriteSystem
  { 1, 38, 39, 0, 0 }, // 115 IIC_ENTER_WriteMicrocoded
  { 0, 0, 0, 0, 0 }, // 116 IIC_SSE_EXTRACTPS_RM
  { 0, 0, 0, 0, 0 }, // 117 IIC_SSE_EXTRACTPS_RR
  { 1, 39, 40, 0, 0 }, // 118 IIC_F2XM1_WriteMicrocoded
  { 1, 40, 41, 0, 0 }, // 119 IIC_CALL_FAR_PTR_WriteJump
  { 1, 41, 42, 0, 0 }, // 120 IIC_CALL_FAR_MEM_WriteJumpLd
  { 1, 41, 42, 0, 0 }, // 121 IIC_CALL_FAR_MEM_WriteJump
  { 1, 42, 43, 0, 0 }, // 122 IIC_JMP_FAR_PTR_WriteJump
  { 1, 38, 39, 0, 0 }, // 123 IIC_JMP_FAR_MEM_WriteJumpLd
  { 1, 38, 39, 0, 0 }, // 124 IIC_JMP_FAR_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 125 IIC_FCOMPP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 126 IIC_FPSTP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 127 IIC_FFREE_WriteMicrocoded
  { 1, 43, 44, 0, 0 }, // 128 IIC_FLDCW_WriteLoad
  { 1, 26, 27, 0, 0 }, // 129 IIC_FLDL_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 130 IIC_FNCLEX_WriteMicrocoded
  { 1, 45, 46, 0, 0 }, // 131 IIC_FNINIT_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 132 IIC_FNOP_WriteMicrocoded
  { 1, 28, 29, 0, 0 }, // 133 IIC_FNSTCW_WriteALU
  { 1, 26, 27, 0, 0 }, // 134 IIC_FNSTSW_WriteALU
  { 1, 46, 47, 0, 0 }, // 135 IIC_FPATAN_WriteMicrocoded
  { 1, 47, 48, 0, 0 }, // 136 IIC_FPREM_WriteMicrocoded
  { 1, 48, 49, 0, 0 }, // 137 IIC_FPREM1_WriteMicrocoded
  { 1, 49, 50, 0, 0 }, // 138 IIC_FPTAN_WriteMicrocoded
  { 1, 50, 51, 0, 0 }, // 139 IIC_FRNDINT_WriteMicrocoded
  { 1, 51, 52, 0, 0 }, // 140 IIC_FSCALE_WriteMicrocoded
  { 1, 52, 53, 0, 0 }, // 141 IIC_FSINCOS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 142 IIC_FXAM_WriteMicrocoded
  { 1, 53, 54, 0, 0 }, // 143 IIC_FXRSTOR_WriteMicrocoded
  { 1, 54, 55, 0, 0 }, // 144 IIC_FXSAVE_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 145 IIC_FXTRACT_WriteMicrocoded
  { 1, 55, 56, 0, 0 }, // 146 IIC_FYL2X_WriteMicrocoded
  { 1, 56, 57, 0, 0 }, // 147 IIC_FYL2XP1_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 148 IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 149 IIC_SSE_BIT_P_RR_WriteFAdd
  { 1, 5, 6, 0, 0 }, // 150 IIC_SSE_MOVA_P_RM_WriteLoad
  { 1, 14, 15, 0, 0 }, // 151 IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd
  { 1, 28, 29, 0, 0 }, // 152 IIC_SSE_HADDSUB_RR_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 153 IIC_HLT_WriteSystem
  { 1, 35, 36, 0, 0 }, // 154 IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 155 IIC_IDIV16_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 156 IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 157 IIC_IDIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 158 IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 159 IIC_IDIV64_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 160 IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 161 IIC_IDIV8_WriteIDiv
  { 1, 7, 9, 0, 0 }, // 162 IIC_FILD_WriteLoad
  { 1, 28, 29, 0, 0 }, // 163 IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 164 IIC_IMUL16_RR_WriteIMul
  { 1, 2, 3, 0, 0 }, // 165 IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 166 IIC_IMUL16_RMI_WriteIMulLd
  { 1, 6, 7, 0, 0 }, // 167 IIC_IMUL16_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 168 IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 169 IIC_IMUL32_RR_WriteIMul
  { 1, 57, 58, 0, 0 }, // 170 IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 171 IIC_IMUL32_RMI_WriteIMulLd
  { 1, 57, 58, 0, 0 }, // 172 IIC_IMUL32_RRI_WriteIMul
  { 1, 58, 59, 0, 0 }, // 173 IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 174 IIC_IMUL64_RR_WriteIMul
  { 1, 58, 59, 0, 0 }, // 175 IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd
  { 1, 22, 23, 0, 0 }, // 176 IIC_IMUL64_RMI_WriteIMulLd
  { 1, 22, 23, 0, 0 }, // 177 IIC_IMUL64_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 178 IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 179 IIC_IMUL8_WriteIMul
  { 1, 59, 60, 0, 0 }, // 180 IIC_INS_WriteSystem
  { 1, 60, 61, 0, 0 }, // 181 IIC_IN_RI_WriteSystem
  { 1, 61, 62, 0, 0 }, // 182 IIC_IN_RR_WriteSystem
  { 0, 0, 0, 0, 0 }, // 183 IIC_SSE_INSERTPS_RM
  { 0, 0, 0, 0, 0 }, // 184 IIC_SSE_INSERTPS_RR
  { 1, 62, 63, 0, 0 }, // 185 IIC_INT_WriteSystem
  { 1, 31, 32, 0, 0 }, // 186 IIC_INT3_WriteSystem
  { 1, 63, 64, 0, 0 }, // 187 IIC_INVD_WriteSystem
  { 1, 48, 49, 0, 0 }, // 188 IIC_INVLPG_WriteSystem
  { 1, 64, 65, 0, 0 }, // 189 IIC_IRET_WriteSystem
  { 1, 16, 17, 0, 0 }, // 190 IIC_FST_WriteStore
  { 1, 6, 7, 0, 0 }, // 191 IIC_FIST_WriteStore
  { 1, 2, 3, 0, 0 }, // 192 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 193 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 194 WriteLoad
  { 1, 15, 16, 0, 0 }, // 195 IIC_Jcc_WriteJump
  { 1, 20, 21, 0, 0 }, // 196 IIC_JCXZ_WriteJump
  { 1, 16, 17, 0, 0 }, // 197 IIC_JMP_MEM_WriteJumpLd
  { 1, 15, 16, 0, 0 }, // 198 IIC_JMP_REG_WriteJump
  { 1, 15, 16, 0, 0 }, // 199 IIC_JMP_REL_WriteJump
  { 1, 4, 5, 0, 0 }, // 200 IIC_AHF_WriteALU
  { 1, 30, 31, 0, 0 }, // 201 IIC_LAR_RM_WriteSystem
  { 1, 65, 66, 0, 0 }, // 202 IIC_LAR_RR_WriteSystem
  { 1, 22, 23, 0, 0 }, // 203 IIC_CMPX_LOCK_WriteALULd_WriteRMW
  { 1, 24, 25, 0, 0 }, // 204 IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 205 IIC_CMPX_LOCK_8_WriteALULd_WriteRMW
  { 1, 25, 26, 0, 0 }, // 206 IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW
  { 1, 21, 22, 0, 0 }, // 207 IIC_SSE_LDDQU_WriteLoad
  { 1, 43, 44, 0, 0 }, // 208 IIC_SSE_LDMXCSR_WriteLoad
  { 1, 26, 27, 0, 0 }, // 209 IIC_LXS_WriteSystem
  { 1, 4, 5, 0, 0 }, // 210 IIC_FLDZ_WriteZero
  { 1, 6, 7, 0, 0 }, // 211 IIC_FIST_WriteZero
  { 1, 5, 6, 0, 0 }, // 212 IIC_FLD_WriteLoad
  { 1, 20, 21, 0, 0 }, // 213 IIC_FLD80_WriteLoad
  { 1, 5, 6, 0, 0 }, // 214 IIC_FLD_WriteMove
  { 1, 16, 17, 0, 0 }, // 215 IIC_LEA_16_WriteLEA
  { 1, 15, 16, 0, 0 }, // 216 IIC_LEA_WriteLEA
  { 1, 16, 17, 0, 0 }, // 217 IIC_LEAVE_WriteALU
  { 1, 4, 5, 0, 0 }, // 218 IIC_SSE_LFENCE_WriteLoad
  { 1, 66, 67, 0, 0 }, // 219 IIC_LGDT_WriteSystem
  { 1, 66, 67, 0, 0 }, // 220 IIC_LIDT_WriteSystem
  { 1, 67, 68, 0, 0 }, // 221 IIC_LLDT_MEM_WriteSystem
  { 1, 68, 69, 0, 0 }, // 222 IIC_LLDT_REG_WriteSystem
  { 1, 69, 70, 0, 0 }, // 223 IIC_LMSW_REG_WriteSystem
  { 1, 70, 71, 0, 0 }, // 224 IIC_LMSW_MEM_WriteSystem
  { 1, 5, 6, 0, 0 }, // 225 IIC_ALU_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 226 IIC_ALU_NONMEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 227 IIC_LODS_WriteMicrocoded
  { 1, 25, 26, 0, 0 }, // 228 IIC_LOOP_WriteJump
  { 1, 28, 29, 0, 0 }, // 229 IIC_LOOPE_WriteJump
  { 1, 71, 72, 0, 0 }, // 230 IIC_LOOPNE_WriteJump
  { 1, 37, 38, 0, 0 }, // 231 IIC_RET_WriteJumpLd
  { 1, 50, 51, 0, 0 }, // 232 IIC_LSL_RM_WriteSystem
  { 1, 72, 73, 0, 0 }, // 233 IIC_LSL_RR_WriteSystem
  { 1, 73, 74, 0, 0 }, // 234 IIC_LTR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 235 IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 236 IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 237 IIC_SSE_MASKMOV_WriteStore
  { 1, 5, 6, 0, 0 }, // 238 IIC_SSE_MFENCE_WriteLoad
  { 1, 28, 29, 0, 0 }, // 239 IIC_MMX_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 240 IIC_MMX_CVT_PD_RR_WriteCvtF2I
  { 1, 7, 9, 0, 0 }, // 241 IIC_MMX_CVT_PS_RM_WriteCvtF2ILd
  { 1, 9, 10, 0, 0 }, // 242 IIC_MMX_CVT_PS_RR_WriteCvtF2I
  { 1, 43, 44, 0, 0 }, // 243 IIC_MMX_EMMS
  { 1, 5, 6, 0, 0 }, // 244 IIC_MMX_MASKMOV_WriteShuffle
  { 1, 74, 75, 0, 0 }, // 245 IIC_MMX_MOV_REG_MM_WriteMove
  { 1, 5, 6, 0, 0 }, // 246 IIC_MMX_MOV_MM_RM_WriteStore
  { 1, 5, 6, 0, 0 }, // 247 IIC_MMX_MOV_MM_RM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 248 IIC_MMX_MOV_MM_RM_WriteMove
  { 1, 4, 5, 0, 0 }, // 249 IIC_MMX_MOVQ_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 250 IIC_MMX_MOVQ_RM_WriteStore
  { 0, 0, 0, 0, 0 }, // 251 WriteMove
  { 1, 5, 6, 0, 0 }, // 252 IIC_MMX_MOVQ_RM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 253 IIC_MMX_ALU_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 254 IIC_MMX_ALU_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 255 IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 256 IIC_MMX_PCK_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 257 IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 258 IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 259 IIC_MMX_ALUQ_RR_WriteVecALU
  { 1, 4, 5, 0, 0 }, // 260 IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 261 IIC_MMX_MISC_FUNC_MEM_WriteVecALU
  { 1, 20, 21, 0, 0 }, // 262 IIC_MMX_PEXTR_WriteShuffle
  { 1, 6, 7, 0, 0 }, // 263 IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 43, 44, 0, 0 }, // 264 IIC_MMX_PHADDSUBW_RR_WriteVecALU
  { 1, 20, 21, 0, 0 }, // 265 IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 266 IIC_MMX_PHADDSUBD_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 267 IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 268 IIC_MMX_PINSRW_WriteShuffle
  { 1, 75, 76, 0, 0 }, // 269 IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd
  { 1, 75, 76, 0, 0 }, // 270 IIC_MMX_PMUL_WriteVecIMul
  { 1, 20, 21, 0, 0 }, // 271 IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 272 IIC_MMX_PSADBW_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 273 IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 274 IIC_MMX_PSHUF_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 275 IIC_MMX_PSHUF_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 276 IIC_MMX_SHIFT_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 277 IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 278 IIC_MMX_SHIFT_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 279 IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 280 IIC_MMX_UNPCK_H_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 281 IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 282 IIC_MMX_UNPCK_L_WriteShuffle
  { 1, 76, 77, 0, 0 }, // 283 IIC_SSE_MONITOR_WriteSystem
  { 1, 5, 6, 0, 0 }, // 284 IIC_MOV_MEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 285 IIC_MOV_MEM_WriteStore
  { 1, 16, 17, 0, 0 }, // 286 IIC_MOV_MEM_SR_WriteMove
  { 1, 4, 5, 0, 0 }, // 287 IIC_MOV_WriteMove
  { 1, 5, 6, 0, 0 }, // 288 IIC_MOV_MEM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 289 IIC_MOV_REG_SR_WriteMove
  { 1, 77, 78, 0, 0 }, // 290 IIC_MOV_SR_MEM_WriteMove
  { 1, 3, 4, 0, 0 }, // 291 IIC_MOV_SR_REG_WriteMove
  { 1, 78, 79, 0, 0 }, // 292 IIC_MOV_CR_REG_WriteSystem
  { 1, 79, 80, 0, 0 }, // 293 IIC_MOV_DR_REG_WriteSystem
  { 1, 4, 5, 0, 0 }, // 294 IIC_ALU_NONMEM_WriteZero
  { 1, 58, 59, 0, 0 }, // 295 IIC_MOV_REG_CR_WriteSystem
  { 1, 80, 81, 0, 0 }, // 296 IIC_MOV_REG_DR_WriteSystem
  { 1, 4, 5, 0, 0 }, // 297 IIC_ALU_NONMEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 298 IIC_SSE_MOVDQ_WriteMove
  { 1, 5, 6, 0, 0 }, // 299 IIC_SSE_MOVDQ_WriteLoad
  { 1, 5, 6, 0, 0 }, // 300 IIC_SSE_MOVA_P_MR_WriteStore
  { 1, 4, 5, 0, 0 }, // 301 IIC_SSE_MOVA_P_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 302 IIC_MOVBE_WriteStore
  { 1, 5, 6, 0, 0 }, // 303 IIC_MOVBE_WriteALULd
  { 1, 5, 6, 0, 0 }, // 304 IIC_SSE_MOV_LH_WriteShuffleLd
  { 1, 5, 6, 0, 0 }, // 305 IIC_SSE_MOV_LH_WriteShuffle
  { 1, 16, 17, 0, 0 }, // 306 IIC_SSE_MOVU_P_MR_WriteStore
  { 1, 21, 22, 0, 0 }, // 307 IIC_SSE_MOVU_P_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 308 IIC_SSE_MOVU_P_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 309 IIC_SSE_MOV_LH_WriteStore
  { 1, 5, 6, 0, 0 }, // 310 IIC_SSE_MOV_LH_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 311 IIC_SSE_MOVDQ_WriteStore
  { 1, 74, 75, 0, 0 }, // 312 IIC_SSE_MOVMSK_WriteVecLogic
  { 1, 5, 6, 0, 0 }, // 313 IIC_SSE_MOVNT_WriteStore
  { 1, 74, 75, 0, 0 }, // 314 IIC_SSE_MOVD_ToGP_WriteMove
  { 1, 21, 22, 0, 0 }, // 315 IIC_MOVS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 316 IIC_SSE_MOV_S_MR_WriteStore
  { 1, 5, 6, 0, 0 }, // 317 IIC_SSE_MOV_S_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 318 IIC_SSE_MOV_S_RR_WriteMove
  { 1, 21, 22, 0, 0 }, // 319 IIC_MOVSX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 320 IIC_MOVSX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 321 IIC_MOVSX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 322 IIC_MOVSX_WriteALU
  { 1, 5, 6, 0, 0 }, // 323 IIC_SSE_MOVDQ_WriteVecLogicLd
  { 1, 4, 5, 0, 0 }, // 324 IIC_SSE_MOVQ_RR_WriteVecLogic
  { 1, 21, 22, 0, 0 }, // 325 IIC_MOVZX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 326 IIC_MOVZX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 327 IIC_MOVZX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 328 IIC_MOVZX_WriteALU
  { 1, 57, 58, 0, 0 }, // 329 IIC_SSE_INTMUL_P_RM
  { 1, 57, 58, 0, 0 }, // 330 IIC_SSE_INTMUL_P_RR
  { 1, 28, 29, 0, 0 }, // 331 IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 332 IIC_MUL16_REG_WriteIMul
  { 1, 2, 3, 0, 0 }, // 333 IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 334 IIC_MUL32_REG_WriteIMul
  { 1, 58, 59, 0, 0 }, // 335 IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 336 IIC_MUL64_WriteIMul
  { 1, 2, 3, 0, 0 }, // 337 IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 338 IIC_MUL8_WriteIMul
  { 1, 26, 27, 0, 0 }, // 339 IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 340 IIC_SSE_MUL_F64P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 341 IIC_SSE_MUL_F32P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 342 IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 343 IIC_SSE_MUL_F64S_RR_WriteFMul
  { 1, 75, 76, 0, 0 }, // 344 IIC_SSE_MUL_F32S_RR_WriteFMul
  { 1, 2, 3, 0, 0 }, // 345 IIC_MUL8_WriteIMulLd_WriteIMulH
  { 1, 2, 3, 0, 0 }, // 346 IIC_MUL8_WriteIMul_WriteIMulH
  { 1, 50, 51, 0, 0 }, // 347 IIC_SSE_MWAIT_WriteSystem
  { 1, 4, 5, 0, 0 }, // 348 IIC_NOP_WriteZero
  { 1, 81, 82, 0, 0 }, // 349 IIC_OUT_IR_WriteSystem
  { 1, 32, 33, 0, 0 }, // 350 IIC_OUT_RR_WriteSystem
  { 1, 82, 83, 0, 0 }, // 351 IIC_OUTS_WriteSystem
  { 1, 5, 6, 0, 0 }, // 352 IIC_SSE_PABS_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 353 IIC_SSE_PABS_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 354 IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 355 IIC_SSE_INTALU_P_RR_WriteVecALU
  { 1, 21, 22, 0, 0 }, // 356 IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 357 IIC_SSE_INTALUQ_P_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 358 IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 359 IIC_SSE_PALIGNRR_WriteShuffle
  { 1, 71, 72, 0, 0 }, // 360 IIC_SSE_PAUSE_WriteLoad
  { 0, 0, 0, 0, 0 }, // 361 IIC_SSE_PCLMULQDQ_RM
  { 0, 0, 0, 0, 0 }, // 362 IIC_SSE_PCLMULQDQ_RR
  { 1, 20, 21, 0, 0 }, // 363 IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 364 IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 365 IIC_SSE_PHADDSUBD_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 366 WriteVecALULd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 367 WriteVecALU
  { 1, 28, 29, 0, 0 }, // 368 IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 369 IIC_SSE_PHADDSUBW_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 370 IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 371 IIC_SSE_PINSRW_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 372 WriteVecIMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 373 WriteVecIMul
  { 1, 57, 58, 0, 0 }, // 374 IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 375 IIC_SSE_PMADD_WriteVecIMul
  { 1, 57, 58, 0, 0 }, // 376 IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 377 IIC_SSE_INTMUL_P_RR_WriteVecIMul
  { 1, 16, 17, 0, 0 }, // 378 IIC_POP_REG16_WriteLoad
  { 1, 21, 22, 0, 0 }, // 379 IIC_POP_MEM_WriteLoad
  { 1, 18, 20, 0, 0 }, // 380 IIC_POP_REG_WriteLoad
  { 1, 14, 15, 0, 0 }, // 381 IIC_POP_A_WriteLoad
  { 0, 0, 0, 0, 0 }, // 382 IIC_SSE_POPCNT_RM
  { 0, 0, 0, 0, 0 }, // 383 IIC_SSE_POPCNT_RR
  { 1, 83, 84, 0, 0 }, // 384 IIC_POP_SR_WriteSystem
  { 1, 38, 39, 0, 0 }, // 385 IIC_POP_F_WriteLoad
  { 1, 77, 78, 0, 0 }, // 386 IIC_POP_FD_WriteLoad
  { 1, 84, 85, 0, 0 }, // 387 IIC_POP_SR_SS_WriteSystem
  { 1, 43, 44, 0, 0 }, // 388 IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 389 IIC_SSE_PSHUFB_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 390 IIC_SSE_PSHUF_MI_WriteShuffleLd
  { 1, 5, 6, 0, 0 }, // 391 IIC_SSE_PSHUF_RI_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 392 IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 393 IIC_SSE_PSIGN_RR_WriteVecALU
  { 1, 4, 5, 0, 0 }, // 394 IIC_SSE_INTSHDQ_P_RI_WriteVecShift
  { 1, 4, 5, 0, 0 }, // 395 IIC_SSE_INTSH_P_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 396 IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 397 IIC_SSE_INTSH_P_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 398 IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 399 IIC_SSE_UNPCK_WriteShuffle
  { 1, 18, 20, 0, 0 }, // 400 IIC_PUSH_REG_WriteStore
  { 1, 16, 17, 0, 0 }, // 401 IIC_PUSH_MEM_WriteStore
  { 1, 18, 20, 0, 0 }, // 402 IIC_PUSH_IMM_WriteStore
  { 1, 28, 29, 0, 0 }, // 403 IIC_PUSH_A_WriteStore
  { 1, 16, 17, 0, 0 }, // 404 IIC_PUSH_SR_WriteSystem
  { 1, 16, 17, 0, 0 }, // 405 IIC_PUSH_CS_WriteSystem
  { 1, 14, 15, 0, 0 }, // 406 IIC_PUSH_F_WriteStore
  { 1, 5, 6, 0, 0 }, // 407 IIC_SR_WriteShiftLd_WriteRMW
  { 1, 5, 6, 0, 0 }, // 408 IIC_SR_WriteShift
  { 1, 26, 27, 0, 0 }, // 409 IIC_SSE_RCPP_RM_WriteFRcpLd
  { 1, 14, 15, 0, 0 }, // 410 IIC_SSE_RCPP_RR_WriteFRcp
  { 1, 75, 76, 0, 0 }, // 411 IIC_SSE_RCPS_RM_WriteFRcpLd
  { 1, 75, 76, 0, 0 }, // 412 IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 413 WriteFRcp
  { 1, 75, 76, 0, 0 }, // 414 IIC_SSE_RCPS_RR_WriteFRcp
  { 1, 85, 86, 0, 0 }, // 415 IIC_RDMSR_WriteSystem
  { 1, 50, 51, 0, 0 }, // 416 IIC_RDPMC_WriteSystem
  { 1, 86, 87, 0, 0 }, // 417 IIC_RDTSC_WriteSystem
  { 1, 87, 88, 0, 0 }, // 418 IIC_REP_MOVS_WriteMicrocoded
  { 1, 82, 83, 0, 0 }, // 419 IIC_REP_STOS_WriteMicrocoded
  { 1, 18, 20, 0, 0 }, // 420 IIC_RET_IMM_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 421 WriteShiftLd
  { 0, 0, 0, 0, 0 }, // 422 WriteShift
  { 0, 0, 0, 0, 0 }, // 423 IIC_SSE_ROUNDPS_REG
  { 0, 0, 0, 0, 0 }, // 424 IIC_SSE_ROUNDPS_MEM
  { 1, 88, 89, 0, 0 }, // 425 IIC_RSM_WriteSystem
  { 1, 34, 35, 0, 0 }, // 426 IIC_SSE_SQRTPS_RM_WriteFSqrtLd
  { 1, 34, 35, 0, 0 }, // 427 IIC_SSE_SQRTPS_RR_WriteFSqrt
  { 1, 36, 37, 0, 0 }, // 428 IIC_SSE_SQRTSS_RM_WriteFSqrtLd
  { 1, 36, 37, 0, 0 }, // 429 IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 430 WriteFSqrt
  { 1, 36, 37, 0, 0 }, // 431 IIC_SSE_SQRTSS_RR_WriteFSqrt
  { 0, 0, 0, 0, 0 }, // 432 WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 433 IIC_SCAS_WriteMicrocoded
  { 1, 16, 17, 0, 0 }, // 434 IIC_SET_M_WriteALU_WriteStore
  { 1, 4, 5, 0, 0 }, // 435 IIC_SET_R_WriteALU
  { 1, 5, 6, 0, 0 }, // 436 IIC_SSE_SFENCE_WriteLoad
  { 1, 20, 21, 0, 0 }, // 437 IIC_SGDT_WriteSystem
  { 1, 6, 7, 0, 0 }, // 438 IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 439 IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 440 IIC_SHD16_REG_CL_WriteShift
  { 1, 6, 7, 0, 0 }, // 441 IIC_SHD16_REG_IM_WriteShift
  { 1, 20, 21, 0, 0 }, // 442 IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 20, 21, 0, 0 }, // 443 IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 444 IIC_SHD32_REG_CL_WriteShift
  { 1, 16, 17, 0, 0 }, // 445 IIC_SHD32_REG_IM_WriteShift
  { 1, 14, 15, 0, 0 }, // 446 IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 447 IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 28, 29, 0, 0 }, // 448 IIC_SHD64_REG_CL_WriteShift
  { 1, 14, 15, 0, 0 }, // 449 IIC_SHD64_REG_IM_WriteShift
  { 1, 5, 6, 0, 0 }, // 450 IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 451 IIC_SSE_SHUFP_WriteShuffle
  { 1, 20, 21, 0, 0 }, // 452 IIC_SIDT_WriteSystem
  { 1, 21, 22, 0, 0 }, // 453 IIC_SLDT_WriteSystem
  { 1, 14, 15, 0, 0 }, // 454 IIC_SMSW_WriteSystem
  { 1, 33, 34, 0, 0 }, // 455 IIC_SSE_SQRTPD_RM_WriteFSqrtLd
  { 1, 33, 34, 0, 0 }, // 456 IIC_SSE_SQRTPD_RR_WriteFSqrt
  { 1, 35, 36, 0, 0 }, // 457 IIC_SSE_SQRTSD_RM_WriteFSqrtLd
  { 1, 35, 36, 0, 0 }, // 458 IIC_SSE_SQRTSD_RR_WriteFSqrt
  { 1, 4, 5, 0, 0 }, // 459 IIC_STC_WriteALU
  { 1, 3, 4, 0, 0 }, // 460 IIC_STD_WriteALU
  { 1, 71, 72, 0, 0 }, // 461 IIC_STI_WriteALU
  { 1, 17, 18, 0, 0 }, // 462 IIC_SSE_STMXCSR_WriteStore
  { 1, 4, 5, 0, 0 }, // 463 IIC_STOS_WriteMicrocoded
  { 1, 21, 22, 0, 0 }, // 464 IIC_STR_WriteSystem
  { 1, 43, 44, 0, 0 }, // 465 IIC_FST80_WriteStore
  { 1, 16, 17, 0, 0 }, // 466 IIC_FST_WriteMove
  { 1, 24, 25, 0, 0 }, // 467 IIC_SWAPGS_WriteSystem
  { 1, 89, 90, 0, 0 }, // 468 IIC_SYSCALL_WriteSystem
  { 1, 80, 81, 0, 0 }, // 469 IIC_SYS_ENTER_EXIT_WriteSystem
  { 1, 15, 16, 0, 0 }, // 470 IIC_JMP_REL_WriteJumpLd
  { 1, 16, 17, 0, 0 }, // 471 IIC_JMP_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 472 IIC_JMP_REG_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 473 WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 474 WriteJump
  { 1, 14, 15, 0, 0 }, // 475 IIC_FUCOMI_WriteFAdd
  { 1, 15, 16, 0, 0 }, // 476 IIC_FUCOM_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 477 WriteFAdd
  { 1, 2, 3, 0, 0 }, // 478 IIC_SSE_ALU_F64P_RM
  { 1, 6, 7, 0, 0 }, // 479 IIC_SSE_ALU_F64P_RR
  { 1, 7, 9, 0, 0 }, // 480 IIC_SSE_ALU_F32P_RM
  { 1, 9, 10, 0, 0 }, // 481 IIC_SSE_ALU_F32P_RR
  { 0, 0, 0, 0, 0 }, // 482 WriteCvtI2FLd
  { 0, 0, 0, 0, 0 }, // 483 WriteCvtI2F
  { 0, 0, 0, 0, 0 }, // 484 WriteCvtF2ILd
  { 0, 0, 0, 0, 0 }, // 485 WriteCvtF2I
  { 1, 2, 3, 0, 0 }, // 486 IIC_SSE_CVT_PS_RM
  { 1, 6, 7, 0, 0 }, // 487 IIC_SSE_CVT_PS_RR
  { 0, 0, 0, 0, 0 }, // 488 WriteCvtF2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 489 WriteCvtF2F
  { 0, 0, 0, 0, 0 }, // 490 WriteCvtI2FLd_ReadAfterLd
  { 1, 90, 91, 0, 0 }, // 491 IIC_VERR_WriteSystem
  { 1, 91, 92, 0, 0 }, // 492 IIC_VERW_REG_WriteSystem
  { 1, 30, 31, 0, 0 }, // 493 IIC_VERW_MEM_WriteSystem
  { 1, 5, 6, 0, 0 }, // 494 IIC_SSE_MOVDQ
  { 0, 0, 0, 0, 0 }, // 495 WriteShuffleLd
  { 0, 0, 0, 0, 0 }, // 496 WriteShuffle
  { 1, 5, 6, 0, 0 }, // 497 IIC_SSE_MOV_LH
  { 0, 0, 0, 0, 0 }, // 498 WriteStore
  { 1, 74, 75, 0, 0 }, // 499 IIC_SSE_MOVD_ToGP
  { 1, 5, 6, 0, 0 }, // 500 IIC_SSE_MOV_S_MR
  { 1, 5, 6, 0, 0 }, // 501 IIC_SSE_MOV_S_RM
  { 1, 4, 5, 0, 0 }, // 502 IIC_SSE_MOV_S_RR
  { 1, 4, 5, 0, 0 }, // 503 IIC_SSE_MOVQ_RR
  { 0, 0, 0, 0, 0 }, // 504 WriteVecALULd
  { 0, 0, 0, 0, 0 }, // 505 WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 506 IIC_SSE_BIT_P_RM
  { 1, 4, 5, 0, 0 }, // 507 IIC_SSE_BIT_P_RR
  { 0, 0, 0, 0, 0 }, // 508 WriteVecShift
  { 1, 21, 22, 0, 0 }, // 509 IIC_SSE_INTSH_P_RM
  { 1, 16, 17, 0, 0 }, // 510 IIC_SSE_INTSH_P_RR
  { 1, 5, 6, 0, 0 }, // 511 IIC_SSE_UNPCK
  { 0, 0, 0, 0, 0 }, // 512 WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 513 WriteFSqrtLd_ReadAfterLd
  { 1, 33, 34, 0, 0 }, // 514 IIC_SSE_SQRTPD_RM
  { 1, 33, 34, 0, 0 }, // 515 IIC_SSE_SQRTPD_RR
  { 1, 34, 35, 0, 0 }, // 516 IIC_SSE_SQRTPS_RM
  { 1, 34, 35, 0, 0 }, // 517 IIC_SSE_SQRTPS_RR
  { 1, 36, 37, 0, 0 }, // 518 IIC_SSE_SQRTSS_RR
  { 1, 36, 37, 0, 0 }, // 519 IIC_SSE_SQRTSS_RM
  { 1, 4, 5, 0, 0 }, // 520 IIC_WAIT_WriteMicrocoded
  { 1, 92, 93, 0, 0 }, // 521 IIC_WRMSR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 522 IIC_XADD_MEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 523 IIC_XADD_REG_WriteALU
  { 1, 16, 17, 0, 0 }, // 524 IIC_XCHG_REG_WriteALU
  { 1, 21, 22, 0, 0 }, // 525 IIC_XCHG_MEM_WriteALULd_WriteRMW
  { 1, 18, 20, 0, 0 }, // 526 IIC_FXCH_WriteMove
  { 1, 6, 7, 0, 0 }, // 527 IIC_XLAT_WriteLoad
  { 0, ~0U, ~0U, ~0U, ~0U } // end marker
};

static const llvm::InstrItinerary SLMItineraries[] = {
  { 0, 0, 0, 0, 0 }, // 0 NoInstrModel
  { 1, 93, 94, 0, 0 }, // 1 IIC_AAA_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 2 IIC_AAD_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 3 IIC_AAM_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 4 IIC_AAS_WriteMicrocoded
  { 1, 94, 95, 0, 0 }, // 5 IIC_BIN_CARRY_NONMEM_WriteALU
  { 1, 95, 97, 0, 0 }, // 6 IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW
  { 1, 95, 97, 0, 0 }, // 7 IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd
  { 1, 97, 99, 0, 0 }, // 8 IIC_BIN_MEM_WriteALULd
  { 1, 93, 94, 0, 0 }, // 9 IIC_BIN_NONMEM_WriteALU
  { 1, 97, 99, 0, 0 }, // 10 IIC_BIN_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 11 WriteALU
  { 1, 97, 99, 0, 0 }, // 12 IIC_BIN_MEM_WriteALULd_ReadAfterLd
  { 1, 99, 101, 0, 0 }, // 13 IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd
  { 1, 101, 102, 0, 0 }, // 14 IIC_SSE_ALU_F64P_RR_WriteFAdd
  { 1, 102, 104, 0, 0 }, // 15 IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd
  { 1, 104, 105, 0, 0 }, // 16 IIC_SSE_ALU_F32P_RR_WriteFAdd
  { 1, 102, 104, 0, 0 }, // 17 IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd
  { 1, 104, 105, 0, 0 }, // 18 IIC_SSE_ALU_F64S_RR_WriteFAdd
  { 1, 102, 104, 0, 0 }, // 19 IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd
  { 1, 104, 105, 0, 0 }, // 20 IIC_SSE_ALU_F32S_RR_WriteFAdd
  { 1, 101, 102, 0, 0 }, // 21 IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd
  { 1, 104, 105, 0, 0 }, // 22 IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd
  { 1, 105, 106, 0, 0 }, // 23 IIC_AES
  { 0, 0, 0, 0, 0 }, // 24 WriteVecLogicLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 25 WriteVecLogic
  { 1, 97, 99, 0, 0 }, // 26 IIC_ARPL_MEM_WriteSystem
  { 1, 93, 94, 0, 0 }, // 27 IIC_ARPL_REG_WriteSystem
  { 0, 0, 0, 0, 0 }, // 28 WriteZero
  { 1, 106, 108, 0, 0 }, // 29 IIC_SSE_INTALU_P_RM
  { 1, 108, 109, 0, 0 }, // 30 IIC_SSE_INTALU_P_RR
  { 1, 97, 99, 0, 0 }, // 31 IIC_ALU_MEM
  { 1, 93, 94, 0, 0 }, // 32 IIC_ALU_NONMEM
  { 1, 93, 94, 0, 0 }, // 33 IIC_BOUND_WriteSystem
  { 1, 109, 111, 0, 0 }, // 34 IIC_BIT_SCAN_MEM_WriteShiftLd
  { 1, 111, 112, 0, 0 }, // 35 IIC_BIT_SCAN_REG_WriteShift
  { 1, 93, 94, 0, 0 }, // 36 IIC_BSWAP_WriteALU
  { 1, 97, 99, 0, 0 }, // 37 IIC_BT_MI_WriteALU
  { 1, 97, 99, 0, 0 }, // 38 IIC_BT_MR_WriteALULd
  { 1, 93, 94, 0, 0 }, // 39 IIC_BT_RI_WriteALU
  { 1, 93, 94, 0, 0 }, // 40 IIC_BT_RR_WriteALU
  { 1, 97, 99, 0, 0 }, // 41 IIC_BTX_MI_WriteALULd_WriteRMW
  { 1, 97, 99, 0, 0 }, // 42 IIC_BTX_MR_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 43 IIC_BTX_RI_WriteALU
  { 1, 93, 94, 0, 0 }, // 44 IIC_BTX_RR_WriteALU
  { 1, 97, 99, 0, 0 }, // 45 IIC_CALL_MEM_WriteJumpLd
  { 1, 93, 94, 0, 0 }, // 46 IIC_CALL_RI_WriteJump
  { 1, 97, 99, 0, 0 }, // 47 IIC_CALL_MEM_WriteJump
  { 1, 112, 113, 0, 0 }, // 48 IIC_CBW
  { 1, 93, 94, 0, 0 }, // 49 IIC_CLC_WriteALU
  { 1, 93, 94, 0, 0 }, // 50 IIC_CLD_WriteALU
  { 1, 93, 94, 0, 0 }, // 51 IIC_SSE_PREFETCH_WriteLoad
  { 1, 93, 94, 0, 0 }, // 52 IIC_CLI_WriteALU
  { 1, 93, 94, 0, 0 }, // 53 IIC_CLTS_WriteALU
  { 1, 93, 94, 0, 0 }, // 54 IIC_CMC_WriteALU
  { 1, 95, 97, 0, 0 }, // 55 IIC_CMOV16_RM_WriteALULd_ReadAfterLd
  { 1, 94, 95, 0, 0 }, // 56 IIC_CMOV16_RR_WriteALU
  { 1, 95, 97, 0, 0 }, // 57 IIC_CMOV32_RM_WriteALULd_ReadAfterLd
  { 1, 94, 95, 0, 0 }, // 58 IIC_CMOV32_RR_WriteALU
  { 1, 93, 94, 0, 0 }, // 59 IIC_CMPS_WriteMicrocoded
  { 1, 113, 114, 0, 0 }, // 60 IIC_CMPXCHG_16B_WriteALULd_WriteRMW
  { 1, 113, 114, 0, 0 }, // 61 IIC_CMPXCHG_MEM_WriteALULd_WriteRMW
  { 1, 113, 114, 0, 0 }, // 62 IIC_CMPXCHG_REG_WriteALU
  { 1, 113, 114, 0, 0 }, // 63 IIC_CMPXCHG_8B_WriteALULd_WriteRMW
  { 1, 114, 116, 0, 0 }, // 64 IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW
  { 1, 114, 116, 0, 0 }, // 65 IIC_CMPXCHG_REG8_WriteALU
  { 1, 108, 109, 0, 0 }, // 66 IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 67 IIC_SSE_COMIS_RR_WriteFAdd
  { 1, 93, 94, 0, 0 }, // 68 IIC_FCOMI_WriteFAdd
  { 1, 116, 117, 0, 0 }, // 69 IIC_CPUID_WriteSystem
  { 1, 117, 119, 0, 0 }, // 70 IIC_CRC32_MEM
  { 1, 119, 120, 0, 0 }, // 71 IIC_CRC32_REG
  { 1, 120, 121, 0, 0 }, // 72 IIC_SSE_CVT_PD_RR_WriteCvtI2FLd
  { 1, 121, 123, 0, 0 }, // 73 IIC_SSE_CVT_PD_RM_WriteCvtI2F
  { 1, 99, 101, 0, 0 }, // 74 IIC_SSE_CVT_PS_RM_WriteCvtI2FLd
  { 1, 101, 102, 0, 0 }, // 75 IIC_SSE_CVT_PS_RR_WriteCvtI2F
  { 1, 121, 123, 0, 0 }, // 76 IIC_SSE_CVT_PD_RM_WriteCvtF2ILd
  { 1, 120, 121, 0, 0 }, // 77 IIC_SSE_CVT_PD_RR_WriteCvtF2I
  { 1, 121, 123, 0, 0 }, // 78 IIC_SSE_CVT_PD_RM_WriteCvtF2FLd
  { 1, 120, 121, 0, 0 }, // 79 IIC_SSE_CVT_PD_RR_WriteCvtF2F
  { 1, 99, 101, 0, 0 }, // 80 IIC_SSE_CVT_PS_RM_WriteCvtF2ILd
  { 1, 101, 102, 0, 0 }, // 81 IIC_SSE_CVT_PS_RR_WriteCvtF2I
  { 1, 99, 101, 0, 0 }, // 82 IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd
  { 1, 101, 102, 0, 0 }, // 83 IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I
  { 1, 106, 108, 0, 0 }, // 84 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd
  { 1, 108, 109, 0, 0 }, // 85 IIC_SSE_CVT_Scalar_RR_WriteCvtF2F
  { 1, 106, 108, 0, 0 }, // 86 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd
  { 1, 108, 109, 0, 0 }, // 87 IIC_SSE_CVT_Scalar_RR_WriteCvtI2F
  { 1, 99, 101, 0, 0 }, // 88 IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd
  { 1, 101, 102, 0, 0 }, // 89 IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I
  { 1, 99, 101, 0, 0 }, // 90 IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd
  { 1, 101, 102, 0, 0 }, // 91 IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I
  { 1, 93, 94, 0, 0 }, // 92 IIC_DAA_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 93 IIC_DAS_WriteMicrocoded
  { 1, 97, 99, 0, 0 }, // 94 IIC_UNARY_MEM_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 95 IIC_UNARY_REG_WriteALU
  { 1, 123, 124, 0, 0 }, // 96 IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 123, 124, 0, 0 }, // 97 IIC_DIV16_WriteIDiv
  { 1, 123, 124, 0, 0 }, // 98 IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 123, 124, 0, 0 }, // 99 IIC_DIV32_WriteIDiv
  { 1, 124, 125, 0, 0 }, // 100 IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 124, 125, 0, 0 }, // 101 IIC_DIV64_WriteIDiv
  { 1, 125, 127, 0, 0 }, // 102 IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 127, 128, 0, 0 }, // 103 IIC_DIV8_REG_WriteIDiv
  { 1, 128, 130, 0, 0 }, // 104 IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd
  { 1, 130, 131, 0, 0 }, // 105 IIC_SSE_DIV_F64P_RR_WriteFDiv
  { 1, 130, 131, 0, 0 }, // 106 IIC_SSE_DIV_F32P_RR_WriteFDiv
  { 1, 131, 133, 0, 0 }, // 107 IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd
  { 1, 133, 134, 0, 0 }, // 108 IIC_SSE_DIV_F64S_RR_WriteFDiv
  { 1, 133, 134, 0, 0 }, // 109 IIC_SSE_DIV_F32S_RR_WriteFDiv
  { 1, 134, 136, 0, 0 }, // 110 IIC_SSE_DPPD_RM
  { 1, 136, 137, 0, 0 }, // 111 IIC_SSE_DPPD_RR
  { 1, 137, 138, 0, 0 }, // 112 IIC_SSE_DPPS_RR
  { 1, 93, 94, 0, 0 }, // 113 IIC_RET_WriteSystem
  { 0, 0, 0, 0, 0 }, // 114 WriteSystem
  { 1, 93, 94, 0, 0 }, // 115 IIC_ENTER_WriteMicrocoded
  { 1, 99, 101, 0, 0 }, // 116 IIC_SSE_EXTRACTPS_RM
  { 1, 101, 102, 0, 0 }, // 117 IIC_SSE_EXTRACTPS_RR
  { 1, 138, 139, 0, 0 }, // 118 IIC_F2XM1_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 119 IIC_CALL_FAR_PTR_WriteJump
  { 1, 97, 99, 0, 0 }, // 120 IIC_CALL_FAR_MEM_WriteJumpLd
  { 1, 97, 99, 0, 0 }, // 121 IIC_CALL_FAR_MEM_WriteJump
  { 1, 93, 94, 0, 0 }, // 122 IIC_JMP_FAR_PTR_WriteJump
  { 1, 97, 99, 0, 0 }, // 123 IIC_JMP_FAR_MEM_WriteJumpLd
  { 1, 97, 99, 0, 0 }, // 124 IIC_JMP_FAR_MEM_WriteJump
  { 1, 93, 94, 0, 0 }, // 125 IIC_FCOMPP_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 126 IIC_FPSTP_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 127 IIC_FFREE_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 128 IIC_FLDCW_WriteLoad
  { 1, 93, 94, 0, 0 }, // 129 IIC_FLDL_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 130 IIC_FNCLEX_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 131 IIC_FNINIT_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 132 IIC_FNOP_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 133 IIC_FNSTCW_WriteALU
  { 1, 93, 94, 0, 0 }, // 134 IIC_FNSTSW_WriteALU
  { 1, 139, 140, 0, 0 }, // 135 IIC_FPATAN_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 136 IIC_FPREM_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 137 IIC_FPREM1_WriteMicrocoded
  { 1, 140, 141, 0, 0 }, // 138 IIC_FPTAN_WriteMicrocoded
  { 1, 141, 142, 0, 0 }, // 139 IIC_FRNDINT_WriteMicrocoded
  { 1, 142, 143, 0, 0 }, // 140 IIC_FSCALE_WriteMicrocoded
  { 1, 140, 141, 0, 0 }, // 141 IIC_FSINCOS_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 142 IIC_FXAM_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 143 IIC_FXRSTOR_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 144 IIC_FXSAVE_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 145 IIC_FXTRACT_WriteMicrocoded
  { 1, 139, 140, 0, 0 }, // 146 IIC_FYL2X_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 147 IIC_FYL2XP1_WriteMicrocoded
  { 1, 106, 108, 0, 0 }, // 148 IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 149 IIC_SSE_BIT_P_RR_WriteFAdd
  { 1, 106, 108, 0, 0 }, // 150 IIC_SSE_MOVA_P_RM_WriteLoad
  { 1, 143, 145, 0, 0 }, // 151 IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd
  { 1, 145, 146, 0, 0 }, // 152 IIC_SSE_HADDSUB_RR_WriteFAdd
  { 1, 93, 94, 0, 0 }, // 153 IIC_HLT_WriteSystem
  { 1, 146, 147, 0, 0 }, // 154 IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 146, 147, 0, 0 }, // 155 IIC_IDIV16_WriteIDiv
  { 1, 146, 147, 0, 0 }, // 156 IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 146, 147, 0, 0 }, // 157 IIC_IDIV32_WriteIDiv
  { 1, 147, 148, 0, 0 }, // 158 IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 147, 148, 0, 0 }, // 159 IIC_IDIV64_WriteIDiv
  { 1, 148, 149, 0, 0 }, // 160 IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 148, 149, 0, 0 }, // 161 IIC_IDIV8_WriteIDiv
  { 1, 93, 94, 0, 0 }, // 162 IIC_FILD_WriteLoad
  { 1, 114, 116, 0, 0 }, // 163 IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 112, 113, 0, 0 }, // 164 IIC_IMUL16_RR_WriteIMul
  { 1, 149, 151, 0, 0 }, // 165 IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd
  { 1, 149, 151, 0, 0 }, // 166 IIC_IMUL16_RMI_WriteIMulLd
  { 1, 112, 113, 0, 0 }, // 167 IIC_IMUL16_RRI_WriteIMul
  { 1, 114, 116, 0, 0 }, // 168 IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 119, 120, 0, 0 }, // 169 IIC_IMUL32_RR_WriteIMul
  { 1, 117, 119, 0, 0 }, // 170 IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd
  { 1, 117, 119, 0, 0 }, // 171 IIC_IMUL32_RMI_WriteIMulLd
  { 1, 119, 120, 0, 0 }, // 172 IIC_IMUL32_RRI_WriteIMul
  { 1, 113, 114, 0, 0 }, // 173 IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 112, 113, 0, 0 }, // 174 IIC_IMUL64_RR_WriteIMul
  { 1, 149, 151, 0, 0 }, // 175 IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd
  { 1, 149, 151, 0, 0 }, // 176 IIC_IMUL64_RMI_WriteIMulLd
  { 1, 112, 113, 0, 0 }, // 177 IIC_IMUL64_RRI_WriteIMul
  { 1, 113, 114, 0, 0 }, // 178 IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 113, 114, 0, 0 }, // 179 IIC_IMUL8_WriteIMul
  { 1, 93, 94, 0, 0 }, // 180 IIC_INS_WriteSystem
  { 1, 93, 94, 0, 0 }, // 181 IIC_IN_RI_WriteSystem
  { 1, 93, 94, 0, 0 }, // 182 IIC_IN_RR_WriteSystem
  { 1, 106, 108, 0, 0 }, // 183 IIC_SSE_INSERTPS_RM
  { 1, 108, 109, 0, 0 }, // 184 IIC_SSE_INSERTPS_RR
  { 1, 93, 94, 0, 0 }, // 185 IIC_INT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 186 IIC_INT3_WriteSystem
  { 1, 93, 94, 0, 0 }, // 187 IIC_INVD_WriteSystem
  { 1, 93, 94, 0, 0 }, // 188 IIC_INVLPG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 189 IIC_IRET_WriteSystem
  { 1, 93, 94, 0, 0 }, // 190 IIC_FST_WriteStore
  { 1, 93, 94, 0, 0 }, // 191 IIC_FIST_WriteStore
  { 1, 106, 108, 0, 0 }, // 192 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd
  { 1, 106, 108, 0, 0 }, // 193 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 194 WriteLoad
  { 1, 93, 94, 0, 0 }, // 195 IIC_Jcc_WriteJump
  { 1, 93, 94, 0, 0 }, // 196 IIC_JCXZ_WriteJump
  { 1, 97, 99, 0, 0 }, // 197 IIC_JMP_MEM_WriteJumpLd
  { 1, 93, 94, 0, 0 }, // 198 IIC_JMP_REG_WriteJump
  { 1, 93, 94, 0, 0 }, // 199 IIC_JMP_REL_WriteJump
  { 1, 93, 94, 0, 0 }, // 200 IIC_AHF_WriteALU
  { 1, 93, 94, 0, 0 }, // 201 IIC_LAR_RM_WriteSystem
  { 1, 93, 94, 0, 0 }, // 202 IIC_LAR_RR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 203 IIC_CMPX_LOCK_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 204 IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 205 IIC_CMPX_LOCK_8_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 206 IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW
  { 1, 108, 109, 0, 0 }, // 207 IIC_SSE_LDDQU_WriteLoad
  { 1, 93, 94, 0, 0 }, // 208 IIC_SSE_LDMXCSR_WriteLoad
  { 1, 93, 94, 0, 0 }, // 209 IIC_LXS_WriteSystem
  { 1, 93, 94, 0, 0 }, // 210 IIC_FLDZ_WriteZero
  { 1, 93, 94, 0, 0 }, // 211 IIC_FIST_WriteZero
  { 1, 93, 94, 0, 0 }, // 212 IIC_FLD_WriteLoad
  { 1, 93, 94, 0, 0 }, // 213 IIC_FLD80_WriteLoad
  { 1, 93, 94, 0, 0 }, // 214 IIC_FLD_WriteMove
  { 1, 93, 94, 0, 0 }, // 215 IIC_LEA_16_WriteLEA
  { 1, 93, 94, 0, 0 }, // 216 IIC_LEA_WriteLEA
  { 1, 93, 94, 0, 0 }, // 217 IIC_LEAVE_WriteALU
  { 1, 93, 94, 0, 0 }, // 218 IIC_SSE_LFENCE_WriteLoad
  { 1, 93, 94, 0, 0 }, // 219 IIC_LGDT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 220 IIC_LIDT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 221 IIC_LLDT_MEM_WriteSystem
  { 1, 93, 94, 0, 0 }, // 222 IIC_LLDT_REG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 223 IIC_LMSW_REG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 224 IIC_LMSW_MEM_WriteSystem
  { 1, 97, 99, 0, 0 }, // 225 IIC_ALU_MEM_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 226 IIC_ALU_NONMEM_WriteALULd_WriteRMW
  { 1, 93, 94, 0, 0 }, // 227 IIC_LODS_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 228 IIC_LOOP_WriteJump
  { 1, 93, 94, 0, 0 }, // 229 IIC_LOOPE_WriteJump
  { 1, 93, 94, 0, 0 }, // 230 IIC_LOOPNE_WriteJump
  { 1, 93, 94, 0, 0 }, // 231 IIC_RET_WriteJumpLd
  { 1, 93, 94, 0, 0 }, // 232 IIC_LSL_RM_WriteSystem
  { 1, 93, 94, 0, 0 }, // 233 IIC_LSL_RR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 234 IIC_LTR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 235 IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 236 IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW
  { 1, 120, 121, 0, 0 }, // 237 IIC_SSE_MASKMOV_WriteStore
  { 1, 93, 94, 0, 0 }, // 238 IIC_SSE_MFENCE_WriteLoad
  { 1, 93, 94, 0, 0 }, // 239 IIC_MMX_CVT_PD_RM_WriteCvtF2ILd
  { 1, 93, 94, 0, 0 }, // 240 IIC_MMX_CVT_PD_RR_WriteCvtF2I
  { 1, 93, 94, 0, 0 }, // 241 IIC_MMX_CVT_PS_RM_WriteCvtF2ILd
  { 1, 93, 94, 0, 0 }, // 242 IIC_MMX_CVT_PS_RR_WriteCvtF2I
  { 1, 151, 152, 0, 0 }, // 243 IIC_MMX_EMMS
  { 1, 93, 94, 0, 0 }, // 244 IIC_MMX_MASKMOV_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 245 IIC_MMX_MOV_REG_MM_WriteMove
  { 1, 93, 94, 0, 0 }, // 246 IIC_MMX_MOV_MM_RM_WriteStore
  { 1, 93, 94, 0, 0 }, // 247 IIC_MMX_MOV_MM_RM_WriteLoad
  { 1, 93, 94, 0, 0 }, // 248 IIC_MMX_MOV_MM_RM_WriteMove
  { 1, 93, 94, 0, 0 }, // 249 IIC_MMX_MOVQ_RR_WriteMove
  { 1, 93, 94, 0, 0 }, // 250 IIC_MMX_MOVQ_RM_WriteStore
  { 0, 0, 0, 0, 0 }, // 251 WriteMove
  { 1, 93, 94, 0, 0 }, // 252 IIC_MMX_MOVQ_RM_WriteLoad
  { 1, 93, 94, 0, 0 }, // 253 IIC_MMX_ALU_RM_WriteVecALULd
  { 1, 93, 94, 0, 0 }, // 254 IIC_MMX_ALU_RR_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 255 IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 256 IIC_MMX_PCK_RR_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 257 IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 258 IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 259 IIC_MMX_ALUQ_RR_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 260 IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 261 IIC_MMX_MISC_FUNC_MEM_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 262 IIC_MMX_PEXTR_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 263 IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 264 IIC_MMX_PHADDSUBW_RR_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 265 IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 266 IIC_MMX_PHADDSUBD_RR_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 267 IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 268 IIC_MMX_PINSRW_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 269 IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 270 IIC_MMX_PMUL_WriteVecIMul
  { 1, 93, 94, 0, 0 }, // 271 IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 272 IIC_MMX_PSADBW_WriteVecALU
  { 1, 93, 94, 0, 0 }, // 273 IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 274 IIC_MMX_PSHUF_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 275 IIC_MMX_PSHUF_WriteShuffleLd
  { 1, 93, 94, 0, 0 }, // 276 IIC_MMX_SHIFT_RI_WriteVecShift
  { 1, 93, 94, 0, 0 }, // 277 IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 278 IIC_MMX_SHIFT_RR_WriteVecShift
  { 1, 93, 94, 0, 0 }, // 279 IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 280 IIC_MMX_UNPCK_H_RR_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 281 IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 282 IIC_MMX_UNPCK_L_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 283 IIC_SSE_MONITOR_WriteSystem
  { 1, 97, 99, 0, 0 }, // 284 IIC_MOV_MEM_WriteALU
  { 1, 97, 99, 0, 0 }, // 285 IIC_MOV_MEM_WriteStore
  { 1, 93, 94, 0, 0 }, // 286 IIC_MOV_MEM_SR_WriteMove
  { 1, 93, 94, 0, 0 }, // 287 IIC_MOV_WriteMove
  { 1, 97, 99, 0, 0 }, // 288 IIC_MOV_MEM_WriteLoad
  { 1, 93, 94, 0, 0 }, // 289 IIC_MOV_REG_SR_WriteMove
  { 1, 93, 94, 0, 0 }, // 290 IIC_MOV_SR_MEM_WriteMove
  { 1, 93, 94, 0, 0 }, // 291 IIC_MOV_SR_REG_WriteMove
  { 1, 93, 94, 0, 0 }, // 292 IIC_MOV_CR_REG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 293 IIC_MOV_DR_REG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 294 IIC_ALU_NONMEM_WriteZero
  { 1, 93, 94, 0, 0 }, // 295 IIC_MOV_REG_CR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 296 IIC_MOV_REG_DR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 297 IIC_ALU_NONMEM_WriteALU
  { 1, 108, 109, 0, 0 }, // 298 IIC_SSE_MOVDQ_WriteMove
  { 1, 108, 109, 0, 0 }, // 299 IIC_SSE_MOVDQ_WriteLoad
  { 1, 106, 108, 0, 0 }, // 300 IIC_SSE_MOVA_P_MR_WriteStore
  { 1, 108, 109, 0, 0 }, // 301 IIC_SSE_MOVA_P_RR_WriteMove
  { 1, 93, 94, 0, 0 }, // 302 IIC_MOVBE_WriteStore
  { 1, 93, 94, 0, 0 }, // 303 IIC_MOVBE_WriteALULd
  { 1, 108, 109, 0, 0 }, // 304 IIC_SSE_MOV_LH_WriteShuffleLd
  { 1, 108, 109, 0, 0 }, // 305 IIC_SSE_MOV_LH_WriteShuffle
  { 1, 106, 108, 0, 0 }, // 306 IIC_SSE_MOVU_P_MR_WriteStore
  { 1, 106, 108, 0, 0 }, // 307 IIC_SSE_MOVU_P_RM_WriteLoad
  { 1, 108, 109, 0, 0 }, // 308 IIC_SSE_MOVU_P_RR_WriteMove
  { 1, 108, 109, 0, 0 }, // 309 IIC_SSE_MOV_LH_WriteStore
  { 1, 108, 109, 0, 0 }, // 310 IIC_SSE_MOV_LH_WriteShuffleLd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 311 IIC_SSE_MOVDQ_WriteStore
  { 1, 108, 109, 0, 0 }, // 312 IIC_SSE_MOVMSK_WriteVecLogic
  { 1, 93, 94, 0, 0 }, // 313 IIC_SSE_MOVNT_WriteStore
  { 1, 152, 154, 0, 0 }, // 314 IIC_SSE_MOVD_ToGP_WriteMove
  { 1, 93, 94, 0, 0 }, // 315 IIC_MOVS_WriteMicrocoded
  { 1, 106, 108, 0, 0 }, // 316 IIC_SSE_MOV_S_MR_WriteStore
  { 1, 106, 108, 0, 0 }, // 317 IIC_SSE_MOV_S_RM_WriteLoad
  { 1, 108, 109, 0, 0 }, // 318 IIC_SSE_MOV_S_RR_WriteMove
  { 1, 97, 99, 0, 0 }, // 319 IIC_MOVSX_R16_M8_WriteALULd
  { 1, 93, 94, 0, 0 }, // 320 IIC_MOVSX_R16_R8_WriteALU
  { 1, 93, 94, 0, 0 }, // 321 IIC_MOVSX_WriteALULd
  { 1, 93, 94, 0, 0 }, // 322 IIC_MOVSX_WriteALU
  { 1, 108, 109, 0, 0 }, // 323 IIC_SSE_MOVDQ_WriteVecLogicLd
  { 1, 108, 109, 0, 0 }, // 324 IIC_SSE_MOVQ_RR_WriteVecLogic
  { 1, 97, 99, 0, 0 }, // 325 IIC_MOVZX_R16_M8_WriteALULd
  { 1, 93, 94, 0, 0 }, // 326 IIC_MOVZX_R16_R8_WriteALU
  { 1, 93, 94, 0, 0 }, // 327 IIC_MOVZX_WriteALULd
  { 1, 93, 94, 0, 0 }, // 328 IIC_MOVZX_WriteALU
  { 1, 154, 156, 0, 0 }, // 329 IIC_SSE_INTMUL_P_RM
  { 1, 156, 157, 0, 0 }, // 330 IIC_SSE_INTMUL_P_RR
  { 1, 149, 151, 0, 0 }, // 331 IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 112, 113, 0, 0 }, // 332 IIC_MUL16_REG_WriteIMul
  { 1, 117, 119, 0, 0 }, // 333 IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 119, 120, 0, 0 }, // 334 IIC_MUL32_REG_WriteIMul
  { 1, 112, 113, 0, 0 }, // 335 IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 112, 113, 0, 0 }, // 336 IIC_MUL64_WriteIMul
  { 1, 112, 113, 0, 0 }, // 337 IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 112, 113, 0, 0 }, // 338 IIC_MUL8_WriteIMul
  { 1, 99, 101, 0, 0 }, // 339 IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd
  { 1, 101, 102, 0, 0 }, // 340 IIC_SSE_MUL_F64P_RR_WriteFMul
  { 1, 157, 158, 0, 0 }, // 341 IIC_SSE_MUL_F32P_RR_WriteFMul
  { 1, 158, 160, 0, 0 }, // 342 IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd
  { 1, 157, 158, 0, 0 }, // 343 IIC_SSE_MUL_F64S_RR_WriteFMul
  { 1, 108, 109, 0, 0 }, // 344 IIC_SSE_MUL_F32S_RR_WriteFMul
  { 1, 112, 113, 0, 0 }, // 345 IIC_MUL8_WriteIMulLd_WriteIMulH
  { 1, 112, 113, 0, 0 }, // 346 IIC_MUL8_WriteIMul_WriteIMulH
  { 1, 93, 94, 0, 0 }, // 347 IIC_SSE_MWAIT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 348 IIC_NOP_WriteZero
  { 1, 93, 94, 0, 0 }, // 349 IIC_OUT_IR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 350 IIC_OUT_RR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 351 IIC_OUTS_WriteSystem
  { 1, 106, 108, 0, 0 }, // 352 IIC_SSE_PABS_RM_WriteVecALULd
  { 1, 108, 109, 0, 0 }, // 353 IIC_SSE_PABS_RR_WriteVecALU
  { 1, 106, 108, 0, 0 }, // 354 IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 355 IIC_SSE_INTALU_P_RR_WriteVecALU
  { 1, 99, 101, 0, 0 }, // 356 IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 101, 102, 0, 0 }, // 357 IIC_SSE_INTALUQ_P_RR_WriteVecALU
  { 1, 106, 108, 0, 0 }, // 358 IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 359 IIC_SSE_PALIGNRR_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 360 IIC_SSE_PAUSE_WriteLoad
  { 1, 160, 162, 0, 0 }, // 361 IIC_SSE_PCLMULQDQ_RM
  { 1, 162, 163, 0, 0 }, // 362 IIC_SSE_PCLMULQDQ_RR
  { 1, 101, 102, 0, 0 }, // 363 IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd
  { 1, 143, 145, 0, 0 }, // 364 IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 145, 146, 0, 0 }, // 365 IIC_SSE_PHADDSUBD_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 366 WriteVecALULd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 367 WriteVecALU
  { 1, 163, 165, 0, 0 }, // 368 IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 163, 165, 0, 0 }, // 369 IIC_SSE_PHADDSUBW_RR_WriteVecALU
  { 1, 108, 109, 0, 0 }, // 370 IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 371 IIC_SSE_PINSRW_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 372 WriteVecIMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 373 WriteVecIMul
  { 1, 120, 121, 0, 0 }, // 374 IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd
  { 1, 120, 121, 0, 0 }, // 375 IIC_SSE_PMADD_WriteVecIMul
  { 1, 154, 156, 0, 0 }, // 376 IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd
  { 1, 156, 157, 0, 0 }, // 377 IIC_SSE_INTMUL_P_RR_WriteVecIMul
  { 1, 93, 94, 0, 0 }, // 378 IIC_POP_REG16_WriteLoad
  { 1, 93, 94, 0, 0 }, // 379 IIC_POP_MEM_WriteLoad
  { 1, 93, 94, 0, 0 }, // 380 IIC_POP_REG_WriteLoad
  { 1, 93, 94, 0, 0 }, // 381 IIC_POP_A_WriteLoad
  { 1, 165, 167, 0, 0 }, // 382 IIC_SSE_POPCNT_RM
  { 1, 167, 168, 0, 0 }, // 383 IIC_SSE_POPCNT_RR
  { 1, 93, 94, 0, 0 }, // 384 IIC_POP_SR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 385 IIC_POP_F_WriteLoad
  { 1, 93, 94, 0, 0 }, // 386 IIC_POP_FD_WriteLoad
  { 1, 93, 94, 0, 0 }, // 387 IIC_POP_SR_SS_WriteSystem
  { 1, 121, 123, 0, 0 }, // 388 IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd
  { 1, 120, 121, 0, 0 }, // 389 IIC_SSE_PSHUFB_RR_WriteShuffle
  { 1, 168, 170, 0, 0 }, // 390 IIC_SSE_PSHUF_MI_WriteShuffleLd
  { 1, 170, 171, 0, 0 }, // 391 IIC_SSE_PSHUF_RI_WriteShuffle
  { 1, 106, 108, 0, 0 }, // 392 IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd
  { 1, 108, 109, 0, 0 }, // 393 IIC_SSE_PSIGN_RR_WriteVecALU
  { 1, 170, 171, 0, 0 }, // 394 IIC_SSE_INTSHDQ_P_RI_WriteVecShift
  { 1, 170, 171, 0, 0 }, // 395 IIC_SSE_INTSH_P_RI_WriteVecShift
  { 1, 171, 173, 0, 0 }, // 396 IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 173, 174, 0, 0 }, // 397 IIC_SSE_INTSH_P_RR_WriteVecShift
  { 1, 170, 171, 0, 0 }, // 398 IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd
  { 1, 170, 171, 0, 0 }, // 399 IIC_SSE_UNPCK_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 400 IIC_PUSH_REG_WriteStore
  { 1, 93, 94, 0, 0 }, // 401 IIC_PUSH_MEM_WriteStore
  { 1, 93, 94, 0, 0 }, // 402 IIC_PUSH_IMM_WriteStore
  { 1, 93, 94, 0, 0 }, // 403 IIC_PUSH_A_WriteStore
  { 1, 93, 94, 0, 0 }, // 404 IIC_PUSH_SR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 405 IIC_PUSH_CS_WriteSystem
  { 1, 93, 94, 0, 0 }, // 406 IIC_PUSH_F_WriteStore
  { 1, 174, 176, 0, 0 }, // 407 IIC_SR_WriteShiftLd_WriteRMW
  { 1, 174, 176, 0, 0 }, // 408 IIC_SR_WriteShift
  { 1, 176, 178, 0, 0 }, // 409 IIC_SSE_RCPP_RM_WriteFRcpLd
  { 1, 178, 179, 0, 0 }, // 410 IIC_SSE_RCPP_RR_WriteFRcp
  { 1, 179, 181, 0, 0 }, // 411 IIC_SSE_RCPS_RM_WriteFRcpLd
  { 1, 179, 181, 0, 0 }, // 412 IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 413 WriteFRcp
  { 1, 181, 182, 0, 0 }, // 414 IIC_SSE_RCPS_RR_WriteFRcp
  { 1, 93, 94, 0, 0 }, // 415 IIC_RDMSR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 416 IIC_RDPMC_WriteSystem
  { 1, 182, 183, 0, 0 }, // 417 IIC_RDTSC_WriteSystem
  { 1, 93, 94, 0, 0 }, // 418 IIC_REP_MOVS_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 419 IIC_REP_STOS_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 420 IIC_RET_IMM_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 421 WriteShiftLd
  { 0, 0, 0, 0, 0 }, // 422 WriteShift
  { 1, 120, 121, 0, 0 }, // 423 IIC_SSE_ROUNDPS_REG
  { 1, 121, 123, 0, 0 }, // 424 IIC_SSE_ROUNDPS_MEM
  { 1, 93, 94, 0, 0 }, // 425 IIC_RSM_WriteSystem
  { 1, 183, 185, 0, 0 }, // 426 IIC_SSE_SQRTPS_RM_WriteFSqrtLd
  { 1, 185, 186, 0, 0 }, // 427 IIC_SSE_SQRTPS_RR_WriteFSqrt
  { 1, 186, 188, 0, 0 }, // 428 IIC_SSE_SQRTSS_RM_WriteFSqrtLd
  { 1, 186, 188, 0, 0 }, // 429 IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 430 WriteFSqrt
  { 1, 188, 189, 0, 0 }, // 431 IIC_SSE_SQRTSS_RR_WriteFSqrt
  { 0, 0, 0, 0, 0 }, // 432 WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 433 IIC_SCAS_WriteMicrocoded
  { 1, 97, 99, 0, 0 }, // 434 IIC_SET_M_WriteALU_WriteStore
  { 1, 93, 94, 0, 0 }, // 435 IIC_SET_R_WriteALU
  { 1, 93, 94, 0, 0 }, // 436 IIC_SSE_SFENCE_WriteLoad
  { 1, 93, 94, 0, 0 }, // 437 IIC_SGDT_WriteSystem
  { 1, 189, 191, 0, 0 }, // 438 IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 191, 193, 0, 0 }, // 439 IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 193, 194, 0, 0 }, // 440 IIC_SHD16_REG_CL_WriteShift
  { 1, 194, 195, 0, 0 }, // 441 IIC_SHD16_REG_IM_WriteShift
  { 1, 189, 191, 0, 0 }, // 442 IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 191, 193, 0, 0 }, // 443 IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 193, 194, 0, 0 }, // 444 IIC_SHD32_REG_CL_WriteShift
  { 1, 194, 195, 0, 0 }, // 445 IIC_SHD32_REG_IM_WriteShift
  { 1, 189, 191, 0, 0 }, // 446 IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 191, 193, 0, 0 }, // 447 IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 193, 194, 0, 0 }, // 448 IIC_SHD64_REG_CL_WriteShift
  { 1, 194, 195, 0, 0 }, // 449 IIC_SHD64_REG_IM_WriteShift
  { 1, 170, 171, 0, 0 }, // 450 IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd
  { 1, 170, 171, 0, 0 }, // 451 IIC_SSE_SHUFP_WriteShuffle
  { 1, 93, 94, 0, 0 }, // 452 IIC_SIDT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 453 IIC_SLDT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 454 IIC_SMSW_WriteSystem
  { 1, 183, 185, 0, 0 }, // 455 IIC_SSE_SQRTPD_RM_WriteFSqrtLd
  { 1, 185, 186, 0, 0 }, // 456 IIC_SSE_SQRTPD_RR_WriteFSqrt
  { 1, 186, 188, 0, 0 }, // 457 IIC_SSE_SQRTSD_RM_WriteFSqrtLd
  { 1, 188, 189, 0, 0 }, // 458 IIC_SSE_SQRTSD_RR_WriteFSqrt
  { 1, 93, 94, 0, 0 }, // 459 IIC_STC_WriteALU
  { 1, 93, 94, 0, 0 }, // 460 IIC_STD_WriteALU
  { 1, 93, 94, 0, 0 }, // 461 IIC_STI_WriteALU
  { 1, 93, 94, 0, 0 }, // 462 IIC_SSE_STMXCSR_WriteStore
  { 1, 93, 94, 0, 0 }, // 463 IIC_STOS_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 464 IIC_STR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 465 IIC_FST80_WriteStore
  { 1, 93, 94, 0, 0 }, // 466 IIC_FST_WriteMove
  { 1, 93, 94, 0, 0 }, // 467 IIC_SWAPGS_WriteSystem
  { 1, 93, 94, 0, 0 }, // 468 IIC_SYSCALL_WriteSystem
  { 1, 93, 94, 0, 0 }, // 469 IIC_SYS_ENTER_EXIT_WriteSystem
  { 1, 93, 94, 0, 0 }, // 470 IIC_JMP_REL_WriteJumpLd
  { 1, 97, 99, 0, 0 }, // 471 IIC_JMP_MEM_WriteJump
  { 1, 93, 94, 0, 0 }, // 472 IIC_JMP_REG_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 473 WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 474 WriteJump
  { 1, 93, 94, 0, 0 }, // 475 IIC_FUCOMI_WriteFAdd
  { 1, 93, 94, 0, 0 }, // 476 IIC_FUCOM_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 477 WriteFAdd
  { 1, 99, 101, 0, 0 }, // 478 IIC_SSE_ALU_F64P_RM
  { 1, 101, 102, 0, 0 }, // 479 IIC_SSE_ALU_F64P_RR
  { 1, 102, 104, 0, 0 }, // 480 IIC_SSE_ALU_F32P_RM
  { 1, 104, 105, 0, 0 }, // 481 IIC_SSE_ALU_F32P_RR
  { 0, 0, 0, 0, 0 }, // 482 WriteCvtI2FLd
  { 0, 0, 0, 0, 0 }, // 483 WriteCvtI2F
  { 0, 0, 0, 0, 0 }, // 484 WriteCvtF2ILd
  { 0, 0, 0, 0, 0 }, // 485 WriteCvtF2I
  { 1, 99, 101, 0, 0 }, // 486 IIC_SSE_CVT_PS_RM
  { 1, 101, 102, 0, 0 }, // 487 IIC_SSE_CVT_PS_RR
  { 0, 0, 0, 0, 0 }, // 488 WriteCvtF2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 489 WriteCvtF2F
  { 0, 0, 0, 0, 0 }, // 490 WriteCvtI2FLd_ReadAfterLd
  { 1, 93, 94, 0, 0 }, // 491 IIC_VERR_WriteSystem
  { 1, 93, 94, 0, 0 }, // 492 IIC_VERW_REG_WriteSystem
  { 1, 93, 94, 0, 0 }, // 493 IIC_VERW_MEM_WriteSystem
  { 1, 108, 109, 0, 0 }, // 494 IIC_SSE_MOVDQ
  { 0, 0, 0, 0, 0 }, // 495 WriteShuffleLd
  { 0, 0, 0, 0, 0 }, // 496 WriteShuffle
  { 1, 108, 109, 0, 0 }, // 497 IIC_SSE_MOV_LH
  { 0, 0, 0, 0, 0 }, // 498 WriteStore
  { 1, 152, 154, 0, 0 }, // 499 IIC_SSE_MOVD_ToGP
  { 1, 106, 108, 0, 0 }, // 500 IIC_SSE_MOV_S_MR
  { 1, 106, 108, 0, 0 }, // 501 IIC_SSE_MOV_S_RM
  { 1, 108, 109, 0, 0 }, // 502 IIC_SSE_MOV_S_RR
  { 1, 108, 109, 0, 0 }, // 503 IIC_SSE_MOVQ_RR
  { 0, 0, 0, 0, 0 }, // 504 WriteVecALULd
  { 0, 0, 0, 0, 0 }, // 505 WriteShuffleLd_ReadAfterLd
  { 1, 106, 108, 0, 0 }, // 506 IIC_SSE_BIT_P_RM
  { 1, 108, 109, 0, 0 }, // 507 IIC_SSE_BIT_P_RR
  { 0, 0, 0, 0, 0 }, // 508 WriteVecShift
  { 1, 171, 173, 0, 0 }, // 509 IIC_SSE_INTSH_P_RM
  { 1, 173, 174, 0, 0 }, // 510 IIC_SSE_INTSH_P_RR
  { 1, 170, 171, 0, 0 }, // 511 IIC_SSE_UNPCK
  { 0, 0, 0, 0, 0 }, // 512 WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 513 WriteFSqrtLd_ReadAfterLd
  { 1, 183, 185, 0, 0 }, // 514 IIC_SSE_SQRTPD_RM
  { 1, 185, 186, 0, 0 }, // 515 IIC_SSE_SQRTPD_RR
  { 1, 183, 185, 0, 0 }, // 516 IIC_SSE_SQRTPS_RM
  { 1, 185, 186, 0, 0 }, // 517 IIC_SSE_SQRTPS_RR
  { 1, 188, 189, 0, 0 }, // 518 IIC_SSE_SQRTSS_RR
  { 1, 186, 188, 0, 0 }, // 519 IIC_SSE_SQRTSS_RM
  { 1, 93, 94, 0, 0 }, // 520 IIC_WAIT_WriteMicrocoded
  { 1, 93, 94, 0, 0 }, // 521 IIC_WRMSR_WriteSystem
  { 1, 195, 197, 0, 0 }, // 522 IIC_XADD_MEM_WriteALULd_WriteRMW
  { 1, 197, 198, 0, 0 }, // 523 IIC_XADD_REG_WriteALU
  { 1, 197, 198, 0, 0 }, // 524 IIC_XCHG_REG_WriteALU
  { 1, 195, 197, 0, 0 }, // 525 IIC_XCHG_MEM_WriteALULd_WriteRMW
  { 1, 108, 109, 0, 0 }, // 526 IIC_FXCH_WriteMove
  { 1, 93, 94, 0, 0 }, // 527 IIC_XLAT_WriteLoad
  { 0, ~0U, ~0U, ~0U, ~0U } // end marker
};

// ===============================================================
// Data tables for the new per-operand machine model.

// {ProcResourceIdx, Cycles}
extern const llvm::MCWriteProcResEntry X86WriteProcResTable[] = {
  { 0,  0}, // Invalid
  { 9,  1}, // #1
  {10,  1}, // #2
  { 4,  1}, // #3
  { 8,  1}, // #4
  { 9,  1}, // #5
  {10,  3}, // #6
  { 8,  1}, // #7
  { 9,  1}, // #8
  {10,  2}, // #9
  { 3,  1}, // #10
  { 7,  1}, // #11
  { 8,  1}, // #12
  { 9,  1}, // #13
  {10,  2}, // #14
  { 3,  1}, // #15
  { 7,  1}, // #16
  { 9,  1}, // #17
  {10,  1}, // #18
  { 6,  1}, // #19
  { 8,  1}, // #20
  { 9,  1}, // #21
  {10,  2}, // #22
  { 6,  1}, // #23
  { 9,  1}, // #24
  {10,  1}, // #25
  { 5,  1}, // #26
  { 6,  1}, // #27
  { 7,  1}, // #28
  { 8,  1}, // #29
  { 9,  1}, // #30
  {10,  2}, // #31
  { 5,  1}, // #32
  { 6,  1}, // #33
  { 7,  1}, // #34
  { 9,  1}, // #35
  {10,  1}, // #36
  { 8,  1}, // #37
  {10,  1}, // #38
  { 1, 10}, // #39
  { 2,  1}, // #40
  { 6,  1}, // #41
  { 8,  1}, // #42
  { 9,  1}, // #43
  {10,  2}, // #44
  { 1, 10}, // #45
  { 2,  1}, // #46
  { 6,  1}, // #47
  { 9,  1}, // #48
  {10,  1}, // #49
  { 4,  1}, // #50
  { 8,  1}, // #51
  {10,  2}, // #52
  { 4,  1}, // #53
  { 6,  1}, // #54
  { 8,  1}, // #55
  { 9,  1}, // #56
  {10,  3}, // #57
  {12,  1}, // #58
  {13,  1}, // #59
  { 4,  1}, // #60
  { 8,  1}, // #61
  {11,  1}, // #62
  {12,  1}, // #63
  {13,  3}, // #64
  { 8,  1}, // #65
  {11,  1}, // #66
  {12,  1}, // #67
  {13,  2}, // #68
  { 3,  1}, // #69
  { 7,  1}, // #70
  { 8,  1}, // #71
  { 9,  1}, // #72
  {11,  1}, // #73
  {12,  1}, // #74
  {13,  2}, // #75
  { 3,  1}, // #76
  { 7,  1}, // #77
  { 9,  1}, // #78
  {12,  1}, // #79
  {13,  1}, // #80
  { 8,  1}, // #81
  {10,  1}, // #82
  {11,  1}, // #83
  {12,  1}, // #84
  {13,  2}, // #85
  {10,  1}, // #86
  {12,  1}, // #87
  {13,  1}, // #88
  { 5,  1}, // #89
  { 6,  1}, // #90
  { 7,  1}, // #91
  { 8,  1}, // #92
  { 9,  1}, // #93
  {10,  1}, // #94
  {11,  1}, // #95
  {12,  1}, // #96
  {13,  2}, // #97
  { 5,  1}, // #98
  { 6,  1}, // #99
  { 7,  1}, // #100
  { 9,  1}, // #101
  {10,  1}, // #102
  {12,  1}, // #103
  {13,  1}, // #104
  { 8,  1}, // #105
  {11,  1}, // #106
  {13,  1}, // #107
  { 1, 10}, // #108
  { 2,  1}, // #109
  { 6,  1}, // #110
  { 8,  1}, // #111
  { 9,  1}, // #112
  {10,  1}, // #113
  {11,  1}, // #114
  {12,  1}, // #115
  {13,  2}, // #116
  { 1, 10}, // #117
  { 2,  1}, // #118
  { 6,  1}, // #119
  { 9,  1}, // #120
  {10,  1}, // #121
  {12,  1}, // #122
  {13,  1}, // #123
  { 4,  1}, // #124
  {11,  1}, // #125
  {13,  2}, // #126
  { 4,  1}, // #127
  { 8,  1}, // #128
  {10,  1}, // #129
  {11,  1}, // #130
  {12,  1}, // #131
  {13,  3}, // #132
  { 4,  1}, // #133
  {11,  1}, // #134
  {12,  1}, // #135
  {13,  3} // #136
}; // X86WriteProcResTable

// {Cycles, WriteResourceID}
extern const llvm::MCWriteLatencyEntry X86WriteLatencyTable[] = {
  { 0,  0}, // Invalid
  {100,  0}, // #1 WriteMicrocoded_WriteSystem
  { 1,  0}, // #2 WriteALU_WriteVecLogic_WriteZero_WriteShift_WriteJump_WriteStore_WriteMove_WriteLEA_WriteShuffle_WriteVecALU_WriteVecShift
  { 5,  0}, // #3 WriteALULd_WriteVecLogicLd_WriteShiftLd_WriteJumpLd_WriteVecALULd_WriteShuffleLd_WriteVecIMul_WriteVecShiftLd_WriteFMul_WriteFRcp
  { 1,  0}, // #4 WriteRMW
  { 7,  0}, // #5 WriteFAddLd_WriteCvtF2ILd_WriteCvtF2FLd_WriteIMulLd
  { 3,  0}, // #6 WriteFAdd_WriteCvtF2I_WriteCvtF2F_WriteIMul_WriteIMulH
  { 4,  0}, // #7 WriteLoad_WriteCvtI2F
  { 8,  0}, // #8 WriteCvtI2FLd
  {29,  0}, // #9 WriteIDivLd
  {25,  0}, // #10 WriteIDiv
  {16,  0}, // #11 WriteFDivLd
  {12,  0}, // #12 WriteFDiv
  { 9,  0}, // #13 WriteVecIMulLd_WriteFMulLd_WriteFRcpLd
  { 3,  0}, // #14 WriteIMul
  { 3,  0}, // #15 WriteIMulH
  {19,  0}, // #16 WriteFSqrtLd
  {15,  0}, // #17 WriteFSqrt
  { 1,  0}, // #18 WriteALU
  { 1,  0} // #19 WriteStore
}; // X86WriteLatencyTable

// {UseIdx, WriteResourceID, Cycles}
extern const llvm::MCReadAdvanceEntry X86ReadAdvanceTable[] = {
  {0,  0,  0}, // Invalid
  {0,  0,  4}, // #1
  {5,  0,  4}, // #2
  {6,  0,  4} // #3
}; // X86ReadAdvanceTable

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc SandyBridgeModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #7
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0,  1, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #22
  {DBGFIELD("IIC_AES")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #23
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #24
  {DBGFIELD("WriteVecLogic")      1, 0, 0,  1, 2,  2, 1,  0, 0}, // #25
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #26
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #27
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #28
  {DBGFIELD("IIC_SSE_INTALU_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #29
  {DBGFIELD("IIC_SSE_INTALU_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #30
  {DBGFIELD("IIC_ALU_MEM")        0, 0, 0,  0, 0,  0, 0,  0, 0}, // #31
  {DBGFIELD("IIC_ALU_NONMEM")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #32
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #33
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 19, 4,  3, 1,  0, 0}, // #34
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #35
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #37
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #38
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #39
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #41
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #42
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #45
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #46
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #48
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #49
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #50
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #51
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #55
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #57
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #59
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #60
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #61
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #63
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #64
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #66
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #67
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #69
  {DBGFIELD("IIC_CRC32_MEM")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #70
  {DBGFIELD("IIC_CRC32_REG")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #71
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 10, 5,  8, 1,  0, 0}, // #72
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 15, 4,  7, 1,  0, 0}, // #73
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 10, 5,  8, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 15, 4,  7, 1,  0, 0}, // #75
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #76
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #78
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #79
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 10, 5,  8, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 15, 4,  7, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #91
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #92
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #93
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #94
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #95
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #96
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #97
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #98
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #99
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #100
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #102
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #103
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 40, 5, 11, 1,  1, 1}, // #104
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 46, 4, 12, 1,  0, 0}, // #105
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 46, 4, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 40, 5, 11, 1,  1, 1}, // #107
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 46, 4, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 46, 4, 12, 1,  0, 0}, // #109
  {DBGFIELD("IIC_SSE_DPPD_RM")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #110
  {DBGFIELD("IIC_SSE_DPPD_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #111
  {DBGFIELD("IIC_SSE_DPPS_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #112
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #113
  {DBGFIELD("WriteSystem")        1, 0, 0,  1, 2,  1, 1,  0, 0}, // #114
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #115
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #116
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #117
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #118
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #119
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #120
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #121
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #122
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #123
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #124
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #126
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #127
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #128
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #129
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #130
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #131
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #132
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #133
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #148
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #149
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #150
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #151
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #152
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #154
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #155
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #156
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #157
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #158
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #159
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 39, 6,  9, 1,  2, 2}, // #160
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 45, 5, 10, 1,  0, 0}, // #161
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #162
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #163
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #164
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #165
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #166
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #168
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #170
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #171
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #173
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #175
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #178
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #180
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #182
  {DBGFIELD("IIC_SSE_INSERTPS_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #183
  {DBGFIELD("IIC_SSE_INSERTPS_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #184
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #185
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #186
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #187
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #188
  {DBGFIELD("IIC_IRET_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #189
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #190
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #191
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #192
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  8, 1,  1, 1}, // #193
  {DBGFIELD("WriteLoad")          1, 0, 0, 37, 2,  7, 1,  0, 0}, // #194
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 32, 5,  2, 1,  0, 0}, // #195
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #196
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #197
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #198
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #199
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #200
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #201
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #202
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #203
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #204
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #205
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #206
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #207
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #208
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #209
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 37, 2,  7, 1,  0, 0}, // #212
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #213
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #214
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #215
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 16, 3,  2, 1,  0, 0}, // #216
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LFENCE_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #218
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #221
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #222
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #223
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #224
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #225
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #226
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #230
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #235
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #236
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #237
  {DBGFIELD("IIC_SSE_MFENCE_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #238
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #239
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #240
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #241
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #242
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #243
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #244
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #245
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #246
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #247
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #250
  {DBGFIELD("WriteMove")          1, 0, 0,  1, 2,  2, 1,  0, 0}, // #251
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #252
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #255
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #257
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #258
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #260
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #263
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #265
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #267
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #271
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #272
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #273
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #274
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #276
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 19, 4,  3, 1,  1, 1}, // #277
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #278
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #279
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #281
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #283
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #285
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #287
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #289
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #291
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #293
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #295
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #296
  {DBGFIELD("IIC_ALU_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #297
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #299
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #303
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #304
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #306
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #307
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #310
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #311
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #313
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #314
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #320
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #321
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #325
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #327
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #328
  {DBGFIELD("IIC_SSE_INTMUL_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_INTMUL_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #330
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #331
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #332
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #333
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #334
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #335
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #336
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #337
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #338
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #339
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #340
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #342
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #343
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 10, 5,  5, 2,  0, 0}, // #345
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 15, 4, 14, 2,  0, 0}, // #346
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #347
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #348
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #349
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #350
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #351
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #352
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #353
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #354
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #355
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #356
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #358
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_PAUSE_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #361
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #363
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #364
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #365
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #366
  {DBGFIELD("WriteVecALU")        1, 0, 0, 16, 3,  2, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #368
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #370
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #371
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #372
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 46, 4,  3, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #374
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #375
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #376
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #377
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #378
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #379
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #380
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #381
  {DBGFIELD("IIC_SSE_POPCNT_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_POPCNT_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #383
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #384
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #385
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #386
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #388
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #389
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #390
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #391
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #392
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #393
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #394
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #395
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 19, 4,  3, 1,  1, 1}, // #396
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #397
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #398
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #399
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #400
  {DBGFIELD("IIC_PUSH_MEM_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #401
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #402
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #404
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #405
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #406
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #407
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 23, 3,  2, 1,  0, 0}, // #408
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 40, 5, 13, 1,  0, 0}, // #409
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #410
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd") 1, 0, 0, 40, 5, 13, 1,  0, 0}, // #411
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #412
  {DBGFIELD("WriteFRcp")          1, 0, 0, 46, 4,  3, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 46, 4,  3, 1,  0, 0}, // #414
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #415
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #416
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #417
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #418
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #419
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #420
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 19, 4,  3, 1,  0, 0}, // #421
  {DBGFIELD("WriteShift")         1, 0, 0, 23, 3,  2, 1,  0, 0}, // #422
  {DBGFIELD("IIC_SSE_ROUNDPS_REG") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #423
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #424
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #425
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 40, 5, 16, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 46, 4, 17, 1,  0, 0}, // #427
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd") 1, 0, 0, 40, 5, 16, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 40, 5, 16, 1,  1, 1}, // #429
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 46, 4, 17, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 46, 4, 17, 1,  0, 0}, // #431
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 19, 4,  3, 1,  2, 1}, // #432
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0,  3, 4, 18, 2,  0, 0}, // #434
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #435
  {DBGFIELD("IIC_SSE_SFENCE_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #437
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #438
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #439
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #442
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #443
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #444
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 53, 5,  3, 2,  0, 0}, // #447
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 23, 3,  2, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #450
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #451
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #452
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #453
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 40, 5, 16, 1,  0, 0}, // #455
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 46, 4, 17, 1,  0, 0}, // #456
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd") 1, 0, 0, 40, 5, 16, 1,  0, 0}, // #457
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 46, 4, 17, 1,  0, 0}, // #458
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #459
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #460
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #462
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #463
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #464
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 50, 3,  2, 1,  0, 0}, // #465
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #469
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #470
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 32, 5,  2, 1,  0, 0}, // #471
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 26, 6,  3, 1,  0, 0}, // #472
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 26, 6,  3, 1,  0, 0}, // #473
  {DBGFIELD("WriteJump")          1, 0, 0, 32, 5,  2, 1,  0, 0}, // #474
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #475
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #476
  {DBGFIELD("WriteFAdd")          1, 0, 0, 15, 4,  6, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SSE_ALU_F64P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #478
  {DBGFIELD("IIC_SSE_ALU_F64P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #479
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #480
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #481
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 10, 5,  8, 1,  0, 0}, // #482
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 15, 4,  7, 1,  0, 0}, // #483
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 10, 5,  5, 1,  0, 0}, // #484
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 15, 4,  6, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SSE_CVT_PS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #486
  {DBGFIELD("IIC_SSE_CVT_PS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #487
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #488
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 15, 4,  6, 1,  0, 0}, // #489
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  8, 1,  1, 1}, // #490
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #491
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #494
  {DBGFIELD("WriteShuffleLd")     1, 0, 0, 11, 4,  3, 1,  0, 0}, // #495
  {DBGFIELD("WriteShuffle")       1, 0, 0, 16, 3,  2, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #497
  {DBGFIELD("WriteStore")         1, 0, 0, 50, 3,  2, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #499
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #500
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #502
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #503
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 11, 4,  3, 1,  0, 0}, // #504
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #505
  {DBGFIELD("IIC_SSE_BIT_P_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #506
  {DBGFIELD("IIC_SSE_BIT_P_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #507
  {DBGFIELD("WriteVecShift")      1, 0, 0, 23, 3,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_SSE_INTSH_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #509
  {DBGFIELD("IIC_SSE_INTSH_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #510
  {DBGFIELD("IIC_SSE_UNPCK")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #511
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 40, 5, 13, 1,  1, 1}, // #512
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 40, 5, 16, 1,  1, 1}, // #513
  {DBGFIELD("IIC_SSE_SQRTPD_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #514
  {DBGFIELD("IIC_SSE_SQRTPD_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #515
  {DBGFIELD("IIC_SSE_SQRTPS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #516
  {DBGFIELD("IIC_SSE_SQRTPS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #517
  {DBGFIELD("IIC_SSE_SQRTSS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #518
  {DBGFIELD("IIC_SSE_SQRTSS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #519
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #520
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #521
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #522
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #524
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #525
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #526
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 37, 2,  7, 1,  0, 0} // #527
}; // SandyBridgeModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc HaswellModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 65, 4,  3, 1,  1, 1}, // #7
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 58, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 65, 4,  3, 1,  1, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #22
  {DBGFIELD("IIC_AES")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #23
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 71, 5,  3, 1,  1, 1}, // #24
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 78, 3,  2, 1,  0, 0}, // #25
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #26
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #27
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #28
  {DBGFIELD("IIC_SSE_INTALU_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #29
  {DBGFIELD("IIC_SSE_INTALU_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #30
  {DBGFIELD("IIC_ALU_MEM")        0, 0, 0,  0, 0,  0, 0,  0, 0}, // #31
  {DBGFIELD("IIC_ALU_NONMEM")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #32
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #33
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 81, 5,  3, 1,  0, 0}, // #34
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #35
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #36
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #37
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #38
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #39
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #41
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #42
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #45
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #46
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #48
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #49
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #50
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #51
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 58, 2,  2, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 65, 4,  3, 1,  1, 1}, // #55
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 65, 4,  3, 1,  1, 1}, // #57
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #59
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #60
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #61
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #62
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #63
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #64
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #66
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #67
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #68
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #69
  {DBGFIELD("IIC_CRC32_MEM")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #70
  {DBGFIELD("IIC_CRC32_REG")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #71
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 69, 7,  8, 1,  0, 0}, // #72
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 76, 5,  7, 1,  0, 0}, // #73
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 69, 7,  8, 1,  0, 0}, // #74
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 76, 5,  7, 1,  0, 0}, // #75
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #76
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #77
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #78
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #79
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 69, 7,  8, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 76, 5,  7, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #91
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #92
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #93
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #94
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #95
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #96
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #97
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #98
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #99
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #100
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #101
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #102
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #103
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 109, 8, 11, 1,  1, 1}, // #104
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 118, 6, 12, 1,  0, 0}, // #105
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 118, 6, 12, 1,  0, 0}, // #106
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 109, 8, 11, 1,  1, 1}, // #107
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 118, 6, 12, 1,  0, 0}, // #108
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 118, 6, 12, 1,  0, 0}, // #109
  {DBGFIELD("IIC_SSE_DPPD_RM")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #110
  {DBGFIELD("IIC_SSE_DPPD_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #111
  {DBGFIELD("IIC_SSE_DPPS_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #112
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #113
  {DBGFIELD("WriteSystem")        1, 0, 0, 58, 2,  1, 1,  0, 0}, // #114
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #115
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #116
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #117
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #118
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #119
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #120
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #121
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #122
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #123
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #124
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #125
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #126
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #127
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #128
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #129
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #130
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #131
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #132
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #133
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #148
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #149
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #150
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #151
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #152
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #154
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #155
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #156
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #157
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #158
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #159
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 108, 9,  9, 1,  2, 2}, // #160
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 117, 7, 10, 1,  0, 0}, // #161
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #162
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #163
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #164
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #165
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #166
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #167
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #168
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #169
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #170
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #171
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #172
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #173
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #174
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #175
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #177
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #178
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #179
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #180
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #182
  {DBGFIELD("IIC_SSE_INSERTPS_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #183
  {DBGFIELD("IIC_SSE_INSERTPS_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #184
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #185
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #186
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #187
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #188
  {DBGFIELD("IIC_IRET_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #189
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #190
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #191
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #192
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 69, 7,  8, 1,  1, 1}, // #193
  {DBGFIELD("WriteLoad")          1, 0, 0, 105, 3,  7, 1,  0, 0}, // #194
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 98, 7,  2, 1,  0, 0}, // #195
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #196
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #197
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #198
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #199
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #200
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #201
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #202
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #203
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #204
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #205
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #206
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #207
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #208
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #209
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #210
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #211
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 105, 3,  7, 1,  0, 0}, // #212
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #213
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 58, 2,  2, 1,  0, 0}, // #214
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #215
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 77, 4,  2, 1,  0, 0}, // #216
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #217
  {DBGFIELD("IIC_SSE_LFENCE_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #218
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #219
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #220
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #221
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #222
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #223
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #224
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #225
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #226
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #228
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #230
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #234
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #235
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #236
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #237
  {DBGFIELD("IIC_SSE_MFENCE_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #238
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #239
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #240
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 69, 7,  5, 1,  0, 0}, // #241
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #242
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #243
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #244
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #245
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #246
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #247
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #248
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #250
  {DBGFIELD("WriteMove")          1, 0, 0, 58, 2,  2, 1,  0, 0}, // #251
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #252
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 70, 6,  3, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #255
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #257
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #258
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #259
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #260
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #263
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #264
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #265
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #266
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #267
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #271
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #272
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #273
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #274
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 70, 6,  3, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 119, 5,  2, 1,  0, 0}, // #276
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 110, 7,  3, 1,  1, 1}, // #277
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 119, 5,  2, 1,  0, 0}, // #278
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #279
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #280
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #281
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #282
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #283
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #285
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #286
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 58, 2,  2, 1,  0, 0}, // #287
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #288
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #289
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #290
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #291
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #293
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #295
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #296
  {DBGFIELD("IIC_ALU_NONMEM_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #297
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #299
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #302
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #303
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffleLd") 1, 0, 0, 70, 6,  3, 1,  0, 0}, // #304
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #305
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #306
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #307
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #308
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_SSE_MOV_LH_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #310
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #311
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 78, 3,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #313
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #314
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #319
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #320
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #321
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 71, 5,  3, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 78, 3,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #325
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #326
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 65, 4,  3, 1,  0, 0}, // #327
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #328
  {DBGFIELD("IIC_SSE_INTMUL_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #329
  {DBGFIELD("IIC_SSE_INTMUL_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #330
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #331
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #332
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #333
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #334
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #335
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #336
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  2, 2}, // #337
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #338
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #339
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #340
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #341
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #342
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #343
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #344
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 69, 7,  5, 2,  0, 0}, // #345
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 76, 5, 14, 2,  0, 0}, // #346
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #347
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #348
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #349
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #350
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #351
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 70, 6,  3, 1,  0, 0}, // #352
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #353
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #354
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #355
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #356
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #357
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #358
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_PAUSE_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #360
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #361
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #362
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #363
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #364
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #365
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #366
  {DBGFIELD("WriteVecALU")        1, 0, 0, 77, 4,  2, 1,  0, 0}, // #367
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #368
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #369
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #370
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #371
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #372
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 118, 6,  3, 1,  0, 0}, // #373
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #374
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #375
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #376
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #377
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #378
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #379
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #380
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #381
  {DBGFIELD("IIC_SSE_POPCNT_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #382
  {DBGFIELD("IIC_SSE_POPCNT_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #383
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #384
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #385
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #386
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #387
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #388
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #389
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 70, 6,  3, 1,  0, 0}, // #390
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #391
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #392
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #393
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 119, 5,  2, 1,  0, 0}, // #394
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 119, 5,  2, 1,  0, 0}, // #395
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 110, 7,  3, 1,  1, 1}, // #396
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 119, 5,  2, 1,  0, 0}, // #397
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #398
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #399
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #400
  {DBGFIELD("IIC_PUSH_MEM_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #401
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #402
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #403
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #404
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #405
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #406
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #407
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 86, 3,  2, 1,  0, 0}, // #408
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 109, 8, 13, 1,  0, 0}, // #409
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #410
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd") 1, 0, 0, 109, 8, 13, 1,  0, 0}, // #411
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #412
  {DBGFIELD("WriteFRcp")          1, 0, 0, 118, 6,  3, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 118, 6,  3, 1,  0, 0}, // #414
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #415
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #416
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #417
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #418
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #419
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #420
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 81, 5,  3, 1,  0, 0}, // #421
  {DBGFIELD("WriteShift")         1, 0, 0, 86, 3,  2, 1,  0, 0}, // #422
  {DBGFIELD("IIC_SSE_ROUNDPS_REG") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #423
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #424
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #425
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 109, 8, 16, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 118, 6, 17, 1,  0, 0}, // #427
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd") 1, 0, 0, 109, 8, 16, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 109, 8, 16, 1,  1, 1}, // #429
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 118, 6, 17, 1,  0, 0}, // #430
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 118, 6, 17, 1,  0, 0}, // #431
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 81, 5,  3, 1,  2, 1}, // #432
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #433
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 133, 4, 18, 2,  0, 0}, // #434
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #435
  {DBGFIELD("IIC_SSE_SFENCE_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0}, // #436
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #437
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #438
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #439
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #440
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #441
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #442
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #443
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #444
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #445
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #446
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 127, 6,  3, 2,  0, 0}, // #447
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #448
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 86, 3,  2, 1,  0, 0}, // #449
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #450
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffle") 1, 0, 0, 77, 4,  2, 1,  0, 0}, // #451
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #452
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #453
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #454
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 109, 8, 16, 1,  0, 0}, // #455
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 118, 6, 17, 1,  0, 0}, // #456
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd") 1, 0, 0, 109, 8, 16, 1,  0, 0}, // #457
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 118, 6, 17, 1,  0, 0}, // #458
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #459
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #460
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 58, 2,  2, 1,  0, 0}, // #461
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #462
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #463
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #464
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 124, 3,  2, 1,  0, 0}, // #465
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 58, 2,  2, 1,  0, 0}, // #466
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #469
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #470
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 98, 7,  2, 1,  0, 0}, // #471
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 89, 9,  3, 1,  0, 0}, // #472
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 89, 9,  3, 1,  0, 0}, // #473
  {DBGFIELD("WriteJump")          1, 0, 0, 98, 7,  2, 1,  0, 0}, // #474
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #475
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 76, 5,  6, 1,  0, 0}, // #476
  {DBGFIELD("WriteFAdd")          1, 0, 0, 76, 5,  6, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SSE_ALU_F64P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #478
  {DBGFIELD("IIC_SSE_ALU_F64P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #479
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #480
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #481
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 69, 7,  8, 1,  0, 0}, // #482
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 76, 5,  7, 1,  0, 0}, // #483
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 69, 7,  5, 1,  0, 0}, // #484
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 76, 5,  6, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SSE_CVT_PS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #486
  {DBGFIELD("IIC_SSE_CVT_PS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #487
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 69, 7,  5, 1,  1, 1}, // #488
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 76, 5,  6, 1,  0, 0}, // #489
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 69, 7,  8, 1,  1, 1}, // #490
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #491
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #492
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #494
  {DBGFIELD("WriteShuffleLd")     1, 0, 0, 70, 6,  3, 1,  0, 0}, // #495
  {DBGFIELD("WriteShuffle")       1, 0, 0, 77, 4,  2, 1,  0, 0}, // #496
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #497
  {DBGFIELD("WriteStore")         1, 0, 0, 124, 3,  2, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #499
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #500
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #501
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #502
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #503
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 70, 6,  3, 1,  0, 0}, // #504
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 70, 6,  3, 1,  1, 1}, // #505
  {DBGFIELD("IIC_SSE_BIT_P_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #506
  {DBGFIELD("IIC_SSE_BIT_P_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #507
  {DBGFIELD("WriteVecShift")      1, 0, 0, 119, 5,  2, 1,  0, 0}, // #508
  {DBGFIELD("IIC_SSE_INTSH_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #509
  {DBGFIELD("IIC_SSE_INTSH_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #510
  {DBGFIELD("IIC_SSE_UNPCK")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #511
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 109, 8, 13, 1,  1, 1}, // #512
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 109, 8, 16, 1,  1, 1}, // #513
  {DBGFIELD("IIC_SSE_SQRTPD_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #514
  {DBGFIELD("IIC_SSE_SQRTPD_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #515
  {DBGFIELD("IIC_SSE_SQRTPS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #516
  {DBGFIELD("IIC_SSE_SQRTPS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #517
  {DBGFIELD("IIC_SSE_SQRTSS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #518
  {DBGFIELD("IIC_SSE_SQRTSS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #519
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #520
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 58, 2,  1, 1,  0, 0}, // #521
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #522
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #523
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #524
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #525
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 58, 2,  2, 1,  0, 0}, // #526
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 105, 3,  7, 1,  0, 0} // #527
}; // HaswellModelSchedClasses

static const llvm::MCSchedModel NoSchedModel(
  MCSchedModel::DefaultIssueWidth,
  MCSchedModel::DefaultMicroOpBufferSize,
  MCSchedModel::DefaultLoadLatency,
  MCSchedModel::DefaultHighLatency,
  MCSchedModel::DefaultMispredictPenalty,
  1, // CompleteModel
  0, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel GenericModel(
  4, // IssueWidth
  32, // MicroOpBufferSize
  4, // LoadLatency
  10, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // CompleteModel
  1, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel AtomModel(
  2, // IssueWidth
  0, // MicroOpBufferSize
  3, // LoadLatency
  30, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // CompleteModel
  2, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  AtomItineraries);

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

static const llvm::MCSchedModel SandyBridgeModel(
  4, // IssueWidth
  168, // MicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // CompleteModel
  3, // Processor ID
  SandyBridgeModelProcResources,
  SandyBridgeModelSchedClasses,
  11,
  528,
  NoItineraries);

// {Name, NumUnits, SuperIdx, IsBuffered}
static const llvm::MCProcResourceDesc HaswellModelProcResources[] = {
  {DBGFIELD("InvalidUnit")     0, 0, 0},
  {DBGFIELD("HWDivider")       1, 0, -1}, // #1
  {DBGFIELD("HWPort0")         1, 0, -1}, // #2
  {DBGFIELD("HWPort1")         1, 0, -1}, // #3
  {DBGFIELD("HWPort4")         1, 0, -1}, // #4
  {DBGFIELD("HWPort5")         1, 0, -1}, // #5
  {DBGFIELD("HWPort05")        2, 0, -1}, // #6
  {DBGFIELD("HWPort15")        2, 0, -1}, // #7
  {DBGFIELD("HWPort23")        2, 0, -1}, // #8
  {DBGFIELD("HWPort015")       3, 0, -1}, // #9
  {DBGFIELD("HWPort056")       3, 0, -1}, // #10
  {DBGFIELD("HWPort237")       3, 0, -1}, // #11
  {DBGFIELD("HWPort0156")      4, 0, -1}, // #12
  {DBGFIELD("HWPortAny")       8, 0, 60}  // #13
};

static const llvm::MCSchedModel HaswellModel(
  4, // IssueWidth
  192, // MicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // CompleteModel
  4, // Processor ID
  HaswellModelProcResources,
  HaswellModelSchedClasses,
  14,
  528,
  NoItineraries);

static const llvm::MCSchedModel SLMModel(
  2, // IssueWidth
  MCSchedModel::DefaultMicroOpBufferSize,
  3, // LoadLatency
  30, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // CompleteModel
  5, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  SLMItineraries);

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
  { "bdver1", (const void *)&GenericModel },
  { "bdver2", (const void *)&GenericModel },
  { "bdver3", (const void *)&GenericModel },
  { "btver1", (const void *)&GenericModel },
  { "btver2", (const void *)&GenericModel },
  { "c3", (const void *)&GenericModel },
  { "c3-2", (const void *)&GenericModel },
  { "core-avx-i", (const void *)&SandyBridgeModel },
  { "core-avx2", (const void *)&HaswellModel },
  { "core2", (const void *)&SandyBridgeModel },
  { "corei7", (const void *)&SandyBridgeModel },
  { "corei7-avx", (const void *)&SandyBridgeModel },
  { "generic", (const void *)&GenericModel },
  { "geode", (const void *)&GenericModel },
  { "i386", (const void *)&GenericModel },
  { "i486", (const void *)&GenericModel },
  { "i586", (const void *)&GenericModel },
  { "i686", (const void *)&GenericModel },
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
  { "slm", (const void *)&SLMModel },
  { "westmere", (const void *)&SandyBridgeModel },
  { "winchip-c6", (const void *)&GenericModel },
  { "winchip2", (const void *)&GenericModel },
  { "x86-64", (const void *)&GenericModel },
  { "yonah", (const void *)&SandyBridgeModel }
};
#undef DBGFIELD
static inline void InitX86MCSubtargetInfo(MCSubtargetInfo *II, StringRef TT, StringRef CPU, StringRef FS) {
  II->InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV, 
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable, 
                      X86Stages, X86OperandCycles, X86ForwardingPaths, 50, 55);
}

} // End llvm namespace 
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
  uint64_t Bits = getFeatureBits();
  if ((Bits & X86::Feature3DNow) != 0 && X863DNowLevel < ThreeDNow) X863DNowLevel = ThreeDNow;
  if ((Bits & X86::Feature3DNowA) != 0 && X863DNowLevel < ThreeDNowA) X863DNowLevel = ThreeDNowA;
  if ((Bits & X86::Feature64Bit) != 0) HasX86_64 = true;
  if ((Bits & X86::FeatureADX) != 0) HasADX = true;
  if ((Bits & X86::FeatureAES) != 0) HasAES = true;
  if ((Bits & X86::FeatureAVX) != 0 && X86SSELevel < AVX) X86SSELevel = AVX;
  if ((Bits & X86::FeatureAVX2) != 0 && X86SSELevel < AVX2) X86SSELevel = AVX2;
  if ((Bits & X86::FeatureAVX512) != 0 && X86SSELevel < AVX512F) X86SSELevel = AVX512F;
  if ((Bits & X86::FeatureBMI) != 0) HasBMI = true;
  if ((Bits & X86::FeatureBMI2) != 0) HasBMI2 = true;
  if ((Bits & X86::FeatureCDI) != 0) HasCDI = true;
  if ((Bits & X86::FeatureCMOV) != 0) HasCMov = true;
  if ((Bits & X86::FeatureCMPXCHG16B) != 0) HasCmpxchg16b = true;
  if ((Bits & X86::FeatureCallRegIndirect) != 0) CallRegIndirect = true;
  if ((Bits & X86::FeatureERI) != 0) HasERI = true;
  if ((Bits & X86::FeatureF16C) != 0) HasF16C = true;
  if ((Bits & X86::FeatureFMA) != 0) HasFMA = true;
  if ((Bits & X86::FeatureFMA4) != 0) HasFMA4 = true;
  if ((Bits & X86::FeatureFSGSBase) != 0) HasFSGSBase = true;
  if ((Bits & X86::FeatureFastUAMem) != 0) IsUAMemFast = true;
  if ((Bits & X86::FeatureHLE) != 0) HasHLE = true;
  if ((Bits & X86::FeatureLEAUsesAG) != 0) LEAUsesAG = true;
  if ((Bits & X86::FeatureLZCNT) != 0) HasLZCNT = true;
  if ((Bits & X86::FeatureLeaForSP) != 0) UseLeaForSP = true;
  if ((Bits & X86::FeatureMMX) != 0 && X86SSELevel < MMX) X86SSELevel = MMX;
  if ((Bits & X86::FeatureMOVBE) != 0) HasMOVBE = true;
  if ((Bits & X86::FeaturePCLMUL) != 0) HasPCLMUL = true;
  if ((Bits & X86::FeaturePFI) != 0) HasPFI = true;
  if ((Bits & X86::FeaturePOPCNT) != 0) HasPOPCNT = true;
  if ((Bits & X86::FeaturePRFCHW) != 0) HasPRFCHW = true;
  if ((Bits & X86::FeaturePadShortFunctions) != 0) PadShortFunctions = true;
  if ((Bits & X86::FeatureRDRAND) != 0) HasRDRAND = true;
  if ((Bits & X86::FeatureRDSEED) != 0) HasRDSEED = true;
  if ((Bits & X86::FeatureRTM) != 0) HasRTM = true;
  if ((Bits & X86::FeatureSHA) != 0) HasSHA = true;
  if ((Bits & X86::FeatureSSE1) != 0 && X86SSELevel < SSE1) X86SSELevel = SSE1;
  if ((Bits & X86::FeatureSSE2) != 0 && X86SSELevel < SSE2) X86SSELevel = SSE2;
  if ((Bits & X86::FeatureSSE3) != 0 && X86SSELevel < SSE3) X86SSELevel = SSE3;
  if ((Bits & X86::FeatureSSE4A) != 0) HasSSE4A = true;
  if ((Bits & X86::FeatureSSE41) != 0 && X86SSELevel < SSE41) X86SSELevel = SSE41;
  if ((Bits & X86::FeatureSSE42) != 0 && X86SSELevel < SSE42) X86SSELevel = SSE42;
  if ((Bits & X86::FeatureSSSE3) != 0 && X86SSELevel < SSSE3) X86SSELevel = SSSE3;
  if ((Bits & X86::FeatureSlowBTMem) != 0) IsBTMemSlow = true;
  if ((Bits & X86::FeatureSlowDivide) != 0) HasSlowDivide = true;
  if ((Bits & X86::FeatureTBM) != 0) HasTBM = true;
  if ((Bits & X86::FeatureVectorUAMem) != 0) HasVectorUAMem = true;
  if ((Bits & X86::FeatureXOP) != 0) HasXOP = true;
  if ((Bits & X86::Mode64Bit) != 0) In64BitMode = true;
  if ((Bits & X86::ProcIntelAtom) != 0 && X86ProcFamily < IntelAtom) X86ProcFamily = IntelAtom;
  if ((Bits & X86::ProcIntelSLM) != 0 && X86ProcFamily < IntelSLM) X86ProcFamily = IntelSLM;
}
#endif // GET_SUBTARGETINFO_TARGET_DESC


#ifdef GET_SUBTARGETINFO_HEADER
#undef GET_SUBTARGETINFO_HEADER
namespace llvm {
class DFAPacketizer;
struct X86GenSubtargetInfo : public TargetSubtargetInfo {
  explicit X86GenSubtargetInfo(StringRef TT, StringRef CPU, StringRef FS);
public:
  unsigned resolveSchedClass(unsigned SchedClass, const MachineInstr *DefMI, const TargetSchedModel *SchedModel) const;
  DFAPacketizer *createDFAPacketizer(const InstrItineraryData *IID) const;
};
} // End llvm namespace 
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
X86GenSubtargetInfo::X86GenSubtargetInfo(StringRef TT, StringRef CPU, StringRef FS)
  : TargetSubtargetInfo() {
  InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV, 
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable, 
                      X86Stages, X86OperandCycles, X86ForwardingPaths, 50, 55);
}

unsigned X86GenSubtargetInfo
::resolveSchedClass(unsigned SchedClass, const MachineInstr *MI, const TargetSchedModel *SchedModel) const {
  report_fatal_error("Expected a variant SchedClass");
} // X86GenSubtargetInfo::resolveSchedClass
} // End llvm namespace 
#endif // GET_SUBTARGETINFO_CTOR

