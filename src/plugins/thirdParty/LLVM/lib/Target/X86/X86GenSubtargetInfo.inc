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
  const uint64_t FeatureBWI = 1ULL << 10;
  const uint64_t FeatureCDI = 1ULL << 11;
  const uint64_t FeatureCMOV = 1ULL << 12;
  const uint64_t FeatureCMPXCHG16B = 1ULL << 13;
  const uint64_t FeatureCallRegIndirect = 1ULL << 14;
  const uint64_t FeatureDQI = 1ULL << 15;
  const uint64_t FeatureERI = 1ULL << 16;
  const uint64_t FeatureF16C = 1ULL << 17;
  const uint64_t FeatureFMA = 1ULL << 18;
  const uint64_t FeatureFMA4 = 1ULL << 19;
  const uint64_t FeatureFSGSBase = 1ULL << 20;
  const uint64_t FeatureFastUAMem = 1ULL << 21;
  const uint64_t FeatureHLE = 1ULL << 22;
  const uint64_t FeatureLEAUsesAG = 1ULL << 23;
  const uint64_t FeatureLZCNT = 1ULL << 24;
  const uint64_t FeatureLeaForSP = 1ULL << 25;
  const uint64_t FeatureMMX = 1ULL << 26;
  const uint64_t FeatureMOVBE = 1ULL << 27;
  const uint64_t FeaturePCLMUL = 1ULL << 28;
  const uint64_t FeaturePFI = 1ULL << 29;
  const uint64_t FeaturePOPCNT = 1ULL << 30;
  const uint64_t FeaturePRFCHW = 1ULL << 31;
  const uint64_t FeaturePadShortFunctions = 1ULL << 32;
  const uint64_t FeatureRDRAND = 1ULL << 33;
  const uint64_t FeatureRDSEED = 1ULL << 34;
  const uint64_t FeatureRTM = 1ULL << 35;
  const uint64_t FeatureSHA = 1ULL << 36;
  const uint64_t FeatureSSE1 = 1ULL << 37;
  const uint64_t FeatureSSE2 = 1ULL << 38;
  const uint64_t FeatureSSE3 = 1ULL << 39;
  const uint64_t FeatureSSE4A = 1ULL << 40;
  const uint64_t FeatureSSE41 = 1ULL << 41;
  const uint64_t FeatureSSE42 = 1ULL << 42;
  const uint64_t FeatureSSSE3 = 1ULL << 43;
  const uint64_t FeatureSlowBTMem = 1ULL << 44;
  const uint64_t FeatureSlowDivide = 1ULL << 45;
  const uint64_t FeatureSlowIncDec = 1ULL << 46;
  const uint64_t FeatureSlowLEA = 1ULL << 47;
  const uint64_t FeatureSlowSHLD = 1ULL << 48;
  const uint64_t FeatureTBM = 1ULL << 49;
  const uint64_t FeatureVLX = 1ULL << 50;
  const uint64_t FeatureVectorUAMem = 1ULL << 51;
  const uint64_t FeatureXOP = 1ULL << 52;
  const uint64_t Mode16Bit = 1ULL << 53;
  const uint64_t Mode32Bit = 1ULL << 54;
  const uint64_t Mode64Bit = 1ULL << 55;
  const uint64_t ProcIntelAtom = 1ULL << 56;
  const uint64_t ProcIntelSLM = 1ULL << 57;
}
} // End llvm namespace
#endif // GET_SUBTARGETINFO_ENUM


