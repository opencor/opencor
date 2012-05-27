//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// Subtarget Enumeration Source Fragment
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//


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
  const uint64_t FeatureCLMUL = 1ULL << 8;
  const uint64_t FeatureCMOV = 1ULL << 9;
  const uint64_t FeatureCMPXCHG16B = 1ULL << 10;
  const uint64_t FeatureF16C = 1ULL << 11;
  const uint64_t FeatureFMA3 = 1ULL << 12;
  const uint64_t FeatureFMA4 = 1ULL << 13;
  const uint64_t FeatureFSGSBase = 1ULL << 14;
  const uint64_t FeatureFastUAMem = 1ULL << 15;
  const uint64_t FeatureLZCNT = 1ULL << 16;
  const uint64_t FeatureLeaForSP = 1ULL << 17;
  const uint64_t FeatureMMX = 1ULL << 18;
  const uint64_t FeatureMOVBE = 1ULL << 19;
  const uint64_t FeaturePOPCNT = 1ULL << 20;
  const uint64_t FeatureRDRAND = 1ULL << 21;
  const uint64_t FeatureSSE1 = 1ULL << 22;
  const uint64_t FeatureSSE2 = 1ULL << 23;
  const uint64_t FeatureSSE3 = 1ULL << 24;
  const uint64_t FeatureSSE4A = 1ULL << 25;
  const uint64_t FeatureSSE41 = 1ULL << 26;
  const uint64_t FeatureSSE42 = 1ULL << 27;
  const uint64_t FeatureSSSE3 = 1ULL << 28;
  const uint64_t FeatureSlowBTMem = 1ULL << 29;
  const uint64_t FeatureVectorUAMem = 1ULL << 30;
  const uint64_t FeatureXOP = 1ULL << 31;
  const uint64_t Mode64Bit = 1ULL << 32;
  const uint64_t ProcIntelAtom = 1ULL << 33;
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
  { "aes", "Enable AES instructions", X86::FeatureAES, 0ULL },
  { "atom", "Intel Atom processors", X86::ProcIntelAtom, 0ULL },
  { "avx", "Enable AVX instructions", X86::FeatureAVX, X86::FeatureSSE42 },
  { "avx2", "Enable AVX2 instructions", X86::FeatureAVX2, X86::FeatureAVX },
  { "bmi", "Support BMI instructions", X86::FeatureBMI, 0ULL },
  { "bmi2", "Support BMI2 instructions", X86::FeatureBMI2, 0ULL },
  { "clmul", "Enable carry-less multiplication instructions", X86::FeatureCLMUL, 0ULL },
  { "cmov", "Enable conditional move instructions", X86::FeatureCMOV, 0ULL },
  { "cmpxchg16b", "64-bit with cmpxchg16b", X86::FeatureCMPXCHG16B, X86::Feature64Bit },
  { "f16c", "Support 16-bit floating point conversion instructions", X86::FeatureF16C, 0ULL },
  { "fast-unaligned-mem", "Fast unaligned memory access", X86::FeatureFastUAMem, 0ULL },
  { "fma3", "Enable three-operand fused multiple-add", X86::FeatureFMA3, X86::FeatureAVX },
  { "fma4", "Enable four-operand fused multiple-add", X86::FeatureFMA4, X86::FeatureAVX },
  { "fsgsbase", "Support FS/GS Base instructions", X86::FeatureFSGSBase, 0ULL },
  { "lea-sp", "Use LEA for adjusting the stack pointer", X86::FeatureLeaForSP, 0ULL },
  { "lzcnt", "Support LZCNT instruction", X86::FeatureLZCNT, 0ULL },
  { "mmx", "Enable MMX instructions", X86::FeatureMMX, 0ULL },
  { "movbe", "Support MOVBE instruction", X86::FeatureMOVBE, 0ULL },
  { "popcnt", "Support POPCNT instruction", X86::FeaturePOPCNT, 0ULL },
  { "rdrand", "Support RDRAND instruction", X86::FeatureRDRAND, 0ULL },
  { "slow-bt-mem", "Bit testing of memory is slow", X86::FeatureSlowBTMem, 0ULL },
  { "sse", "Enable SSE instructions", X86::FeatureSSE1, X86::FeatureMMX | X86::FeatureCMOV },
  { "sse2", "Enable SSE2 instructions", X86::FeatureSSE2, X86::FeatureSSE1 },
  { "sse3", "Enable SSE3 instructions", X86::FeatureSSE3, X86::FeatureSSE2 },
  { "sse41", "Enable SSE 4.1 instructions", X86::FeatureSSE41, X86::FeatureSSSE3 },
  { "sse42", "Enable SSE 4.2 instructions", X86::FeatureSSE42, X86::FeatureSSE41 },
  { "sse4a", "Support SSE 4a instructions", X86::FeatureSSE4A, X86::FeatureSSE3 },
  { "ssse3", "Enable SSSE3 instructions", X86::FeatureSSSE3, X86::FeatureSSE3 },
  { "vector-unaligned-mem", "Allow unaligned memory operands on vector/SIMD instructions", X86::FeatureVectorUAMem, 0ULL },
  { "xop", "Enable XOP instructions", X86::FeatureXOP, 0ULL }
};

