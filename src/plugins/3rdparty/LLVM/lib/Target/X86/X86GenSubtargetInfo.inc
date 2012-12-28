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
  const uint64_t Feature3DNow = 1ULL << 0;
  const uint64_t Feature3DNowA = 1ULL << 1;
  const uint64_t Feature64Bit = 1ULL << 2;
  const uint64_t FeatureAES = 1ULL << 3;
  const uint64_t FeatureAVX = 1ULL << 4;
  const uint64_t FeatureAVX2 = 1ULL << 5;
  const uint64_t FeatureBMI = 1ULL << 6;
  const uint64_t FeatureBMI2 = 1ULL << 7;
  const uint64_t FeatureCMOV = 1ULL << 8;
  const uint64_t FeatureCMPXCHG16B = 1ULL << 9;
  const uint64_t FeatureF16C = 1ULL << 10;
  const uint64_t FeatureFMA = 1ULL << 11;
  const uint64_t FeatureFMA4 = 1ULL << 12;
  const uint64_t FeatureFSGSBase = 1ULL << 13;
  const uint64_t FeatureFastUAMem = 1ULL << 14;
  const uint64_t FeatureLZCNT = 1ULL << 15;
  const uint64_t FeatureLeaForSP = 1ULL << 16;
  const uint64_t FeatureMMX = 1ULL << 17;
  const uint64_t FeatureMOVBE = 1ULL << 18;
  const uint64_t FeaturePCLMUL = 1ULL << 19;
  const uint64_t FeaturePOPCNT = 1ULL << 20;
  const uint64_t FeatureRDRAND = 1ULL << 21;
  const uint64_t FeatureRTM = 1ULL << 22;
  const uint64_t FeatureSSE1 = 1ULL << 23;
  const uint64_t FeatureSSE2 = 1ULL << 24;
  const uint64_t FeatureSSE3 = 1ULL << 25;
  const uint64_t FeatureSSE4A = 1ULL << 26;
  const uint64_t FeatureSSE41 = 1ULL << 27;
  const uint64_t FeatureSSE42 = 1ULL << 28;
  const uint64_t FeatureSSSE3 = 1ULL << 29;
  const uint64_t FeatureSlowBTMem = 1ULL << 30;
  const uint64_t FeatureSlowDivide = 1ULL << 31;
  const uint64_t FeatureVectorUAMem = 1ULL << 32;
  const uint64_t FeatureXOP = 1ULL << 33;
  const uint64_t Mode64Bit = 1ULL << 34;
  const uint64_t ProcIntelAtom = 1ULL << 35;
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
  { "aes", "Enable AES instructions", X86::FeatureAES, X86::FeatureSSE2 },
  { "atom", "Intel Atom processors", X86::ProcIntelAtom, 0ULL },
  { "avx", "Enable AVX instructions", X86::FeatureAVX, X86::FeatureSSE42 },
  { "avx2", "Enable AVX2 instructions", X86::FeatureAVX2, X86::FeatureAVX },
  { "bmi", "Support BMI instructions", X86::FeatureBMI, 0ULL },
  { "bmi2", "Support BMI2 instructions", X86::FeatureBMI2, 0ULL },
  { "cmov", "Enable conditional move instructions", X86::FeatureCMOV, 0ULL },
  { "cmpxchg16b", "64-bit with cmpxchg16b", X86::FeatureCMPXCHG16B, X86::Feature64Bit },
  { "f16c", "Support 16-bit floating point conversion instructions", X86::FeatureF16C, 0ULL },
  { "fast-unaligned-mem", "Fast unaligned memory access", X86::FeatureFastUAMem, 0ULL },
  { "fma", "Enable three-operand fused multiple-add", X86::FeatureFMA, X86::FeatureAVX },
  { "fma4", "Enable four-operand fused multiple-add", X86::FeatureFMA4, X86::FeatureAVX | X86::FeatureSSE4A },
  { "fsgsbase", "Support FS/GS Base instructions", X86::FeatureFSGSBase, 0ULL },
  { "idiv-to-divb", "Use small divide for positive values less than 256", X86::FeatureSlowDivide, 0ULL },
  { "lea-sp", "Use LEA for adjusting the stack pointer", X86::FeatureLeaForSP, 0ULL },
  { "lzcnt", "Support LZCNT instruction", X86::FeatureLZCNT, 0ULL },
  { "mmx", "Enable MMX instructions", X86::FeatureMMX, 0ULL },
  { "movbe", "Support MOVBE instruction", X86::FeatureMOVBE, 0ULL },
  { "pclmul", "Enable packed carry-less multiplication instructions", X86::FeaturePCLMUL, X86::FeatureSSE2 },
  { "popcnt", "Support POPCNT instruction", X86::FeaturePOPCNT, 0ULL },
  { "rdrand", "Support RDRAND instruction", X86::FeatureRDRAND, 0ULL },
  { "rtm", "Support RTM instructions", X86::FeatureRTM, 0ULL },
  { "slow-bt-mem", "Bit testing of memory is slow", X86::FeatureSlowBTMem, 0ULL },
  { "sse", "Enable SSE instructions", X86::FeatureSSE1, X86::FeatureMMX | X86::FeatureCMOV },
  { "sse2", "Enable SSE2 instructions", X86::FeatureSSE2, X86::FeatureSSE1 },
  { "sse3", "Enable SSE3 instructions", X86::FeatureSSE3, X86::FeatureSSE2 },
  { "sse41", "Enable SSE 4.1 instructions", X86::FeatureSSE41, X86::FeatureSSSE3 },
  { "sse42", "Enable SSE 4.2 instructions", X86::FeatureSSE42, X86::FeatureSSE41 },
  { "sse4a", "Support SSE 4a instructions", X86::FeatureSSE4A, X86::FeatureSSE3 },
  { "ssse3", "Enable SSSE3 instructions", X86::FeatureSSSE3, X86::FeatureSSE3 },
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
  { "atom", "Select the atom processor", X86::ProcIntelAtom | X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeatureSlowBTMem | X86::FeatureLeaForSP | X86::FeatureSlowDivide, 0ULL },
  { "bdver1", "Select the bdver1 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "bdver2", "Select the bdver2 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureFMA, 0ULL },
  { "btver1", "Select the btver1 processor", X86::FeatureSSSE3 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "c3", "Select the c3 processor", X86::Feature3DNow, 0ULL },
  { "c3-2", "Select the c3-2 processor", X86::FeatureSSE1, 0ULL },
  { "core-avx-i", "Select the core-avx-i processor", X86::FeatureAVX | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase, 0ULL },
  { "core-avx2", "Select the core-avx2 processor", X86::FeatureAVX2 | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureFMA | X86::FeatureRTM, 0ULL },
  { "core2", "Select the core2 processor", X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "corei7", "Select the corei7 processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES, 0ULL },
  { "corei7-avx", "Select the corei7-avx processor", X86::FeatureAVX | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL, 0ULL },
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
  { "westmere", "Select the westmere processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL, 0ULL },
  { "winchip-c6", "Select the winchip-c6 processor", X86::FeatureMMX, 0ULL },
  { "winchip2", "Select the winchip2 processor", X86::Feature3DNow, 0ULL },
  { "x86-64", "Select the x86-64 processor", X86::FeatureSSE2 | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
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

extern const llvm::InstrStage X86Stages[] = {
  { 0, 0, 0, llvm::InstrStage::Required }, // No itinerary
  { 13, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 1
  { 7, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 2
  { 21, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 3
  { 1, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 4
  { 1, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 5
  { 23, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 6
  { 24, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 7
  { 11, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 8
  { 16, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 9
  { 2, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 10
  { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 11
  { 9, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 12
  { 40, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 13
  { 39, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 14
  { 15, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 15
  { 1, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 16-17
  { 3, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 18
  { 14, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 19
  { 33, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 20
  { 18, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 21
  { 22, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 22
  { 6, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 23
  { 121, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 24
  { 20, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 25
  { 68, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 26
  { 50, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 27
  { 130, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 28
  { 32, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 29
  { 99, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 30
  { 5, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 31-32
  { 4, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 33
  { 5, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 34
  { 10, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 35
  { 25, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 36
  { 63, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 37
  { 8, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 38
  { 183, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 39
  { 55, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 40
  { 71, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 41
  { 168, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 42
  { 46, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 43
  { 77, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 44
  { 174, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 45
  { 141, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 46
  { 140, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 47
  { 146, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 48
  { 147, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 49
  { 62, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 50
  { 5, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 51
  { 12, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 52
  { 59, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 53
  { 127, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 54
  { 1003, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 55
  { 92, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 56
  { 94, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 57
  { 109, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 58
  { 31, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 59
  { 54, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 60
  { 44, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 61
  { 64, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 62
  { 60, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 63
  { 67, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 64
  { 69, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 65
  { 17, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 66
  { 49, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 67
  { 83, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 68
  { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 69
  { 3, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 70
  { 4, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 71
  { 136, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 72
  { 123, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 73
  { 88, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 74
  { 26, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 75
  { 74, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 76
  { 72, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 77
  { 29, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 78
  { 48, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 79
  { 78, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 80
  { 30, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 81
  { 75, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 82
  { 79, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 83
  { 741, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 84
  { 70, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 85
  { 34, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 86
  { 125, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 87
  { 45, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 88
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

static const llvm::InstrItinerary *NoItineraries = 0;

static const llvm::InstrItinerary AtomItineraries[] = {
  { 0, 0, 0, 0, 0 }, // 0 NoItinerary
  { 1, 1, 2, 0, 0 }, // 1 IIC_AAA
  { 1, 2, 3, 0, 0 }, // 2 IIC_AAD
  { 1, 3, 4, 0, 0 }, // 3 IIC_AAM
  { 1, 1, 2, 0, 0 }, // 4 IIC_AAS
  { 1, 4, 5, 0, 0 }, // 5 IIC_AHF
  { 1, 5, 6, 0, 0 }, // 6 IIC_ALU_MEM
  { 1, 4, 5, 0, 0 }, // 7 IIC_ALU_NONMEM
  { 1, 6, 7, 0, 0 }, // 8 IIC_ARPL_MEM
  { 1, 7, 8, 0, 0 }, // 9 IIC_ARPL_REG
  { 1, 5, 6, 0, 0 }, // 10 IIC_BIN_MEM
  { 1, 4, 5, 0, 0 }, // 11 IIC_BIN_NONMEM
  { 1, 8, 9, 0, 0 }, // 12 IIC_BOUND
  { 1, 9, 10, 0, 0 }, // 13 IIC_BSF
  { 1, 9, 10, 0, 0 }, // 14 IIC_BSR
  { 1, 5, 6, 0, 0 }, // 15 IIC_BSWAP
  { 1, 10, 11, 0, 0 }, // 16 IIC_BTX_MI
  { 1, 8, 9, 0, 0 }, // 17 IIC_BTX_MR
  { 1, 11, 12, 0, 0 }, // 18 IIC_BTX_RI
  { 1, 11, 12, 0, 0 }, // 19 IIC_BTX_RR
  { 1, 4, 5, 0, 0 }, // 20 IIC_BT_MI
  { 1, 12, 13, 0, 0 }, // 21 IIC_BT_MR
  { 1, 11, 12, 0, 0 }, // 22 IIC_BT_RI
  { 1, 11, 12, 0, 0 }, // 23 IIC_BT_RR
  { 1, 13, 14, 0, 0 }, // 24 IIC_CALL_FAR_MEM
  { 1, 14, 15, 0, 0 }, // 25 IIC_CALL_FAR_PTR
  { 1, 15, 16, 0, 0 }, // 26 IIC_CALL_MEM
  { 1, 16, 18, 0, 0 }, // 27 IIC_CALL_RI
  { 1, 4, 5, 0, 0 }, // 28 IIC_CLC
  { 1, 18, 19, 0, 0 }, // 29 IIC_CLD
  { 1, 19, 20, 0, 0 }, // 30 IIC_CLI
  { 1, 20, 21, 0, 0 }, // 31 IIC_CLTS
  { 1, 4, 5, 0, 0 }, // 32 IIC_CMC
  { 1, 5, 6, 0, 0 }, // 33 IIC_CMOV16_RM
  { 1, 4, 5, 0, 0 }, // 34 IIC_CMOV16_RR
  { 1, 5, 6, 0, 0 }, // 35 IIC_CMOV32_RM
  { 1, 4, 5, 0, 0 }, // 36 IIC_CMOV32_RR
  { 1, 18, 19, 0, 0 }, // 37 IIC_CMPS
  { 1, 21, 22, 0, 0 }, // 38 IIC_CMPXCHG_8B
  { 1, 22, 23, 0, 0 }, // 39 IIC_CMPXCHG_16B
  { 1, 19, 20, 0, 0 }, // 40 IIC_CMPXCHG_MEM
  { 1, 23, 24, 0, 0 }, // 41 IIC_CMPXCHG_MEM8
  { 1, 15, 16, 0, 0 }, // 42 IIC_CMPXCHG_REG
  { 1, 12, 13, 0, 0 }, // 43 IIC_CMPXCHG_REG8
  { 1, 19, 20, 0, 0 }, // 44 IIC_CMPX_LOCK
  { 1, 23, 24, 0, 0 }, // 45 IIC_CMPX_LOCK_8
  { 1, 21, 22, 0, 0 }, // 46 IIC_CMPX_LOCK_8B
  { 1, 22, 23, 0, 0 }, // 47 IIC_CMPX_LOCK_16B
  { 1, 24, 25, 0, 0 }, // 48 IIC_CPUID
  { 1, 21, 22, 0, 0 }, // 49 IIC_DAA
  { 1, 25, 26, 0, 0 }, // 50 IIC_DAS
  { 1, 4, 5, 0, 0 }, // 51 IIC_DEFAULT
  { 1, 26, 27, 0, 0 }, // 52 IIC_DIV8_MEM
  { 1, 27, 28, 0, 0 }, // 53 IIC_DIV8_REG
  { 1, 27, 28, 0, 0 }, // 54 IIC_DIV16
  { 1, 27, 28, 0, 0 }, // 55 IIC_DIV32
  { 1, 28, 29, 0, 0 }, // 56 IIC_DIV64
  { 1, 29, 30, 0, 0 }, // 57 IIC_ENTER
  { 1, 30, 31, 0, 0 }, // 58 IIC_F2XM1
  { 1, 12, 13, 0, 0 }, // 59 IIC_FCOMI
  { 1, 11, 12, 0, 0 }, // 60 IIC_FCOMPP
  { 1, 4, 5, 0, 0 }, // 61 IIC_FFREE
  { 1, 31, 33, 0, 0 }, // 62 IIC_FILD
  { 1, 23, 24, 0, 0 }, // 63 IIC_FIST
  { 1, 5, 6, 0, 0 }, // 64 IIC_FLD
  { 1, 33, 34, 0, 0 }, // 65 IIC_FLD80
  { 1, 34, 35, 0, 0 }, // 66 IIC_FLDCW
  { 1, 35, 36, 0, 0 }, // 67 IIC_FLDL
  { 1, 4, 5, 0, 0 }, // 68 IIC_FLDZ
  { 1, 36, 37, 0, 0 }, // 69 IIC_FNCLEX
  { 1, 37, 38, 0, 0 }, // 70 IIC_FNINIT
  { 1, 4, 5, 0, 0 }, // 71 IIC_FNOP
  { 1, 38, 39, 0, 0 }, // 72 IIC_FNSTCW
  { 1, 35, 36, 0, 0 }, // 73 IIC_FNSTSW
  { 1, 39, 40, 0, 0 }, // 74 IIC_FPATAN
  { 1, 40, 41, 0, 0 }, // 75 IIC_FPREM
  { 1, 41, 42, 0, 0 }, // 76 IIC_FPREM1
  { 1, 4, 5, 0, 0 }, // 77 IIC_FPSTP
  { 1, 42, 43, 0, 0 }, // 78 IIC_FPTAN
  { 1, 43, 44, 0, 0 }, // 79 IIC_FRNDINT
  { 1, 44, 45, 0, 0 }, // 80 IIC_FSCALE
  { 1, 45, 46, 0, 0 }, // 81 IIC_FSINCOS
  { 1, 10, 11, 0, 0 }, // 82 IIC_FST
  { 1, 34, 35, 0, 0 }, // 83 IIC_FST80
  { 1, 11, 12, 0, 0 }, // 84 IIC_FUCOM
  { 1, 12, 13, 0, 0 }, // 85 IIC_FUCOMI
  { 1, 5, 6, 0, 0 }, // 86 IIC_FXAM
  { 1, 16, 18, 0, 0 }, // 87 IIC_FXCH
  { 1, 46, 47, 0, 0 }, // 88 IIC_FXRSTOR
  { 1, 47, 48, 0, 0 }, // 89 IIC_FXSAVE
  { 1, 36, 37, 0, 0 }, // 90 IIC_FXTRACT
  { 1, 48, 49, 0, 0 }, // 91 IIC_FYL2X
  { 1, 49, 50, 0, 0 }, // 92 IIC_FYL2XP1
  { 1, 24, 25, 0, 0 }, // 93 IIC_HLT
  { 1, 50, 51, 0, 0 }, // 94 IIC_IDIV8
  { 1, 50, 51, 0, 0 }, // 95 IIC_IDIV16
  { 1, 50, 51, 0, 0 }, // 96 IIC_IDIV32
  { 1, 28, 29, 0, 0 }, // 97 IIC_IDIV64
  { 1, 2, 3, 0, 0 }, // 98 IIC_IMUL8
  { 1, 38, 39, 0, 0 }, // 99 IIC_IMUL16_MEM
  { 1, 2, 3, 0, 0 }, // 100 IIC_IMUL16_RM
  { 1, 2, 3, 0, 0 }, // 101 IIC_IMUL16_RMI
  { 1, 23, 24, 0, 0 }, // 102 IIC_IMUL16_RR
  { 1, 23, 24, 0, 0 }, // 103 IIC_IMUL16_RRI
  { 1, 2, 3, 0, 0 }, // 104 IIC_IMUL32_MEM
  { 1, 51, 52, 0, 0 }, // 105 IIC_IMUL32_RM
  { 1, 51, 52, 0, 0 }, // 106 IIC_IMUL32_RMI
  { 1, 51, 52, 0, 0 }, // 107 IIC_IMUL32_RR
  { 1, 51, 52, 0, 0 }, // 108 IIC_IMUL32_RRI
  { 1, 52, 53, 0, 0 }, // 109 IIC_IMUL64
  { 1, 52, 53, 0, 0 }, // 110 IIC_IMUL64_RM
  { 1, 19, 20, 0, 0 }, // 111 IIC_IMUL64_RMI
  { 1, 52, 53, 0, 0 }, // 112 IIC_IMUL64_RR
  { 1, 19, 20, 0, 0 }, // 113 IIC_IMUL64_RRI
  { 1, 53, 54, 0, 0 }, // 114 IIC_INS
  { 1, 54, 55, 0, 0 }, // 115 IIC_INT
  { 1, 28, 29, 0, 0 }, // 116 IIC_INT3
  { 1, 55, 56, 0, 0 }, // 117 IIC_INVD
  { 1, 41, 42, 0, 0 }, // 118 IIC_INVLPG
  { 1, 56, 57, 0, 0 }, // 119 IIC_IN_RI
  { 1, 57, 58, 0, 0 }, // 120 IIC_IN_RR
  { 1, 58, 59, 0, 0 }, // 121 IIC_IRET
  { 1, 33, 34, 0, 0 }, // 122 IIC_JCXZ
  { 1, 29, 30, 0, 0 }, // 123 IIC_JMP_FAR_MEM
  { 1, 59, 60, 0, 0 }, // 124 IIC_JMP_FAR_PTR
  { 1, 10, 11, 0, 0 }, // 125 IIC_JMP_MEM
  { 1, 11, 12, 0, 0 }, // 126 IIC_JMP_REG
  { 1, 11, 12, 0, 0 }, // 127 IIC_JMP_REL
  { 1, 11, 12, 0, 0 }, // 128 IIC_Jcc
  { 1, 27, 28, 0, 0 }, // 129 IIC_LAR_RM
  { 1, 60, 61, 0, 0 }, // 130 IIC_LAR_RR
  { 1, 11, 12, 0, 0 }, // 131 IIC_LEA
  { 1, 10, 11, 0, 0 }, // 132 IIC_LEAVE
  { 1, 10, 11, 0, 0 }, // 133 IIC_LEA_16
  { 1, 61, 62, 0, 0 }, // 134 IIC_LGDT
  { 1, 61, 62, 0, 0 }, // 135 IIC_LIDT
  { 1, 62, 63, 0, 0 }, // 136 IIC_LLDT_MEM
  { 1, 63, 64, 0, 0 }, // 137 IIC_LLDT_REG
  { 1, 64, 65, 0, 0 }, // 138 IIC_LMSW_MEM
  { 1, 65, 66, 0, 0 }, // 139 IIC_LMSW_REG
  { 1, 10, 11, 0, 0 }, // 140 IIC_LODS
  { 1, 21, 22, 0, 0 }, // 141 IIC_LOOP
  { 1, 38, 39, 0, 0 }, // 142 IIC_LOOPE
  { 1, 66, 67, 0, 0 }, // 143 IIC_LOOPNE
  { 1, 43, 44, 0, 0 }, // 144 IIC_LSL_RM
  { 1, 67, 68, 0, 0 }, // 145 IIC_LSL_RR
  { 1, 68, 69, 0, 0 }, // 146 IIC_LTR
  { 1, 35, 36, 0, 0 }, // 147 IIC_LXS
  { 1, 18, 19, 0, 0 }, // 148 IIC_MMX_ALUQ_RM
  { 1, 10, 11, 0, 0 }, // 149 IIC_MMX_ALUQ_RR
  { 1, 5, 6, 0, 0 }, // 150 IIC_MMX_ALU_RM
  { 1, 4, 5, 0, 0 }, // 151 IIC_MMX_ALU_RR
  { 1, 38, 39, 0, 0 }, // 152 IIC_MMX_CVT_PD_RM
  { 1, 2, 3, 0, 0 }, // 153 IIC_MMX_CVT_PD_RR
  { 1, 31, 33, 0, 0 }, // 154 IIC_MMX_CVT_PS_RM
  { 1, 69, 70, 0, 0 }, // 155 IIC_MMX_CVT_PS_RR
  { 1, 5, 6, 0, 0 }, // 156 IIC_MMX_MASKMOV
  { 1, 5, 6, 0, 0 }, // 157 IIC_MMX_MISC_FUNC_MEM
  { 1, 4, 5, 0, 0 }, // 158 IIC_MMX_MISC_FUNC_REG
  { 1, 5, 6, 0, 0 }, // 159 IIC_MMX_MOVQ_RM
  { 1, 4, 5, 0, 0 }, // 160 IIC_MMX_MOVQ_RR
  { 1, 5, 6, 0, 0 }, // 161 IIC_MMX_MOV_MM_RM
  { 1, 70, 71, 0, 0 }, // 162 IIC_MMX_MOV_REG_MM
  { 1, 5, 6, 0, 0 }, // 163 IIC_MMX_PCK_RM
  { 1, 4, 5, 0, 0 }, // 164 IIC_MMX_PCK_RR
  { 1, 33, 34, 0, 0 }, // 165 IIC_MMX_PEXTR
  { 1, 33, 34, 0, 0 }, // 166 IIC_MMX_PHADDSUBD_RM
  { 1, 18, 19, 0, 0 }, // 167 IIC_MMX_PHADDSUBD_RR
  { 1, 23, 24, 0, 0 }, // 168 IIC_MMX_PHADDSUBW_RM
  { 1, 34, 35, 0, 0 }, // 169 IIC_MMX_PHADDSUBW_RR
  { 1, 5, 6, 0, 0 }, // 170 IIC_MMX_PINSRW
  { 1, 71, 72, 0, 0 }, // 171 IIC_MMX_PMUL
  { 1, 33, 34, 0, 0 }, // 172 IIC_MMX_PSADBW
  { 1, 5, 6, 0, 0 }, // 173 IIC_MMX_PSHUF
  { 1, 4, 5, 0, 0 }, // 174 IIC_MMX_SHIFT_RI
  { 1, 18, 19, 0, 0 }, // 175 IIC_MMX_SHIFT_RM
  { 1, 10, 11, 0, 0 }, // 176 IIC_MMX_SHIFT_RR
  { 1, 5, 6, 0, 0 }, // 177 IIC_MMX_UNPCK_H_RM
  { 1, 4, 5, 0, 0 }, // 178 IIC_MMX_UNPCK_H_RR
  { 1, 5, 6, 0, 0 }, // 179 IIC_MMX_UNPCK_L
  { 1, 4, 5, 0, 0 }, // 180 IIC_MOV
  { 1, 5, 6, 0, 0 }, // 181 IIC_MOVBE
  { 1, 18, 19, 0, 0 }, // 182 IIC_MOVS
  { 1, 5, 6, 0, 0 }, // 183 IIC_MOVSX
  { 1, 18, 19, 0, 0 }, // 184 IIC_MOVSX_R16_M8
  { 1, 10, 11, 0, 0 }, // 185 IIC_MOVSX_R16_R8
  { 1, 5, 6, 0, 0 }, // 186 IIC_MOVZX
  { 1, 18, 19, 0, 0 }, // 187 IIC_MOVZX_R16_M8
  { 1, 10, 11, 0, 0 }, // 188 IIC_MOVZX_R16_R8
  { 1, 72, 73, 0, 0 }, // 189 IIC_MOV_CR_REG
  { 1, 73, 74, 0, 0 }, // 190 IIC_MOV_DR_REG
  { 1, 5, 6, 0, 0 }, // 191 IIC_MOV_MEM
  { 1, 10, 11, 0, 0 }, // 192 IIC_MOV_MEM_SR
  { 1, 52, 53, 0, 0 }, // 193 IIC_MOV_REG_CR
  { 1, 74, 75, 0, 0 }, // 194 IIC_MOV_REG_DR
  { 1, 5, 6, 0, 0 }, // 195 IIC_MOV_REG_SR
  { 1, 75, 76, 0, 0 }, // 196 IIC_MOV_SR_MEM
  { 1, 3, 4, 0, 0 }, // 197 IIC_MOV_SR_REG
  { 1, 2, 3, 0, 0 }, // 198 IIC_MUL8
  { 1, 38, 39, 0, 0 }, // 199 IIC_MUL16_MEM
  { 1, 2, 3, 0, 0 }, // 200 IIC_MUL16_REG
  { 1, 2, 3, 0, 0 }, // 201 IIC_MUL32_MEM
  { 1, 23, 24, 0, 0 }, // 202 IIC_MUL32_REG
  { 1, 52, 53, 0, 0 }, // 203 IIC_MUL64
  { 1, 4, 5, 0, 0 }, // 204 IIC_NOP
  { 1, 76, 77, 0, 0 }, // 205 IIC_OUTS
  { 1, 77, 78, 0, 0 }, // 206 IIC_OUT_IR
  { 1, 26, 27, 0, 0 }, // 207 IIC_OUT_RR
  { 1, 12, 13, 0, 0 }, // 208 IIC_POP_A
  { 1, 29, 30, 0, 0 }, // 209 IIC_POP_F
  { 1, 75, 76, 0, 0 }, // 210 IIC_POP_FD
  { 1, 18, 19, 0, 0 }, // 211 IIC_POP_MEM
  { 1, 16, 18, 0, 0 }, // 212 IIC_POP_REG
  { 1, 10, 11, 0, 0 }, // 213 IIC_POP_REG16
  { 1, 78, 79, 0, 0 }, // 214 IIC_POP_SR
  { 1, 79, 80, 0, 0 }, // 215 IIC_POP_SR_SS
  { 1, 38, 39, 0, 0 }, // 216 IIC_PUSH_A
  { 1, 10, 11, 0, 0 }, // 217 IIC_PUSH_CS
  { 1, 12, 13, 0, 0 }, // 218 IIC_PUSH_F
  { 1, 16, 18, 0, 0 }, // 219 IIC_PUSH_IMM
  { 1, 10, 11, 0, 0 }, // 220 IIC_PUSH_MEM
  { 1, 16, 18, 0, 0 }, // 221 IIC_PUSH_REG
  { 1, 10, 11, 0, 0 }, // 222 IIC_PUSH_SR
  { 1, 80, 81, 0, 0 }, // 223 IIC_RDMSR
  { 1, 43, 44, 0, 0 }, // 224 IIC_RDPMC
  { 1, 81, 82, 0, 0 }, // 225 IIC_RDTSC
  { 1, 82, 83, 0, 0 }, // 226 IIC_REP_MOVS
  { 1, 76, 77, 0, 0 }, // 227 IIC_REP_STOS
  { 1, 83, 84, 0, 0 }, // 228 IIC_RET
  { 1, 16, 18, 0, 0 }, // 229 IIC_RET_IMM
  { 1, 84, 85, 0, 0 }, // 230 IIC_RSM
  { 1, 10, 11, 0, 0 }, // 231 IIC_SCAS
  { 1, 10, 11, 0, 0 }, // 232 IIC_SET_M
  { 1, 4, 5, 0, 0 }, // 233 IIC_SET_R
  { 1, 33, 34, 0, 0 }, // 234 IIC_SGDT
  { 1, 23, 24, 0, 0 }, // 235 IIC_SHD16_MEM_CL
  { 1, 23, 24, 0, 0 }, // 236 IIC_SHD16_MEM_IM
  { 1, 23, 24, 0, 0 }, // 237 IIC_SHD16_REG_CL
  { 1, 23, 24, 0, 0 }, // 238 IIC_SHD16_REG_IM
  { 1, 33, 34, 0, 0 }, // 239 IIC_SHD32_MEM_CL
  { 1, 33, 34, 0, 0 }, // 240 IIC_SHD32_MEM_IM
  { 1, 10, 11, 0, 0 }, // 241 IIC_SHD32_REG_CL
  { 1, 10, 11, 0, 0 }, // 242 IIC_SHD32_REG_IM
  { 1, 12, 13, 0, 0 }, // 243 IIC_SHD64_MEM_CL
  { 1, 12, 13, 0, 0 }, // 244 IIC_SHD64_MEM_IM
  { 1, 38, 39, 0, 0 }, // 245 IIC_SHD64_REG_CL
  { 1, 12, 13, 0, 0 }, // 246 IIC_SHD64_REG_IM
  { 1, 33, 34, 0, 0 }, // 247 IIC_SIDT
  { 1, 18, 19, 0, 0 }, // 248 IIC_SLDT
  { 1, 12, 13, 0, 0 }, // 249 IIC_SMSW
  { 1, 5, 6, 0, 0 }, // 250 IIC_SR
  { 1, 31, 33, 0, 0 }, // 251 IIC_SSE_ALU_F32P_RM
  { 1, 69, 70, 0, 0 }, // 252 IIC_SSE_ALU_F32P_RR
  { 1, 31, 33, 0, 0 }, // 253 IIC_SSE_ALU_F32S_RM
  { 1, 69, 70, 0, 0 }, // 254 IIC_SSE_ALU_F32S_RR
  { 1, 2, 3, 0, 0 }, // 255 IIC_SSE_ALU_F64P_RM
  { 1, 23, 24, 0, 0 }, // 256 IIC_SSE_ALU_F64P_RR
  { 1, 31, 33, 0, 0 }, // 257 IIC_SSE_ALU_F64S_RM
  { 1, 69, 70, 0, 0 }, // 258 IIC_SSE_ALU_F64S_RR
  { 1, 5, 6, 0, 0 }, // 259 IIC_SSE_BIT_P_RM
  { 1, 4, 5, 0, 0 }, // 260 IIC_SSE_BIT_P_RR
  { 1, 2, 3, 0, 0 }, // 261 IIC_SSE_CMPP_RM
  { 1, 23, 24, 0, 0 }, // 262 IIC_SSE_CMPP_RR
  { 1, 35, 36, 0, 0 }, // 263 IIC_SSE_COMIS_RM
  { 1, 12, 13, 0, 0 }, // 264 IIC_SSE_COMIS_RR
  { 1, 38, 39, 0, 0 }, // 265 IIC_SSE_CVT_PD_RM
  { 1, 2, 3, 0, 0 }, // 266 IIC_SSE_CVT_PD_RR
  { 1, 2, 3, 0, 0 }, // 267 IIC_SSE_CVT_PS_RM
  { 1, 23, 24, 0, 0 }, // 268 IIC_SSE_CVT_PS_RR
  { 1, 12, 13, 0, 0 }, // 269 IIC_SSE_CVT_SD2SI_RM
  { 1, 38, 39, 0, 0 }, // 270 IIC_SSE_CVT_SD2SI_RR
  { 1, 12, 13, 0, 0 }, // 271 IIC_SSE_CVT_SS2SI32_RM
  { 1, 38, 39, 0, 0 }, // 272 IIC_SSE_CVT_SS2SI32_RR
  { 1, 35, 36, 0, 0 }, // 273 IIC_SSE_CVT_SS2SI64_RM
  { 1, 12, 13, 0, 0 }, // 274 IIC_SSE_CVT_SS2SI64_RR
  { 1, 2, 3, 0, 0 }, // 275 IIC_SSE_CVT_Scalar_RM
  { 1, 23, 24, 0, 0 }, // 276 IIC_SSE_CVT_Scalar_RR
  { 1, 85, 86, 0, 0 }, // 277 IIC_SSE_DIV_F32P_RR
  { 1, 86, 87, 0, 0 }, // 278 IIC_SSE_DIV_F32S_RR
  { 1, 87, 88, 0, 0 }, // 279 IIC_SSE_DIV_F64P_RM
  { 1, 87, 88, 0, 0 }, // 280 IIC_SSE_DIV_F64P_RR
  { 1, 50, 51, 0, 0 }, // 281 IIC_SSE_DIV_F64S_RM
  { 1, 50, 51, 0, 0 }, // 282 IIC_SSE_DIV_F64S_RR
  { 1, 12, 13, 0, 0 }, // 283 IIC_SSE_HADDSUB_RM
  { 1, 38, 39, 0, 0 }, // 284 IIC_SSE_HADDSUB_RR
  { 1, 18, 19, 0, 0 }, // 285 IIC_SSE_INTALUQ_P_RM
  { 1, 10, 11, 0, 0 }, // 286 IIC_SSE_INTALUQ_P_RR
  { 1, 5, 6, 0, 0 }, // 287 IIC_SSE_INTALU_P_RM
  { 1, 4, 5, 0, 0 }, // 288 IIC_SSE_INTALU_P_RR
  { 1, 51, 52, 0, 0 }, // 289 IIC_SSE_INTMUL_P_RM
  { 1, 51, 52, 0, 0 }, // 290 IIC_SSE_INTMUL_P_RR
  { 1, 4, 5, 0, 0 }, // 291 IIC_SSE_INTSH_P_RI
  { 1, 18, 19, 0, 0 }, // 292 IIC_SSE_INTSH_P_RM
  { 1, 10, 11, 0, 0 }, // 293 IIC_SSE_INTSH_P_RR
  { 1, 18, 19, 0, 0 }, // 294 IIC_SSE_LDDQU
  { 1, 34, 35, 0, 0 }, // 295 IIC_SSE_LDMXCSR
  { 1, 4, 5, 0, 0 }, // 296 IIC_SSE_LFENCE
  { 1, 10, 11, 0, 0 }, // 297 IIC_SSE_MASKMOV
  { 1, 5, 6, 0, 0 }, // 298 IIC_SSE_MFENCE
  { 1, 88, 89, 0, 0 }, // 299 IIC_SSE_MONITOR
  { 1, 5, 6, 0, 0 }, // 300 IIC_SSE_MOVA_P_MR
  { 1, 5, 6, 0, 0 }, // 301 IIC_SSE_MOVA_P_RM
  { 1, 4, 5, 0, 0 }, // 302 IIC_SSE_MOVA_P_RR
  { 1, 5, 6, 0, 0 }, // 303 IIC_SSE_MOVDQ
  { 1, 70, 71, 0, 0 }, // 304 IIC_SSE_MOVD_ToGP
  { 1, 70, 71, 0, 0 }, // 305 IIC_SSE_MOVMSK
  { 1, 5, 6, 0, 0 }, // 306 IIC_SSE_MOVNT
  { 1, 4, 5, 0, 0 }, // 307 IIC_SSE_MOVQ_RR
  { 1, 10, 11, 0, 0 }, // 308 IIC_SSE_MOVU_P_MR
  { 1, 18, 19, 0, 0 }, // 309 IIC_SSE_MOVU_P_RM
  { 1, 4, 5, 0, 0 }, // 310 IIC_SSE_MOVU_P_RR
  { 1, 5, 6, 0, 0 }, // 311 IIC_SSE_MOV_LH
  { 1, 5, 6, 0, 0 }, // 312 IIC_SSE_MOV_S_MR
  { 1, 5, 6, 0, 0 }, // 313 IIC_SSE_MOV_S_RM
  { 1, 4, 5, 0, 0 }, // 314 IIC_SSE_MOV_S_RR
  { 1, 51, 52, 0, 0 }, // 315 IIC_SSE_MUL_F32P_RR
  { 1, 71, 72, 0, 0 }, // 316 IIC_SSE_MUL_F32S_RR
  { 1, 35, 36, 0, 0 }, // 317 IIC_SSE_MUL_F64P_RM
  { 1, 12, 13, 0, 0 }, // 318 IIC_SSE_MUL_F64P_RR
  { 1, 51, 52, 0, 0 }, // 319 IIC_SSE_MUL_F64S_RM
  { 1, 51, 52, 0, 0 }, // 320 IIC_SSE_MUL_F64S_RR
  { 1, 43, 44, 0, 0 }, // 321 IIC_SSE_MWAIT
  { 1, 5, 6, 0, 0 }, // 322 IIC_SSE_PABS_RM
  { 1, 4, 5, 0, 0 }, // 323 IIC_SSE_PABS_RR
  { 1, 5, 6, 0, 0 }, // 324 IIC_SSE_PALIGNR
  { 1, 66, 67, 0, 0 }, // 325 IIC_SSE_PAUSE
  { 1, 33, 34, 0, 0 }, // 326 IIC_SSE_PEXTRW
  { 1, 33, 34, 0, 0 }, // 327 IIC_SSE_PHADDSUBD_RM
  { 1, 18, 19, 0, 0 }, // 328 IIC_SSE_PHADDSUBD_RR
  { 1, 38, 39, 0, 0 }, // 329 IIC_SSE_PHADDSUBW_RM
  { 1, 2, 3, 0, 0 }, // 330 IIC_SSE_PHADDSUBW_RR
  { 1, 5, 6, 0, 0 }, // 331 IIC_SSE_PINSRW
  { 1, 51, 52, 0, 0 }, // 332 IIC_SSE_PMADD
  { 1, 5, 6, 0, 0 }, // 333 IIC_SSE_PREFETCH
  { 1, 5, 6, 0, 0 }, // 334 IIC_SSE_PSHUF
  { 1, 34, 35, 0, 0 }, // 335 IIC_SSE_PSHUFB_RM
  { 1, 33, 34, 0, 0 }, // 336 IIC_SSE_PSHUFB_RR
  { 1, 5, 6, 0, 0 }, // 337 IIC_SSE_PSIGN_RM
  { 1, 4, 5, 0, 0 }, // 338 IIC_SSE_PSIGN_RR
  { 1, 35, 36, 0, 0 }, // 339 IIC_SSE_RCPP_RM
  { 1, 12, 13, 0, 0 }, // 340 IIC_SSE_RCPP_RR
  { 1, 71, 72, 0, 0 }, // 341 IIC_SSE_RCPS_RM
  { 1, 71, 72, 0, 0 }, // 342 IIC_SSE_RCPS_RR
  { 1, 5, 6, 0, 0 }, // 343 IIC_SSE_SFENCE
  { 1, 5, 6, 0, 0 }, // 344 IIC_SSE_SHUFP
  { 1, 19, 20, 0, 0 }, // 345 IIC_SSE_SQRTP_RM
  { 1, 1, 2, 0, 0 }, // 346 IIC_SSE_SQRTP_RR
  { 1, 52, 53, 0, 0 }, // 347 IIC_SSE_SQRTS_RM
  { 1, 8, 9, 0, 0 }, // 348 IIC_SSE_SQRTS_RR
  { 1, 15, 16, 0, 0 }, // 349 IIC_SSE_STMXCSR
  { 1, 5, 6, 0, 0 }, // 350 IIC_SSE_UNPCK
  { 1, 4, 5, 0, 0 }, // 351 IIC_STC
  { 1, 3, 4, 0, 0 }, // 352 IIC_STD
  { 1, 66, 67, 0, 0 }, // 353 IIC_STI
  { 1, 4, 5, 0, 0 }, // 354 IIC_STOS
  { 1, 18, 19, 0, 0 }, // 355 IIC_STR
  { 1, 22, 23, 0, 0 }, // 356 IIC_SWAPGS
  { 1, 89, 90, 0, 0 }, // 357 IIC_SYSCALL
  { 1, 74, 75, 0, 0 }, // 358 IIC_SYS_ENTER_EXIT
  { 1, 5, 6, 0, 0 }, // 359 IIC_UNARY_MEM
  { 1, 4, 5, 0, 0 }, // 360 IIC_UNARY_REG
  { 1, 90, 91, 0, 0 }, // 361 IIC_VERR
  { 1, 27, 28, 0, 0 }, // 362 IIC_VERW_MEM
  { 1, 91, 92, 0, 0 }, // 363 IIC_VERW_REG
  { 1, 4, 5, 0, 0 }, // 364 IIC_WAIT
  { 1, 92, 93, 0, 0 }, // 365 IIC_WRMSR
  { 1, 18, 19, 0, 0 }, // 366 IIC_XADD_LOCK_MEM
  { 0, 0, 0, 0, 0 }, // 367 IIC_XADD_LOCK_MEM8
  { 1, 18, 19, 0, 0 }, // 368 IIC_XADD_MEM
  { 1, 10, 11, 0, 0 }, // 369 IIC_XADD_REG
  { 1, 18, 19, 0, 0 }, // 370 IIC_XCHG_MEM
  { 1, 10, 11, 0, 0 }, // 371 IIC_XCHG_REG
  { 1, 23, 24, 0, 0 }, // 372 IIC_XLAT
  { 0, ~0U, ~0U, ~0U, ~0U } // end marker
};

// ===============================================================
// Data tables for the new per-operand machine model.

// {ProcResourceIdx, Cycles}
extern const llvm::MCWriteProcResEntry X86WriteProcResTable[] = {
  { 0,  0}, // Invalid
}; // X86WriteProcResTable

// {Cycles, WriteResourceID}
extern const llvm::MCWriteLatencyEntry X86WriteLatencyTable[] = {
  { 0,  0}, // Invalid
}; // X86WriteLatencyTable

// {UseIdx, WriteResourceID, Cycles}
extern const llvm::MCReadAdvanceEntry X86ReadAdvanceTable[] = {
  {0,  0,  0}, // Invalid
}; // X86ReadAdvanceTable

static const llvm::MCSchedModel NoSchedModel(
  MCSchedModel::DefaultIssueWidth,
  MCSchedModel::DefaultMinLatency,
  MCSchedModel::DefaultLoadLatency,
  MCSchedModel::DefaultHighLatency,
  MCSchedModel::DefaultMispredictPenalty,
  0, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel GenericModel(
  4, // IssueWidth
  0, // MinLatency
  4, // LoadLatency
  10, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel AtomModel(
  2, // IssueWidth
  1, // MinLatency
  3, // LoadLatency
  30, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  2, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  AtomItineraries);

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
  { "btver1", (const void *)&GenericModel },
  { "c3", (const void *)&GenericModel },
  { "c3-2", (const void *)&GenericModel },
  { "core-avx-i", (const void *)&GenericModel },
  { "core-avx2", (const void *)&GenericModel },
  { "core2", (const void *)&GenericModel },
  { "corei7", (const void *)&GenericModel },
  { "corei7-avx", (const void *)&GenericModel },
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
  { "nehalem", (const void *)&GenericModel },
  { "nocona", (const void *)&GenericModel },
  { "opteron", (const void *)&GenericModel },
  { "opteron-sse3", (const void *)&GenericModel },
  { "penryn", (const void *)&GenericModel },
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
  { "westmere", (const void *)&GenericModel },
  { "winchip-c6", (const void *)&GenericModel },
  { "winchip2", (const void *)&GenericModel },
  { "x86-64", (const void *)&GenericModel },
  { "yonah", (const void *)&GenericModel }
};
#undef DBGFIELD
static inline void InitX86MCSubtargetInfo(MCSubtargetInfo *II, StringRef TT, StringRef CPU, StringRef FS) {
  II->InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV, 
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable, 
                      X86Stages, X86OperandCycles, X86ForwardingPaths, 36, 51);
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
  if ((Bits & X86::FeatureAES) != 0) HasAES = true;
  if ((Bits & X86::FeatureAVX) != 0 && X86SSELevel < AVX) X86SSELevel = AVX;
  if ((Bits & X86::FeatureAVX2) != 0 && X86SSELevel < AVX2) X86SSELevel = AVX2;
  if ((Bits & X86::FeatureBMI) != 0) HasBMI = true;
  if ((Bits & X86::FeatureBMI2) != 0) HasBMI2 = true;
  if ((Bits & X86::FeatureCMOV) != 0) HasCMov = true;
  if ((Bits & X86::FeatureCMPXCHG16B) != 0) HasCmpxchg16b = true;
  if ((Bits & X86::FeatureF16C) != 0) HasF16C = true;
  if ((Bits & X86::FeatureFMA) != 0) HasFMA = true;
  if ((Bits & X86::FeatureFMA4) != 0) HasFMA4 = true;
  if ((Bits & X86::FeatureFSGSBase) != 0) HasFSGSBase = true;
  if ((Bits & X86::FeatureFastUAMem) != 0) IsUAMemFast = true;
  if ((Bits & X86::FeatureLZCNT) != 0) HasLZCNT = true;
  if ((Bits & X86::FeatureLeaForSP) != 0) UseLeaForSP = true;
  if ((Bits & X86::FeatureMMX) != 0 && X86SSELevel < MMX) X86SSELevel = MMX;
  if ((Bits & X86::FeatureMOVBE) != 0) HasMOVBE = true;
  if ((Bits & X86::FeaturePCLMUL) != 0) HasPCLMUL = true;
  if ((Bits & X86::FeaturePOPCNT) != 0) HasPOPCNT = true;
  if ((Bits & X86::FeatureRDRAND) != 0) HasRDRAND = true;
  if ((Bits & X86::FeatureRTM) != 0) HasRTM = true;
  if ((Bits & X86::FeatureSSE1) != 0 && X86SSELevel < SSE1) X86SSELevel = SSE1;
  if ((Bits & X86::FeatureSSE2) != 0 && X86SSELevel < SSE2) X86SSELevel = SSE2;
  if ((Bits & X86::FeatureSSE3) != 0 && X86SSELevel < SSE3) X86SSELevel = SSE3;
  if ((Bits & X86::FeatureSSE4A) != 0) HasSSE4A = true;
  if ((Bits & X86::FeatureSSE41) != 0 && X86SSELevel < SSE41) X86SSELevel = SSE41;
  if ((Bits & X86::FeatureSSE42) != 0 && X86SSELevel < SSE42) X86SSELevel = SSE42;
  if ((Bits & X86::FeatureSSSE3) != 0 && X86SSELevel < SSSE3) X86SSELevel = SSSE3;
  if ((Bits & X86::FeatureSlowBTMem) != 0) IsBTMemSlow = true;
  if ((Bits & X86::FeatureSlowDivide) != 0) HasSlowDivide = true;
  if ((Bits & X86::FeatureVectorUAMem) != 0) HasVectorUAMem = true;
  if ((Bits & X86::FeatureXOP) != 0) HasXOP = true;
  if ((Bits & X86::Mode64Bit) != 0) In64BitMode = true;
  if ((Bits & X86::ProcIntelAtom) != 0 && X86ProcFamily < IntelAtom) X86ProcFamily = IntelAtom;
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
                      X86Stages, X86OperandCycles, X86ForwardingPaths, 36, 51);
}

unsigned X86GenSubtargetInfo
::resolveSchedClass(unsigned SchedClass, const MachineInstr *MI, const TargetSchedModel *SchedModel) const {
  report_fatal_error("Expected a variant SchedClass");
} // X86GenSubtargetInfo::resolveSchedClass
} // End llvm namespace 
#endif // GET_SUBTARGETINFO_CTOR