#ifdef GET_SUBTARGETINFO_MC_DESC
#undef GET_SUBTARGETINFO_MC_DESC
namespace llvm {
// Sorted (by key) array of values for CPU features.
extern const llvm::SubtargetFeatureKV X86FeatureKV[] = {
  { "16bit-mode", "16-bit mode (i8086)", X86::Mode16Bit, 0ULL },
  { "32bit-mode", "32-bit mode (80386)", X86::Mode32Bit, 0ULL },
  { "3dnow", "Enable 3DNow! instructions", X86::Feature3DNow, X86::FeatureMMX },
  { "3dnowa", "Enable 3DNow! Athlon instructions", X86::Feature3DNowA, X86::Feature3DNow },
  { "64bit", "Support 64-bit instructions", X86::Feature64Bit, X86::FeatureCMOV },
  { "64bit-mode", "64-bit mode (x86_64)", X86::Mode64Bit, 0ULL },
  { "adx", "Support ADX instructions", X86::FeatureADX, 0ULL },
  { "aes", "Enable AES instructions", X86::FeatureAES, X86::FeatureSSE2 },
  { "atom", "Intel Atom processors", X86::ProcIntelAtom, 0ULL },
  { "avx", "Enable AVX instructions", X86::FeatureAVX, X86::FeatureSSE42 },
  { "avx2", "Enable AVX2 instructions", X86::FeatureAVX2, X86::FeatureAVX },
  { "avx512bw", "Enable AVX-512 Byte and Word Instructions", X86::FeatureBWI, X86::FeatureAVX512 },
  { "avx512cd", "Enable AVX-512 Conflict Detection Instructions", X86::FeatureCDI, X86::FeatureAVX512 },
  { "avx512dq", "Enable AVX-512 Doubleword and Quadword Instructions", X86::FeatureDQI, X86::FeatureAVX512 },
  { "avx512er", "Enable AVX-512 Exponential and Reciprocal Instructions", X86::FeatureERI, X86::FeatureAVX512 },
  { "avx512f", "Enable AVX-512 instructions", X86::FeatureAVX512, X86::FeatureAVX2 },
  { "avx512pf", "Enable AVX-512 PreFetch Instructions", X86::FeaturePFI, X86::FeatureAVX512 },
  { "avx512vl", "Enable AVX-512 Vector Length eXtensions", X86::FeatureVLX, X86::FeatureAVX512 },
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
  { "slow-incdec", "INC and DEC instructions are slower than ADD and SUB", X86::FeatureSlowIncDec, 0ULL },
  { "slow-lea", "LEA instruction with certain arguments is slow", X86::FeatureSlowLEA, 0ULL },
  { "slow-shld", "SHLD instruction is slow", X86::FeatureSlowSHLD, 0ULL },
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
  { "amdfam10", "Select the amdfam10 processor", X86::FeatureSSE4A | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon", "Select the athlon processor", X86::Feature3DNowA | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon-4", "Select the athlon-4 processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon-fx", "Select the athlon-fx processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon-mp", "Select the athlon-mp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon-tbird", "Select the athlon-tbird processor", X86::Feature3DNowA | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon-xp", "Select the athlon-xp processor", X86::FeatureSSE1 | X86::Feature3DNowA | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon64", "Select the athlon64 processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "athlon64-sse3", "Select the athlon64-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "atom", "Select the atom processor", X86::ProcIntelAtom | X86::FeatureSSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeatureSlowBTMem | X86::FeatureLeaForSP | X86::FeatureSlowDivide | X86::FeatureCallRegIndirect | X86::FeatureLEAUsesAG | X86::FeaturePadShortFunctions, 0ULL },
  { "bdver1", "Select the bdver1 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowSHLD, 0ULL },
  { "bdver2", "Select the bdver2 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureTBM | X86::FeatureFMA | X86::FeatureSlowSHLD, 0ULL },
  { "bdver3", "Select the bdver3 processor", X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureTBM | X86::FeatureFMA | X86::FeatureFSGSBase, 0ULL },
  { "bdver4", "Select the bdver4 processor", X86::FeatureAVX2 | X86::FeatureXOP | X86::FeatureFMA4 | X86::FeatureCMPXCHG16B | X86::FeatureAES | X86::FeaturePRFCHW | X86::FeaturePCLMUL | X86::FeatureF16C | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureTBM | X86::FeatureFMA | X86::FeatureFSGSBase, 0ULL },
  { "btver1", "Select the btver1 processor", X86::FeatureSSSE3 | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeaturePRFCHW | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowSHLD, 0ULL },
  { "btver2", "Select the btver2 processor", X86::FeatureAVX | X86::FeatureSSE4A | X86::FeatureCMPXCHG16B | X86::FeaturePRFCHW | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureBMI | X86::FeatureF16C | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeaturePOPCNT | X86::FeatureSlowSHLD, 0ULL },
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
  { "k8", "Select the k8 processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "k8-sse3", "Select the k8-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "knl", "Select the knl processor", X86::FeatureAVX512 | X86::FeatureERI | X86::FeatureCDI | X86::FeaturePFI | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureFMA | X86::FeatureRTM | X86::FeatureHLE | X86::FeatureSlowIncDec, 0ULL },
  { "nehalem", "Select the nehalem processor", X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureFastUAMem | X86::FeaturePOPCNT, 0ULL },
  { "nocona", "Select the nocona processor", X86::FeatureSSE3 | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem, 0ULL },
  { "opteron", "Select the opteron processor", X86::FeatureSSE2 | X86::Feature3DNowA | X86::Feature64Bit | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
  { "opteron-sse3", "Select the opteron-sse3 processor", X86::FeatureSSE3 | X86::Feature3DNowA | X86::FeatureCMPXCHG16B | X86::FeatureSlowBTMem | X86::FeatureSlowSHLD, 0ULL },
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
  { "skx", "Select the skx processor", X86::FeatureAVX512 | X86::FeatureCDI | X86::FeatureDQI | X86::FeatureBWI | X86::FeatureVLX | X86::FeatureCMPXCHG16B | X86::FeatureFastUAMem | X86::FeaturePOPCNT | X86::FeatureAES | X86::FeaturePCLMUL | X86::FeatureRDRAND | X86::FeatureF16C | X86::FeatureFSGSBase | X86::FeatureMOVBE | X86::FeatureLZCNT | X86::FeatureBMI | X86::FeatureBMI2 | X86::FeatureFMA | X86::FeatureRTM | X86::FeatureHLE | X86::FeatureSlowIncDec, 0ULL },
  { "slm", "Select the slm processor", X86::ProcIntelSLM | X86::FeatureSSE42 | X86::FeatureCMPXCHG16B | X86::FeatureMOVBE | X86::FeaturePOPCNT | X86::FeaturePCLMUL | X86::FeatureAES | X86::FeatureCallRegIndirect | X86::FeaturePRFCHW | X86::FeatureSlowLEA | X86::FeatureSlowIncDec | X86::FeatureSlowBTMem | X86::FeatureFastUAMem, 0ULL },
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

static const llvm::InstrItinerary *NoItineraries = nullptr;

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
  { 0, 0, 0, 0, 0 }, // 23 WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 24 WriteFAdd
  { 0, 0, 0, 0, 0 }, // 25 IIC_AES_WriteAESDecEncLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 26 IIC_AES_WriteAESDecEnc
  { 0, 0, 0, 0, 0 }, // 27 IIC_AES_WriteAESIMCLd
  { 0, 0, 0, 0, 0 }, // 28 IIC_AES_WriteAESIMC
  { 0, 0, 0, 0, 0 }, // 29 WriteAESKeyGenLd
  { 0, 0, 0, 0, 0 }, // 30 WriteAESKeyGen
  { 0, 0, 0, 0, 0 }, // 31 WriteVecLogicLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 32 WriteVecLogic
  { 1, 10, 11, 0, 0 }, // 33 IIC_ARPL_MEM_WriteSystem
  { 1, 11, 12, 0, 0 }, // 34 IIC_ARPL_REG_WriteSystem
  { 0, 0, 0, 0, 0 }, // 35 WriteZero
  { 1, 5, 6, 0, 0 }, // 36 IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 37 IIC_SSE_INTALU_P_RR_WriteFBlend
  { 1, 5, 6, 0, 0 }, // 38 IIC_ALU_MEM
  { 1, 4, 5, 0, 0 }, // 39 IIC_ALU_NONMEM
  { 1, 12, 13, 0, 0 }, // 40 IIC_BOUND_WriteSystem
  { 1, 13, 14, 0, 0 }, // 41 IIC_BIT_SCAN_MEM_WriteShiftLd
  { 1, 13, 14, 0, 0 }, // 42 IIC_BIT_SCAN_REG_WriteShift
  { 1, 5, 6, 0, 0 }, // 43 IIC_BSWAP_WriteALU
  { 1, 4, 5, 0, 0 }, // 44 IIC_BT_MI_WriteALU
  { 1, 14, 15, 0, 0 }, // 45 IIC_BT_MR_WriteALULd
  { 1, 15, 16, 0, 0 }, // 46 IIC_BT_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 47 IIC_BT_RR_WriteALU
  { 1, 16, 17, 0, 0 }, // 48 IIC_BTX_MI_WriteALULd_WriteRMW
  { 1, 12, 13, 0, 0 }, // 49 IIC_BTX_MR_WriteALULd_WriteRMW
  { 1, 15, 16, 0, 0 }, // 50 IIC_BTX_RI_WriteALU
  { 1, 15, 16, 0, 0 }, // 51 IIC_BTX_RR_WriteALU
  { 1, 17, 18, 0, 0 }, // 52 IIC_CALL_MEM_WriteJumpLd
  { 1, 18, 20, 0, 0 }, // 53 IIC_CALL_RI_WriteJump
  { 1, 17, 18, 0, 0 }, // 54 IIC_CALL_MEM_WriteJump
  { 1, 20, 21, 0, 0 }, // 55 IIC_CBW
  { 1, 4, 5, 0, 0 }, // 56 IIC_CLC_WriteALU
  { 1, 21, 22, 0, 0 }, // 57 IIC_CLD_WriteALU
  { 1, 5, 6, 0, 0 }, // 58 IIC_SSE_PREFETCH_WriteLoad
  { 1, 22, 23, 0, 0 }, // 59 IIC_CLI_WriteALU
  { 1, 23, 24, 0, 0 }, // 60 IIC_CLTS_WriteALU
  { 1, 4, 5, 0, 0 }, // 61 IIC_CMC_WriteALU
  { 1, 5, 6, 0, 0 }, // 62 IIC_CMOV16_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 63 IIC_CMOV16_RR_WriteALU
  { 1, 5, 6, 0, 0 }, // 64 IIC_CMOV32_RM_WriteALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 65 IIC_CMOV32_RR_WriteALU
  { 1, 21, 22, 0, 0 }, // 66 IIC_CMPS_WriteMicrocoded
  { 1, 24, 25, 0, 0 }, // 67 IIC_CMPXCHG_16B_WriteALULd_WriteRMW
  { 1, 22, 23, 0, 0 }, // 68 IIC_CMPXCHG_MEM_WriteALULd_WriteRMW
  { 1, 17, 18, 0, 0 }, // 69 IIC_CMPXCHG_REG_WriteALU
  { 1, 25, 26, 0, 0 }, // 70 IIC_CMPXCHG_8B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 71 IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 72 IIC_CMPXCHG_REG8_WriteALU
  { 1, 26, 27, 0, 0 }, // 73 IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 74 IIC_SSE_COMIS_RR_WriteFAdd
  { 1, 14, 15, 0, 0 }, // 75 IIC_FCOMI_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 76 IIC_CPUID_WriteSystem
  { 0, 0, 0, 0, 0 }, // 77 IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 78 IIC_CRC32_REG_WriteFAdd
  { 1, 2, 3, 0, 0 }, // 79 IIC_SSE_CVT_PD_RR_WriteCvtI2FLd
  { 1, 28, 29, 0, 0 }, // 80 IIC_SSE_CVT_PD_RM_WriteCvtI2F
  { 1, 2, 3, 0, 0 }, // 81 IIC_SSE_CVT_PS_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 82 IIC_SSE_CVT_PS_RR_WriteCvtI2F
  { 1, 28, 29, 0, 0 }, // 83 IIC_SSE_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 84 IIC_SSE_CVT_PD_RR_WriteCvtF2I
  { 1, 28, 29, 0, 0 }, // 85 IIC_SSE_CVT_PD_RM_WriteCvtF2FLd
  { 1, 2, 3, 0, 0 }, // 86 IIC_SSE_CVT_PD_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 87 IIC_SSE_CVT_PS_RM_WriteCvtF2ILd
  { 1, 6, 7, 0, 0 }, // 88 IIC_SSE_CVT_PS_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 89 IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 90 IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I
  { 1, 2, 3, 0, 0 }, // 91 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd
  { 1, 6, 7, 0, 0 }, // 92 IIC_SSE_CVT_Scalar_RR_WriteCvtF2F
  { 1, 2, 3, 0, 0 }, // 93 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd
  { 1, 6, 7, 0, 0 }, // 94 IIC_SSE_CVT_Scalar_RR_WriteCvtI2F
  { 1, 26, 27, 0, 0 }, // 95 IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd
  { 1, 14, 15, 0, 0 }, // 96 IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I
  { 1, 14, 15, 0, 0 }, // 97 IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd
  { 1, 28, 29, 0, 0 }, // 98 IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I
  { 1, 25, 26, 0, 0 }, // 99 IIC_DAA_WriteMicrocoded
  { 1, 29, 30, 0, 0 }, // 100 IIC_DAS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 101 IIC_UNARY_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 102 IIC_UNARY_REG_WriteALU
  { 1, 30, 31, 0, 0 }, // 103 IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 104 IIC_DIV16_WriteIDiv
  { 1, 30, 31, 0, 0 }, // 105 IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 106 IIC_DIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 107 IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 108 IIC_DIV64_WriteIDiv
  { 1, 32, 33, 0, 0 }, // 109 IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 30, 31, 0, 0 }, // 110 IIC_DIV8_REG_WriteIDiv
  { 1, 33, 34, 0, 0 }, // 111 IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd
  { 1, 33, 34, 0, 0 }, // 112 IIC_SSE_DIV_F64P_RR_WriteFDiv
  { 1, 34, 35, 0, 0 }, // 113 IIC_SSE_DIV_F32P_RR_WriteFDiv
  { 0, 0, 0, 0, 0 }, // 114 WriteFDivLd
  { 0, 0, 0, 0, 0 }, // 115 WriteFDiv
  { 1, 35, 36, 0, 0 }, // 116 IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 117 IIC_SSE_DIV_F64S_RR_WriteFDiv
  { 1, 36, 37, 0, 0 }, // 118 IIC_SSE_DIV_F32S_RR_WriteFDiv
  { 0, 0, 0, 0, 0 }, // 119 IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 120 IIC_SSE_DPPD_RR_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 121 IIC_SSE_DPPS_RR_WriteFAdd
  { 1, 37, 38, 0, 0 }, // 122 IIC_RET_WriteSystem
  { 0, 0, 0, 0, 0 }, // 123 WriteSystem
  { 1, 38, 39, 0, 0 }, // 124 IIC_ENTER_WriteMicrocoded
  { 0, 0, 0, 0, 0 }, // 125 IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 126 IIC_SSE_EXTRACTPS_RR_WriteFBlend
  { 1, 39, 40, 0, 0 }, // 127 IIC_F2XM1_WriteMicrocoded
  { 1, 40, 41, 0, 0 }, // 128 IIC_CALL_FAR_PTR_WriteJump
  { 1, 41, 42, 0, 0 }, // 129 IIC_CALL_FAR_MEM_WriteJumpLd
  { 1, 41, 42, 0, 0 }, // 130 IIC_CALL_FAR_MEM_WriteJump
  { 1, 42, 43, 0, 0 }, // 131 IIC_JMP_FAR_PTR_WriteJump
  { 1, 38, 39, 0, 0 }, // 132 IIC_JMP_FAR_MEM_WriteJumpLd
  { 1, 38, 39, 0, 0 }, // 133 IIC_JMP_FAR_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 134 IIC_FCOMPP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 135 IIC_FPSTP_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 136 IIC_FFREE_WriteMicrocoded
  { 1, 43, 44, 0, 0 }, // 137 IIC_FLDCW_WriteLoad
  { 1, 26, 27, 0, 0 }, // 138 IIC_FLDL_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 139 IIC_FNCLEX_WriteMicrocoded
  { 1, 45, 46, 0, 0 }, // 140 IIC_FNINIT_WriteMicrocoded
  { 1, 4, 5, 0, 0 }, // 141 IIC_FNOP_WriteMicrocoded
  { 1, 28, 29, 0, 0 }, // 142 IIC_FNSTCW_WriteALU
  { 1, 26, 27, 0, 0 }, // 143 IIC_FNSTSW_WriteALU
  { 1, 46, 47, 0, 0 }, // 144 IIC_FPATAN_WriteMicrocoded
  { 1, 47, 48, 0, 0 }, // 145 IIC_FPREM_WriteMicrocoded
  { 1, 48, 49, 0, 0 }, // 146 IIC_FPREM1_WriteMicrocoded
  { 1, 49, 50, 0, 0 }, // 147 IIC_FPTAN_WriteMicrocoded
  { 1, 50, 51, 0, 0 }, // 148 IIC_FRNDINT_WriteMicrocoded
  { 1, 51, 52, 0, 0 }, // 149 IIC_FSCALE_WriteMicrocoded
  { 1, 52, 53, 0, 0 }, // 150 IIC_FSINCOS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 151 IIC_FXAM_WriteMicrocoded
  { 1, 53, 54, 0, 0 }, // 152 IIC_FXRSTOR_WriteMicrocoded
  { 1, 54, 55, 0, 0 }, // 153 IIC_FXSAVE_WriteMicrocoded
  { 1, 44, 45, 0, 0 }, // 154 IIC_FXTRACT_WriteMicrocoded
  { 1, 55, 56, 0, 0 }, // 155 IIC_FYL2X_WriteMicrocoded
  { 1, 56, 57, 0, 0 }, // 156 IIC_FYL2XP1_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 157 IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 158 IIC_SSE_BIT_P_RR_WriteFAdd
  { 1, 5, 6, 0, 0 }, // 159 IIC_SSE_MOVA_P_RM_WriteLoad
  { 1, 14, 15, 0, 0 }, // 160 IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd
  { 1, 28, 29, 0, 0 }, // 161 IIC_SSE_HADDSUB_RR_WriteFAdd
  { 1, 27, 28, 0, 0 }, // 162 IIC_HLT_WriteSystem
  { 1, 35, 36, 0, 0 }, // 163 IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 164 IIC_IDIV16_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 165 IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 166 IIC_IDIV32_WriteIDiv
  { 1, 31, 32, 0, 0 }, // 167 IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 31, 32, 0, 0 }, // 168 IIC_IDIV64_WriteIDiv
  { 1, 35, 36, 0, 0 }, // 169 IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 35, 36, 0, 0 }, // 170 IIC_IDIV8_WriteIDiv
  { 1, 7, 9, 0, 0 }, // 171 IIC_FILD_WriteLoad
  { 1, 28, 29, 0, 0 }, // 172 IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 173 IIC_IMUL16_RR_WriteIMul
  { 1, 2, 3, 0, 0 }, // 174 IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 175 IIC_IMUL16_RMI_WriteIMulLd
  { 1, 6, 7, 0, 0 }, // 176 IIC_IMUL16_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 177 IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 178 IIC_IMUL32_RR_WriteIMul
  { 1, 57, 58, 0, 0 }, // 179 IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 180 IIC_IMUL32_RMI_WriteIMulLd
  { 1, 57, 58, 0, 0 }, // 181 IIC_IMUL32_RRI_WriteIMul
  { 1, 58, 59, 0, 0 }, // 182 IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 183 IIC_IMUL64_RR_WriteIMul
  { 1, 58, 59, 0, 0 }, // 184 IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd
  { 1, 22, 23, 0, 0 }, // 185 IIC_IMUL64_RMI_WriteIMulLd
  { 1, 22, 23, 0, 0 }, // 186 IIC_IMUL64_RRI_WriteIMul
  { 1, 2, 3, 0, 0 }, // 187 IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 188 IIC_IMUL8_WriteIMul
  { 1, 59, 60, 0, 0 }, // 189 IIC_IN_RI_WriteSystem
  { 1, 60, 61, 0, 0 }, // 190 IIC_IN_RR_WriteSystem
  { 1, 61, 62, 0, 0 }, // 191 IIC_INS_WriteSystem
  { 0, 0, 0, 0, 0 }, // 192 IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 193 IIC_SSE_INSERTPS_RR_WriteFShuffle
  { 1, 62, 63, 0, 0 }, // 194 IIC_INT_WriteSystem
  { 1, 31, 32, 0, 0 }, // 195 IIC_INT3_WriteSystem
  { 1, 63, 64, 0, 0 }, // 196 IIC_INVD_WriteSystem
  { 1, 48, 49, 0, 0 }, // 197 IIC_INVLPG_WriteSystem
  { 1, 64, 65, 0, 0 }, // 198 IIC_IRET_WriteSystem
  { 1, 16, 17, 0, 0 }, // 199 IIC_FST_WriteStore
  { 1, 6, 7, 0, 0 }, // 200 IIC_FIST_WriteStore
  { 1, 2, 3, 0, 0 }, // 201 IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 202 IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 203 WriteLoad
  { 1, 15, 16, 0, 0 }, // 204 IIC_Jcc_WriteJump
  { 1, 20, 21, 0, 0 }, // 205 IIC_JCXZ_WriteJump
  { 1, 16, 17, 0, 0 }, // 206 IIC_JMP_MEM_WriteJumpLd
  { 1, 15, 16, 0, 0 }, // 207 IIC_JMP_REG_WriteJump
  { 1, 15, 16, 0, 0 }, // 208 IIC_JMP_REL_WriteJump
  { 1, 4, 5, 0, 0 }, // 209 IIC_AHF_WriteALU
  { 1, 30, 31, 0, 0 }, // 210 IIC_LAR_RM_WriteSystem
  { 1, 65, 66, 0, 0 }, // 211 IIC_LAR_RR_WriteSystem
  { 1, 22, 23, 0, 0 }, // 212 IIC_CMPX_LOCK_WriteALULd_WriteRMW
  { 1, 24, 25, 0, 0 }, // 213 IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 214 IIC_CMPX_LOCK_8_WriteALULd_WriteRMW
  { 1, 25, 26, 0, 0 }, // 215 IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW
  { 1, 21, 22, 0, 0 }, // 216 IIC_SSE_LDDQU_WriteLoad
  { 1, 43, 44, 0, 0 }, // 217 IIC_SSE_LDMXCSR_WriteLoad
  { 1, 26, 27, 0, 0 }, // 218 IIC_LXS_WriteSystem
  { 1, 4, 5, 0, 0 }, // 219 IIC_FLDZ_WriteZero
  { 1, 6, 7, 0, 0 }, // 220 IIC_FIST_WriteZero
  { 1, 5, 6, 0, 0 }, // 221 IIC_FLD_WriteLoad
  { 1, 20, 21, 0, 0 }, // 222 IIC_FLD80_WriteLoad
  { 1, 5, 6, 0, 0 }, // 223 IIC_FLD_WriteMove
  { 1, 16, 17, 0, 0 }, // 224 IIC_LEA_16_WriteLEA
  { 1, 15, 16, 0, 0 }, // 225 IIC_LEA_WriteLEA
  { 1, 16, 17, 0, 0 }, // 226 IIC_LEAVE_WriteALU
  { 1, 4, 5, 0, 0 }, // 227 IIC_SSE_LFENCE_WriteFence
  { 1, 66, 67, 0, 0 }, // 228 IIC_LGDT_WriteSystem
  { 1, 66, 67, 0, 0 }, // 229 IIC_LIDT_WriteSystem
  { 1, 67, 68, 0, 0 }, // 230 IIC_LLDT_MEM_WriteSystem
  { 1, 68, 69, 0, 0 }, // 231 IIC_LLDT_REG_WriteSystem
  { 1, 69, 70, 0, 0 }, // 232 IIC_LMSW_REG_WriteSystem
  { 1, 70, 71, 0, 0 }, // 233 IIC_LMSW_MEM_WriteSystem
  { 1, 5, 6, 0, 0 }, // 234 IIC_ALU_MEM_WriteALULd_WriteRMW
  { 1, 4, 5, 0, 0 }, // 235 IIC_ALU_NONMEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 236 IIC_LODS_WriteMicrocoded
  { 1, 25, 26, 0, 0 }, // 237 IIC_LOOP_WriteJump
  { 1, 28, 29, 0, 0 }, // 238 IIC_LOOPE_WriteJump
  { 1, 71, 72, 0, 0 }, // 239 IIC_LOOPNE_WriteJump
  { 1, 37, 38, 0, 0 }, // 240 IIC_RET_WriteJumpLd
  { 1, 50, 51, 0, 0 }, // 241 IIC_LSL_RM_WriteSystem
  { 1, 72, 73, 0, 0 }, // 242 IIC_LSL_RR_WriteSystem
  { 1, 73, 74, 0, 0 }, // 243 IIC_LTR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 244 IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 245 IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 246 IIC_SSE_MASKMOV_WriteStore
  { 1, 5, 6, 0, 0 }, // 247 IIC_SSE_MFENCE_WriteFence
  { 1, 28, 29, 0, 0 }, // 248 IIC_MMX_CVT_PD_RM_WriteCvtF2ILd
  { 1, 2, 3, 0, 0 }, // 249 IIC_MMX_CVT_PD_RR_WriteCvtF2I
  { 1, 7, 9, 0, 0 }, // 250 IIC_MMX_CVT_PS_RM_WriteCvtF2ILd
  { 1, 9, 10, 0, 0 }, // 251 IIC_MMX_CVT_PS_RR_WriteCvtF2I
  { 1, 43, 44, 0, 0 }, // 252 IIC_MMX_EMMS
  { 1, 5, 6, 0, 0 }, // 253 IIC_MMX_MASKMOV_WriteShuffle
  { 1, 74, 75, 0, 0 }, // 254 IIC_MMX_MOV_REG_MM_WriteMove
  { 1, 5, 6, 0, 0 }, // 255 IIC_MMX_MOV_MM_RM_WriteStore
  { 1, 5, 6, 0, 0 }, // 256 IIC_MMX_MOV_MM_RM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 257 IIC_MMX_MOV_MM_RM_WriteMove
  { 1, 4, 5, 0, 0 }, // 258 IIC_MMX_MOVQ_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 259 IIC_MMX_MOVQ_RM_WriteStore
  { 0, 0, 0, 0, 0 }, // 260 WriteMove
  { 1, 5, 6, 0, 0 }, // 261 IIC_MMX_MOVQ_RM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 262 IIC_MMX_ALU_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 263 IIC_MMX_ALU_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 264 IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 265 IIC_MMX_PCK_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 266 IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 267 IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 268 IIC_MMX_ALUQ_RR_WriteVecALU
  { 1, 4, 5, 0, 0 }, // 269 IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 270 IIC_MMX_MISC_FUNC_MEM_WriteVecALU
  { 1, 20, 21, 0, 0 }, // 271 IIC_MMX_PEXTR_WriteShuffle
  { 1, 6, 7, 0, 0 }, // 272 IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 43, 44, 0, 0 }, // 273 IIC_MMX_PHADDSUBW_RR_WriteVecALU
  { 1, 20, 21, 0, 0 }, // 274 IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 275 IIC_MMX_PHADDSUBD_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 276 IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 277 IIC_MMX_PINSRW_WriteShuffle
  { 1, 75, 76, 0, 0 }, // 278 IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd
  { 1, 75, 76, 0, 0 }, // 279 IIC_MMX_PMUL_WriteVecIMul
  { 1, 20, 21, 0, 0 }, // 280 IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 281 IIC_MMX_PSADBW_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 282 IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 283 IIC_MMX_PSHUF_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 284 IIC_MMX_PSHUF_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 285 IIC_MMX_SHIFT_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 286 IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 287 IIC_MMX_SHIFT_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 288 IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 289 IIC_MMX_UNPCK_H_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 290 IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 291 IIC_MMX_UNPCK_L_WriteShuffle
  { 1, 76, 77, 0, 0 }, // 292 IIC_SSE_MONITOR_WriteSystem
  { 1, 5, 6, 0, 0 }, // 293 IIC_MOV_MEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 294 IIC_MOV_MEM_WriteStore
  { 1, 16, 17, 0, 0 }, // 295 IIC_MOV_MEM_SR_WriteMove
  { 1, 4, 5, 0, 0 }, // 296 IIC_MOV_WriteMove
  { 1, 5, 6, 0, 0 }, // 297 IIC_MOV_MEM_WriteLoad
  { 1, 5, 6, 0, 0 }, // 298 IIC_MOV_REG_SR_WriteMove
  { 1, 77, 78, 0, 0 }, // 299 IIC_MOV_SR_MEM_WriteMove
  { 1, 3, 4, 0, 0 }, // 300 IIC_MOV_SR_REG_WriteMove
  { 1, 78, 79, 0, 0 }, // 301 IIC_MOV_CR_REG_WriteSystem
  { 1, 79, 80, 0, 0 }, // 302 IIC_MOV_DR_REG_WriteSystem
  { 1, 4, 5, 0, 0 }, // 303 IIC_ALU_NONMEM_WriteZero
  { 1, 58, 59, 0, 0 }, // 304 IIC_MOV_REG_CR_WriteSystem
  { 1, 80, 81, 0, 0 }, // 305 IIC_MOV_REG_DR_WriteSystem
  { 1, 4, 5, 0, 0 }, // 306 IIC_ALU_NONMEM_WriteALU
  { 1, 5, 6, 0, 0 }, // 307 IIC_SSE_MOVDQ_WriteMove
  { 1, 5, 6, 0, 0 }, // 308 IIC_SSE_MOVDQ_WriteLoad
  { 1, 5, 6, 0, 0 }, // 309 IIC_SSE_MOVA_P_MR_WriteStore
  { 1, 4, 5, 0, 0 }, // 310 IIC_SSE_MOVA_P_RR_WriteFShuffle
  { 1, 4, 5, 0, 0 }, // 311 IIC_SSE_MOVA_P_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 312 IIC_MOVBE_WriteStore
  { 1, 5, 6, 0, 0 }, // 313 IIC_MOVBE_WriteALULd
  { 1, 5, 6, 0, 0 }, // 314 IIC_SSE_MOV_LH_WriteLoad
  { 1, 5, 6, 0, 0 }, // 315 IIC_SSE_MOV_LH_WriteFShuffle
  { 1, 16, 17, 0, 0 }, // 316 IIC_SSE_MOVU_P_MR_WriteStore
  { 1, 21, 22, 0, 0 }, // 317 IIC_SSE_MOVU_P_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 318 IIC_SSE_MOVU_P_RR_WriteMove
  { 1, 5, 6, 0, 0 }, // 319 IIC_SSE_MOV_LH_WriteStore
  { 1, 5, 6, 0, 0 }, // 320 IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd
  { 1, 74, 75, 0, 0 }, // 321 IIC_SSE_MOVMSK_WriteVecLogic
  { 1, 5, 6, 0, 0 }, // 322 IIC_SSE_MOVNT_WriteStore
  { 1, 5, 6, 0, 0 }, // 323 IIC_SSE_MOVDQ_WriteStore
  { 1, 74, 75, 0, 0 }, // 324 IIC_SSE_MOVD_ToGP_WriteMove
  { 1, 4, 5, 0, 0 }, // 325 IIC_SSE_MOVQ_RR_WriteVecLogic
  { 1, 21, 22, 0, 0 }, // 326 IIC_MOVS_WriteMicrocoded
  { 1, 5, 6, 0, 0 }, // 327 IIC_SSE_MOV_S_MR_WriteStore
  { 1, 5, 6, 0, 0 }, // 328 IIC_SSE_MOV_S_RM_WriteLoad
  { 1, 4, 5, 0, 0 }, // 329 IIC_SSE_MOV_S_RR_WriteFShuffle
  { 1, 21, 22, 0, 0 }, // 330 IIC_MOVSX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 331 IIC_MOVSX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 332 IIC_MOVSX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 333 IIC_MOVSX_WriteALU
  { 1, 4, 5, 0, 0 }, // 334 IIC_SSE_MOVU_P_RR_WriteFShuffle
  { 1, 5, 6, 0, 0 }, // 335 IIC_SSE_MOVDQ_WriteVecLogicLd
  { 1, 21, 22, 0, 0 }, // 336 IIC_MOVZX_R16_M8_WriteALULd
  { 1, 16, 17, 0, 0 }, // 337 IIC_MOVZX_R16_R8_WriteALU
  { 1, 5, 6, 0, 0 }, // 338 IIC_MOVZX_WriteALULd
  { 1, 5, 6, 0, 0 }, // 339 IIC_MOVZX_WriteALU
  { 0, 0, 0, 0, 0 }, // 340 IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 341 IIC_SSE_MPSADBW_RR_WriteMPSAD
  { 1, 28, 29, 0, 0 }, // 342 IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 343 IIC_MUL16_REG_WriteIMul
  { 1, 2, 3, 0, 0 }, // 344 IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 6, 7, 0, 0 }, // 345 IIC_MUL32_REG_WriteIMul
  { 1, 58, 59, 0, 0 }, // 346 IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 58, 59, 0, 0 }, // 347 IIC_MUL64_WriteIMul
  { 1, 2, 3, 0, 0 }, // 348 IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 349 IIC_MUL8_WriteIMul
  { 1, 26, 27, 0, 0 }, // 350 IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd
  { 1, 14, 15, 0, 0 }, // 351 IIC_SSE_MUL_F64P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 352 IIC_SSE_MUL_F32P_RR_WriteFMul
  { 1, 57, 58, 0, 0 }, // 353 IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 354 IIC_SSE_MUL_F64S_RR_WriteFMul
  { 1, 75, 76, 0, 0 }, // 355 IIC_SSE_MUL_F32S_RR_WriteFMul
  { 1, 2, 3, 0, 0 }, // 356 IIC_MUL8_WriteIMulLd_WriteIMulH
  { 1, 2, 3, 0, 0 }, // 357 IIC_MUL8_WriteIMul_WriteIMulH
  { 0, 0, 0, 0, 0 }, // 358 WriteFMulLd
  { 0, 0, 0, 0, 0 }, // 359 WriteFMul
  { 1, 50, 51, 0, 0 }, // 360 IIC_SSE_MWAIT_WriteSystem
  { 1, 4, 5, 0, 0 }, // 361 IIC_NOP_WriteZero
  { 1, 81, 82, 0, 0 }, // 362 IIC_OUT_IR_WriteSystem
  { 1, 32, 33, 0, 0 }, // 363 IIC_OUT_RR_WriteSystem
  { 1, 82, 83, 0, 0 }, // 364 IIC_OUTS_WriteSystem
  { 1, 5, 6, 0, 0 }, // 365 IIC_SSE_PABS_RM_WriteVecALULd
  { 1, 4, 5, 0, 0 }, // 366 IIC_SSE_PABS_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 367 WriteShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 368 WriteShuffle
  { 1, 5, 6, 0, 0 }, // 369 IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 370 IIC_SSE_INTALU_P_RR_WriteVecALU
  { 1, 21, 22, 0, 0 }, // 371 IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 372 IIC_SSE_INTALUQ_P_RR_WriteVecALU
  { 1, 5, 6, 0, 0 }, // 373 IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 374 IIC_SSE_PALIGNRR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 375 IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 376 IIC_SSE_BIT_P_RR_WriteVecLogic
  { 1, 71, 72, 0, 0 }, // 377 IIC_SSE_PAUSE_WriteNop
  { 0, 0, 0, 0, 0 }, // 378 IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 379 IIC_SSE_PCLMULQDQ_RR_WriteCLMul
  { 0, 0, 0, 0, 0 }, // 380 WriteVecALULd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 381 WriteVecALU
  { 0, 0, 0, 0, 0 }, // 382 WritePCmpEStrILd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 383 WritePCmpEStrI
  { 0, 0, 0, 0, 0 }, // 384 WritePCmpEStrMLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 385 WritePCmpEStrM
  { 0, 0, 0, 0, 0 }, // 386 WritePCmpIStrILd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 387 WritePCmpIStrI
  { 0, 0, 0, 0, 0 }, // 388 WritePCmpIStrMLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 389 WritePCmpIStrM
  { 0, 0, 0, 0, 0 }, // 390 WriteShuffleLd_WriteRMW
  { 1, 20, 21, 0, 0 }, // 391 IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 392 IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd
  { 1, 21, 22, 0, 0 }, // 393 IIC_SSE_PHADDSUBD_RR_WriteVecALU
  { 1, 28, 29, 0, 0 }, // 394 IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd
  { 1, 2, 3, 0, 0 }, // 395 IIC_SSE_PHADDSUBW_RR_WriteVecALU
  { 0, 0, 0, 0, 0 }, // 396 WriteVecIMulLd
  { 0, 0, 0, 0, 0 }, // 397 WriteVecIMul
  { 1, 5, 6, 0, 0 }, // 398 IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 399 IIC_SSE_PINSRW_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 400 WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 401 IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 402 IIC_SSE_PMADD_WriteVecIMul
  { 1, 5, 6, 0, 0 }, // 403 IIC_SSE_INTALU_P_RM_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 404 IIC_SSE_INTALU_P_RR_WriteShuffle
  { 0, 0, 0, 0, 0 }, // 405 WriteShuffleLd
  { 1, 57, 58, 0, 0 }, // 406 IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd
  { 1, 57, 58, 0, 0 }, // 407 IIC_SSE_INTMUL_P_RR_WriteVecIMul
  { 0, 0, 0, 0, 0 }, // 408 WriteFAddLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 409 IIC_POP_REG16_WriteLoad
  { 1, 21, 22, 0, 0 }, // 410 IIC_POP_MEM_WriteLoad
  { 1, 18, 20, 0, 0 }, // 411 IIC_POP_REG_WriteLoad
  { 1, 14, 15, 0, 0 }, // 412 IIC_POP_A_WriteLoad
  { 0, 0, 0, 0, 0 }, // 413 IIC_SSE_POPCNT_RM_WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 414 IIC_SSE_POPCNT_RR_WriteFAdd
  { 1, 83, 84, 0, 0 }, // 415 IIC_POP_SR_WriteSystem
  { 1, 38, 39, 0, 0 }, // 416 IIC_POP_F_WriteLoad
  { 1, 77, 78, 0, 0 }, // 417 IIC_POP_FD_WriteLoad
  { 1, 84, 85, 0, 0 }, // 418 IIC_POP_SR_SS_WriteSystem
  { 1, 43, 44, 0, 0 }, // 419 IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd
  { 1, 20, 21, 0, 0 }, // 420 IIC_SSE_PSHUFB_RR_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 421 IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 422 IIC_SSE_PSHUF_RI_WriteShuffle
  { 1, 5, 6, 0, 0 }, // 423 IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 424 IIC_SSE_PSIGN_RR_WriteVecALU
  { 1, 4, 5, 0, 0 }, // 425 IIC_SSE_INTSHDQ_P_RI_WriteVecShift
  { 1, 4, 5, 0, 0 }, // 426 IIC_SSE_INTSH_P_RI_WriteVecShift
  { 1, 21, 22, 0, 0 }, // 427 IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 428 IIC_SSE_INTSH_P_RR_WriteVecShift
  { 1, 5, 6, 0, 0 }, // 429 IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 430 IIC_SSE_UNPCK_WriteShuffle
  { 1, 18, 20, 0, 0 }, // 431 IIC_PUSH_IMM_WriteStore
  { 1, 18, 20, 0, 0 }, // 432 IIC_PUSH_REG_WriteStore
  { 1, 16, 17, 0, 0 }, // 433 IIC_PUSH_MEM_WriteStore
  { 1, 28, 29, 0, 0 }, // 434 IIC_PUSH_A_WriteStore
  { 1, 16, 17, 0, 0 }, // 435 IIC_PUSH_SR_WriteSystem
  { 1, 16, 17, 0, 0 }, // 436 IIC_PUSH_CS_WriteSystem
  { 1, 14, 15, 0, 0 }, // 437 IIC_PUSH_F_WriteStore
  { 1, 5, 6, 0, 0 }, // 438 IIC_SR_WriteShiftLd_WriteRMW
  { 1, 5, 6, 0, 0 }, // 439 IIC_SR_WriteShift
  { 1, 26, 27, 0, 0 }, // 440 IIC_SSE_RCPP_RM_WriteFRcpLd
  { 1, 14, 15, 0, 0 }, // 441 IIC_SSE_RCPP_RR_WriteFRcp
  { 1, 75, 76, 0, 0 }, // 442 IIC_SSE_RCPS_RM_WriteFRcpLd
  { 1, 75, 76, 0, 0 }, // 443 IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 444 WriteFRcp
  { 1, 75, 76, 0, 0 }, // 445 IIC_SSE_RCPS_RR_WriteFRcp
  { 1, 85, 86, 0, 0 }, // 446 IIC_RDMSR_WriteSystem
  { 1, 50, 51, 0, 0 }, // 447 IIC_RDPMC_WriteSystem
  { 1, 86, 87, 0, 0 }, // 448 IIC_RDTSC_WriteSystem
  { 1, 87, 88, 0, 0 }, // 449 IIC_REP_MOVS_WriteMicrocoded
  { 1, 82, 83, 0, 0 }, // 450 IIC_REP_STOS_WriteMicrocoded
  { 1, 18, 20, 0, 0 }, // 451 IIC_RET_IMM_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 452 WriteShiftLd
  { 0, 0, 0, 0, 0 }, // 453 WriteShift
  { 0, 0, 0, 0, 0 }, // 454 IIC_SSE_ROUNDPS_REG_WriteFAddLd
  { 0, 0, 0, 0, 0 }, // 455 IIC_SSE_ROUNDPS_REG_WriteFAdd
  { 0, 0, 0, 0, 0 }, // 456 IIC_SSE_ROUNDPS_MEM_WriteFAddLd
  { 1, 88, 89, 0, 0 }, // 457 IIC_RSM_WriteSystem
  { 1, 34, 35, 0, 0 }, // 458 IIC_SSE_SQRTPS_RM_WriteFSqrtLd
  { 1, 34, 35, 0, 0 }, // 459 IIC_SSE_SQRTPS_RR_WriteFSqrt
  { 1, 36, 37, 0, 0 }, // 460 IIC_SSE_SQRTSS_RM_WriteFSqrtLd
  { 1, 36, 37, 0, 0 }, // 461 IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 462 WriteFSqrt
  { 1, 36, 37, 0, 0 }, // 463 IIC_SSE_SQRTSS_RR_WriteFSqrt
  { 0, 0, 0, 0, 0 }, // 464 WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd
  { 1, 16, 17, 0, 0 }, // 465 IIC_SCAS_WriteMicrocoded
  { 1, 16, 17, 0, 0 }, // 466 IIC_SET_M_WriteALU_WriteStore
  { 1, 4, 5, 0, 0 }, // 467 IIC_SET_R_WriteALU
  { 1, 5, 6, 0, 0 }, // 468 IIC_SSE_SFENCE_WriteFence
  { 1, 20, 21, 0, 0 }, // 469 IIC_SGDT_WriteSystem
  { 1, 6, 7, 0, 0 }, // 470 IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 471 IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 6, 7, 0, 0 }, // 472 IIC_SHD16_REG_CL_WriteShift
  { 1, 6, 7, 0, 0 }, // 473 IIC_SHD16_REG_IM_WriteShift
  { 1, 20, 21, 0, 0 }, // 474 IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 20, 21, 0, 0 }, // 475 IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 476 IIC_SHD32_REG_CL_WriteShift
  { 1, 16, 17, 0, 0 }, // 477 IIC_SHD32_REG_IM_WriteShift
  { 1, 14, 15, 0, 0 }, // 478 IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW
  { 1, 14, 15, 0, 0 }, // 479 IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW
  { 1, 28, 29, 0, 0 }, // 480 IIC_SHD64_REG_CL_WriteShift
  { 1, 14, 15, 0, 0 }, // 481 IIC_SHD64_REG_IM_WriteShift
  { 1, 5, 6, 0, 0 }, // 482 IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 483 IIC_SSE_SHUFP_WriteFShuffle
  { 1, 20, 21, 0, 0 }, // 484 IIC_SIDT_WriteSystem
  { 1, 21, 22, 0, 0 }, // 485 IIC_SLDT_WriteSystem
  { 1, 14, 15, 0, 0 }, // 486 IIC_SMSW_WriteSystem
  { 1, 33, 34, 0, 0 }, // 487 IIC_SSE_SQRTPD_RM_WriteFSqrtLd
  { 1, 33, 34, 0, 0 }, // 488 IIC_SSE_SQRTPD_RR_WriteFSqrt
  { 1, 35, 36, 0, 0 }, // 489 IIC_SSE_SQRTSD_RM_WriteFSqrtLd
  { 1, 35, 36, 0, 0 }, // 490 IIC_SSE_SQRTSD_RR_WriteFSqrt
  { 1, 4, 5, 0, 0 }, // 491 IIC_STC_WriteALU
  { 1, 3, 4, 0, 0 }, // 492 IIC_STD_WriteALU
  { 1, 71, 72, 0, 0 }, // 493 IIC_STI_WriteALU
  { 1, 17, 18, 0, 0 }, // 494 IIC_SSE_STMXCSR_WriteStore
  { 1, 4, 5, 0, 0 }, // 495 IIC_STOS_WriteMicrocoded
  { 1, 21, 22, 0, 0 }, // 496 IIC_STR_WriteSystem
  { 1, 43, 44, 0, 0 }, // 497 IIC_FST80_WriteStore
  { 1, 16, 17, 0, 0 }, // 498 IIC_FST_WriteMove
  { 1, 24, 25, 0, 0 }, // 499 IIC_SWAPGS_WriteSystem
  { 1, 89, 90, 0, 0 }, // 500 IIC_SYSCALL_WriteSystem
  { 1, 80, 81, 0, 0 }, // 501 IIC_SYS_ENTER_EXIT_WriteSystem
  { 1, 15, 16, 0, 0 }, // 502 IIC_JMP_REL_WriteJumpLd
  { 1, 16, 17, 0, 0 }, // 503 IIC_JMP_MEM_WriteJump
  { 1, 15, 16, 0, 0 }, // 504 IIC_JMP_REG_WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 505 WriteJumpLd
  { 0, 0, 0, 0, 0 }, // 506 WriteJump
  { 1, 14, 15, 0, 0 }, // 507 IIC_FUCOMI_WriteFAdd
  { 1, 15, 16, 0, 0 }, // 508 IIC_FUCOM_WriteFAdd
  { 1, 5, 6, 0, 0 }, // 509 IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 510 IIC_SSE_UNPCK_WriteFShuffle
  { 1, 2, 3, 0, 0 }, // 511 IIC_SSE_ALU_F64P_RM
  { 1, 6, 7, 0, 0 }, // 512 IIC_SSE_ALU_F64P_RR
  { 1, 7, 9, 0, 0 }, // 513 IIC_SSE_ALU_F32P_RM
  { 1, 9, 10, 0, 0 }, // 514 IIC_SSE_ALU_F32P_RR
  { 1, 5, 6, 0, 0 }, // 515 IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 516 IIC_ALU_NONMEM_WriteFBlend
  { 0, 0, 0, 0, 0 }, // 517 WriteFVarBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 518 WriteFVarBlend
  { 0, 0, 0, 0, 0 }, // 519 WriteFShuffleLd
  { 0, 0, 0, 0, 0 }, // 520 WriteFShuffle256
  { 0, 0, 0, 0, 0 }, // 521 WriteFShuffle
  { 1, 9, 10, 0, 0 }, // 522 IIC_SSE_ALU_F32S_RR
  { 0, 0, 0, 0, 0 }, // 523 WriteCvtI2FLd
  { 0, 0, 0, 0, 0 }, // 524 WriteCvtI2F
  { 0, 0, 0, 0, 0 }, // 525 WriteCvtF2ILd
  { 0, 0, 0, 0, 0 }, // 526 WriteCvtF2I
  { 0, 0, 0, 0, 0 }, // 527 WriteCvtF2FLd
  { 0, 0, 0, 0, 0 }, // 528 WriteCvtF2F
  { 0, 0, 0, 0, 0 }, // 529 WriteCvtF2FLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 530 WriteCvtF2FLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 531 WriteCvtI2FLd_ReadAfterLd
  { 1, 90, 91, 0, 0 }, // 532 IIC_VERR_WriteSystem
  { 1, 91, 92, 0, 0 }, // 533 IIC_VERW_REG_WriteSystem
  { 1, 30, 31, 0, 0 }, // 534 IIC_VERW_MEM_WriteSystem
  { 0, 0, 0, 0, 0 }, // 535 WriteStore
  { 0, 0, 0, 0, 0 }, // 536 WriteShuffle256
  { 1, 5, 6, 0, 0 }, // 537 IIC_ALU_MEM_WriteFBlendLd_WriteRMW
  { 0, 0, 0, 0, 0 }, // 538 WriteFShuffleLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 539 WriteShuffle256Ld_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 540 IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 541 IIC_ALU_NONMEM_WriteFShuffle
  { 1, 5, 6, 0, 0 }, // 542 IIC_SSE_MOVDQ
  { 1, 5, 6, 0, 0 }, // 543 IIC_SSE_MOV_LH
  { 1, 74, 75, 0, 0 }, // 544 IIC_SSE_MOVD_ToGP
  { 1, 5, 6, 0, 0 }, // 545 IIC_SSE_MOV_S_MR
  { 1, 5, 6, 0, 0 }, // 546 IIC_SSE_MOV_S_RM
  { 1, 4, 5, 0, 0 }, // 547 IIC_SSE_MOV_S_RR
  { 1, 4, 5, 0, 0 }, // 548 IIC_SSE_MOVQ_RR
  { 1, 5, 6, 0, 0 }, // 549 IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 550 IIC_ALU_NONMEM_WriteMPSAD
  { 0, 0, 0, 0, 0 }, // 551 WriteVecALULd
  { 1, 5, 6, 0, 0 }, // 552 IIC_SSE_INTALU_P_RM
  { 1, 4, 5, 0, 0 }, // 553 IIC_SSE_INTALU_P_RR
  { 1, 5, 6, 0, 0 }, // 554 IIC_SSE_BIT_P_RM
  { 1, 4, 5, 0, 0 }, // 555 IIC_SSE_BIT_P_RR
  { 0, 0, 0, 0, 0 }, // 556 WriteBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 557 WriteBlend
  { 0, 0, 0, 0, 0 }, // 558 WriteVarBlendLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 559 WriteVarBlend
  { 1, 5, 6, 0, 0 }, // 560 IIC_ALU_MEM_WriteBlendLd_ReadAfterLd
  { 1, 4, 5, 0, 0 }, // 561 IIC_ALU_NONMEM_WriteBlend
  { 0, 0, 0, 0, 0 }, // 562 WriteCLMulLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 563 WriteCLMul
  { 0, 0, 0, 0, 0 }, // 564 WriteFShuffle256Ld_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 565 IIC_ALU_MEM_WriteShuffleLd
  { 1, 4, 5, 0, 0 }, // 566 IIC_ALU_NONMEM_WriteShuffle
  { 1, 57, 58, 0, 0 }, // 567 IIC_SSE_INTMUL_P_RM
  { 1, 57, 58, 0, 0 }, // 568 IIC_SSE_INTMUL_P_RR
  { 1, 5, 6, 0, 0 }, // 569 IIC_SSE_PSHUF_MI_WriteShuffleLd
  { 0, 0, 0, 0, 0 }, // 570 WriteVecShift
  { 1, 21, 22, 0, 0 }, // 571 IIC_SSE_INTSH_P_RM
  { 1, 16, 17, 0, 0 }, // 572 IIC_SSE_INTSH_P_RR
  { 0, 0, 0, 0, 0 }, // 573 WriteVarVecShiftLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 574 WriteVarVecShift
  { 1, 5, 6, 0, 0 }, // 575 IIC_SSE_UNPCK
  { 0, 0, 0, 0, 0 }, // 576 WriteFRcpLd_ReadAfterLd
  { 0, 0, 0, 0, 0 }, // 577 WriteFSqrtLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 578 IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd
  { 1, 5, 6, 0, 0 }, // 579 IIC_SSE_SHUFP_WriteShuffle
  { 1, 33, 34, 0, 0 }, // 580 IIC_SSE_SQRTPD_RM
  { 1, 33, 34, 0, 0 }, // 581 IIC_SSE_SQRTPD_RR
  { 1, 34, 35, 0, 0 }, // 582 IIC_SSE_SQRTPS_RM
  { 1, 34, 35, 0, 0 }, // 583 IIC_SSE_SQRTPS_RR
  { 1, 36, 37, 0, 0 }, // 584 IIC_SSE_SQRTSS_RR
  { 1, 36, 37, 0, 0 }, // 585 IIC_SSE_SQRTSS_RM
  { 1, 4, 5, 0, 0 }, // 586 IIC_WAIT_WriteMicrocoded
  { 1, 92, 93, 0, 0 }, // 587 IIC_WRMSR_WriteSystem
  { 1, 21, 22, 0, 0 }, // 588 IIC_XADD_MEM_WriteALULd_WriteRMW
  { 1, 16, 17, 0, 0 }, // 589 IIC_XADD_REG_WriteALU
  { 1, 16, 17, 0, 0 }, // 590 IIC_XCHG_REG_WriteALU
  { 1, 21, 22, 0, 0 }, // 591 IIC_XCHG_MEM_WriteALULd_WriteRMW
  { 1, 18, 20, 0, 0 }, // 592 IIC_FXCH_WriteMove
  { 1, 6, 7, 0, 0 }, // 593 IIC_XLAT_WriteLoad
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
  { 8,  1}, // #19
  { 9,  2}, // #20
  {10,  3}, // #21
  { 9,  2}, // #22
  {10,  2}, // #23
  { 8,  1}, // #24
  { 9, 10}, // #25
  {10, 11}, // #26
  { 9, 11}, // #27
  {10, 11}, // #28
  { 6,  1}, // #29
  { 8,  1}, // #30
  { 9,  1}, // #31
  {10,  2}, // #32
  { 6,  1}, // #33
  { 9,  1}, // #34
  {10,  1}, // #35
  { 5,  1}, // #36
  { 6,  1}, // #37
  { 7,  1}, // #38
  { 8,  1}, // #39
  { 9,  1}, // #40
  {10,  2}, // #41
  { 5,  1}, // #42
  { 6,  1}, // #43
  { 7,  1}, // #44
  { 9,  1}, // #45
  {10,  1}, // #46
  { 8,  1}, // #47
  {10,  1}, // #48
  { 1, 10}, // #49
  { 2,  1}, // #50
  { 6,  1}, // #51
  { 8,  1}, // #52
  { 9,  1}, // #53
  {10,  2}, // #54
  { 1, 10}, // #55
  { 2,  1}, // #56
  { 6,  1}, // #57
  { 9,  1}, // #58
  {10,  1}, // #59
  { 4,  1}, // #60
  { 6,  1}, // #61
  { 8,  1}, // #62
  { 9,  1}, // #63
  {10,  3}, // #64
  { 4,  1}, // #65
  { 8,  1}, // #66
  {10,  2}, // #67
  { 2,  1}, // #68
  { 3,  1}, // #69
  { 5,  1}, // #70
  { 6,  2}, // #71
  { 7,  2}, // #72
  { 8,  1}, // #73
  { 9,  3}, // #74
  {10,  4}, // #75
  { 2,  1}, // #76
  { 3,  1}, // #77
  { 5,  1}, // #78
  { 6,  2}, // #79
  { 7,  2}, // #80
  { 9,  3}, // #81
  {10,  3}, // #82
  { 8,  1}, // #83
  { 9, 17}, // #84
  {10, 18}, // #85
  { 9, 18}, // #86
  {10, 18}, // #87
  { 8,  1}, // #88
  { 9,  7}, // #89
  {10,  8}, // #90
  { 9,  8}, // #91
  {10,  8}, // #92
  { 4,  1}, // #93
  { 7,  1}, // #94
  { 8,  1}, // #95
  { 9,  1}, // #96
  {10,  3}, // #97
  { 2,  1}, // #98
  { 5,  1}, // #99
  { 6,  2}, // #100
  { 7,  1}, // #101
  { 8,  1}, // #102
  { 9,  2}, // #103
  {10,  3}, // #104
  { 2,  1}, // #105
  { 5,  1}, // #106
  { 6,  2}, // #107
  { 7,  1}, // #108
  { 9,  2}, // #109
  {10,  2}, // #110
  { 3,  1}, // #111
  { 4,  1}, // #112
  { 7,  1}, // #113
  { 8,  1}, // #114
  { 9,  1}, // #115
  {10,  3}, // #116
  { 3,  1}, // #117
  { 5,  1}, // #118
  { 6,  1}, // #119
  { 7,  2}, // #120
  { 8,  1}, // #121
  { 9,  2}, // #122
  {10,  3}, // #123
  { 3,  1}, // #124
  { 5,  1}, // #125
  { 6,  1}, // #126
  { 7,  2}, // #127
  { 9,  2}, // #128
  {10,  2}, // #129
  {13,  1}, // #130
  {14,  1}, // #131
  { 4,  1}, // #132
  {10,  1}, // #133
  {12,  1}, // #134
  {13,  1}, // #135
  {14,  3}, // #136
  {10,  1}, // #137
  {12,  1}, // #138
  {13,  1}, // #139
  {14,  2}, // #140
  { 3,  1}, // #141
  { 8,  1}, // #142
  { 9,  1}, // #143
  {10,  1}, // #144
  {11,  1}, // #145
  {12,  1}, // #146
  {13,  1}, // #147
  {14,  2}, // #148
  { 3,  1}, // #149
  { 8,  1}, // #150
  { 9,  1}, // #151
  {11,  1}, // #152
  {13,  1}, // #153
  {14,  1}, // #154
  { 5,  1}, // #155
  { 6,  1}, // #156
  { 8,  1}, // #157
  {10,  1}, // #158
  {11,  1}, // #159
  {12,  1}, // #160
  {13,  1}, // #161
  {14,  2}, // #162
  { 5,  1}, // #163
  { 6,  1}, // #164
  { 8,  1}, // #165
  {11,  1}, // #166
  {13,  1}, // #167
  {14,  1}, // #168
  { 5,  2}, // #169
  { 6,  2}, // #170
  { 8,  2}, // #171
  {10,  1}, // #172
  {11,  2}, // #173
  {12,  1}, // #174
  {13,  2}, // #175
  {14,  3}, // #176
  { 5,  2}, // #177
  { 6,  2}, // #178
  { 8,  2}, // #179
  {11,  2}, // #180
  {13,  2}, // #181
  {14,  2}, // #182
  { 2,  2}, // #183
  { 5,  7}, // #184
  { 6,  9}, // #185
  { 7,  2}, // #186
  { 8,  7}, // #187
  {10,  1}, // #188
  {11,  9}, // #189
  {12,  1}, // #190
  {13,  9}, // #191
  {14, 10}, // #192
  { 2,  2}, // #193
  { 5,  8}, // #194
  { 6, 10}, // #195
  { 7,  2}, // #196
  { 8,  8}, // #197
  {11, 10}, // #198
  {13, 10}, // #199
  {14, 10}, // #200
  { 7,  1}, // #201
  {10,  1}, // #202
  {12,  1}, // #203
  {13,  1}, // #204
  {14,  2}, // #205
  { 7,  1}, // #206
  {13,  1}, // #207
  {14,  1}, // #208
  {10,  1}, // #209
  {12,  1}, // #210
  {14,  1}, // #211
  { 1, 10}, // #212
  { 2,  1}, // #213
  { 6,  1}, // #214
  { 7,  1}, // #215
  {10,  1}, // #216
  {11,  1}, // #217
  {12,  1}, // #218
  {13,  1}, // #219
  {14,  2}, // #220
  { 1, 10}, // #221
  { 2,  1}, // #222
  { 6,  1}, // #223
  { 7,  1}, // #224
  {11,  1}, // #225
  {13,  1}, // #226
  {14,  1}, // #227
  { 4,  1}, // #228
  {10,  1}, // #229
  {11,  1}, // #230
  {12,  1}, // #231
  {13,  1}, // #232
  {14,  3}, // #233
  { 4,  1}, // #234
  {12,  1}, // #235
  {14,  2}, // #236
  { 4,  1}, // #237
  {10,  1}, // #238
  {12,  1}, // #239
  {14,  2}, // #240
  { 2,  1}, // #241
  { 5,  2}, // #242
  { 6,  3}, // #243
  { 7,  1}, // #244
  { 8,  2}, // #245
  {10,  1}, // #246
  {11,  3}, // #247
  {12,  1}, // #248
  {13,  3}, // #249
  {14,  4}, // #250
  { 2,  1}, // #251
  { 5,  2}, // #252
  { 6,  3}, // #253
  { 7,  1}, // #254
  { 8,  2}, // #255
  {11,  3}, // #256
  {13,  3}, // #257
  {14,  3}, // #258
  { 2,  2}, // #259
  { 5,  1}, // #260
  { 6,  3}, // #261
  { 7,  2}, // #262
  { 8,  1}, // #263
  {10,  1}, // #264
  {11,  3}, // #265
  {12,  1}, // #266
  {13,  3}, // #267
  {14,  4}, // #268
  { 2,  2}, // #269
  { 5,  1}, // #270
  { 6,  3}, // #271
  { 7,  2}, // #272
  { 8,  1}, // #273
  {11,  3}, // #274
  {13,  3}, // #275
  {14,  3}, // #276
  { 2,  3}, // #277
  { 5,  2}, // #278
  { 6,  5}, // #279
  { 7,  3}, // #280
  { 8,  2}, // #281
  { 9,  2}, // #282
  {10,  1}, // #283
  {11,  5}, // #284
  {12,  1}, // #285
  {13,  7}, // #286
  {14,  8}, // #287
  { 6,  6}, // #288
  { 9,  2}, // #289
  {11,  6}, // #290
  {13,  8}, // #291
  {14,  8}, // #292
  { 6,  6}, // #293
  { 9,  2}, // #294
  {10,  1}, // #295
  {11,  6}, // #296
  {12,  1}, // #297
  {13,  8}, // #298
  {14,  9}, // #299
  { 2,  3}, // #300
  { 5,  4}, // #301
  { 6,  7}, // #302
  { 7,  3}, // #303
  { 8,  4}, // #304
  { 9,  2}, // #305
  {11,  7}, // #306
  {13,  9}, // #307
  {14,  9}, // #308
  { 2,  3}, // #309
  { 6,  3}, // #310
  { 7,  3}, // #311
  {10,  1}, // #312
  {11,  3}, // #313
  {12,  1}, // #314
  {13,  3}, // #315
  {14,  4}, // #316
  { 2,  3}, // #317
  { 6,  3}, // #318
  { 7,  3}, // #319
  {11,  3}, // #320
  {13,  3}, // #321
  {14,  3}, // #322
  { 4,  1}, // #323
  { 5,  1}, // #324
  { 6,  1}, // #325
  { 8,  1}, // #326
  {10,  1}, // #327
  {11,  1}, // #328
  {12,  1}, // #329
  {13,  1}, // #330
  {14,  3}, // #331
  { 4,  1}, // #332
  { 7,  1}, // #333
  {10,  1}, // #334
  {12,  1}, // #335
  {13,  1}, // #336
  {14,  3}, // #337
  { 4,  1}, // #338
  {12,  1}, // #339
  {13,  1}, // #340
  {14,  3}, // #341
  { 3,  1}, // #342
  { 4,  1}, // #343
  { 8,  1}, // #344
  { 9,  1}, // #345
  {10,  1}, // #346
  {11,  1}, // #347
  {12,  1}, // #348
  {13,  1}, // #349
  {14,  3}, // #350
  { 1,  1}, // #351
  { 3,  1}, // #352
  { 2,  1}, // #353
  { 3,  1}, // #354
  { 7,  1}, // #355
  { 1,  5}, // #356
  { 3,  5}, // #357
  { 7,  1}, // #358
  { 1,  1}, // #359
  { 3,  1}, // #360
  { 7,  1}, // #361
  { 4,  1}, // #362
  { 6,  1}, // #363
  { 7,  1}, // #364
  { 6,  1}, // #365
  { 7,  1}, // #366
  { 8, 25}, // #367
  { 6,  1}, // #368
  { 8, 25}, // #369
  { 1,  1}, // #370
  { 3,  1}, // #371
  { 7,  1}, // #372
  { 9, 34}, // #373
  { 1,  1}, // #374
  { 3,  1}, // #375
  { 9, 34}, // #376
  { 1,  1}, // #377
  { 3,  1}, // #378
  { 7,  2}, // #379
  { 1,  1}, // #380
  { 3,  1}, // #381
  { 7,  1}, // #382
  {10,  2}, // #383
  { 1,  1}, // #384
  { 3,  1}, // #385
  {10,  2}, // #386
  { 1,  1}, // #387
  { 3,  1}, // #388
  { 5,  1}, // #389
  { 6,  1}, // #390
  { 7,  1}, // #391
  { 1, 10}, // #392
  { 3, 10}, // #393
  { 7,  1}, // #394
  { 1, 21}, // #395
  { 3, 21}, // #396
  { 7,  1}, // #397
  { 1, 17}, // #398
  { 3, 17}, // #399
  { 7,  1}, // #400
  { 1, 13}, // #401
  { 3, 13}, // #402
  { 7,  1}, // #403
  { 4,  1}, // #404
  { 6,  1}, // #405
  { 7,  2} // #406
}; // X86WriteProcResTable

// {Cycles, WriteResourceID}
extern const llvm::MCWriteLatencyEntry X86WriteLatencyTable[] = {
  { 0,  0}, // Invalid
  {100,  0}, // #1 WriteMicrocoded_WriteSystem
  { 1,  0}, // #2 WriteALU_WriteVecLogic_WriteZero_WriteFBlend_WriteShift_WriteJump_WriteFShuffle_WriteStore_WriteMove_WriteLEA_WriteFence_WriteShuffle_WriteVecALU_WriteVecShift_WriteNop_WriteFShuffle256_WriteShuffle256_WriteBlend_WriteVarVecShift_WriteFVarBlend_WriteVarBlend
  { 5,  0}, // #3 WriteALULd_WriteVecLogicLd_WriteFBlendLd_WriteShiftLd_WriteJumpLd_WriteFShuffleLd_WriteVecALULd_WriteShuffleLd_WriteVecIMul_WriteVecShiftLd_WriteFMul_WriteFRcp_WriteShuffle256Ld_WriteBlendLd_WriteFShuffle256Ld_WriteVarVecShiftLd
  { 1,  0}, // #4 WriteRMW
  { 7,  0}, // #5 WriteFAddLd_WriteCvtF2ILd_WriteCvtF2FLd_WriteIMulLd_WriteAESDecEncLd_WriteCLMulLd_WriteShuffle256Ld_WriteFShuffle256Ld_WriteCvtI2FLd_WriteVecIMulLd_WriteMPSAD
  { 3,  0}, // #6 WriteFAdd_WriteCvtF2I_WriteCvtF2F_WriteIMul_WriteIMulH_WritePCmpIStrILd_WriteFShuffle256_WriteShuffle256_WriteLoad
  { 8,  0}, // #7 WriteAESDecEncLd_WriteAESIMCLd_WriteAESKeyGenLd_WriteCvtI2FLd_WriteFMulLd_WriteFRcpLd
  { 4,  0}, // #8 WriteLoad_WriteCvtI2F_WritePCmpEStrILd_WriteALULd_WriteVecLogicLd_WriteFBlendLd_WriteShiftLd_WriteJumpLd_WriteCvtF2I_WriteCvtF2F_WriteFShuffleLd_WriteVecALULd_WriteShuffleLd_WriteVecIMul_WriteVecShiftLd_WriteFVarBlendLd_WriteShuffle256Ld_WriteBlendLd_WriteVarBlendLd_WriteFShuffle256Ld_WriteVarVecShiftLd
  {29,  0}, // #9 WriteIDivLd
  {25,  0}, // #10 WriteIDiv
  {16,  0}, // #11 WriteFDivLd
  {12,  0}, // #12 WriteFDiv
  { 9,  0}, // #13 WriteVecIMulLd_WriteFMulLd_WriteFRcpLd
  { 6,  0}, // #14 WriteMPSADLd_WriteFVarBlendLd_WriteVarBlendLd_WriteVarVecShiftLd_WriteFAddLd_WriteIMulLd
  { 3,  0}, // #15 WriteIMul
  { 3,  0}, // #16 WriteIMulH
  {14,  0}, // #17 WriteCLMulLd_WriteAESIMCLd
  {11,  0}, // #18 WritePCmpEStrMLd_WritePCmpIStrMLd_WritePCmpEStrILd_WritePCmpIStrILd
  {19,  0}, // #19 WriteFSqrtLd
  {15,  0}, // #20 WriteFSqrt
  { 1,  0}, // #21 WriteALU
  { 1,  0}, // #22 WriteStore
  { 2,  0}, // #23 WriteFVarBlend_WriteVarBlend_WriteVarVecShift
  { 7,  0}, // #24 WriteCvtF2FLd
  { 1,  0}, // #25 WriteRMW
  {10,  0}, // #26 WriteAESKeyGenLd_WritePCmpEStrMLd_WritePCmpIStrMLd_WriteMPSADLd_WriteCLMulLd
  { 4,  0}, // #27 WriteALULd_WriteFBlendLd_WriteShuffleLd_WriteShiftLd
  { 1,  0}, // #28 WriteRMW
  {37,  0}, // #29 WriteFDivLd
  {34,  0}, // #30 WriteFDiv
  { 6,  0}, // #31 WriteIMulLd
  { 1,  0}, // #32 WriteIMulH
  { 3,  0}, // #33 WriteIMul
  { 1,  0}, // #34 WriteIMulH
  {21,  0}, // #35 WritePCmpEStrILd
  {17,  0}, // #36 WritePCmpEStrMLd_WritePCmpIStrILd
  {13,  0}, // #37 WritePCmpIStrMLd
  {18,  0} // #38 WriteFSqrtLd
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
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 10, 5,  5, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 15, 4,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 19, 3,  7, 1,  1, 1}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 22, 2,  7, 1,  0, 0}, // #26
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 19, 3,  7, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 22, 2,  7, 1,  0, 0}, // #28
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 24, 3,  7, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 27, 2,  7, 1,  0, 0}, // #30
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #31
  {DBGFIELD("WriteVecLogic")      1, 0, 0,  1, 2,  2, 1,  0, 0}, // #32
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #35
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 29, 4,  3, 1,  1, 1}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #37
  {DBGFIELD("IIC_ALU_MEM")        0, 0, 0,  0, 0,  0, 0,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_NONMEM")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #39
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 29, 4,  3, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #55
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #57
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #62
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #64
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #66
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #67
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #72
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #73
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #74
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #75
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #76
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #77
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #78
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 10, 5,  7, 1,  0, 0}, // #79
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 15, 4,  8, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 10, 5,  7, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 15, 4,  8, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 10, 5,  7, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 15, 4,  8, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #98
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #99
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #100
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #101
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #102
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #103
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #105
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #107
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #109
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #110
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 50, 5, 11, 1,  1, 1}, // #111
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 56, 4, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 56, 4, 12, 1,  0, 0}, // #113
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 50, 5, 11, 1,  0, 0}, // #114
  {DBGFIELD("WriteFDiv")          1, 0, 0, 56, 4, 12, 1,  0, 0}, // #115
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 50, 5, 11, 1,  1, 1}, // #116
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 56, 4, 12, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 56, 4, 12, 1,  0, 0}, // #118
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #119
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #121
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #122
  {DBGFIELD("WriteSystem")        1, 0, 0,  1, 2,  1, 1,  0, 0}, // #123
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #126
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #127
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #130
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #133
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #158
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #161
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #162
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #164
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #166
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #168
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 49, 6,  9, 1,  2, 2}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 55, 5, 10, 1,  0, 0}, // #170
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #171
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #172
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #173
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #174
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #175
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #177
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #178
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #179
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #180
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #182
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #183
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #184
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #185
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #188
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #193
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_IRET_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #199
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #200
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #201
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  7, 1,  1, 1}, // #202
  {DBGFIELD("WriteLoad")          1, 0, 0, 47, 2,  8, 1,  0, 0}, // #203
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 42, 5,  2, 1,  0, 0}, // #204
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #205
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #208
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #210
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #211
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #212
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #215
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #216
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #217
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #218
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #219
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 47, 2,  8, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #223
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #224
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 16, 3,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #228
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #234
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #235
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #236
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #237
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #241
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #244
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #245
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #246
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #247
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #248
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #251
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #252
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #259
  {DBGFIELD("WriteMove")          1, 0, 0,  1, 2,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #264
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #265
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #266
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #267
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #271
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #272
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #273
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #274
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #276
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #278
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #280
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #281
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #282
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #283
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #285
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 29, 4,  3, 1,  1, 1}, // #286
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #287
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #288
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #289
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #290
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #291
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #295
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #297
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #299
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #302
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #305
  {DBGFIELD("IIC_ALU_NONMEM_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #306
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #308
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #310
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #311
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #313
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #320
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #325
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #328
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #330
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #332
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #335
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #336
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0,  7, 3,  3, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 68, 8, 14, 1,  1, 1}, // #340
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 76, 7, 14, 1,  0, 0}, // #341
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #342
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #344
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #346
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #347
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  2, 2}, // #348
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #349
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #350
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #351
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #352
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #353
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #354
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 10, 5,  5, 2,  0, 0}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 15, 4, 15, 2,  0, 0}, // #357
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 50, 5, 13, 1,  0, 0}, // #358
  {DBGFIELD("WriteFMul")          1, 0, 0, 56, 4,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #360
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #361
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #362
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #363
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #364
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #365
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #366
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #367
  {DBGFIELD("WriteShuffle")       1, 0, 0, 16, 3,  2, 1,  0, 0}, // #368
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #369
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #370
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #371
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #373
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0,  7, 3,  3, 1,  1, 1}, // #375
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #377
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 83, 3, 17, 1,  1, 1}, // #378
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 86, 2, 17, 1,  0, 0}, // #379
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #380
  {DBGFIELD("WriteVecALU")        1, 0, 0, 16, 3,  2, 1,  0, 0}, // #381
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 88, 3,  8, 1,  1, 1}, // #382
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 91, 2,  8, 1,  0, 0}, // #383
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 88, 3, 18, 1,  1, 1}, // #384
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 91, 2, 18, 1,  0, 0}, // #385
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 73, 3,  6, 1,  1, 1}, // #386
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 81, 2,  6, 1,  0, 0}, // #387
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 73, 3, 18, 1,  1, 1}, // #388
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 81, 2, 18, 1,  0, 0}, // #389
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 93, 5,  3, 2,  0, 0}, // #390
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #391
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #392
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #393
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #394
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #395
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 50, 5, 13, 1,  0, 0}, // #396
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 56, 4,  3, 1,  0, 0}, // #397
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #398
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #399
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #400
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #401
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #402
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #404
  {DBGFIELD("WriteShuffleLd")     1, 0, 0, 11, 4,  3, 1,  0, 0}, // #405
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #406
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #407
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #408
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #409
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #410
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #411
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #414
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #415
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #418
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #419
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #421
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #422
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #423
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #424
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #425
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 29, 4,  3, 1,  1, 1}, // #427
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #429
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #431
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_PUSH_MEM_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #435
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #436
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #437
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #438
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 33, 3,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 50, 5, 13, 1,  0, 0}, // #440
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #441
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd") 1, 0, 0, 50, 5, 13, 1,  0, 0}, // #442
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #443
  {DBGFIELD("WriteFRcp")          1, 0, 0, 56, 4,  3, 1,  0, 0}, // #444
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 56, 4,  3, 1,  0, 0}, // #445
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #446
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #447
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #448
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #449
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #450
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #451
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 29, 4,  3, 1,  0, 0}, // #452
  {DBGFIELD("WriteShift")         1, 0, 0, 33, 3,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #454
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #455
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 10, 5,  5, 1,  0, 0}, // #456
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 50, 5, 19, 1,  0, 0}, // #458
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 56, 4, 20, 1,  0, 0}, // #459
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd") 1, 0, 0, 50, 5, 19, 1,  0, 0}, // #460
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 50, 5, 19, 1,  1, 1}, // #461
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 56, 4, 20, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 56, 4, 20, 1,  0, 0}, // #463
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 29, 4,  3, 1,  2, 1}, // #464
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #465
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0,  3, 4, 21, 2,  0, 0}, // #466
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #469
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #470
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #471
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #472
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #475
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #482
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #483
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #486
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 50, 5, 19, 1,  0, 0}, // #487
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 56, 4, 20, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd") 1, 0, 0, 50, 5, 19, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 56, 4, 20, 1,  0, 0}, // #490
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #492
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0,  1, 2,  2, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #494
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #495
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #496
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 65, 3,  2, 1,  0, 0}, // #497
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0,  1, 2,  2, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #499
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #500
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #501
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #502
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #503
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 36, 6,  3, 1,  0, 0}, // #504
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 36, 6,  3, 1,  0, 0}, // #505
  {DBGFIELD("WriteJump")          1, 0, 0, 42, 5,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #507
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 15, 4,  6, 1,  0, 0}, // #508
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #509
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #510
  {DBGFIELD("IIC_SSE_ALU_F64P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #511
  {DBGFIELD("IIC_SSE_ALU_F64P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #512
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #513
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #514
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 29, 4,  3, 1,  1, 1}, // #515
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 33, 3,  2, 1,  0, 0}, // #516
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 98, 7, 14, 1,  1, 1}, // #517
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 105, 6, 23, 1,  0, 0}, // #518
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 36, 6,  3, 1,  0, 0}, // #519
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 56, 4,  2, 1,  0, 0}, // #520
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 42, 5,  2, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #522
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 10, 5,  7, 1,  0, 0}, // #523
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 15, 4,  8, 1,  0, 0}, // #524
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 10, 5,  5, 1,  0, 0}, // #525
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 15, 4,  6, 1,  0, 0}, // #526
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 10, 5,  5, 1,  0, 0}, // #527
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 15, 4,  6, 1,  0, 0}, // #528
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 111, 6, 24, 2,  0, 0}, // #529
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  5, 1,  1, 1}, // #530
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 5,  7, 1,  1, 1}, // #531
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #532
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #533
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #534
  {DBGFIELD("WriteStore")         1, 0, 0, 65, 3,  2, 1,  0, 0}, // #535
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 56, 4,  2, 1,  0, 0}, // #536
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 60, 5,  3, 2,  0, 0}, // #537
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #538
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 50, 5,  3, 1,  1, 1}, // #539
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 36, 6,  3, 1,  1, 1}, // #540
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 42, 5,  2, 1,  0, 0}, // #541
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #542
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #543
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #544
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #545
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #546
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #547
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #548
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 68, 8, 14, 1,  1, 1}, // #549
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 76, 7, 14, 1,  0, 0}, // #550
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 11, 4,  3, 1,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_INTALU_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_INTALU_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_BIT_P_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_BIT_P_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #556
  {DBGFIELD("WriteBlend")         1, 0, 0, 16, 3,  2, 1,  0, 0}, // #557
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 117, 7, 14, 1,  1, 1}, // #558
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 124, 6, 23, 1,  0, 0}, // #559
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #560
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #561
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 83, 3, 17, 1,  1, 1}, // #562
  {DBGFIELD("WriteCLMul")         1, 0, 0, 86, 2, 17, 1,  0, 0}, // #563
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 50, 5,  3, 1,  1, 1}, // #564
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #565
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_INTMUL_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("IIC_SSE_INTMUL_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #568
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 11, 4,  3, 1,  0, 0}, // #569
  {DBGFIELD("WriteVecShift")      1, 0, 0, 33, 3,  2, 1,  0, 0}, // #570
  {DBGFIELD("IIC_SSE_INTSH_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #571
  {DBGFIELD("IIC_SSE_INTSH_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #572
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 50, 5,  3, 1,  1, 1}, // #573
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 56, 4,  2, 1,  0, 0}, // #574
  {DBGFIELD("IIC_SSE_UNPCK")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #575
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 50, 5, 13, 1,  1, 1}, // #576
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 50, 5, 19, 1,  1, 1}, // #577
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 11, 4,  3, 1,  1, 1}, // #578
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffle") 1, 0, 0, 16, 3,  2, 1,  0, 0}, // #579
  {DBGFIELD("IIC_SSE_SQRTPD_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #580
  {DBGFIELD("IIC_SSE_SQRTPD_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #581
  {DBGFIELD("IIC_SSE_SQRTPS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #582
  {DBGFIELD("IIC_SSE_SQRTPS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #583
  {DBGFIELD("IIC_SSE_SQRTSS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #584
  {DBGFIELD("IIC_SSE_SQRTSS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #585
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #586
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0,  1, 2,  1, 1,  0, 0}, // #587
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #588
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #589
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #590
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0,  3, 4,  3, 2,  0, 0}, // #591
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0,  1, 2,  2, 1,  0, 0}, // #592
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 47, 2,  8, 1,  0, 0} // #593
}; // SandyBridgeModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc HaswellModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 137, 4,  3, 1,  1, 1}, // #7
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 130, 2,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 137, 4,  3, 1,  1, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 141, 8,  5, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 149, 6,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 155, 8,  5, 1,  1, 1}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 163, 6,  5, 1,  0, 0}, // #26
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 169, 8, 17, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 177, 6, 17, 1,  0, 0}, // #28
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 183, 10, 26, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 193, 8, 26, 1,  0, 0}, // #30
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 144, 5,  3, 1,  1, 1}, // #31
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 152, 3,  2, 1,  0, 0}, // #32
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #35
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 144, 5,  3, 1,  1, 1}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #37
  {DBGFIELD("IIC_ALU_MEM")        0, 0, 0,  0, 0,  0, 0,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_NONMEM")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #39
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #55
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #57
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 130, 2,  2, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 137, 4,  3, 1,  1, 1}, // #62
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 137, 4,  3, 1,  1, 1}, // #64
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #66
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #67
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #72
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #73
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #74
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #75
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #76
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #77
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #78
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 141, 8,  7, 1,  0, 0}, // #79
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 149, 6,  8, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 141, 8,  7, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 149, 6,  8, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 141, 8,  7, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 149, 6,  8, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #98
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #99
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #100
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #101
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #102
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #103
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #105
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #107
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #109
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #110
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 213, 8, 11, 1,  1, 1}, // #111
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 222, 6, 12, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 222, 6, 12, 1,  0, 0}, // #113
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 213, 8, 11, 1,  0, 0}, // #114
  {DBGFIELD("WriteFDiv")          1, 0, 0, 222, 6, 12, 1,  0, 0}, // #115
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 213, 8, 11, 1,  1, 1}, // #116
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 222, 6, 12, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 222, 6, 12, 1,  0, 0}, // #118
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #119
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #121
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #122
  {DBGFIELD("WriteSystem")        1, 0, 0, 130, 2,  1, 1,  0, 0}, // #123
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 228, 6,  3, 2,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #126
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #127
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #130
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #133
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #158
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #161
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #162
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #164
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #166
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #168
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 212, 9,  9, 1,  2, 2}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 221, 7, 10, 1,  0, 0}, // #170
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #171
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #172
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #173
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #174
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #175
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #177
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #178
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #179
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #180
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #182
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #183
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #184
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #185
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #188
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #193
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_IRET_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #199
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #200
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #201
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 141, 8,  7, 1,  1, 1}, // #202
  {DBGFIELD("WriteLoad")          1, 0, 0, 209, 3,  8, 1,  0, 0}, // #203
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 206, 3,  2, 1,  0, 0}, // #204
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #205
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #208
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #210
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #211
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #212
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #215
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #216
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #217
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #218
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #219
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 209, 3,  8, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 130, 2,  2, 1,  0, 0}, // #223
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #224
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 165, 4,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 237, 4,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #228
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #234
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #235
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #236
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #237
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #241
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #244
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #245
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #246
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 237, 4,  2, 1,  0, 0}, // #247
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #248
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #251
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #252
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #259
  {DBGFIELD("WriteMove")          1, 0, 0, 130, 2,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 157, 6,  3, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #264
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #265
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #266
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #267
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #269
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #271
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #272
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #273
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #274
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #276
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #278
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #280
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #281
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #282
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #283
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 155, 8,  3, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 222, 6,  2, 1,  0, 0}, // #285
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 213, 8,  3, 1,  1, 1}, // #286
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 222, 6,  2, 1,  0, 0}, // #287
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #288
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #289
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #290
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #291
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #295
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 130, 2,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #297
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #299
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #302
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #305
  {DBGFIELD("IIC_ALU_NONMEM_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #306
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #308
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #310
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #311
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #313
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #320
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #325
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #328
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #330
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #332
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 144, 5,  3, 1,  0, 0}, // #335
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #336
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 137, 4,  3, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 241, 10, 14, 1,  1, 1}, // #340
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 251, 8, 14, 1,  0, 0}, // #341
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #342
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #344
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #346
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #347
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  2, 2}, // #348
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #349
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #350
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #351
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #352
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #353
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #354
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 141, 8,  5, 2,  0, 0}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 149, 6, 15, 2,  0, 0}, // #357
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 213, 8, 13, 1,  0, 0}, // #358
  {DBGFIELD("WriteFMul")          1, 0, 0, 222, 6,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #360
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #361
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #362
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #363
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #364
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 157, 6,  3, 1,  0, 0}, // #365
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #366
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #367
  {DBGFIELD("WriteShuffle")       1, 0, 0, 163, 6,  2, 1,  0, 0}, // #368
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #369
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #370
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #371
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #373
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 144, 5,  3, 1,  1, 1}, // #375
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #377
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 259, 10,  5, 1,  1, 1}, // #378
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 269, 8,  5, 1,  0, 0}, // #379
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #380
  {DBGFIELD("WriteVecALU")        1, 0, 0, 165, 4,  2, 1,  0, 0}, // #381
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 277, 11, 18, 1,  1, 1}, // #382
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 288, 5, 18, 1,  0, 0}, // #383
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 293, 7, 26, 1,  1, 1}, // #384
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 300, 9, 26, 1,  0, 0}, // #385
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 309, 8, 18, 1,  1, 1}, // #386
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 317, 6, 18, 1,  0, 0}, // #387
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 309, 8, 26, 1,  1, 1}, // #388
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 317, 6, 26, 1,  0, 0}, // #389
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 323, 9,  3, 2,  0, 0}, // #390
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #391
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #392
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #393
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #394
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #395
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 213, 8, 13, 1,  0, 0}, // #396
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 222, 6,  3, 1,  0, 0}, // #397
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #398
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #399
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #400
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #401
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #402
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 155, 8,  3, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #404
  {DBGFIELD("WriteShuffleLd")     1, 0, 0, 155, 8,  3, 1,  0, 0}, // #405
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #406
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #407
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #408
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #409
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #410
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #411
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #414
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #415
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #418
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #419
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #421
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #422
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #423
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #424
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 222, 6,  2, 1,  0, 0}, // #425
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 222, 6,  2, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 213, 8,  3, 1,  1, 1}, // #427
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 222, 6,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #429
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #431
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_PUSH_MEM_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #435
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #436
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #437
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #438
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 206, 3,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 213, 8, 13, 1,  0, 0}, // #440
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #441
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd") 1, 0, 0, 213, 8, 13, 1,  0, 0}, // #442
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #443
  {DBGFIELD("WriteFRcp")          1, 0, 0, 222, 6,  3, 1,  0, 0}, // #444
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 222, 6,  3, 1,  0, 0}, // #445
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #446
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #447
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #448
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #449
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #450
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #451
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 201, 5,  3, 1,  0, 0}, // #452
  {DBGFIELD("WriteShift")         1, 0, 0, 206, 3,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #454
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #455
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 141, 8,  5, 1,  0, 0}, // #456
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 213, 8, 19, 1,  0, 0}, // #458
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 222, 6, 20, 1,  0, 0}, // #459
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd") 1, 0, 0, 213, 8, 19, 1,  0, 0}, // #460
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 213, 8, 19, 1,  1, 1}, // #461
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 222, 6, 20, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 222, 6, 20, 1,  0, 0}, // #463
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 201, 5,  3, 1,  2, 1}, // #464
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #465
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 338, 4, 21, 2,  0, 0}, // #466
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 237, 4,  2, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #469
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #470
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #471
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #472
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #475
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 332, 6,  3, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #482
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #483
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #486
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 213, 8, 19, 1,  0, 0}, // #487
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 222, 6, 20, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd") 1, 0, 0, 213, 8, 19, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 222, 6, 20, 1,  0, 0}, // #490
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #492
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 130, 2,  2, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #494
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #495
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #496
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 234, 3,  2, 1,  0, 0}, // #497
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 130, 2,  2, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #499
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #500
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #501
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #502
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 206, 3,  2, 1,  0, 0}, // #503
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 201, 5,  3, 1,  0, 0}, // #504
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 201, 5,  3, 1,  0, 0}, // #505
  {DBGFIELD("WriteJump")          1, 0, 0, 206, 3,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #507
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 149, 6,  6, 1,  0, 0}, // #508
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #509
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #510
  {DBGFIELD("IIC_SSE_ALU_F64P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #511
  {DBGFIELD("IIC_SSE_ALU_F64P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #512
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #513
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #514
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 144, 5,  3, 1,  1, 1}, // #515
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 152, 3,  2, 1,  0, 0}, // #516
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 169, 8, 14, 1,  1, 1}, // #517
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 177, 6, 23, 1,  0, 0}, // #518
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 155, 8,  3, 1,  0, 0}, // #519
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 163, 6,  6, 1,  0, 0}, // #520
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 163, 6,  2, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #522
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 141, 8,  7, 1,  0, 0}, // #523
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 149, 6,  8, 1,  0, 0}, // #524
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 141, 8,  5, 1,  0, 0}, // #525
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 149, 6,  6, 1,  0, 0}, // #526
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 141, 8,  5, 1,  0, 0}, // #527
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 149, 6,  6, 1,  0, 0}, // #528
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 342, 9, 24, 2,  0, 0}, // #529
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 141, 8,  5, 1,  1, 1}, // #530
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 141, 8,  7, 1,  1, 1}, // #531
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #532
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #533
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #534
  {DBGFIELD("WriteStore")         1, 0, 0, 234, 3,  2, 1,  0, 0}, // #535
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 163, 6,  6, 1,  0, 0}, // #536
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 228, 6,  3, 2,  0, 0}, // #537
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #538
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 155, 8,  5, 1,  1, 1}, // #539
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #540
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #541
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #542
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #543
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #544
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #545
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #546
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #547
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #548
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 241, 10, 14, 1,  1, 1}, // #549
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 251, 8, 14, 1,  0, 0}, // #550
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 157, 6,  3, 1,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_INTALU_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_INTALU_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_BIT_P_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_BIT_P_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #556
  {DBGFIELD("WriteBlend")         1, 0, 0, 165, 4,  2, 1,  0, 0}, // #557
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 169, 8, 14, 1,  1, 1}, // #558
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 177, 6, 23, 1,  0, 0}, // #559
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 157, 6,  3, 1,  1, 1}, // #560
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 165, 4,  2, 1,  0, 0}, // #561
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 259, 10,  5, 1,  1, 1}, // #562
  {DBGFIELD("WriteCLMul")         1, 0, 0, 269, 8,  5, 1,  0, 0}, // #563
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 155, 8,  5, 1,  1, 1}, // #564
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 155, 8,  3, 1,  0, 0}, // #565
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_INTMUL_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("IIC_SSE_INTMUL_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #568
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 155, 8,  3, 1,  0, 0}, // #569
  {DBGFIELD("WriteVecShift")      1, 0, 0, 222, 6,  2, 1,  0, 0}, // #570
  {DBGFIELD("IIC_SSE_INTSH_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #571
  {DBGFIELD("IIC_SSE_INTSH_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #572
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 259, 10, 14, 1,  1, 1}, // #573
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 269, 8, 23, 1,  0, 0}, // #574
  {DBGFIELD("IIC_SSE_UNPCK")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #575
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 213, 8, 13, 1,  1, 1}, // #576
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 213, 8, 19, 1,  1, 1}, // #577
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 155, 8,  3, 1,  1, 1}, // #578
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffle") 1, 0, 0, 163, 6,  2, 1,  0, 0}, // #579
  {DBGFIELD("IIC_SSE_SQRTPD_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #580
  {DBGFIELD("IIC_SSE_SQRTPD_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #581
  {DBGFIELD("IIC_SSE_SQRTPS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #582
  {DBGFIELD("IIC_SSE_SQRTPS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #583
  {DBGFIELD("IIC_SSE_SQRTSS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #584
  {DBGFIELD("IIC_SSE_SQRTSS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #585
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #586
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 130, 2,  1, 1,  0, 0}, // #587
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #588
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #589
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #590
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 132, 5,  3, 2,  0, 0}, // #591
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 130, 2,  2, 1,  0, 0}, // #592
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 209, 3,  8, 1,  0, 0} // #593
}; // HaswellModelSchedClasses

// {Name, NumMicroOps, BeginGroup, EndGroup, WriteProcResIdx,#, WriteLatencyIdx,#, ReadAdvanceIdx,#}
static const llvm::MCSchedClassDesc SLMModelSchedClasses[] = {
  {DBGFIELD("InvalidSchedClass")  65535, 0, 0,  0, 0,  0, 0,  0, 0},
  {DBGFIELD("IIC_AAA_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #1
  {DBGFIELD("IIC_AAD_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #2
  {DBGFIELD("IIC_AAM_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #3
  {DBGFIELD("IIC_AAS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #4
  {DBGFIELD("IIC_BIN_CARRY_NONMEM_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #5
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #6
  {DBGFIELD("IIC_BIN_CARRY_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 37, 2,  8, 1,  4, 1}, // #7
  {DBGFIELD("IIC_BIN_MEM_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #8
  {DBGFIELD("IIC_BIN_NONMEM_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #9
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #10
  {DBGFIELD("WriteALU")           1, 0, 0, 29, 1,  2, 1,  0, 0}, // #11
  {DBGFIELD("IIC_BIN_MEM_WriteALULd_ReadAfterLd") 1, 0, 0, 37, 2,  8, 1,  4, 1}, // #12
  {DBGFIELD("IIC_SSE_ALU_F64P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #13
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #14
  {DBGFIELD("IIC_SSE_ALU_F32P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #15
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #16
  {DBGFIELD("IIC_SSE_ALU_F64S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #17
  {DBGFIELD("IIC_SSE_ALU_F64S_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #18
  {DBGFIELD("IIC_SSE_ALU_F32S_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #19
  {DBGFIELD("IIC_SSE_ALU_F32S_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #20
  {DBGFIELD("IIC_SSE_ALU_F64P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #21
  {DBGFIELD("IIC_SSE_ALU_F32P_RR_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #22
  {DBGFIELD("WriteFAddLd")        1, 0, 0, 353, 3, 14, 1,  0, 0}, // #23
  {DBGFIELD("WriteFAdd")          1, 0, 0, 68, 2,  6, 1,  0, 0}, // #24
  {DBGFIELD("IIC_AES_WriteAESDecEncLd_ReadAfterLd") 1, 0, 0, 356, 3,  7, 1,  4, 1}, // #25
  {DBGFIELD("IIC_AES_WriteAESDecEnc") 1, 0, 0, 356, 2,  7, 1,  0, 0}, // #26
  {DBGFIELD("IIC_AES_WriteAESIMCLd") 1, 0, 0, 356, 3,  7, 1,  0, 0}, // #27
  {DBGFIELD("IIC_AES_WriteAESIMC") 1, 0, 0, 356, 2,  7, 1,  0, 0}, // #28
  {DBGFIELD("WriteAESKeyGenLd")   1, 0, 0, 356, 3,  7, 1,  0, 0}, // #29
  {DBGFIELD("WriteAESKeyGen")     1, 0, 0, 356, 2,  7, 1,  0, 0}, // #30
  {DBGFIELD("WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #31
  {DBGFIELD("WriteVecLogic")      1, 0, 0, 10, 1,  2, 1,  0, 0}, // #32
  {DBGFIELD("IIC_ARPL_MEM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #33
  {DBGFIELD("IIC_ARPL_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #34
  {DBGFIELD("WriteZero")          1, 0, 0,  0, 0,  2, 1,  0, 0}, // #35
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #36
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteFBlend") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #37
  {DBGFIELD("IIC_ALU_MEM")        0, 0, 0,  0, 0,  0, 0,  0, 0}, // #38
  {DBGFIELD("IIC_ALU_NONMEM")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #39
  {DBGFIELD("IIC_BOUND_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #40
  {DBGFIELD("IIC_BIT_SCAN_MEM_WriteShiftLd") 1, 0, 0, 362, 3,  8, 1,  0, 0}, // #41
  {DBGFIELD("IIC_BIT_SCAN_REG_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #42
  {DBGFIELD("IIC_BSWAP_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #43
  {DBGFIELD("IIC_BT_MI_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #44
  {DBGFIELD("IIC_BT_MR_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #45
  {DBGFIELD("IIC_BT_RI_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #46
  {DBGFIELD("IIC_BT_RR_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #47
  {DBGFIELD("IIC_BTX_MI_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #48
  {DBGFIELD("IIC_BTX_MR_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #49
  {DBGFIELD("IIC_BTX_RI_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #50
  {DBGFIELD("IIC_BTX_RR_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #51
  {DBGFIELD("IIC_CALL_MEM_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #52
  {DBGFIELD("IIC_CALL_RI_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #53
  {DBGFIELD("IIC_CALL_MEM_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #54
  {DBGFIELD("IIC_CBW")            0, 0, 0,  0, 0,  0, 0,  0, 0}, // #55
  {DBGFIELD("IIC_CLC_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #56
  {DBGFIELD("IIC_CLD_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #57
  {DBGFIELD("IIC_SSE_PREFETCH_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #58
  {DBGFIELD("IIC_CLI_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #59
  {DBGFIELD("IIC_CLTS_WriteALU")  1, 0, 0, 29, 1,  2, 1,  0, 0}, // #60
  {DBGFIELD("IIC_CMC_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #61
  {DBGFIELD("IIC_CMOV16_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 37, 2,  8, 1,  4, 1}, // #62
  {DBGFIELD("IIC_CMOV16_RR_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #63
  {DBGFIELD("IIC_CMOV32_RM_WriteALULd_ReadAfterLd") 1, 0, 0, 37, 2,  8, 1,  4, 1}, // #64
  {DBGFIELD("IIC_CMOV32_RR_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #65
  {DBGFIELD("IIC_CMPS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #66
  {DBGFIELD("IIC_CMPXCHG_16B_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #67
  {DBGFIELD("IIC_CMPXCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #68
  {DBGFIELD("IIC_CMPXCHG_REG_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #69
  {DBGFIELD("IIC_CMPXCHG_8B_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #70
  {DBGFIELD("IIC_CMPXCHG_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #71
  {DBGFIELD("IIC_CMPXCHG_REG8_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #72
  {DBGFIELD("IIC_SSE_COMIS_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #73
  {DBGFIELD("IIC_SSE_COMIS_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #74
  {DBGFIELD("IIC_FCOMI_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #75
  {DBGFIELD("IIC_CPUID_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #76
  {DBGFIELD("IIC_CRC32_MEM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #77
  {DBGFIELD("IIC_CRC32_REG_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #78
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtI2FLd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #79
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtI2F") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #80
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtI2FLd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #81
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtI2F") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #82
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #83
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #84
  {DBGFIELD("IIC_SSE_CVT_PD_RM_WriteCvtF2FLd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #85
  {DBGFIELD("IIC_SSE_CVT_PD_RR_WriteCvtF2F") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #86
  {DBGFIELD("IIC_SSE_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #87
  {DBGFIELD("IIC_SSE_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #88
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #89
  {DBGFIELD("IIC_SSE_CVT_SD2SI_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #90
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #91
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtF2F") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #92
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #93
  {DBGFIELD("IIC_SSE_CVT_Scalar_RR_WriteCvtI2F") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #94
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #95
  {DBGFIELD("IIC_SSE_CVT_SS2SI64_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #96
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #97
  {DBGFIELD("IIC_SSE_CVT_SS2SI32_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #98
  {DBGFIELD("IIC_DAA_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #99
  {DBGFIELD("IIC_DAS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #100
  {DBGFIELD("IIC_UNARY_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #101
  {DBGFIELD("IIC_UNARY_REG_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #102
  {DBGFIELD("IIC_DIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #103
  {DBGFIELD("IIC_DIV16_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #104
  {DBGFIELD("IIC_DIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #105
  {DBGFIELD("IIC_DIV32_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #106
  {DBGFIELD("IIC_DIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #107
  {DBGFIELD("IIC_DIV64_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #108
  {DBGFIELD("IIC_DIV8_MEM_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #109
  {DBGFIELD("IIC_DIV8_REG_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #110
  {DBGFIELD("IIC_SSE_DIV_F64P_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 370, 4, 29, 1,  4, 1}, // #111
  {DBGFIELD("IIC_SSE_DIV_F64P_RR_WriteFDiv") 1, 0, 0, 374, 3, 30, 1,  0, 0}, // #112
  {DBGFIELD("IIC_SSE_DIV_F32P_RR_WriteFDiv") 1, 0, 0, 374, 3, 30, 1,  0, 0}, // #113
  {DBGFIELD("WriteFDivLd")        1, 0, 0, 370, 4, 29, 1,  0, 0}, // #114
  {DBGFIELD("WriteFDiv")          1, 0, 0, 374, 3, 30, 1,  0, 0}, // #115
  {DBGFIELD("IIC_SSE_DIV_F64S_RM_WriteFDivLd_ReadAfterLd") 1, 0, 0, 370, 4, 29, 1,  4, 1}, // #116
  {DBGFIELD("IIC_SSE_DIV_F64S_RR_WriteFDiv") 1, 0, 0, 374, 3, 30, 1,  0, 0}, // #117
  {DBGFIELD("IIC_SSE_DIV_F32S_RR_WriteFDiv") 1, 0, 0, 374, 3, 30, 1,  0, 0}, // #118
  {DBGFIELD("IIC_SSE_DPPD_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #119
  {DBGFIELD("IIC_SSE_DPPD_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #120
  {DBGFIELD("IIC_SSE_DPPS_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #121
  {DBGFIELD("IIC_RET_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #122
  {DBGFIELD("WriteSystem")        1, 0, 0, 351, 2,  1, 1,  0, 0}, // #123
  {DBGFIELD("IIC_ENTER_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #124
  {DBGFIELD("IIC_SSE_EXTRACTPS_RM_WriteFBlendLd_WriteRMW") 2, 0, 0, 377, 3, 27, 2,  0, 0}, // #125
  {DBGFIELD("IIC_SSE_EXTRACTPS_RR_WriteFBlend") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #126
  {DBGFIELD("IIC_F2XM1_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #127
  {DBGFIELD("IIC_CALL_FAR_PTR_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #128
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #129
  {DBGFIELD("IIC_CALL_FAR_MEM_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #130
  {DBGFIELD("IIC_JMP_FAR_PTR_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #131
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #132
  {DBGFIELD("IIC_JMP_FAR_MEM_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #133
  {DBGFIELD("IIC_FCOMPP_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #134
  {DBGFIELD("IIC_FPSTP_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #135
  {DBGFIELD("IIC_FFREE_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #136
  {DBGFIELD("IIC_FLDCW_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #137
  {DBGFIELD("IIC_FLDL_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #138
  {DBGFIELD("IIC_FNCLEX_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #139
  {DBGFIELD("IIC_FNINIT_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #140
  {DBGFIELD("IIC_FNOP_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #141
  {DBGFIELD("IIC_FNSTCW_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #142
  {DBGFIELD("IIC_FNSTSW_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #143
  {DBGFIELD("IIC_FPATAN_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #144
  {DBGFIELD("IIC_FPREM_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #145
  {DBGFIELD("IIC_FPREM1_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #146
  {DBGFIELD("IIC_FPTAN_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #147
  {DBGFIELD("IIC_FRNDINT_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #148
  {DBGFIELD("IIC_FSCALE_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #149
  {DBGFIELD("IIC_FSINCOS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #150
  {DBGFIELD("IIC_FXAM_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #151
  {DBGFIELD("IIC_FXRSTOR_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #152
  {DBGFIELD("IIC_FXSAVE_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #153
  {DBGFIELD("IIC_FXTRACT_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #154
  {DBGFIELD("IIC_FYL2X_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #155
  {DBGFIELD("IIC_FYL2XP1_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #156
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #157
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #158
  {DBGFIELD("IIC_SSE_MOVA_P_RM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #159
  {DBGFIELD("IIC_SSE_HADDSUB_RM_WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #160
  {DBGFIELD("IIC_SSE_HADDSUB_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #161
  {DBGFIELD("IIC_HLT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #162
  {DBGFIELD("IIC_IDIV16_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #163
  {DBGFIELD("IIC_IDIV16_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #164
  {DBGFIELD("IIC_IDIV32_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #165
  {DBGFIELD("IIC_IDIV32_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #166
  {DBGFIELD("IIC_IDIV64_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #167
  {DBGFIELD("IIC_IDIV64_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #168
  {DBGFIELD("IIC_IDIV8_WriteIDivLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 365, 3,  9, 1,  5, 2}, // #169
  {DBGFIELD("IIC_IDIV8_WriteIDiv") 1, 0, 0, 368, 2, 10, 1,  0, 0}, // #170
  {DBGFIELD("IIC_FILD_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #171
  {DBGFIELD("IIC_IMUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #172
  {DBGFIELD("IIC_IMUL16_RR_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #173
  {DBGFIELD("IIC_IMUL16_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  4, 1}, // #174
  {DBGFIELD("IIC_IMUL16_RMI_WriteIMulLd") 1, 0, 0, 36, 3, 14, 1,  0, 0}, // #175
  {DBGFIELD("IIC_IMUL16_RRI_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #176
  {DBGFIELD("IIC_IMUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #177
  {DBGFIELD("IIC_IMUL32_RR_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #178
  {DBGFIELD("IIC_IMUL32_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  4, 1}, // #179
  {DBGFIELD("IIC_IMUL32_RMI_WriteIMulLd") 1, 0, 0, 36, 3, 14, 1,  0, 0}, // #180
  {DBGFIELD("IIC_IMUL32_RRI_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #181
  {DBGFIELD("IIC_IMUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #182
  {DBGFIELD("IIC_IMUL64_RR_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #183
  {DBGFIELD("IIC_IMUL64_RM_WriteIMulLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  4, 1}, // #184
  {DBGFIELD("IIC_IMUL64_RMI_WriteIMulLd") 1, 0, 0, 36, 3, 14, 1,  0, 0}, // #185
  {DBGFIELD("IIC_IMUL64_RRI_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #186
  {DBGFIELD("IIC_IMUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #187
  {DBGFIELD("IIC_IMUL8_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #188
  {DBGFIELD("IIC_IN_RI_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #189
  {DBGFIELD("IIC_IN_RR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #190
  {DBGFIELD("IIC_INS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #191
  {DBGFIELD("IIC_SSE_INSERTPS_RM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #192
  {DBGFIELD("IIC_SSE_INSERTPS_RR_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #193
  {DBGFIELD("IIC_INT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #194
  {DBGFIELD("IIC_INT3_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #195
  {DBGFIELD("IIC_INVD_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #196
  {DBGFIELD("IIC_INVLPG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #197
  {DBGFIELD("IIC_IRET_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #198
  {DBGFIELD("IIC_FST_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #199
  {DBGFIELD("IIC_FIST_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #200
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 2,  5, 1,  4, 1}, // #201
  {DBGFIELD("IIC_SSE_CVT_Scalar_RM_WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 2,  5, 1,  4, 1}, // #202
  {DBGFIELD("WriteLoad")          1, 0, 0, 11, 1,  6, 1,  0, 0}, // #203
  {DBGFIELD("IIC_Jcc_WriteJump")  1, 0, 0, 36, 2,  2, 1,  0, 0}, // #204
  {DBGFIELD("IIC_JCXZ_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #205
  {DBGFIELD("IIC_JMP_MEM_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #206
  {DBGFIELD("IIC_JMP_REG_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #207
  {DBGFIELD("IIC_JMP_REL_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #208
  {DBGFIELD("IIC_AHF_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #209
  {DBGFIELD("IIC_LAR_RM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #210
  {DBGFIELD("IIC_LAR_RR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #211
  {DBGFIELD("IIC_CMPX_LOCK_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #212
  {DBGFIELD("IIC_CMPX_LOCK_16B_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #213
  {DBGFIELD("IIC_CMPX_LOCK_8_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #214
  {DBGFIELD("IIC_CMPX_LOCK_8B_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #215
  {DBGFIELD("IIC_SSE_LDDQU_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #216
  {DBGFIELD("IIC_SSE_LDMXCSR_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #217
  {DBGFIELD("IIC_LXS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #218
  {DBGFIELD("IIC_FLDZ_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #219
  {DBGFIELD("IIC_FIST_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #220
  {DBGFIELD("IIC_FLD_WriteLoad")  1, 0, 0, 11, 1,  6, 1,  0, 0}, // #221
  {DBGFIELD("IIC_FLD80_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #222
  {DBGFIELD("IIC_FLD_WriteMove")  1, 0, 0, 29, 1,  2, 1,  0, 0}, // #223
  {DBGFIELD("IIC_LEA_16_WriteLEA") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #224
  {DBGFIELD("IIC_LEA_WriteLEA")   1, 0, 0, 36, 2,  2, 1,  0, 0}, // #225
  {DBGFIELD("IIC_LEAVE_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #226
  {DBGFIELD("IIC_SSE_LFENCE_WriteFence") 1, 0, 0, 11, 1,  2, 1,  0, 0}, // #227
  {DBGFIELD("IIC_LGDT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #228
  {DBGFIELD("IIC_LIDT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #229
  {DBGFIELD("IIC_LLDT_MEM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #230
  {DBGFIELD("IIC_LLDT_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #231
  {DBGFIELD("IIC_LMSW_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #232
  {DBGFIELD("IIC_LMSW_MEM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #233
  {DBGFIELD("IIC_ALU_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #234
  {DBGFIELD("IIC_ALU_NONMEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #235
  {DBGFIELD("IIC_LODS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #236
  {DBGFIELD("IIC_LOOP_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #237
  {DBGFIELD("IIC_LOOPE_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #238
  {DBGFIELD("IIC_LOOPNE_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #239
  {DBGFIELD("IIC_RET_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #240
  {DBGFIELD("IIC_LSL_RM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #241
  {DBGFIELD("IIC_LSL_RR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #242
  {DBGFIELD("IIC_LTR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #243
  {DBGFIELD("IIC_XADD_LOCK_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #244
  {DBGFIELD("IIC_XADD_LOCK_MEM8_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #245
  {DBGFIELD("IIC_SSE_MASKMOV_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #246
  {DBGFIELD("IIC_SSE_MFENCE_WriteFence") 1, 0, 0, 11, 1,  2, 1,  0, 0}, // #247
  {DBGFIELD("IIC_MMX_CVT_PD_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #248
  {DBGFIELD("IIC_MMX_CVT_PD_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #249
  {DBGFIELD("IIC_MMX_CVT_PS_RM_WriteCvtF2ILd") 1, 0, 0, 10, 2,  5, 1,  0, 0}, // #250
  {DBGFIELD("IIC_MMX_CVT_PS_RR_WriteCvtF2I") 1, 0, 0, 10, 1,  8, 1,  0, 0}, // #251
  {DBGFIELD("IIC_MMX_EMMS")       0, 0, 0,  0, 0,  0, 0,  0, 0}, // #252
  {DBGFIELD("IIC_MMX_MASKMOV_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #253
  {DBGFIELD("IIC_MMX_MOV_REG_MM_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #254
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #255
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #256
  {DBGFIELD("IIC_MMX_MOV_MM_RM_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #257
  {DBGFIELD("IIC_MMX_MOVQ_RR_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #258
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #259
  {DBGFIELD("WriteMove")          1, 0, 0, 29, 1,  2, 1,  0, 0}, // #260
  {DBGFIELD("IIC_MMX_MOVQ_RM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #261
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd") 1, 0, 0, 10, 2,  8, 1,  0, 0}, // #262
  {DBGFIELD("IIC_MMX_ALU_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #263
  {DBGFIELD("IIC_MMX_PCK_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #264
  {DBGFIELD("IIC_MMX_PCK_RR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #265
  {DBGFIELD("IIC_MMX_ALU_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #266
  {DBGFIELD("IIC_MMX_ALUQ_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #267
  {DBGFIELD("IIC_MMX_ALUQ_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #268
  {DBGFIELD("IIC_MMX_MISC_FUNC_REG_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #269
  {DBGFIELD("IIC_MMX_MISC_FUNC_MEM_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #270
  {DBGFIELD("IIC_MMX_PEXTR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #271
  {DBGFIELD("IIC_MMX_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #272
  {DBGFIELD("IIC_MMX_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #273
  {DBGFIELD("IIC_MMX_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #274
  {DBGFIELD("IIC_MMX_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #275
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #276
  {DBGFIELD("IIC_MMX_PINSRW_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #277
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 359, 3,  5, 1,  4, 1}, // #278
  {DBGFIELD("IIC_MMX_PMUL_WriteVecIMul") 1, 0, 0, 351, 2,  8, 1,  0, 0}, // #279
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #280
  {DBGFIELD("IIC_MMX_PSADBW_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #281
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #282
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #283
  {DBGFIELD("IIC_MMX_PSHUF_WriteShuffleLd") 1, 0, 0, 359, 3,  8, 1,  0, 0}, // #284
  {DBGFIELD("IIC_MMX_SHIFT_RI_WriteVecShift") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #285
  {DBGFIELD("IIC_MMX_SHIFT_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #286
  {DBGFIELD("IIC_MMX_SHIFT_RR_WriteVecShift") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #287
  {DBGFIELD("IIC_MMX_UNPCK_H_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #288
  {DBGFIELD("IIC_MMX_UNPCK_H_RR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #289
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #290
  {DBGFIELD("IIC_MMX_UNPCK_L_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #291
  {DBGFIELD("IIC_SSE_MONITOR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #292
  {DBGFIELD("IIC_MOV_MEM_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #293
  {DBGFIELD("IIC_MOV_MEM_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #294
  {DBGFIELD("IIC_MOV_MEM_SR_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #295
  {DBGFIELD("IIC_MOV_WriteMove")  1, 0, 0, 29, 1,  2, 1,  0, 0}, // #296
  {DBGFIELD("IIC_MOV_MEM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #297
  {DBGFIELD("IIC_MOV_REG_SR_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #298
  {DBGFIELD("IIC_MOV_SR_MEM_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #299
  {DBGFIELD("IIC_MOV_SR_REG_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #300
  {DBGFIELD("IIC_MOV_CR_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #301
  {DBGFIELD("IIC_MOV_DR_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #302
  {DBGFIELD("IIC_ALU_NONMEM_WriteZero") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #303
  {DBGFIELD("IIC_MOV_REG_CR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #304
  {DBGFIELD("IIC_MOV_REG_DR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #305
  {DBGFIELD("IIC_ALU_NONMEM_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #306
  {DBGFIELD("IIC_SSE_MOVDQ_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #307
  {DBGFIELD("IIC_SSE_MOVDQ_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #308
  {DBGFIELD("IIC_SSE_MOVA_P_MR_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #309
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #310
  {DBGFIELD("IIC_SSE_MOVA_P_RR_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #311
  {DBGFIELD("IIC_MOVBE_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #312
  {DBGFIELD("IIC_MOVBE_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #313
  {DBGFIELD("IIC_SSE_MOV_LH_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #314
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #315
  {DBGFIELD("IIC_SSE_MOVU_P_MR_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #316
  {DBGFIELD("IIC_SSE_MOVU_P_RM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #317
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #318
  {DBGFIELD("IIC_SSE_MOV_LH_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #319
  {DBGFIELD("IIC_SSE_MOV_LH_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #320
  {DBGFIELD("IIC_SSE_MOVMSK_WriteVecLogic") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #321
  {DBGFIELD("IIC_SSE_MOVNT_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #322
  {DBGFIELD("IIC_SSE_MOVDQ_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #323
  {DBGFIELD("IIC_SSE_MOVD_ToGP_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #324
  {DBGFIELD("IIC_SSE_MOVQ_RR_WriteVecLogic") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #325
  {DBGFIELD("IIC_MOVS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #326
  {DBGFIELD("IIC_SSE_MOV_S_MR_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #327
  {DBGFIELD("IIC_SSE_MOV_S_RM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #328
  {DBGFIELD("IIC_SSE_MOV_S_RR_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #329
  {DBGFIELD("IIC_MOVSX_R16_M8_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #330
  {DBGFIELD("IIC_MOVSX_R16_R8_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #331
  {DBGFIELD("IIC_MOVSX_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #332
  {DBGFIELD("IIC_MOVSX_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #333
  {DBGFIELD("IIC_SSE_MOVU_P_RR_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #334
  {DBGFIELD("IIC_SSE_MOVDQ_WriteVecLogicLd") 1, 0, 0, 10, 2,  8, 1,  0, 0}, // #335
  {DBGFIELD("IIC_MOVZX_R16_M8_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #336
  {DBGFIELD("IIC_MOVZX_R16_R8_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #337
  {DBGFIELD("IIC_MOVZX_WriteALULd") 1, 0, 0, 37, 2,  8, 1,  0, 0}, // #338
  {DBGFIELD("IIC_MOVZX_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #339
  {DBGFIELD("IIC_SSE_MPSADBW_RM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 359, 3, 26, 1,  4, 1}, // #340
  {DBGFIELD("IIC_SSE_MPSADBW_RR_WriteMPSAD") 1, 0, 0, 351, 2,  5, 1,  0, 0}, // #341
  {DBGFIELD("IIC_MUL16_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #342
  {DBGFIELD("IIC_MUL16_REG_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #343
  {DBGFIELD("IIC_MUL32_MEM_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #344
  {DBGFIELD("IIC_MUL32_REG_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #345
  {DBGFIELD("IIC_MUL64_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #346
  {DBGFIELD("IIC_MUL64_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #347
  {DBGFIELD("IIC_MUL8_WriteIMulLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd_ReadAfterLd") 1, 0, 0, 36, 3, 14, 1,  5, 2}, // #348
  {DBGFIELD("IIC_MUL8_WriteIMul") 1, 0, 0, 36, 2,  6, 1,  0, 0}, // #349
  {DBGFIELD("IIC_SSE_MUL_F64P_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 380, 4,  7, 1,  4, 1}, // #350
  {DBGFIELD("IIC_SSE_MUL_F64P_RR_WriteFMul") 1, 0, 0, 384, 3,  3, 1,  0, 0}, // #351
  {DBGFIELD("IIC_SSE_MUL_F32P_RR_WriteFMul") 1, 0, 0, 384, 3,  3, 1,  0, 0}, // #352
  {DBGFIELD("IIC_SSE_MUL_F64S_RM_WriteFMulLd_ReadAfterLd") 1, 0, 0, 380, 4,  7, 1,  4, 1}, // #353
  {DBGFIELD("IIC_SSE_MUL_F64S_RR_WriteFMul") 1, 0, 0, 384, 3,  3, 1,  0, 0}, // #354
  {DBGFIELD("IIC_SSE_MUL_F32S_RR_WriteFMul") 1, 0, 0, 384, 3,  3, 1,  0, 0}, // #355
  {DBGFIELD("IIC_MUL8_WriteIMulLd_WriteIMulH") 2, 0, 0, 387, 5, 31, 2,  0, 0}, // #356
  {DBGFIELD("IIC_MUL8_WriteIMul_WriteIMulH") 2, 0, 0, 387, 4, 33, 2,  0, 0}, // #357
  {DBGFIELD("WriteFMulLd")        1, 0, 0, 380, 4,  7, 1,  0, 0}, // #358
  {DBGFIELD("WriteFMul")          1, 0, 0, 384, 3,  3, 1,  0, 0}, // #359
  {DBGFIELD("IIC_SSE_MWAIT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #360
  {DBGFIELD("IIC_NOP_WriteZero")  1, 0, 0,  0, 0,  2, 1,  0, 0}, // #361
  {DBGFIELD("IIC_OUT_IR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #362
  {DBGFIELD("IIC_OUT_RR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #363
  {DBGFIELD("IIC_OUTS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #364
  {DBGFIELD("IIC_SSE_PABS_RM_WriteVecALULd") 1, 0, 0, 10, 2,  8, 1,  0, 0}, // #365
  {DBGFIELD("IIC_SSE_PABS_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #366
  {DBGFIELD("WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #367
  {DBGFIELD("WriteShuffle")       1, 0, 0, 351, 2,  2, 1,  0, 0}, // #368
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #369
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #370
  {DBGFIELD("IIC_SSE_INTALUQ_P_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #371
  {DBGFIELD("IIC_SSE_INTALUQ_P_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #372
  {DBGFIELD("IIC_SSE_PALIGNRM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #373
  {DBGFIELD("IIC_SSE_PALIGNRR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #374
  {DBGFIELD("IIC_SSE_BIT_P_RM_WriteVecLogicLd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #375
  {DBGFIELD("IIC_SSE_BIT_P_RR_WriteVecLogic") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #376
  {DBGFIELD("IIC_SSE_PAUSE_WriteNop") 1, 0, 0,  0, 0,  2, 1,  0, 0}, // #377
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RM_WriteCLMulLd_ReadAfterLd") 1, 0, 0, 392, 3, 26, 1,  4, 1}, // #378
  {DBGFIELD("IIC_SSE_PCLMULQDQ_RR_WriteCLMul") 1, 0, 0, 392, 2, 26, 1,  0, 0}, // #379
  {DBGFIELD("WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #380
  {DBGFIELD("WriteVecALU")        1, 0, 0, 10, 1,  2, 1,  0, 0}, // #381
  {DBGFIELD("WritePCmpEStrILd_ReadAfterLd") 1, 0, 0, 395, 3, 35, 1,  4, 1}, // #382
  {DBGFIELD("WritePCmpEStrI")     1, 0, 0, 395, 2, 35, 1,  0, 0}, // #383
  {DBGFIELD("WritePCmpEStrMLd_ReadAfterLd") 1, 0, 0, 398, 3, 36, 1,  4, 1}, // #384
  {DBGFIELD("WritePCmpEStrM")     1, 0, 0, 398, 2, 36, 1,  0, 0}, // #385
  {DBGFIELD("WritePCmpIStrILd_ReadAfterLd") 1, 0, 0, 398, 3, 36, 1,  4, 1}, // #386
  {DBGFIELD("WritePCmpIStrI")     1, 0, 0, 398, 2, 36, 1,  0, 0}, // #387
  {DBGFIELD("WritePCmpIStrMLd_ReadAfterLd") 1, 0, 0, 401, 3, 37, 1,  4, 1}, // #388
  {DBGFIELD("WritePCmpIStrM")     1, 0, 0, 401, 2, 37, 1,  0, 0}, // #389
  {DBGFIELD("WriteShuffleLd_WriteRMW") 2, 0, 0, 377, 3, 27, 2,  0, 0}, // #390
  {DBGFIELD("IIC_SSE_PEXTRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #391
  {DBGFIELD("IIC_SSE_PHADDSUBD_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #392
  {DBGFIELD("IIC_SSE_PHADDSUBD_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #393
  {DBGFIELD("IIC_SSE_PHADDSUBW_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #394
  {DBGFIELD("IIC_SSE_PHADDSUBW_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #395
  {DBGFIELD("WriteVecIMulLd")     1, 0, 0, 359, 3,  5, 1,  0, 0}, // #396
  {DBGFIELD("WriteVecIMul")       1, 0, 0, 351, 2,  8, 1,  0, 0}, // #397
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #398
  {DBGFIELD("IIC_SSE_PINSRW_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #399
  {DBGFIELD("WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 359, 3,  5, 1,  4, 1}, // #400
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 359, 3,  5, 1,  4, 1}, // #401
  {DBGFIELD("IIC_SSE_PMADD_WriteVecIMul") 1, 0, 0, 351, 2,  8, 1,  0, 0}, // #402
  {DBGFIELD("IIC_SSE_INTALU_P_RM_WriteShuffleLd") 1, 0, 0, 359, 3,  8, 1,  0, 0}, // #403
  {DBGFIELD("IIC_SSE_INTALU_P_RR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #404
  {DBGFIELD("WriteShuffleLd")     1, 0, 0, 359, 3,  8, 1,  0, 0}, // #405
  {DBGFIELD("IIC_SSE_INTMUL_P_RM_WriteVecIMulLd_ReadAfterLd") 1, 0, 0, 359, 3,  5, 1,  4, 1}, // #406
  {DBGFIELD("IIC_SSE_INTMUL_P_RR_WriteVecIMul") 1, 0, 0, 351, 2,  8, 1,  0, 0}, // #407
  {DBGFIELD("WriteFAddLd_ReadAfterLd") 1, 0, 0, 353, 3, 14, 1,  4, 1}, // #408
  {DBGFIELD("IIC_POP_REG16_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #409
  {DBGFIELD("IIC_POP_MEM_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #410
  {DBGFIELD("IIC_POP_REG_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #411
  {DBGFIELD("IIC_POP_A_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #412
  {DBGFIELD("IIC_SSE_POPCNT_RM_WriteFAddLd") 1, 0, 0, 353, 3, 14, 1,  0, 0}, // #413
  {DBGFIELD("IIC_SSE_POPCNT_RR_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #414
  {DBGFIELD("IIC_POP_SR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #415
  {DBGFIELD("IIC_POP_F_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #416
  {DBGFIELD("IIC_POP_FD_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0}, // #417
  {DBGFIELD("IIC_POP_SR_SS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #418
  {DBGFIELD("IIC_SSE_PSHUFB_RM_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #419
  {DBGFIELD("IIC_SSE_PSHUFB_RR_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #420
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #421
  {DBGFIELD("IIC_SSE_PSHUF_RI_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #422
  {DBGFIELD("IIC_SSE_PSIGN_RM_WriteVecALULd_ReadAfterLd") 1, 0, 0, 10, 2,  8, 1,  4, 1}, // #423
  {DBGFIELD("IIC_SSE_PSIGN_RR_WriteVecALU") 1, 0, 0, 10, 1,  2, 1,  0, 0}, // #424
  {DBGFIELD("IIC_SSE_INTSHDQ_P_RI_WriteVecShift") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #425
  {DBGFIELD("IIC_SSE_INTSH_P_RI_WriteVecShift") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #426
  {DBGFIELD("IIC_SSE_INTSH_P_RM_WriteVecShiftLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #427
  {DBGFIELD("IIC_SSE_INTSH_P_RR_WriteVecShift") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #428
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #429
  {DBGFIELD("IIC_SSE_UNPCK_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #430
  {DBGFIELD("IIC_PUSH_IMM_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #431
  {DBGFIELD("IIC_PUSH_REG_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #432
  {DBGFIELD("IIC_PUSH_MEM_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #433
  {DBGFIELD("IIC_PUSH_A_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #434
  {DBGFIELD("IIC_PUSH_SR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #435
  {DBGFIELD("IIC_PUSH_CS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #436
  {DBGFIELD("IIC_PUSH_F_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #437
  {DBGFIELD("IIC_SR_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #438
  {DBGFIELD("IIC_SR_WriteShift")  1, 0, 0, 60, 2,  2, 1,  0, 0}, // #439
  {DBGFIELD("IIC_SSE_RCPP_RM_WriteFRcpLd") 1, 0, 0, 359, 3,  7, 1,  0, 0}, // #440
  {DBGFIELD("IIC_SSE_RCPP_RR_WriteFRcp") 1, 0, 0, 351, 2,  3, 1,  0, 0}, // #441
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd") 1, 0, 0, 359, 3,  7, 1,  0, 0}, // #442
  {DBGFIELD("IIC_SSE_RCPS_RM_WriteFRcpLd_ReadAfterLd") 1, 0, 0, 359, 3,  7, 1,  4, 1}, // #443
  {DBGFIELD("WriteFRcp")          1, 0, 0, 351, 2,  3, 1,  0, 0}, // #444
  {DBGFIELD("IIC_SSE_RCPS_RR_WriteFRcp") 1, 0, 0, 351, 2,  3, 1,  0, 0}, // #445
  {DBGFIELD("IIC_RDMSR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #446
  {DBGFIELD("IIC_RDPMC_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #447
  {DBGFIELD("IIC_RDTSC_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #448
  {DBGFIELD("IIC_REP_MOVS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #449
  {DBGFIELD("IIC_REP_STOS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #450
  {DBGFIELD("IIC_RET_IMM_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #451
  {DBGFIELD("WriteShiftLd")       1, 0, 0, 362, 3,  8, 1,  0, 0}, // #452
  {DBGFIELD("WriteShift")         1, 0, 0, 60, 2,  2, 1,  0, 0}, // #453
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAddLd") 1, 0, 0, 353, 3, 14, 1,  0, 0}, // #454
  {DBGFIELD("IIC_SSE_ROUNDPS_REG_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #455
  {DBGFIELD("IIC_SSE_ROUNDPS_MEM_WriteFAddLd") 1, 0, 0, 353, 3, 14, 1,  0, 0}, // #456
  {DBGFIELD("IIC_RSM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #457
  {DBGFIELD("IIC_SSE_SQRTPS_RM_WriteFSqrtLd") 1, 0, 0, 359, 3, 38, 1,  0, 0}, // #458
  {DBGFIELD("IIC_SSE_SQRTPS_RR_WriteFSqrt") 1, 0, 0, 351, 2, 20, 1,  0, 0}, // #459
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd") 1, 0, 0, 359, 3, 38, 1,  0, 0}, // #460
  {DBGFIELD("IIC_SSE_SQRTSS_RM_WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 359, 3, 38, 1,  4, 1}, // #461
  {DBGFIELD("WriteFSqrt")         1, 0, 0, 351, 2, 20, 1,  0, 0}, // #462
  {DBGFIELD("IIC_SSE_SQRTSS_RR_WriteFSqrt") 1, 0, 0, 351, 2, 20, 1,  0, 0}, // #463
  {DBGFIELD("WriteShiftLd_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadDefault_ReadAfterLd") 1, 0, 0, 362, 3,  8, 1,  5, 1}, // #464
  {DBGFIELD("IIC_SCAS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #465
  {DBGFIELD("IIC_SET_M_WriteALU_WriteStore") 2, 0, 0, 100, 2, 21, 2,  0, 0}, // #466
  {DBGFIELD("IIC_SET_R_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #467
  {DBGFIELD("IIC_SSE_SFENCE_WriteFence") 1, 0, 0, 11, 1,  2, 1,  0, 0}, // #468
  {DBGFIELD("IIC_SGDT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #469
  {DBGFIELD("IIC_SHD16_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #470
  {DBGFIELD("IIC_SHD16_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #471
  {DBGFIELD("IIC_SHD16_REG_CL_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #472
  {DBGFIELD("IIC_SHD16_REG_IM_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #473
  {DBGFIELD("IIC_SHD32_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #474
  {DBGFIELD("IIC_SHD32_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #475
  {DBGFIELD("IIC_SHD32_REG_CL_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #476
  {DBGFIELD("IIC_SHD32_REG_IM_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #477
  {DBGFIELD("IIC_SHD64_MEM_CL_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #478
  {DBGFIELD("IIC_SHD64_MEM_IM_WriteShiftLd_WriteRMW") 2, 0, 0, 404, 3, 27, 2,  0, 0}, // #479
  {DBGFIELD("IIC_SHD64_REG_CL_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #480
  {DBGFIELD("IIC_SHD64_REG_IM_WriteShift") 1, 0, 0, 60, 2,  2, 1,  0, 0}, // #481
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #482
  {DBGFIELD("IIC_SSE_SHUFP_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #483
  {DBGFIELD("IIC_SIDT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #484
  {DBGFIELD("IIC_SLDT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #485
  {DBGFIELD("IIC_SMSW_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #486
  {DBGFIELD("IIC_SSE_SQRTPD_RM_WriteFSqrtLd") 1, 0, 0, 359, 3, 38, 1,  0, 0}, // #487
  {DBGFIELD("IIC_SSE_SQRTPD_RR_WriteFSqrt") 1, 0, 0, 351, 2, 20, 1,  0, 0}, // #488
  {DBGFIELD("IIC_SSE_SQRTSD_RM_WriteFSqrtLd") 1, 0, 0, 359, 3, 38, 1,  0, 0}, // #489
  {DBGFIELD("IIC_SSE_SQRTSD_RR_WriteFSqrt") 1, 0, 0, 351, 2, 20, 1,  0, 0}, // #490
  {DBGFIELD("IIC_STC_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #491
  {DBGFIELD("IIC_STD_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #492
  {DBGFIELD("IIC_STI_WriteALU")   1, 0, 0, 29, 1,  2, 1,  0, 0}, // #493
  {DBGFIELD("IIC_SSE_STMXCSR_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #494
  {DBGFIELD("IIC_STOS_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #495
  {DBGFIELD("IIC_STR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #496
  {DBGFIELD("IIC_FST80_WriteStore") 1, 0, 0, 37, 2,  2, 1,  0, 0}, // #497
  {DBGFIELD("IIC_FST_WriteMove")  1, 0, 0, 29, 1,  2, 1,  0, 0}, // #498
  {DBGFIELD("IIC_SWAPGS_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #499
  {DBGFIELD("IIC_SYSCALL_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #500
  {DBGFIELD("IIC_SYS_ENTER_EXIT_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #501
  {DBGFIELD("IIC_JMP_REL_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #502
  {DBGFIELD("IIC_JMP_MEM_WriteJump") 1, 0, 0, 36, 2,  2, 1,  0, 0}, // #503
  {DBGFIELD("IIC_JMP_REG_WriteJumpLd") 1, 0, 0, 36, 3,  8, 1,  0, 0}, // #504
  {DBGFIELD("WriteJumpLd")        1, 0, 0, 36, 3,  8, 1,  0, 0}, // #505
  {DBGFIELD("WriteJump")          1, 0, 0, 36, 2,  2, 1,  0, 0}, // #506
  {DBGFIELD("IIC_FUCOMI_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #507
  {DBGFIELD("IIC_FUCOM_WriteFAdd") 1, 0, 0, 68, 2,  6, 1,  0, 0}, // #508
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #509
  {DBGFIELD("IIC_SSE_UNPCK_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #510
  {DBGFIELD("IIC_SSE_ALU_F64P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #511
  {DBGFIELD("IIC_SSE_ALU_F64P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #512
  {DBGFIELD("IIC_SSE_ALU_F32P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #513
  {DBGFIELD("IIC_SSE_ALU_F32P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #514
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #515
  {DBGFIELD("IIC_ALU_NONMEM_WriteFBlend") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #516
  {DBGFIELD("WriteFVarBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #517
  {DBGFIELD("WriteFVarBlend")     1, 0, 0, 351, 2,  2, 1,  0, 0}, // #518
  {DBGFIELD("WriteFShuffleLd")    1, 0, 0, 359, 3,  8, 1,  0, 0}, // #519
  {DBGFIELD("WriteFShuffle256")   1, 0, 0, 351, 2,  2, 1,  0, 0}, // #520
  {DBGFIELD("WriteFShuffle")      1, 0, 0, 351, 2,  2, 1,  0, 0}, // #521
  {DBGFIELD("IIC_SSE_ALU_F32S_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #522
  {DBGFIELD("WriteCvtI2FLd")      1, 0, 0, 10, 2,  5, 1,  0, 0}, // #523
  {DBGFIELD("WriteCvtI2F")        1, 0, 0, 10, 1,  8, 1,  0, 0}, // #524
  {DBGFIELD("WriteCvtF2ILd")      1, 0, 0, 10, 2,  5, 1,  0, 0}, // #525
  {DBGFIELD("WriteCvtF2I")        1, 0, 0, 10, 1,  8, 1,  0, 0}, // #526
  {DBGFIELD("WriteCvtF2FLd")      1, 0, 0, 10, 2,  5, 1,  0, 0}, // #527
  {DBGFIELD("WriteCvtF2F")        1, 0, 0, 10, 1,  8, 1,  0, 0}, // #528
  {DBGFIELD("WriteCvtF2FLd_WriteRMW") 2, 0, 0, 378, 2, 24, 2,  0, 0}, // #529
  {DBGFIELD("WriteCvtF2FLd_ReadAfterLd") 1, 0, 0, 10, 2,  5, 1,  4, 1}, // #530
  {DBGFIELD("WriteCvtI2FLd_ReadAfterLd") 1, 0, 0, 10, 2,  5, 1,  4, 1}, // #531
  {DBGFIELD("IIC_VERR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #532
  {DBGFIELD("IIC_VERW_REG_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #533
  {DBGFIELD("IIC_VERW_MEM_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #534
  {DBGFIELD("WriteStore")         1, 0, 0, 37, 2,  2, 1,  0, 0}, // #535
  {DBGFIELD("WriteShuffle256")    1, 0, 0, 351, 2,  2, 1,  0, 0}, // #536
  {DBGFIELD("IIC_ALU_MEM_WriteFBlendLd_WriteRMW") 2, 0, 0, 377, 3, 27, 2,  0, 0}, // #537
  {DBGFIELD("WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #538
  {DBGFIELD("WriteShuffle256Ld_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #539
  {DBGFIELD("IIC_ALU_MEM_WriteFShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #540
  {DBGFIELD("IIC_ALU_NONMEM_WriteFShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #541
  {DBGFIELD("IIC_SSE_MOVDQ")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #542
  {DBGFIELD("IIC_SSE_MOV_LH")     0, 0, 0,  0, 0,  0, 0,  0, 0}, // #543
  {DBGFIELD("IIC_SSE_MOVD_ToGP")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #544
  {DBGFIELD("IIC_SSE_MOV_S_MR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #545
  {DBGFIELD("IIC_SSE_MOV_S_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #546
  {DBGFIELD("IIC_SSE_MOV_S_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #547
  {DBGFIELD("IIC_SSE_MOVQ_RR")    0, 0, 0,  0, 0,  0, 0,  0, 0}, // #548
  {DBGFIELD("IIC_ALU_MEM_WriteMPSADLd_ReadAfterLd") 1, 0, 0, 359, 3, 26, 1,  4, 1}, // #549
  {DBGFIELD("IIC_ALU_NONMEM_WriteMPSAD") 1, 0, 0, 351, 2,  5, 1,  0, 0}, // #550
  {DBGFIELD("WriteVecALULd")      1, 0, 0, 10, 2,  8, 1,  0, 0}, // #551
  {DBGFIELD("IIC_SSE_INTALU_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #552
  {DBGFIELD("IIC_SSE_INTALU_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #553
  {DBGFIELD("IIC_SSE_BIT_P_RM")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #554
  {DBGFIELD("IIC_SSE_BIT_P_RR")   0, 0, 0,  0, 0,  0, 0,  0, 0}, // #555
  {DBGFIELD("WriteBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #556
  {DBGFIELD("WriteBlend")         1, 0, 0, 351, 2,  2, 1,  0, 0}, // #557
  {DBGFIELD("WriteVarBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #558
  {DBGFIELD("WriteVarBlend")      1, 0, 0, 351, 2,  2, 1,  0, 0}, // #559
  {DBGFIELD("IIC_ALU_MEM_WriteBlendLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #560
  {DBGFIELD("IIC_ALU_NONMEM_WriteBlend") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #561
  {DBGFIELD("WriteCLMulLd_ReadAfterLd") 1, 0, 0, 392, 3, 26, 1,  4, 1}, // #562
  {DBGFIELD("WriteCLMul")         1, 0, 0, 392, 2, 26, 1,  0, 0}, // #563
  {DBGFIELD("WriteFShuffle256Ld_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #564
  {DBGFIELD("IIC_ALU_MEM_WriteShuffleLd") 1, 0, 0, 359, 3,  8, 1,  0, 0}, // #565
  {DBGFIELD("IIC_ALU_NONMEM_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #566
  {DBGFIELD("IIC_SSE_INTMUL_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #567
  {DBGFIELD("IIC_SSE_INTMUL_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #568
  {DBGFIELD("IIC_SSE_PSHUF_MI_WriteShuffleLd") 1, 0, 0, 359, 3,  8, 1,  0, 0}, // #569
  {DBGFIELD("WriteVecShift")      1, 0, 0, 351, 2,  2, 1,  0, 0}, // #570
  {DBGFIELD("IIC_SSE_INTSH_P_RM") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #571
  {DBGFIELD("IIC_SSE_INTSH_P_RR") 0, 0, 0,  0, 0,  0, 0,  0, 0}, // #572
  {DBGFIELD("WriteVarVecShiftLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #573
  {DBGFIELD("WriteVarVecShift")   1, 0, 0, 351, 2,  2, 1,  0, 0}, // #574
  {DBGFIELD("IIC_SSE_UNPCK")      0, 0, 0,  0, 0,  0, 0,  0, 0}, // #575
  {DBGFIELD("WriteFRcpLd_ReadAfterLd") 1, 0, 0, 359, 3,  7, 1,  4, 1}, // #576
  {DBGFIELD("WriteFSqrtLd_ReadAfterLd") 1, 0, 0, 359, 3, 38, 1,  4, 1}, // #577
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffleLd_ReadAfterLd") 1, 0, 0, 359, 3,  8, 1,  4, 1}, // #578
  {DBGFIELD("IIC_SSE_SHUFP_WriteShuffle") 1, 0, 0, 351, 2,  2, 1,  0, 0}, // #579
  {DBGFIELD("IIC_SSE_SQRTPD_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #580
  {DBGFIELD("IIC_SSE_SQRTPD_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #581
  {DBGFIELD("IIC_SSE_SQRTPS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #582
  {DBGFIELD("IIC_SSE_SQRTPS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #583
  {DBGFIELD("IIC_SSE_SQRTSS_RR")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #584
  {DBGFIELD("IIC_SSE_SQRTSS_RM")  0, 0, 0,  0, 0,  0, 0,  0, 0}, // #585
  {DBGFIELD("IIC_WAIT_WriteMicrocoded") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #586
  {DBGFIELD("IIC_WRMSR_WriteSystem") 1, 0, 0, 351, 2,  1, 1,  0, 0}, // #587
  {DBGFIELD("IIC_XADD_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #588
  {DBGFIELD("IIC_XADD_REG_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #589
  {DBGFIELD("IIC_XCHG_REG_WriteALU") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #590
  {DBGFIELD("IIC_XCHG_MEM_WriteALULd_WriteRMW") 2, 0, 0, 119, 2, 27, 2,  0, 0}, // #591
  {DBGFIELD("IIC_FXCH_WriteMove") 1, 0, 0, 29, 1,  2, 1,  0, 0}, // #592
  {DBGFIELD("IIC_XLAT_WriteLoad") 1, 0, 0, 11, 1,  6, 1,  0, 0} // #593
}; // SLMModelSchedClasses

static const llvm::MCSchedModel NoSchedModel(
  MCSchedModel::DefaultIssueWidth,
  MCSchedModel::DefaultMicroOpBufferSize,
  MCSchedModel::DefaultLoopMicroOpBufferSize,
  MCSchedModel::DefaultLoadLatency,
  MCSchedModel::DefaultHighLatency,
  MCSchedModel::DefaultMispredictPenalty,
  0, // PostRAScheduler
  1, // CompleteModel
  0, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel GenericModel(
  4, // IssueWidth
  32, // MicroOpBufferSize
  MCSchedModel::DefaultLoopMicroOpBufferSize,
  4, // LoadLatency
  10, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  0, // PostRAScheduler
  1, // CompleteModel
  1, // Processor ID
  0, 0, 0, 0, // No instruction-level machine model.
  NoItineraries);

static const llvm::MCSchedModel AtomModel(
  2, // IssueWidth
  0, // MicroOpBufferSize
  10, // LoopMicroOpBufferSize
  3, // LoadLatency
  30, // HighLatency
  MCSchedModel::DefaultMispredictPenalty,
  1, // PostRAScheduler
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
  28, // LoopMicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // PostRAScheduler
  0, // CompleteModel
  3, // Processor ID
  SandyBridgeModelProcResources,
  SandyBridgeModelSchedClasses,
  11,
  594,
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
  {DBGFIELD("HWPort06")        2, 0, -1}, // #7
  {DBGFIELD("HWPort15")        2, 0, -1}, // #8
  {DBGFIELD("HWPort16")        2, 0, -1}, // #9
  {DBGFIELD("HWPort23")        2, 0, -1}, // #10
  {DBGFIELD("HWPort015")       3, 0, -1}, // #11
  {DBGFIELD("HWPort237")       3, 0, -1}, // #12
  {DBGFIELD("HWPort0156")      4, 0, -1}, // #13
  {DBGFIELD("HWPortAny")       8, 0, 60}  // #14
};

static const llvm::MCSchedModel HaswellModel(
  4, // IssueWidth
  192, // MicroOpBufferSize
  50, // LoopMicroOpBufferSize
  4, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  16, // MispredictPenalty
  0, // PostRAScheduler
  0, // CompleteModel
  4, // Processor ID
  HaswellModelProcResources,
  HaswellModelSchedClasses,
  15,
  594,
  NoItineraries);

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

static const llvm::MCSchedModel SLMModel(
  2, // IssueWidth
  32, // MicroOpBufferSize
  10, // LoopMicroOpBufferSize
  3, // LoadLatency
  MCSchedModel::DefaultHighLatency,
  10, // MispredictPenalty
  1, // PostRAScheduler
  0, // CompleteModel
  5, // Processor ID
  SLMModelProcResources,
  SLMModelSchedClasses,
  11,
  594,
  NoItineraries);

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
  { "bdver4", (const void *)&GenericModel },
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
  { "skx", (const void *)&HaswellModel },
  { "slm", (const void *)&SLMModel },
  { "westmere", (const void *)&SandyBridgeModel },
  { "winchip-c6", (const void *)&GenericModel },
  { "winchip2", (const void *)&GenericModel },
  { "x86-64", (const void *)&SandyBridgeModel },
  { "yonah", (const void *)&SandyBridgeModel }
};
#undef DBGFIELD
static inline void InitX86MCSubtargetInfo(MCSubtargetInfo *II, StringRef TT, StringRef CPU, StringRef FS) {
  II->InitMCSubtargetInfo(TT, CPU, FS, X86FeatureKV, X86SubTypeKV,
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable,
                      X86Stages, X86OperandCycles, X86ForwardingPaths);
}

} // End llvm namespace
#endif // GET_SUBTARGETINFO_MC_DESC


#ifdef GET_SUBTARGETINFO_TARGET_DESC
#undef GET_SUBTARGETINFO_TARGET_DESC
#include "llvm/Support/Debug.h"
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
  if ((Bits & X86::FeatureBWI) != 0) HasBWI = true;
  if ((Bits & X86::FeatureCDI) != 0) HasCDI = true;
  if ((Bits & X86::FeatureCMOV) != 0) HasCMov = true;
  if ((Bits & X86::FeatureCMPXCHG16B) != 0) HasCmpxchg16b = true;
  if ((Bits & X86::FeatureCallRegIndirect) != 0) CallRegIndirect = true;
  if ((Bits & X86::FeatureDQI) != 0) HasDQI = true;
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
  if ((Bits & X86::FeatureSlowIncDec) != 0) SlowIncDec = true;
  if ((Bits & X86::FeatureSlowLEA) != 0) SlowLEA = true;
  if ((Bits & X86::FeatureSlowSHLD) != 0) IsSHLDSlow = true;
  if ((Bits & X86::FeatureTBM) != 0) HasTBM = true;
  if ((Bits & X86::FeatureVLX) != 0) HasVLX = true;
  if ((Bits & X86::FeatureVectorUAMem) != 0) HasVectorUAMem = true;
  if ((Bits & X86::FeatureXOP) != 0) HasXOP = true;
  if ((Bits & X86::Mode16Bit) != 0) In16BitMode = true;
  if ((Bits & X86::Mode32Bit) != 0) In32BitMode = true;
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
  unsigned resolveSchedClass(unsigned SchedClass, const MachineInstr *DefMI, const TargetSchedModel *SchedModel) const override;
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
  InitMCSubtargetInfo(TT, CPU, FS, makeArrayRef(X86FeatureKV, 58), makeArrayRef(X86SubTypeKV, 57),
                      X86ProcSchedKV, X86WriteProcResTable, X86WriteLatencyTable, X86ReadAdvanceTable,
                      X86Stages, X86OperandCycles, X86ForwardingPaths);
}

unsigned X86GenSubtargetInfo
::resolveSchedClass(unsigned SchedClass, const MachineInstr *MI, const TargetSchedModel *SchedModel) const {
  report_fatal_error("Expected a variant SchedClass");
} // X86GenSubtargetInfo::resolveSchedClass
} // End llvm namespace
#endif // GET_SUBTARGETINFO_CTOR