// Sorted (by key) array of values for CPU subtype.
extern const llvm::SubtargetFeatureKV X86SubTypeKV[] = {
  { "amdfam10", "Select the amdfam10 processor", X86::FeatureSSE3 | X86::FeatureSSE4A | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowBTMem, 0ULL },
  { "athlon", "Select the athlon processor", X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-4", "Select the athlon-4 processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-fx", "Select the athlon-fx processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-mp", "Select the athlon-mp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-tbird", "Select the athlon-tbird processor", X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon-xp", "Select the athlon-xp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem, 0ULL },
  { "athlon64", "Select the athlon64 processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "athlon64-sse3", "Select the athlon64-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "atom", "Select the atom processor", X86::ProcIntelAtom | X86::FeatureSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeatureSlowBTMem | X86::FeatureLeaForSP, 0ULL },
  { "bdver1", "Select the bdver1 processor", X86::FeatureSSE42 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeatureCLMUL | X86::FeatureXOP | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "bdver2", "Select the bdver2 processor", X86::FeatureSSE42 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeatureCLMUL | X86::FeatureXOP | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI, 0ULL },
  { "btver1", "Select the btver1 processor", X86::FeatureSSSE3 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeatureLZCNT | X86::FeaturePOPCNT, 0ULL },
  { "c3", "Select the c3 processor", X86::Feature3DNow, 0ULL },
  { "c3-2", "Select the c3-2 processor", X86::FeatureSSE1, 0ULL },
  { "core-avx-i", "Select the core-avx-i processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeatureCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase, 0ULL },
  { "core-avx2", "Select the core-avx2 processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeatureCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2, 0ULL },
  { "core2", "Select the core2 processor", X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "corei7", "Select the corei7 processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES, 0ULL },
  { "corei7-avx", "Select the corei7-avx processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeatureCLMUL, 0ULL },
  { "generic", "Select the generic processor", 0ULL, 0ULL },
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
  { "westmere", "Select the westmere processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeatureCLMUL, 0ULL },
  { "winchip-c6", "Select the winchip-c6 processor", X86::FeatureMMX, 0ULL },
  { "winchip2", "Select the winchip2 processor", X86::Feature3DNow, 0ULL },
  { "x86-64", "Select the x86-64 processor", X86::FeatureSSE2 | X86::Feature64Bit | X86::FeatureSlowBTMem, 0ULL },
  { "yonah", "Select the yonah processor", X86::FeatureSSE3 | X86::FeatureSlowBTMem, 0ULL }
};


// Functional units for itineraries "AtomItineraries"
namespace AtomItinerariesFU {
  const unsigned Port0 = 1 << 0;
  const unsigned Port1 = 1 << 1;
}

extern const llvm::InstrStage X86Stages[] = {
  { 0, 0, 0, llvm::InstrStage::Required }, // No itinerary
  { 1, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 1
  { 1, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 2
  { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 3
  { 2, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 4
  { 7, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 5
  { 8, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 6
  { 6, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 7
  { 12, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 8
  { 5, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 9
  { 14, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 10
  { 62, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 11
  { 130, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 12
  { 50, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 13
  { 68, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 14
  { 4, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 15
  { 9, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 16
  { 32, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 17
  { 31, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 18
  { 18, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 19
  { 17, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 20
  { 1, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 1, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 21-22
  { 15, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 23
  { 40, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 24
  { 39, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 25
  { 79, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 26
  { 3, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 27
  { 75, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 28
  { 74, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 29
  { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 30
  { 5, AtomItinerariesFU::Port0, 0, (llvm::InstrStage::ReservationKinds)0 },   { 5, AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 31-32
  { 4, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 33
  { 34, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 34
  { 10, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 35
  { 70, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 36
  { 125, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 37
  { 13, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 38
  { 11, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 39
  { 3, AtomItinerariesFU::Port0, -1, (llvm::InstrStage::ReservationKinds)0 }, // 40
  { 5, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 41
  { 46, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 42
  { 45, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 43
  { 22, AtomItinerariesFU::Port0 | AtomItinerariesFU::Port1, -1, (llvm::InstrStage::ReservationKinds)0 }, // 44
  { 0, 0, 0, llvm::InstrStage::Required } // End itinerary
};
extern const unsigned X86OperandCycles[] = {
  0, // No itinerary
  0 // End itinerary
};
extern const unsigned X86ForwardingPathes[] = {
  0, // No itinerary
  0 // End itinerary
};

static const llvm::InstrItinerary AtomItineraries[] = {
  { 1, 2, 3, 0, 0 }, // 0 IIC_ALU_MEM
  { 1, 1, 2, 0, 0 }, // 1 IIC_ALU_NONMEM
  { 1, 2, 3, 0, 0 }, // 2 IIC_BIN_MEM
  { 1, 1, 2, 0, 0 }, // 3 IIC_BIN_NONMEM
  { 1, 24, 25, 0, 0 }, // 4 IIC_CALL_FAR_MEM
  { 1, 25, 26, 0, 0 }, // 5 IIC_CALL_FAR_PTR
  { 1, 23, 24, 0, 0 }, // 6 IIC_CALL_MEM
  { 1, 21, 23, 0, 0 }, // 7 IIC_CALL_RI
  { 1, 2, 3, 0, 0 }, // 8 IIC_CMOV16_RM
  { 1, 1, 2, 0, 0 }, // 9 IIC_CMOV16_RR
  { 1, 2, 3, 0, 0 }, // 10 IIC_CMOV32_RM
  { 1, 1, 2, 0, 0 }, // 11 IIC_CMOV32_RR
  { 1, 2, 3, 0, 0 }, // 12 IIC_CMOV64_RM
  { 1, 1, 2, 0, 0 }, // 13 IIC_CMOV64_RR
  { 1, 10, 11, 0, 0 }, // 14 IIC_CMPX_LOCK
  { 1, 7, 8, 0, 0 }, // 15 IIC_CMPX_LOCK_8
  { 1, 19, 20, 0, 0 }, // 16 IIC_CMPX_LOCK_8B
  { 1, 44, 45, 0, 0 }, // 17 IIC_CMPX_LOCK_16B
  { 1, 1, 2, 0, 0 }, // 18 IIC_DEFAULT
  { 1, 14, 15, 0, 0 }, // 19 IIC_DIV8_MEM
  { 1, 13, 14, 0, 0 }, // 20 IIC_DIV8_REG
  { 1, 13, 14, 0, 0 }, // 21 IIC_DIV16
  { 1, 13, 14, 0, 0 }, // 22 IIC_DIV32
  { 1, 12, 13, 0, 0 }, // 23 IIC_DIV64
  { 1, 11, 12, 0, 0 }, // 24 IIC_IDIV8
  { 1, 11, 12, 0, 0 }, // 25 IIC_IDIV16
  { 1, 11, 12, 0, 0 }, // 26 IIC_IDIV32
  { 1, 12, 13, 0, 0 }, // 27 IIC_IDIV64
  { 1, 5, 6, 0, 0 }, // 28 IIC_IMUL8
  { 1, 6, 7, 0, 0 }, // 29 IIC_IMUL16_MEM
  { 1, 5, 6, 0, 0 }, // 30 IIC_IMUL16_REG
  { 1, 5, 6, 0, 0 }, // 31 IIC_IMUL16_RM
  { 1, 5, 6, 0, 0 }, // 32 IIC_IMUL16_RMI
  { 1, 7, 8, 0, 0 }, // 33 IIC_IMUL16_RR
  { 1, 7, 8, 0, 0 }, // 34 IIC_IMUL16_RRI
  { 1, 5, 6, 0, 0 }, // 35 IIC_IMUL32_MEM
  { 1, 7, 8, 0, 0 }, // 36 IIC_IMUL32_REG
  { 1, 9, 10, 0, 0 }, // 37 IIC_IMUL32_RM
  { 1, 9, 10, 0, 0 }, // 38 IIC_IMUL32_RMI
  { 1, 9, 10, 0, 0 }, // 39 IIC_IMUL32_RR
  { 1, 9, 10, 0, 0 }, // 40 IIC_IMUL32_RRI
  { 1, 8, 9, 0, 0 }, // 41 IIC_IMUL64
  { 1, 8, 9, 0, 0 }, // 42 IIC_IMUL64_RM
  { 1, 10, 11, 0, 0 }, // 43 IIC_IMUL64_RMI
  { 1, 8, 9, 0, 0 }, // 44 IIC_IMUL64_RR
  { 1, 10, 11, 0, 0 }, // 45 IIC_IMUL64_RRI
  { 1, 15, 16, 0, 0 }, // 46 IIC_JCXZ
  { 1, 17, 18, 0, 0 }, // 47 IIC_JMP_FAR_MEM
  { 1, 18, 19, 0, 0 }, // 48 IIC_JMP_FAR_PTR
  { 1, 4, 5, 0, 0 }, // 49 IIC_JMP_MEM
  { 1, 3, 4, 0, 0 }, // 50 IIC_JMP_REG
  { 1, 3, 4, 0, 0 }, // 51 IIC_JMP_REL
  { 1, 3, 4, 0, 0 }, // 52 IIC_Jcc
  { 1, 3, 4, 0, 0 }, // 53 IIC_LEA
  { 1, 4, 5, 0, 0 }, // 54 IIC_LEA_16
  { 1, 19, 20, 0, 0 }, // 55 IIC_LOOP
  { 1, 6, 7, 0, 0 }, // 56 IIC_LOOPE
  { 1, 20, 21, 0, 0 }, // 57 IIC_LOOPNE
  { 1, 2, 3, 0, 0 }, // 58 IIC_MOVSX
  { 1, 27, 28, 0, 0 }, // 59 IIC_MOVSX_R16_M8
  { 1, 4, 5, 0, 0 }, // 60 IIC_MOVSX_R16_R8
  { 1, 1, 2, 0, 0 }, // 61 IIC_MOVSX_R16_R16
  { 1, 1, 2, 0, 0 }, // 62 IIC_MOVSX_R32_R32
  { 1, 2, 3, 0, 0 }, // 63 IIC_MOVZX
  { 1, 27, 28, 0, 0 }, // 64 IIC_MOVZX_R16_M8
  { 1, 4, 5, 0, 0 }, // 65 IIC_MOVZX_R16_R8
  { 1, 5, 6, 0, 0 }, // 66 IIC_MUL8
  { 1, 6, 7, 0, 0 }, // 67 IIC_MUL16_MEM
  { 1, 5, 6, 0, 0 }, // 68 IIC_MUL16_REG
  { 1, 5, 6, 0, 0 }, // 69 IIC_MUL32_MEM
  { 1, 7, 8, 0, 0 }, // 70 IIC_MUL32_REG
  { 1, 8, 9, 0, 0 }, // 71 IIC_MUL64
  { 1, 28, 29, 0, 0 }, // 72 IIC_REP_MOVS
  { 1, 29, 30, 0, 0 }, // 73 IIC_REP_STOS
  { 1, 26, 27, 0, 0 }, // 74 IIC_RET
  { 1, 21, 23, 0, 0 }, // 75 IIC_RET_IMM
  { 1, 4, 5, 0, 0 }, // 76 IIC_SET_M
  { 1, 1, 2, 0, 0 }, // 77 IIC_SET_R
  { 1, 7, 8, 0, 0 }, // 78 IIC_SHD16_MEM_CL
  { 1, 7, 8, 0, 0 }, // 79 IIC_SHD16_MEM_IM
  { 1, 7, 8, 0, 0 }, // 80 IIC_SHD16_REG_CL
  { 1, 7, 8, 0, 0 }, // 81 IIC_SHD16_REG_IM
  { 1, 15, 16, 0, 0 }, // 82 IIC_SHD32_MEM_CL
  { 1, 15, 16, 0, 0 }, // 83 IIC_SHD32_MEM_IM
  { 1, 4, 5, 0, 0 }, // 84 IIC_SHD32_REG_CL
  { 1, 4, 5, 0, 0 }, // 85 IIC_SHD32_REG_IM
  { 1, 16, 17, 0, 0 }, // 86 IIC_SHD64_MEM_CL
  { 1, 16, 17, 0, 0 }, // 87 IIC_SHD64_MEM_IM
  { 1, 6, 7, 0, 0 }, // 88 IIC_SHD64_REG_CL
  { 1, 16, 17, 0, 0 }, // 89 IIC_SHD64_REG_IM
  { 1, 2, 3, 0, 0 }, // 90 IIC_SR
  { 1, 31, 33, 0, 0 }, // 91 IIC_SSE_ALU_F32P_RM
  { 1, 30, 31, 0, 0 }, // 92 IIC_SSE_ALU_F32P_RR
  { 1, 31, 33, 0, 0 }, // 93 IIC_SSE_ALU_F32S_RM
  { 1, 30, 31, 0, 0 }, // 94 IIC_SSE_ALU_F32S_RR
  { 1, 5, 6, 0, 0 }, // 95 IIC_SSE_ALU_F64P_RM
  { 1, 7, 8, 0, 0 }, // 96 IIC_SSE_ALU_F64P_RR
  { 1, 31, 33, 0, 0 }, // 97 IIC_SSE_ALU_F64S_RM
  { 1, 30, 31, 0, 0 }, // 98 IIC_SSE_ALU_F64S_RR
  { 1, 2, 3, 0, 0 }, // 99 IIC_SSE_BIT_P_RM
  { 1, 1, 2, 0, 0 }, // 100 IIC_SSE_BIT_P_RR
  { 1, 5, 6, 0, 0 }, // 101 IIC_SSE_CMPP_RM
  { 1, 7, 8, 0, 0 }, // 102 IIC_SSE_CMPP_RR
  { 1, 35, 36, 0, 0 }, // 103 IIC_SSE_COMIS_RM
  { 1, 16, 17, 0, 0 }, // 104 IIC_SSE_COMIS_RR
  { 1, 6, 7, 0, 0 }, // 105 IIC_SSE_CVT_PD_RM
  { 1, 5, 6, 0, 0 }, // 106 IIC_SSE_CVT_PD_RR
  { 1, 0, 0, 0, 0 }, // 107 IIC_SSE_CVT_PI2PS_RM
  { 1, 0, 0, 0, 0 }, // 108 IIC_SSE_CVT_PI2PS_RR
  { 1, 5, 6, 0, 0 }, // 109 IIC_SSE_CVT_PS_RM
  { 1, 7, 8, 0, 0 }, // 110 IIC_SSE_CVT_PS_RR
  { 1, 16, 17, 0, 0 }, // 111 IIC_SSE_CVT_SD2SI_RM
  { 1, 6, 7, 0, 0 }, // 112 IIC_SSE_CVT_SD2SI_RR
  { 1, 16, 17, 0, 0 }, // 113 IIC_SSE_CVT_SS2SI32_RM
  { 1, 6, 7, 0, 0 }, // 114 IIC_SSE_CVT_SS2SI32_RR
  { 1, 35, 36, 0, 0 }, // 115 IIC_SSE_CVT_SS2SI64_RM
  { 1, 16, 17, 0, 0 }, // 116 IIC_SSE_CVT_SS2SI64_RR
  { 1, 5, 6, 0, 0 }, // 117 IIC_SSE_CVT_Scalar_RM
  { 1, 7, 8, 0, 0 }, // 118 IIC_SSE_CVT_Scalar_RR
  { 1, 36, 37, 0, 0 }, // 119 IIC_SSE_DIV_F32P_RM
  { 1, 36, 37, 0, 0 }, // 120 IIC_SSE_DIV_F32P_RR
  { 1, 34, 35, 0, 0 }, // 121 IIC_SSE_DIV_F32S_RM
  { 1, 34, 35, 0, 0 }, // 122 IIC_SSE_DIV_F32S_RR
  { 1, 37, 38, 0, 0 }, // 123 IIC_SSE_DIV_F64P_RM
  { 1, 37, 38, 0, 0 }, // 124 IIC_SSE_DIV_F64P_RR
  { 1, 11, 12, 0, 0 }, // 125 IIC_SSE_DIV_F64S_RM
  { 1, 11, 12, 0, 0 }, // 126 IIC_SSE_DIV_F64S_RR
  { 1, 16, 17, 0, 0 }, // 127 IIC_SSE_HADDSUB_RM
  { 1, 6, 7, 0, 0 }, // 128 IIC_SSE_HADDSUB_RR
  { 1, 27, 28, 0, 0 }, // 129 IIC_SSE_INTALUQ_P_RM
  { 1, 4, 5, 0, 0 }, // 130 IIC_SSE_INTALUQ_P_RR
  { 1, 2, 3, 0, 0 }, // 131 IIC_SSE_INTALU_P_RM
  { 1, 1, 2, 0, 0 }, // 132 IIC_SSE_INTALU_P_RR
  { 1, 9, 10, 0, 0 }, // 133 IIC_SSE_INTMUL_P_RM
  { 1, 9, 10, 0, 0 }, // 134 IIC_SSE_INTMUL_P_RR
  { 1, 1, 2, 0, 0 }, // 135 IIC_SSE_INTSH_P_RI
  { 1, 27, 28, 0, 0 }, // 136 IIC_SSE_INTSH_P_RM
  { 1, 4, 5, 0, 0 }, // 137 IIC_SSE_INTSH_P_RR
  { 1, 27, 28, 0, 0 }, // 138 IIC_SSE_LDDQU
  { 1, 41, 42, 0, 0 }, // 139 IIC_SSE_LDMXCSR
  { 1, 1, 2, 0, 0 }, // 140 IIC_SSE_LFENCE
  { 1, 4, 5, 0, 0 }, // 141 IIC_SSE_MASKMOV
  { 1, 2, 3, 0, 0 }, // 142 IIC_SSE_MFENCE
  { 1, 43, 44, 0, 0 }, // 143 IIC_SSE_MONITOR
  { 1, 2, 3, 0, 0 }, // 144 IIC_SSE_MOVA_P_MR
  { 1, 2, 3, 0, 0 }, // 145 IIC_SSE_MOVA_P_RM
  { 1, 1, 2, 0, 0 }, // 146 IIC_SSE_MOVA_P_RR
  { 1, 2, 3, 0, 0 }, // 147 IIC_SSE_MOVDQ
  { 1, 40, 41, 0, 0 }, // 148 IIC_SSE_MOVD_ToGP
  { 1, 40, 41, 0, 0 }, // 149 IIC_SSE_MOVMSK
  { 1, 2, 3, 0, 0 }, // 150 IIC_SSE_MOVNT
  { 1, 1, 2, 0, 0 }, // 151 IIC_SSE_MOVQ_RR
  { 1, 4, 5, 0, 0 }, // 152 IIC_SSE_MOVU_P_MR
  { 1, 27, 28, 0, 0 }, // 153 IIC_SSE_MOVU_P_RM
  { 1, 1, 2, 0, 0 }, // 154 IIC_SSE_MOVU_P_RR
  { 1, 2, 3, 0, 0 }, // 155 IIC_SSE_MOV_LH
  { 1, 2, 3, 0, 0 }, // 156 IIC_SSE_MOV_S_MR
  { 1, 2, 3, 0, 0 }, // 157 IIC_SSE_MOV_S_RM
  { 1, 1, 2, 0, 0 }, // 158 IIC_SSE_MOV_S_RR
  { 1, 9, 10, 0, 0 }, // 159 IIC_SSE_MUL_F32P_RM
  { 1, 9, 10, 0, 0 }, // 160 IIC_SSE_MUL_F32P_RR
  { 1, 33, 34, 0, 0 }, // 161 IIC_SSE_MUL_F32S_RM
  { 1, 33, 34, 0, 0 }, // 162 IIC_SSE_MUL_F32S_RR
  { 1, 35, 36, 0, 0 }, // 163 IIC_SSE_MUL_F64P_RM
  { 1, 16, 17, 0, 0 }, // 164 IIC_SSE_MUL_F64P_RR
  { 1, 9, 10, 0, 0 }, // 165 IIC_SSE_MUL_F64S_RM
  { 1, 9, 10, 0, 0 }, // 166 IIC_SSE_MUL_F64S_RR
  { 1, 42, 43, 0, 0 }, // 167 IIC_SSE_MWAIT
  { 1, 2, 3, 0, 0 }, // 168 IIC_SSE_PABS_RM
  { 1, 1, 2, 0, 0 }, // 169 IIC_SSE_PABS_RR
  { 1, 2, 3, 0, 0 }, // 170 IIC_SSE_PALIGNR
  { 1, 20, 21, 0, 0 }, // 171 IIC_SSE_PAUSE
  { 1, 15, 16, 0, 0 }, // 172 IIC_SSE_PEXTRW
  { 1, 15, 16, 0, 0 }, // 173 IIC_SSE_PHADDSUBD_RM
  { 1, 27, 28, 0, 0 }, // 174 IIC_SSE_PHADDSUBD_RR
  { 1, 6, 7, 0, 0 }, // 175 IIC_SSE_PHADDSUBSW_RM
  { 1, 5, 6, 0, 0 }, // 176 IIC_SSE_PHADDSUBSW_RR
  { 1, 6, 7, 0, 0 }, // 177 IIC_SSE_PHADDSUBW_RM
  { 1, 5, 6, 0, 0 }, // 178 IIC_SSE_PHADDSUBW_RR
  { 1, 2, 3, 0, 0 }, // 179 IIC_SSE_PINSRW
  { 1, 9, 10, 0, 0 }, // 180 IIC_SSE_PMADD
  { 1, 9, 10, 0, 0 }, // 181 IIC_SSE_PMULHRSW
  { 1, 2, 3, 0, 0 }, // 182 IIC_SSE_PREFETCH
  { 1, 2, 3, 0, 0 }, // 183 IIC_SSE_PSHUF
  { 1, 41, 42, 0, 0 }, // 184 IIC_SSE_PSHUFB_RM
  { 1, 15, 16, 0, 0 }, // 185 IIC_SSE_PSHUFB_RR
  { 1, 2, 3, 0, 0 }, // 186 IIC_SSE_PSIGN_RM
  { 1, 1, 2, 0, 0 }, // 187 IIC_SSE_PSIGN_RR
  { 1, 35, 36, 0, 0 }, // 188 IIC_SSE_RCPP_RM
  { 1, 16, 17, 0, 0 }, // 189 IIC_SSE_RCPP_RR
  { 1, 33, 34, 0, 0 }, // 190 IIC_SSE_RCPS_RM
  { 1, 33, 34, 0, 0 }, // 191 IIC_SSE_RCPS_RR
  { 1, 2, 3, 0, 0 }, // 192 IIC_SSE_SFENCE
  { 1, 2, 3, 0, 0 }, // 193 IIC_SSE_SHUFP
  { 1, 10, 11, 0, 0 }, // 194 IIC_SSE_SQRTP_RM
  { 1, 38, 39, 0, 0 }, // 195 IIC_SSE_SQRTP_RR
  { 1, 8, 9, 0, 0 }, // 196 IIC_SSE_SQRTS_RM
  { 1, 39, 40, 0, 0 }, // 197 IIC_SSE_SQRTS_RR
  { 1, 23, 24, 0, 0 }, // 198 IIC_SSE_STMXCSR
  { 1, 2, 3, 0, 0 }, // 199 IIC_SSE_UNPCK
  { 1, 2, 3, 0, 0 }, // 200 IIC_UNARY_MEM
  { 1, 1, 2, 0, 0 }, // 201 IIC_UNARY_REG
  { 1, 27, 28, 0, 0 }, // 202 IIC_XADD_LOCK_MEM
  { 1, 0, 0, 0, 0 }, // 203 IIC_XADD_LOCK_MEM8
  { 1, 0, 0, 0, 0 }, // 204 NoItinerary
  { 1, ~0U, ~0U, ~0U, ~0U } // end marker
};

static const llvm::InstrItinerary GenericItineraries[] = {
  { 1, 0, 0, 0, 0 }, // 0 IIC_ALU_MEM
  { 1, 0, 0, 0, 0 }, // 1 IIC_ALU_NONMEM
  { 1, 0, 0, 0, 0 }, // 2 IIC_BIN_MEM
  { 1, 0, 0, 0, 0 }, // 3 IIC_BIN_NONMEM
  { 1, 0, 0, 0, 0 }, // 4 IIC_CALL_FAR_MEM
  { 1, 0, 0, 0, 0 }, // 5 IIC_CALL_FAR_PTR
  { 1, 0, 0, 0, 0 }, // 6 IIC_CALL_MEM
  { 1, 0, 0, 0, 0 }, // 7 IIC_CALL_RI
  { 1, 0, 0, 0, 0 }, // 8 IIC_CMOV16_RM
  { 1, 0, 0, 0, 0 }, // 9 IIC_CMOV16_RR
  { 1, 0, 0, 0, 0 }, // 10 IIC_CMOV32_RM
  { 1, 0, 0, 0, 0 }, // 11 IIC_CMOV32_RR
  { 1, 0, 0, 0, 0 }, // 12 IIC_CMOV64_RM
  { 1, 0, 0, 0, 0 }, // 13 IIC_CMOV64_RR
  { 1, 0, 0, 0, 0 }, // 14 IIC_CMPX_LOCK
  { 1, 0, 0, 0, 0 }, // 15 IIC_CMPX_LOCK_8
  { 1, 0, 0, 0, 0 }, // 16 IIC_CMPX_LOCK_8B
  { 1, 0, 0, 0, 0 }, // 17 IIC_CMPX_LOCK_16B
  { 1, 0, 0, 0, 0 }, // 18 IIC_DEFAULT
  { 1, 0, 0, 0, 0 }, // 19 IIC_DIV8_MEM
  { 1, 0, 0, 0, 0 }, // 20 IIC_DIV8_REG
  { 1, 0, 0, 0, 0 }, // 21 IIC_DIV16
  { 1, 0, 0, 0, 0 }, // 22 IIC_DIV32
  { 1, 0, 0, 0, 0 }, // 23 IIC_DIV64
  { 1, 0, 0, 0, 0 }, // 24 IIC_IDIV8
  { 1, 0, 0, 0, 0 }, // 25 IIC_IDIV16
  { 1, 0, 0, 0, 0 }, // 26 IIC_IDIV32
  { 1, 0, 0, 0, 0 }, // 27 IIC_IDIV64
  { 1, 0, 0, 0, 0 }, // 28 IIC_IMUL8
  { 1, 0, 0, 0, 0 }, // 29 IIC_IMUL16_MEM
  { 1, 0, 0, 0, 0 }, // 30 IIC_IMUL16_REG
  { 1, 0, 0, 0, 0 }, // 31 IIC_IMUL16_RM
  { 1, 0, 0, 0, 0 }, // 32 IIC_IMUL16_RMI
  { 1, 0, 0, 0, 0 }, // 33 IIC_IMUL16_RR
  { 1, 0, 0, 0, 0 }, // 34 IIC_IMUL16_RRI
  { 1, 0, 0, 0, 0 }, // 35 IIC_IMUL32_MEM
  { 1, 0, 0, 0, 0 }, // 36 IIC_IMUL32_REG
  { 1, 0, 0, 0, 0 }, // 37 IIC_IMUL32_RM
  { 1, 0, 0, 0, 0 }, // 38 IIC_IMUL32_RMI
  { 1, 0, 0, 0, 0 }, // 39 IIC_IMUL32_RR
  { 1, 0, 0, 0, 0 }, // 40 IIC_IMUL32_RRI
  { 1, 0, 0, 0, 0 }, // 41 IIC_IMUL64
  { 1, 0, 0, 0, 0 }, // 42 IIC_IMUL64_RM
  { 1, 0, 0, 0, 0 }, // 43 IIC_IMUL64_RMI
  { 1, 0, 0, 0, 0 }, // 44 IIC_IMUL64_RR
  { 1, 0, 0, 0, 0 }, // 45 IIC_IMUL64_RRI
  { 1, 0, 0, 0, 0 }, // 46 IIC_JCXZ
  { 1, 0, 0, 0, 0 }, // 47 IIC_JMP_FAR_MEM
  { 1, 0, 0, 0, 0 }, // 48 IIC_JMP_FAR_PTR
  { 1, 0, 0, 0, 0 }, // 49 IIC_JMP_MEM
  { 1, 0, 0, 0, 0 }, // 50 IIC_JMP_REG
  { 1, 0, 0, 0, 0 }, // 51 IIC_JMP_REL
  { 1, 0, 0, 0, 0 }, // 52 IIC_Jcc
  { 1, 0, 0, 0, 0 }, // 53 IIC_LEA
  { 1, 0, 0, 0, 0 }, // 54 IIC_LEA_16
  { 1, 0, 0, 0, 0 }, // 55 IIC_LOOP
  { 1, 0, 0, 0, 0 }, // 56 IIC_LOOPE
  { 1, 0, 0, 0, 0 }, // 57 IIC_LOOPNE
  { 1, 0, 0, 0, 0 }, // 58 IIC_MOVSX
  { 1, 0, 0, 0, 0 }, // 59 IIC_MOVSX_R16_M8
  { 1, 0, 0, 0, 0 }, // 60 IIC_MOVSX_R16_R8
  { 1, 0, 0, 0, 0 }, // 61 IIC_MOVSX_R16_R16
  { 1, 0, 0, 0, 0 }, // 62 IIC_MOVSX_R32_R32
  { 1, 0, 0, 0, 0 }, // 63 IIC_MOVZX
  { 1, 0, 0, 0, 0 }, // 64 IIC_MOVZX_R16_M8
  { 1, 0, 0, 0, 0 }, // 65 IIC_MOVZX_R16_R8
  { 1, 0, 0, 0, 0 }, // 66 IIC_MUL8
  { 1, 0, 0, 0, 0 }, // 67 IIC_MUL16_MEM
  { 1, 0, 0, 0, 0 }, // 68 IIC_MUL16_REG
  { 1, 0, 0, 0, 0 }, // 69 IIC_MUL32_MEM
  { 1, 0, 0, 0, 0 }, // 70 IIC_MUL32_REG
  { 1, 0, 0, 0, 0 }, // 71 IIC_MUL64
  { 1, 0, 0, 0, 0 }, // 72 IIC_REP_MOVS
  { 1, 0, 0, 0, 0 }, // 73 IIC_REP_STOS
  { 1, 0, 0, 0, 0 }, // 74 IIC_RET
  { 1, 0, 0, 0, 0 }, // 75 IIC_RET_IMM
  { 1, 0, 0, 0, 0 }, // 76 IIC_SET_M
  { 1, 0, 0, 0, 0 }, // 77 IIC_SET_R
  { 1, 0, 0, 0, 0 }, // 78 IIC_SHD16_MEM_CL
  { 1, 0, 0, 0, 0 }, // 79 IIC_SHD16_MEM_IM
  { 1, 0, 0, 0, 0 }, // 80 IIC_SHD16_REG_CL
  { 1, 0, 0, 0, 0 }, // 81 IIC_SHD16_REG_IM
  { 1, 0, 0, 0, 0 }, // 82 IIC_SHD32_MEM_CL
  { 1, 0, 0, 0, 0 }, // 83 IIC_SHD32_MEM_IM
  { 1, 0, 0, 0, 0 }, // 84 IIC_SHD32_REG_CL
  { 1, 0, 0, 0, 0 }, // 85 IIC_SHD32_REG_IM
  { 1, 0, 0, 0, 0 }, // 86 IIC_SHD64_MEM_CL
  { 1, 0, 0, 0, 0 }, // 87 IIC_SHD64_MEM_IM
  { 1, 0, 0, 0, 0 }, // 88 IIC_SHD64_REG_CL
  { 1, 0, 0, 0, 0 }, // 89 IIC_SHD64_REG_IM
  { 1, 0, 0, 0, 0 }, // 90 IIC_SR
  { 1, 0, 0, 0, 0 }, // 91 IIC_SSE_ALU_F32P_RM
  { 1, 0, 0, 0, 0 }, // 92 IIC_SSE_ALU_F32P_RR
  { 1, 0, 0, 0, 0 }, // 93 IIC_SSE_ALU_F32S_RM
  { 1, 0, 0, 0, 0 }, // 94 IIC_SSE_ALU_F32S_RR
  { 1, 0, 0, 0, 0 }, // 95 IIC_SSE_ALU_F64P_RM
  { 1, 0, 0, 0, 0 }, // 96 IIC_SSE_ALU_F64P_RR
  { 1, 0, 0, 0, 0 }, // 97 IIC_SSE_ALU_F64S_RM
  { 1, 0, 0, 0, 0 }, // 98 IIC_SSE_ALU_F64S_RR
  { 1, 0, 0, 0, 0 }, // 99 IIC_SSE_BIT_P_RM
  { 1, 0, 0, 0, 0 }, // 100 IIC_SSE_BIT_P_RR
  { 1, 0, 0, 0, 0 }, // 101 IIC_SSE_CMPP_RM
  { 1, 0, 0, 0, 0 }, // 102 IIC_SSE_CMPP_RR
  { 1, 0, 0, 0, 0 }, // 103 IIC_SSE_COMIS_RM
  { 1, 0, 0, 0, 0 }, // 104 IIC_SSE_COMIS_RR
  { 1, 0, 0, 0, 0 }, // 105 IIC_SSE_CVT_PD_RM
  { 1, 0, 0, 0, 0 }, // 106 IIC_SSE_CVT_PD_RR
  { 1, 0, 0, 0, 0 }, // 107 IIC_SSE_CVT_PI2PS_RM
  { 1, 0, 0, 0, 0 }, // 108 IIC_SSE_CVT_PI2PS_RR
  { 1, 0, 0, 0, 0 }, // 109 IIC_SSE_CVT_PS_RM
  { 1, 0, 0, 0, 0 }, // 110 IIC_SSE_CVT_PS_RR
  { 1, 0, 0, 0, 0 }, // 111 IIC_SSE_CVT_SD2SI_RM
  { 1, 0, 0, 0, 0 }, // 112 IIC_SSE_CVT_SD2SI_RR
  { 1, 0, 0, 0, 0 }, // 113 IIC_SSE_CVT_SS2SI32_RM
  { 1, 0, 0, 0, 0 }, // 114 IIC_SSE_CVT_SS2SI32_RR
  { 1, 0, 0, 0, 0 }, // 115 IIC_SSE_CVT_SS2SI64_RM
  { 1, 0, 0, 0, 0 }, // 116 IIC_SSE_CVT_SS2SI64_RR
  { 1, 0, 0, 0, 0 }, // 117 IIC_SSE_CVT_Scalar_RM
  { 1, 0, 0, 0, 0 }, // 118 IIC_SSE_CVT_Scalar_RR
  { 1, 0, 0, 0, 0 }, // 119 IIC_SSE_DIV_F32P_RM
  { 1, 0, 0, 0, 0 }, // 120 IIC_SSE_DIV_F32P_RR
  { 1, 0, 0, 0, 0 }, // 121 IIC_SSE_DIV_F32S_RM
  { 1, 0, 0, 0, 0 }, // 122 IIC_SSE_DIV_F32S_RR
  { 1, 0, 0, 0, 0 }, // 123 IIC_SSE_DIV_F64P_RM
  { 1, 0, 0, 0, 0 }, // 124 IIC_SSE_DIV_F64P_RR
  { 1, 0, 0, 0, 0 }, // 125 IIC_SSE_DIV_F64S_RM
  { 1, 0, 0, 0, 0 }, // 126 IIC_SSE_DIV_F64S_RR
  { 1, 0, 0, 0, 0 }, // 127 IIC_SSE_HADDSUB_RM
  { 1, 0, 0, 0, 0 }, // 128 IIC_SSE_HADDSUB_RR
  { 1, 0, 0, 0, 0 }, // 129 IIC_SSE_INTALUQ_P_RM
  { 1, 0, 0, 0, 0 }, // 130 IIC_SSE_INTALUQ_P_RR
  { 1, 0, 0, 0, 0 }, // 131 IIC_SSE_INTALU_P_RM
  { 1, 0, 0, 0, 0 }, // 132 IIC_SSE_INTALU_P_RR
  { 1, 0, 0, 0, 0 }, // 133 IIC_SSE_INTMUL_P_RM
  { 1, 0, 0, 0, 0 }, // 134 IIC_SSE_INTMUL_P_RR
  { 1, 0, 0, 0, 0 }, // 135 IIC_SSE_INTSH_P_RI
  { 1, 0, 0, 0, 0 }, // 136 IIC_SSE_INTSH_P_RM
  { 1, 0, 0, 0, 0 }, // 137 IIC_SSE_INTSH_P_RR
  { 1, 0, 0, 0, 0 }, // 138 IIC_SSE_LDDQU
  { 1, 0, 0, 0, 0 }, // 139 IIC_SSE_LDMXCSR
  { 1, 0, 0, 0, 0 }, // 140 IIC_SSE_LFENCE
  { 1, 0, 0, 0, 0 }, // 141 IIC_SSE_MASKMOV
  { 1, 0, 0, 0, 0 }, // 142 IIC_SSE_MFENCE
  { 1, 0, 0, 0, 0 }, // 143 IIC_SSE_MONITOR
  { 1, 0, 0, 0, 0 }, // 144 IIC_SSE_MOVA_P_MR
  { 1, 0, 0, 0, 0 }, // 145 IIC_SSE_MOVA_P_RM
  { 1, 0, 0, 0, 0 }, // 146 IIC_SSE_MOVA_P_RR
  { 1, 0, 0, 0, 0 }, // 147 IIC_SSE_MOVDQ
  { 1, 0, 0, 0, 0 }, // 148 IIC_SSE_MOVD_ToGP
  { 1, 0, 0, 0, 0 }, // 149 IIC_SSE_MOVMSK
  { 1, 0, 0, 0, 0 }, // 150 IIC_SSE_MOVNT
  { 1, 0, 0, 0, 0 }, // 151 IIC_SSE_MOVQ_RR
  { 1, 0, 0, 0, 0 }, // 152 IIC_SSE_MOVU_P_MR
  { 1, 0, 0, 0, 0 }, // 153 IIC_SSE_MOVU_P_RM
  { 1, 0, 0, 0, 0 }, // 154 IIC_SSE_MOVU_P_RR
  { 1, 0, 0, 0, 0 }, // 155 IIC_SSE_MOV_LH
  { 1, 0, 0, 0, 0 }, // 156 IIC_SSE_MOV_S_MR
  { 1, 0, 0, 0, 0 }, // 157 IIC_SSE_MOV_S_RM
  { 1, 0, 0, 0, 0 }, // 158 IIC_SSE_MOV_S_RR
  { 1, 0, 0, 0, 0 }, // 159 IIC_SSE_MUL_F32P_RM
  { 1, 0, 0, 0, 0 }, // 160 IIC_SSE_MUL_F32P_RR
  { 1, 0, 0, 0, 0 }, // 161 IIC_SSE_MUL_F32S_RM
  { 1, 0, 0, 0, 0 }, // 162 IIC_SSE_MUL_F32S_RR
  { 1, 0, 0, 0, 0 }, // 163 IIC_SSE_MUL_F64P_RM
  { 1, 0, 0, 0, 0 }, // 164 IIC_SSE_MUL_F64P_RR
  { 1, 0, 0, 0, 0 }, // 165 IIC_SSE_MUL_F64S_RM
  { 1, 0, 0, 0, 0 }, // 166 IIC_SSE_MUL_F64S_RR
  { 1, 0, 0, 0, 0 }, // 167 IIC_SSE_MWAIT
  { 1, 0, 0, 0, 0 }, // 168 IIC_SSE_PABS_RM
  { 1, 0, 0, 0, 0 }, // 169 IIC_SSE_PABS_RR
  { 1, 0, 0, 0, 0 }, // 170 IIC_SSE_PALIGNR
  { 1, 0, 0, 0, 0 }, // 171 IIC_SSE_PAUSE
  { 1, 0, 0, 0, 0 }, // 172 IIC_SSE_PEXTRW
  { 1, 0, 0, 0, 0 }, // 173 IIC_SSE_PHADDSUBD_RM
  { 1, 0, 0, 0, 0 }, // 174 IIC_SSE_PHADDSUBD_RR
  { 1, 0, 0, 0, 0 }, // 175 IIC_SSE_PHADDSUBSW_RM
  { 1, 0, 0, 0, 0 }, // 176 IIC_SSE_PHADDSUBSW_RR
  { 1, 0, 0, 0, 0 }, // 177 IIC_SSE_PHADDSUBW_RM
  { 1, 0, 0, 0, 0 }, // 178 IIC_SSE_PHADDSUBW_RR
  { 1, 0, 0, 0, 0 }, // 179 IIC_SSE_PINSRW
  { 1, 0, 0, 0, 0 }, // 180 IIC_SSE_PMADD
  { 1, 0, 0, 0, 0 }, // 181 IIC_SSE_PMULHRSW
  { 1, 0, 0, 0, 0 }, // 182 IIC_SSE_PREFETCH
  { 1, 0, 0, 0, 0 }, // 183 IIC_SSE_PSHUF
  { 1, 0, 0, 0, 0 }, // 184 IIC_SSE_PSHUFB_RM
  { 1, 0, 0, 0, 0 }, // 185 IIC_SSE_PSHUFB_RR
  { 1, 0, 0, 0, 0 }, // 186 IIC_SSE_PSIGN_RM
  { 1, 0, 0, 0, 0 }, // 187 IIC_SSE_PSIGN_RR
  { 1, 0, 0, 0, 0 }, // 188 IIC_SSE_RCPP_RM
  { 1, 0, 0, 0, 0 }, // 189 IIC_SSE_RCPP_RR
  { 1, 0, 0, 0, 0 }, // 190 IIC_SSE_RCPS_RM
  { 1, 0, 0, 0, 0 }, // 191 IIC_SSE_RCPS_RR
  { 1, 0, 0, 0, 0 }, // 192 IIC_SSE_SFENCE
  { 1, 0, 0, 0, 0 }, // 193 IIC_SSE_SHUFP
  { 1, 0, 0, 0, 0 }, // 194 IIC_SSE_SQRTP_RM
  { 1, 0, 0, 0, 0 }, // 195 IIC_SSE_SQRTP_RR
  { 1, 0, 0, 0, 0 }, // 196 IIC_SSE_SQRTS_RM
  { 1, 0, 0, 0, 0 }, // 197 IIC_SSE_SQRTS_RR
  { 1, 0, 0, 0, 0 }, // 198 IIC_SSE_STMXCSR
  { 1, 0, 0, 0, 0 }, // 199 IIC_SSE_UNPCK
  { 1, 0, 0, 0, 0 }, // 200 IIC_UNARY_MEM
  { 1, 0, 0, 0, 0 }, // 201 IIC_UNARY_REG
  { 1, 0, 0, 0, 0 }, // 202 IIC_XADD_LOCK_MEM
  { 1, 0, 0, 0, 0 }, // 203 IIC_XADD_LOCK_MEM8
  { 1, 0, 0, 0, 0 }, // 204 NoItinerary
  { 1, ~0U, ~0U, ~0U, ~0U } // end marker
};

// Sorted (by key) array of itineraries for CPU subtype.
extern const llvm::SubtargetInfoKV X86ProcItinKV[] = {
  { "amdfam10", (void *)&GenericItineraries },
  { "athlon", (void *)&GenericItineraries },
  { "athlon-4", (void *)&GenericItineraries },
  { "athlon-fx", (void *)&GenericItineraries },
  { "athlon-mp", (void *)&GenericItineraries },
  { "athlon-tbird", (void *)&GenericItineraries },
  { "athlon-xp", (void *)&GenericItineraries },
  { "athlon64", (void *)&GenericItineraries },
  { "athlon64-sse3", (void *)&GenericItineraries },
  { "atom", (void *)&AtomItineraries },
  { "bdver1", (void *)&GenericItineraries },
  { "bdver2", (void *)&GenericItineraries },
  { "btver1", (void *)&GenericItineraries },
  { "c3", (void *)&GenericItineraries },
  { "c3-2", (void *)&GenericItineraries },
  { "core-avx-i", (void *)&GenericItineraries },
  { "core-avx2", (void *)&GenericItineraries },
  { "core2", (void *)&GenericItineraries },
  { "corei7", (void *)&GenericItineraries },
  { "corei7-avx", (void *)&GenericItineraries },
  { "generic", (void *)&GenericItineraries },
  { "i386", (void *)&GenericItineraries },
  { "i486", (void *)&GenericItineraries },
  { "i586", (void *)&GenericItineraries },
  { "i686", (void *)&GenericItineraries },
  { "k6", (void *)&GenericItineraries },
  { "k6-2", (void *)&GenericItineraries },
  { "k6-3", (void *)&GenericItineraries },
  { "k8", (void *)&GenericItineraries },
  { "k8-sse3", (void *)&GenericItineraries },
  { "nehalem", (void *)&GenericItineraries },
  { "nocona", (void *)&GenericItineraries },
  { "opteron", (void *)&GenericItineraries },
  { "opteron-sse3", (void *)&GenericItineraries },
  { "penryn", (void *)&GenericItineraries },
  { "pentium", (void *)&GenericItineraries },
  { "pentium-m", (void *)&GenericItineraries },
  { "pentium-mmx", (void *)&GenericItineraries },
  { "pentium2", (void *)&GenericItineraries },
  { "pentium3", (void *)&GenericItineraries },
  { "pentium3m", (void *)&GenericItineraries },
  { "pentium4", (void *)&GenericItineraries },
  { "pentium4m", (void *)&GenericItineraries },
  { "pentiumpro", (void *)&GenericItineraries },
  { "prescott", (void *)&GenericItineraries },
  { "westmere", (void *)&GenericItineraries },
  { "winchip-c6", (void *)&GenericItineraries },
  { "winchip2", (void *)&GenericItineraries },
  { "x86-64", (void *)&GenericItineraries },
  { "yonah", (void *)&GenericItineraries }
};

static inline void InitX86MCSubtargetInfo(MCSubtargetInfo *II, StringRef TT, StringRef CPU, StringRef FS) {
  II->InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV, X86ProcItinKV, X86Stages, X86OperandCycles, X86ForwardingPathes, 34, 50);
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
  DEBUG(dbgs() << "\nCPU:" << CPU);
  uint64_t Bits = ReInitMCSubtargetInfo(CPU, FS);
  if ((Bits & X86::Feature3DNow) != 0 && X863DNowLevel < ThreeDNow) X863DNowLevel = ThreeDNow;
  if ((Bits & X86::Feature3DNowA) != 0 && X863DNowLevel < ThreeDNowA) X863DNowLevel = ThreeDNowA;
  if ((Bits & X86::Feature64Bit) != 0) HasX86_64 = true;
  if ((Bits & X86::FeatureAES) != 0) HasAES = true;
  if ((Bits & X86::FeatureAVX) != 0 && X86SSELevel < AVX) X86SSELevel = AVX;
  if ((Bits & X86::FeatureAVX2) != 0 && X86SSELevel < AVX2) X86SSELevel = AVX2;
  if ((Bits & X86::FeatureBMI) != 0) HasBMI = true;
  if ((Bits & X86::FeatureBMI2) != 0) HasBMI2 = true;
  if ((Bits & X86::FeatureCLMUL) != 0) HasCLMUL = true;
  if ((Bits & X86::FeatureCMOV) != 0) HasCMov = true;
  if ((Bits & X86::FeatureCMPXCHG16B) != 0) HasCmpxchg16b = true;
  if ((Bits & X86::FeatureF16C) != 0) HasF16C = true;
  if ((Bits & X86::FeatureFMA3) != 0) HasFMA3 = true;
  if ((Bits & X86::FeatureFMA4) != 0) HasFMA4 = true;
  if ((Bits & X86::FeatureFSGSBase) != 0) HasFSGSBase = true;
  if ((Bits & X86::FeatureFastUAMem) != 0) IsUAMemFast = true;
  if ((Bits & X86::FeatureLZCNT) != 0) HasLZCNT = true;
  if ((Bits & X86::FeatureLeaForSP) != 0) UseLeaForSP = true;
  if ((Bits & X86::FeatureMMX) != 0 && X86SSELevel < MMX) X86SSELevel = MMX;
  if ((Bits & X86::FeatureMOVBE) != 0) HasMOVBE = true;
  if ((Bits & X86::FeaturePOPCNT) != 0) HasPOPCNT = true;
  if ((Bits & X86::FeatureRDRAND) != 0) HasRDRAND = true;
  if ((Bits & X86::FeatureSSE1) != 0 && X86SSELevel < SSE1) X86SSELevel = SSE1;
  if ((Bits & X86::FeatureSSE2) != 0 && X86SSELevel < SSE2) X86SSELevel = SSE2;
  if ((Bits & X86::FeatureSSE3) != 0 && X86SSELevel < SSE3) X86SSELevel = SSE3;
  if ((Bits & X86::FeatureSSE4A) != 0) HasSSE4A = true;
  if ((Bits & X86::FeatureSSE41) != 0 && X86SSELevel < SSE41) X86SSELevel = SSE41;
  if ((Bits & X86::FeatureSSE42) != 0 && X86SSELevel < SSE42) X86SSELevel = SSE42;
  if ((Bits & X86::FeatureSSSE3) != 0 && X86SSELevel < SSSE3) X86SSELevel = SSSE3;
  if ((Bits & X86::FeatureSlowBTMem) != 0) IsBTMemSlow = true;
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
  DFAPacketizer *createDFAPacketizer(const InstrItineraryData *IID) const;
};
} // End llvm namespace 
#endif // GET_SUBTARGETINFO_HEADER


#ifdef GET_SUBTARGETINFO_CTOR
#undef GET_SUBTARGETINFO_CTOR
namespace llvm {
extern const llvm::SubtargetFeatureKV X86FeatureKV[];
extern const llvm::SubtargetFeatureKV X86SubTypeKV[];
extern const llvm::SubtargetInfoKV X86ProcItinKV[];
extern const llvm::InstrStage X86Stages[];
extern const unsigned X86OperandCycles[];
extern const unsigned X86ForwardingPathes[];
X86GenSubtargetInfo::X86GenSubtargetInfo(StringRef TT, StringRef CPU, StringRef FS)
  : TargetSubtargetInfo() {
  InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV, X86ProcItinKV, X86Stages, X86OperandCycles, X86ForwardingPathes, 34, 50);
}

} // End llvm namespace 
#endif // GET_SUBTARGETINFO_CTOR

