/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Assembly Matcher Source Fragment                                           *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_ASSEMBLER_HEADER
#undef GET_ASSEMBLER_HEADER
  // This should be included into the middle of the declaration of
  // your subclasses implementation of MCTargetAsmParser.
  unsigned ComputeAvailableFeatures(uint64_t FeatureBits) const;
  void convertToMCInst(unsigned Kind, MCInst &Inst, unsigned Opcode,
                       const SmallVectorImpl<MCParsedAsmOperand*> &Operands);
  void convertToMapAndConstraints(unsigned Kind,
                           const SmallVectorImpl<MCParsedAsmOperand*> &Operands);
  bool mnemonicIsValid(StringRef Mnemonic);
  unsigned MatchInstructionImpl(
                           const SmallVectorImpl<MCParsedAsmOperand*> &Operands,
                                MCInst &Inst,
                                unsigned &ErrorInfo, bool matchingInlineAsm,
                                unsigned VariantID = 0);
#endif // GET_ASSEMBLER_HEADER_INFO


#ifdef GET_OPERAND_DIAGNOSTIC_TYPES
#undef GET_OPERAND_DIAGNOSTIC_TYPES

#endif // GET_OPERAND_DIAGNOSTIC_TYPES


#ifdef GET_REGISTER_MATCHER
#undef GET_REGISTER_MATCHER

// Flags for subtarget features that participate in instruction matching.
enum SubtargetFeatureFlag {
  Feature_In32BitMode = (1 << 0),
  Feature_In64BitMode = (1 << 1),
  Feature_None = 0
};

static unsigned MatchRegisterName(StringRef Name) {
  switch (Name.size()) {
  default: break;
  case 2:	 // 25 strings to match.
    switch (Name[0]) {
    default: break;
    case 'a':	 // 3 strings to match.
      switch (Name[1]) {
      default: break;
      case 'h':	 // 1 string to match.
        return 1;	 // "ah"
      case 'l':	 // 1 string to match.
        return 2;	 // "al"
      case 'x':	 // 1 string to match.
        return 3;	 // "ax"
      }
      break;
    case 'b':	 // 4 strings to match.
      switch (Name[1]) {
      default: break;
      case 'h':	 // 1 string to match.
        return 4;	 // "bh"
      case 'l':	 // 1 string to match.
        return 5;	 // "bl"
      case 'p':	 // 1 string to match.
        return 6;	 // "bp"
      case 'x':	 // 1 string to match.
        return 8;	 // "bx"
      }
      break;
    case 'c':	 // 4 strings to match.
      switch (Name[1]) {
      default: break;
      case 'h':	 // 1 string to match.
        return 9;	 // "ch"
      case 'l':	 // 1 string to match.
        return 10;	 // "cl"
      case 's':	 // 1 string to match.
        return 27;	 // "cs"
      case 'x':	 // 1 string to match.
        return 28;	 // "cx"
      }
      break;
    case 'd':	 // 5 strings to match.
      switch (Name[1]) {
      default: break;
      case 'h':	 // 1 string to match.
        return 29;	 // "dh"
      case 'i':	 // 1 string to match.
        return 30;	 // "di"
      case 'l':	 // 1 string to match.
        return 32;	 // "dl"
      case 's':	 // 1 string to match.
        return 41;	 // "ds"
      case 'x':	 // 1 string to match.
        return 42;	 // "dx"
      }
      break;
    case 'e':	 // 1 string to match.
      if (Name[1] != 's')
        break;
      return 52;	 // "es"
    case 'f':	 // 1 string to match.
      if (Name[1] != 's')
        break;
      return 63;	 // "fs"
    case 'g':	 // 1 string to match.
      if (Name[1] != 's')
        break;
      return 64;	 // "gs"
    case 'i':	 // 1 string to match.
      if (Name[1] != 'p')
        break;
      return 65;	 // "ip"
    case 'r':	 // 2 strings to match.
      switch (Name[1]) {
      default: break;
      case '8':	 // 1 string to match.
        return 74;	 // "r8"
      case '9':	 // 1 string to match.
        return 78;	 // "r9"
      }
      break;
    case 's':	 // 3 strings to match.
      switch (Name[1]) {
      default: break;
      case 'i':	 // 1 string to match.
        return 116;	 // "si"
      case 'p':	 // 1 string to match.
        return 118;	 // "sp"
      case 's':	 // 1 string to match.
        return 120;	 // "ss"
      }
      break;
    }
    break;
  case 3:	 // 69 strings to match.
    switch (Name[0]) {
    default: break;
    case 'b':	 // 1 string to match.
      if (memcmp(Name.data()+1, "pl", 2))
        break;
      return 7;	 // "bpl"
    case 'c':	 // 10 strings to match.
      if (Name[1] != 'r')
        break;
      switch (Name[2]) {
      default: break;
      case '0':	 // 1 string to match.
        return 11;	 // "cr0"
      case '1':	 // 1 string to match.
        return 12;	 // "cr1"
      case '2':	 // 1 string to match.
        return 13;	 // "cr2"
      case '3':	 // 1 string to match.
        return 14;	 // "cr3"
      case '4':	 // 1 string to match.
        return 15;	 // "cr4"
      case '5':	 // 1 string to match.
        return 16;	 // "cr5"
      case '6':	 // 1 string to match.
        return 17;	 // "cr6"
      case '7':	 // 1 string to match.
        return 18;	 // "cr7"
      case '8':	 // 1 string to match.
        return 19;	 // "cr8"
      case '9':	 // 1 string to match.
        return 20;	 // "cr9"
      }
      break;
    case 'd':	 // 9 strings to match.
      switch (Name[1]) {
      default: break;
      case 'i':	 // 1 string to match.
        if (Name[2] != 'l')
          break;
        return 31;	 // "dil"
      case 'r':	 // 8 strings to match.
        switch (Name[2]) {
        default: break;
        case '0':	 // 1 string to match.
          return 33;	 // "dr0"
        case '1':	 // 1 string to match.
          return 34;	 // "dr1"
        case '2':	 // 1 string to match.
          return 35;	 // "dr2"
        case '3':	 // 1 string to match.
          return 36;	 // "dr3"
        case '4':	 // 1 string to match.
          return 37;	 // "dr4"
        case '5':	 // 1 string to match.
          return 38;	 // "dr5"
        case '6':	 // 1 string to match.
          return 39;	 // "dr6"
        case '7':	 // 1 string to match.
          return 40;	 // "dr7"
        }
        break;
      }
      break;
    case 'e':	 // 10 strings to match.
      switch (Name[1]) {
      default: break;
      case 'a':	 // 1 string to match.
        if (Name[2] != 'x')
          break;
        return 43;	 // "eax"
      case 'b':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'p':	 // 1 string to match.
          return 44;	 // "ebp"
        case 'x':	 // 1 string to match.
          return 45;	 // "ebx"
        }
        break;
      case 'c':	 // 1 string to match.
        if (Name[2] != 'x')
          break;
        return 46;	 // "ecx"
      case 'd':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'i':	 // 1 string to match.
          return 47;	 // "edi"
        case 'x':	 // 1 string to match.
          return 48;	 // "edx"
        }
        break;
      case 'i':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'p':	 // 1 string to match.
          return 50;	 // "eip"
        case 'z':	 // 1 string to match.
          return 51;	 // "eiz"
        }
        break;
      case 's':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'i':	 // 1 string to match.
          return 53;	 // "esi"
        case 'p':	 // 1 string to match.
          return 54;	 // "esp"
        }
        break;
      }
      break;
    case 'f':	 // 7 strings to match.
      if (Name[1] != 'p')
        break;
      switch (Name[2]) {
      default: break;
      case '0':	 // 1 string to match.
        return 55;	 // "fp0"
      case '1':	 // 1 string to match.
        return 56;	 // "fp1"
      case '2':	 // 1 string to match.
        return 57;	 // "fp2"
      case '3':	 // 1 string to match.
        return 58;	 // "fp3"
      case '4':	 // 1 string to match.
        return 59;	 // "fp4"
      case '5':	 // 1 string to match.
        return 60;	 // "fp5"
      case '6':	 // 1 string to match.
        return 61;	 // "fp6"
      }
      break;
    case 'm':	 // 8 strings to match.
      if (Name[1] != 'm')
        break;
      switch (Name[2]) {
      default: break;
      case '0':	 // 1 string to match.
        return 66;	 // "mm0"
      case '1':	 // 1 string to match.
        return 67;	 // "mm1"
      case '2':	 // 1 string to match.
        return 68;	 // "mm2"
      case '3':	 // 1 string to match.
        return 69;	 // "mm3"
      case '4':	 // 1 string to match.
        return 70;	 // "mm4"
      case '5':	 // 1 string to match.
        return 71;	 // "mm5"
      case '6':	 // 1 string to match.
        return 72;	 // "mm6"
      case '7':	 // 1 string to match.
        return 73;	 // "mm7"
      }
      break;
    case 'r':	 // 22 strings to match.
      switch (Name[1]) {
      default: break;
      case '1':	 // 6 strings to match.
        switch (Name[2]) {
        default: break;
        case '0':	 // 1 string to match.
          return 82;	 // "r10"
        case '1':	 // 1 string to match.
          return 86;	 // "r11"
        case '2':	 // 1 string to match.
          return 90;	 // "r12"
        case '3':	 // 1 string to match.
          return 94;	 // "r13"
        case '4':	 // 1 string to match.
          return 98;	 // "r14"
        case '5':	 // 1 string to match.
          return 102;	 // "r15"
        }
        break;
      case '8':	 // 3 strings to match.
        switch (Name[2]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 75;	 // "r8b"
        case 'd':	 // 1 string to match.
          return 76;	 // "r8d"
        case 'w':	 // 1 string to match.
          return 77;	 // "r8w"
        }
        break;
      case '9':	 // 3 strings to match.
        switch (Name[2]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 79;	 // "r9b"
        case 'd':	 // 1 string to match.
          return 80;	 // "r9d"
        case 'w':	 // 1 string to match.
          return 81;	 // "r9w"
        }
        break;
      case 'a':	 // 1 string to match.
        if (Name[2] != 'x')
          break;
        return 106;	 // "rax"
      case 'b':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'p':	 // 1 string to match.
          return 107;	 // "rbp"
        case 'x':	 // 1 string to match.
          return 108;	 // "rbx"
        }
        break;
      case 'c':	 // 1 string to match.
        if (Name[2] != 'x')
          break;
        return 109;	 // "rcx"
      case 'd':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'i':	 // 1 string to match.
          return 110;	 // "rdi"
        case 'x':	 // 1 string to match.
          return 111;	 // "rdx"
        }
        break;
      case 'i':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'p':	 // 1 string to match.
          return 112;	 // "rip"
        case 'z':	 // 1 string to match.
          return 113;	 // "riz"
        }
        break;
      case 's':	 // 2 strings to match.
        switch (Name[2]) {
        default: break;
        case 'i':	 // 1 string to match.
          return 114;	 // "rsi"
        case 'p':	 // 1 string to match.
          return 115;	 // "rsp"
        }
        break;
      }
      break;
    case 's':	 // 2 strings to match.
      switch (Name[1]) {
      default: break;
      case 'i':	 // 1 string to match.
        if (Name[2] != 'l')
          break;
        return 117;	 // "sil"
      case 'p':	 // 1 string to match.
        if (Name[2] != 'l')
          break;
        return 119;	 // "spl"
      }
      break;
    }
    break;
  case 4:	 // 45 strings to match.
    switch (Name[0]) {
    default: break;
    case 'c':	 // 6 strings to match.
      if (memcmp(Name.data()+1, "r1", 2))
        break;
      switch (Name[3]) {
      default: break;
      case '0':	 // 1 string to match.
        return 21;	 // "cr10"
      case '1':	 // 1 string to match.
        return 22;	 // "cr11"
      case '2':	 // 1 string to match.
        return 23;	 // "cr12"
      case '3':	 // 1 string to match.
        return 24;	 // "cr13"
      case '4':	 // 1 string to match.
        return 25;	 // "cr14"
      case '5':	 // 1 string to match.
        return 26;	 // "cr15"
      }
      break;
    case 'f':	 // 1 string to match.
      if (memcmp(Name.data()+1, "psw", 3))
        break;
      return 62;	 // "fpsw"
    case 'r':	 // 18 strings to match.
      if (Name[1] != '1')
        break;
      switch (Name[2]) {
      default: break;
      case '0':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 83;	 // "r10b"
        case 'd':	 // 1 string to match.
          return 84;	 // "r10d"
        case 'w':	 // 1 string to match.
          return 85;	 // "r10w"
        }
        break;
      case '1':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 87;	 // "r11b"
        case 'd':	 // 1 string to match.
          return 88;	 // "r11d"
        case 'w':	 // 1 string to match.
          return 89;	 // "r11w"
        }
        break;
      case '2':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 91;	 // "r12b"
        case 'd':	 // 1 string to match.
          return 92;	 // "r12d"
        case 'w':	 // 1 string to match.
          return 93;	 // "r12w"
        }
        break;
      case '3':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 95;	 // "r13b"
        case 'd':	 // 1 string to match.
          return 96;	 // "r13d"
        case 'w':	 // 1 string to match.
          return 97;	 // "r13w"
        }
        break;
      case '4':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 99;	 // "r14b"
        case 'd':	 // 1 string to match.
          return 100;	 // "r14d"
        case 'w':	 // 1 string to match.
          return 101;	 // "r14w"
        }
        break;
      case '5':	 // 3 strings to match.
        switch (Name[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          return 103;	 // "r15b"
        case 'd':	 // 1 string to match.
          return 104;	 // "r15d"
        case 'w':	 // 1 string to match.
          return 105;	 // "r15w"
        }
        break;
      }
      break;
    case 'x':	 // 10 strings to match.
      if (memcmp(Name.data()+1, "mm", 2))
        break;
      switch (Name[3]) {
      default: break;
      case '0':	 // 1 string to match.
        return 129;	 // "xmm0"
      case '1':	 // 1 string to match.
        return 130;	 // "xmm1"
      case '2':	 // 1 string to match.
        return 131;	 // "xmm2"
      case '3':	 // 1 string to match.
        return 132;	 // "xmm3"
      case '4':	 // 1 string to match.
        return 133;	 // "xmm4"
      case '5':	 // 1 string to match.
        return 134;	 // "xmm5"
      case '6':	 // 1 string to match.
        return 135;	 // "xmm6"
      case '7':	 // 1 string to match.
        return 136;	 // "xmm7"
      case '8':	 // 1 string to match.
        return 137;	 // "xmm8"
      case '9':	 // 1 string to match.
        return 138;	 // "xmm9"
      }
      break;
    case 'y':	 // 10 strings to match.
      if (memcmp(Name.data()+1, "mm", 2))
        break;
      switch (Name[3]) {
      default: break;
      case '0':	 // 1 string to match.
        return 145;	 // "ymm0"
      case '1':	 // 1 string to match.
        return 146;	 // "ymm1"
      case '2':	 // 1 string to match.
        return 147;	 // "ymm2"
      case '3':	 // 1 string to match.
        return 148;	 // "ymm3"
      case '4':	 // 1 string to match.
        return 149;	 // "ymm4"
      case '5':	 // 1 string to match.
        return 150;	 // "ymm5"
      case '6':	 // 1 string to match.
        return 151;	 // "ymm6"
      case '7':	 // 1 string to match.
        return 152;	 // "ymm7"
      case '8':	 // 1 string to match.
        return 153;	 // "ymm8"
      case '9':	 // 1 string to match.
        return 154;	 // "ymm9"
      }
      break;
    }
    break;
  case 5:	 // 21 strings to match.
    switch (Name[0]) {
    default: break;
    case 'f':	 // 1 string to match.
      if (memcmp(Name.data()+1, "lags", 4))
        break;
      return 49;	 // "flags"
    case 's':	 // 8 strings to match.
      if (memcmp(Name.data()+1, "t(", 2))
        break;
      switch (Name[3]) {
      default: break;
      case '0':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 121;	 // "st(0)"
      case '1':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 122;	 // "st(1)"
      case '2':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 123;	 // "st(2)"
      case '3':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 124;	 // "st(3)"
      case '4':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 125;	 // "st(4)"
      case '5':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 126;	 // "st(5)"
      case '6':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 127;	 // "st(6)"
      case '7':	 // 1 string to match.
        if (Name[4] != ')')
          break;
        return 128;	 // "st(7)"
      }
      break;
    case 'x':	 // 6 strings to match.
      if (memcmp(Name.data()+1, "mm1", 3))
        break;
      switch (Name[4]) {
      default: break;
      case '0':	 // 1 string to match.
        return 139;	 // "xmm10"
      case '1':	 // 1 string to match.
        return 140;	 // "xmm11"
      case '2':	 // 1 string to match.
        return 141;	 // "xmm12"
      case '3':	 // 1 string to match.
        return 142;	 // "xmm13"
      case '4':	 // 1 string to match.
        return 143;	 // "xmm14"
      case '5':	 // 1 string to match.
        return 144;	 // "xmm15"
      }
      break;
    case 'y':	 // 6 strings to match.
      if (memcmp(Name.data()+1, "mm1", 3))
        break;
      switch (Name[4]) {
      default: break;
      case '0':	 // 1 string to match.
        return 155;	 // "ymm10"
      case '1':	 // 1 string to match.
        return 156;	 // "ymm11"
      case '2':	 // 1 string to match.
        return 157;	 // "ymm12"
      case '3':	 // 1 string to match.
        return 158;	 // "ymm13"
      case '4':	 // 1 string to match.
        return 159;	 // "ymm14"
      case '5':	 // 1 string to match.
        return 160;	 // "ymm15"
      }
      break;
    }
    break;
  }
  return 0;
}

#endif // GET_REGISTER_MATCHER


#ifdef GET_SUBTARGET_FEATURE_NAME
#undef GET_SUBTARGET_FEATURE_NAME

// User-level names for subtarget features that participate in
// instruction matching.
static const char *getSubtargetFeatureName(unsigned Val) {
  switch(Val) {
  case Feature_In32BitMode: return "";
  case Feature_In64BitMode: return "";
  default: return "(unknown)";
  }
}

#endif // GET_SUBTARGET_FEATURE_NAME


#ifdef GET_MATCHER_IMPLEMENTATION
#undef GET_MATCHER_IMPLEMENTATION

static void applyMnemonicAliases(StringRef &Mnemonic, unsigned Features) {
  switch (Mnemonic.size()) {
  default: break;
  case 2:	 // 2 strings to match.
    if (Mnemonic[0] != 'j')
      break;
    switch (Mnemonic[1]) {
    default: break;
    case 'c':	 // 1 string to match.
      Mnemonic = "jb";	 // "jc"
      return;
    case 'z':	 // 1 string to match.
      Mnemonic = "je";	 // "jz"
      return;
    }
    break;
  case 3:	 // 13 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'c':	 // 4 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'b':	 // 1 string to match.
        if (Mnemonic[2] != 'w')
          break;
        Mnemonic = "cbtw";	 // "cbw"
        return;
      case 'd':	 // 1 string to match.
        if (Mnemonic[2] != 'q')
          break;
        Mnemonic = "cltd";	 // "cdq"
        return;
      case 'q':	 // 1 string to match.
        if (Mnemonic[2] != 'o')
          break;
        Mnemonic = "cqto";	 // "cqo"
        return;
      case 'w':	 // 1 string to match.
        if (Mnemonic[2] != 'd')
          break;
        Mnemonic = "cwtd";	 // "cwd"
        return;
      }
      break;
    case 'j':	 // 8 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'n':	 // 6 strings to match.
        switch (Mnemonic[2]) {
        default: break;
        case 'a':	 // 1 string to match.
          Mnemonic = "jbe";	 // "jna"
          return;
        case 'b':	 // 1 string to match.
          Mnemonic = "jae";	 // "jnb"
          return;
        case 'c':	 // 1 string to match.
          Mnemonic = "jae";	 // "jnc"
          return;
        case 'g':	 // 1 string to match.
          Mnemonic = "jle";	 // "jng"
          return;
        case 'l':	 // 1 string to match.
          Mnemonic = "jge";	 // "jnl"
          return;
        case 'z':	 // 1 string to match.
          Mnemonic = "jne";	 // "jnz"
          return;
        }
        break;
      case 'p':	 // 2 strings to match.
        switch (Mnemonic[2]) {
        default: break;
        case 'e':	 // 1 string to match.
          Mnemonic = "jp";	 // "jpe"
          return;
        case 'o':	 // 1 string to match.
          Mnemonic = "jnp";	 // "jpo"
          return;
        }
        break;
      }
      break;
    case 'p':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "op", 2))
        break;
      if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "pop"
        Mnemonic = "popl";
      else if ((Features & Feature_In64BitMode) == Feature_In64BitMode)
        Mnemonic = "popq";
      return;
    }
    break;
  case 4:	 // 22 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'c':	 // 3 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'a':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "ll", 2))
          break;
        if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "call"
          Mnemonic = "calll";
        else if ((Features & Feature_In64BitMode) == Feature_In64BitMode)
          Mnemonic = "callq";
        return;
      case 'd':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "qe", 2))
          break;
        Mnemonic = "cltq";	 // "cdqe"
        return;
      case 'w':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "de", 2))
          break;
        Mnemonic = "cwtl";	 // "cwde"
        return;
      }
      break;
    case 'i':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "ret", 3))
        break;
      Mnemonic = "iretl";	 // "iret"
      return;
    case 'j':	 // 4 strings to match.
      if (Mnemonic[1] != 'n')
        break;
      switch (Mnemonic[2]) {
      default: break;
      case 'a':	 // 1 string to match.
        if (Mnemonic[3] != 'e')
          break;
        Mnemonic = "jb";	 // "jnae"
        return;
      case 'b':	 // 1 string to match.
        if (Mnemonic[3] != 'e')
          break;
        Mnemonic = "ja";	 // "jnbe"
        return;
      case 'g':	 // 1 string to match.
        if (Mnemonic[3] != 'e')
          break;
        Mnemonic = "jl";	 // "jnge"
        return;
      case 'l':	 // 1 string to match.
        if (Mnemonic[3] != 'e')
          break;
        Mnemonic = "jg";	 // "jnle"
        return;
      }
      break;
    case 'l':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "ret", 3))
        break;
      Mnemonic = "lretl";	 // "lret"
      return;
    case 'p':	 // 2 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'o':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "pf", 2))
          break;
        if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "popf"
          Mnemonic = "popfl";
        else if ((Features & Feature_In64BitMode) == Feature_In64BitMode)
          Mnemonic = "popfq";
        return;
      case 'u':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "sh", 2))
          break;
        if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "push"
          Mnemonic = "pushl";
        else if ((Features & Feature_In64BitMode) == Feature_In64BitMode)
          Mnemonic = "pushq";
        return;
      }
      break;
    case 'r':	 // 4 strings to match.
      if (Mnemonic[1] != 'e')
        break;
      switch (Mnemonic[2]) {
      default: break;
      case 'p':	 // 2 strings to match.
        switch (Mnemonic[3]) {
        default: break;
        case 'e':	 // 1 string to match.
          Mnemonic = "rep";	 // "repe"
          return;
        case 'z':	 // 1 string to match.
          Mnemonic = "rep";	 // "repz"
          return;
        }
        break;
      case 't':	 // 2 strings to match.
        switch (Mnemonic[3]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "retl"
            Mnemonic = "ret";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "retq"
            Mnemonic = "ret";
          return;
        }
        break;
      }
      break;
    case 's':	 // 6 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'a':	 // 4 strings to match.
        if (Mnemonic[2] != 'l')
          break;
        switch (Mnemonic[3]) {
        default: break;
        case 'b':	 // 1 string to match.
          Mnemonic = "shlb";	 // "salb"
          return;
        case 'l':	 // 1 string to match.
          Mnemonic = "shll";	 // "sall"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "shlq";	 // "salq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "shlw";	 // "salw"
          return;
        }
        break;
      case 'e':	 // 2 strings to match.
        if (Mnemonic[2] != 't')
          break;
        switch (Mnemonic[3]) {
        default: break;
        case 'c':	 // 1 string to match.
          Mnemonic = "setb";	 // "setc"
          return;
        case 'z':	 // 1 string to match.
          Mnemonic = "sete";	 // "setz"
          return;
        }
        break;
      }
      break;
    case 'u':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "d2a", 3))
        break;
      Mnemonic = "ud2";	 // "ud2a"
      return;
    }
    break;
  case 5:	 // 27 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'f':	 // 2 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'i':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "ldq", 3))
          break;
        Mnemonic = "fildll";	 // "fildq"
        return;
      case 'w':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "ait", 3))
          break;
        Mnemonic = "wait";	 // "fwait"
        return;
      }
      break;
    case 'l':	 // 5 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'g':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "dt", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "lgdtl"
            Mnemonic = "lgdt";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "lgdtq"
            Mnemonic = "lgdt";
          return;
        }
        break;
      case 'i':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "dt", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "lidtl"
            Mnemonic = "lidt";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "lidtq"
            Mnemonic = "lidt";
          return;
        }
        break;
      case 'o':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "opz", 3))
          break;
        Mnemonic = "loope";	 // "loopz"
        return;
      }
      break;
    case 'p':	 // 2 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'o':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "pfd", 3))
          break;
        Mnemonic = "popfl";	 // "popfd"
        return;
      case 'u':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "shf", 3))
          break;
        if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "pushf"
          Mnemonic = "pushfl";
        else if ((Features & Feature_In64BitMode) == Feature_In64BitMode)
          Mnemonic = "pushfq";
        return;
      }
      break;
    case 'r':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "epnz", 4))
        break;
      Mnemonic = "repne";	 // "repnz"
      return;
    case 's':	 // 16 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'e':	 // 8 strings to match.
        if (Mnemonic[2] != 't')
          break;
        switch (Mnemonic[3]) {
        default: break;
        case 'n':	 // 6 strings to match.
          switch (Mnemonic[4]) {
          default: break;
          case 'a':	 // 1 string to match.
            Mnemonic = "setbe";	 // "setna"
            return;
          case 'b':	 // 1 string to match.
            Mnemonic = "setae";	 // "setnb"
            return;
          case 'c':	 // 1 string to match.
            Mnemonic = "setae";	 // "setnc"
            return;
          case 'g':	 // 1 string to match.
            Mnemonic = "setle";	 // "setng"
            return;
          case 'l':	 // 1 string to match.
            Mnemonic = "setge";	 // "setnl"
            return;
          case 'z':	 // 1 string to match.
            Mnemonic = "setne";	 // "setnz"
            return;
          }
          break;
        case 'p':	 // 2 strings to match.
          switch (Mnemonic[4]) {
          default: break;
          case 'e':	 // 1 string to match.
            Mnemonic = "setp";	 // "setpe"
            return;
          case 'o':	 // 1 string to match.
            Mnemonic = "setnp";	 // "setpo"
            return;
          }
          break;
        }
        break;
      case 'g':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "dt", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "sgdtl"
            Mnemonic = "sgdt";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "sgdtq"
            Mnemonic = "sgdt";
          return;
        }
        break;
      case 'i':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "dt", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "sidtl"
            Mnemonic = "sidt";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "sidtq"
            Mnemonic = "sidt";
          return;
        }
        break;
      case 'm':	 // 4 strings to match.
        if (memcmp(Mnemonic.data()+2, "ov", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'b':	 // 1 string to match.
          Mnemonic = "movsb";	 // "smovb"
          return;
        case 'l':	 // 1 string to match.
          Mnemonic = "movsl";	 // "smovl"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "movsq";	 // "smovq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "movsw";	 // "smovw"
          return;
        }
        break;
      }
      break;
    case 'v':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "errw", 4))
        break;
      Mnemonic = "verr";	 // "verrw"
      return;
    }
    break;
  case 6:	 // 20 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'c':	 // 6 strings to match.
      if (memcmp(Mnemonic.data()+1, "mov", 3))
        break;
      switch (Mnemonic[4]) {
      default: break;
      case 'c':	 // 3 strings to match.
        switch (Mnemonic[5]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmovbl";	 // "cmovcl"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmovbq";	 // "cmovcq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovbw";	 // "cmovcw"
          return;
        }
        break;
      case 'z':	 // 3 strings to match.
        switch (Mnemonic[5]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmovel";	 // "cmovzl"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmoveq";	 // "cmovzq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovew";	 // "cmovzw"
          return;
        }
        break;
      }
      break;
    case 'f':	 // 5 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'c':	 // 3 strings to match.
        switch (Mnemonic[2]) {
        default: break;
        case 'm':	 // 2 strings to match.
          if (memcmp(Mnemonic.data()+3, "ov", 2))
            break;
          switch (Mnemonic[5]) {
          default: break;
          case 'a':	 // 1 string to match.
            Mnemonic = "fcmovnbe";	 // "fcmova"
            return;
          case 'z':	 // 1 string to match.
            Mnemonic = "fcmove";	 // "fcmovz"
            return;
          }
          break;
        case 'o':	 // 1 string to match.
          if (memcmp(Mnemonic.data()+3, "mip", 3))
            break;
          Mnemonic = "fcompi";	 // "fcomip"
          return;
        }
        break;
      case 'i':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "stpq", 4))
          break;
        Mnemonic = "fistpll";	 // "fistpq"
        return;
      case 'l':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "dcww", 4))
          break;
        Mnemonic = "fldcw";	 // "fldcww"
        return;
      }
      break;
    case 'l':	 // 3 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'e':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "ave", 3))
          break;
        switch (Mnemonic[5]) {
        default: break;
        case 'l':	 // 1 string to match.
          if ((Features & Feature_In32BitMode) == Feature_In32BitMode)	 // "leavel"
            Mnemonic = "leave";
          return;
        case 'q':	 // 1 string to match.
          if ((Features & Feature_In64BitMode) == Feature_In64BitMode)	 // "leaveq"
            Mnemonic = "leave";
          return;
        }
        break;
      case 'o':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "opnz", 4))
          break;
        Mnemonic = "loopne";	 // "loopnz"
        return;
      }
      break;
    case 'p':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "ushfd", 5))
        break;
      Mnemonic = "pushfl";	 // "pushfd"
      return;
    case 's':	 // 5 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'e':	 // 4 strings to match.
        if (memcmp(Mnemonic.data()+2, "tn", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'a':	 // 1 string to match.
          if (Mnemonic[5] != 'e')
            break;
          Mnemonic = "setb";	 // "setnae"
          return;
        case 'b':	 // 1 string to match.
          if (Mnemonic[5] != 'e')
            break;
          Mnemonic = "seta";	 // "setnbe"
          return;
        case 'g':	 // 1 string to match.
          if (Mnemonic[5] != 'e')
            break;
          Mnemonic = "setl";	 // "setnge"
          return;
        case 'l':	 // 1 string to match.
          if (Mnemonic[5] != 'e')
            break;
          Mnemonic = "setg";	 // "setnle"
          return;
        }
        break;
      case 'y':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "sret", 4))
          break;
        Mnemonic = "sysretl";	 // "sysret"
        return;
      }
      break;
    }
    break;
  case 7:	 // 31 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'c':	 // 24 strings to match.
      if (memcmp(Mnemonic.data()+1, "mov", 3))
        break;
      switch (Mnemonic[4]) {
      default: break;
      case 'n':	 // 18 strings to match.
        switch (Mnemonic[5]) {
        default: break;
        case 'a':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovbel";	 // "cmovnal"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovbeq";	 // "cmovnaq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovbew";	 // "cmovnaw"
            return;
          }
          break;
        case 'b':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovael";	 // "cmovnbl"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovaeq";	 // "cmovnbq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovaew";	 // "cmovnbw"
            return;
          }
          break;
        case 'c':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovael";	 // "cmovncl"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovaeq";	 // "cmovncq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovaew";	 // "cmovncw"
            return;
          }
          break;
        case 'g':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovlel";	 // "cmovngl"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovleq";	 // "cmovngq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovlew";	 // "cmovngw"
            return;
          }
          break;
        case 'l':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovgel";	 // "cmovnll"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovgeq";	 // "cmovnlq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovgew";	 // "cmovnlw"
            return;
          }
          break;
        case 'z':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovnel";	 // "cmovnzl"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovneq";	 // "cmovnzq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovnew";	 // "cmovnzw"
            return;
          }
          break;
        }
        break;
      case 'p':	 // 6 strings to match.
        switch (Mnemonic[5]) {
        default: break;
        case 'e':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovpl";	 // "cmovpel"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovpq";	 // "cmovpeq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovpw";	 // "cmovpew"
            return;
          }
          break;
        case 'o':	 // 3 strings to match.
          switch (Mnemonic[6]) {
          default: break;
          case 'l':	 // 1 string to match.
            Mnemonic = "cmovnpl";	 // "cmovpol"
            return;
          case 'q':	 // 1 string to match.
            Mnemonic = "cmovnpq";	 // "cmovpoq"
            return;
          case 'w':	 // 1 string to match.
            Mnemonic = "cmovnpw";	 // "cmovpow"
            return;
          }
          break;
        }
        break;
      }
      break;
    case 'f':	 // 6 strings to match.
      switch (Mnemonic[1]) {
      default: break;
      case 'c':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "mov", 3))
          break;
        switch (Mnemonic[5]) {
        default: break;
        case 'a':	 // 1 string to match.
          if (Mnemonic[6] != 'e')
            break;
          Mnemonic = "fcmovnb";	 // "fcmovae"
          return;
        case 'n':	 // 1 string to match.
          if (Mnemonic[6] != 'a')
            break;
          Mnemonic = "fcmovbe";	 // "fcmovna"
          return;
        }
        break;
      case 'i':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "sttpq", 5))
          break;
        Mnemonic = "fisttpll";	 // "fisttpq"
        return;
      case 'n':	 // 2 strings to match.
        if (memcmp(Mnemonic.data()+2, "st", 2))
          break;
        switch (Mnemonic[4]) {
        default: break;
        case 'c':	 // 1 string to match.
          if (memcmp(Mnemonic.data()+5, "ww", 2))
            break;
          Mnemonic = "fnstcw";	 // "fnstcww"
          return;
        case 's':	 // 1 string to match.
          if (memcmp(Mnemonic.data()+5, "ww", 2))
            break;
          Mnemonic = "fnstsw";	 // "fnstsww"
          return;
        }
        break;
      case 'u':	 // 1 string to match.
        if (memcmp(Mnemonic.data()+2, "comip", 5))
          break;
        Mnemonic = "fucompi";	 // "fucomip"
        return;
      }
      break;
    case 's':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "ysexit", 6))
        break;
      Mnemonic = "sysexitl";	 // "sysexit"
      return;
    }
    break;
  case 8:	 // 13 strings to match.
    switch (Mnemonic[0]) {
    default: break;
    case 'c':	 // 12 strings to match.
      if (memcmp(Mnemonic.data()+1, "movn", 4))
        break;
      switch (Mnemonic[5]) {
      default: break;
      case 'a':	 // 3 strings to match.
        if (Mnemonic[6] != 'e')
          break;
        switch (Mnemonic[7]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmovbl";	 // "cmovnael"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmovbq";	 // "cmovnaeq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovbw";	 // "cmovnaew"
          return;
        }
        break;
      case 'b':	 // 3 strings to match.
        if (Mnemonic[6] != 'e')
          break;
        switch (Mnemonic[7]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmoval";	 // "cmovnbel"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmovaq";	 // "cmovnbeq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovaw";	 // "cmovnbew"
          return;
        }
        break;
      case 'g':	 // 3 strings to match.
        if (Mnemonic[6] != 'e')
          break;
        switch (Mnemonic[7]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmovll";	 // "cmovngel"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmovlq";	 // "cmovngeq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovlw";	 // "cmovngew"
          return;
        }
        break;
      case 'l':	 // 3 strings to match.
        if (Mnemonic[6] != 'e')
          break;
        switch (Mnemonic[7]) {
        default: break;
        case 'l':	 // 1 string to match.
          Mnemonic = "cmovgl";	 // "cmovnlel"
          return;
        case 'q':	 // 1 string to match.
          Mnemonic = "cmovgq";	 // "cmovnleq"
          return;
        case 'w':	 // 1 string to match.
          Mnemonic = "cmovgw";	 // "cmovnlew"
          return;
        }
        break;
      }
      break;
    case 'f':	 // 1 string to match.
      if (memcmp(Mnemonic.data()+1, "cmovnae", 7))
        break;
      Mnemonic = "fcmovb";	 // "fcmovnae"
      return;
    }
    break;
  }
}

namespace {
enum OperatorConversionKind {
  CVT_Done,
  CVT_Reg,
  CVT_Tied,
  CVT_imm_10,
  CVT_addImmOperands,
  CVT_addMem8Operands,
  CVT_addMem16Operands,
  CVT_addMem32Operands,
  CVT_addMem64Operands,
  CVT_addMem128Operands,
  CVT_addAbsMemOperands,
  CVT_addMemOperands,
  CVT_regST0,
  CVT_regST1,
  CVT_addMem80Operands,
  CVT_imm_17,
  CVT_imm_1,
  CVT_imm_16,
  CVT_imm_0,
  CVT_addMem256Operands,
  CVT_addMemVX64Operands,
  CVT_addMemVX32Operands,
  CVT_addMemVY32Operands,
  CVT_addMemVY64Operands,
  CVT_NUM_CONVERTERS
};

enum InstructionConversionKind {
  Convert_NoOperands,
  Convert__imm_10,
  Convert__Imm1_0,
  Convert__Imm1_1,
  Convert__Reg1_0__Tie0__Reg1_1,
  Convert__Reg1_0__Tie0__Imm1_1,
  Convert__Reg1_0__Tie0__Mem85_1,
  Convert__Reg1_0__Tie0__ImmSExti16i81_1,
  Convert__Reg1_0__Tie0__Mem165_1,
  Convert__Reg1_0__Tie0__ImmSExti32i81_1,
  Convert__Reg1_0__Tie0__Mem325_1,
  Convert__ImmSExti64i321_1,
  Convert__Reg1_0__Tie0__ImmSExti64i81_1,
  Convert__Reg1_0__Tie0__ImmSExti64i321_1,
  Convert__Reg1_0__Tie0__Mem645_1,
  Convert__Mem165_0__Reg1_1,
  Convert__Mem165_0__ImmSExti16i81_1,
  Convert__Mem165_0__Imm1_1,
  Convert__Mem325_0__Reg1_1,
  Convert__Mem325_0__ImmSExti32i81_1,
  Convert__Mem325_0__Imm1_1,
  Convert__Mem645_0__Reg1_1,
  Convert__Mem645_0__ImmSExti64i81_1,
  Convert__Mem645_0__ImmSExti64i321_1,
  Convert__Mem85_0__Reg1_1,
  Convert__Mem85_0__Imm1_1,
  Convert__Reg1_1__Tie0__Reg1_0,
  Convert__Mem85_1__Reg1_0,
  Convert__Reg1_1__Tie0__Imm1_0,
  Convert__Mem85_1__Imm1_0,
  Convert__Reg1_1__Tie0__Mem85_0,
  Convert__Mem325_1__Reg1_0,
  Convert__Reg1_1__Tie0__ImmSExti32i81_0,
  Convert__Mem325_1__ImmSExti32i81_0,
  Convert__Mem325_1__Imm1_0,
  Convert__Reg1_1__Tie0__Mem325_0,
  Convert__Mem645_1__Reg1_0,
  Convert__Reg1_1__Tie0__ImmSExti64i81_0,
  Convert__Mem645_1__ImmSExti64i81_0,
  Convert__ImmSExti64i321_0,
  Convert__Reg1_1__Tie0__ImmSExti64i321_0,
  Convert__Mem645_1__ImmSExti64i321_0,
  Convert__Reg1_1__Tie0__Mem645_0,
  Convert__Mem165_1__Reg1_0,
  Convert__Reg1_1__Tie0__ImmSExti16i81_0,
  Convert__Mem165_1__ImmSExti16i81_0,
  Convert__Mem165_1__Imm1_0,
  Convert__Reg1_1__Tie0__Mem165_0,
  Convert__Reg1_0__Tie0__Mem1285_1,
  Convert__Reg1_1__Tie0__Mem1285_0,
  Convert__Reg1_1__Reg1_0,
  Convert__Reg1_0__Reg1_1,
  Convert__Reg1_0__Mem1285_1,
  Convert__Reg1_1__Mem1285_0,
  Convert__Reg1_0__Reg1_1__Imm1_2,
  Convert__Reg1_0__Mem1285_1__Imm1_2,
  Convert__Reg1_2__Reg1_1__Imm1_0,
  Convert__Reg1_2__Mem1285_1__Imm1_0,
  Convert__Reg1_0__Reg1_1__Reg1_2,
  Convert__Reg1_0__Reg1_1__Mem325_2,
  Convert__Reg1_0__Reg1_1__Mem645_2,
  Convert__Reg1_2__Reg1_1__Reg1_0,
  Convert__Reg1_2__Reg1_1__Mem325_0,
  Convert__Reg1_2__Reg1_1__Mem645_0,
  Convert__Reg1_0__Mem165_1,
  Convert__Reg1_1__Mem165_0,
  Convert__Reg1_0__Mem325_1__Reg1_2,
  Convert__Reg1_0__Mem645_1__Reg1_2,
  Convert__Reg1_2__Mem325_1__Reg1_0,
  Convert__Reg1_2__Mem645_1__Reg1_0,
  Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2,
  Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2,
  Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0,
  Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0,
  Convert__Reg1_2__Tie0__Reg1_1,
  Convert__Reg1_2__Tie0__Mem1285_1,
  Convert__Reg1_0__Mem325_1,
  Convert__Reg1_0__Mem645_1,
  Convert__Reg1_1__Mem325_0,
  Convert__Reg1_1__Mem645_0,
  Convert__Reg1_0__Tie0,
  Convert__Reg1_0__ImmSExti16i81_1,
  Convert__Reg1_0__ImmSExti32i81_1,
  Convert__Reg1_0__ImmSExti64i81_1,
  Convert__Reg1_1__ImmSExti32i81_0,
  Convert__Reg1_1__ImmSExti64i81_0,
  Convert__Reg1_1__ImmSExti16i81_0,
  Convert__Reg1_0,
  Convert__AbsMem1_0,
  Convert__Mem325_0,
  Convert__Mem645_0,
  Convert__Imm1_1__Imm1_0,
  Convert__Reg1_1,
  Convert__Mem325_1,
  Convert__Mem645_1,
  Convert__Mem85_0,
  Convert__Reg1_0__Tie0__Reg1_0,
  Convert__Reg1_0__Imm1_1,
  Convert__Reg1_0__Mem85_1,
  Convert__Reg1_0__ImmSExti64i321_1,
  Convert__Reg1_3__Tie0__Reg1_2__Imm1_0,
  Convert__Reg1_2__Tie0__Reg1_3__Imm1_0,
  Convert__Reg1_2__Tie0__Mem1285_3__Imm1_0,
  Convert__Reg1_3__Tie0__Mem1285_2__Imm1_0,
  Convert__Reg1_2__Tie0__Mem645_3__Imm1_0,
  Convert__Reg1_3__Tie0__Mem645_2__Imm1_0,
  Convert__Reg1_3__Tie0__Mem325_2__Imm1_0,
  Convert__Reg1_2__Tie0__Mem325_3__Imm1_0,
  Convert__Reg1_1__Imm1_0,
  Convert__Reg1_1__Mem85_0,
  Convert__Reg1_0__Tie0__Reg1_1__Imm1_2,
  Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2,
  Convert__Reg1_2__Tie0__Reg1_1__Imm1_0,
  Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0,
  Convert__Reg1_1__ImmSExti64i321_0,
  Convert__Reg1_0__Tie0__Mem645_1__Imm1_2,
  Convert__Reg1_2__Tie0__Mem645_1__Imm1_0,
  Convert__Reg1_0__Tie0__Mem325_1__Imm1_2,
  Convert__Reg1_2__Tie0__Mem325_1__Imm1_0,
  Convert__Mem1285_0,
  Convert__Reg1_0__Mem5_1,
  Convert__Reg1_1__Mem5_0,
  Convert__Mem165_0,
  Convert__Imm1_0__Imm1_1,
  Convert__Reg1_0__Reg1_1__ImmSExti32i81_2,
  Convert__Reg1_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem325_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem325_0__Reg1_1__ImmSExti32i81_2,
  Convert__Reg1_0__Tie0__Imm1_1__Imm1_2,
  Convert__Reg1_2__Tie0__Imm1_1__Imm1_0,
  Convert__regST0,
  Convert__regST1,
  Convert__Mem805_0,
  Convert__Mem5_0,
  Convert__Reg1_0__Reg1_1__ImmSExti16i81_2,
  Convert__Reg1_0__Mem165_1__ImmSExti16i81_2,
  Convert__Reg1_0__Mem165_1__Imm1_2,
  Convert__Reg1_0__Mem325_1__ImmSExti32i81_2,
  Convert__Reg1_0__Mem325_1__Imm1_2,
  Convert__Reg1_0__Reg1_1__ImmSExti64i81_2,
  Convert__Reg1_0__Reg1_1__ImmSExti64i321_2,
  Convert__Reg1_0__Mem645_1__ImmSExti64i81_2,
  Convert__Reg1_0__Mem645_1__ImmSExti64i321_2,
  Convert__Reg1_1__Reg1_1__ImmSExti32i81_0,
  Convert__Reg1_1__Reg1_1__Imm1_0,
  Convert__Reg1_2__Mem325_1__ImmSExti32i81_0,
  Convert__Reg1_2__Mem325_1__Imm1_0,
  Convert__Reg1_1__Reg1_1__ImmSExti64i81_0,
  Convert__Reg1_1__Reg1_1__ImmSExti64i321_0,
  Convert__Reg1_2__Reg1_1__ImmSExti64i81_0,
  Convert__Reg1_2__Mem645_1__ImmSExti64i81_0,
  Convert__Reg1_2__Reg1_1__ImmSExti64i321_0,
  Convert__Reg1_2__Mem645_1__ImmSExti64i321_0,
  Convert__Reg1_1__Reg1_1__ImmSExti16i81_0,
  Convert__Reg1_2__Reg1_1__ImmSExti16i81_0,
  Convert__Reg1_2__Mem165_1__ImmSExti16i81_0,
  Convert__Reg1_2__Mem165_1__Imm1_0,
  Convert__Reg1_0__Tie0__Mem325_1__ImmZExtu32u81_2,
  Convert__Reg1_2__Tie0__Mem325_1__ImmZExtu32u81_0,
  Convert__Reg1_0__Tie0__Reg1_1__Imm1_2__Imm1_3,
  Convert__Reg1_3__Tie0__Reg1_2__Imm1_1__Imm1_0,
  Convert__Mem5_1,
  Convert__AbsMem1_1,
  Convert__Mem1285_1__Reg1_0,
  Convert__Mem1285_0__Reg1_1,
  Convert__Reg1_1__Tie0__Reg1_0__imm_17,
  Convert__Reg1_0__Tie0__Reg1_1__imm_17,
  Convert__Reg1_0__Tie0__Mem1285_1__imm_17,
  Convert__Reg1_1__Tie0__Mem1285_0__imm_17,
  Convert__Reg1_1__Tie0__Reg1_0__imm_1,
  Convert__Reg1_0__Tie0__Reg1_1__imm_1,
  Convert__Reg1_0__Tie0__Mem1285_1__imm_1,
  Convert__Reg1_1__Tie0__Mem1285_0__imm_1,
  Convert__Reg1_1__Tie0__Reg1_0__imm_16,
  Convert__Reg1_0__Tie0__Reg1_1__imm_16,
  Convert__Reg1_0__Tie0__Mem1285_1__imm_16,
  Convert__Reg1_1__Tie0__Mem1285_0__imm_16,
  Convert__Reg1_1__Tie0__Reg1_0__imm_0,
  Convert__Reg1_0__Tie0__Reg1_1__imm_0,
  Convert__Reg1_0__Tie0__Mem1285_1__imm_0,
  Convert__Reg1_1__Tie0__Mem1285_0__imm_0,
  Convert__Mem85_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem85_0__Reg1_1__ImmSExti32i81_2,
  Convert__Mem645_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem645_0__Reg1_1__ImmSExti32i81_2,
  Convert__Mem165_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem165_0__Reg1_1__ImmSExti32i81_2,
  Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2,
  Convert__Reg1_0__Tie0__Mem85_1__ImmSExti32i81_2,
  Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0,
  Convert__Reg1_2__Tie0__Mem85_1__ImmSExti32i81_0,
  Convert__Reg1_0__Tie0__Mem325_1__ImmSExti32i81_2,
  Convert__Reg1_2__Tie0__Mem325_1__ImmSExti32i81_0,
  Convert__Reg1_0__Tie0__Mem645_1__ImmSExti32i81_2,
  Convert__Reg1_2__Tie0__Mem645_1__ImmSExti32i81_0,
  Convert__Reg1_0__Tie0__Mem165_1__ImmSExti32i81_2,
  Convert__Reg1_2__Tie0__Mem165_1__ImmSExti32i81_0,
  Convert__Reg1_0__Mem645_1__Imm1_2,
  Convert__Reg1_2__Mem645_1__Imm1_0,
  Convert__ImmSExti64i81_0,
  Convert__ImmSExti32i81_0,
  Convert__Mem645_0__Imm1_1,
  Convert__Reg1_1__Tie0,
  Convert__Mem85_1,
  Convert__Mem645_1__Imm1_0,
  Convert__Mem165_1,
  Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2,
  Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0,
  Convert__Reg1_0__Tie0__Mem5_1__ImmSExti32i81_2,
  Convert__Reg1_2__Tie0__Mem5_1__ImmSExti32i81_0,
  Convert__Mem165_0__Reg1_1__Imm1_2,
  Convert__Mem325_0__Reg1_1__Imm1_2,
  Convert__Mem645_0__Reg1_1__Imm1_2,
  Convert__Mem325_2__Reg1_1,
  Convert__Mem325_2__Reg1_1__Imm1_0,
  Convert__Mem645_2__Reg1_1,
  Convert__Mem645_2__Reg1_1__Imm1_0,
  Convert__Mem165_2__Reg1_1,
  Convert__Mem165_2__Reg1_1__Imm1_0,
  Convert__Reg1_0__Reg1_1__Mem1285_2,
  Convert__Reg1_0__Reg1_1__Mem2565_2,
  Convert__Reg1_2__Reg1_1__Mem1285_0,
  Convert__Reg1_2__Reg1_1__Mem2565_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3,
  Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3,
  Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3,
  Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0,
  Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0,
  Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0,
  Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3,
  Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3,
  Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0,
  Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3,
  Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0,
  Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0,
  Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0,
  Convert__Reg1_2__Reg1_3__Mem1285_4__Imm1_0,
  Convert__Reg1_2__Reg1_3__Mem2565_4__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem1285_2__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem2565_2__Imm1_0,
  Convert__Reg1_2__Reg1_3__Mem645_4__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem645_2__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem325_2__Imm1_0,
  Convert__Reg1_2__Reg1_3__Mem325_4__Imm1_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3,
  Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3,
  Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3,
  Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0,
  Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0,
  Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0,
  Convert__Reg1_0__Reg1_1__Mem645_2__Imm1_3,
  Convert__Reg1_3__Reg1_2__Mem645_1__Imm1_0,
  Convert__Reg1_0__Reg1_1__Mem325_2__Imm1_3,
  Convert__Reg1_3__Reg1_2__Mem325_1__Imm1_0,
  Convert__Reg1_0__Mem2565_1,
  Convert__Reg1_1__Mem2565_0,
  Convert__Mem1285_2__Reg1_1__ImmSExti32i81_0,
  Convert__Mem1285_0__Reg1_1__ImmSExti32i81_2,
  Convert__Mem1285_2__Reg1_1__Imm1_0,
  Convert__Mem1285_0__Reg1_1__Imm1_2,
  Convert__Reg1_2__Tie0__Reg1_1__Reg1_0,
  Convert__Reg1_0__Tie0__Reg1_1__Reg1_2,
  Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2,
  Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2,
  Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0,
  Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0,
  Convert__Reg1_0__Tie0__Reg1_1__Mem645_2,
  Convert__Reg1_2__Tie0__Reg1_1__Mem645_0,
  Convert__Reg1_0__Tie0__Reg1_1__Mem325_2,
  Convert__Reg1_2__Tie0__Reg1_1__Mem325_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3,
  Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0,
  Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3,
  Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3,
  Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0,
  Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3,
  Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3,
  Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0,
  Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0,
  Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1,
  Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1,
  Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1,
  Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1,
  Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1,
  Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1,
  Convert__Reg1_2__Reg1_0__Tie0__MemVY645_1__Tie1,
  Convert__Reg1_0__Reg1_2__Tie0__MemVY645_1__Tie1,
  Convert__Reg1_0__Reg1_1__Mem325_2__ImmZExtu32u81_3,
  Convert__Reg1_3__Reg1_2__Mem325_1__ImmZExtu32u81_0,
  Convert__Mem1285_2__Reg1_1__Reg1_0,
  Convert__Mem2565_2__Reg1_1__Reg1_0,
  Convert__Mem1285_0__Reg1_1__Reg1_2,
  Convert__Mem2565_0__Reg1_1__Reg1_2,
  Convert__Mem2565_1__Reg1_0,
  Convert__Mem2565_0__Reg1_1,
  Convert__Reg1_2__Reg1_1__Reg1_0__imm_17,
  Convert__Reg1_0__Reg1_1__Reg1_2__imm_17,
  Convert__Reg1_0__Reg1_1__Mem1285_2__imm_17,
  Convert__Reg1_2__Reg1_1__Mem1285_0__imm_17,
  Convert__Reg1_2__Reg1_1__Reg1_0__imm_1,
  Convert__Reg1_0__Reg1_1__Reg1_2__imm_1,
  Convert__Reg1_0__Reg1_1__Mem1285_2__imm_1,
  Convert__Reg1_2__Reg1_1__Mem1285_0__imm_1,
  Convert__Reg1_2__Reg1_1__Reg1_0__imm_16,
  Convert__Reg1_0__Reg1_1__Reg1_2__imm_16,
  Convert__Reg1_0__Reg1_1__Mem1285_2__imm_16,
  Convert__Reg1_2__Reg1_1__Mem1285_0__imm_16,
  Convert__Reg1_2__Reg1_1__Reg1_0__imm_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__imm_0,
  Convert__Reg1_0__Reg1_1__Mem1285_2__imm_0,
  Convert__Reg1_2__Reg1_1__Mem1285_0__imm_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3__Imm1_4,
  Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3__Imm1_4,
  Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3__Imm1_4,
  Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3__Imm1_4,
  Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem1285_2__Reg1_1__Imm1_0,
  Convert__Reg1_4__Reg1_3__Mem2565_2__Reg1_1__Imm1_0,
  Convert__Reg1_4__Reg1_3__Reg1_2__Mem1285_1__Imm1_0,
  Convert__Reg1_4__Reg1_3__Reg1_2__Mem2565_1__Imm1_0,
  Convert__Reg1_0__Mem2565_1__Imm1_2,
  Convert__Reg1_2__Mem2565_1__Imm1_0,
  Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3,
  Convert__Reg1_0__Reg1_1__Mem85_2__ImmSExti32i81_3,
  Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0,
  Convert__Reg1_3__Reg1_2__Mem85_1__ImmSExti32i81_0,
  Convert__Reg1_0__Reg1_1__Mem325_2__ImmSExti32i81_3,
  Convert__Reg1_3__Reg1_2__Mem325_1__ImmSExti32i81_0,
  Convert__Reg1_0__Reg1_1__Mem645_2__ImmSExti32i81_3,
  Convert__Reg1_3__Reg1_2__Mem645_1__ImmSExti32i81_0,
  Convert__Reg1_0__Reg1_1__Mem165_2__ImmSExti32i81_3,
  Convert__Reg1_3__Reg1_2__Mem165_1__ImmSExti32i81_0,
  Convert__Reg1_2__Mem1285_1__Reg1_0,
  Convert__Reg1_0__Mem1285_1__Reg1_2,
  Convert__Reg1_0__Mem2565_1__ImmSExti32i81_2,
  Convert__Reg1_2__Mem2565_1__ImmSExti32i81_0,
  Convert__Reg1_0__Reg1_1__Mem5_2__ImmSExti32i81_3,
  Convert__Reg1_3__Reg1_2__Mem5_1__ImmSExti32i81_0,
  CVT_NUM_SIGNATURES
};

} // end anonymous namespace

static const uint8_t ConversionTable[CVT_NUM_SIGNATURES][11] = {
  // Convert_NoOperands
  { CVT_Done },
  // Convert__imm_10
  { CVT_imm_10, 0, CVT_Done },
  // Convert__Imm1_0
  { CVT_addImmOperands, 1, CVT_Done },
  // Convert__Imm1_1
  { CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Imm1_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem85_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem8Operands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__ImmSExti16i81_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem165_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem16Operands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__ImmSExti32i81_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem325_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_Done },
  // Convert__ImmSExti64i321_1
  { CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__ImmSExti64i81_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__ImmSExti64i321_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem645_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem64Operands, 2, CVT_Done },
  // Convert__Mem165_0__Reg1_1
  { CVT_addMem16Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Mem165_0__ImmSExti16i81_1
  { CVT_addMem16Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem165_0__Imm1_1
  { CVT_addMem16Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem325_0__Reg1_1
  { CVT_addMem32Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Mem325_0__ImmSExti32i81_1
  { CVT_addMem32Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem325_0__Imm1_1
  { CVT_addMem32Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem645_0__Reg1_1
  { CVT_addMem64Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Mem645_0__ImmSExti64i81_1
  { CVT_addMem64Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem645_0__ImmSExti64i321_1
  { CVT_addMem64Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Mem85_0__Reg1_1
  { CVT_addMem8Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Mem85_0__Imm1_1
  { CVT_addMem8Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_1__Tie0__Reg1_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Reg, 1, CVT_Done },
  // Convert__Mem85_1__Reg1_0
  { CVT_addMem8Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__Imm1_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem85_1__Imm1_0
  { CVT_addMem8Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem85_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem8Operands, 1, CVT_Done },
  // Convert__Mem325_1__Reg1_0
  { CVT_addMem32Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__ImmSExti32i81_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem325_1__ImmSExti32i81_0
  { CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem325_1__Imm1_0
  { CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem325_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Mem645_1__Reg1_0
  { CVT_addMem64Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__ImmSExti64i81_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem645_1__ImmSExti64i81_0
  { CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__ImmSExti64i321_0
  { CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__ImmSExti64i321_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem645_1__ImmSExti64i321_0
  { CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem645_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Mem165_1__Reg1_0
  { CVT_addMem16Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__ImmSExti16i81_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_1__ImmSExti16i81_0
  { CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_1__Imm1_0
  { CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem165_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem16Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem1285_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_1__Reg1_0
  { CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Reg1_0__Mem1285_1
  { CVT_Reg, 1, CVT_addMem128Operands, 2, CVT_Done },
  // Convert__Reg1_1__Mem1285_0
  { CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Imm1_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem1285_1__Imm1_2
  { CVT_Reg, 1, CVT_addMem128Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem1285_1__Imm1_0
  { CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem325_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem645_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem64Operands, 3, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Reg1_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem325_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem645_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Reg1_0__Mem165_1
  { CVT_Reg, 1, CVT_addMem16Operands, 2, CVT_Done },
  // Convert__Reg1_1__Mem165_0
  { CVT_Reg, 2, CVT_addMem16Operands, 1, CVT_Done },
  // Convert__Reg1_0__Mem325_1__Reg1_2
  { CVT_Reg, 1, CVT_addMem32Operands, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Reg1_0__Mem645_1__Reg1_2
  { CVT_Reg, 1, CVT_addMem64Operands, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Reg1_2__Mem325_1__Reg1_0
  { CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_2__Mem645_1__Reg1_0
  { CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem1285_1
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_Done },
  // Convert__Reg1_0__Mem325_1
  { CVT_Reg, 1, CVT_addMem32Operands, 2, CVT_Done },
  // Convert__Reg1_0__Mem645_1
  { CVT_Reg, 1, CVT_addMem64Operands, 2, CVT_Done },
  // Convert__Reg1_1__Mem325_0
  { CVT_Reg, 2, CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Reg1_1__Mem645_0
  { CVT_Reg, 2, CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Done },
  // Convert__Reg1_0__ImmSExti16i81_1
  { CVT_Reg, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__ImmSExti32i81_1
  { CVT_Reg, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__ImmSExti64i81_1
  { CVT_Reg, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_1__ImmSExti32i81_0
  { CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__ImmSExti64i81_0
  { CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__ImmSExti16i81_0
  { CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0
  { CVT_Reg, 1, CVT_Done },
  // Convert__AbsMem1_0
  { CVT_addAbsMemOperands, 1, CVT_Done },
  // Convert__Mem325_0
  { CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Mem645_0
  { CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Imm1_1__Imm1_0
  { CVT_addImmOperands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1
  { CVT_Reg, 2, CVT_Done },
  // Convert__Mem325_1
  { CVT_addMem32Operands, 2, CVT_Done },
  // Convert__Mem645_1
  { CVT_addMem64Operands, 2, CVT_Done },
  // Convert__Mem85_0
  { CVT_addMem8Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_0
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Imm1_1
  { CVT_Reg, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Mem85_1
  { CVT_Reg, 1, CVT_addMem8Operands, 2, CVT_Done },
  // Convert__Reg1_0__ImmSExti64i321_1
  { CVT_Reg, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_3__Tie0__Reg1_2__Imm1_0
  { CVT_Reg, 4, CVT_Tied, 0, CVT_Reg, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_3__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 4, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem1285_3__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem128Operands, 4, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Tie0__Mem1285_2__Imm1_0
  { CVT_Reg, 4, CVT_Tied, 0, CVT_addMem128Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem645_3__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem64Operands, 4, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Tie0__Mem645_2__Imm1_0
  { CVT_Reg, 4, CVT_Tied, 0, CVT_addMem64Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Tie0__Mem325_2__Imm1_0
  { CVT_Reg, 4, CVT_Tied, 0, CVT_addMem32Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem325_3__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem32Operands, 4, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Imm1_0
  { CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Mem85_0
  { CVT_Reg, 2, CVT_addMem8Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Imm1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__ImmSExti64i321_0
  { CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem645_1__Imm1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem64Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem645_1__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem325_1__Imm1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem325_1__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem1285_0
  { CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_0__Mem5_1
  { CVT_Reg, 1, CVT_addMemOperands, 2, CVT_Done },
  // Convert__Reg1_1__Mem5_0
  { CVT_Reg, 2, CVT_addMemOperands, 1, CVT_Done },
  // Convert__Mem165_0
  { CVT_addMem16Operands, 1, CVT_Done },
  // Convert__Imm1_0__Imm1_1
  { CVT_addImmOperands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_0__Reg1_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Reg1_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem325_2__Reg1_1__ImmSExti32i81_0
  { CVT_addMem32Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem325_0__Reg1_1__ImmSExti32i81_2
  { CVT_addMem32Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Imm1_1__Imm1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Imm1_1__Imm1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addImmOperands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__regST0
  { CVT_regST0, 0, CVT_Done },
  // Convert__regST1
  { CVT_regST1, 0, CVT_Done },
  // Convert__Mem805_0
  { CVT_addMem80Operands, 1, CVT_Done },
  // Convert__Mem5_0
  { CVT_addMemOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__ImmSExti16i81_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem165_1__ImmSExti16i81_2
  { CVT_Reg, 1, CVT_addMem16Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem165_1__Imm1_2
  { CVT_Reg, 1, CVT_addMem16Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem325_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_addMem32Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem325_1__Imm1_2
  { CVT_Reg, 1, CVT_addMem32Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Reg1_1__ImmSExti64i81_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Reg1_1__ImmSExti64i321_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem645_1__ImmSExti64i81_2
  { CVT_Reg, 1, CVT_addMem64Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Mem645_1__ImmSExti64i321_2
  { CVT_Reg, 1, CVT_addMem64Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_1__Reg1_1__ImmSExti32i81_0
  { CVT_Reg, 2, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Reg1_1__Imm1_0
  { CVT_Reg, 2, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem325_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem325_1__Imm1_0
  { CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Reg1_1__ImmSExti64i81_0
  { CVT_Reg, 2, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Reg1_1__ImmSExti64i321_0
  { CVT_Reg, 2, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__ImmSExti64i81_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem645_1__ImmSExti64i81_0
  { CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__ImmSExti64i321_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem645_1__ImmSExti64i321_0
  { CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_1__Reg1_1__ImmSExti16i81_0
  { CVT_Reg, 2, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__ImmSExti16i81_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem165_1__ImmSExti16i81_0
  { CVT_Reg, 3, CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem165_1__Imm1_0
  { CVT_Reg, 3, CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem325_1__ImmZExtu32u81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem325_1__ImmZExtu32u81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Imm1_2__Imm1_3
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Tie0__Reg1_2__Imm1_1__Imm1_0
  { CVT_Reg, 4, CVT_Tied, 0, CVT_Reg, 3, CVT_addImmOperands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem5_1
  { CVT_addMemOperands, 2, CVT_Done },
  // Convert__AbsMem1_1
  { CVT_addAbsMemOperands, 2, CVT_Done },
  // Convert__Mem1285_1__Reg1_0
  { CVT_addMem128Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Mem1285_0__Reg1_1
  { CVT_addMem128Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Reg1_1__Tie0__Reg1_0__imm_17
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Reg, 1, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__imm_17
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__imm_17
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem1285_0__imm_17
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem128Operands, 1, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Reg1_0__imm_1
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Reg, 1, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__imm_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__imm_1
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem1285_0__imm_1
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem128Operands, 1, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Reg1_0__imm_16
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Reg, 1, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__imm_16
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__imm_16
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem1285_0__imm_16
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem128Operands, 1, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Reg1_0__imm_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Reg, 1, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__imm_0
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem1285_1__imm_0
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem128Operands, 2, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_1__Tie0__Mem1285_0__imm_0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_addMem128Operands, 1, CVT_imm_0, 0, CVT_Done },
  // Convert__Mem85_2__Reg1_1__ImmSExti32i81_0
  { CVT_addMem8Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem85_0__Reg1_1__ImmSExti32i81_2
  { CVT_addMem8Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem645_2__Reg1_1__ImmSExti32i81_0
  { CVT_addMem64Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem645_0__Reg1_1__ImmSExti32i81_2
  { CVT_addMem64Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem165_2__Reg1_1__ImmSExti32i81_0
  { CVT_addMem16Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_0__Reg1_1__ImmSExti32i81_2
  { CVT_addMem16Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem85_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem8Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem85_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem8Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem325_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem325_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem645_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem64Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem645_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem165_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMem16Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem165_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Mem645_1__Imm1_2
  { CVT_Reg, 1, CVT_addMem64Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Mem645_1__Imm1_0
  { CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__ImmSExti64i81_0
  { CVT_addImmOperands, 1, CVT_Done },
  // Convert__ImmSExti32i81_0
  { CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem645_0__Imm1_1
  { CVT_addMem64Operands, 1, CVT_addImmOperands, 2, CVT_Done },
  // Convert__Reg1_1__Tie0
  { CVT_Reg, 2, CVT_Tied, 0, CVT_Done },
  // Convert__Mem85_1
  { CVT_addMem8Operands, 2, CVT_Done },
  // Convert__Mem645_1__Imm1_0
  { CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_1
  { CVT_addMem16Operands, 2, CVT_Done },
  // Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_addMem128Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Mem5_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_addMemOperands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Mem5_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_addMemOperands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_0__Reg1_1__Imm1_2
  { CVT_addMem16Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem325_0__Reg1_1__Imm1_2
  { CVT_addMem32Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem645_0__Reg1_1__Imm1_2
  { CVT_addMem64Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem325_2__Reg1_1
  { CVT_addMem32Operands, 3, CVT_Reg, 2, CVT_Done },
  // Convert__Mem325_2__Reg1_1__Imm1_0
  { CVT_addMem32Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem645_2__Reg1_1
  { CVT_addMem64Operands, 3, CVT_Reg, 2, CVT_Done },
  // Convert__Mem645_2__Reg1_1__Imm1_0
  { CVT_addMem64Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem165_2__Reg1_1
  { CVT_addMem16Operands, 3, CVT_Reg, 2, CVT_Done },
  // Convert__Mem165_2__Reg1_1__Imm1_0
  { CVT_addMem16Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem2565_2
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem1285_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem2565_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem256Operands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_Reg, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_Reg, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_Reg, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_Reg, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 4, CVT_Reg, 5, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_3__Mem1285_4__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 4, CVT_addMem128Operands, 5, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_3__Mem2565_4__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 4, CVT_addMem256Operands, 5, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem1285_2__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem128Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem2565_2__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem256Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_3__Mem645_4__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 4, CVT_addMem64Operands, 5, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem645_2__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem64Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem325_2__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem32Operands, 3, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_3__Mem325_4__Imm1_0
  { CVT_Reg, 3, CVT_Reg, 4, CVT_addMem32Operands, 5, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem645_2__Imm1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem64Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem645_1__Imm1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem325_2__Imm1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem325_1__Imm1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Mem2565_1
  { CVT_Reg, 1, CVT_addMem256Operands, 2, CVT_Done },
  // Convert__Reg1_1__Mem2565_0
  { CVT_Reg, 2, CVT_addMem256Operands, 1, CVT_Done },
  // Convert__Mem1285_2__Reg1_1__ImmSExti32i81_0
  { CVT_addMem128Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem1285_0__Reg1_1__ImmSExti32i81_2
  { CVT_addMem128Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Mem1285_2__Reg1_1__Imm1_0
  { CVT_addMem128Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem1285_0__Reg1_1__Imm1_2
  { CVT_addMem128Operands, 1, CVT_Reg, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Reg1_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Reg1_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem256Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Mem645_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem64Operands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Mem645_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Reg1_0__Tie0__Reg1_1__Mem325_2
  { CVT_Reg, 1, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_Done },
  // Convert__Reg1_2__Tie0__Reg1_1__Mem325_0
  { CVT_Reg, 3, CVT_Tied, 0, CVT_Reg, 2, CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem128Operands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem256Operands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addMem256Operands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem64Operands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem64Operands, 3, CVT_Reg, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addMem64Operands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem32Operands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_Reg, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addMem32Operands, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1
  { CVT_Reg, 3, CVT_Reg, 1, CVT_Tied, 0, CVT_addMemVX64Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1
  { CVT_Reg, 1, CVT_Reg, 3, CVT_Tied, 0, CVT_addMemVX64Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1
  { CVT_Reg, 3, CVT_Reg, 1, CVT_Tied, 0, CVT_addMemVX32Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1
  { CVT_Reg, 1, CVT_Reg, 3, CVT_Tied, 0, CVT_addMemVX32Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1
  { CVT_Reg, 3, CVT_Reg, 1, CVT_Tied, 0, CVT_addMemVY32Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1
  { CVT_Reg, 1, CVT_Reg, 3, CVT_Tied, 0, CVT_addMemVY32Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_2__Reg1_0__Tie0__MemVY645_1__Tie1
  { CVT_Reg, 3, CVT_Reg, 1, CVT_Tied, 0, CVT_addMemVY64Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_2__Tie0__MemVY645_1__Tie1
  { CVT_Reg, 1, CVT_Reg, 3, CVT_Tied, 0, CVT_addMemVY64Operands, 2, CVT_Tied, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem325_2__ImmZExtu32u81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem325_1__ImmZExtu32u81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Mem1285_2__Reg1_1__Reg1_0
  { CVT_addMem128Operands, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Mem2565_2__Reg1_1__Reg1_0
  { CVT_addMem256Operands, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Mem1285_0__Reg1_1__Reg1_2
  { CVT_addMem128Operands, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Mem2565_0__Reg1_1__Reg1_2
  { CVT_addMem256Operands, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Mem2565_1__Reg1_0
  { CVT_addMem256Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Mem2565_0__Reg1_1
  { CVT_addMem256Operands, 1, CVT_Reg, 2, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Reg1_0__imm_17
  { CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__imm_17
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__imm_17
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem1285_0__imm_17
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_imm_17, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Reg1_0__imm_1
  { CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__imm_1
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__imm_1
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem1285_0__imm_1
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_imm_1, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Reg1_0__imm_16
  { CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__imm_16
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__imm_16
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem1285_0__imm_16
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_imm_16, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Reg1_0__imm_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_Reg, 1, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__imm_0
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__imm_0
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_2__Reg1_1__Mem1285_0__imm_0
  { CVT_Reg, 3, CVT_Reg, 2, CVT_addMem128Operands, 1, CVT_imm_0, 0, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_Reg, 4, CVT_addImmOperands, 5, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3__Imm1_4
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem128Operands, 4, CVT_addImmOperands, 5, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3__Imm1_4
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem128Operands, 3, CVT_Reg, 4, CVT_addImmOperands, 5, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3__Imm1_4
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addMem256Operands, 4, CVT_addImmOperands, 5, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3__Imm1_4
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem256Operands, 3, CVT_Reg, 4, CVT_addImmOperands, 5, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem1285_2__Reg1_1__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem128Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Mem2565_2__Reg1_1__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_addMem256Operands, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Reg1_2__Mem1285_1__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_4__Reg1_3__Reg1_2__Mem2565_1__Imm1_0
  { CVT_Reg, 5, CVT_Reg, 4, CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Mem2565_1__Imm1_2
  { CVT_Reg, 1, CVT_addMem256Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Mem2565_1__Imm1_0
  { CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_Reg, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem85_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem8Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_Reg, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem85_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem8Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem325_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem32Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem325_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem32Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem645_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem64Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem645_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem64Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem165_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMem16Operands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem165_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMem16Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_2__Mem1285_1__Reg1_0
  { CVT_Reg, 3, CVT_addMem128Operands, 2, CVT_Reg, 1, CVT_Done },
  // Convert__Reg1_0__Mem1285_1__Reg1_2
  { CVT_Reg, 1, CVT_addMem128Operands, 2, CVT_Reg, 3, CVT_Done },
  // Convert__Reg1_0__Mem2565_1__ImmSExti32i81_2
  { CVT_Reg, 1, CVT_addMem256Operands, 2, CVT_addImmOperands, 3, CVT_Done },
  // Convert__Reg1_2__Mem2565_1__ImmSExti32i81_0
  { CVT_Reg, 3, CVT_addMem256Operands, 2, CVT_addImmOperands, 1, CVT_Done },
  // Convert__Reg1_0__Reg1_1__Mem5_2__ImmSExti32i81_3
  { CVT_Reg, 1, CVT_Reg, 2, CVT_addMemOperands, 3, CVT_addImmOperands, 4, CVT_Done },
  // Convert__Reg1_3__Reg1_2__Mem5_1__ImmSExti32i81_0
  { CVT_Reg, 4, CVT_Reg, 3, CVT_addMemOperands, 2, CVT_addImmOperands, 1, CVT_Done },
};

void X86AsmParser::
convertToMCInst(unsigned Kind, MCInst &Inst, unsigned Opcode,
                const SmallVectorImpl<MCParsedAsmOperand*> &Operands) {
  assert(Kind < CVT_NUM_SIGNATURES && "Invalid signature!");
  const uint8_t *Converter = ConversionTable[Kind];
  Inst.setOpcode(Opcode);
  for (const uint8_t *p = Converter; *p; p+= 2) {
    switch (*p) {
    default: llvm_unreachable("invalid conversion entry!");
    case CVT_Reg:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addRegOperands(Inst, 1);
      break;
    case CVT_Tied:
      Inst.addOperand(Inst.getOperand(*(p + 1)));
      break;
    case CVT_imm_10:
      Inst.addOperand(MCOperand::CreateImm(10));
      break;
    case CVT_addImmOperands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addImmOperands(Inst, 1);
      break;
    case CVT_addMem8Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem8Operands(Inst, 5);
      break;
    case CVT_addMem16Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem16Operands(Inst, 5);
      break;
    case CVT_addMem32Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem32Operands(Inst, 5);
      break;
    case CVT_addMem64Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem64Operands(Inst, 5);
      break;
    case CVT_addMem128Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem128Operands(Inst, 5);
      break;
    case CVT_addAbsMemOperands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addAbsMemOperands(Inst, 1);
      break;
    case CVT_addMemOperands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMemOperands(Inst, 5);
      break;
    case CVT_regST0:
      Inst.addOperand(MCOperand::CreateReg(X86::ST0));
      break;
    case CVT_regST1:
      Inst.addOperand(MCOperand::CreateReg(X86::ST1));
      break;
    case CVT_addMem80Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem80Operands(Inst, 5);
      break;
    case CVT_imm_17:
      Inst.addOperand(MCOperand::CreateImm(17));
      break;
    case CVT_imm_1:
      Inst.addOperand(MCOperand::CreateImm(1));
      break;
    case CVT_imm_16:
      Inst.addOperand(MCOperand::CreateImm(16));
      break;
    case CVT_imm_0:
      Inst.addOperand(MCOperand::CreateImm(0));
      break;
    case CVT_addMem256Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMem256Operands(Inst, 5);
      break;
    case CVT_addMemVX64Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMemVX64Operands(Inst, 5);
      break;
    case CVT_addMemVX32Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMemVX32Operands(Inst, 5);
      break;
    case CVT_addMemVY32Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMemVY32Operands(Inst, 5);
      break;
    case CVT_addMemVY64Operands:
      static_cast<X86Operand*>(Operands[*(p + 1)])->addMemVY64Operands(Inst, 5);
      break;
    }
  }
}

void X86AsmParser::
convertToMapAndConstraints(unsigned Kind,
                           const SmallVectorImpl<MCParsedAsmOperand*> &Operands) {
  assert(Kind < CVT_NUM_SIGNATURES && "Invalid signature!");
  unsigned NumMCOperands = 0;
  const uint8_t *Converter = ConversionTable[Kind];
  for (const uint8_t *p = Converter; *p; p+= 2) {
    switch (*p) {
    default: llvm_unreachable("invalid conversion entry!");
    case CVT_Reg:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      ++NumMCOperands;
      break;
    case CVT_Tied:
      ++NumMCOperands;
      break;
    case CVT_imm_10:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("");
      ++NumMCOperands;
      break;
    case CVT_addImmOperands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 1;
      break;
    case CVT_addMem8Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMem16Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMem32Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMem64Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMem128Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addAbsMemOperands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 1;
      break;
    case CVT_addMemOperands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_regST0:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      ++NumMCOperands;
      break;
    case CVT_regST1:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      ++NumMCOperands;
      break;
    case CVT_addMem80Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_imm_17:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("");
      ++NumMCOperands;
      break;
    case CVT_imm_1:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("");
      ++NumMCOperands;
      break;
    case CVT_imm_16:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("");
      ++NumMCOperands;
      break;
    case CVT_imm_0:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("");
      ++NumMCOperands;
      break;
    case CVT_addMem256Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMemVX64Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMemVX32Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMemVY32Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    case CVT_addMemVY64Operands:
      Operands[*(p + 1)]->setMCOperandNum(NumMCOperands);
      Operands[*(p + 1)]->setConstraint("m");
      NumMCOperands += 5;
      break;
    }
  }
}

namespace {

/// MatchClassKind - The kinds of classes which participate in
/// instruction matching.
enum MatchClassKind {
  InvalidMatchClass = 0,
  MCK__PCT_al, // '%al'
  MCK__PCT_ax, // '%ax'
  MCK__PCT_cl, // '%cl'
  MCK__PCT_dx, // '%dx'
  MCK__PCT_eax, // '%eax'
  MCK__PCT_ecx, // '%ecx'
  MCK__PCT_edx, // '%edx'
  MCK__PCT_rax, // '%rax'
  MCK__PCT_rcx, // '%rcx'
  MCK__PCT_rdx, // '%rdx'
  MCK__PCT_st_40_0_41_, // '%st(0)'
  MCK__PCT_xmm0, // '%xmm0'
  MCK__STAR_, // '*'
  MCK_pd, // 'pd'
  MCK_ps, // 'ps'
  MCK_sd, // 'sd'
  MCK_ss, // 'ss'
  MCK_AL, // register class 'AL'
  MCK_CL, // register class 'CL'
  MCK_GR8_ABCD_L, // register class 'GR8_ABCD_L'
  MCK_GR8_ABCD_H, // register class 'GR8_ABCD_H'
  MCK_GR8_NOREX, // register class 'GR8_NOREX'
  MCK_GR8, // register class 'GR8'
  MCK_AX, // register class 'AX'
  MCK_DX, // register class 'DX'
  MCK_GR16_ABCD, // register class 'GR16_ABCD'
  MCK_GR16_NOREX, // register class 'GR16_NOREX'
  MCK_GR16, // register class 'GR16'
  MCK_EAX, // register class 'EAX'
  MCK_EDX, // register class 'EDX'
  MCK_GR32_AD, // register class 'GR32_AD'
  MCK_ECX, // register class 'ECX'
  MCK_Reg20, // derived register class
  MCK_Reg21, // derived register class
  MCK_Reg22, // derived register class
  MCK_Reg23, // derived register class
  MCK_Reg25, // derived register class
  MCK_GR32_ABCD, // register class 'GR32_ABCD'
  MCK_GR32_NOAX, // register class 'GR32_NOAX'
  MCK_GR32_TC, // register class 'GR32_TC'
  MCK_GR32_NOREX_NOSP, // register class 'GR32_NOREX_NOSP'
  MCK_GR32_NOREX, // register class 'GR32_NOREX'
  MCK_GR32_NOSP, // register class 'GR32_NOSP'
  MCK_GR32, // register class 'GR32'
  MCK_RAX, // register class 'RAX'
  MCK_RDX, // register class 'RDX'
  MCK_Reg28, // derived register class
  MCK_RCX, // register class 'RCX'
  MCK_Reg43, // derived register class
  MCK_Reg29, // derived register class
  MCK_Reg49, // derived register class
  MCK_Reg51, // derived register class
  MCK_Reg44, // derived register class
  MCK_Reg37, // derived register class
  MCK_Reg45, // derived register class
  MCK_Reg38, // derived register class
  MCK_Reg40, // derived register class
  MCK_Reg46, // derived register class
  MCK_Reg50, // derived register class
  MCK_Reg48, // derived register class
  MCK_Reg47, // derived register class
  MCK_GR64_ABCD, // register class 'GR64_ABCD'
  MCK_GR64_NOREX_NOSP, // register class 'GR64_NOREX_NOSP'
  MCK_Reg32, // derived register class
  MCK_Reg33, // derived register class
  MCK_GR64_NOREX, // register class 'GR64_NOREX'
  MCK_GR64_TCW64, // register class 'GR64_TCW64'
  MCK_GR64_NOSP, // register class 'GR64_NOSP'
  MCK_GR64_TC, // register class 'GR64_TC'
  MCK_GR64, // register class 'GR64'
  MCK_VR64, // register class 'VR64'
  MCK_RFP32, // register class 'RFP32,RFP64,RFP80'
  MCK_XMM0, // register class 'XMM0'
  MCK_FR32, // register class 'FR32,FR64,VR128'
  MCK_VR256, // register class 'VR256'
  MCK_ST0, // register class 'ST0'
  MCK_RST, // register class 'RST'
  MCK_FPCCR, // register class 'FPCCR'
  MCK_CCR, // register class 'CCR'
  MCK_CS, // register class 'CS'
  MCK_DS, // register class 'DS'
  MCK_SS, // register class 'SS'
  MCK_ES, // register class 'ES'
  MCK_FS, // register class 'FS'
  MCK_GS, // register class 'GS'
  MCK_SEGMENT_REG, // register class 'SEGMENT_REG'
  MCK_DEBUG_REG, // register class 'DEBUG_REG'
  MCK_CONTROL_REG, // register class 'CONTROL_REG'
  MCK_ImmSExti64i8, // user defined class 'ImmSExti64i8AsmOperand'
  MCK_ImmSExti16i8, // user defined class 'ImmSExti16i8AsmOperand'
  MCK_ImmSExti32i8, // user defined class 'ImmSExti32i8AsmOperand'
  MCK_ImmSExti64i32, // user defined class 'ImmSExti64i32AsmOperand'
  MCK_ImmZExtu32u8, // user defined class 'ImmZExtu32u8AsmOperand'
  MCK_Imm, // user defined class 'ImmAsmOperand'
  MCK_AbsMem, // user defined class 'X86AbsMemAsmOperand'
  MCK_Mem128, // user defined class 'X86Mem128AsmOperand'
  MCK_Mem16, // user defined class 'X86Mem16AsmOperand'
  MCK_Mem256, // user defined class 'X86Mem256AsmOperand'
  MCK_Mem32, // user defined class 'X86Mem32AsmOperand'
  MCK_Mem64, // user defined class 'X86Mem64AsmOperand'
  MCK_Mem80, // user defined class 'X86Mem80AsmOperand'
  MCK_Mem8, // user defined class 'X86Mem8AsmOperand'
  MCK_Mem, // user defined class 'X86MemAsmOperand'
  MCK_MemVX32, // user defined class 'X86MemVX32Operand'
  MCK_MemVX64, // user defined class 'X86MemVX64Operand'
  MCK_MemVY32, // user defined class 'X86MemVY32Operand'
  MCK_MemVY64, // user defined class 'X86MemVY64Operand'
  NumMatchClassKinds
};

}

static MatchClassKind matchTokenString(StringRef Name) {
  switch (Name.size()) {
  default: break;
  case 1:	 // 1 string to match.
    if (Name[0] != '*')
      break;
    return MCK__STAR_;	 // "*"
  case 2:	 // 4 strings to match.
    switch (Name[0]) {
    default: break;
    case 'p':	 // 2 strings to match.
      switch (Name[1]) {
      default: break;
      case 'd':	 // 1 string to match.
        return MCK_pd;	 // "pd"
      case 's':	 // 1 string to match.
        return MCK_ps;	 // "ps"
      }
      break;
    case 's':	 // 2 strings to match.
      switch (Name[1]) {
      default: break;
      case 'd':	 // 1 string to match.
        return MCK_sd;	 // "sd"
      case 's':	 // 1 string to match.
        return MCK_ss;	 // "ss"
      }
      break;
    }
    break;
  case 3:	 // 4 strings to match.
    if (Name[0] != '%')
      break;
    switch (Name[1]) {
    default: break;
    case 'a':	 // 2 strings to match.
      switch (Name[2]) {
      default: break;
      case 'l':	 // 1 string to match.
        return MCK__PCT_al;	 // "%al"
      case 'x':	 // 1 string to match.
        return MCK__PCT_ax;	 // "%ax"
      }
      break;
    case 'c':	 // 1 string to match.
      if (Name[2] != 'l')
        break;
      return MCK__PCT_cl;	 // "%cl"
    case 'd':	 // 1 string to match.
      if (Name[2] != 'x')
        break;
      return MCK__PCT_dx;	 // "%dx"
    }
    break;
  case 4:	 // 6 strings to match.
    if (Name[0] != '%')
      break;
    switch (Name[1]) {
    default: break;
    case 'e':	 // 3 strings to match.
      switch (Name[2]) {
      default: break;
      case 'a':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_eax;	 // "%eax"
      case 'c':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_ecx;	 // "%ecx"
      case 'd':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_edx;	 // "%edx"
      }
      break;
    case 'r':	 // 3 strings to match.
      switch (Name[2]) {
      default: break;
      case 'a':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_rax;	 // "%rax"
      case 'c':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_rcx;	 // "%rcx"
      case 'd':	 // 1 string to match.
        if (Name[3] != 'x')
          break;
        return MCK__PCT_rdx;	 // "%rdx"
      }
      break;
    }
    break;
  case 5:	 // 1 string to match.
    if (memcmp(Name.data()+0, "%xmm0", 5))
      break;
    return MCK__PCT_xmm0;	 // "%xmm0"
  case 6:	 // 1 string to match.
    if (memcmp(Name.data()+0, "%st(0)", 6))
      break;
    return MCK__PCT_st_40_0_41_;	 // "%st(0)"
  }
  return InvalidMatchClass;
}

/// isSubclass - Compute whether \p A is a subclass of \p B.
static bool isSubclass(MatchClassKind A, MatchClassKind B) {
  if (A == B)
    return true;

  switch (A) {
  default:
    return false;

  case MCK_AL:
    switch (B) {
    default: return false;
    case MCK_GR8_ABCD_L: return true;
    case MCK_GR8_NOREX: return true;
    case MCK_GR8: return true;
    }

  case MCK_CL:
    switch (B) {
    default: return false;
    case MCK_GR8_ABCD_L: return true;
    case MCK_GR8_NOREX: return true;
    case MCK_GR8: return true;
    }

  case MCK_GR8_ABCD_L:
    switch (B) {
    default: return false;
    case MCK_GR8_NOREX: return true;
    case MCK_GR8: return true;
    }

  case MCK_GR8_ABCD_H:
    switch (B) {
    default: return false;
    case MCK_GR8_NOREX: return true;
    case MCK_GR8: return true;
    }

  case MCK_GR8_NOREX:
    return B == MCK_GR8;

  case MCK_AX:
    switch (B) {
    default: return false;
    case MCK_GR16_ABCD: return true;
    case MCK_GR16_NOREX: return true;
    case MCK_GR16: return true;
    }

  case MCK_DX:
    switch (B) {
    default: return false;
    case MCK_GR16_ABCD: return true;
    case MCK_GR16_NOREX: return true;
    case MCK_GR16: return true;
    }

  case MCK_GR16_ABCD:
    switch (B) {
    default: return false;
    case MCK_GR16_NOREX: return true;
    case MCK_GR16: return true;
    }

  case MCK_GR16_NOREX:
    return B == MCK_GR16;

  case MCK_EAX:
    switch (B) {
    default: return false;
    case MCK_GR32_AD: return true;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_TC: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_EDX:
    switch (B) {
    default: return false;
    case MCK_GR32_AD: return true;
    case MCK_Reg20: return true;
    case MCK_Reg21: return true;
    case MCK_Reg22: return true;
    case MCK_Reg23: return true;
    case MCK_Reg25: return true;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_TC: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_GR32_AD:
    switch (B) {
    default: return false;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_TC: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_ECX:
    switch (B) {
    default: return false;
    case MCK_Reg20: return true;
    case MCK_Reg21: return true;
    case MCK_Reg22: return true;
    case MCK_Reg23: return true;
    case MCK_Reg25: return true;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_TC: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_Reg20:
    switch (B) {
    default: return false;
    case MCK_Reg21: return true;
    case MCK_Reg22: return true;
    case MCK_Reg23: return true;
    case MCK_Reg25: return true;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_TC: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_Reg21:
    switch (B) {
    default: return false;
    case MCK_Reg22: return true;
    case MCK_Reg23: return true;
    case MCK_Reg25: return true;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_Reg22:
    switch (B) {
    default: return false;
    case MCK_Reg23: return true;
    case MCK_Reg25: return true;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_Reg23:
    switch (B) {
    default: return false;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32: return true;
    }

  case MCK_Reg25:
    switch (B) {
    default: return false;
    case MCK_GR32_NOAX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_GR32_ABCD:
    switch (B) {
    default: return false;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_GR32_NOAX:
    return B == MCK_GR32;

  case MCK_GR32_TC:
    switch (B) {
    default: return false;
    case MCK_GR32_ABCD: return true;
    case MCK_GR32_NOREX_NOSP: return true;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_GR32_NOREX_NOSP:
    switch (B) {
    default: return false;
    case MCK_GR32_NOREX: return true;
    case MCK_GR32_NOSP: return true;
    case MCK_GR32: return true;
    }

  case MCK_GR32_NOREX:
    return B == MCK_GR32;

  case MCK_GR32_NOSP:
    return B == MCK_GR32;

  case MCK_RAX:
    switch (B) {
    default: return false;
    case MCK_Reg28: return true;
    case MCK_Reg29: return true;
    case MCK_Reg37: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_RDX:
    switch (B) {
    default: return false;
    case MCK_Reg28: return true;
    case MCK_Reg43: return true;
    case MCK_Reg29: return true;
    case MCK_Reg49: return true;
    case MCK_Reg51: return true;
    case MCK_Reg44: return true;
    case MCK_Reg37: return true;
    case MCK_Reg45: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_Reg46: return true;
    case MCK_Reg50: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg28:
    switch (B) {
    default: return false;
    case MCK_Reg29: return true;
    case MCK_Reg37: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_RCX:
    switch (B) {
    default: return false;
    case MCK_Reg43: return true;
    case MCK_Reg29: return true;
    case MCK_Reg49: return true;
    case MCK_Reg51: return true;
    case MCK_Reg44: return true;
    case MCK_Reg37: return true;
    case MCK_Reg45: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_Reg46: return true;
    case MCK_Reg50: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg43:
    switch (B) {
    default: return false;
    case MCK_Reg29: return true;
    case MCK_Reg49: return true;
    case MCK_Reg51: return true;
    case MCK_Reg44: return true;
    case MCK_Reg37: return true;
    case MCK_Reg45: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_Reg46: return true;
    case MCK_Reg50: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg29:
    switch (B) {
    default: return false;
    case MCK_Reg37: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg49:
    switch (B) {
    default: return false;
    case MCK_Reg37: return true;
    case MCK_Reg45: return true;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_Reg46: return true;
    case MCK_Reg50: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg51:
    switch (B) {
    default: return false;
    case MCK_Reg38: return true;
    case MCK_Reg50: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_TCW64: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg44:
    switch (B) {
    default: return false;
    case MCK_Reg45: return true;
    case MCK_Reg46: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_ABCD: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg37:
    switch (B) {
    default: return false;
    case MCK_Reg38: return true;
    case MCK_Reg40: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg45:
    switch (B) {
    default: return false;
    case MCK_Reg46: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg38:
    switch (B) {
    default: return false;
    case MCK_Reg33: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg40:
    switch (B) {
    default: return false;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg46:
    switch (B) {
    default: return false;
    case MCK_Reg47: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg50:
    switch (B) {
    default: return false;
    case MCK_Reg38: return true;
    case MCK_Reg48: return true;
    case MCK_Reg47: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg48:
    switch (B) {
    default: return false;
    case MCK_Reg47: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg47:
    switch (B) {
    default: return false;
    case MCK_Reg33: return true;
    case MCK_GR64: return true;
    }

  case MCK_GR64_ABCD:
    switch (B) {
    default: return false;
    case MCK_GR64_NOREX_NOSP: return true;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64: return true;
    }

  case MCK_GR64_NOREX_NOSP:
    switch (B) {
    default: return false;
    case MCK_Reg32: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg32:
    switch (B) {
    default: return false;
    case MCK_Reg33: return true;
    case MCK_GR64_NOREX: return true;
    case MCK_GR64: return true;
    }

  case MCK_Reg33:
    return B == MCK_GR64;

  case MCK_GR64_NOREX:
    return B == MCK_GR64;

  case MCK_GR64_TCW64:
    switch (B) {
    default: return false;
    case MCK_Reg38: return true;
    case MCK_Reg33: return true;
    case MCK_GR64_NOSP: return true;
    case MCK_GR64_TC: return true;
    case MCK_GR64: return true;
    }

  case MCK_GR64_NOSP:
    switch (B) {
    default: return false;
    case MCK_Reg33: return true;
    case MCK_GR64: return true;
    }

  case MCK_GR64_TC:
    return B == MCK_GR64;

  case MCK_XMM0:
    return B == MCK_FR32;

  case MCK_ST0:
    return B == MCK_RST;

  case MCK_CS:
    return B == MCK_SEGMENT_REG;

  case MCK_DS:
    return B == MCK_SEGMENT_REG;

  case MCK_SS:
    return B == MCK_SEGMENT_REG;

  case MCK_ES:
    return B == MCK_SEGMENT_REG;

  case MCK_FS:
    return B == MCK_SEGMENT_REG;

  case MCK_GS:
    return B == MCK_SEGMENT_REG;

  case MCK_ImmSExti64i8:
    switch (B) {
    default: return false;
    case MCK_ImmSExti16i8: return true;
    case MCK_ImmSExti32i8: return true;
    case MCK_ImmSExti64i32: return true;
    case MCK_Imm: return true;
    }

  case MCK_ImmSExti16i8:
    switch (B) {
    default: return false;
    case MCK_ImmSExti64i32: return true;
    case MCK_Imm: return true;
    }

  case MCK_ImmSExti32i8:
    return B == MCK_Imm;

  case MCK_ImmSExti64i32:
    return B == MCK_Imm;

  case MCK_ImmZExtu32u8:
    return B == MCK_Imm;

  case MCK_AbsMem:
    return B == MCK_Mem;
  }
}

static unsigned validateOperandClass(MCParsedAsmOperand *GOp, MatchClassKind Kind) {
  X86Operand &Operand = *(X86Operand*)GOp;
  if (Kind == InvalidMatchClass)
    return MCTargetAsmParser::Match_InvalidOperand;

  if (Operand.isToken())
    return isSubclass(matchTokenString(Operand.getToken()), Kind) ?
             MCTargetAsmParser::Match_Success :
             MCTargetAsmParser::Match_InvalidOperand;

  // 'ImmSExti64i8' class
  if (Kind == MCK_ImmSExti64i8) {
    if (Operand.isImmSExti64i8())
      return MCTargetAsmParser::Match_Success;
  }

  // 'ImmSExti16i8' class
  if (Kind == MCK_ImmSExti16i8) {
    if (Operand.isImmSExti16i8())
      return MCTargetAsmParser::Match_Success;
  }

  // 'ImmSExti32i8' class
  if (Kind == MCK_ImmSExti32i8) {
    if (Operand.isImmSExti32i8())
      return MCTargetAsmParser::Match_Success;
  }

  // 'ImmSExti64i32' class
  if (Kind == MCK_ImmSExti64i32) {
    if (Operand.isImmSExti64i32())
      return MCTargetAsmParser::Match_Success;
  }

  // 'ImmZExtu32u8' class
  if (Kind == MCK_ImmZExtu32u8) {
    if (Operand.isImmZExtu32u8())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Imm' class
  if (Kind == MCK_Imm) {
    if (Operand.isImm())
      return MCTargetAsmParser::Match_Success;
  }

  // 'AbsMem' class
  if (Kind == MCK_AbsMem) {
    if (Operand.isAbsMem())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem128' class
  if (Kind == MCK_Mem128) {
    if (Operand.isMem128())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem16' class
  if (Kind == MCK_Mem16) {
    if (Operand.isMem16())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem256' class
  if (Kind == MCK_Mem256) {
    if (Operand.isMem256())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem32' class
  if (Kind == MCK_Mem32) {
    if (Operand.isMem32())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem64' class
  if (Kind == MCK_Mem64) {
    if (Operand.isMem64())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem80' class
  if (Kind == MCK_Mem80) {
    if (Operand.isMem80())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem8' class
  if (Kind == MCK_Mem8) {
    if (Operand.isMem8())
      return MCTargetAsmParser::Match_Success;
  }

  // 'Mem' class
  if (Kind == MCK_Mem) {
    if (Operand.isMem())
      return MCTargetAsmParser::Match_Success;
  }

  // 'MemVX32' class
  if (Kind == MCK_MemVX32) {
    if (Operand.isMemVX32())
      return MCTargetAsmParser::Match_Success;
  }

  // 'MemVX64' class
  if (Kind == MCK_MemVX64) {
    if (Operand.isMemVX64())
      return MCTargetAsmParser::Match_Success;
  }

  // 'MemVY32' class
  if (Kind == MCK_MemVY32) {
    if (Operand.isMemVY32())
      return MCTargetAsmParser::Match_Success;
  }

  // 'MemVY64' class
  if (Kind == MCK_MemVY64) {
    if (Operand.isMemVY64())
      return MCTargetAsmParser::Match_Success;
  }

  if (Operand.isReg()) {
    MatchClassKind OpKind;
    switch (Operand.getReg()) {
    default: OpKind = InvalidMatchClass; break;
    case X86::AL: OpKind = MCK_AL; break;
    case X86::DL: OpKind = MCK_GR8_ABCD_L; break;
    case X86::CL: OpKind = MCK_CL; break;
    case X86::BL: OpKind = MCK_GR8_ABCD_L; break;
    case X86::AH: OpKind = MCK_GR8_ABCD_H; break;
    case X86::DH: OpKind = MCK_GR8_ABCD_H; break;
    case X86::CH: OpKind = MCK_GR8_ABCD_H; break;
    case X86::BH: OpKind = MCK_GR8_ABCD_H; break;
    case X86::SIL: OpKind = MCK_GR8; break;
    case X86::DIL: OpKind = MCK_GR8; break;
    case X86::BPL: OpKind = MCK_GR8; break;
    case X86::SPL: OpKind = MCK_GR8; break;
    case X86::R8B: OpKind = MCK_GR8; break;
    case X86::R9B: OpKind = MCK_GR8; break;
    case X86::R10B: OpKind = MCK_GR8; break;
    case X86::R11B: OpKind = MCK_GR8; break;
    case X86::R12B: OpKind = MCK_GR8; break;
    case X86::R13B: OpKind = MCK_GR8; break;
    case X86::R14B: OpKind = MCK_GR8; break;
    case X86::R15B: OpKind = MCK_GR8; break;
    case X86::AX: OpKind = MCK_AX; break;
    case X86::DX: OpKind = MCK_DX; break;
    case X86::CX: OpKind = MCK_GR16_ABCD; break;
    case X86::BX: OpKind = MCK_GR16_ABCD; break;
    case X86::SI: OpKind = MCK_GR16_NOREX; break;
    case X86::DI: OpKind = MCK_GR16_NOREX; break;
    case X86::BP: OpKind = MCK_GR16_NOREX; break;
    case X86::SP: OpKind = MCK_GR16_NOREX; break;
    case X86::R8W: OpKind = MCK_GR16; break;
    case X86::R9W: OpKind = MCK_GR16; break;
    case X86::R10W: OpKind = MCK_GR16; break;
    case X86::R11W: OpKind = MCK_GR16; break;
    case X86::R12W: OpKind = MCK_GR16; break;
    case X86::R13W: OpKind = MCK_GR16; break;
    case X86::R14W: OpKind = MCK_GR16; break;
    case X86::R15W: OpKind = MCK_GR16; break;
    case X86::EAX: OpKind = MCK_EAX; break;
    case X86::EDX: OpKind = MCK_EDX; break;
    case X86::ECX: OpKind = MCK_ECX; break;
    case X86::EBX: OpKind = MCK_Reg21; break;
    case X86::ESI: OpKind = MCK_Reg22; break;
    case X86::EDI: OpKind = MCK_Reg22; break;
    case X86::EBP: OpKind = MCK_Reg22; break;
    case X86::ESP: OpKind = MCK_Reg23; break;
    case X86::R8D: OpKind = MCK_Reg25; break;
    case X86::R9D: OpKind = MCK_Reg25; break;
    case X86::R10D: OpKind = MCK_Reg25; break;
    case X86::R11D: OpKind = MCK_Reg25; break;
    case X86::R12D: OpKind = MCK_Reg25; break;
    case X86::R13D: OpKind = MCK_Reg25; break;
    case X86::R14D: OpKind = MCK_Reg25; break;
    case X86::R15D: OpKind = MCK_Reg25; break;
    case X86::RAX: OpKind = MCK_RAX; break;
    case X86::RDX: OpKind = MCK_RDX; break;
    case X86::RCX: OpKind = MCK_RCX; break;
    case X86::RBX: OpKind = MCK_Reg44; break;
    case X86::RSI: OpKind = MCK_Reg49; break;
    case X86::RDI: OpKind = MCK_Reg49; break;
    case X86::RBP: OpKind = MCK_Reg45; break;
    case X86::RSP: OpKind = MCK_Reg46; break;
    case X86::R8: OpKind = MCK_Reg51; break;
    case X86::R9: OpKind = MCK_Reg51; break;
    case X86::R10: OpKind = MCK_Reg48; break;
    case X86::R11: OpKind = MCK_Reg51; break;
    case X86::R12: OpKind = MCK_Reg48; break;
    case X86::R13: OpKind = MCK_Reg48; break;
    case X86::R14: OpKind = MCK_Reg48; break;
    case X86::R15: OpKind = MCK_Reg48; break;
    case X86::RIP: OpKind = MCK_Reg40; break;
    case X86::MM0: OpKind = MCK_VR64; break;
    case X86::MM1: OpKind = MCK_VR64; break;
    case X86::MM2: OpKind = MCK_VR64; break;
    case X86::MM3: OpKind = MCK_VR64; break;
    case X86::MM4: OpKind = MCK_VR64; break;
    case X86::MM5: OpKind = MCK_VR64; break;
    case X86::MM6: OpKind = MCK_VR64; break;
    case X86::MM7: OpKind = MCK_VR64; break;
    case X86::FP0: OpKind = MCK_RFP32; break;
    case X86::FP1: OpKind = MCK_RFP32; break;
    case X86::FP2: OpKind = MCK_RFP32; break;
    case X86::FP3: OpKind = MCK_RFP32; break;
    case X86::FP4: OpKind = MCK_RFP32; break;
    case X86::FP5: OpKind = MCK_RFP32; break;
    case X86::FP6: OpKind = MCK_RFP32; break;
    case X86::XMM0: OpKind = MCK_XMM0; break;
    case X86::XMM1: OpKind = MCK_FR32; break;
    case X86::XMM2: OpKind = MCK_FR32; break;
    case X86::XMM3: OpKind = MCK_FR32; break;
    case X86::XMM4: OpKind = MCK_FR32; break;
    case X86::XMM5: OpKind = MCK_FR32; break;
    case X86::XMM6: OpKind = MCK_FR32; break;
    case X86::XMM7: OpKind = MCK_FR32; break;
    case X86::XMM8: OpKind = MCK_FR32; break;
    case X86::XMM9: OpKind = MCK_FR32; break;
    case X86::XMM10: OpKind = MCK_FR32; break;
    case X86::XMM11: OpKind = MCK_FR32; break;
    case X86::XMM12: OpKind = MCK_FR32; break;
    case X86::XMM13: OpKind = MCK_FR32; break;
    case X86::XMM14: OpKind = MCK_FR32; break;
    case X86::XMM15: OpKind = MCK_FR32; break;
    case X86::YMM0: OpKind = MCK_VR256; break;
    case X86::YMM1: OpKind = MCK_VR256; break;
    case X86::YMM2: OpKind = MCK_VR256; break;
    case X86::YMM3: OpKind = MCK_VR256; break;
    case X86::YMM4: OpKind = MCK_VR256; break;
    case X86::YMM5: OpKind = MCK_VR256; break;
    case X86::YMM6: OpKind = MCK_VR256; break;
    case X86::YMM7: OpKind = MCK_VR256; break;
    case X86::YMM8: OpKind = MCK_VR256; break;
    case X86::YMM9: OpKind = MCK_VR256; break;
    case X86::YMM10: OpKind = MCK_VR256; break;
    case X86::YMM11: OpKind = MCK_VR256; break;
    case X86::YMM12: OpKind = MCK_VR256; break;
    case X86::YMM13: OpKind = MCK_VR256; break;
    case X86::YMM14: OpKind = MCK_VR256; break;
    case X86::YMM15: OpKind = MCK_VR256; break;
    case X86::ST0: OpKind = MCK_ST0; break;
    case X86::ST1: OpKind = MCK_RST; break;
    case X86::ST2: OpKind = MCK_RST; break;
    case X86::ST3: OpKind = MCK_RST; break;
    case X86::ST4: OpKind = MCK_RST; break;
    case X86::ST5: OpKind = MCK_RST; break;
    case X86::ST6: OpKind = MCK_RST; break;
    case X86::ST7: OpKind = MCK_RST; break;
    case X86::FPSW: OpKind = MCK_FPCCR; break;
    case X86::EFLAGS: OpKind = MCK_CCR; break;
    case X86::CS: OpKind = MCK_CS; break;
    case X86::DS: OpKind = MCK_DS; break;
    case X86::SS: OpKind = MCK_SS; break;
    case X86::ES: OpKind = MCK_ES; break;
    case X86::FS: OpKind = MCK_FS; break;
    case X86::GS: OpKind = MCK_GS; break;
    case X86::DR0: OpKind = MCK_DEBUG_REG; break;
    case X86::DR1: OpKind = MCK_DEBUG_REG; break;
    case X86::DR2: OpKind = MCK_DEBUG_REG; break;
    case X86::DR3: OpKind = MCK_DEBUG_REG; break;
    case X86::DR4: OpKind = MCK_DEBUG_REG; break;
    case X86::DR5: OpKind = MCK_DEBUG_REG; break;
    case X86::DR6: OpKind = MCK_DEBUG_REG; break;
    case X86::DR7: OpKind = MCK_DEBUG_REG; break;
    case X86::CR0: OpKind = MCK_CONTROL_REG; break;
    case X86::CR1: OpKind = MCK_CONTROL_REG; break;
    case X86::CR2: OpKind = MCK_CONTROL_REG; break;
    case X86::CR3: OpKind = MCK_CONTROL_REG; break;
    case X86::CR4: OpKind = MCK_CONTROL_REG; break;
    case X86::CR5: OpKind = MCK_CONTROL_REG; break;
    case X86::CR6: OpKind = MCK_CONTROL_REG; break;
    case X86::CR7: OpKind = MCK_CONTROL_REG; break;
    case X86::CR8: OpKind = MCK_CONTROL_REG; break;
    case X86::CR9: OpKind = MCK_CONTROL_REG; break;
    case X86::CR10: OpKind = MCK_CONTROL_REG; break;
    case X86::CR11: OpKind = MCK_CONTROL_REG; break;
    case X86::CR12: OpKind = MCK_CONTROL_REG; break;
    case X86::CR13: OpKind = MCK_CONTROL_REG; break;
    case X86::CR14: OpKind = MCK_CONTROL_REG; break;
    case X86::CR15: OpKind = MCK_CONTROL_REG; break;
    }
    return isSubclass(OpKind, Kind) ? MCTargetAsmParser::Match_Success :
                                      MCTargetAsmParser::Match_InvalidOperand;
  }

  return MCTargetAsmParser::Match_InvalidOperand;
}

unsigned X86AsmParser::
ComputeAvailableFeatures(uint64_t FB) const {
  unsigned Features = 0;
  if (((FB & X86::Mode64Bit) == 0))
    Features |= Feature_In32BitMode;
  if (((FB & X86::Mode64Bit) != 0))
    Features |= Feature_In64BitMode;
  return Features;
}

static const char *const MnemonicTable =
    "\003aaa\003aad\003aam\003aas\003adc\004adcb\004adcl\004adcq\004adcw\003"
    "add\004addb\004addl\005addpd\005addps\004addq\005addsd\005addss\010adds"
    "ubpd\010addsubps\004addw\006aesdec\naesdeclast\006aesenc\naesenclast\006"
    "aesimc\017aeskeygenassist\003and\004andb\004andl\004andn\005andnl\006an"
    "dnpd\006andnps\005andnq\005andpd\005andps\004andq\004andw\004arpl\005be"
    "xtr\006bextrl\006bextrq\007blendpd\007blendps\010blendvpd\010blendvps\004"
    "blsi\005blsil\005blsiq\006blsmsk\007blsmskl\007blsmskq\004blsr\005blsrl"
    "\005blsrq\005bound\003bsf\004bsfl\004bsfq\004bsfw\003bsr\004bsrl\004bsr"
    "q\004bsrw\005bswap\006bswapl\006bswapq\002bt\003btc\004btcl\004btcq\004"
    "btcw\003btl\003btq\003btr\004btrl\004btrq\004btrw\003bts\004btsl\004bts"
    "q\004btsw\003btw\004bzhi\005bzhil\005bzhiq\004call\005calll\005callq\005"
    "callw\004cbtw\003cbw\003cdq\004cdqe\003clc\003cld\007clflush\004clgi\003"
    "cli\004clrb\004clrl\004clrq\004clrw\004cltd\004cltq\004clts\003cmc\005c"
    "mova\006cmovae\007cmovael\007cmovaeq\007cmovaew\006cmoval\006cmovaq\006"
    "cmovaw\005cmovb\006cmovbe\007cmovbel\007cmovbeq\007cmovbew\006cmovbl\006"
    "cmovbq\006cmovbw\005cmove\006cmovel\006cmoveq\006cmovew\005cmovg\006cmo"
    "vge\007cmovgel\007cmovgeq\007cmovgew\006cmovgl\006cmovgq\006cmovgw\005c"
    "movl\006cmovle\007cmovlel\007cmovleq\007cmovlew\006cmovll\006cmovlq\006"
    "cmovlw\006cmovne\007cmovnel\007cmovneq\007cmovnew\006cmovno\007cmovnol\007"
    "cmovnoq\007cmovnow\006cmovnp\007cmovnpl\007cmovnpq\007cmovnpw\006cmovns"
    "\007cmovnsl\007cmovnsq\007cmovnsw\005cmovo\006cmovol\006cmovoq\006cmovo"
    "w\005cmovp\006cmovpl\006cmovpq\006cmovpw\005cmovs\006cmovsl\006cmovsq\006"
    "cmovsw\003cmp\004cmpb\004cmpl\005cmppd\005cmpps\004cmpq\005cmpsb\005cmp"
    "sd\005cmpsl\005cmpsq\005cmpss\005cmpsw\004cmpw\007cmpxchg\ncmpxchg16b\t"
    "cmpxchg8b\010cmpxchgb\010cmpxchgl\010cmpxchgq\010cmpxchgw\006comisd\006"
    "comiss\005cpuid\003cqo\004cqto\005crc32\006crc32b\006crc32l\006crc32q\006"
    "crc32w\002cs\010cvtdq2pd\010cvtdq2ps\010cvtpd2dq\010cvtpd2pi\010cvtpd2p"
    "s\010cvtpi2pd\010cvtpi2ps\010cvtps2dq\010cvtps2pd\010cvtps2pi\010cvtsd2"
    "si\tcvtsd2sil\tcvtsd2siq\010cvtsd2ss\010cvtsi2sd\tcvtsi2sdq\010cvtsi2ss"
    "\tcvtsi2ssq\010cvtss2sd\010cvtss2si\tcvtss2sil\tcvtss2siq\tcvttpd2dq\tc"
    "vttpd2pi\tcvttps2dq\tcvttps2pi\tcvttsd2si\ncvttsd2siq\tcvttss2si\ncvtts"
    "s2siq\003cwd\004cwde\004cwtd\004cwtl\003daa\003das\006data16\003dec\004"
    "decb\004decl\004decq\004decw\003div\004divb\004divl\005divpd\005divps\004"
    "divq\005divsd\005divss\004divw\004dppd\004dpps\002ds\004emms\005enter\002"
    "es\textractps\005extrq\005f2xm1\004fabs\004fadd\005faddl\005faddp\005fa"
    "dds\004fbld\005fbstp\004fchs\006fcmovb\007fcmovbe\006fcmove\007fcmovnb\010"
    "fcmovnbe\007fcmovne\007fcmovnu\006fcmovu\004fcom\005fcomi\005fcoml\005f"
    "comp\006fcompi\006fcompl\006fcompp\006fcomps\005fcoms\004fcos\007fdecst"
    "p\004fdiv\005fdivl\005fdivp\005fdivr\006fdivrl\006fdivrp\006fdivrs\005f"
    "divs\005femms\005ffree\005fiadd\006fiaddl\006fiadds\005ficom\006ficoml\006"
    "ficomp\007ficompl\007ficomps\006ficoms\005fidiv\006fidivl\006fidivr\007"
    "fidivrl\007fidivrs\006fidivs\004fild\005fildl\006fildll\005filds\005fim"
    "ul\006fimull\006fimuls\007fincstp\004fist\005fistl\005fistp\006fistpl\007"
    "fistpll\006fistps\005fists\006fisttp\007fisttpl\010fisttpll\007fisttps\005"
    "fisub\006fisubl\006fisubr\007fisubrl\007fisubrs\006fisubs\003fld\004fld"
    "1\005fldcw\006fldenv\004fldl\006fldl2e\006fldl2t\006fldlg2\006fldln2\005"
    "fldpi\004flds\004fldt\004fldz\004fmul\005fmull\005fmulp\005fmuls\006fnc"
    "lex\006fninit\004fnop\006fnsave\006fnstcw\007fnstenv\006fnstsw\006fpata"
    "n\005fprem\006fprem1\005fptan\007frndint\006frstor\002fs\006fscale\004f"
    "sin\007fsincos\005fsqrt\003fst\004fstl\004fstp\005fstpl\005fstps\005fst"
    "pt\004fsts\004fsub\005fsubl\005fsubp\005fsubr\006fsubrl\006fsubrp\006fs"
    "ubrs\005fsubs\004ftst\005fucom\006fucomi\006fucomp\007fucompi\007fucomp"
    "p\004fxam\004fxch\007fxrstor\010fxrstorq\006fxsave\007fxsaveq\007fxtrac"
    "t\005fyl2x\007fyl2xp1\002gs\006haddpd\006haddps\003hlt\006hsubpd\006hsu"
    "bps\004idiv\005idivb\005idivl\005idivq\005idivw\004imul\005imulb\005imu"
    "ll\005imulq\005imulw\002in\003inb\003inc\004incb\004incl\004incq\004inc"
    "w\003inl\003ins\004insb\010insertps\007insertq\004insl\004insw\003int\004"
    "int3\004into\004invd\006invept\006invlpg\007invlpga\007invpcid\007invvp"
    "id\003inw\004iret\005iretd\005iretl\005iretq\005iretw\002ja\003jae\002j"
    "b\003jbe\004jcxz\002je\005jecxz\002jg\003jge\002jl\003jle\003jmp\004jmp"
    "l\004jmpq\004jmpw\003jne\003jno\003jnp\003jns\002jo\002jp\005jrcxz\002j"
    "s\004lahf\003lar\004larl\004larq\004larw\005lcall\006lcalll\006lcallq\006"
    "lcallw\005lddqu\007ldmxcsr\003lds\004ldsl\004ldsw\003lea\004leal\004lea"
    "q\005leave\004leaw\003les\004lesl\004lesw\006lfence\003lfs\004lfsl\004l"
    "fsq\004lfsw\004lgdt\005lgdtw\003lgs\004lgsl\004lgsq\004lgsw\004lidt\005"
    "lidtw\004ljmp\005ljmpl\005ljmpq\005ljmpw\004lldt\005lldtw\004lmsw\005lm"
    "sww\004lock\005lodsb\005lodsd\005lodsl\005lodsq\005lodsw\004loop\005loo"
    "pe\006loopne\005lretl\005lretq\005lretw\003lsl\004lsll\004lslq\004lslw\003"
    "lss\004lssl\004lssq\004lssw\003ltr\004ltrw\005lzcnt\006lzcntl\006lzcntq"
    "\006lzcntw\nmaskmovdqu\010maskmovq\005maxpd\005maxps\005maxsd\005maxss\006"
    "mfence\005minpd\005minps\005minsd\005minss\007monitor\007montmul\003mov"
    "\006movabs\007movabsq\006movapd\006movaps\004movb\005movbe\006movbel\006"
    "movbeq\006movbew\004movd\007movddup\007movdq2q\006movdqa\006movdqu\007m"
    "ovhlps\006movhpd\006movhps\004movl\007movlhps\006movlpd\006movlps\010mo"
    "vmskpd\010movmskps\007movntdq\010movntdqa\006movnti\007movntil\007movnt"
    "iq\007movntpd\007movntps\006movntq\007movntsd\007movntss\004movq\007mov"
    "q2dq\005movsb\006movsbl\006movsbq\006movsbw\005movsd\010movshdup\005mov"
    "sl\010movsldup\006movslq\005movsq\005movss\005movsw\006movswl\006movswq"
    "\005movsx\006movsxd\006movupd\006movups\004movw\006movzbl\006movzbq\006"
    "movzbw\006movzwl\006movzwq\005movzx\007mpsadbw\003mul\004mulb\004mull\005"
    "mulpd\005mulps\004mulq\005mulsd\005mulss\004mulw\004mulx\005mulxl\005mu"
    "lxq\005mwait\003neg\004negb\004negl\004negq\004negw\003nop\004nopl\004n"
    "opw\003not\004notb\004notl\004notq\004notw\002or\003orb\003orl\004orpd\004"
    "orps\003orq\003orw\003out\004outb\004outl\005outsb\005outsd\005outsl\005"
    "outsw\004outw\005pabsb\005pabsd\005pabsw\010packssdw\010packsswb\010pac"
    "kusdw\010packuswb\005paddb\005paddd\005paddq\006paddsb\006paddsw\007pad"
    "dusb\007paddusw\005paddw\007palignr\004pand\005pandn\005pause\005pavgb\007"
    "pavgusb\005pavgw\010pblendvb\007pblendw\014pclmulhqhqdq\014pclmulhqlqdq"
    "\014pclmullqhqdq\014pclmullqlqdq\tpclmulqdq\007pcmpeqb\007pcmpeqd\007pc"
    "mpeqq\007pcmpeqw\tpcmpestri\tpcmpestrm\007pcmpgtb\007pcmpgtd\007pcmpgtq"
    "\007pcmpgtw\tpcmpistri\tpcmpistrm\004pdep\005pdepl\005pdepq\004pext\005"
    "pextl\005pextq\006pextrb\006pextrd\006pextrq\006pextrw\005pf2id\005pf2i"
    "w\005pfacc\005pfadd\007pfcmpeq\007pfcmpge\007pfcmpgt\005pfmax\005pfmin\005"
    "pfmul\006pfnacc\007pfpnacc\005pfrcp\010pfrcpit1\010pfrcpit2\010pfrsqit1"
    "\007pfrsqrt\005pfsub\006pfsubr\006phaddd\007phaddsw\006phaddw\nphminpos"
    "uw\006phsubd\007phsubsw\006phsubw\005pi2fd\005pi2fw\006pinsrb\006pinsrd"
    "\006pinsrq\006pinsrw\tpmaddubsw\007pmaddwd\006pmaxsb\006pmaxsd\006pmaxs"
    "w\006pmaxub\006pmaxud\006pmaxuw\006pminsb\006pminsd\006pminsw\006pminub"
    "\006pminud\006pminuw\010pmovmskb\010pmovsxbd\010pmovsxbq\010pmovsxbw\010"
    "pmovsxdq\010pmovsxwd\010pmovsxwq\010pmovzxbd\010pmovzxbq\010pmovzxbw\010"
    "pmovzxdq\010pmovzxwd\010pmovzxwq\006pmuldq\010pmulhrsw\007pmulhrw\007pm"
    "ulhuw\006pmulhw\006pmulld\006pmullw\007pmuludq\003pop\004popa\005popal\006"
    "popcnt\007popcntl\007popcntq\007popcntw\004popf\005popfd\005popfl\005po"
    "pfq\005popfw\004popl\004popq\004popw\003por\010prefetch\013prefetchnta\n"
    "prefetcht0\nprefetcht1\nprefetcht2\tprefetchw\006psadbw\006pshufb\006ps"
    "hufd\007pshufhw\007pshuflw\006pshufw\006psignb\006psignd\006psignw\005p"
    "slld\006pslldq\005psllq\005psllw\005psrad\005psraw\005psrld\006psrldq\005"
    "psrlq\005psrlw\005psubb\005psubd\005psubq\006psubsb\006psubsw\007psubus"
    "b\007psubusw\005psubw\006pswapd\005ptest\tpunpckhbw\tpunpckhdq\npunpckh"
    "qdq\tpunpckhwd\tpunpcklbw\tpunpckldq\npunpcklqdq\tpunpcklwd\004push\005"
    "pusha\006pushal\005pushf\006pushfd\006pushfl\006pushfq\006pushfw\005pus"
    "hl\005pushq\005pushw\004pxor\003rcl\004rclb\004rcll\004rclq\004rclw\005"
    "rcpps\005rcpss\003rcr\004rcrb\004rcrl\004rcrq\004rcrw\010rdfsbase\trdfs"
    "basel\trdfsbaseq\010rdgsbase\trdgsbasel\trdgsbaseq\005rdmsr\005rdpmc\006"
    "rdrand\007rdrandl\007rdrandq\007rdrandw\005rdtsc\006rdtscp\003rep\005re"
    "pne\003ret\004retf\004retw\005rex64\003rol\004rolb\004roll\004rolq\004r"
    "olw\003ror\004rorb\004rorl\004rorq\004rorw\004rorx\005rorxl\005rorxq\007"
    "roundpd\007roundps\007roundsd\007roundss\003rsm\007rsqrtps\007rsqrtss\004"
    "sahf\003sar\004sarb\004sarl\004sarq\004sarw\004sarx\005sarxl\005sarxq\003"
    "sbb\004sbbb\004sbbl\004sbbq\004sbbw\005scasb\005scasd\005scasl\005scasq"
    "\005scasw\004seta\005setae\004setb\005setbe\004sete\004setg\005setge\004"
    "setl\005setle\005setne\005setno\005setnp\005setns\004seto\004setp\004se"
    "ts\006sfence\004sgdt\005sgdtw\003shl\004shlb\004shld\005shldl\005shldq\005"
    "shldw\004shll\004shlq\004shlw\004shlx\005shlxl\005shlxq\003shr\004shrb\004"
    "shrd\005shrdl\005shrdq\005shrdw\004shrl\004shrq\004shrw\004shrx\005shrx"
    "l\005shrxq\006shufpd\006shufps\004sidt\005sidtw\006skinit\004sldt\005sl"
    "dtl\005sldtq\005sldtw\004smsw\005smswl\005smswq\005smsww\006sqrtpd\006s"
    "qrtps\006sqrtsd\006sqrtss\002ss\003stc\003std\004stgi\003sti\007stmxcsr"
    "\005stosb\005stosd\005stosl\005stosq\005stosw\003str\004strl\004strq\004"
    "strw\003sub\004subb\004subl\005subpd\005subps\004subq\005subsd\005subss"
    "\004subw\006swapgs\007syscall\010sysenter\007sysexit\010sysexitl\010sys"
    "exitq\006sysret\007sysretl\007sysretq\004test\005testb\005testl\005test"
    "q\005testw\005tzcnt\006tzcntl\006tzcntq\006tzcntw\007ucomisd\007ucomiss"
    "\003ud2\004ud2b\010unpckhpd\010unpckhps\010unpcklpd\010unpcklps\006vadd"
    "pd\006vaddps\006vaddsd\006vaddss\tvaddsubpd\tvaddsubps\007vaesdec\013va"
    "esdeclast\007vaesenc\013vaesenclast\007vaesimc\020vaeskeygenassist\007v"
    "andnpd\007vandnps\006vandpd\006vandps\010vblendpd\010vblendps\tvblendvp"
    "d\tvblendvps\016vbroadcastf128\016vbroadcasti128\014vbroadcastsd\014vbr"
    "oadcastss\004vcmp\006vcmppd\006vcmpps\006vcmpsd\006vcmpss\007vcomisd\007"
    "vcomiss\tvcvtdq2pd\tvcvtdq2ps\tvcvtpd2dq\nvcvtpd2dqx\nvcvtpd2dqy\tvcvtp"
    "d2ps\nvcvtpd2psx\nvcvtpd2psy\tvcvtph2ps\tvcvtps2dq\tvcvtps2pd\tvcvtps2p"
    "h\tvcvtsd2si\nvcvtsd2sil\nvcvtsd2siq\tvcvtsd2ss\tvcvtsi2sd\nvcvtsi2sdl\n"
    "vcvtsi2sdq\tvcvtsi2ss\nvcvtsi2ssq\tvcvtss2sd\tvcvtss2si\nvcvtss2sil\nvc"
    "vtss2siq\nvcvttpd2dq\013vcvttpd2dqx\013vcvttpd2dqy\nvcvttps2dq\nvcvttsd"
    "2si\013vcvttsd2siq\nvcvttss2si\013vcvttss2siq\006vdivpd\006vdivps\006vd"
    "ivsd\006vdivss\005vdppd\005vdpps\004verr\004verw\014vextractf128\014vex"
    "tracti128\nvextractps\013vfmadd132pd\013vfmadd132ps\013vfmadd132sd\013v"
    "fmadd132ss\013vfmadd213pd\013vfmadd213ps\013vfmadd213sd\013vfmadd213ss\013"
    "vfmadd231pd\013vfmadd231ps\013vfmadd231sd\013vfmadd231ss\010vfmaddpd\010"
    "vfmaddps\010vfmaddsd\010vfmaddss\016vfmaddsub132pd\016vfmaddsub132ps\016"
    "vfmaddsub213pd\016vfmaddsub213ps\016vfmaddsub231pd\016vfmaddsub231ps\013"
    "vfmaddsubpd\013vfmaddsubps\013vfmsub132pd\013vfmsub132ps\013vfmsub132sd"
    "\013vfmsub132ss\013vfmsub213pd\013vfmsub213ps\013vfmsub213sd\013vfmsub2"
    "13ss\013vfmsub231pd\013vfmsub231ps\013vfmsub231sd\013vfmsub231ss\016vfm"
    "subadd132pd\016vfmsubadd132ps\016vfmsubadd213pd\016vfmsubadd213ps\016vf"
    "msubadd231pd\016vfmsubadd231ps\013vfmsubaddpd\013vfmsubaddps\010vfmsubp"
    "d\010vfmsubps\010vfmsubsd\010vfmsubss\014vfnmadd132pd\014vfnmadd132ps\014"
    "vfnmadd132sd\014vfnmadd132ss\014vfnmadd213pd\014vfnmadd213ps\014vfnmadd"
    "213sd\014vfnmadd213ss\014vfnmadd231pd\014vfnmadd231ps\014vfnmadd231sd\014"
    "vfnmadd231ss\tvfnmaddpd\tvfnmaddps\tvfnmaddsd\tvfnmaddss\014vfnmsub132p"
    "d\014vfnmsub132ps\014vfnmsub132sd\014vfnmsub132ss\014vfnmsub213pd\014vf"
    "nmsub213ps\014vfnmsub213sd\014vfnmsub213ss\014vfnmsub231pd\014vfnmsub23"
    "1ps\014vfnmsub231sd\014vfnmsub231ss\tvfnmsubpd\tvfnmsubps\tvfnmsubsd\tv"
    "fnmsubss\007vfrczpd\007vfrczps\007vfrczsd\007vfrczss\nvgatherdpd\nvgath"
    "erdps\nvgatherqpd\nvgatherqps\007vhaddpd\007vhaddps\007vhsubpd\007vhsub"
    "ps\013vinsertf128\013vinserti128\tvinsertps\006vlddqu\010vldmxcsr\013vm"
    "askmovdqu\nvmaskmovpd\nvmaskmovps\006vmaxpd\006vmaxps\006vmaxsd\006vmax"
    "ss\006vmcall\007vmclear\006vmfunc\006vminpd\006vminps\006vminsd\006vmin"
    "ss\010vmlaunch\006vmload\007vmmcall\007vmovapd\007vmovaps\005vmovd\010v"
    "movddup\007vmovdqa\007vmovdqu\010vmovhlps\007vmovhpd\007vmovhps\010vmov"
    "lhps\007vmovlpd\007vmovlps\tvmovmskpd\tvmovmskps\010vmovntdq\tvmovntdqa"
    "\010vmovntpd\010vmovntps\005vmovq\006vmovsd\tvmovshdup\tvmovsldup\006vm"
    "ovss\007vmovupd\007vmovups\010vmpsadbw\007vmptrld\007vmptrst\006vmread\007"
    "vmreadl\007vmreadq\010vmresume\005vmrun\006vmsave\006vmulpd\006vmulps\006"
    "vmulsd\006vmulss\007vmwrite\010vmwritel\010vmwriteq\006vmxoff\005vmxon\005"
    "vorpd\005vorps\006vpabsb\006vpabsd\006vpabsw\tvpackssdw\tvpacksswb\tvpa"
    "ckusdw\tvpackuswb\006vpaddb\006vpaddd\006vpaddq\007vpaddsb\007vpaddsw\010"
    "vpaddusb\010vpaddusw\006vpaddw\010vpalignr\005vpand\006vpandn\006vpavgb"
    "\006vpavgw\010vpblendd\tvpblendvb\010vpblendw\014vpbroadcastb\014vpbroa"
    "dcastd\014vpbroadcastq\014vpbroadcastw\015vpclmulhqhqdq\015vpclmulhqlqd"
    "q\015vpclmullqhqdq\015vpclmullqlqdq\nvpclmulqdq\006vpcmov\010vpcmpeqb\010"
    "vpcmpeqd\010vpcmpeqq\010vpcmpeqw\nvpcmpestri\nvpcmpestrm\010vpcmpgtb\010"
    "vpcmpgtd\010vpcmpgtq\010vpcmpgtw\nvpcmpistri\nvpcmpistrm\006vpcomb\006v"
    "pcomd\006vpcomq\007vpcomub\007vpcomud\007vpcomuq\007vpcomuw\006vpcomw\n"
    "vperm2f128\nvperm2i128\006vpermd\nvpermil2pd\nvpermil2ps\tvpermilpd\tvp"
    "ermilps\007vpermpd\007vpermps\006vpermq\007vpextrb\007vpextrd\007vpextr"
    "q\007vpextrw\nvpgatherdd\nvpgatherdq\nvpgatherqd\nvpgatherqq\010vphaddb"
    "d\010vphaddbq\010vphaddbw\007vphaddd\010vphadddq\010vphaddsw\tvphaddubd"
    "\tvphaddubq\tvphaddubw\tvphaddudq\tvphadduwd\tvphadduwq\007vphaddw\010v"
    "phaddwd\010vphaddwq\013vphminposuw\010vphsubbw\007vphsubd\010vphsubdq\010"
    "vphsubsw\007vphsubw\010vphsubwd\007vpinsrb\007vpinsrd\007vpinsrq\007vpi"
    "nsrw\010vpmacsdd\tvpmacsdqh\tvpmacsdql\tvpmacssdd\nvpmacssdqh\nvpmacssd"
    "ql\tvpmacsswd\tvpmacssww\010vpmacswd\010vpmacsww\nvpmadcsswd\tvpmadcswd"
    "\nvpmaddubsw\010vpmaddwd\nvpmaskmovd\nvpmaskmovq\007vpmaxsb\007vpmaxsd\007"
    "vpmaxsw\007vpmaxub\007vpmaxud\007vpmaxuw\007vpminsb\007vpminsd\007vpmin"
    "sw\007vpminub\007vpminud\007vpminuw\tvpmovmskb\tvpmovsxbd\tvpmovsxbq\tv"
    "pmovsxbw\tvpmovsxdq\tvpmovsxwd\tvpmovsxwq\tvpmovzxbd\tvpmovzxbq\tvpmovz"
    "xbw\tvpmovzxdq\tvpmovzxwd\tvpmovzxwq\007vpmuldq\tvpmulhrsw\010vpmulhuw\007"
    "vpmulhw\007vpmulld\007vpmullw\010vpmuludq\004vpor\006vpperm\006vprotb\006"
    "vprotd\006vprotq\006vprotw\007vpsadbw\006vpshab\006vpshad\006vpshaq\006"
    "vpshaw\006vpshlb\006vpshld\006vpshlq\006vpshlw\007vpshufb\007vpshufd\010"
    "vpshufhw\010vpshuflw\007vpsignb\007vpsignd\007vpsignw\006vpslld\007vpsl"
    "ldq\006vpsllq\007vpsllvd\007vpsllvq\006vpsllw\006vpsrad\007vpsravd\006v"
    "psraw\006vpsrld\007vpsrldq\006vpsrlq\007vpsrlvd\007vpsrlvq\006vpsrlw\006"
    "vpsubb\006vpsubd\006vpsubq\007vpsubsb\007vpsubsw\010vpsubusb\010vpsubus"
    "w\006vpsubw\006vptest\nvpunpckhbw\nvpunpckhdq\013vpunpckhqdq\nvpunpckhw"
    "d\nvpunpcklbw\nvpunpckldq\013vpunpcklqdq\nvpunpcklwd\005vpxor\006vrcpps"
    "\006vrcpss\010vroundpd\010vroundps\010vroundsd\010vroundss\010vrsqrtps\010"
    "vrsqrtss\007vshufpd\007vshufps\007vsqrtpd\007vsqrtps\007vsqrtsd\007vsqr"
    "tss\010vstmxcsr\006vsubpd\006vsubps\006vsubsd\006vsubss\007vtestpd\007v"
    "testps\010vucomisd\010vucomiss\tvunpckhpd\tvunpckhps\tvunpcklpd\tvunpck"
    "lps\006vxorpd\006vxorps\010vzeroall\nvzeroupper\004wait\006wbinvd\010wr"
    "fsbase\twrfsbasel\twrfsbaseq\010wrgsbase\twrgsbasel\twrgsbaseq\005wrmsr"
    "\006xabort\004xadd\005xaddb\005xaddl\005xaddq\005xaddw\006xbegin\004xch"
    "g\005xchgb\005xchgl\005xchgq\005xchgw\txcryptcbc\txcryptcfb\txcryptctr\t"
    "xcryptecb\txcryptofb\004xend\006xgetbv\005xlatb\003xor\004xorb\004xorl\005"
    "xorpd\005xorps\004xorq\004xorw\006xrstor\007xrstorq\005xsave\010xsaveop"
    "t\txsaveoptq\006xsaveq\006xsetbv\005xsha1\007xsha256\006xstore\txstorer"
    "ng";

namespace {
  struct MatchEntry {
    uint16_t Mnemonic;
    uint16_t Opcode;
    uint16_t ConvertFn;
    uint8_t RequiredFeatures;
    uint8_t Classes[5];
    uint8_t AsmVariantID;

    StringRef getMnemonic() const {
      return StringRef(MnemonicTable + Mnemonic + 1,
                       MnemonicTable[Mnemonic]);
    }
  };

  // Predicate for searching for an opcode.
  struct LessOpcode {
    bool operator()(const MatchEntry &LHS, StringRef RHS) {
      return LHS.getMnemonic() < RHS;
    }
    bool operator()(StringRef LHS, const MatchEntry &RHS) {
      return LHS < RHS.getMnemonic();
    }
    bool operator()(const MatchEntry &LHS, const MatchEntry &RHS) {
      return LHS.getMnemonic() < RHS.getMnemonic();
    }
  };
} // end anonymous namespace.

static const MatchEntry MatchTable[7830] = {
  { 0 /* aaa */, X86::AAA, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 0 /* aaa */, X86::AAA, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 4 /* aad */, X86::AAD8i8, Convert__imm_10, 0, {  }, 0},
  { 4 /* aad */, X86::AAD8i8, Convert__imm_10, 0, {  }, 1},
  { 4 /* aad */, X86::AAD8i8, Convert__Imm1_0, Feature_In32BitMode, { MCK_Imm }, 0},
  { 4 /* aad */, X86::AAD8i8, Convert__Imm1_0, Feature_In32BitMode, { MCK_Imm }, 1},
  { 8 /* aam */, X86::AAM8i8, Convert__imm_10, 0, {  }, 0},
  { 8 /* aam */, X86::AAM8i8, Convert__imm_10, 0, {  }, 1},
  { 8 /* aam */, X86::AAM8i8, Convert__Imm1_0, Feature_In32BitMode, { MCK_Imm }, 0},
  { 8 /* aam */, X86::AAM8i8, Convert__Imm1_0, Feature_In32BitMode, { MCK_Imm }, 1},
  { 12 /* aas */, X86::AAS, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 12 /* aas */, X86::AAS, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 16 /* adc */, X86::ADC8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 16 /* adc */, X86::ADC8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 16 /* adc */, X86::ADC16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 16 /* adc */, X86::ADC16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 16 /* adc */, X86::ADC16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 16 /* adc */, X86::ADC32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 16 /* adc */, X86::ADC32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 16 /* adc */, X86::ADC32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 16 /* adc */, X86::ADC64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 16 /* adc */, X86::ADC64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 16 /* adc */, X86::ADC64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 16 /* adc */, X86::ADC64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 16 /* adc */, X86::ADC64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 16 /* adc */, X86::ADC16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 16 /* adc */, X86::ADC16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 16 /* adc */, X86::ADC16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 16 /* adc */, X86::ADC32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 16 /* adc */, X86::ADC32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 16 /* adc */, X86::ADC64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 16 /* adc */, X86::ADC64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 16 /* adc */, X86::ADC64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 16 /* adc */, X86::ADC8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 16 /* adc */, X86::ADC8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 20 /* adcb */, X86::ADC8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 20 /* adcb */, X86::ADC8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 20 /* adcb */, X86::ADC8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 20 /* adcb */, X86::ADC8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 20 /* adcb */, X86::ADC8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 20 /* adcb */, X86::ADC8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 25 /* adcl */, X86::ADC32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 25 /* adcl */, X86::ADC32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 25 /* adcl */, X86::ADC32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 25 /* adcl */, X86::ADC32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 25 /* adcl */, X86::ADC32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 25 /* adcl */, X86::ADC32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 25 /* adcl */, X86::ADC32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 25 /* adcl */, X86::ADC32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 30 /* adcq */, X86::ADC64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 30 /* adcq */, X86::ADC64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 30 /* adcq */, X86::ADC64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 30 /* adcq */, X86::ADC64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 30 /* adcq */, X86::ADC64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 30 /* adcq */, X86::ADC64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 30 /* adcq */, X86::ADC64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 30 /* adcq */, X86::ADC64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 35 /* adcw */, X86::ADC16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 35 /* adcw */, X86::ADC16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 35 /* adcw */, X86::ADC16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 35 /* adcw */, X86::ADC16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 35 /* adcw */, X86::ADC16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 35 /* adcw */, X86::ADC16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 35 /* adcw */, X86::ADC16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 35 /* adcw */, X86::ADC16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 40 /* add */, X86::ADD8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 40 /* add */, X86::ADD8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 40 /* add */, X86::ADD16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 40 /* add */, X86::ADD16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 40 /* add */, X86::ADD16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 40 /* add */, X86::ADD32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 40 /* add */, X86::ADD32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 40 /* add */, X86::ADD32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 40 /* add */, X86::ADD64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 40 /* add */, X86::ADD64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 40 /* add */, X86::ADD64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 40 /* add */, X86::ADD64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 40 /* add */, X86::ADD64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 40 /* add */, X86::ADD16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 40 /* add */, X86::ADD16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 40 /* add */, X86::ADD16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 40 /* add */, X86::ADD32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 40 /* add */, X86::ADD32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 40 /* add */, X86::ADD64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 40 /* add */, X86::ADD64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 40 /* add */, X86::ADD64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 40 /* add */, X86::ADD8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 40 /* add */, X86::ADD8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 44 /* addb */, X86::ADD8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 44 /* addb */, X86::ADD8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 44 /* addb */, X86::ADD8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 44 /* addb */, X86::ADD8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 44 /* addb */, X86::ADD8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 44 /* addb */, X86::ADD8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 49 /* addl */, X86::ADD32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 49 /* addl */, X86::ADD32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 49 /* addl */, X86::ADD32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 49 /* addl */, X86::ADD32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 49 /* addl */, X86::ADD32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 49 /* addl */, X86::ADD32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 49 /* addl */, X86::ADD32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 49 /* addl */, X86::ADD32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 54 /* addpd */, X86::ADDPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 54 /* addpd */, X86::ADDPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 54 /* addpd */, X86::ADDPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 54 /* addpd */, X86::ADDPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 60 /* addps */, X86::ADDPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 60 /* addps */, X86::ADDPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 60 /* addps */, X86::ADDPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 60 /* addps */, X86::ADDPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 66 /* addq */, X86::ADD64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 66 /* addq */, X86::ADD64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 66 /* addq */, X86::ADD64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 66 /* addq */, X86::ADD64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 66 /* addq */, X86::ADD64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 66 /* addq */, X86::ADD64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 66 /* addq */, X86::ADD64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 66 /* addq */, X86::ADD64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 71 /* addsd */, X86::ADDSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 71 /* addsd */, X86::ADDSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 71 /* addsd */, X86::ADDSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 71 /* addsd */, X86::ADDSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 77 /* addss */, X86::ADDSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 77 /* addss */, X86::ADDSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 77 /* addss */, X86::ADDSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 77 /* addss */, X86::ADDSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 83 /* addsubpd */, X86::ADDSUBPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 83 /* addsubpd */, X86::ADDSUBPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 83 /* addsubpd */, X86::ADDSUBPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 83 /* addsubpd */, X86::ADDSUBPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 92 /* addsubps */, X86::ADDSUBPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 92 /* addsubps */, X86::ADDSUBPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 92 /* addsubps */, X86::ADDSUBPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 92 /* addsubps */, X86::ADDSUBPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 101 /* addw */, X86::ADD16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 101 /* addw */, X86::ADD16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 101 /* addw */, X86::ADD16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 101 /* addw */, X86::ADD16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 101 /* addw */, X86::ADD16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 101 /* addw */, X86::ADD16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 101 /* addw */, X86::ADD16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 101 /* addw */, X86::ADD16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 106 /* aesdec */, X86::AESDECrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 106 /* aesdec */, X86::AESDECrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 106 /* aesdec */, X86::AESDECrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 106 /* aesdec */, X86::AESDECrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 113 /* aesdeclast */, X86::AESDECLASTrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 113 /* aesdeclast */, X86::AESDECLASTrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 113 /* aesdeclast */, X86::AESDECLASTrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 113 /* aesdeclast */, X86::AESDECLASTrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 124 /* aesenc */, X86::AESENCrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 124 /* aesenc */, X86::AESENCrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 124 /* aesenc */, X86::AESENCrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 124 /* aesenc */, X86::AESENCrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 131 /* aesenclast */, X86::AESENCLASTrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 131 /* aesenclast */, X86::AESENCLASTrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 131 /* aesenclast */, X86::AESENCLASTrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 131 /* aesenclast */, X86::AESENCLASTrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 142 /* aesimc */, X86::AESIMCrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 142 /* aesimc */, X86::AESIMCrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 142 /* aesimc */, X86::AESIMCrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 142 /* aesimc */, X86::AESIMCrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 149 /* aeskeygenassist */, X86::AESKEYGENASSIST128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 149 /* aeskeygenassist */, X86::AESKEYGENASSIST128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 149 /* aeskeygenassist */, X86::AESKEYGENASSIST128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 149 /* aeskeygenassist */, X86::AESKEYGENASSIST128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 165 /* and */, X86::AND8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 165 /* and */, X86::AND8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 165 /* and */, X86::AND8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 165 /* and */, X86::AND8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 165 /* and */, X86::AND16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 165 /* and */, X86::AND16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 165 /* and */, X86::AND16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 165 /* and */, X86::AND16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 165 /* and */, X86::AND16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 165 /* and */, X86::AND32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 165 /* and */, X86::AND32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 165 /* and */, X86::AND32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 165 /* and */, X86::AND32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 165 /* and */, X86::AND32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 165 /* and */, X86::AND64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 165 /* and */, X86::AND64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 165 /* and */, X86::AND64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 165 /* and */, X86::AND64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 165 /* and */, X86::AND64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 165 /* and */, X86::AND16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 165 /* and */, X86::AND16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 165 /* and */, X86::AND16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 165 /* and */, X86::AND32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 165 /* and */, X86::AND32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 165 /* and */, X86::AND32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 165 /* and */, X86::AND64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 165 /* and */, X86::AND64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 165 /* and */, X86::AND64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 165 /* and */, X86::AND8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 165 /* and */, X86::AND8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 169 /* andb */, X86::AND8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 169 /* andb */, X86::AND8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 169 /* andb */, X86::AND8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 169 /* andb */, X86::AND8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 169 /* andb */, X86::AND8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 169 /* andb */, X86::AND8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 174 /* andl */, X86::AND32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 174 /* andl */, X86::AND32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 174 /* andl */, X86::AND32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 174 /* andl */, X86::AND32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 174 /* andl */, X86::AND32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 174 /* andl */, X86::AND32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 174 /* andl */, X86::AND32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 174 /* andl */, X86::AND32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 179 /* andn */, X86::ANDN32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 179 /* andn */, X86::ANDN32rm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_GR32, MCK_GR32, MCK_Mem32 }, 1},
  { 179 /* andn */, X86::ANDN64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 179 /* andn */, X86::ANDN64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_GR64, MCK_GR64, MCK_Mem64 }, 1},
  { 184 /* andnl */, X86::ANDN32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 184 /* andnl */, X86::ANDN32rm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32, MCK_GR32 }, 0},
  { 190 /* andnpd */, X86::ANDNPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 190 /* andnpd */, X86::FsANDNPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 190 /* andnpd */, X86::ANDNPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 190 /* andnpd */, X86::FsANDNPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 190 /* andnpd */, X86::ANDNPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 190 /* andnpd */, X86::FsANDNPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 190 /* andnpd */, X86::ANDNPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 190 /* andnpd */, X86::FsANDNPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 197 /* andnps */, X86::ANDNPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 197 /* andnps */, X86::FsANDNPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 197 /* andnps */, X86::ANDNPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 197 /* andnps */, X86::FsANDNPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 197 /* andnps */, X86::ANDNPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 197 /* andnps */, X86::FsANDNPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 197 /* andnps */, X86::ANDNPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 197 /* andnps */, X86::FsANDNPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 204 /* andnq */, X86::ANDN64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 204 /* andnq */, X86::ANDN64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64, MCK_GR64 }, 0},
  { 210 /* andpd */, X86::ANDPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 210 /* andpd */, X86::FsANDPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 210 /* andpd */, X86::ANDPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 210 /* andpd */, X86::FsANDPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 210 /* andpd */, X86::ANDPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 210 /* andpd */, X86::FsANDPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 210 /* andpd */, X86::ANDPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 210 /* andpd */, X86::FsANDPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 216 /* andps */, X86::ANDPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 216 /* andps */, X86::FsANDPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 216 /* andps */, X86::ANDPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 216 /* andps */, X86::FsANDPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 216 /* andps */, X86::ANDPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 216 /* andps */, X86::FsANDPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 216 /* andps */, X86::ANDPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 216 /* andps */, X86::FsANDPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 222 /* andq */, X86::AND64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 222 /* andq */, X86::AND64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 222 /* andq */, X86::AND64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 222 /* andq */, X86::AND64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 222 /* andq */, X86::AND64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 222 /* andq */, X86::AND64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 222 /* andq */, X86::AND64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 222 /* andq */, X86::AND64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 227 /* andw */, X86::AND16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 227 /* andw */, X86::AND16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 227 /* andw */, X86::AND16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 227 /* andw */, X86::AND16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 227 /* andw */, X86::AND16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 227 /* andw */, X86::AND16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 227 /* andw */, X86::AND16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 227 /* andw */, X86::AND16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 232 /* arpl */, X86::ARPL16rr, Convert__Reg1_0__Reg1_1, Feature_In32BitMode, { MCK_GR16, MCK_GR16 }, 0},
  { 232 /* arpl */, X86::ARPL16rr, Convert__Reg1_1__Reg1_0, Feature_In32BitMode, { MCK_GR16, MCK_GR16 }, 1},
  { 232 /* arpl */, X86::ARPL16mr, Convert__Reg1_0__Mem165_1, Feature_In32BitMode, { MCK_GR16, MCK_Mem16 }, 0},
  { 232 /* arpl */, X86::ARPL16mr, Convert__Reg1_1__Mem165_0, Feature_In32BitMode, { MCK_Mem16, MCK_GR16 }, 1},
  { 237 /* bextr */, X86::BEXTR32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 237 /* bextr */, X86::BEXTR32rm, Convert__Reg1_0__Mem325_1__Reg1_2, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 1},
  { 237 /* bextr */, X86::BEXTR64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 237 /* bextr */, X86::BEXTR64rm, Convert__Reg1_0__Mem645_1__Reg1_2, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 1},
  { 243 /* bextrl */, X86::BEXTR32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 243 /* bextrl */, X86::BEXTR32rm, Convert__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 0},
  { 250 /* bextrq */, X86::BEXTR64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 250 /* bextrq */, X86::BEXTR64rm, Convert__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 0},
  { 257 /* blendpd */, X86::BLENDPDrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 257 /* blendpd */, X86::BLENDPDrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 257 /* blendpd */, X86::BLENDPDrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 257 /* blendpd */, X86::BLENDPDrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 265 /* blendps */, X86::BLENDPSrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 265 /* blendps */, X86::BLENDPSrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 265 /* blendps */, X86::BLENDPSrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 265 /* blendps */, X86::BLENDPSrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 273 /* blendvpd */, X86::BLENDVPDrr0, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 273 /* blendvpd */, X86::BLENDVPDrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 273 /* blendvpd */, X86::BLENDVPDrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 273 /* blendvpd */, X86::BLENDVPDrm0, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 273 /* blendvpd */, X86::BLENDVPDrr0, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_XMM0, MCK_FR32, MCK_FR32 }, 0},
  { 273 /* blendvpd */, X86::BLENDVPDrm0, Convert__Reg1_2__Tie0__Mem1285_1, 0, { MCK_XMM0, MCK_Mem128, MCK_FR32 }, 0},
  { 273 /* blendvpd */, X86::BLENDVPDrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32, MCK__PCT_xmm0 }, 1},
  { 273 /* blendvpd */, X86::BLENDVPDrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128, MCK__PCT_xmm0 }, 1},
  { 282 /* blendvps */, X86::BLENDVPSrr0, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 282 /* blendvps */, X86::BLENDVPSrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 282 /* blendvps */, X86::BLENDVPSrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 282 /* blendvps */, X86::BLENDVPSrm0, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 282 /* blendvps */, X86::BLENDVPSrr0, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_XMM0, MCK_FR32, MCK_FR32 }, 0},
  { 282 /* blendvps */, X86::BLENDVPSrm0, Convert__Reg1_2__Tie0__Mem1285_1, 0, { MCK_XMM0, MCK_Mem128, MCK_FR32 }, 0},
  { 282 /* blendvps */, X86::BLENDVPSrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32, MCK__PCT_xmm0 }, 1},
  { 282 /* blendvps */, X86::BLENDVPSrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128, MCK__PCT_xmm0 }, 1},
  { 291 /* blsi */, X86::BLSI32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 291 /* blsi */, X86::BLSI32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 291 /* blsi */, X86::BLSI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 291 /* blsi */, X86::BLSI64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 296 /* blsil */, X86::BLSI32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 296 /* blsil */, X86::BLSI32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 302 /* blsiq */, X86::BLSI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 302 /* blsiq */, X86::BLSI64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 308 /* blsmsk */, X86::BLSMSK32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 308 /* blsmsk */, X86::BLSMSK32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 308 /* blsmsk */, X86::BLSMSK64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 308 /* blsmsk */, X86::BLSMSK64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 315 /* blsmskl */, X86::BLSMSK32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 315 /* blsmskl */, X86::BLSMSK32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 323 /* blsmskq */, X86::BLSMSK64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 323 /* blsmskq */, X86::BLSMSK64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 331 /* blsr */, X86::BLSR32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 331 /* blsr */, X86::BLSR32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 331 /* blsr */, X86::BLSR64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 331 /* blsr */, X86::BLSR64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 336 /* blsrl */, X86::BLSR32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 336 /* blsrl */, X86::BLSR32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 342 /* blsrq */, X86::BLSR64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 342 /* blsrq */, X86::BLSR64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 348 /* bound */, X86::BOUNDS16rm, Convert__Reg1_0__Mem165_1, Feature_In32BitMode, { MCK_GR16, MCK_Mem16 }, 1},
  { 348 /* bound */, X86::BOUNDS32rm, Convert__Reg1_0__Mem325_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem32 }, 1},
  { 348 /* bound */, X86::BOUNDS16rm, Convert__Reg1_1__Mem165_0, Feature_In32BitMode, { MCK_Mem16, MCK_GR16 }, 0},
  { 348 /* bound */, X86::BOUNDS32rm, Convert__Reg1_1__Mem325_0, Feature_In32BitMode, { MCK_Mem32, MCK_GR32 }, 0},
  { 354 /* bsf */, X86::BSF16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 354 /* bsf */, X86::BSF16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 354 /* bsf */, X86::BSF32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 354 /* bsf */, X86::BSF32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 354 /* bsf */, X86::BSF64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 354 /* bsf */, X86::BSF64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 358 /* bsfl */, X86::BSF32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 358 /* bsfl */, X86::BSF32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 363 /* bsfq */, X86::BSF64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 363 /* bsfq */, X86::BSF64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 368 /* bsfw */, X86::BSF16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 368 /* bsfw */, X86::BSF16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 373 /* bsr */, X86::BSR16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 373 /* bsr */, X86::BSR16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 373 /* bsr */, X86::BSR32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 373 /* bsr */, X86::BSR32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 373 /* bsr */, X86::BSR64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 373 /* bsr */, X86::BSR64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 377 /* bsrl */, X86::BSR32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 377 /* bsrl */, X86::BSR32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 382 /* bsrq */, X86::BSR64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 382 /* bsrq */, X86::BSR64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 387 /* bsrw */, X86::BSR16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 387 /* bsrw */, X86::BSR16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 392 /* bswap */, X86::BSWAP32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 392 /* bswap */, X86::BSWAP64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 398 /* bswapl */, X86::BSWAP32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 405 /* bswapq */, X86::BSWAP64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 412 /* bt */, X86::BT16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 412 /* bt */, X86::BT16ri8, Convert__Reg1_0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 412 /* bt */, X86::BT32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 412 /* bt */, X86::BT32ri8, Convert__Reg1_0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 412 /* bt */, X86::BT64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 412 /* bt */, X86::BT64ri8, Convert__Reg1_0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 412 /* bt */, X86::BT32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 412 /* bt */, X86::BT32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 1},
  { 412 /* bt */, X86::BT16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 412 /* bt */, X86::BT16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 412 /* bt */, X86::BT32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 412 /* bt */, X86::BT32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 412 /* bt */, X86::BT64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 412 /* bt */, X86::BT64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 415 /* btc */, X86::BTC16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 415 /* btc */, X86::BTC16ri8, Convert__Reg1_0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 415 /* btc */, X86::BTC32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 415 /* btc */, X86::BTC32ri8, Convert__Reg1_0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 415 /* btc */, X86::BTC64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 415 /* btc */, X86::BTC64ri8, Convert__Reg1_0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 415 /* btc */, X86::BTC16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 415 /* btc */, X86::BTC16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 415 /* btc */, X86::BTC32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 415 /* btc */, X86::BTC32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 415 /* btc */, X86::BTC64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 415 /* btc */, X86::BTC64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 419 /* btcl */, X86::BTC32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 419 /* btcl */, X86::BTC32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 419 /* btcl */, X86::BTC32ri8, Convert__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 419 /* btcl */, X86::BTC32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 424 /* btcq */, X86::BTC64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 424 /* btcq */, X86::BTC64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 424 /* btcq */, X86::BTC64ri8, Convert__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 424 /* btcq */, X86::BTC64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 429 /* btcw */, X86::BTC16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 429 /* btcw */, X86::BTC16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 429 /* btcw */, X86::BTC16ri8, Convert__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 429 /* btcw */, X86::BTC16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 434 /* btl */, X86::BT32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 434 /* btl */, X86::BT32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 434 /* btl */, X86::BT32ri8, Convert__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 434 /* btl */, X86::BT32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 438 /* btq */, X86::BT64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 438 /* btq */, X86::BT64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 438 /* btq */, X86::BT64ri8, Convert__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 438 /* btq */, X86::BT64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 442 /* btr */, X86::BTR16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 442 /* btr */, X86::BTR16ri8, Convert__Reg1_0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 442 /* btr */, X86::BTR32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 442 /* btr */, X86::BTR32ri8, Convert__Reg1_0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 442 /* btr */, X86::BTR64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 442 /* btr */, X86::BTR64ri8, Convert__Reg1_0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 442 /* btr */, X86::BTR16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 442 /* btr */, X86::BTR16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 442 /* btr */, X86::BTR32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 442 /* btr */, X86::BTR32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 442 /* btr */, X86::BTR64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 442 /* btr */, X86::BTR64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 446 /* btrl */, X86::BTR32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 446 /* btrl */, X86::BTR32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 446 /* btrl */, X86::BTR32ri8, Convert__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 446 /* btrl */, X86::BTR32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 451 /* btrq */, X86::BTR64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 451 /* btrq */, X86::BTR64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 451 /* btrq */, X86::BTR64ri8, Convert__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 451 /* btrq */, X86::BTR64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 456 /* btrw */, X86::BTR16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 456 /* btrw */, X86::BTR16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 456 /* btrw */, X86::BTR16ri8, Convert__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 456 /* btrw */, X86::BTR16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 461 /* bts */, X86::BTS16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 461 /* bts */, X86::BTS16ri8, Convert__Reg1_0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 461 /* bts */, X86::BTS32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 461 /* bts */, X86::BTS32ri8, Convert__Reg1_0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 461 /* bts */, X86::BTS64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 461 /* bts */, X86::BTS64ri8, Convert__Reg1_0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 461 /* bts */, X86::BTS16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 461 /* bts */, X86::BTS16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 461 /* bts */, X86::BTS32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 461 /* bts */, X86::BTS32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 461 /* bts */, X86::BTS64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 461 /* bts */, X86::BTS64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 465 /* btsl */, X86::BTS32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 465 /* btsl */, X86::BTS32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 465 /* btsl */, X86::BTS32ri8, Convert__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 465 /* btsl */, X86::BTS32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 470 /* btsq */, X86::BTS64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 470 /* btsq */, X86::BTS64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 470 /* btsq */, X86::BTS64ri8, Convert__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 470 /* btsq */, X86::BTS64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 475 /* btsw */, X86::BTS16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 475 /* btsw */, X86::BTS16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 475 /* btsw */, X86::BTS16ri8, Convert__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 475 /* btsw */, X86::BTS16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 480 /* btw */, X86::BT16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 480 /* btw */, X86::BT16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 480 /* btw */, X86::BT16ri8, Convert__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 480 /* btw */, X86::BT16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 484 /* bzhi */, X86::BZHI32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 484 /* bzhi */, X86::BZHI32rm, Convert__Reg1_0__Mem325_1__Reg1_2, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 1},
  { 484 /* bzhi */, X86::BZHI64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 484 /* bzhi */, X86::BZHI64rm, Convert__Reg1_0__Mem645_1__Reg1_2, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 1},
  { 489 /* bzhil */, X86::BZHI32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 489 /* bzhil */, X86::BZHI32rm, Convert__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 0},
  { 495 /* bzhiq */, X86::BZHI64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 495 /* bzhiq */, X86::BZHI64rm, Convert__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 0},
  { 501 /* call */, X86::CALL32r, Convert__Reg1_0, Feature_In32BitMode, { MCK_GR32 }, 1},
  { 501 /* call */, X86::CALL64r, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 501 /* call */, X86::CALL64pcrel32, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 1},
  { 501 /* call */, X86::CALLpcrel32, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 1},
  { 501 /* call */, X86::CALL32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 1},
  { 501 /* call */, X86::CALL64m, Convert__Mem645_0, Feature_In64BitMode, { MCK_Mem64 }, 1},
  { 501 /* call */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 501 /* call */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 506 /* calll */, X86::CALLpcrel32, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 0},
  { 506 /* calll */, X86::CALL32r, Convert__Reg1_1, Feature_In32BitMode, { MCK__STAR_, MCK_GR32 }, 0},
  { 506 /* calll */, X86::CALL32m, Convert__Mem325_1, Feature_In32BitMode, { MCK__STAR_, MCK_Mem32 }, 0},
  { 506 /* calll */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 506 /* calll */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 512 /* callq */, X86::CALL64pcrel32, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 0},
  { 512 /* callq */, X86::CALL64r, Convert__Reg1_1, Feature_In64BitMode, { MCK__STAR_, MCK_GR64 }, 0},
  { 512 /* callq */, X86::CALL64m, Convert__Mem645_1, Feature_In64BitMode, { MCK__STAR_, MCK_Mem64 }, 0},
  { 518 /* callw */, X86::CALLpcrel16, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 518 /* callw */, X86::CALLpcrel16, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 518 /* callw */, X86::FARCALL16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 518 /* callw */, X86::FARCALL16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 524 /* cbtw */, X86::CBW, Convert_NoOperands, 0, {  }, 0},
  { 529 /* cbw */, X86::CBW, Convert_NoOperands, 0, {  }, 1},
  { 533 /* cdq */, X86::CDQ, Convert_NoOperands, 0, {  }, 1},
  { 537 /* cdqe */, X86::CDQE, Convert_NoOperands, 0, {  }, 1},
  { 542 /* clc */, X86::CLC, Convert_NoOperands, 0, {  }, 0},
  { 542 /* clc */, X86::CLC, Convert_NoOperands, 0, {  }, 1},
  { 546 /* cld */, X86::CLD, Convert_NoOperands, 0, {  }, 0},
  { 546 /* cld */, X86::CLD, Convert_NoOperands, 0, {  }, 1},
  { 550 /* clflush */, X86::CLFLUSH, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 550 /* clflush */, X86::CLFLUSH, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 558 /* clgi */, X86::CLGI, Convert_NoOperands, 0, {  }, 0},
  { 558 /* clgi */, X86::CLGI, Convert_NoOperands, 0, {  }, 1},
  { 563 /* cli */, X86::CLI, Convert_NoOperands, 0, {  }, 0},
  { 563 /* cli */, X86::CLI, Convert_NoOperands, 0, {  }, 1},
  { 567 /* clrb */, X86::XOR8rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR8 }, 0},
  { 567 /* clrb */, X86::XOR8rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR8 }, 1},
  { 572 /* clrl */, X86::XOR32rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR32 }, 0},
  { 572 /* clrl */, X86::XOR32rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR32 }, 1},
  { 577 /* clrq */, X86::XOR64rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR64 }, 0},
  { 577 /* clrq */, X86::XOR64rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR64 }, 1},
  { 582 /* clrw */, X86::XOR16rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR16 }, 0},
  { 582 /* clrw */, X86::XOR16rr, Convert__Reg1_0__Tie0__Reg1_0, 0, { MCK_GR16 }, 1},
  { 587 /* cltd */, X86::CDQ, Convert_NoOperands, 0, {  }, 0},
  { 592 /* cltq */, X86::CDQE, Convert_NoOperands, 0, {  }, 0},
  { 597 /* clts */, X86::CLTS, Convert_NoOperands, 0, {  }, 0},
  { 597 /* clts */, X86::CLTS, Convert_NoOperands, 0, {  }, 1},
  { 602 /* cmc */, X86::CMC, Convert_NoOperands, 0, {  }, 0},
  { 602 /* cmc */, X86::CMC, Convert_NoOperands, 0, {  }, 1},
  { 606 /* cmova */, X86::CMOVA16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 606 /* cmova */, X86::CMOVA16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 606 /* cmova */, X86::CMOVA32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 606 /* cmova */, X86::CMOVA32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 606 /* cmova */, X86::CMOVA64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 606 /* cmova */, X86::CMOVA64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 612 /* cmovae */, X86::CMOVAE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 612 /* cmovae */, X86::CMOVAE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 612 /* cmovae */, X86::CMOVAE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 612 /* cmovae */, X86::CMOVAE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 612 /* cmovae */, X86::CMOVAE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 612 /* cmovae */, X86::CMOVAE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 619 /* cmovael */, X86::CMOVAE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 619 /* cmovael */, X86::CMOVAE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 627 /* cmovaeq */, X86::CMOVAE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 627 /* cmovaeq */, X86::CMOVAE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 635 /* cmovaew */, X86::CMOVAE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 635 /* cmovaew */, X86::CMOVAE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 643 /* cmoval */, X86::CMOVA32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 643 /* cmoval */, X86::CMOVA32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 650 /* cmovaq */, X86::CMOVA64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 650 /* cmovaq */, X86::CMOVA64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 657 /* cmovaw */, X86::CMOVA16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 657 /* cmovaw */, X86::CMOVA16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 664 /* cmovb */, X86::CMOVB16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 664 /* cmovb */, X86::CMOVB16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 664 /* cmovb */, X86::CMOVB32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 664 /* cmovb */, X86::CMOVB32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 664 /* cmovb */, X86::CMOVB64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 664 /* cmovb */, X86::CMOVB64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 670 /* cmovbe */, X86::CMOVBE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 677 /* cmovbel */, X86::CMOVBE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 677 /* cmovbel */, X86::CMOVBE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 685 /* cmovbeq */, X86::CMOVBE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 685 /* cmovbeq */, X86::CMOVBE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 693 /* cmovbew */, X86::CMOVBE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 693 /* cmovbew */, X86::CMOVBE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 701 /* cmovbl */, X86::CMOVB32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 701 /* cmovbl */, X86::CMOVB32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 708 /* cmovbq */, X86::CMOVB64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 708 /* cmovbq */, X86::CMOVB64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 715 /* cmovbw */, X86::CMOVB16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 715 /* cmovbw */, X86::CMOVB16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 722 /* cmove */, X86::CMOVE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 722 /* cmove */, X86::CMOVE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 722 /* cmove */, X86::CMOVE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 722 /* cmove */, X86::CMOVE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 722 /* cmove */, X86::CMOVE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 722 /* cmove */, X86::CMOVE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 728 /* cmovel */, X86::CMOVE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 728 /* cmovel */, X86::CMOVE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 735 /* cmoveq */, X86::CMOVE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 735 /* cmoveq */, X86::CMOVE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 742 /* cmovew */, X86::CMOVE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 742 /* cmovew */, X86::CMOVE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 749 /* cmovg */, X86::CMOVG16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 749 /* cmovg */, X86::CMOVG16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 749 /* cmovg */, X86::CMOVG32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 749 /* cmovg */, X86::CMOVG32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 749 /* cmovg */, X86::CMOVG64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 749 /* cmovg */, X86::CMOVG64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 755 /* cmovge */, X86::CMOVGE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 755 /* cmovge */, X86::CMOVGE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 755 /* cmovge */, X86::CMOVGE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 755 /* cmovge */, X86::CMOVGE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 755 /* cmovge */, X86::CMOVGE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 755 /* cmovge */, X86::CMOVGE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 762 /* cmovgel */, X86::CMOVGE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 762 /* cmovgel */, X86::CMOVGE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 770 /* cmovgeq */, X86::CMOVGE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 770 /* cmovgeq */, X86::CMOVGE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 778 /* cmovgew */, X86::CMOVGE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 778 /* cmovgew */, X86::CMOVGE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 786 /* cmovgl */, X86::CMOVG32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 786 /* cmovgl */, X86::CMOVG32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 793 /* cmovgq */, X86::CMOVG64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 793 /* cmovgq */, X86::CMOVG64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 800 /* cmovgw */, X86::CMOVG16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 800 /* cmovgw */, X86::CMOVG16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 807 /* cmovl */, X86::CMOVL16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 807 /* cmovl */, X86::CMOVL16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 807 /* cmovl */, X86::CMOVL32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 807 /* cmovl */, X86::CMOVL32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 807 /* cmovl */, X86::CMOVL64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 807 /* cmovl */, X86::CMOVL64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 813 /* cmovle */, X86::CMOVLE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 813 /* cmovle */, X86::CMOVLE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 813 /* cmovle */, X86::CMOVLE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 813 /* cmovle */, X86::CMOVLE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 813 /* cmovle */, X86::CMOVLE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 813 /* cmovle */, X86::CMOVLE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 820 /* cmovlel */, X86::CMOVLE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 820 /* cmovlel */, X86::CMOVLE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 828 /* cmovleq */, X86::CMOVLE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 828 /* cmovleq */, X86::CMOVLE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 836 /* cmovlew */, X86::CMOVLE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 836 /* cmovlew */, X86::CMOVLE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 844 /* cmovll */, X86::CMOVL32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 844 /* cmovll */, X86::CMOVL32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 851 /* cmovlq */, X86::CMOVL64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 851 /* cmovlq */, X86::CMOVL64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 858 /* cmovlw */, X86::CMOVL16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 858 /* cmovlw */, X86::CMOVL16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 865 /* cmovne */, X86::CMOVNE16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 865 /* cmovne */, X86::CMOVNE16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 865 /* cmovne */, X86::CMOVNE32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 865 /* cmovne */, X86::CMOVNE32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 865 /* cmovne */, X86::CMOVNE64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 865 /* cmovne */, X86::CMOVNE64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 872 /* cmovnel */, X86::CMOVNE32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 872 /* cmovnel */, X86::CMOVNE32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 880 /* cmovneq */, X86::CMOVNE64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 880 /* cmovneq */, X86::CMOVNE64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 888 /* cmovnew */, X86::CMOVNE16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 888 /* cmovnew */, X86::CMOVNE16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 896 /* cmovno */, X86::CMOVNO16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 896 /* cmovno */, X86::CMOVNO16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 896 /* cmovno */, X86::CMOVNO32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 896 /* cmovno */, X86::CMOVNO32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 896 /* cmovno */, X86::CMOVNO64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 896 /* cmovno */, X86::CMOVNO64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 903 /* cmovnol */, X86::CMOVNO32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 903 /* cmovnol */, X86::CMOVNO32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 911 /* cmovnoq */, X86::CMOVNO64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 911 /* cmovnoq */, X86::CMOVNO64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 919 /* cmovnow */, X86::CMOVNO16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 919 /* cmovnow */, X86::CMOVNO16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 927 /* cmovnp */, X86::CMOVNP16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 927 /* cmovnp */, X86::CMOVNP16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 927 /* cmovnp */, X86::CMOVNP32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 927 /* cmovnp */, X86::CMOVNP32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 927 /* cmovnp */, X86::CMOVNP64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 927 /* cmovnp */, X86::CMOVNP64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 934 /* cmovnpl */, X86::CMOVNP32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 934 /* cmovnpl */, X86::CMOVNP32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 942 /* cmovnpq */, X86::CMOVNP64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 942 /* cmovnpq */, X86::CMOVNP64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 950 /* cmovnpw */, X86::CMOVNP16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 950 /* cmovnpw */, X86::CMOVNP16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 958 /* cmovns */, X86::CMOVNS16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 958 /* cmovns */, X86::CMOVNS16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 958 /* cmovns */, X86::CMOVNS32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 958 /* cmovns */, X86::CMOVNS32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 958 /* cmovns */, X86::CMOVNS64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 958 /* cmovns */, X86::CMOVNS64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 965 /* cmovnsl */, X86::CMOVNS32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 965 /* cmovnsl */, X86::CMOVNS32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 973 /* cmovnsq */, X86::CMOVNS64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 973 /* cmovnsq */, X86::CMOVNS64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 981 /* cmovnsw */, X86::CMOVNS16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 981 /* cmovnsw */, X86::CMOVNS16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 989 /* cmovo */, X86::CMOVO16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 989 /* cmovo */, X86::CMOVO16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 989 /* cmovo */, X86::CMOVO32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 989 /* cmovo */, X86::CMOVO32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 989 /* cmovo */, X86::CMOVO64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 989 /* cmovo */, X86::CMOVO64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 995 /* cmovol */, X86::CMOVO32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 995 /* cmovol */, X86::CMOVO32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1002 /* cmovoq */, X86::CMOVO64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1002 /* cmovoq */, X86::CMOVO64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1009 /* cmovow */, X86::CMOVO16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 1009 /* cmovow */, X86::CMOVO16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 1016 /* cmovp */, X86::CMOVP16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 1016 /* cmovp */, X86::CMOVP16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 1016 /* cmovp */, X86::CMOVP32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 1016 /* cmovp */, X86::CMOVP32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 1016 /* cmovp */, X86::CMOVP64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 1016 /* cmovp */, X86::CMOVP64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 1022 /* cmovpl */, X86::CMOVP32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 1022 /* cmovpl */, X86::CMOVP32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1029 /* cmovpq */, X86::CMOVP64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1029 /* cmovpq */, X86::CMOVP64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1036 /* cmovpw */, X86::CMOVP16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 1036 /* cmovpw */, X86::CMOVP16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 1043 /* cmovs */, X86::CMOVS16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 1043 /* cmovs */, X86::CMOVS16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 1043 /* cmovs */, X86::CMOVS32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 1043 /* cmovs */, X86::CMOVS32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 1043 /* cmovs */, X86::CMOVS64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 1043 /* cmovs */, X86::CMOVS64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 1049 /* cmovsl */, X86::CMOVS32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 1049 /* cmovsl */, X86::CMOVS32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1056 /* cmovsq */, X86::CMOVS64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1056 /* cmovsq */, X86::CMOVS64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1063 /* cmovsw */, X86::CMOVS16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 1063 /* cmovsw */, X86::CMOVS16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 1070 /* cmp */, X86::CMP8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP8rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 1070 /* cmp */, X86::CMP8ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP8rm, Convert__Reg1_0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 1070 /* cmp */, X86::CMP16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 1070 /* cmp */, X86::CMP16ri8, Convert__Reg1_0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 1070 /* cmp */, X86::CMP16ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 1070 /* cmp */, X86::CMP32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 1070 /* cmp */, X86::CMP32ri8, Convert__Reg1_0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 1070 /* cmp */, X86::CMP32ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 1070 /* cmp */, X86::CMP64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 1070 /* cmp */, X86::CMP64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 1070 /* cmp */, X86::CMP64ri8, Convert__Reg1_0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 1070 /* cmp */, X86::CMP64ri32, Convert__Reg1_0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 1070 /* cmp */, X86::CMP64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 1070 /* cmp */, X86::CMP16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 1070 /* cmp */, X86::CMP16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 1070 /* cmp */, X86::CMP16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 1070 /* cmp */, X86::CMP32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 1070 /* cmp */, X86::CMP32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMP64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 1070 /* cmp */, X86::CMP64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 1070 /* cmp */, X86::CMP64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 1070 /* cmp */, X86::CMP8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 1070 /* cmp */, X86::CMP8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 1070 /* cmp */, X86::CMPPDrri, Convert__Reg1_3__Tie0__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPPDrri, Convert__Reg1_2__Tie0__Reg1_3__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_FR32 }, 1},
  { 1070 /* cmp */, X86::CMPPDrmi, Convert__Reg1_2__Tie0__Mem1285_3__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_Mem128 }, 1},
  { 1070 /* cmp */, X86::CMPPDrmi, Convert__Reg1_3__Tie0__Mem1285_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_Mem128, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPPSrri, Convert__Reg1_3__Tie0__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPPSrri, Convert__Reg1_2__Tie0__Reg1_3__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_FR32 }, 1},
  { 1070 /* cmp */, X86::CMPPSrmi, Convert__Reg1_2__Tie0__Mem1285_3__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_Mem128 }, 1},
  { 1070 /* cmp */, X86::CMPPSrmi, Convert__Reg1_3__Tie0__Mem1285_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_Mem128, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPSDrr, Convert__Reg1_3__Tie0__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPSDrr, Convert__Reg1_2__Tie0__Reg1_3__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_FR32 }, 1},
  { 1070 /* cmp */, X86::CMPSDrm, Convert__Reg1_2__Tie0__Mem645_3__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_Mem64 }, 1},
  { 1070 /* cmp */, X86::CMPSDrm, Convert__Reg1_3__Tie0__Mem645_2__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_Mem64, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPSSrr, Convert__Reg1_3__Tie0__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_ss, MCK_FR32, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPSSrm, Convert__Reg1_3__Tie0__Mem325_2__Imm1_0, 0, { MCK_Imm, MCK_ss, MCK_Mem32, MCK_FR32 }, 0},
  { 1070 /* cmp */, X86::CMPSSrr, Convert__Reg1_2__Tie0__Reg1_3__Imm1_0, 0, { MCK_Imm, MCK_SS, MCK_FR32, MCK_FR32 }, 1},
  { 1070 /* cmp */, X86::CMPSSrm, Convert__Reg1_2__Tie0__Mem325_3__Imm1_0, 0, { MCK_Imm, MCK_SS, MCK_FR32, MCK_Mem32 }, 1},
  { 1074 /* cmpb */, X86::CMP8rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 1074 /* cmpb */, X86::CMP8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 1074 /* cmpb */, X86::CMP8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 1074 /* cmpb */, X86::CMP8ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 1074 /* cmpb */, X86::CMP8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 1074 /* cmpb */, X86::CMP8rm, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 1079 /* cmpl */, X86::CMP32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 1079 /* cmpl */, X86::CMP32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 1079 /* cmpl */, X86::CMP32ri8, Convert__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 1079 /* cmpl */, X86::CMP32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 1079 /* cmpl */, X86::CMP32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 1079 /* cmpl */, X86::CMP32ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 1079 /* cmpl */, X86::CMP32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 1079 /* cmpl */, X86::CMP32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1084 /* cmppd */, X86::CMPPDrri_alt, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 1084 /* cmppd */, X86::CMPPDrmi_alt, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 1084 /* cmppd */, X86::CMPPDrri_alt, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 1084 /* cmppd */, X86::CMPPDrmi_alt, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 1090 /* cmpps */, X86::CMPPSrri_alt, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 1090 /* cmpps */, X86::CMPPSrmi_alt, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 1090 /* cmpps */, X86::CMPPSrri_alt, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 1090 /* cmpps */, X86::CMPPSrmi_alt, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 1096 /* cmpq */, X86::CMP64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1096 /* cmpq */, X86::CMP64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 1096 /* cmpq */, X86::CMP64ri8, Convert__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 1096 /* cmpq */, X86::CMP64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 1096 /* cmpq */, X86::CMP64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 1096 /* cmpq */, X86::CMP64ri32, Convert__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 1096 /* cmpq */, X86::CMP64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 1096 /* cmpq */, X86::CMP64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1101 /* cmpsb */, X86::CMPS8, Convert_NoOperands, 0, {  }, 0},
  { 1101 /* cmpsb */, X86::CMPS8, Convert_NoOperands, 0, {  }, 1},
  { 1107 /* cmpsd */, X86::CMPS32, Convert_NoOperands, 0, {  }, 1},
  { 1107 /* cmpsd */, X86::CMPSDrr_alt, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 1107 /* cmpsd */, X86::CMPSDrm_alt, Convert__Reg1_0__Tie0__Mem645_1__Imm1_2, 0, { MCK_FR32, MCK_Mem64, MCK_Imm }, 1},
  { 1107 /* cmpsd */, X86::CMPSDrr_alt, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 1107 /* cmpsd */, X86::CMPSDrm_alt, Convert__Reg1_2__Tie0__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64, MCK_FR32 }, 0},
  { 1113 /* cmpsl */, X86::CMPS32, Convert_NoOperands, 0, {  }, 0},
  { 1119 /* cmpsq */, X86::CMPS64, Convert_NoOperands, 0, {  }, 0},
  { 1119 /* cmpsq */, X86::CMPS64, Convert_NoOperands, 0, {  }, 1},
  { 1125 /* cmpss */, X86::CMPSSrr_alt, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 1125 /* cmpss */, X86::CMPSSrm_alt, Convert__Reg1_0__Tie0__Mem325_1__Imm1_2, 0, { MCK_FR32, MCK_Mem32, MCK_Imm }, 1},
  { 1125 /* cmpss */, X86::CMPSSrr_alt, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 1125 /* cmpss */, X86::CMPSSrm_alt, Convert__Reg1_2__Tie0__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32, MCK_FR32 }, 0},
  { 1131 /* cmpsw */, X86::CMPS16, Convert_NoOperands, 0, {  }, 0},
  { 1131 /* cmpsw */, X86::CMPS16, Convert_NoOperands, 0, {  }, 1},
  { 1137 /* cmpw */, X86::CMP16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 1137 /* cmpw */, X86::CMP16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 1137 /* cmpw */, X86::CMP16ri8, Convert__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 1137 /* cmpw */, X86::CMP16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 1137 /* cmpw */, X86::CMP16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 1137 /* cmpw */, X86::CMP16ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 1137 /* cmpw */, X86::CMP16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 1137 /* cmpw */, X86::CMP16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 1142 /* cmpxchg */, X86::CMPXCHG8rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG16rm, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG32rm, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG64rm, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 1142 /* cmpxchg */, X86::CMPXCHG8rm, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 1150 /* cmpxchg16b */, X86::CMPXCHG16B, Convert__Mem1285_0, 0, { MCK_Mem128 }, 0},
  { 1150 /* cmpxchg16b */, X86::CMPXCHG16B, Convert__Mem1285_0, 0, { MCK_Mem128 }, 1},
  { 1161 /* cmpxchg8b */, X86::CMPXCHG8B, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1161 /* cmpxchg8b */, X86::CMPXCHG8B, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1171 /* cmpxchgb */, X86::CMPXCHG8rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 1171 /* cmpxchgb */, X86::CMPXCHG8rm, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 1180 /* cmpxchgl */, X86::CMPXCHG32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 1180 /* cmpxchgl */, X86::CMPXCHG32rm, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 1189 /* cmpxchgq */, X86::CMPXCHG64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1189 /* cmpxchgq */, X86::CMPXCHG64rm, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 1198 /* cmpxchgw */, X86::CMPXCHG16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 1198 /* cmpxchgw */, X86::CMPXCHG16rm, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 1207 /* comisd */, X86::COMISDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1207 /* comisd */, X86::COMISDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1207 /* comisd */, X86::COMISDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1207 /* comisd */, X86::COMISDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1214 /* comiss */, X86::COMISSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1214 /* comiss */, X86::COMISSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1214 /* comiss */, X86::COMISSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1214 /* comiss */, X86::COMISSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1221 /* cpuid */, X86::CPUID, Convert_NoOperands, 0, {  }, 0},
  { 1221 /* cpuid */, X86::CPUID, Convert_NoOperands, 0, {  }, 1},
  { 1227 /* cqo */, X86::CQO, Convert_NoOperands, 0, {  }, 1},
  { 1231 /* cqto */, X86::CQO, Convert_NoOperands, 0, {  }, 0},
  { 1236 /* crc32 */, X86::CRC32r32r8, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR8 }, 1},
  { 1236 /* crc32 */, X86::CRC32r32r16, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR16 }, 1},
  { 1236 /* crc32 */, X86::CRC32r32r32, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 1236 /* crc32 */, X86::CRC32r32m16, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR32, MCK_Mem16 }, 1},
  { 1236 /* crc32 */, X86::CRC32r32m32, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 1236 /* crc32 */, X86::CRC32r32m8, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR32, MCK_Mem8 }, 1},
  { 1236 /* crc32 */, X86::CRC32r64r8, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR8 }, 1},
  { 1236 /* crc32 */, X86::CRC32r64r64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 1236 /* crc32 */, X86::CRC32r64m64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 1236 /* crc32 */, X86::CRC32r64m8, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR64, MCK_Mem8 }, 1},
  { 1242 /* crc32b */, X86::CRC32r32r8, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 0},
  { 1242 /* crc32b */, X86::CRC32r64r8, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 0},
  { 1242 /* crc32b */, X86::CRC32r32m8, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR32 }, 0},
  { 1242 /* crc32b */, X86::CRC32r64m8, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR64 }, 0},
  { 1249 /* crc32l */, X86::CRC32r32r32, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 1249 /* crc32l */, X86::CRC32r32m32, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1256 /* crc32q */, X86::CRC32r64r64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 1256 /* crc32q */, X86::CRC32r64m64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1263 /* crc32w */, X86::CRC32r32r16, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 0},
  { 1263 /* crc32w */, X86::CRC32r32m16, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR32 }, 0},
  { 1270 /* cs */, X86::CS_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 1270 /* cs */, X86::CS_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 1273 /* cvtdq2pd */, X86::CVTDQ2PDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1273 /* cvtdq2pd */, X86::CVTDQ2PDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1273 /* cvtdq2pd */, X86::CVTDQ2PDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1273 /* cvtdq2pd */, X86::CVTDQ2PDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1282 /* cvtdq2ps */, X86::CVTDQ2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1282 /* cvtdq2ps */, X86::CVTDQ2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1282 /* cvtdq2ps */, X86::CVTDQ2PSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1282 /* cvtdq2ps */, X86::CVTDQ2PSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1291 /* cvtpd2dq */, X86::CVTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1291 /* cvtpd2dq */, X86::CVTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1291 /* cvtpd2dq */, X86::CVTPD2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1291 /* cvtpd2dq */, X86::CVTPD2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1300 /* cvtpd2pi */, X86::MMX_CVTPD2PIirr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 1300 /* cvtpd2pi */, X86::MMX_CVTPD2PIirm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR64, MCK_Mem128 }, 1},
  { 1300 /* cvtpd2pi */, X86::MMX_CVTPD2PIirr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 1300 /* cvtpd2pi */, X86::MMX_CVTPD2PIirm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR64 }, 0},
  { 1309 /* cvtpd2ps */, X86::CVTPD2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1309 /* cvtpd2ps */, X86::CVTPD2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1309 /* cvtpd2ps */, X86::CVTPD2PSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1309 /* cvtpd2ps */, X86::CVTPD2PSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1318 /* cvtpi2pd */, X86::MMX_CVTPI2PDirr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_FR32 }, 0},
  { 1318 /* cvtpi2pd */, X86::MMX_CVTPI2PDirr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR64 }, 1},
  { 1318 /* cvtpi2pd */, X86::MMX_CVTPI2PDirm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1318 /* cvtpi2pd */, X86::MMX_CVTPI2PDirm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1327 /* cvtpi2ps */, X86::MMX_CVTPI2PSirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_FR32 }, 0},
  { 1327 /* cvtpi2ps */, X86::MMX_CVTPI2PSirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_VR64 }, 1},
  { 1327 /* cvtpi2ps */, X86::MMX_CVTPI2PSirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1327 /* cvtpi2ps */, X86::MMX_CVTPI2PSirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1336 /* cvtps2dq */, X86::CVTPS2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1336 /* cvtps2dq */, X86::CVTPS2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1336 /* cvtps2dq */, X86::CVTPS2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1336 /* cvtps2dq */, X86::CVTPS2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1345 /* cvtps2pd */, X86::CVTPS2PDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1345 /* cvtps2pd */, X86::CVTPS2PDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1345 /* cvtps2pd */, X86::CVTPS2PDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1345 /* cvtps2pd */, X86::CVTPS2PDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1354 /* cvtps2pi */, X86::MMX_CVTPS2PIirr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 1354 /* cvtps2pi */, X86::MMX_CVTPS2PIirm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 1354 /* cvtps2pi */, X86::MMX_CVTPS2PIirr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 1354 /* cvtps2pi */, X86::MMX_CVTPS2PIirm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 1363 /* cvtsd2si */, X86::CVTSD2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 1363 /* cvtsd2si */, X86::CVTSD2SIrm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 1363 /* cvtsd2si */, X86::CVTSD2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 1363 /* cvtsd2si */, X86::CVTSD2SI64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 1372 /* cvtsd2sil */, X86::CVTSD2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 1372 /* cvtsd2sil */, X86::CVTSD2SIrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 1382 /* cvtsd2siq */, X86::CVTSD2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 1382 /* cvtsd2siq */, X86::CVTSD2SI64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 1392 /* cvtsd2ss */, X86::CVTSD2SSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1392 /* cvtsd2ss */, X86::CVTSD2SSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1392 /* cvtsd2ss */, X86::CVTSD2SSrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1392 /* cvtsd2ss */, X86::CVTSD2SSrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SD64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SD64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1401 /* cvtsi2sd */, X86::CVTSI2SDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 1410 /* cvtsi2sdq */, X86::CVTSI2SD64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 1410 /* cvtsi2sdq */, X86::CVTSI2SD64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SS64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SS64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1420 /* cvtsi2ss */, X86::CVTSI2SSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 1429 /* cvtsi2ssq */, X86::CVTSI2SS64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 1429 /* cvtsi2ssq */, X86::CVTSI2SS64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1439 /* cvtss2sd */, X86::CVTSS2SDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1439 /* cvtss2sd */, X86::CVTSS2SDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1439 /* cvtss2sd */, X86::CVTSS2SDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 1439 /* cvtss2sd */, X86::CVTSS2SDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 1448 /* cvtss2si */, X86::CVTSS2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 1448 /* cvtss2si */, X86::CVTSS2SIrm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 1448 /* cvtss2si */, X86::CVTSS2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 1448 /* cvtss2si */, X86::CVTSS2SI64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 1457 /* cvtss2sil */, X86::CVTSS2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 1457 /* cvtss2sil */, X86::CVTSS2SIrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 1467 /* cvtss2siq */, X86::CVTSS2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 1467 /* cvtss2siq */, X86::CVTSS2SI64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 1477 /* cvttpd2dq */, X86::CVTTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1477 /* cvttpd2dq */, X86::CVTTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1477 /* cvttpd2dq */, X86::CVTTPD2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1477 /* cvttpd2dq */, X86::CVTTPD2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1487 /* cvttpd2pi */, X86::MMX_CVTTPD2PIirr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 1487 /* cvttpd2pi */, X86::MMX_CVTTPD2PIirm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR64, MCK_Mem128 }, 1},
  { 1487 /* cvttpd2pi */, X86::MMX_CVTTPD2PIirr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 1487 /* cvttpd2pi */, X86::MMX_CVTTPD2PIirm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR64 }, 0},
  { 1497 /* cvttps2dq */, X86::CVTTPS2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1497 /* cvttps2dq */, X86::CVTTPS2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1497 /* cvttps2dq */, X86::CVTTPS2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1497 /* cvttps2dq */, X86::CVTTPS2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1507 /* cvttps2pi */, X86::MMX_CVTTPS2PIirr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 1507 /* cvttps2pi */, X86::MMX_CVTTPS2PIirm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 1507 /* cvttps2pi */, X86::MMX_CVTTPS2PIirr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 1507 /* cvttps2pi */, X86::MMX_CVTTPS2PIirm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR32, MCK_Mem64 }, 1},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SI64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 1517 /* cvttsd2si */, X86::CVTTSD2SIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR32 }, 0},
  { 1527 /* cvttsd2siq */, X86::CVTTSD2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 1527 /* cvttsd2siq */, X86::CVTTSD2SI64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 1538 /* cvttss2si */, X86::CVTTSS2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 1538 /* cvttss2si */, X86::CVTTSS2SIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 1538 /* cvttss2si */, X86::CVTTSS2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 1538 /* cvttss2si */, X86::CVTTSS2SI64rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR64, MCK_Mem32 }, 1},
  { 1538 /* cvttss2si */, X86::CVTTSS2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 1538 /* cvttss2si */, X86::CVTTSS2SIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 1548 /* cvttss2siq */, X86::CVTTSS2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 1548 /* cvttss2siq */, X86::CVTTSS2SI64rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR64 }, 0},
  { 1559 /* cwd */, X86::CWD, Convert_NoOperands, 0, {  }, 1},
  { 1563 /* cwde */, X86::CWDE, Convert_NoOperands, 0, {  }, 1},
  { 1568 /* cwtd */, X86::CWD, Convert_NoOperands, 0, {  }, 0},
  { 1573 /* cwtl */, X86::CWDE, Convert_NoOperands, 0, {  }, 0},
  { 1578 /* daa */, X86::DAA, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 1578 /* daa */, X86::DAA, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 1582 /* das */, X86::DAS, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 1582 /* das */, X86::DAS, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 1586 /* data16 */, X86::DATA16_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 1586 /* data16 */, X86::DATA16_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 1593 /* dec */, X86::DEC8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 1593 /* dec */, X86::DEC16r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR16 }, 1},
  { 1593 /* dec */, X86::DEC64_16r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR16 }, 1},
  { 1593 /* dec */, X86::DEC32r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR32 }, 1},
  { 1593 /* dec */, X86::DEC64_32r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 1593 /* dec */, X86::DEC64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 1593 /* dec */, X86::DEC16m, Convert__Mem165_0, Feature_In32BitMode, { MCK_Mem16 }, 1},
  { 1593 /* dec */, X86::DEC64_16m, Convert__Mem165_0, Feature_In64BitMode, { MCK_Mem16 }, 1},
  { 1593 /* dec */, X86::DEC32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 1},
  { 1593 /* dec */, X86::DEC64_32m, Convert__Mem325_0, Feature_In64BitMode, { MCK_Mem32 }, 1},
  { 1593 /* dec */, X86::DEC64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1593 /* dec */, X86::DEC8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 1597 /* decb */, X86::DEC8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 1597 /* decb */, X86::DEC8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 1602 /* decl */, X86::DEC32r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR32 }, 0},
  { 1602 /* decl */, X86::DEC64_32r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 1602 /* decl */, X86::DEC32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 0},
  { 1602 /* decl */, X86::DEC64_32m, Convert__Mem325_0, Feature_In64BitMode, { MCK_Mem32 }, 0},
  { 1607 /* decq */, X86::DEC64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 1607 /* decq */, X86::DEC64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1612 /* decw */, X86::DEC16r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR16 }, 0},
  { 1612 /* decw */, X86::DEC64_16r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR16 }, 0},
  { 1612 /* decw */, X86::DEC16m, Convert__Mem165_0, Feature_In32BitMode, { MCK_Mem16 }, 0},
  { 1612 /* decw */, X86::DEC64_16m, Convert__Mem165_0, Feature_In64BitMode, { MCK_Mem16 }, 0},
  { 1617 /* div */, X86::DIV8r, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 1617 /* div */, X86::DIV16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 1617 /* div */, X86::DIV32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 1617 /* div */, X86::DIV64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 1617 /* div */, X86::DIV16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 1617 /* div */, X86::DIV32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1617 /* div */, X86::DIV64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1617 /* div */, X86::DIV8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 1621 /* divb */, X86::DIV8r, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 1621 /* divb */, X86::DIV8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 1621 /* divb */, X86::DIV8r, Convert__Reg1_0, 0, { MCK_GR8, MCK__PCT_al }, 1},
  { 1621 /* divb */, X86::DIV8r, Convert__Reg1_0, 0, { MCK_GR8, MCK_AL }, 0},
  { 1621 /* divb */, X86::DIV8m, Convert__Mem85_0, 0, { MCK_Mem8, MCK__PCT_al }, 1},
  { 1621 /* divb */, X86::DIV8m, Convert__Mem85_0, 0, { MCK_Mem8, MCK_AL }, 0},
  { 1626 /* divl */, X86::DIV32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 1626 /* divl */, X86::DIV32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1626 /* divl */, X86::DIV32r, Convert__Reg1_0, 0, { MCK_GR32, MCK__PCT_eax }, 1},
  { 1626 /* divl */, X86::DIV32r, Convert__Reg1_0, 0, { MCK_GR32, MCK_EAX }, 0},
  { 1626 /* divl */, X86::DIV32m, Convert__Mem325_0, 0, { MCK_Mem32, MCK__PCT_eax }, 1},
  { 1626 /* divl */, X86::DIV32m, Convert__Mem325_0, 0, { MCK_Mem32, MCK_EAX }, 0},
  { 1631 /* divpd */, X86::DIVPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1631 /* divpd */, X86::DIVPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1631 /* divpd */, X86::DIVPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1631 /* divpd */, X86::DIVPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1637 /* divps */, X86::DIVPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1637 /* divps */, X86::DIVPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1637 /* divps */, X86::DIVPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 1637 /* divps */, X86::DIVPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 1643 /* divq */, X86::DIV64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 1643 /* divq */, X86::DIV64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1643 /* divq */, X86::DIV64r, Convert__Reg1_0, 0, { MCK_GR64, MCK__PCT_rax }, 1},
  { 1643 /* divq */, X86::DIV64r, Convert__Reg1_0, 0, { MCK_GR64, MCK_RAX }, 0},
  { 1643 /* divq */, X86::DIV64m, Convert__Mem645_0, 0, { MCK_Mem64, MCK__PCT_rax }, 1},
  { 1643 /* divq */, X86::DIV64m, Convert__Mem645_0, 0, { MCK_Mem64, MCK_RAX }, 0},
  { 1648 /* divsd */, X86::DIVSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1648 /* divsd */, X86::DIVSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1648 /* divsd */, X86::DIVSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 1648 /* divsd */, X86::DIVSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 1654 /* divss */, X86::DIVSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1654 /* divss */, X86::DIVSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1654 /* divss */, X86::DIVSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 1654 /* divss */, X86::DIVSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 1660 /* divw */, X86::DIV16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 1660 /* divw */, X86::DIV16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 1660 /* divw */, X86::DIV16r, Convert__Reg1_0, 0, { MCK_GR16, MCK__PCT_ax }, 1},
  { 1660 /* divw */, X86::DIV16r, Convert__Reg1_0, 0, { MCK_GR16, MCK_AX }, 0},
  { 1660 /* divw */, X86::DIV16m, Convert__Mem165_0, 0, { MCK_Mem16, MCK__PCT_ax }, 1},
  { 1660 /* divw */, X86::DIV16m, Convert__Mem165_0, 0, { MCK_Mem16, MCK_AX }, 0},
  { 1665 /* dppd */, X86::DPPDrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 1665 /* dppd */, X86::DPPDrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 1665 /* dppd */, X86::DPPDrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 1665 /* dppd */, X86::DPPDrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 1670 /* dpps */, X86::DPPSrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 1670 /* dpps */, X86::DPPSrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 1670 /* dpps */, X86::DPPSrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 1670 /* dpps */, X86::DPPSrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 1675 /* ds */, X86::DS_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 1675 /* ds */, X86::DS_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 1678 /* emms */, X86::MMX_EMMS, Convert_NoOperands, 0, {  }, 0},
  { 1678 /* emms */, X86::MMX_EMMS, Convert_NoOperands, 0, {  }, 1},
  { 1683 /* enter */, X86::ENTER, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 0},
  { 1683 /* enter */, X86::ENTER, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 1},
  { 1689 /* es */, X86::ES_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 1689 /* es */, X86::ES_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 1692 /* extractps */, X86::EXTRACTPSrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 1692 /* extractps */, X86::EXTRACTPSrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 1692 /* extractps */, X86::EXTRACTPSmr, Convert__Mem325_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem32 }, 0},
  { 1692 /* extractps */, X86::EXTRACTPSmr, Convert__Mem325_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 1702 /* extrq */, X86::EXTRQ, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 1702 /* extrq */, X86::EXTRQ, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 1702 /* extrq */, X86::EXTRQI, Convert__Reg1_0__Tie0__Imm1_1__Imm1_2, 0, { MCK_FR32, MCK_Imm, MCK_Imm }, 1},
  { 1702 /* extrq */, X86::EXTRQI, Convert__Reg1_2__Tie0__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm, MCK_FR32 }, 0},
  { 1708 /* f2xm1 */, X86::F2XM1, Convert_NoOperands, 0, {  }, 0},
  { 1708 /* f2xm1 */, X86::F2XM1, Convert_NoOperands, 0, {  }, 1},
  { 1714 /* fabs */, X86::ABS_F, Convert_NoOperands, 0, {  }, 0},
  { 1714 /* fabs */, X86::ABS_F, Convert_NoOperands, 0, {  }, 1},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1719 /* fadd */, X86::ADD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1719 /* fadd */, X86::ADD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1719 /* fadd */, X86::ADD_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1719 /* fadd */, X86::ADD_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1719 /* fadd */, X86::ADD_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 1724 /* faddl */, X86::ADD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__regST1, 0, {  }, 0},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__regST1, 0, {  }, 1},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1730 /* faddp */, X86::ADD_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1736 /* fadds */, X86::ADD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1742 /* fbld */, X86::FBLDm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1742 /* fbld */, X86::FBLDm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1747 /* fbstp */, X86::FBSTPm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1747 /* fbstp */, X86::FBSTPm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1753 /* fchs */, X86::CHS_F, Convert_NoOperands, 0, {  }, 0},
  { 1753 /* fchs */, X86::CHS_F, Convert_NoOperands, 0, {  }, 1},
  { 1758 /* fcmovb */, X86::CMOVB_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1758 /* fcmovb */, X86::CMOVB_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1765 /* fcmovbe */, X86::CMOVBE_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1765 /* fcmovbe */, X86::CMOVBE_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1773 /* fcmove */, X86::CMOVE_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1773 /* fcmove */, X86::CMOVE_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1780 /* fcmovnb */, X86::CMOVNB_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1780 /* fcmovnb */, X86::CMOVNB_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1788 /* fcmovnbe */, X86::CMOVNBE_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1788 /* fcmovnbe */, X86::CMOVNBE_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1797 /* fcmovne */, X86::CMOVNE_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1797 /* fcmovne */, X86::CMOVNE_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1805 /* fcmovnu */, X86::CMOVNP_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1805 /* fcmovnu */, X86::CMOVNP_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1813 /* fcmovu */, X86::CMOVP_F, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 1},
  { 1813 /* fcmovu */, X86::CMOVP_F, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1820 /* fcom */, X86::COM_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1820 /* fcom */, X86::COM_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1820 /* fcom */, X86::FCOM32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1820 /* fcom */, X86::FCOM64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__regST1, 0, {  }, 0},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__regST1, 0, {  }, 1},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1825 /* fcomi */, X86::COM_FIr, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1831 /* fcoml */, X86::FCOM64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1837 /* fcomp */, X86::COMP_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1837 /* fcomp */, X86::COMP_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1837 /* fcomp */, X86::FCOMP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1837 /* fcomp */, X86::FCOMP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__regST1, 0, {  }, 0},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__regST1, 0, {  }, 1},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1843 /* fcompi */, X86::COM_FIPr, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1850 /* fcompl */, X86::FCOMP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1857 /* fcompp */, X86::FCOMPP, Convert_NoOperands, 0, {  }, 0},
  { 1857 /* fcompp */, X86::FCOMPP, Convert_NoOperands, 0, {  }, 1},
  { 1864 /* fcomps */, X86::FCOMP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1871 /* fcoms */, X86::FCOM32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1877 /* fcos */, X86::COS_F, Convert_NoOperands, 0, {  }, 0},
  { 1877 /* fcos */, X86::COS_F, Convert_NoOperands, 0, {  }, 1},
  { 1882 /* fdecstp */, X86::FDECSTP, Convert_NoOperands, 0, {  }, 0},
  { 1882 /* fdecstp */, X86::FDECSTP, Convert_NoOperands, 0, {  }, 1},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1890 /* fdiv */, X86::DIV_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1890 /* fdiv */, X86::DIV_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1890 /* fdiv */, X86::DIVR_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1890 /* fdiv */, X86::DIV_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1890 /* fdiv */, X86::DIV_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 1895 /* fdivl */, X86::DIV_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__regST1, 0, {  }, 0},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__regST1, 0, {  }, 1},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1901 /* fdivp */, X86::DIV_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1901 /* fdivp */, X86::DIVR_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1907 /* fdivr */, X86::DIVR_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1907 /* fdivr */, X86::DIVR_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1907 /* fdivr */, X86::DIV_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1907 /* fdivr */, X86::DIVR_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1907 /* fdivr */, X86::DIVR_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 1913 /* fdivrl */, X86::DIVR_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__regST1, 0, {  }, 0},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__regST1, 0, {  }, 1},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1920 /* fdivrp */, X86::DIVR_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 1920 /* fdivrp */, X86::DIV_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 1927 /* fdivrs */, X86::DIVR_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1934 /* fdivs */, X86::DIV_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1940 /* femms */, X86::FEMMS, Convert_NoOperands, 0, {  }, 0},
  { 1940 /* femms */, X86::FEMMS, Convert_NoOperands, 0, {  }, 1},
  { 1946 /* ffree */, X86::FFREE, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 1946 /* ffree */, X86::FFREE, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 1952 /* fiadd */, X86::ADD_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 1952 /* fiadd */, X86::ADD_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1958 /* fiaddl */, X86::ADD_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1965 /* fiadds */, X86::ADD_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 1972 /* ficom */, X86::FICOM16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 1972 /* ficom */, X86::FICOM32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1978 /* ficoml */, X86::FICOM32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 1985 /* ficomp */, X86::FICOMP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 1985 /* ficomp */, X86::FICOMP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 1992 /* ficompl */, X86::FICOMP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2000 /* ficomps */, X86::FICOMP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2008 /* ficoms */, X86::FICOM16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2015 /* fidiv */, X86::DIV_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2015 /* fidiv */, X86::DIV_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2021 /* fidivl */, X86::DIV_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2028 /* fidivr */, X86::DIVR_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2028 /* fidivr */, X86::DIVR_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2035 /* fidivrl */, X86::DIVR_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2043 /* fidivrs */, X86::DIVR_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2051 /* fidivs */, X86::DIV_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2058 /* fild */, X86::ILD_F16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2058 /* fild */, X86::ILD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2058 /* fild */, X86::ILD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2063 /* fildl */, X86::ILD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2069 /* fildll */, X86::ILD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2076 /* filds */, X86::ILD_F16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2082 /* fimul */, X86::MUL_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2082 /* fimul */, X86::MUL_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2088 /* fimull */, X86::MUL_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2095 /* fimuls */, X86::MUL_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2102 /* fincstp */, X86::FINCSTP, Convert_NoOperands, 0, {  }, 0},
  { 2102 /* fincstp */, X86::FINCSTP, Convert_NoOperands, 0, {  }, 1},
  { 2110 /* fist */, X86::IST_F16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2110 /* fist */, X86::IST_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2115 /* fistl */, X86::IST_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2121 /* fistp */, X86::IST_FP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2121 /* fistp */, X86::IST_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2121 /* fistp */, X86::IST_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2127 /* fistpl */, X86::IST_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2134 /* fistpll */, X86::IST_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2142 /* fistps */, X86::IST_FP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2149 /* fists */, X86::IST_F16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2155 /* fisttp */, X86::ISTT_FP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2155 /* fisttp */, X86::ISTT_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2155 /* fisttp */, X86::ISTT_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2162 /* fisttpl */, X86::ISTT_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2170 /* fisttpll */, X86::ISTT_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2179 /* fisttps */, X86::ISTT_FP16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2187 /* fisub */, X86::SUB_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2187 /* fisub */, X86::SUB_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2193 /* fisubl */, X86::SUB_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2200 /* fisubr */, X86::SUBR_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2200 /* fisubr */, X86::SUBR_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2207 /* fisubrl */, X86::SUBR_FI32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2215 /* fisubrs */, X86::SUBR_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2223 /* fisubs */, X86::SUB_FI16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2230 /* fld */, X86::LD_Frr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2230 /* fld */, X86::LD_Frr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2230 /* fld */, X86::LD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2230 /* fld */, X86::LD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2230 /* fld */, X86::LD_F80m, Convert__Mem805_0, 0, { MCK_Mem80 }, 1},
  { 2234 /* fld1 */, X86::LD_F1, Convert_NoOperands, 0, {  }, 0},
  { 2234 /* fld1 */, X86::LD_F1, Convert_NoOperands, 0, {  }, 1},
  { 2239 /* fldcw */, X86::FLDCW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2239 /* fldcw */, X86::FLDCW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2245 /* fldenv */, X86::FLDENVm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2245 /* fldenv */, X86::FLDENVm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2252 /* fldl */, X86::LD_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2257 /* fldl2e */, X86::FLDL2E, Convert_NoOperands, 0, {  }, 0},
  { 2257 /* fldl2e */, X86::FLDL2E, Convert_NoOperands, 0, {  }, 1},
  { 2264 /* fldl2t */, X86::FLDL2T, Convert_NoOperands, 0, {  }, 0},
  { 2264 /* fldl2t */, X86::FLDL2T, Convert_NoOperands, 0, {  }, 1},
  { 2271 /* fldlg2 */, X86::FLDLG2, Convert_NoOperands, 0, {  }, 0},
  { 2271 /* fldlg2 */, X86::FLDLG2, Convert_NoOperands, 0, {  }, 1},
  { 2278 /* fldln2 */, X86::FLDLN2, Convert_NoOperands, 0, {  }, 0},
  { 2278 /* fldln2 */, X86::FLDLN2, Convert_NoOperands, 0, {  }, 1},
  { 2285 /* fldpi */, X86::FLDPI, Convert_NoOperands, 0, {  }, 0},
  { 2285 /* fldpi */, X86::FLDPI, Convert_NoOperands, 0, {  }, 1},
  { 2291 /* flds */, X86::LD_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2296 /* fldt */, X86::LD_F80m, Convert__Mem805_0, 0, { MCK_Mem80 }, 0},
  { 2301 /* fldz */, X86::LD_F0, Convert_NoOperands, 0, {  }, 0},
  { 2301 /* fldz */, X86::LD_F0, Convert_NoOperands, 0, {  }, 1},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2306 /* fmul */, X86::MUL_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2306 /* fmul */, X86::MUL_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2306 /* fmul */, X86::MUL_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2306 /* fmul */, X86::MUL_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2306 /* fmul */, X86::MUL_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 2311 /* fmull */, X86::MUL_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__regST1, 0, {  }, 0},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__regST1, 0, {  }, 1},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2317 /* fmulp */, X86::MUL_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2323 /* fmuls */, X86::MUL_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2329 /* fnclex */, X86::FNCLEX, Convert_NoOperands, 0, {  }, 0},
  { 2329 /* fnclex */, X86::FNCLEX, Convert_NoOperands, 0, {  }, 1},
  { 2336 /* fninit */, X86::FNINIT, Convert_NoOperands, 0, {  }, 0},
  { 2336 /* fninit */, X86::FNINIT, Convert_NoOperands, 0, {  }, 1},
  { 2343 /* fnop */, X86::FNOP, Convert_NoOperands, 0, {  }, 0},
  { 2343 /* fnop */, X86::FNOP, Convert_NoOperands, 0, {  }, 1},
  { 2348 /* fnsave */, X86::FSAVEm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2348 /* fnsave */, X86::FSAVEm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2355 /* fnstcw */, X86::FNSTCW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2355 /* fnstcw */, X86::FNSTCW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2362 /* fnstenv */, X86::FSTENVm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2362 /* fnstenv */, X86::FSTENVm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, {  }, 0},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, {  }, 1},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK__PCT_al }, 1},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK__PCT_ax }, 1},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK__PCT_eax }, 1},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK_AL }, 0},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK_AX }, 0},
  { 2370 /* fnstsw */, X86::FNSTSW16r, Convert_NoOperands, 0, { MCK_EAX }, 0},
  { 2370 /* fnstsw */, X86::FNSTSWm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2370 /* fnstsw */, X86::FNSTSWm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2377 /* fpatan */, X86::FPATAN, Convert_NoOperands, 0, {  }, 0},
  { 2377 /* fpatan */, X86::FPATAN, Convert_NoOperands, 0, {  }, 1},
  { 2384 /* fprem */, X86::FPREM, Convert_NoOperands, 0, {  }, 0},
  { 2384 /* fprem */, X86::FPREM, Convert_NoOperands, 0, {  }, 1},
  { 2390 /* fprem1 */, X86::FPREM1, Convert_NoOperands, 0, {  }, 0},
  { 2390 /* fprem1 */, X86::FPREM1, Convert_NoOperands, 0, {  }, 1},
  { 2397 /* fptan */, X86::FPTAN, Convert_NoOperands, 0, {  }, 0},
  { 2397 /* fptan */, X86::FPTAN, Convert_NoOperands, 0, {  }, 1},
  { 2403 /* frndint */, X86::FRNDINT, Convert_NoOperands, 0, {  }, 0},
  { 2403 /* frndint */, X86::FRNDINT, Convert_NoOperands, 0, {  }, 1},
  { 2411 /* frstor */, X86::FRSTORm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2411 /* frstor */, X86::FRSTORm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2418 /* fs */, X86::FS_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 2418 /* fs */, X86::FS_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 2421 /* fscale */, X86::FSCALE, Convert_NoOperands, 0, {  }, 0},
  { 2421 /* fscale */, X86::FSCALE, Convert_NoOperands, 0, {  }, 1},
  { 2428 /* fsin */, X86::SIN_F, Convert_NoOperands, 0, {  }, 0},
  { 2428 /* fsin */, X86::SIN_F, Convert_NoOperands, 0, {  }, 1},
  { 2433 /* fsincos */, X86::FSINCOS, Convert_NoOperands, 0, {  }, 0},
  { 2433 /* fsincos */, X86::FSINCOS, Convert_NoOperands, 0, {  }, 1},
  { 2441 /* fsqrt */, X86::SQRT_F, Convert_NoOperands, 0, {  }, 0},
  { 2441 /* fsqrt */, X86::SQRT_F, Convert_NoOperands, 0, {  }, 1},
  { 2447 /* fst */, X86::ST_Frr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2447 /* fst */, X86::ST_Frr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2447 /* fst */, X86::ST_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2447 /* fst */, X86::ST_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2451 /* fstl */, X86::ST_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2456 /* fstp */, X86::ST_FPrr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2456 /* fstp */, X86::ST_FPrr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2456 /* fstp */, X86::ST_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2456 /* fstp */, X86::ST_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2456 /* fstp */, X86::ST_FP80m, Convert__Mem805_0, 0, { MCK_Mem80 }, 1},
  { 2461 /* fstpl */, X86::ST_FP64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2467 /* fstps */, X86::ST_FP32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2473 /* fstpt */, X86::ST_FP80m, Convert__Mem805_0, 0, { MCK_Mem80 }, 0},
  { 2479 /* fsts */, X86::ST_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2484 /* fsub */, X86::SUB_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2484 /* fsub */, X86::SUB_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2484 /* fsub */, X86::SUBR_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2484 /* fsub */, X86::SUB_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2484 /* fsub */, X86::SUB_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 2489 /* fsubl */, X86::SUB_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__regST1, 0, {  }, 0},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__regST1, 0, {  }, 1},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2495 /* fsubp */, X86::SUB_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2495 /* fsubp */, X86::SUBR_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2501 /* fsubr */, X86::SUBR_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2501 /* fsubr */, X86::SUBR_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2501 /* fsubr */, X86::SUB_FrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2501 /* fsubr */, X86::SUBR_FST0r, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2501 /* fsubr */, X86::SUBR_FrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 1},
  { 2507 /* fsubrl */, X86::SUBR_F64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__regST1, 0, {  }, 0},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__regST1, 0, {  }, 1},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2514 /* fsubrp */, X86::SUBR_FPrST0, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__Reg1_1, 0, { MCK__PCT_st_40_0_41_, MCK_RST }, 1},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__Reg1_1, 0, { MCK_ST0, MCK_RST }, 0},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2514 /* fsubrp */, X86::SUB_FPrST0, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2521 /* fsubrs */, X86::SUBR_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2528 /* fsubs */, X86::SUB_F32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2534 /* ftst */, X86::TST_F, Convert_NoOperands, 0, {  }, 0},
  { 2534 /* ftst */, X86::TST_F, Convert_NoOperands, 0, {  }, 1},
  { 2539 /* fucom */, X86::UCOM_Fr, Convert__regST1, 0, {  }, 0},
  { 2539 /* fucom */, X86::UCOM_Fr, Convert__regST1, 0, {  }, 1},
  { 2539 /* fucom */, X86::UCOM_Fr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2539 /* fucom */, X86::UCOM_Fr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__regST1, 0, {  }, 0},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__regST1, 0, {  }, 1},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2545 /* fucomi */, X86::UCOM_FIr, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2552 /* fucomp */, X86::UCOM_FPr, Convert__regST1, 0, {  }, 0},
  { 2552 /* fucomp */, X86::UCOM_FPr, Convert__regST1, 0, {  }, 1},
  { 2552 /* fucomp */, X86::UCOM_FPr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2552 /* fucomp */, X86::UCOM_FPr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__regST1, 0, {  }, 0},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__regST1, 0, {  }, 1},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__regST0, 0, { MCK__PCT_st_40_0_41_, MCK__PCT_st_40_0_41_ }, 1},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__regST0, 0, { MCK_ST0, MCK_ST0 }, 0},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__Reg1_0, 0, { MCK_RST, MCK__PCT_st_40_0_41_ }, 1},
  { 2559 /* fucompi */, X86::UCOM_FIPr, Convert__Reg1_0, 0, { MCK_RST, MCK_ST0 }, 0},
  { 2567 /* fucompp */, X86::UCOM_FPPr, Convert_NoOperands, 0, {  }, 0},
  { 2567 /* fucompp */, X86::UCOM_FPPr, Convert_NoOperands, 0, {  }, 1},
  { 2575 /* fxam */, X86::FXAM, Convert_NoOperands, 0, {  }, 0},
  { 2575 /* fxam */, X86::FXAM, Convert_NoOperands, 0, {  }, 1},
  { 2580 /* fxch */, X86::XCH_F, Convert__regST1, 0, {  }, 0},
  { 2580 /* fxch */, X86::XCH_F, Convert__regST1, 0, {  }, 1},
  { 2580 /* fxch */, X86::XCH_F, Convert__Reg1_0, 0, { MCK_RST }, 0},
  { 2580 /* fxch */, X86::XCH_F, Convert__Reg1_0, 0, { MCK_RST }, 1},
  { 2585 /* fxrstor */, X86::FXRSTOR, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 2585 /* fxrstor */, X86::FXRSTOR, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 2593 /* fxrstorq */, X86::FXRSTOR64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 0},
  { 2593 /* fxrstorq */, X86::FXRSTOR64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 1},
  { 2602 /* fxsave */, X86::FXSAVE, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 2602 /* fxsave */, X86::FXSAVE, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 2609 /* fxsaveq */, X86::FXSAVE64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 0},
  { 2609 /* fxsaveq */, X86::FXSAVE64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 1},
  { 2617 /* fxtract */, X86::FXTRACT, Convert_NoOperands, 0, {  }, 0},
  { 2617 /* fxtract */, X86::FXTRACT, Convert_NoOperands, 0, {  }, 1},
  { 2625 /* fyl2x */, X86::FYL2X, Convert_NoOperands, 0, {  }, 0},
  { 2625 /* fyl2x */, X86::FYL2X, Convert_NoOperands, 0, {  }, 1},
  { 2631 /* fyl2xp1 */, X86::FYL2XP1, Convert_NoOperands, 0, {  }, 0},
  { 2631 /* fyl2xp1 */, X86::FYL2XP1, Convert_NoOperands, 0, {  }, 1},
  { 2639 /* gs */, X86::GS_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 2639 /* gs */, X86::GS_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 2642 /* haddpd */, X86::HADDPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 2642 /* haddpd */, X86::HADDPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 2642 /* haddpd */, X86::HADDPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 2642 /* haddpd */, X86::HADDPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 2649 /* haddps */, X86::HADDPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 2649 /* haddps */, X86::HADDPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 2649 /* haddps */, X86::HADDPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 2649 /* haddps */, X86::HADDPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 2656 /* hlt */, X86::HLT, Convert_NoOperands, 0, {  }, 0},
  { 2656 /* hlt */, X86::HLT, Convert_NoOperands, 0, {  }, 1},
  { 2660 /* hsubpd */, X86::HSUBPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 2660 /* hsubpd */, X86::HSUBPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 2660 /* hsubpd */, X86::HSUBPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 2660 /* hsubpd */, X86::HSUBPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 2667 /* hsubps */, X86::HSUBPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 2667 /* hsubps */, X86::HSUBPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 2667 /* hsubps */, X86::HSUBPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 2667 /* hsubps */, X86::HSUBPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 2674 /* idiv */, X86::IDIV8r, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 2674 /* idiv */, X86::IDIV16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 2674 /* idiv */, X86::IDIV32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 2674 /* idiv */, X86::IDIV64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 2674 /* idiv */, X86::IDIV16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2674 /* idiv */, X86::IDIV32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2674 /* idiv */, X86::IDIV64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2674 /* idiv */, X86::IDIV8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 2679 /* idivb */, X86::IDIV8r, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 2679 /* idivb */, X86::IDIV8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 2679 /* idivb */, X86::IDIV8r, Convert__Reg1_0, 0, { MCK_GR8, MCK__PCT_al }, 1},
  { 2679 /* idivb */, X86::IDIV8r, Convert__Reg1_0, 0, { MCK_GR8, MCK_AL }, 0},
  { 2679 /* idivb */, X86::IDIV8m, Convert__Mem85_0, 0, { MCK_Mem8, MCK__PCT_al }, 1},
  { 2679 /* idivb */, X86::IDIV8m, Convert__Mem85_0, 0, { MCK_Mem8, MCK_AL }, 0},
  { 2685 /* idivl */, X86::IDIV32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 2685 /* idivl */, X86::IDIV32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2685 /* idivl */, X86::IDIV32r, Convert__Reg1_0, 0, { MCK_GR32, MCK__PCT_eax }, 1},
  { 2685 /* idivl */, X86::IDIV32r, Convert__Reg1_0, 0, { MCK_GR32, MCK_EAX }, 0},
  { 2685 /* idivl */, X86::IDIV32m, Convert__Mem325_0, 0, { MCK_Mem32, MCK__PCT_eax }, 1},
  { 2685 /* idivl */, X86::IDIV32m, Convert__Mem325_0, 0, { MCK_Mem32, MCK_EAX }, 0},
  { 2691 /* idivq */, X86::IDIV64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 2691 /* idivq */, X86::IDIV64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2691 /* idivq */, X86::IDIV64r, Convert__Reg1_0, 0, { MCK_GR64, MCK__PCT_rax }, 1},
  { 2691 /* idivq */, X86::IDIV64r, Convert__Reg1_0, 0, { MCK_GR64, MCK_RAX }, 0},
  { 2691 /* idivq */, X86::IDIV64m, Convert__Mem645_0, 0, { MCK_Mem64, MCK__PCT_rax }, 1},
  { 2691 /* idivq */, X86::IDIV64m, Convert__Mem645_0, 0, { MCK_Mem64, MCK_RAX }, 0},
  { 2697 /* idivw */, X86::IDIV16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 2697 /* idivw */, X86::IDIV16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2697 /* idivw */, X86::IDIV16r, Convert__Reg1_0, 0, { MCK_GR16, MCK__PCT_ax }, 1},
  { 2697 /* idivw */, X86::IDIV16r, Convert__Reg1_0, 0, { MCK_GR16, MCK_AX }, 0},
  { 2697 /* idivw */, X86::IDIV16m, Convert__Mem165_0, 0, { MCK_Mem16, MCK__PCT_ax }, 1},
  { 2697 /* idivw */, X86::IDIV16m, Convert__Mem165_0, 0, { MCK_Mem16, MCK_AX }, 0},
  { 2703 /* imul */, X86::IMUL8r, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 2703 /* imul */, X86::IMUL16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 2703 /* imul */, X86::IMUL32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 2703 /* imul */, X86::IMUL64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 2703 /* imul */, X86::IMUL16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 2703 /* imul */, X86::IMUL32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 2703 /* imul */, X86::IMUL64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2703 /* imul */, X86::IMUL8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 2703 /* imul */, X86::IMUL16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 2703 /* imul */, X86::IMUL16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 2703 /* imul */, X86::IMUL32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 2703 /* imul */, X86::IMUL32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 2703 /* imul */, X86::IMUL64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 2703 /* imul */, X86::IMUL64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 2703 /* imul */, X86::IMUL16rri8, Convert__Reg1_0__Reg1_1__ImmSExti16i81_2, 0, { MCK_GR16, MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 2703 /* imul */, X86::IMUL16rri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_GR16, MCK_GR16, MCK_Imm }, 1},
  { 2703 /* imul */, X86::IMUL16rmi8, Convert__Reg1_0__Mem165_1__ImmSExti16i81_2, 0, { MCK_GR16, MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 2703 /* imul */, X86::IMUL16rmi, Convert__Reg1_0__Mem165_1__Imm1_2, 0, { MCK_GR16, MCK_Mem16, MCK_Imm }, 1},
  { 2703 /* imul */, X86::IMUL32rri8, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 2703 /* imul */, X86::IMUL32rri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_GR32, MCK_GR32, MCK_Imm }, 1},
  { 2703 /* imul */, X86::IMUL32rmi8, Convert__Reg1_0__Mem325_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 2703 /* imul */, X86::IMUL32rmi, Convert__Reg1_0__Mem325_1__Imm1_2, 0, { MCK_GR32, MCK_Mem32, MCK_Imm }, 1},
  { 2703 /* imul */, X86::IMUL64rri8, Convert__Reg1_0__Reg1_1__ImmSExti64i81_2, 0, { MCK_GR64, MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 2703 /* imul */, X86::IMUL64rri32, Convert__Reg1_0__Reg1_1__ImmSExti64i321_2, 0, { MCK_GR64, MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 2703 /* imul */, X86::IMUL64rmi8, Convert__Reg1_0__Mem645_1__ImmSExti64i81_2, 0, { MCK_GR64, MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 2703 /* imul */, X86::IMUL64rmi32, Convert__Reg1_0__Mem645_1__ImmSExti64i321_2, 0, { MCK_GR64, MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 2708 /* imulb */, X86::IMUL8r, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 2708 /* imulb */, X86::IMUL8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 2714 /* imull */, X86::IMUL32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 2714 /* imull */, X86::IMUL32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rri8, Convert__Reg1_1__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rri8, Convert__Reg1_1__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 1},
  { 2714 /* imull */, X86::IMUL32rri, Convert__Reg1_1__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rri, Convert__Reg1_1__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 1},
  { 2714 /* imull */, X86::IMUL32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rri8, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rmi8, Convert__Reg1_2__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_GR32 }, 0},
  { 2714 /* imull */, X86::IMUL32rmi, Convert__Reg1_2__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32, MCK_GR32 }, 0},
  { 2720 /* imulq */, X86::IMUL64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rri8, Convert__Reg1_1__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rri8, Convert__Reg1_1__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 1},
  { 2720 /* imulq */, X86::IMUL64rri32, Convert__Reg1_1__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rri32, Convert__Reg1_1__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 1},
  { 2720 /* imulq */, X86::IMUL64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rri8, Convert__Reg1_2__Reg1_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rmi8, Convert__Reg1_2__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rri32, Convert__Reg1_2__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64, MCK_GR64 }, 0},
  { 2720 /* imulq */, X86::IMUL64rmi32, Convert__Reg1_2__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64, MCK_GR64 }, 0},
  { 2726 /* imulw */, X86::IMUL16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rri8, Convert__Reg1_1__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rri8, Convert__Reg1_1__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 1},
  { 2726 /* imulw */, X86::IMUL16rri, Convert__Reg1_1__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rri, Convert__Reg1_1__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 1},
  { 2726 /* imulw */, X86::IMUL16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rri8, Convert__Reg1_2__Reg1_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rmi8, Convert__Reg1_2__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16, MCK_GR16 }, 0},
  { 2726 /* imulw */, X86::IMUL16rmi, Convert__Reg1_2__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16, MCK_GR16 }, 0},
  { 2732 /* in */, X86::IN8rr, Convert_NoOperands, 0, { MCK_AL, MCK_DX }, 1},
  { 2732 /* in */, X86::IN8ri, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 2732 /* in */, X86::IN16rr, Convert_NoOperands, 0, { MCK_AX, MCK_DX }, 1},
  { 2732 /* in */, X86::IN16ri, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 2732 /* in */, X86::IN32rr, Convert_NoOperands, 0, { MCK_EAX, MCK_DX }, 1},
  { 2732 /* in */, X86::IN32ri, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 2735 /* inb */, X86::IN8rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 2735 /* inb */, X86::IN8rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 2735 /* inb */, X86::IN8ri, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 2735 /* inb */, X86::IN8ri, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 2735 /* inb */, X86::IN8rr, Convert_NoOperands, 0, { MCK_DX, MCK_AL }, 0},
  { 2735 /* inb */, X86::IN8ri, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 2739 /* inc */, X86::INC8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 2739 /* inc */, X86::INC16r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR16 }, 1},
  { 2739 /* inc */, X86::INC64_16r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR16 }, 1},
  { 2739 /* inc */, X86::INC32r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR32 }, 1},
  { 2739 /* inc */, X86::INC64_32r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 2739 /* inc */, X86::INC64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 2739 /* inc */, X86::INC16m, Convert__Mem165_0, Feature_In32BitMode, { MCK_Mem16 }, 1},
  { 2739 /* inc */, X86::INC64_16m, Convert__Mem165_0, Feature_In64BitMode, { MCK_Mem16 }, 1},
  { 2739 /* inc */, X86::INC32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 1},
  { 2739 /* inc */, X86::INC64_32m, Convert__Mem325_0, Feature_In64BitMode, { MCK_Mem32 }, 1},
  { 2739 /* inc */, X86::INC64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 2739 /* inc */, X86::INC8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 2743 /* incb */, X86::INC8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 2743 /* incb */, X86::INC8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 2748 /* incl */, X86::INC32r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR32 }, 0},
  { 2748 /* incl */, X86::INC64_32r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 2748 /* incl */, X86::INC32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 0},
  { 2748 /* incl */, X86::INC64_32m, Convert__Mem325_0, Feature_In64BitMode, { MCK_Mem32 }, 0},
  { 2753 /* incq */, X86::INC64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 2753 /* incq */, X86::INC64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 2758 /* incw */, X86::INC16r, Convert__Reg1_0__Tie0, Feature_In32BitMode, { MCK_GR16 }, 0},
  { 2758 /* incw */, X86::INC64_16r, Convert__Reg1_0__Tie0, Feature_In64BitMode, { MCK_GR16 }, 0},
  { 2758 /* incw */, X86::INC16m, Convert__Mem165_0, Feature_In32BitMode, { MCK_Mem16 }, 0},
  { 2758 /* incw */, X86::INC64_16m, Convert__Mem165_0, Feature_In64BitMode, { MCK_Mem16 }, 0},
  { 2763 /* inl */, X86::IN32rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 2763 /* inl */, X86::IN32rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 2763 /* inl */, X86::IN32ri, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 2763 /* inl */, X86::IN32ri, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 2763 /* inl */, X86::IN32rr, Convert_NoOperands, 0, { MCK_DX, MCK_EAX }, 0},
  { 2763 /* inl */, X86::IN32ri, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 2767 /* ins */, X86::IN16, Convert_NoOperands, 0, {  }, 1},
  { 2767 /* ins */, X86::IN32, Convert_NoOperands, 0, {  }, 1},
  { 2767 /* ins */, X86::IN8, Convert_NoOperands, 0, {  }, 1},
  { 2771 /* insb */, X86::IN8, Convert_NoOperands, 0, {  }, 0},
  { 2776 /* insertps */, X86::INSERTPSrr, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 2776 /* insertps */, X86::INSERTPSrm, Convert__Reg1_0__Tie0__Mem325_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem32, MCK_ImmZExtu32u8 }, 1},
  { 2776 /* insertps */, X86::INSERTPSrr, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 2776 /* insertps */, X86::INSERTPSrm, Convert__Reg1_2__Tie0__Mem325_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem32, MCK_FR32 }, 0},
  { 2785 /* insertq */, X86::INSERTQ, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 2785 /* insertq */, X86::INSERTQ, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 2785 /* insertq */, X86::INSERTQI, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Imm, MCK_Imm }, 1},
  { 2785 /* insertq */, X86::INSERTQI, Convert__Reg1_3__Tie0__Reg1_2__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 2793 /* insl */, X86::IN32, Convert_NoOperands, 0, {  }, 0},
  { 2798 /* insw */, X86::IN16, Convert_NoOperands, 0, {  }, 0},
  { 2803 /* int */, X86::INT, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 2803 /* int */, X86::INT, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 2807 /* int3 */, X86::INT3, Convert_NoOperands, 0, {  }, 0},
  { 2807 /* int3 */, X86::INT3, Convert_NoOperands, 0, {  }, 1},
  { 2812 /* into */, X86::INTO, Convert_NoOperands, 0, {  }, 0},
  { 2812 /* into */, X86::INTO, Convert_NoOperands, 0, {  }, 1},
  { 2817 /* invd */, X86::INVD, Convert_NoOperands, 0, {  }, 0},
  { 2817 /* invd */, X86::INVD, Convert_NoOperands, 0, {  }, 1},
  { 2822 /* invept */, X86::INVEPT32, Convert__Reg1_0__Mem1285_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem128 }, 1},
  { 2822 /* invept */, X86::INVEPT64, Convert__Reg1_0__Mem1285_1, Feature_In64BitMode, { MCK_GR64, MCK_Mem128 }, 1},
  { 2822 /* invept */, X86::INVEPT32, Convert__Reg1_1__Mem1285_0, Feature_In32BitMode, { MCK_Mem128, MCK_GR32 }, 0},
  { 2822 /* invept */, X86::INVEPT64, Convert__Reg1_1__Mem1285_0, Feature_In64BitMode, { MCK_Mem128, MCK_GR64 }, 0},
  { 2829 /* invlpg */, X86::INVLPG, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 2829 /* invlpg */, X86::INVLPG, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 2836 /* invlpga */, X86::INVLPGA32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX, MCK_ECX }, 1},
  { 2836 /* invlpga */, X86::INVLPGA32, Convert_NoOperands, Feature_In32BitMode, { MCK_ECX, MCK_EAX }, 0},
  { 2836 /* invlpga */, X86::INVLPGA64, Convert_NoOperands, Feature_In64BitMode, { MCK_ECX, MCK_RAX }, 0},
  { 2836 /* invlpga */, X86::INVLPGA64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX, MCK_ECX }, 1},
  { 2844 /* invpcid */, X86::INVPCID32, Convert__Reg1_0__Mem1285_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem128 }, 1},
  { 2844 /* invpcid */, X86::INVPCID64, Convert__Reg1_0__Mem1285_1, Feature_In64BitMode, { MCK_GR64, MCK_Mem128 }, 1},
  { 2844 /* invpcid */, X86::INVPCID32, Convert__Reg1_1__Mem1285_0, Feature_In32BitMode, { MCK_Mem128, MCK_GR32 }, 0},
  { 2844 /* invpcid */, X86::INVPCID64, Convert__Reg1_1__Mem1285_0, Feature_In64BitMode, { MCK_Mem128, MCK_GR64 }, 0},
  { 2852 /* invvpid */, X86::INVVPID32, Convert__Reg1_0__Mem1285_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem128 }, 1},
  { 2852 /* invvpid */, X86::INVVPID64, Convert__Reg1_0__Mem1285_1, Feature_In64BitMode, { MCK_GR64, MCK_Mem128 }, 1},
  { 2852 /* invvpid */, X86::INVVPID32, Convert__Reg1_1__Mem1285_0, Feature_In32BitMode, { MCK_Mem128, MCK_GR32 }, 0},
  { 2852 /* invvpid */, X86::INVVPID64, Convert__Reg1_1__Mem1285_0, Feature_In64BitMode, { MCK_Mem128, MCK_GR64 }, 0},
  { 2860 /* inw */, X86::IN16rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 2860 /* inw */, X86::IN16rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 2860 /* inw */, X86::IN16ri, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 2860 /* inw */, X86::IN16ri, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 2860 /* inw */, X86::IN16rr, Convert_NoOperands, 0, { MCK_DX, MCK_AX }, 0},
  { 2860 /* inw */, X86::IN16ri, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 2864 /* iret */, X86::IRET16, Convert_NoOperands, 0, {  }, 1},
  { 2869 /* iretd */, X86::IRET32, Convert_NoOperands, 0, {  }, 1},
  { 2875 /* iretl */, X86::IRET32, Convert_NoOperands, 0, {  }, 0},
  { 2881 /* iretq */, X86::IRET64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 2881 /* iretq */, X86::IRET64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 2887 /* iretw */, X86::IRET16, Convert_NoOperands, 0, {  }, 0},
  { 2893 /* ja */, X86::JA_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2893 /* ja */, X86::JA_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2893 /* ja */, X86::JA_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2893 /* ja */, X86::JA_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2896 /* jae */, X86::JAE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2896 /* jae */, X86::JAE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2896 /* jae */, X86::JAE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2896 /* jae */, X86::JAE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2900 /* jb */, X86::JB_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2900 /* jb */, X86::JB_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2900 /* jb */, X86::JB_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2900 /* jb */, X86::JB_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2903 /* jbe */, X86::JBE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2903 /* jbe */, X86::JBE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2903 /* jbe */, X86::JBE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2903 /* jbe */, X86::JBE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2907 /* jcxz */, X86::JCXZ, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 0},
  { 2907 /* jcxz */, X86::JCXZ, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 1},
  { 2912 /* je */, X86::JE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2912 /* je */, X86::JE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2912 /* je */, X86::JE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2912 /* je */, X86::JE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2915 /* jecxz */, X86::JECXZ_32, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 0},
  { 2915 /* jecxz */, X86::JECXZ_64, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 0},
  { 2915 /* jecxz */, X86::JECXZ_32, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem }, 1},
  { 2915 /* jecxz */, X86::JECXZ_64, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 1},
  { 2921 /* jg */, X86::JG_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2921 /* jg */, X86::JG_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2921 /* jg */, X86::JG_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2921 /* jg */, X86::JG_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2924 /* jge */, X86::JGE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2924 /* jge */, X86::JGE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2924 /* jge */, X86::JGE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2924 /* jge */, X86::JGE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2928 /* jl */, X86::JL_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2928 /* jl */, X86::JL_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2928 /* jl */, X86::JL_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2928 /* jl */, X86::JL_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2931 /* jle */, X86::JLE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2931 /* jle */, X86::JLE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2931 /* jle */, X86::JLE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2931 /* jle */, X86::JLE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2935 /* jmp */, X86::JMP32r, Convert__Reg1_0, Feature_In32BitMode, { MCK_GR32 }, 1},
  { 2935 /* jmp */, X86::JMP64r, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 2935 /* jmp */, X86::JMP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2935 /* jmp */, X86::JMP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2935 /* jmp */, X86::JMP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2935 /* jmp */, X86::JMP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2935 /* jmp */, X86::JMP32m, Convert__Mem325_0, Feature_In32BitMode, { MCK_Mem32 }, 1},
  { 2935 /* jmp */, X86::JMP64m, Convert__Mem645_0, Feature_In64BitMode, { MCK_Mem64 }, 1},
  { 2935 /* jmp */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 2935 /* jmp */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 2939 /* jmpl */, X86::JMP32r, Convert__Reg1_1, Feature_In32BitMode, { MCK__STAR_, MCK_GR32 }, 0},
  { 2939 /* jmpl */, X86::JMP32m, Convert__Mem325_1, Feature_In32BitMode, { MCK__STAR_, MCK_Mem32 }, 0},
  { 2939 /* jmpl */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 2939 /* jmpl */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 2944 /* jmpq */, X86::JMP64pcrel32, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2944 /* jmpq */, X86::JMP64pcrel32, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2944 /* jmpq */, X86::JMP64r, Convert__Reg1_1, Feature_In64BitMode, { MCK__STAR_, MCK_GR64 }, 0},
  { 2944 /* jmpq */, X86::JMP64m, Convert__Mem645_1, Feature_In64BitMode, { MCK__STAR_, MCK_Mem64 }, 0},
  { 2949 /* jmpw */, X86::FARJMP16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 2949 /* jmpw */, X86::FARJMP16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 2954 /* jne */, X86::JNE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2954 /* jne */, X86::JNE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2954 /* jne */, X86::JNE_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2954 /* jne */, X86::JNE_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2958 /* jno */, X86::JNO_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2958 /* jno */, X86::JNO_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2958 /* jno */, X86::JNO_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2958 /* jno */, X86::JNO_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2962 /* jnp */, X86::JNP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2962 /* jnp */, X86::JNP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2962 /* jnp */, X86::JNP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2962 /* jnp */, X86::JNP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2966 /* jns */, X86::JNS_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2966 /* jns */, X86::JNS_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2966 /* jns */, X86::JNS_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2966 /* jns */, X86::JNS_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2970 /* jo */, X86::JO_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2970 /* jo */, X86::JO_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2970 /* jo */, X86::JO_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2970 /* jo */, X86::JO_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2973 /* jp */, X86::JP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2973 /* jp */, X86::JP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2973 /* jp */, X86::JP_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2973 /* jp */, X86::JP_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2976 /* jrcxz */, X86::JRCXZ, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 0},
  { 2976 /* jrcxz */, X86::JRCXZ, Convert__AbsMem1_0, Feature_In64BitMode, { MCK_AbsMem }, 1},
  { 2982 /* js */, X86::JS_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2982 /* js */, X86::JS_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 2982 /* js */, X86::JS_1, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2982 /* js */, X86::JS_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 2985 /* lahf */, X86::LAHF, Convert_NoOperands, 0, {  }, 0},
  { 2985 /* lahf */, X86::LAHF, Convert_NoOperands, 0, {  }, 1},
  { 2990 /* lar */, X86::LAR16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 2990 /* lar */, X86::LAR16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 2990 /* lar */, X86::LAR32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 2990 /* lar */, X86::LAR32rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR32, MCK_Mem16 }, 1},
  { 2990 /* lar */, X86::LAR64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR32 }, 1},
  { 2990 /* lar */, X86::LAR64rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR64, MCK_Mem16 }, 1},
  { 2994 /* larl */, X86::LAR32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 2994 /* larl */, X86::LAR32rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR32 }, 0},
  { 2999 /* larq */, X86::LAR64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR64 }, 0},
  { 2999 /* larq */, X86::LAR64rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR64 }, 0},
  { 3004 /* larw */, X86::LAR16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 3004 /* larw */, X86::LAR16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 3009 /* lcall */, X86::FARCALL16m, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3009 /* lcall */, X86::FARCALL32m, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3009 /* lcall */, X86::FARCALL64, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3009 /* lcall */, X86::FARCALL32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3009 /* lcall */, X86::FARCALL32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 1},
  { 3009 /* lcall */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3009 /* lcall */, X86::FARCALL16i, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3009 /* lcall */, X86::FARCALL32i, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3009 /* lcall */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3015 /* lcalll */, X86::FARCALL32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3015 /* lcalll */, X86::FARCALL32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3022 /* lcallq */, X86::FARCALL64, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3029 /* lcallw */, X86::FARCALL16m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3029 /* lcallw */, X86::FARCALL16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3036 /* lddqu */, X86::LDDQUrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3036 /* lddqu */, X86::LDDQUrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3042 /* ldmxcsr */, X86::LDMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 3042 /* ldmxcsr */, X86::LDMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 3050 /* lds */, X86::LDS16rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR16, MCK_Mem }, 1},
  { 3050 /* lds */, X86::LDS32rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 3054 /* ldsl */, X86::LDS32rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 3059 /* ldsw */, X86::LDS16rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR16 }, 0},
  { 3064 /* lea */, X86::LEA16r, Convert__Reg1_0__Mem325_1, 0, { MCK_GR16, MCK_Mem32 }, 1},
  { 3064 /* lea */, X86::LEA32r, Convert__Reg1_0__Mem325_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem32 }, 1},
  { 3064 /* lea */, X86::LEA64_32r, Convert__Reg1_0__Mem5_1, Feature_In64BitMode, { MCK_GR32, MCK_Mem }, 1},
  { 3064 /* lea */, X86::LEA64r, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 3068 /* leal */, X86::LEA32r, Convert__Reg1_1__Mem325_0, Feature_In32BitMode, { MCK_Mem32, MCK_GR32 }, 0},
  { 3068 /* leal */, X86::LEA64_32r, Convert__Reg1_1__Mem5_0, Feature_In64BitMode, { MCK_Mem, MCK_GR32 }, 0},
  { 3073 /* leaq */, X86::LEA64r, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 3078 /* leave */, X86::LEAVE, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 3078 /* leave */, X86::LEAVE64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 3078 /* leave */, X86::LEAVE, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 3078 /* leave */, X86::LEAVE64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 3084 /* leaw */, X86::LEA16r, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR16 }, 0},
  { 3089 /* les */, X86::LES16rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR16, MCK_Mem }, 1},
  { 3089 /* les */, X86::LES32rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 3093 /* lesl */, X86::LES32rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 3098 /* lesw */, X86::LES16rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR16 }, 0},
  { 3103 /* lfence */, X86::LFENCE, Convert_NoOperands, 0, {  }, 0},
  { 3103 /* lfence */, X86::LFENCE, Convert_NoOperands, 0, {  }, 1},
  { 3110 /* lfs */, X86::LFS16rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR16, MCK_Mem }, 1},
  { 3110 /* lfs */, X86::LFS32rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 3110 /* lfs */, X86::LFS64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 3114 /* lfsl */, X86::LFS32rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 3119 /* lfsq */, X86::LFS64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 3124 /* lfsw */, X86::LFS16rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR16 }, 0},
  { 3129 /* lgdt */, X86::LGDTm, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 3129 /* lgdt */, X86::LGDTm, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3134 /* lgdtw */, X86::LGDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 0},
  { 3134 /* lgdtw */, X86::LGDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 1},
  { 3140 /* lgs */, X86::LGS16rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR16, MCK_Mem }, 1},
  { 3140 /* lgs */, X86::LGS32rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 3140 /* lgs */, X86::LGS64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 3144 /* lgsl */, X86::LGS32rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 3149 /* lgsq */, X86::LGS64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 3154 /* lgsw */, X86::LGS16rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR16 }, 0},
  { 3159 /* lidt */, X86::LIDTm, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 3159 /* lidt */, X86::LIDTm, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3164 /* lidtw */, X86::LIDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 0},
  { 3164 /* lidtw */, X86::LIDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 1},
  { 3170 /* ljmp */, X86::FARJMP16m, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3170 /* ljmp */, X86::FARJMP32m, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3170 /* ljmp */, X86::FARJMP64, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 3170 /* ljmp */, X86::FARJMP32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3170 /* ljmp */, X86::FARJMP32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 1},
  { 3170 /* ljmp */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3170 /* ljmp */, X86::FARJMP16i, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3170 /* ljmp */, X86::FARJMP32i, Convert__Imm1_0__Imm1_1, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3170 /* ljmp */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 1},
  { 3175 /* ljmpl */, X86::FARJMP32m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3175 /* ljmpl */, X86::FARJMP32i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3181 /* ljmpq */, X86::FARJMP64, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3187 /* ljmpw */, X86::FARJMP16m, Convert__Mem5_1, 0, { MCK__STAR_, MCK_Mem }, 0},
  { 3187 /* ljmpw */, X86::FARJMP16i, Convert__Imm1_1__Imm1_0, 0, { MCK_Imm, MCK_Imm }, 0},
  { 3193 /* lldt */, X86::LLDT16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 3193 /* lldt */, X86::LLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3198 /* lldtw */, X86::LLDT16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 3198 /* lldtw */, X86::LLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3204 /* lmsw */, X86::LMSW16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 3204 /* lmsw */, X86::LMSW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3209 /* lmsww */, X86::LMSW16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 3209 /* lmsww */, X86::LMSW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3215 /* lock */, X86::LOCK_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 3215 /* lock */, X86::LOCK_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 3220 /* lodsb */, X86::LODSB, Convert_NoOperands, 0, {  }, 0},
  { 3220 /* lodsb */, X86::LODSB, Convert_NoOperands, 0, {  }, 1},
  { 3226 /* lodsd */, X86::LODSD, Convert_NoOperands, 0, {  }, 1},
  { 3232 /* lodsl */, X86::LODSD, Convert_NoOperands, 0, {  }, 0},
  { 3238 /* lodsq */, X86::LODSQ, Convert_NoOperands, 0, {  }, 0},
  { 3238 /* lodsq */, X86::LODSQ, Convert_NoOperands, 0, {  }, 1},
  { 3244 /* lodsw */, X86::LODSW, Convert_NoOperands, 0, {  }, 0},
  { 3244 /* lodsw */, X86::LODSW, Convert_NoOperands, 0, {  }, 1},
  { 3250 /* loop */, X86::LOOP, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 3250 /* loop */, X86::LOOP, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 3255 /* loope */, X86::LOOPE, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 3255 /* loope */, X86::LOOPE, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 3261 /* loopne */, X86::LOOPNE, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 3261 /* loopne */, X86::LOOPNE, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 3268 /* lretl */, X86::LRETL, Convert_NoOperands, 0, {  }, 0},
  { 3268 /* lretl */, X86::LRETI, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 3274 /* lretq */, X86::LRETQ, Convert_NoOperands, 0, {  }, 0},
  { 3280 /* lretw */, X86::LRETW, Convert_NoOperands, 0, {  }, 0},
  { 3280 /* lretw */, X86::LRETIW, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 3286 /* lsl */, X86::LSL16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 3286 /* lsl */, X86::LSL16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 3286 /* lsl */, X86::LSL32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 3286 /* lsl */, X86::LSL32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 3286 /* lsl */, X86::LSL64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 3286 /* lsl */, X86::LSL64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 3290 /* lsll */, X86::LSL32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 3290 /* lsll */, X86::LSL32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 3295 /* lslq */, X86::LSL64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 3295 /* lslq */, X86::LSL64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 3300 /* lslw */, X86::LSL16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 3300 /* lslw */, X86::LSL16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 3305 /* lss */, X86::LSS16rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR16, MCK_Mem }, 1},
  { 3305 /* lss */, X86::LSS32rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 3305 /* lss */, X86::LSS64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 3309 /* lssl */, X86::LSS32rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 3314 /* lssq */, X86::LSS64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 3319 /* lssw */, X86::LSS16rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR16 }, 0},
  { 3324 /* ltr */, X86::LTRr, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 3324 /* ltr */, X86::LTRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3328 /* ltrw */, X86::LTRr, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 3328 /* ltrw */, X86::LTRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3333 /* lzcnt */, X86::LZCNT16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 3333 /* lzcnt */, X86::LZCNT16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 3333 /* lzcnt */, X86::LZCNT32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 3333 /* lzcnt */, X86::LZCNT32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 3333 /* lzcnt */, X86::LZCNT64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 3333 /* lzcnt */, X86::LZCNT64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 3339 /* lzcntl */, X86::LZCNT32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 3339 /* lzcntl */, X86::LZCNT32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 3346 /* lzcntq */, X86::LZCNT64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 3346 /* lzcntq */, X86::LZCNT64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 3353 /* lzcntw */, X86::LZCNT16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 3353 /* lzcntw */, X86::LZCNT16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 3360 /* maskmovdqu */, X86::MASKMOVDQU, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3360 /* maskmovdqu */, X86::MASKMOVDQU64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3360 /* maskmovdqu */, X86::MASKMOVDQU, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3360 /* maskmovdqu */, X86::MASKMOVDQU64, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3371 /* maskmovq */, X86::MMX_MASKMOVQ64, Convert__Reg1_1__Reg1_0, Feature_In64BitMode, { MCK_VR64, MCK_VR64 }, 0},
  { 3371 /* maskmovq */, X86::MMX_MASKMOVQ64, Convert__Reg1_0__Reg1_1, Feature_In64BitMode, { MCK_VR64, MCK_VR64 }, 1},
  { 3371 /* maskmovq */, X86::MMX_MASKMOVQ, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 3371 /* maskmovq */, X86::MMX_MASKMOVQ, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 3380 /* maxpd */, X86::MAXPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3380 /* maxpd */, X86::MAXPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3380 /* maxpd */, X86::MAXPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3380 /* maxpd */, X86::MAXPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3386 /* maxps */, X86::MAXPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3386 /* maxps */, X86::MAXPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3386 /* maxps */, X86::MAXPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3386 /* maxps */, X86::MAXPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3392 /* maxsd */, X86::MAXSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3392 /* maxsd */, X86::MAXSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3392 /* maxsd */, X86::MAXSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3392 /* maxsd */, X86::MAXSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3398 /* maxss */, X86::MAXSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3398 /* maxss */, X86::MAXSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3398 /* maxss */, X86::MAXSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3398 /* maxss */, X86::MAXSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3404 /* mfence */, X86::MFENCE, Convert_NoOperands, 0, {  }, 0},
  { 3404 /* mfence */, X86::MFENCE, Convert_NoOperands, 0, {  }, 1},
  { 3411 /* minpd */, X86::MINPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3411 /* minpd */, X86::MINPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3411 /* minpd */, X86::MINPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3411 /* minpd */, X86::MINPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3417 /* minps */, X86::MINPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3417 /* minps */, X86::MINPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3417 /* minps */, X86::MINPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3417 /* minps */, X86::MINPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3423 /* minsd */, X86::MINSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3423 /* minsd */, X86::MINSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3423 /* minsd */, X86::MINSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3423 /* minsd */, X86::MINSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3429 /* minss */, X86::MINSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3429 /* minss */, X86::MINSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3429 /* minss */, X86::MINSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3429 /* minss */, X86::MINSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, 0, {  }, 0},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, 0, {  }, 1},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, Feature_In32BitMode, { MCK__PCT_eax, MCK__PCT_ecx, MCK__PCT_edx }, 1},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, Feature_In64BitMode, { MCK__PCT_rax, MCK__PCT_rcx, MCK__PCT_rdx }, 1},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX, MCK_ECX, MCK_EDX }, 0},
  { 3435 /* monitor */, X86::MONITORrrr, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX, MCK_RCX, MCK_RDX }, 0},
  { 3443 /* montmul */, X86::MONTMUL, Convert_NoOperands, 0, {  }, 0},
  { 3443 /* montmul */, X86::MONTMUL, Convert_NoOperands, 0, {  }, 1},
  { 3451 /* mov */, X86::MOV16o16a, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_AL, MCK_AbsMem }, 1},
  { 3451 /* mov */, X86::MOV8o8a, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_AL, MCK_AbsMem }, 1},
  { 3451 /* mov */, X86::MOV8rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 3451 /* mov */, X86::MOV8ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 3451 /* mov */, X86::MOV8rm, Convert__Reg1_0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 3451 /* mov */, X86::MOV16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 3451 /* mov */, X86::MOV16rs, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV16ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 3451 /* mov */, X86::MOV16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 3451 /* mov */, X86::MOV32o32a, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_EAX, MCK_AbsMem }, 1},
  { 3451 /* mov */, X86::MOV32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 3451 /* mov */, X86::MOV32rs, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV32rd, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_DEBUG_REG }, 1},
  { 3451 /* mov */, X86::MOV32rc, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_CONTROL_REG }, 1},
  { 3451 /* mov */, X86::MOV32ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 3451 /* mov */, X86::MOV32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 3451 /* mov */, X86::MOV64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 3451 /* mov */, X86::MOV64rs, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV64rd, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_DEBUG_REG }, 1},
  { 3451 /* mov */, X86::MOV64rc, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_CONTROL_REG }, 1},
  { 3451 /* mov */, X86::MOV64ri32, Convert__Reg1_0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 3451 /* mov */, X86::MOV64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 3451 /* mov */, X86::MOV16sr, Convert__Reg1_0__Reg1_1, 0, { MCK_SEGMENT_REG, MCK_GR16 }, 1},
  { 3451 /* mov */, X86::MOV32sr, Convert__Reg1_0__Reg1_1, 0, { MCK_SEGMENT_REG, MCK_GR32 }, 1},
  { 3451 /* mov */, X86::MOV64sr, Convert__Reg1_0__Reg1_1, 0, { MCK_SEGMENT_REG, MCK_GR64 }, 1},
  { 3451 /* mov */, X86::MOV16sm, Convert__Reg1_0__Mem165_1, 0, { MCK_SEGMENT_REG, MCK_Mem16 }, 1},
  { 3451 /* mov */, X86::MOV32ms, Convert__Mem325_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_Mem32 }, 0},
  { 3451 /* mov */, X86::MOV32sm, Convert__Reg1_0__Mem325_1, 0, { MCK_SEGMENT_REG, MCK_Mem32 }, 1},
  { 3451 /* mov */, X86::MOV32ms, Convert__Mem325_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_Mem32 }, 1},
  { 3451 /* mov */, X86::MOV64sm, Convert__Reg1_0__Mem645_1, 0, { MCK_SEGMENT_REG, MCK_Mem64 }, 1},
  { 3451 /* mov */, X86::MOV32dr, Convert__Reg1_0__Reg1_1, 0, { MCK_DEBUG_REG, MCK_GR32 }, 1},
  { 3451 /* mov */, X86::MOV64dr, Convert__Reg1_0__Reg1_1, 0, { MCK_DEBUG_REG, MCK_GR64 }, 1},
  { 3451 /* mov */, X86::MOV32cr, Convert__Reg1_0__Reg1_1, 0, { MCK_CONTROL_REG, MCK_GR32 }, 1},
  { 3451 /* mov */, X86::MOV64cr, Convert__Reg1_0__Reg1_1, 0, { MCK_CONTROL_REG, MCK_GR64 }, 1},
  { 3451 /* mov */, X86::MOV16ao16, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_AL }, 1},
  { 3451 /* mov */, X86::MOV8ao8, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_AL }, 1},
  { 3451 /* mov */, X86::MOV32ao32, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_EAX }, 1},
  { 3451 /* mov */, X86::MOV16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 3451 /* mov */, X86::MOV16ms, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 3451 /* mov */, X86::MOV32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 3451 /* mov */, X86::MOV32sm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_SEGMENT_REG }, 0},
  { 3451 /* mov */, X86::MOV32ms, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV32sm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 3451 /* mov */, X86::MOV64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 3451 /* mov */, X86::MOV64ms, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_SEGMENT_REG }, 1},
  { 3451 /* mov */, X86::MOV64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 3451 /* mov */, X86::MOV8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 3451 /* mov */, X86::MOV8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 3455 /* movabs */, X86::MOV64ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 3462 /* movabsq */, X86::MOV64ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 3470 /* movapd */, X86::FsMOVAPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3470 /* movapd */, X86::MOVAPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3470 /* movapd */, X86::FsMOVAPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3470 /* movapd */, X86::MOVAPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3470 /* movapd */, X86::MOVAPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3470 /* movapd */, X86::FsMOVAPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3470 /* movapd */, X86::MOVAPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3470 /* movapd */, X86::FsMOVAPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3470 /* movapd */, X86::MOVAPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3470 /* movapd */, X86::MOVAPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3477 /* movaps */, X86::FsMOVAPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3477 /* movaps */, X86::MOVAPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3477 /* movaps */, X86::FsMOVAPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3477 /* movaps */, X86::MOVAPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3477 /* movaps */, X86::MOVAPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3477 /* movaps */, X86::FsMOVAPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3477 /* movaps */, X86::MOVAPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3477 /* movaps */, X86::FsMOVAPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3477 /* movaps */, X86::MOVAPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3477 /* movaps */, X86::MOVAPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3484 /* movb */, X86::MOV8ao8, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_AL, MCK_AbsMem }, 0},
  { 3484 /* movb */, X86::MOV8rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 3484 /* movb */, X86::MOV8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 3484 /* movb */, X86::MOV8ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 3484 /* movb */, X86::MOV8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 3484 /* movb */, X86::MOV8o8a, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_AL }, 0},
  { 3484 /* movb */, X86::MOV8rm, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 3489 /* movbe */, X86::MOVBE16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 3489 /* movbe */, X86::MOVBE32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 3489 /* movbe */, X86::MOVBE64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 3489 /* movbe */, X86::MOVBE16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 3489 /* movbe */, X86::MOVBE32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 3489 /* movbe */, X86::MOVBE64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 3495 /* movbel */, X86::MOVBE32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 3495 /* movbel */, X86::MOVBE32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 3502 /* movbeq */, X86::MOVBE64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 3502 /* movbeq */, X86::MOVBE64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 3509 /* movbew */, X86::MOVBE16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 3509 /* movbew */, X86::MOVBE16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVZDI2PDIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64grr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_VR64 }, 1},
  { 3516 /* movd */, X86::MOVDI2PDIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVDI2SSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVZDI2PDIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVPDI2DIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 3516 /* movd */, X86::MOVSS2DIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 3516 /* movd */, X86::MMX_MOVD64rrv164, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64to64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64from64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR64 }, 1},
  { 3516 /* movd */, X86::MOV64toPQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOV64toSDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVZQI2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64grr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_GR32 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_GR32 }, 1},
  { 3516 /* movd */, X86::MMX_MOVZDI2PDIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_GR32 }, 1},
  { 3516 /* movd */, X86::MMX_MOVD64from64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_GR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64rrv164, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_GR64 }, 1},
  { 3516 /* movd */, X86::MMX_MOVD64to64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_GR64 }, 1},
  { 3516 /* movd */, X86::MMX_MOVD64mr, Convert__Mem325_1__Reg1_0, 0, { MCK_VR64, MCK_Mem32 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64rm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR64, MCK_Mem32 }, 1},
  { 3516 /* movd */, X86::MMX_MOVZDI2PDIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR64, MCK_Mem32 }, 1},
  { 3516 /* movd */, X86::MOVPDI2DIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 3516 /* movd */, X86::MOVSS2DIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 3516 /* movd */, X86::MOVDI2PDIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 3516 /* movd */, X86::MOVDI2SSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 3516 /* movd */, X86::MOVZDI2PDIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 3516 /* movd */, X86::MOVPQIto64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3516 /* movd */, X86::MOVSDto64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3516 /* movd */, X86::MOVPDI2DImr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 3516 /* movd */, X86::MOVSS2DImr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 3516 /* movd */, X86::MOVDI2PDIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3516 /* movd */, X86::MOVDI2SSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3516 /* movd */, X86::MOVZDI2PDIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3516 /* movd */, X86::MMX_MOVD64rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVZDI2PDIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR64 }, 0},
  { 3516 /* movd */, X86::MMX_MOVD64mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_VR64 }, 1},
  { 3516 /* movd */, X86::MOVDI2PDIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVDI2SSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVZDI2PDIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3516 /* movd */, X86::MOVPDI2DImr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 3516 /* movd */, X86::MOVSS2DImr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 3521 /* movddup */, X86::MOVDDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3521 /* movddup */, X86::MOVDDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3521 /* movddup */, X86::MOVDDUPrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3521 /* movddup */, X86::MOVDDUPrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3529 /* movdq2q */, X86::MMX_MOVDQ2Qrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 3529 /* movdq2q */, X86::MMX_MOVFR642Qrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_FR32 }, 1},
  { 3529 /* movdq2q */, X86::MMX_MOVDQ2Qrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 3529 /* movdq2q */, X86::MMX_MOVFR642Qrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR64 }, 0},
  { 3537 /* movdqa */, X86::MOVDQArr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3537 /* movdqa */, X86::MOVDQArr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3537 /* movdqa */, X86::MOVDQAmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3537 /* movdqa */, X86::MOVDQArm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3537 /* movdqa */, X86::MOVDQArm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3537 /* movdqa */, X86::MOVDQAmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3544 /* movdqu */, X86::MOVDQUrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3544 /* movdqu */, X86::MOVDQUrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3544 /* movdqu */, X86::MOVDQUmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3544 /* movdqu */, X86::MOVDQUrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3544 /* movdqu */, X86::MOVDQUrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3544 /* movdqu */, X86::MOVDQUmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3551 /* movhlps */, X86::MOVHLPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3551 /* movhlps */, X86::MOVHLPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3559 /* movhpd */, X86::MOVHPDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3559 /* movhpd */, X86::MOVHPDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3559 /* movhpd */, X86::MOVHPDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3559 /* movhpd */, X86::MOVHPDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3566 /* movhps */, X86::MOVHPSmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3566 /* movhps */, X86::MOVHPSrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3566 /* movhps */, X86::MOVHPSrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3566 /* movhps */, X86::MOVHPSmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3573 /* movl */, X86::MOV32ao32, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_EAX, MCK_AbsMem }, 0},
  { 3573 /* movl */, X86::MOV32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32sr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_SEGMENT_REG }, 0},
  { 3573 /* movl */, X86::MOV32dr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_DEBUG_REG }, 0},
  { 3573 /* movl */, X86::MOV32cr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_CONTROL_REG }, 0},
  { 3573 /* movl */, X86::MOV32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 3573 /* movl */, X86::MOV32rs, Convert__Reg1_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32ms, Convert__Mem325_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_Mem32 }, 0},
  { 3573 /* movl */, X86::MOV32rd, Convert__Reg1_1__Reg1_0, 0, { MCK_DEBUG_REG, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32rc, Convert__Reg1_1__Reg1_0, 0, { MCK_CONTROL_REG, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 3573 /* movl */, X86::MOV32o32a, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_EAX }, 0},
  { 3573 /* movl */, X86::MOV32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 3573 /* movl */, X86::MOV32sm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_SEGMENT_REG }, 0},
  { 3578 /* movlhps */, X86::MOVLHPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3578 /* movlhps */, X86::MOVLHPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3586 /* movlpd */, X86::MOVLPDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3586 /* movlpd */, X86::MOVLPDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3586 /* movlpd */, X86::MOVLPDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3586 /* movlpd */, X86::MOVLPDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3593 /* movlps */, X86::MOVLPSmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3593 /* movlps */, X86::MOVLPSrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3593 /* movlps */, X86::MOVLPSrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3593 /* movlps */, X86::MOVLPSmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3600 /* movmskpd */, X86::MOVMSKPDrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 3600 /* movmskpd */, X86::MOVMSKPDrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3600 /* movmskpd */, X86::MOVMSKPDrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 3600 /* movmskpd */, X86::MOVMSKPDrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3609 /* movmskps */, X86::MOVMSKPSrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 3609 /* movmskps */, X86::MOVMSKPSrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3609 /* movmskps */, X86::MOVMSKPSrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 3609 /* movmskps */, X86::MOVMSKPSrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3618 /* movntdq */, X86::MOVNTDQmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3618 /* movntdq */, X86::MOVNTDQmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3626 /* movntdqa */, X86::MOVNTDQArm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3626 /* movntdqa */, X86::MOVNTDQArm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3635 /* movnti */, X86::MOVNTImr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 3635 /* movnti */, X86::MOVNTI_64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 3642 /* movntil */, X86::MOVNTImr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 3650 /* movntiq */, X86::MOVNTI_64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 3658 /* movntpd */, X86::MOVNTPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3658 /* movntpd */, X86::MOVNTPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3666 /* movntps */, X86::MOVNTPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3666 /* movntps */, X86::MOVNTPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3674 /* movntq */, X86::MMX_MOVNTQmr, Convert__Mem645_1__Reg1_0, 0, { MCK_VR64, MCK_Mem64 }, 0},
  { 3674 /* movntq */, X86::MMX_MOVNTQmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_VR64 }, 1},
  { 3681 /* movntsd */, X86::MOVNTSD, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3681 /* movntsd */, X86::MOVNTSD, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3689 /* movntss */, X86::MOVNTSS, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 3689 /* movntss */, X86::MOVNTSS, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOV64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVD64to64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_VR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVD64to64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_VR64 }, 1},
  { 3697 /* movq */, X86::MOV64toPQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOV64toSDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::VMOVZQI2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVZQI2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVPQIto64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVSDto64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVPQIto64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVSDto64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOV64sr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_SEGMENT_REG }, 0},
  { 3697 /* movq */, X86::MOV64dr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_DEBUG_REG }, 0},
  { 3697 /* movq */, X86::MOV64cr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_CONTROL_REG }, 0},
  { 3697 /* movq */, X86::MOV64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVD64from64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVD64from64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MMX_MOVQ64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVQ64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 3697 /* movq */, X86::MMX_MOVQ64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_VR64, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVQ64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 3697 /* movq */, X86::MOVPQIto64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOVSDto64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64toPQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOV64toSDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOVZQI2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOV64toPQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOV64toSDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::VMOVZQI2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOVZQI2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOVQxrxr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVZPQILo2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVQxrxr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVZPQILo2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVZPQILo2PQIrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3697 /* movq */, X86::MOVLQ128mr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MOVPQI2QImr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MOVSDto64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MOV64toSDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3697 /* movq */, X86::MOVQI2PQIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3697 /* movq */, X86::MOVZQI2PQIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3697 /* movq */, X86::MOV64rs, Convert__Reg1_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64ms, Convert__Mem645_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MOV64rd, Convert__Reg1_1__Reg1_0, 0, { MCK_DEBUG_REG, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64rc, Convert__Reg1_1__Reg1_0, 0, { MCK_CONTROL_REG, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64ri32, Convert__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 3697 /* movq */, X86::MOV64ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MOV64ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 1},
  { 3697 /* movq */, X86::MOVZPQILo2PQIrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOV64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVQ64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 3697 /* movq */, X86::MMX_MOVQ64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_VR64 }, 1},
  { 3697 /* movq */, X86::MOV64toSDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVQI2PQIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVZQI2PQIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3697 /* movq */, X86::MOVLQ128mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVPQI2QImr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOVSDto64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3697 /* movq */, X86::MOV64sm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_SEGMENT_REG }, 0},
  { 3702 /* movq2dq */, X86::MMX_MOVQ2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_FR32 }, 0},
  { 3702 /* movq2dq */, X86::MMX_MOVQ2FR64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_FR32 }, 0},
  { 3702 /* movq2dq */, X86::MMX_MOVQ2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR64 }, 1},
  { 3702 /* movq2dq */, X86::MMX_MOVQ2FR64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR64 }, 1},
  { 3710 /* movsb */, X86::MOVSB, Convert_NoOperands, 0, {  }, 0},
  { 3710 /* movsb */, X86::MOVSB, Convert_NoOperands, 0, {  }, 1},
  { 3716 /* movsbl */, X86::MOVSX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 0},
  { 3716 /* movsbl */, X86::MOVSX32rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR32 }, 0},
  { 3723 /* movsbq */, X86::MOVSX64rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 0},
  { 3723 /* movsbq */, X86::MOVSX64rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR64 }, 0},
  { 3730 /* movsbw */, X86::MOVSX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 0},
  { 3730 /* movsbw */, X86::MOVSX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 0},
  { 3737 /* movsd */, X86::MOVSD, Convert_NoOperands, 0, {  }, 0},
  { 3737 /* movsd */, X86::MOVSD, Convert_NoOperands, 0, {  }, 1},
  { 3737 /* movsd */, X86::MOVSD, Convert_NoOperands, 0, {  }, 1},
  { 3737 /* movsd */, X86::MOVSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3737 /* movsd */, X86::MOVSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3737 /* movsd */, X86::MOVSDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 3737 /* movsd */, X86::MOVSDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3737 /* movsd */, X86::MOVSDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3737 /* movsd */, X86::MOVSDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 3743 /* movshdup */, X86::MOVSHDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3743 /* movshdup */, X86::MOVSHDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3743 /* movshdup */, X86::MOVSHDUPrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3743 /* movshdup */, X86::MOVSHDUPrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3752 /* movsl */, X86::MOVSD, Convert_NoOperands, 0, {  }, 0},
  { 3758 /* movsldup */, X86::MOVSLDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3758 /* movsldup */, X86::MOVSLDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3758 /* movsldup */, X86::MOVSLDUPrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3758 /* movsldup */, X86::MOVSLDUPrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3767 /* movslq */, X86::MOVSX64rr32, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR64 }, 0},
  { 3767 /* movslq */, X86::MOVSX64rm32, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR64 }, 0},
  { 3774 /* movsq */, X86::MOVSQ, Convert_NoOperands, 0, {  }, 0},
  { 3774 /* movsq */, X86::MOVSQ, Convert_NoOperands, 0, {  }, 1},
  { 3780 /* movss */, X86::MOVSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3780 /* movss */, X86::MOVSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3780 /* movss */, X86::MOVSSmr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 3780 /* movss */, X86::MOVSSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3780 /* movss */, X86::MOVSSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3780 /* movss */, X86::MOVSSmr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 3786 /* movsw */, X86::MOVSW, Convert_NoOperands, 0, {  }, 0},
  { 3786 /* movsw */, X86::MOVSW, Convert_NoOperands, 0, {  }, 1},
  { 3792 /* movswl */, X86::MOVSX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 0},
  { 3792 /* movswl */, X86::MOVSX32rm16, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR32 }, 0},
  { 3799 /* movswq */, X86::MOVSX64rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 0},
  { 3799 /* movswq */, X86::MOVSX64rm16, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR64 }, 0},
  { 3806 /* movsx */, X86::MOVSX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 0},
  { 3806 /* movsx */, X86::MOVSX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 0},
  { 3806 /* movsx */, X86::MOVSX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 0},
  { 3806 /* movsx */, X86::MOVSX64rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 1},
  { 3806 /* movsx */, X86::MOVSX16rr8, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR8 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 0},
  { 3806 /* movsx */, X86::MOVSX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 0},
  { 3806 /* movsx */, X86::MOVSX64rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 1},
  { 3806 /* movsx */, X86::MOVSX16rm8, Convert__Reg1_0__Mem85_1, 0, { MCK_GR16, MCK_Mem8 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rr8, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR8 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rr16, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR16 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rr32, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR64 }, 0},
  { 3806 /* movsx */, X86::MOVSX64rr32, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR64 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rm16, Convert__Reg1_0__Mem165_1, 0, { MCK_GR32, MCK_Mem16 }, 1},
  { 3806 /* movsx */, X86::MOVSX32rm8, Convert__Reg1_0__Mem85_1, 0, { MCK_GR32, MCK_Mem8 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rr8, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR8 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rr16, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR16 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rm16, Convert__Reg1_0__Mem165_1, 0, { MCK_GR64, MCK_Mem16 }, 1},
  { 3806 /* movsx */, X86::MOVSX64rm8, Convert__Reg1_0__Mem85_1, 0, { MCK_GR64, MCK_Mem8 }, 1},
  { 3806 /* movsx */, X86::MOVSX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 0},
  { 3806 /* movsx */, X86::MOVSX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 1},
  { 3812 /* movsxd */, X86::MOVSX64rr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR32 }, 1},
  { 3812 /* movsxd */, X86::MOVSX64rm32, Convert__Reg1_0__Mem325_1, 0, { MCK_GR64, MCK_Mem32 }, 1},
  { 3819 /* movupd */, X86::MOVUPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3819 /* movupd */, X86::MOVUPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3819 /* movupd */, X86::MOVUPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3819 /* movupd */, X86::MOVUPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3819 /* movupd */, X86::MOVUPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3819 /* movupd */, X86::MOVUPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3826 /* movups */, X86::MOVUPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3826 /* movups */, X86::MOVUPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3826 /* movups */, X86::MOVUPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 3826 /* movups */, X86::MOVUPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3826 /* movups */, X86::MOVUPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3826 /* movups */, X86::MOVUPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 3833 /* movw */, X86::MOV16ao16, Convert__AbsMem1_1, Feature_In32BitMode, { MCK_AX, MCK_AbsMem }, 0},
  { 3833 /* movw */, X86::MOV16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 3833 /* movw */, X86::MOV16sr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_SEGMENT_REG }, 0},
  { 3833 /* movw */, X86::MOV16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 3833 /* movw */, X86::MOV16rs, Convert__Reg1_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_GR16 }, 0},
  { 3833 /* movw */, X86::MOV16ms, Convert__Mem165_1__Reg1_0, 0, { MCK_SEGMENT_REG, MCK_Mem16 }, 0},
  { 3833 /* movw */, X86::MOV16ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 3833 /* movw */, X86::MOV16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 3833 /* movw */, X86::MOV16o16a, Convert__AbsMem1_0, Feature_In32BitMode, { MCK_AbsMem, MCK_AX }, 0},
  { 3833 /* movw */, X86::MOV16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 3833 /* movw */, X86::MOV16sm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_SEGMENT_REG }, 0},
  { 3838 /* movzbl */, X86::MOVZX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 0},
  { 3838 /* movzbl */, X86::MOVZX32rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR32 }, 0},
  { 3845 /* movzbq */, X86::MOVZX64rr8_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 0},
  { 3845 /* movzbq */, X86::MOVZX64rm8_Q, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR64 }, 0},
  { 3852 /* movzbw */, X86::MOVZX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 0},
  { 3852 /* movzbw */, X86::MOVZX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 0},
  { 3859 /* movzwl */, X86::MOVZX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 0},
  { 3859 /* movzwl */, X86::MOVZX32rm16, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR32 }, 0},
  { 3866 /* movzwq */, X86::MOVZX64rr16_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 0},
  { 3866 /* movzwq */, X86::MOVZX64rm16_Q, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR64 }, 0},
  { 3873 /* movzx */, X86::MOVZX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 0},
  { 3873 /* movzx */, X86::MOVZX16rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR16 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 0},
  { 3873 /* movzx */, X86::MOVZX32rr8, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR32 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rr8_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 0},
  { 3873 /* movzx */, X86::MOVZX64rr8_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR64 }, 1},
  { 3873 /* movzx */, X86::MOVZX16rr8, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR8 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 0},
  { 3873 /* movzx */, X86::MOVZX32rr16, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR32 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rr16_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 0},
  { 3873 /* movzx */, X86::MOVZX64rr16_Q, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR64 }, 1},
  { 3873 /* movzx */, X86::MOVZX16rm8, Convert__Reg1_0__Mem85_1, 0, { MCK_GR16, MCK_Mem8 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rr8, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR8 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rr16, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR16 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rm16, Convert__Reg1_0__Mem165_1, 0, { MCK_GR32, MCK_Mem16 }, 1},
  { 3873 /* movzx */, X86::MOVZX32rm8, Convert__Reg1_0__Mem85_1, 0, { MCK_GR32, MCK_Mem8 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rr8_Q, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR8 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rr16_Q, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR16 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rm16_Q, Convert__Reg1_0__Mem165_1, 0, { MCK_GR64, MCK_Mem16 }, 1},
  { 3873 /* movzx */, X86::MOVZX64rm8_Q, Convert__Reg1_0__Mem85_1, 0, { MCK_GR64, MCK_Mem8 }, 1},
  { 3873 /* movzx */, X86::MOVZX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 0},
  { 3873 /* movzx */, X86::MOVZX16rm8, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR16 }, 1},
  { 3879 /* mpsadbw */, X86::MPSADBWrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 3879 /* mpsadbw */, X86::MPSADBWrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 3879 /* mpsadbw */, X86::MPSADBWrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 3879 /* mpsadbw */, X86::MPSADBWrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 3887 /* mul */, X86::MUL8r, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 3887 /* mul */, X86::MUL16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 3887 /* mul */, X86::MUL32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 3887 /* mul */, X86::MUL64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 3887 /* mul */, X86::MUL16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3887 /* mul */, X86::MUL32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 3887 /* mul */, X86::MUL64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 3887 /* mul */, X86::MUL8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 3891 /* mulb */, X86::MUL8r, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 3891 /* mulb */, X86::MUL8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 3896 /* mull */, X86::MUL32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 3896 /* mull */, X86::MUL32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 3901 /* mulpd */, X86::MULPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3901 /* mulpd */, X86::MULPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3901 /* mulpd */, X86::MULPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3901 /* mulpd */, X86::MULPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3907 /* mulps */, X86::MULPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3907 /* mulps */, X86::MULPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3907 /* mulps */, X86::MULPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 3907 /* mulps */, X86::MULPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 3913 /* mulq */, X86::MUL64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 3913 /* mulq */, X86::MUL64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 3918 /* mulsd */, X86::MULSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3918 /* mulsd */, X86::MULSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3918 /* mulsd */, X86::MULSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 3918 /* mulsd */, X86::MULSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 3924 /* mulss */, X86::MULSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 3924 /* mulss */, X86::MULSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 3924 /* mulss */, X86::MULSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 3924 /* mulss */, X86::MULSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 3930 /* mulw */, X86::MUL16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 3930 /* mulw */, X86::MUL16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3935 /* mulx */, X86::MULX32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 3935 /* mulx */, X86::MULX32rm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_GR32, MCK_GR32, MCK_Mem32 }, 1},
  { 3935 /* mulx */, X86::MULX64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 3935 /* mulx */, X86::MULX64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_GR64, MCK_GR64, MCK_Mem64 }, 1},
  { 3940 /* mulxl */, X86::MULX32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 3940 /* mulxl */, X86::MULX32rm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32, MCK_GR32 }, 0},
  { 3946 /* mulxq */, X86::MULX64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 3946 /* mulxq */, X86::MULX64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64, MCK_GR64 }, 0},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, 0, {  }, 0},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, 0, {  }, 1},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, Feature_In32BitMode, { MCK__PCT_eax, MCK__PCT_ecx }, 1},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, Feature_In64BitMode, { MCK__PCT_rax, MCK__PCT_rcx }, 1},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX, MCK_ECX }, 0},
  { 3952 /* mwait */, X86::MWAITrr, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX, MCK_RCX }, 0},
  { 3958 /* neg */, X86::NEG8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 3958 /* neg */, X86::NEG16r, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 3958 /* neg */, X86::NEG32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 3958 /* neg */, X86::NEG64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 3958 /* neg */, X86::NEG16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3958 /* neg */, X86::NEG32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 3958 /* neg */, X86::NEG64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 3958 /* neg */, X86::NEG8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 3962 /* negb */, X86::NEG8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 3962 /* negb */, X86::NEG8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 3967 /* negl */, X86::NEG32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 3967 /* negl */, X86::NEG32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 3972 /* negq */, X86::NEG64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 3972 /* negq */, X86::NEG64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 3977 /* negw */, X86::NEG16r, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 3977 /* negw */, X86::NEG16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3982 /* nop */, X86::NOOP, Convert_NoOperands, 0, {  }, 0},
  { 3982 /* nop */, X86::NOOP, Convert_NoOperands, 0, {  }, 1},
  { 3982 /* nop */, X86::NOOPW, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3982 /* nop */, X86::NOOPL, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 3986 /* nopl */, X86::NOOPL, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 3991 /* nopw */, X86::NOOPW, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 3996 /* not */, X86::NOT8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 3996 /* not */, X86::NOT16r, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 3996 /* not */, X86::NOT32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 3996 /* not */, X86::NOT64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 3996 /* not */, X86::NOT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 3996 /* not */, X86::NOT32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 3996 /* not */, X86::NOT64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 3996 /* not */, X86::NOT8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 4000 /* notb */, X86::NOT8r, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 4000 /* notb */, X86::NOT8m, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 4005 /* notl */, X86::NOT32r, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 4005 /* notl */, X86::NOT32m, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 4010 /* notq */, X86::NOT64r, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 4010 /* notq */, X86::NOT64m, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 4015 /* notw */, X86::NOT16r, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 4015 /* notw */, X86::NOT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 4020 /* or */, X86::OR8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 4020 /* or */, X86::OR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 4020 /* or */, X86::OR16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 4020 /* or */, X86::OR16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 4020 /* or */, X86::OR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 4020 /* or */, X86::OR32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 4020 /* or */, X86::OR32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 4020 /* or */, X86::OR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 4020 /* or */, X86::OR64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 4020 /* or */, X86::OR64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 4020 /* or */, X86::OR64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 4020 /* or */, X86::OR64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 4020 /* or */, X86::OR64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 4020 /* or */, X86::OR16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 4020 /* or */, X86::OR16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 4020 /* or */, X86::OR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 4020 /* or */, X86::OR32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 4020 /* or */, X86::OR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 4020 /* or */, X86::OR64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 4020 /* or */, X86::OR64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 4020 /* or */, X86::OR64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 4020 /* or */, X86::OR8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 4020 /* or */, X86::OR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 4023 /* orb */, X86::OR8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 4023 /* orb */, X86::OR8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 4023 /* orb */, X86::OR8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 4023 /* orb */, X86::OR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 4023 /* orb */, X86::OR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 4023 /* orb */, X86::OR8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 4027 /* orl */, X86::OR32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 4027 /* orl */, X86::OR32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 4027 /* orl */, X86::OR32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 4027 /* orl */, X86::OR32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 4027 /* orl */, X86::OR32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 4027 /* orl */, X86::OR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 4027 /* orl */, X86::OR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 4027 /* orl */, X86::OR32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 4031 /* orpd */, X86::FsORPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4031 /* orpd */, X86::ORPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4031 /* orpd */, X86::FsORPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4031 /* orpd */, X86::ORPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4031 /* orpd */, X86::FsORPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4031 /* orpd */, X86::ORPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4031 /* orpd */, X86::FsORPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4031 /* orpd */, X86::ORPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4036 /* orps */, X86::FsORPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4036 /* orps */, X86::ORPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4036 /* orps */, X86::FsORPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4036 /* orps */, X86::ORPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4036 /* orps */, X86::FsORPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4036 /* orps */, X86::ORPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4036 /* orps */, X86::FsORPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4036 /* orps */, X86::ORPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4041 /* orq */, X86::OR64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 4041 /* orq */, X86::OR64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 4041 /* orq */, X86::OR64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 4041 /* orq */, X86::OR64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 4041 /* orq */, X86::OR64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 4041 /* orq */, X86::OR64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 4041 /* orq */, X86::OR64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 4041 /* orq */, X86::OR64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 4045 /* orw */, X86::OR16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 4045 /* orw */, X86::OR16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 4045 /* orw */, X86::OR16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 4045 /* orw */, X86::OR16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 4045 /* orw */, X86::OR16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 4045 /* orw */, X86::OR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 4045 /* orw */, X86::OR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 4045 /* orw */, X86::OR16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 4049 /* out */, X86::OUT8rr, Convert_NoOperands, 0, { MCK_DX, MCK_AL }, 1},
  { 4049 /* out */, X86::OUT16rr, Convert_NoOperands, 0, { MCK_DX, MCK_AX }, 1},
  { 4049 /* out */, X86::OUT32rr, Convert_NoOperands, 0, { MCK_DX, MCK_EAX }, 1},
  { 4049 /* out */, X86::OUT8ir, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 1},
  { 4049 /* out */, X86::OUT16ir, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 1},
  { 4049 /* out */, X86::OUT32ir, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 1},
  { 4053 /* outb */, X86::OUT8rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 4053 /* outb */, X86::OUT8rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 4053 /* outb */, X86::OUT8ir, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 4053 /* outb */, X86::OUT8ir, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 4053 /* outb */, X86::OUT8rr, Convert_NoOperands, 0, { MCK_AL, MCK_DX }, 0},
  { 4053 /* outb */, X86::OUT8ir, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 0},
  { 4058 /* outl */, X86::OUT32rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 4058 /* outl */, X86::OUT32rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 4058 /* outl */, X86::OUT32ir, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 4058 /* outl */, X86::OUT32ir, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 4058 /* outl */, X86::OUT32rr, Convert_NoOperands, 0, { MCK_EAX, MCK_DX }, 0},
  { 4058 /* outl */, X86::OUT32ir, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 0},
  { 4063 /* outsb */, X86::OUTSB, Convert_NoOperands, 0, {  }, 0},
  { 4063 /* outsb */, X86::OUTSB, Convert_NoOperands, 0, {  }, 1},
  { 4069 /* outsd */, X86::OUTSD, Convert_NoOperands, 0, {  }, 1},
  { 4075 /* outsl */, X86::OUTSD, Convert_NoOperands, 0, {  }, 0},
  { 4081 /* outsw */, X86::OUTSW, Convert_NoOperands, 0, {  }, 0},
  { 4081 /* outsw */, X86::OUTSW, Convert_NoOperands, 0, {  }, 1},
  { 4087 /* outw */, X86::OUT16rr, Convert_NoOperands, 0, { MCK__PCT_dx }, 1},
  { 4087 /* outw */, X86::OUT16rr, Convert_NoOperands, 0, { MCK_DX }, 0},
  { 4087 /* outw */, X86::OUT16ir, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 4087 /* outw */, X86::OUT16ir, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 4087 /* outw */, X86::OUT16rr, Convert_NoOperands, 0, { MCK_AX, MCK_DX }, 0},
  { 4087 /* outw */, X86::OUT16ir, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 0},
  { 4092 /* pabsb */, X86::MMX_PABSBrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4092 /* pabsb */, X86::MMX_PABSBrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4092 /* pabsb */, X86::MMX_PABSBrm64, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4092 /* pabsb */, X86::PABSBrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4092 /* pabsb */, X86::PABSBrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4092 /* pabsb */, X86::PABSBrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4092 /* pabsb */, X86::PABSBrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4092 /* pabsb */, X86::MMX_PABSBrm64, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4098 /* pabsd */, X86::MMX_PABSDrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4098 /* pabsd */, X86::MMX_PABSDrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4098 /* pabsd */, X86::MMX_PABSDrm64, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4098 /* pabsd */, X86::PABSDrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4098 /* pabsd */, X86::PABSDrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4098 /* pabsd */, X86::PABSDrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4098 /* pabsd */, X86::PABSDrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4098 /* pabsd */, X86::MMX_PABSDrm64, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4104 /* pabsw */, X86::MMX_PABSWrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4104 /* pabsw */, X86::MMX_PABSWrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4104 /* pabsw */, X86::MMX_PABSWrm64, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4104 /* pabsw */, X86::PABSWrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4104 /* pabsw */, X86::PABSWrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4104 /* pabsw */, X86::PABSWrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4104 /* pabsw */, X86::PABSWrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4104 /* pabsw */, X86::MMX_PABSWrm64, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4110 /* packssdw */, X86::MMX_PACKSSDWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4110 /* packssdw */, X86::MMX_PACKSSDWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4110 /* packssdw */, X86::MMX_PACKSSDWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4110 /* packssdw */, X86::PACKSSDWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4110 /* packssdw */, X86::PACKSSDWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4110 /* packssdw */, X86::PACKSSDWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4110 /* packssdw */, X86::PACKSSDWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4110 /* packssdw */, X86::MMX_PACKSSDWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4119 /* packsswb */, X86::MMX_PACKSSWBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4119 /* packsswb */, X86::MMX_PACKSSWBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4119 /* packsswb */, X86::MMX_PACKSSWBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4119 /* packsswb */, X86::PACKSSWBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4119 /* packsswb */, X86::PACKSSWBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4119 /* packsswb */, X86::PACKSSWBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4119 /* packsswb */, X86::PACKSSWBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4119 /* packsswb */, X86::MMX_PACKSSWBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4128 /* packusdw */, X86::PACKUSDWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4128 /* packusdw */, X86::PACKUSDWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4128 /* packusdw */, X86::PACKUSDWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4128 /* packusdw */, X86::PACKUSDWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4137 /* packuswb */, X86::MMX_PACKUSWBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4137 /* packuswb */, X86::MMX_PACKUSWBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4137 /* packuswb */, X86::MMX_PACKUSWBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4137 /* packuswb */, X86::PACKUSWBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4137 /* packuswb */, X86::PACKUSWBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4137 /* packuswb */, X86::PACKUSWBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4137 /* packuswb */, X86::PACKUSWBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4137 /* packuswb */, X86::MMX_PACKUSWBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4146 /* paddb */, X86::MMX_PADDBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4146 /* paddb */, X86::MMX_PADDBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4146 /* paddb */, X86::MMX_PADDBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4146 /* paddb */, X86::PADDBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4146 /* paddb */, X86::PADDBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4146 /* paddb */, X86::PADDBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4146 /* paddb */, X86::PADDBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4146 /* paddb */, X86::MMX_PADDBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4152 /* paddd */, X86::MMX_PADDDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4152 /* paddd */, X86::MMX_PADDDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4152 /* paddd */, X86::MMX_PADDDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4152 /* paddd */, X86::PADDDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4152 /* paddd */, X86::PADDDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4152 /* paddd */, X86::PADDDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4152 /* paddd */, X86::PADDDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4152 /* paddd */, X86::MMX_PADDDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4158 /* paddq */, X86::MMX_PADDQirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4158 /* paddq */, X86::MMX_PADDQirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4158 /* paddq */, X86::MMX_PADDQirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4158 /* paddq */, X86::PADDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4158 /* paddq */, X86::PADDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4158 /* paddq */, X86::PADDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4158 /* paddq */, X86::PADDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4158 /* paddq */, X86::MMX_PADDQirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4164 /* paddsb */, X86::MMX_PADDSBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4164 /* paddsb */, X86::MMX_PADDSBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4164 /* paddsb */, X86::MMX_PADDSBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4164 /* paddsb */, X86::PADDSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4164 /* paddsb */, X86::PADDSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4164 /* paddsb */, X86::PADDSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4164 /* paddsb */, X86::PADDSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4164 /* paddsb */, X86::MMX_PADDSBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4171 /* paddsw */, X86::MMX_PADDSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4171 /* paddsw */, X86::MMX_PADDSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4171 /* paddsw */, X86::MMX_PADDSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4171 /* paddsw */, X86::PADDSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4171 /* paddsw */, X86::PADDSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4171 /* paddsw */, X86::PADDSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4171 /* paddsw */, X86::PADDSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4171 /* paddsw */, X86::MMX_PADDSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4178 /* paddusb */, X86::MMX_PADDUSBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4178 /* paddusb */, X86::MMX_PADDUSBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4178 /* paddusb */, X86::MMX_PADDUSBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4178 /* paddusb */, X86::PADDUSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4178 /* paddusb */, X86::PADDUSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4178 /* paddusb */, X86::PADDUSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4178 /* paddusb */, X86::PADDUSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4178 /* paddusb */, X86::MMX_PADDUSBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4186 /* paddusw */, X86::MMX_PADDUSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4186 /* paddusw */, X86::MMX_PADDUSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4186 /* paddusw */, X86::MMX_PADDUSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4186 /* paddusw */, X86::PADDUSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4186 /* paddusw */, X86::PADDUSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4186 /* paddusw */, X86::PADDUSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4186 /* paddusw */, X86::PADDUSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4186 /* paddusw */, X86::MMX_PADDUSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4194 /* paddw */, X86::MMX_PADDWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4194 /* paddw */, X86::MMX_PADDWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4194 /* paddw */, X86::MMX_PADDWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4194 /* paddw */, X86::PADDWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4194 /* paddw */, X86::PADDWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4194 /* paddw */, X86::PADDWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4194 /* paddw */, X86::PADDWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4194 /* paddw */, X86::MMX_PADDWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4200 /* palignr */, X86::MMX_PALIGNR64irr, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_VR64, MCK_VR64, MCK_Imm }, 1},
  { 4200 /* palignr */, X86::MMX_PALIGNR64irm, Convert__Reg1_0__Tie0__Mem645_1__Imm1_2, 0, { MCK_VR64, MCK_Mem64, MCK_Imm }, 1},
  { 4200 /* palignr */, X86::PALIGNR128rr, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4200 /* palignr */, X86::PALIGNR128rm, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4200 /* palignr */, X86::MMX_PALIGNR64irr, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR64, MCK_VR64 }, 0},
  { 4200 /* palignr */, X86::PALIGNR128rr, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4200 /* palignr */, X86::PALIGNR128rm, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4200 /* palignr */, X86::MMX_PALIGNR64irm, Convert__Reg1_2__Tie0__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64, MCK_VR64 }, 0},
  { 4208 /* pand */, X86::MMX_PANDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4208 /* pand */, X86::MMX_PANDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4208 /* pand */, X86::MMX_PANDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4208 /* pand */, X86::PANDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4208 /* pand */, X86::PANDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4208 /* pand */, X86::PANDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4208 /* pand */, X86::PANDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4208 /* pand */, X86::MMX_PANDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4213 /* pandn */, X86::MMX_PANDNirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4213 /* pandn */, X86::MMX_PANDNirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4213 /* pandn */, X86::MMX_PANDNirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4213 /* pandn */, X86::PANDNrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4213 /* pandn */, X86::PANDNrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4213 /* pandn */, X86::PANDNrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4213 /* pandn */, X86::PANDNrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4213 /* pandn */, X86::MMX_PANDNirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4219 /* pause */, X86::PAUSE, Convert_NoOperands, 0, {  }, 0},
  { 4219 /* pause */, X86::PAUSE, Convert_NoOperands, 0, {  }, 1},
  { 4225 /* pavgb */, X86::MMX_PAVGBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4225 /* pavgb */, X86::MMX_PAVGBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4225 /* pavgb */, X86::MMX_PAVGBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4225 /* pavgb */, X86::PAVGBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4225 /* pavgb */, X86::PAVGBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4225 /* pavgb */, X86::PAVGBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4225 /* pavgb */, X86::PAVGBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4225 /* pavgb */, X86::MMX_PAVGBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4231 /* pavgusb */, X86::PAVGUSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4231 /* pavgusb */, X86::PAVGUSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4231 /* pavgusb */, X86::PAVGUSBrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4231 /* pavgusb */, X86::PAVGUSBrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4239 /* pavgw */, X86::MMX_PAVGWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4239 /* pavgw */, X86::MMX_PAVGWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4239 /* pavgw */, X86::MMX_PAVGWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4239 /* pavgw */, X86::PAVGWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4239 /* pavgw */, X86::PAVGWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4239 /* pavgw */, X86::PAVGWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4239 /* pavgw */, X86::PAVGWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4239 /* pavgw */, X86::MMX_PAVGWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4245 /* pblendvb */, X86::PBLENDVBrr0, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4245 /* pblendvb */, X86::PBLENDVBrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4245 /* pblendvb */, X86::PBLENDVBrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4245 /* pblendvb */, X86::PBLENDVBrm0, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4245 /* pblendvb */, X86::PBLENDVBrr0, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_XMM0, MCK_FR32, MCK_FR32 }, 0},
  { 4245 /* pblendvb */, X86::PBLENDVBrm0, Convert__Reg1_2__Tie0__Mem1285_1, 0, { MCK_XMM0, MCK_Mem128, MCK_FR32 }, 0},
  { 4245 /* pblendvb */, X86::PBLENDVBrr0, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32, MCK__PCT_xmm0 }, 1},
  { 4245 /* pblendvb */, X86::PBLENDVBrm0, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128, MCK__PCT_xmm0 }, 1},
  { 4254 /* pblendw */, X86::PBLENDWrri, Convert__Reg1_0__Tie0__Reg1_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 4254 /* pblendw */, X86::PBLENDWrmi, Convert__Reg1_0__Tie0__Mem1285_1__ImmZExtu32u81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 4254 /* pblendw */, X86::PBLENDWrri, Convert__Reg1_2__Tie0__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32 }, 0},
  { 4254 /* pblendw */, X86::PBLENDWrmi, Convert__Reg1_2__Tie0__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32 }, 0},
  { 4262 /* pclmulhqhqdq */, X86::PCLMULQDQrr, Convert__Reg1_1__Tie0__Reg1_0__imm_17, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4262 /* pclmulhqhqdq */, X86::PCLMULQDQrr, Convert__Reg1_0__Tie0__Reg1_1__imm_17, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4262 /* pclmulhqhqdq */, X86::PCLMULQDQrm, Convert__Reg1_0__Tie0__Mem1285_1__imm_17, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4262 /* pclmulhqhqdq */, X86::PCLMULQDQrm, Convert__Reg1_1__Tie0__Mem1285_0__imm_17, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4275 /* pclmulhqlqdq */, X86::PCLMULQDQrr, Convert__Reg1_1__Tie0__Reg1_0__imm_1, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4275 /* pclmulhqlqdq */, X86::PCLMULQDQrr, Convert__Reg1_0__Tie0__Reg1_1__imm_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4275 /* pclmulhqlqdq */, X86::PCLMULQDQrm, Convert__Reg1_0__Tie0__Mem1285_1__imm_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4275 /* pclmulhqlqdq */, X86::PCLMULQDQrm, Convert__Reg1_1__Tie0__Mem1285_0__imm_1, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4288 /* pclmullqhqdq */, X86::PCLMULQDQrr, Convert__Reg1_1__Tie0__Reg1_0__imm_16, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4288 /* pclmullqhqdq */, X86::PCLMULQDQrr, Convert__Reg1_0__Tie0__Reg1_1__imm_16, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4288 /* pclmullqhqdq */, X86::PCLMULQDQrm, Convert__Reg1_0__Tie0__Mem1285_1__imm_16, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4288 /* pclmullqhqdq */, X86::PCLMULQDQrm, Convert__Reg1_1__Tie0__Mem1285_0__imm_16, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4301 /* pclmullqlqdq */, X86::PCLMULQDQrr, Convert__Reg1_1__Tie0__Reg1_0__imm_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4301 /* pclmullqlqdq */, X86::PCLMULQDQrr, Convert__Reg1_0__Tie0__Reg1_1__imm_0, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4301 /* pclmullqlqdq */, X86::PCLMULQDQrm, Convert__Reg1_0__Tie0__Mem1285_1__imm_0, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4301 /* pclmullqlqdq */, X86::PCLMULQDQrm, Convert__Reg1_1__Tie0__Mem1285_0__imm_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4314 /* pclmulqdq */, X86::PCLMULQDQrr, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4314 /* pclmulqdq */, X86::PCLMULQDQrm, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4314 /* pclmulqdq */, X86::PCLMULQDQrr, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4314 /* pclmulqdq */, X86::PCLMULQDQrm, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4324 /* pcmpeqb */, X86::MMX_PCMPEQBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4324 /* pcmpeqb */, X86::MMX_PCMPEQBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4324 /* pcmpeqb */, X86::MMX_PCMPEQBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4324 /* pcmpeqb */, X86::PCMPEQBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4324 /* pcmpeqb */, X86::PCMPEQBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4324 /* pcmpeqb */, X86::PCMPEQBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4324 /* pcmpeqb */, X86::PCMPEQBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4324 /* pcmpeqb */, X86::MMX_PCMPEQBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4332 /* pcmpeqd */, X86::MMX_PCMPEQDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4332 /* pcmpeqd */, X86::MMX_PCMPEQDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4332 /* pcmpeqd */, X86::MMX_PCMPEQDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4332 /* pcmpeqd */, X86::PCMPEQDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4332 /* pcmpeqd */, X86::PCMPEQDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4332 /* pcmpeqd */, X86::PCMPEQDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4332 /* pcmpeqd */, X86::PCMPEQDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4332 /* pcmpeqd */, X86::MMX_PCMPEQDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4340 /* pcmpeqq */, X86::PCMPEQQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4340 /* pcmpeqq */, X86::PCMPEQQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4340 /* pcmpeqq */, X86::PCMPEQQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4340 /* pcmpeqq */, X86::PCMPEQQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4348 /* pcmpeqw */, X86::MMX_PCMPEQWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4348 /* pcmpeqw */, X86::MMX_PCMPEQWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4348 /* pcmpeqw */, X86::MMX_PCMPEQWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4348 /* pcmpeqw */, X86::PCMPEQWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4348 /* pcmpeqw */, X86::PCMPEQWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4348 /* pcmpeqw */, X86::PCMPEQWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4348 /* pcmpeqw */, X86::PCMPEQWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4348 /* pcmpeqw */, X86::MMX_PCMPEQWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4356 /* pcmpestri */, X86::PCMPESTRIrr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4356 /* pcmpestri */, X86::PCMPESTRIrm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4356 /* pcmpestri */, X86::PCMPESTRIrr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4356 /* pcmpestri */, X86::PCMPESTRIrm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4366 /* pcmpestrm */, X86::PCMPESTRM128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4366 /* pcmpestrm */, X86::PCMPESTRM128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4366 /* pcmpestrm */, X86::PCMPESTRM128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4366 /* pcmpestrm */, X86::PCMPESTRM128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4376 /* pcmpgtb */, X86::MMX_PCMPGTBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4376 /* pcmpgtb */, X86::MMX_PCMPGTBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4376 /* pcmpgtb */, X86::MMX_PCMPGTBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4376 /* pcmpgtb */, X86::PCMPGTBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4376 /* pcmpgtb */, X86::PCMPGTBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4376 /* pcmpgtb */, X86::PCMPGTBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4376 /* pcmpgtb */, X86::PCMPGTBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4376 /* pcmpgtb */, X86::MMX_PCMPGTBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4384 /* pcmpgtd */, X86::MMX_PCMPGTDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4384 /* pcmpgtd */, X86::MMX_PCMPGTDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4384 /* pcmpgtd */, X86::MMX_PCMPGTDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4384 /* pcmpgtd */, X86::PCMPGTDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4384 /* pcmpgtd */, X86::PCMPGTDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4384 /* pcmpgtd */, X86::PCMPGTDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4384 /* pcmpgtd */, X86::PCMPGTDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4384 /* pcmpgtd */, X86::MMX_PCMPGTDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4392 /* pcmpgtq */, X86::PCMPGTQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4392 /* pcmpgtq */, X86::PCMPGTQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4392 /* pcmpgtq */, X86::PCMPGTQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4392 /* pcmpgtq */, X86::PCMPGTQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4400 /* pcmpgtw */, X86::MMX_PCMPGTWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4400 /* pcmpgtw */, X86::MMX_PCMPGTWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4400 /* pcmpgtw */, X86::MMX_PCMPGTWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4400 /* pcmpgtw */, X86::PCMPGTWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4400 /* pcmpgtw */, X86::PCMPGTWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4400 /* pcmpgtw */, X86::PCMPGTWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4400 /* pcmpgtw */, X86::PCMPGTWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4400 /* pcmpgtw */, X86::MMX_PCMPGTWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4408 /* pcmpistri */, X86::PCMPISTRIrr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4408 /* pcmpistri */, X86::PCMPISTRIrm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4408 /* pcmpistri */, X86::PCMPISTRIrr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4408 /* pcmpistri */, X86::PCMPISTRIrm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4418 /* pcmpistrm */, X86::PCMPISTRM128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 4418 /* pcmpistrm */, X86::PCMPISTRM128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 4418 /* pcmpistrm */, X86::PCMPISTRM128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 4418 /* pcmpistrm */, X86::PCMPISTRM128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 4428 /* pdep */, X86::PDEP32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 4428 /* pdep */, X86::PDEP32rm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_GR32, MCK_GR32, MCK_Mem32 }, 1},
  { 4428 /* pdep */, X86::PDEP64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 4428 /* pdep */, X86::PDEP64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_GR64, MCK_GR64, MCK_Mem64 }, 1},
  { 4433 /* pdepl */, X86::PDEP32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 4433 /* pdepl */, X86::PDEP32rm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32, MCK_GR32 }, 0},
  { 4439 /* pdepq */, X86::PDEP64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 4439 /* pdepq */, X86::PDEP64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64, MCK_GR64 }, 0},
  { 4445 /* pext */, X86::PEXT32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 4445 /* pext */, X86::PEXT32rm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_GR32, MCK_GR32, MCK_Mem32 }, 1},
  { 4445 /* pext */, X86::PEXT64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 4445 /* pext */, X86::PEXT64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_GR64, MCK_GR64, MCK_Mem64 }, 1},
  { 4450 /* pextl */, X86::PEXT32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 4450 /* pextl */, X86::PEXT32rm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32, MCK_GR32 }, 0},
  { 4456 /* pextq */, X86::PEXT64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 4456 /* pextq */, X86::PEXT64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64, MCK_GR64 }, 0},
  { 4462 /* pextrb */, X86::PEXTRBrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4462 /* pextrb */, X86::PEXTRBrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 4462 /* pextrb */, X86::PEXTRBmr, Convert__Mem85_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem8 }, 0},
  { 4462 /* pextrb */, X86::PEXTRBmr, Convert__Mem85_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem8, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4469 /* pextrd */, X86::PEXTRDrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4469 /* pextrd */, X86::PEXTRDrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 4469 /* pextrd */, X86::PEXTRDmr, Convert__Mem325_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem32 }, 0},
  { 4469 /* pextrd */, X86::PEXTRDmr, Convert__Mem325_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4476 /* pextrq */, X86::PEXTRQrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4476 /* pextrq */, X86::PEXTRQrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR64 }, 0},
  { 4476 /* pextrq */, X86::PEXTRQmr, Convert__Mem645_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem64 }, 0},
  { 4476 /* pextrq */, X86::PEXTRQmr, Convert__Mem645_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4483 /* pextrw */, X86::MMX_PEXTRWirri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 4483 /* pextrw */, X86::PEXTRWri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4483 /* pextrw */, X86::MMX_PEXTRWirri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64, MCK_GR32 }, 0},
  { 4483 /* pextrw */, X86::PEXTRWri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 4483 /* pextrw */, X86::PEXTRWmr, Convert__Mem165_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem16 }, 0},
  { 4483 /* pextrw */, X86::PEXTRWmr, Convert__Mem165_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem16, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 4490 /* pf2id */, X86::PF2IDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4490 /* pf2id */, X86::PF2IDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4490 /* pf2id */, X86::PF2IDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4490 /* pf2id */, X86::PF2IDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4496 /* pf2iw */, X86::PF2IWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4496 /* pf2iw */, X86::PF2IWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4496 /* pf2iw */, X86::PF2IWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4496 /* pf2iw */, X86::PF2IWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4502 /* pfacc */, X86::PFACCrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4502 /* pfacc */, X86::PFACCrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4502 /* pfacc */, X86::PFACCrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4502 /* pfacc */, X86::PFACCrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4508 /* pfadd */, X86::PFADDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4508 /* pfadd */, X86::PFADDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4508 /* pfadd */, X86::PFADDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4508 /* pfadd */, X86::PFADDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4514 /* pfcmpeq */, X86::PFCMPEQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4514 /* pfcmpeq */, X86::PFCMPEQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4514 /* pfcmpeq */, X86::PFCMPEQrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4514 /* pfcmpeq */, X86::PFCMPEQrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4522 /* pfcmpge */, X86::PFCMPGErr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4522 /* pfcmpge */, X86::PFCMPGErr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4522 /* pfcmpge */, X86::PFCMPGErm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4522 /* pfcmpge */, X86::PFCMPGErm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4530 /* pfcmpgt */, X86::PFCMPGTrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4530 /* pfcmpgt */, X86::PFCMPGTrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4530 /* pfcmpgt */, X86::PFCMPGTrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4530 /* pfcmpgt */, X86::PFCMPGTrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4538 /* pfmax */, X86::PFMAXrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4538 /* pfmax */, X86::PFMAXrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4538 /* pfmax */, X86::PFMAXrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4538 /* pfmax */, X86::PFMAXrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4544 /* pfmin */, X86::PFMINrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4544 /* pfmin */, X86::PFMINrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4544 /* pfmin */, X86::PFMINrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4544 /* pfmin */, X86::PFMINrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4550 /* pfmul */, X86::PFMULrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4550 /* pfmul */, X86::PFMULrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4550 /* pfmul */, X86::PFMULrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4550 /* pfmul */, X86::PFMULrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4556 /* pfnacc */, X86::PFNACCrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4556 /* pfnacc */, X86::PFNACCrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4556 /* pfnacc */, X86::PFNACCrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4556 /* pfnacc */, X86::PFNACCrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4563 /* pfpnacc */, X86::PFPNACCrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4563 /* pfpnacc */, X86::PFPNACCrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4563 /* pfpnacc */, X86::PFPNACCrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4563 /* pfpnacc */, X86::PFPNACCrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4571 /* pfrcp */, X86::PFRCPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4571 /* pfrcp */, X86::PFRCPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4571 /* pfrcp */, X86::PFRCPrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4571 /* pfrcp */, X86::PFRCPrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4577 /* pfrcpit1 */, X86::PFRCPIT1rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4577 /* pfrcpit1 */, X86::PFRCPIT1rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4577 /* pfrcpit1 */, X86::PFRCPIT1rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4577 /* pfrcpit1 */, X86::PFRCPIT1rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4586 /* pfrcpit2 */, X86::PFRCPIT2rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4586 /* pfrcpit2 */, X86::PFRCPIT2rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4586 /* pfrcpit2 */, X86::PFRCPIT2rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4586 /* pfrcpit2 */, X86::PFRCPIT2rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4595 /* pfrsqit1 */, X86::PFRSQIT1rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4595 /* pfrsqit1 */, X86::PFRSQIT1rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4595 /* pfrsqit1 */, X86::PFRSQIT1rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4595 /* pfrsqit1 */, X86::PFRSQIT1rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4604 /* pfrsqrt */, X86::PFRSQRTrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4604 /* pfrsqrt */, X86::PFRSQRTrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4604 /* pfrsqrt */, X86::PFRSQRTrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4604 /* pfrsqrt */, X86::PFRSQRTrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4612 /* pfsub */, X86::PFSUBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4612 /* pfsub */, X86::PFSUBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4612 /* pfsub */, X86::PFSUBrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4612 /* pfsub */, X86::PFSUBrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4618 /* pfsubr */, X86::PFSUBRrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4618 /* pfsubr */, X86::PFSUBRrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4618 /* pfsubr */, X86::PFSUBRrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4618 /* pfsubr */, X86::PFSUBRrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4625 /* phaddd */, X86::MMX_PHADDrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4625 /* phaddd */, X86::MMX_PHADDrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4625 /* phaddd */, X86::MMX_PHADDrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4625 /* phaddd */, X86::PHADDDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4625 /* phaddd */, X86::PHADDDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4625 /* phaddd */, X86::PHADDDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4625 /* phaddd */, X86::PHADDDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4625 /* phaddd */, X86::MMX_PHADDrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4632 /* phaddsw */, X86::MMX_PHADDSWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4632 /* phaddsw */, X86::MMX_PHADDSWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4632 /* phaddsw */, X86::MMX_PHADDSWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4632 /* phaddsw */, X86::PHADDSWrr128, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4632 /* phaddsw */, X86::PHADDSWrr128, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4632 /* phaddsw */, X86::PHADDSWrm128, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4632 /* phaddsw */, X86::PHADDSWrm128, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4632 /* phaddsw */, X86::MMX_PHADDSWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4640 /* phaddw */, X86::MMX_PHADDWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4640 /* phaddw */, X86::MMX_PHADDWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4640 /* phaddw */, X86::MMX_PHADDWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4640 /* phaddw */, X86::PHADDWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4640 /* phaddw */, X86::PHADDWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4640 /* phaddw */, X86::PHADDWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4640 /* phaddw */, X86::PHADDWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4640 /* phaddw */, X86::MMX_PHADDWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4647 /* phminposuw */, X86::PHMINPOSUWrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4647 /* phminposuw */, X86::PHMINPOSUWrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4647 /* phminposuw */, X86::PHMINPOSUWrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4647 /* phminposuw */, X86::PHMINPOSUWrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4658 /* phsubd */, X86::MMX_PHSUBDrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4658 /* phsubd */, X86::MMX_PHSUBDrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4658 /* phsubd */, X86::MMX_PHSUBDrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4658 /* phsubd */, X86::PHSUBDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4658 /* phsubd */, X86::PHSUBDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4658 /* phsubd */, X86::PHSUBDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4658 /* phsubd */, X86::PHSUBDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4658 /* phsubd */, X86::MMX_PHSUBDrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4665 /* phsubsw */, X86::MMX_PHSUBSWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4665 /* phsubsw */, X86::MMX_PHSUBSWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4665 /* phsubsw */, X86::MMX_PHSUBSWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4665 /* phsubsw */, X86::PHSUBSWrr128, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4665 /* phsubsw */, X86::PHSUBSWrr128, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4665 /* phsubsw */, X86::PHSUBSWrm128, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4665 /* phsubsw */, X86::PHSUBSWrm128, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4665 /* phsubsw */, X86::MMX_PHSUBSWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4673 /* phsubw */, X86::MMX_PHSUBWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4673 /* phsubw */, X86::MMX_PHSUBWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4673 /* phsubw */, X86::MMX_PHSUBWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4673 /* phsubw */, X86::PHSUBWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4673 /* phsubw */, X86::PHSUBWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4673 /* phsubw */, X86::PHSUBWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4673 /* phsubw */, X86::PHSUBWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4673 /* phsubw */, X86::MMX_PHSUBWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4680 /* pi2fd */, X86::PI2FDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4680 /* pi2fd */, X86::PI2FDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4680 /* pi2fd */, X86::PI2FDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4680 /* pi2fd */, X86::PI2FDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4686 /* pi2fw */, X86::PI2FWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4686 /* pi2fw */, X86::PI2FWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4686 /* pi2fw */, X86::PI2FWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4686 /* pi2fw */, X86::PI2FWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4692 /* pinsrb */, X86::PINSRBrr, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 4692 /* pinsrb */, X86::PINSRBrm, Convert__Reg1_0__Tie0__Mem85_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem8, MCK_ImmSExti32i8 }, 1},
  { 4692 /* pinsrb */, X86::PINSRBrr, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32 }, 0},
  { 4692 /* pinsrb */, X86::PINSRBrm, Convert__Reg1_2__Tie0__Mem85_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem8, MCK_FR32 }, 0},
  { 4699 /* pinsrd */, X86::PINSRDrr, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 4699 /* pinsrd */, X86::PINSRDrm, Convert__Reg1_0__Tie0__Mem325_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 4699 /* pinsrd */, X86::PINSRDrr, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32 }, 0},
  { 4699 /* pinsrd */, X86::PINSRDrm, Convert__Reg1_2__Tie0__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32, MCK_FR32 }, 0},
  { 4706 /* pinsrq */, X86::PINSRQrr, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_GR64, MCK_ImmSExti32i8 }, 1},
  { 4706 /* pinsrq */, X86::PINSRQrm, Convert__Reg1_0__Tie0__Mem645_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem64, MCK_ImmSExti32i8 }, 1},
  { 4706 /* pinsrq */, X86::PINSRQrr, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR64, MCK_FR32 }, 0},
  { 4706 /* pinsrq */, X86::PINSRQrm, Convert__Reg1_2__Tie0__Mem645_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem64, MCK_FR32 }, 0},
  { 4713 /* pinsrw */, X86::MMX_PINSRWirri, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR64, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 4713 /* pinsrw */, X86::MMX_PINSRWirmi, Convert__Reg1_0__Tie0__Mem165_1__ImmSExti32i81_2, 0, { MCK_VR64, MCK_Mem16, MCK_ImmSExti32i8 }, 1},
  { 4713 /* pinsrw */, X86::PINSRWrri, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 4713 /* pinsrw */, X86::PINSRWrmi, Convert__Reg1_0__Tie0__Mem165_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem16, MCK_ImmSExti32i8 }, 1},
  { 4713 /* pinsrw */, X86::MMX_PINSRWirri, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_VR64 }, 0},
  { 4713 /* pinsrw */, X86::PINSRWrri, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32 }, 0},
  { 4713 /* pinsrw */, X86::MMX_PINSRWirmi, Convert__Reg1_2__Tie0__Mem165_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem16, MCK_VR64 }, 0},
  { 4713 /* pinsrw */, X86::PINSRWrmi, Convert__Reg1_2__Tie0__Mem165_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem16, MCK_FR32 }, 0},
  { 4720 /* pmaddubsw */, X86::MMX_PMADDUBSWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4720 /* pmaddubsw */, X86::MMX_PMADDUBSWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4720 /* pmaddubsw */, X86::MMX_PMADDUBSWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4720 /* pmaddubsw */, X86::PMADDUBSWrr128, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4720 /* pmaddubsw */, X86::PMADDUBSWrr128, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4720 /* pmaddubsw */, X86::PMADDUBSWrm128, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4720 /* pmaddubsw */, X86::PMADDUBSWrm128, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4720 /* pmaddubsw */, X86::MMX_PMADDUBSWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4730 /* pmaddwd */, X86::MMX_PMADDWDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4730 /* pmaddwd */, X86::MMX_PMADDWDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4730 /* pmaddwd */, X86::MMX_PMADDWDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4730 /* pmaddwd */, X86::PMADDWDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4730 /* pmaddwd */, X86::PMADDWDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4730 /* pmaddwd */, X86::PMADDWDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4730 /* pmaddwd */, X86::PMADDWDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4730 /* pmaddwd */, X86::MMX_PMADDWDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4738 /* pmaxsb */, X86::PMAXSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4738 /* pmaxsb */, X86::PMAXSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4738 /* pmaxsb */, X86::PMAXSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4738 /* pmaxsb */, X86::PMAXSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4745 /* pmaxsd */, X86::PMAXSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4745 /* pmaxsd */, X86::PMAXSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4745 /* pmaxsd */, X86::PMAXSDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4745 /* pmaxsd */, X86::PMAXSDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4752 /* pmaxsw */, X86::MMX_PMAXSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4752 /* pmaxsw */, X86::MMX_PMAXSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4752 /* pmaxsw */, X86::MMX_PMAXSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4752 /* pmaxsw */, X86::PMAXSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4752 /* pmaxsw */, X86::PMAXSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4752 /* pmaxsw */, X86::PMAXSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4752 /* pmaxsw */, X86::PMAXSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4752 /* pmaxsw */, X86::MMX_PMAXSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4759 /* pmaxub */, X86::MMX_PMAXUBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4759 /* pmaxub */, X86::MMX_PMAXUBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4759 /* pmaxub */, X86::MMX_PMAXUBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4759 /* pmaxub */, X86::PMAXUBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4759 /* pmaxub */, X86::PMAXUBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4759 /* pmaxub */, X86::PMAXUBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4759 /* pmaxub */, X86::PMAXUBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4759 /* pmaxub */, X86::MMX_PMAXUBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4766 /* pmaxud */, X86::PMAXUDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4766 /* pmaxud */, X86::PMAXUDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4766 /* pmaxud */, X86::PMAXUDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4766 /* pmaxud */, X86::PMAXUDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4773 /* pmaxuw */, X86::PMAXUWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4773 /* pmaxuw */, X86::PMAXUWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4773 /* pmaxuw */, X86::PMAXUWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4773 /* pmaxuw */, X86::PMAXUWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4780 /* pminsb */, X86::PMINSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4780 /* pminsb */, X86::PMINSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4780 /* pminsb */, X86::PMINSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4780 /* pminsb */, X86::PMINSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4787 /* pminsd */, X86::PMINSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4787 /* pminsd */, X86::PMINSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4787 /* pminsd */, X86::PMINSDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4787 /* pminsd */, X86::PMINSDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4794 /* pminsw */, X86::MMX_PMINSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4794 /* pminsw */, X86::MMX_PMINSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4794 /* pminsw */, X86::MMX_PMINSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4794 /* pminsw */, X86::PMINSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4794 /* pminsw */, X86::PMINSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4794 /* pminsw */, X86::PMINSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4794 /* pminsw */, X86::PMINSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4794 /* pminsw */, X86::MMX_PMINSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4801 /* pminub */, X86::MMX_PMINUBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4801 /* pminub */, X86::MMX_PMINUBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4801 /* pminub */, X86::MMX_PMINUBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4801 /* pminub */, X86::PMINUBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4801 /* pminub */, X86::PMINUBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4801 /* pminub */, X86::PMINUBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4801 /* pminub */, X86::PMINUBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4801 /* pminub */, X86::MMX_PMINUBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4808 /* pminud */, X86::PMINUDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4808 /* pminud */, X86::PMINUDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4808 /* pminud */, X86::PMINUDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4808 /* pminud */, X86::PMINUDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4815 /* pminuw */, X86::PMINUWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4815 /* pminuw */, X86::PMINUWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4815 /* pminuw */, X86::PMINUWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4815 /* pminuw */, X86::PMINUWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4822 /* pmovmskb */, X86::MMX_PMOVMSKBrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_VR64 }, 1},
  { 4822 /* pmovmskb */, X86::PMOVMSKBrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 4822 /* pmovmskb */, X86::MMX_PMOVMSKBrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_GR32 }, 0},
  { 4822 /* pmovmskb */, X86::PMOVMSKBrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 4831 /* pmovsxbd */, X86::PMOVSXBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4831 /* pmovsxbd */, X86::PMOVSXBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4831 /* pmovsxbd */, X86::PMOVSXBDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 4831 /* pmovsxbd */, X86::PMOVSXBDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 4840 /* pmovsxbq */, X86::PMOVSXBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4840 /* pmovsxbq */, X86::PMOVSXBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4840 /* pmovsxbq */, X86::PMOVSXBQrm, Convert__Reg1_0__Mem165_1, 0, { MCK_FR32, MCK_Mem16 }, 1},
  { 4840 /* pmovsxbq */, X86::PMOVSXBQrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_FR32 }, 0},
  { 4849 /* pmovsxbw */, X86::PMOVSXBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4849 /* pmovsxbw */, X86::PMOVSXBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4849 /* pmovsxbw */, X86::PMOVSXBWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4849 /* pmovsxbw */, X86::PMOVSXBWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4858 /* pmovsxdq */, X86::PMOVSXDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4858 /* pmovsxdq */, X86::PMOVSXDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4858 /* pmovsxdq */, X86::PMOVSXDQrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4858 /* pmovsxdq */, X86::PMOVSXDQrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4867 /* pmovsxwd */, X86::PMOVSXWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4867 /* pmovsxwd */, X86::PMOVSXWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4867 /* pmovsxwd */, X86::PMOVSXWDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4867 /* pmovsxwd */, X86::PMOVSXWDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4876 /* pmovsxwq */, X86::PMOVSXWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4876 /* pmovsxwq */, X86::PMOVSXWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4876 /* pmovsxwq */, X86::PMOVSXWQrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 4876 /* pmovsxwq */, X86::PMOVSXWQrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 4885 /* pmovzxbd */, X86::PMOVZXBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4885 /* pmovzxbd */, X86::PMOVZXBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4885 /* pmovzxbd */, X86::PMOVZXBDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 4885 /* pmovzxbd */, X86::PMOVZXBDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 4894 /* pmovzxbq */, X86::PMOVZXBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4894 /* pmovzxbq */, X86::PMOVZXBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4894 /* pmovzxbq */, X86::PMOVZXBQrm, Convert__Reg1_0__Mem165_1, 0, { MCK_FR32, MCK_Mem16 }, 1},
  { 4894 /* pmovzxbq */, X86::PMOVZXBQrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_FR32 }, 0},
  { 4903 /* pmovzxbw */, X86::PMOVZXBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4903 /* pmovzxbw */, X86::PMOVZXBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4903 /* pmovzxbw */, X86::PMOVZXBWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4903 /* pmovzxbw */, X86::PMOVZXBWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4912 /* pmovzxdq */, X86::PMOVZXDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4912 /* pmovzxdq */, X86::PMOVZXDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4912 /* pmovzxdq */, X86::PMOVZXDQrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4912 /* pmovzxdq */, X86::PMOVZXDQrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4921 /* pmovzxwd */, X86::PMOVZXWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4921 /* pmovzxwd */, X86::PMOVZXWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4921 /* pmovzxwd */, X86::PMOVZXWDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 4921 /* pmovzxwd */, X86::PMOVZXWDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 4930 /* pmovzxwq */, X86::PMOVZXWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4930 /* pmovzxwq */, X86::PMOVZXWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4930 /* pmovzxwq */, X86::PMOVZXWQrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 4930 /* pmovzxwq */, X86::PMOVZXWQrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 4939 /* pmuldq */, X86::PMULDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4939 /* pmuldq */, X86::PMULDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4939 /* pmuldq */, X86::PMULDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4939 /* pmuldq */, X86::PMULDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4946 /* pmulhrsw */, X86::MMX_PMULHRSWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4946 /* pmulhrsw */, X86::MMX_PMULHRSWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4946 /* pmulhrsw */, X86::MMX_PMULHRSWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4946 /* pmulhrsw */, X86::PMULHRSWrr128, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4946 /* pmulhrsw */, X86::PMULHRSWrr128, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4946 /* pmulhrsw */, X86::PMULHRSWrm128, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4946 /* pmulhrsw */, X86::PMULHRSWrm128, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4946 /* pmulhrsw */, X86::MMX_PMULHRSWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4955 /* pmulhrw */, X86::PMULHRWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4955 /* pmulhrw */, X86::PMULHRWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4955 /* pmulhrw */, X86::PMULHRWrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4955 /* pmulhrw */, X86::PMULHRWrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4963 /* pmulhuw */, X86::MMX_PMULHUWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4963 /* pmulhuw */, X86::MMX_PMULHUWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4963 /* pmulhuw */, X86::MMX_PMULHUWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4963 /* pmulhuw */, X86::PMULHUWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4963 /* pmulhuw */, X86::PMULHUWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4963 /* pmulhuw */, X86::PMULHUWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4963 /* pmulhuw */, X86::PMULHUWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4963 /* pmulhuw */, X86::MMX_PMULHUWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4971 /* pmulhw */, X86::MMX_PMULHWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4971 /* pmulhw */, X86::MMX_PMULHWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4971 /* pmulhw */, X86::MMX_PMULHWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4971 /* pmulhw */, X86::PMULHWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4971 /* pmulhw */, X86::PMULHWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4971 /* pmulhw */, X86::PMULHWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4971 /* pmulhw */, X86::PMULHWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4971 /* pmulhw */, X86::MMX_PMULHWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4978 /* pmulld */, X86::PMULLDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4978 /* pmulld */, X86::PMULLDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4978 /* pmulld */, X86::PMULLDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4978 /* pmulld */, X86::PMULLDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4985 /* pmullw */, X86::MMX_PMULLWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4985 /* pmullw */, X86::MMX_PMULLWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4985 /* pmullw */, X86::MMX_PMULLWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4985 /* pmullw */, X86::PMULLWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4985 /* pmullw */, X86::PMULLWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4985 /* pmullw */, X86::PMULLWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4985 /* pmullw */, X86::PMULLWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4985 /* pmullw */, X86::MMX_PMULLWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 4992 /* pmuludq */, X86::MMX_PMULUDQirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 4992 /* pmuludq */, X86::MMX_PMULUDQirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 4992 /* pmuludq */, X86::MMX_PMULUDQirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 4992 /* pmuludq */, X86::PMULUDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 4992 /* pmuludq */, X86::PMULUDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 4992 /* pmuludq */, X86::PMULUDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 4992 /* pmuludq */, X86::PMULUDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 4992 /* pmuludq */, X86::MMX_PMULUDQirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5000 /* pop */, X86::POP16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 5000 /* pop */, X86::POP16rmr, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 5000 /* pop */, X86::POP32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 5000 /* pop */, X86::POP32rmr, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 5000 /* pop */, X86::POP64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 5000 /* pop */, X86::POP64rmr, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 5000 /* pop */, X86::POPDS16, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 1},
  { 5000 /* pop */, X86::POPDS32, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 1},
  { 5000 /* pop */, X86::POPSS16, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 1},
  { 5000 /* pop */, X86::POPSS32, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 1},
  { 5000 /* pop */, X86::POPES16, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 1},
  { 5000 /* pop */, X86::POPES32, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 1},
  { 5000 /* pop */, X86::POPFS32, Convert_NoOperands, Feature_In32BitMode, { MCK_FS }, 1},
  { 5000 /* pop */, X86::POPFS16, Convert_NoOperands, 0, { MCK_FS }, 1},
  { 5000 /* pop */, X86::POPFS64, Convert_NoOperands, 0, { MCK_FS }, 1},
  { 5000 /* pop */, X86::POPGS32, Convert_NoOperands, Feature_In32BitMode, { MCK_GS }, 1},
  { 5000 /* pop */, X86::POPGS16, Convert_NoOperands, 0, { MCK_GS }, 1},
  { 5000 /* pop */, X86::POPGS64, Convert_NoOperands, 0, { MCK_GS }, 1},
  { 5000 /* pop */, X86::POP16rmm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5000 /* pop */, X86::POP32rmm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5000 /* pop */, X86::POP64rmm, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5004 /* popa */, X86::POPA32, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 5009 /* popal */, X86::POPA32, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 5015 /* popcnt */, X86::POPCNT16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 5015 /* popcnt */, X86::POPCNT16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 5015 /* popcnt */, X86::POPCNT32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 5015 /* popcnt */, X86::POPCNT32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 5015 /* popcnt */, X86::POPCNT64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 5015 /* popcnt */, X86::POPCNT64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 5022 /* popcntl */, X86::POPCNT32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 5022 /* popcntl */, X86::POPCNT32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 5030 /* popcntq */, X86::POPCNT64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 5030 /* popcntq */, X86::POPCNT64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 5038 /* popcntw */, X86::POPCNT16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 5038 /* popcntw */, X86::POPCNT16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 5046 /* popf */, X86::POPF16, Convert_NoOperands, 0, {  }, 1},
  { 5051 /* popfd */, X86::POPF32, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 5057 /* popfl */, X86::POPF32, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 5063 /* popfq */, X86::POPF64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 5063 /* popfq */, X86::POPF64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 5069 /* popfw */, X86::POPF16, Convert_NoOperands, 0, {  }, 0},
  { 5075 /* popl */, X86::POP32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 5075 /* popl */, X86::POP32rmr, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 5075 /* popl */, X86::POPDS32, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 0},
  { 5075 /* popl */, X86::POPSS32, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 0},
  { 5075 /* popl */, X86::POPES32, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 0},
  { 5075 /* popl */, X86::POPFS32, Convert_NoOperands, Feature_In32BitMode, { MCK_FS }, 0},
  { 5075 /* popl */, X86::POPGS32, Convert_NoOperands, Feature_In32BitMode, { MCK_GS }, 0},
  { 5075 /* popl */, X86::POP32rmm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5080 /* popq */, X86::POP64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 5080 /* popq */, X86::POP64rmr, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 5080 /* popq */, X86::POPFS64, Convert_NoOperands, 0, { MCK_FS }, 0},
  { 5080 /* popq */, X86::POPGS64, Convert_NoOperands, 0, { MCK_GS }, 0},
  { 5080 /* popq */, X86::POP64rmm, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5085 /* popw */, X86::POP16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 5085 /* popw */, X86::POP16rmr, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 5085 /* popw */, X86::POPDS16, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 0},
  { 5085 /* popw */, X86::POPSS16, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 0},
  { 5085 /* popw */, X86::POPES16, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 0},
  { 5085 /* popw */, X86::POPFS16, Convert_NoOperands, 0, { MCK_FS }, 0},
  { 5085 /* popw */, X86::POPGS16, Convert_NoOperands, 0, { MCK_GS }, 0},
  { 5085 /* popw */, X86::POP16rmm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5090 /* por */, X86::MMX_PORirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5090 /* por */, X86::MMX_PORirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5090 /* por */, X86::MMX_PORirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5090 /* por */, X86::PORrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5090 /* por */, X86::PORrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5090 /* por */, X86::PORrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5090 /* por */, X86::PORrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5090 /* por */, X86::MMX_PORirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5094 /* prefetch */, X86::PREFETCH, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5094 /* prefetch */, X86::PREFETCH, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5103 /* prefetchnta */, X86::PREFETCHNTA, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5103 /* prefetchnta */, X86::PREFETCHNTA, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5115 /* prefetcht0 */, X86::PREFETCHT0, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5115 /* prefetcht0 */, X86::PREFETCHT0, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5126 /* prefetcht1 */, X86::PREFETCHT1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5126 /* prefetcht1 */, X86::PREFETCHT1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5137 /* prefetcht2 */, X86::PREFETCHT2, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5137 /* prefetcht2 */, X86::PREFETCHT2, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5148 /* prefetchw */, X86::PREFETCHW, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5148 /* prefetchw */, X86::PREFETCHW, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5158 /* psadbw */, X86::MMX_PSADBWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5158 /* psadbw */, X86::MMX_PSADBWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5158 /* psadbw */, X86::MMX_PSADBWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5158 /* psadbw */, X86::PSADBWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5158 /* psadbw */, X86::PSADBWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5158 /* psadbw */, X86::PSADBWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5158 /* psadbw */, X86::PSADBWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5158 /* psadbw */, X86::MMX_PSADBWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5165 /* pshufb */, X86::MMX_PSHUFBrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5165 /* pshufb */, X86::MMX_PSHUFBrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5165 /* pshufb */, X86::MMX_PSHUFBrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5165 /* pshufb */, X86::PSHUFBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5165 /* pshufb */, X86::PSHUFBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5165 /* pshufb */, X86::PSHUFBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5165 /* pshufb */, X86::PSHUFBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5165 /* pshufb */, X86::MMX_PSHUFBrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5172 /* pshufd */, X86::PSHUFDri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 5172 /* pshufd */, X86::PSHUFDmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 5172 /* pshufd */, X86::PSHUFDri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 5172 /* pshufd */, X86::PSHUFDmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 5179 /* pshufhw */, X86::PSHUFHWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 5179 /* pshufhw */, X86::PSHUFHWmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 5179 /* pshufhw */, X86::PSHUFHWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 5179 /* pshufhw */, X86::PSHUFHWmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 5187 /* pshuflw */, X86::PSHUFLWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 5187 /* pshuflw */, X86::PSHUFLWmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 5187 /* pshuflw */, X86::PSHUFLWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 5187 /* pshuflw */, X86::PSHUFLWmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 5195 /* pshufw */, X86::MMX_PSHUFWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR64, MCK_VR64, MCK_Imm }, 1},
  { 5195 /* pshufw */, X86::MMX_PSHUFWmi, Convert__Reg1_0__Mem645_1__Imm1_2, 0, { MCK_VR64, MCK_Mem64, MCK_Imm }, 1},
  { 5195 /* pshufw */, X86::MMX_PSHUFWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR64, MCK_VR64 }, 0},
  { 5195 /* pshufw */, X86::MMX_PSHUFWmi, Convert__Reg1_2__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64, MCK_VR64 }, 0},
  { 5202 /* psignb */, X86::MMX_PSIGNBrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5202 /* psignb */, X86::MMX_PSIGNBrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5202 /* psignb */, X86::MMX_PSIGNBrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5202 /* psignb */, X86::PSIGNBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5202 /* psignb */, X86::PSIGNBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5202 /* psignb */, X86::PSIGNBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5202 /* psignb */, X86::PSIGNBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5202 /* psignb */, X86::MMX_PSIGNBrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5209 /* psignd */, X86::MMX_PSIGNDrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5209 /* psignd */, X86::MMX_PSIGNDrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5209 /* psignd */, X86::MMX_PSIGNDrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5209 /* psignd */, X86::PSIGNDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5209 /* psignd */, X86::PSIGNDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5209 /* psignd */, X86::PSIGNDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5209 /* psignd */, X86::PSIGNDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5209 /* psignd */, X86::MMX_PSIGNDrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5216 /* psignw */, X86::MMX_PSIGNWrr64, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5216 /* psignw */, X86::MMX_PSIGNWrr64, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5216 /* psignw */, X86::MMX_PSIGNWrm64, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5216 /* psignw */, X86::PSIGNWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5216 /* psignw */, X86::PSIGNWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5216 /* psignw */, X86::PSIGNWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5216 /* psignw */, X86::PSIGNWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5216 /* psignw */, X86::MMX_PSIGNWrm64, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5223 /* pslld */, X86::MMX_PSLLDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5223 /* pslld */, X86::MMX_PSLLDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5223 /* pslld */, X86::MMX_PSLLDri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5223 /* pslld */, X86::MMX_PSLLDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5223 /* pslld */, X86::PSLLDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5223 /* pslld */, X86::PSLLDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5223 /* pslld */, X86::PSLLDri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5223 /* pslld */, X86::PSLLDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5223 /* pslld */, X86::MMX_PSLLDri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5223 /* pslld */, X86::PSLLDri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5223 /* pslld */, X86::PSLLDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5223 /* pslld */, X86::MMX_PSLLDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5229 /* pslldq */, X86::PSLLDQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5229 /* pslldq */, X86::PSLLDQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5236 /* psllq */, X86::MMX_PSLLQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5236 /* psllq */, X86::MMX_PSLLQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5236 /* psllq */, X86::MMX_PSLLQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5236 /* psllq */, X86::MMX_PSLLQrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5236 /* psllq */, X86::PSLLQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5236 /* psllq */, X86::PSLLQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5236 /* psllq */, X86::PSLLQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5236 /* psllq */, X86::PSLLQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5236 /* psllq */, X86::MMX_PSLLQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5236 /* psllq */, X86::PSLLQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5236 /* psllq */, X86::PSLLQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5236 /* psllq */, X86::MMX_PSLLQrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5242 /* psllw */, X86::MMX_PSLLWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5242 /* psllw */, X86::MMX_PSLLWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5242 /* psllw */, X86::MMX_PSLLWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5242 /* psllw */, X86::MMX_PSLLWrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5242 /* psllw */, X86::PSLLWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5242 /* psllw */, X86::PSLLWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5242 /* psllw */, X86::PSLLWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5242 /* psllw */, X86::PSLLWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5242 /* psllw */, X86::MMX_PSLLWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5242 /* psllw */, X86::PSLLWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5242 /* psllw */, X86::PSLLWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5242 /* psllw */, X86::MMX_PSLLWrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5248 /* psrad */, X86::MMX_PSRADrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5248 /* psrad */, X86::MMX_PSRADrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5248 /* psrad */, X86::MMX_PSRADri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5248 /* psrad */, X86::MMX_PSRADrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5248 /* psrad */, X86::PSRADrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5248 /* psrad */, X86::PSRADrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5248 /* psrad */, X86::PSRADri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5248 /* psrad */, X86::PSRADrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5248 /* psrad */, X86::MMX_PSRADri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5248 /* psrad */, X86::PSRADri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5248 /* psrad */, X86::PSRADrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5248 /* psrad */, X86::MMX_PSRADrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5254 /* psraw */, X86::MMX_PSRAWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5254 /* psraw */, X86::MMX_PSRAWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5254 /* psraw */, X86::MMX_PSRAWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5254 /* psraw */, X86::MMX_PSRAWrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5254 /* psraw */, X86::PSRAWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5254 /* psraw */, X86::PSRAWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5254 /* psraw */, X86::PSRAWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5254 /* psraw */, X86::PSRAWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5254 /* psraw */, X86::MMX_PSRAWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5254 /* psraw */, X86::PSRAWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5254 /* psraw */, X86::PSRAWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5254 /* psraw */, X86::MMX_PSRAWrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5260 /* psrld */, X86::MMX_PSRLDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5260 /* psrld */, X86::MMX_PSRLDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5260 /* psrld */, X86::MMX_PSRLDri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5260 /* psrld */, X86::MMX_PSRLDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5260 /* psrld */, X86::PSRLDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5260 /* psrld */, X86::PSRLDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5260 /* psrld */, X86::PSRLDri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5260 /* psrld */, X86::PSRLDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5260 /* psrld */, X86::MMX_PSRLDri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5260 /* psrld */, X86::PSRLDri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5260 /* psrld */, X86::PSRLDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5260 /* psrld */, X86::MMX_PSRLDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5266 /* psrldq */, X86::PSRLDQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5266 /* psrldq */, X86::PSRLDQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5273 /* psrlq */, X86::MMX_PSRLQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5273 /* psrlq */, X86::MMX_PSRLQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5273 /* psrlq */, X86::MMX_PSRLQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5273 /* psrlq */, X86::MMX_PSRLQrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5273 /* psrlq */, X86::PSRLQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5273 /* psrlq */, X86::PSRLQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5273 /* psrlq */, X86::PSRLQri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5273 /* psrlq */, X86::PSRLQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5273 /* psrlq */, X86::MMX_PSRLQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5273 /* psrlq */, X86::PSRLQri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5273 /* psrlq */, X86::PSRLQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5273 /* psrlq */, X86::MMX_PSRLQrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5279 /* psrlw */, X86::MMX_PSRLWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5279 /* psrlw */, X86::MMX_PSRLWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5279 /* psrlw */, X86::MMX_PSRLWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_VR64, MCK_ImmSExti32i8 }, 1},
  { 5279 /* psrlw */, X86::MMX_PSRLWrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5279 /* psrlw */, X86::PSRLWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5279 /* psrlw */, X86::PSRLWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5279 /* psrlw */, X86::PSRLWri, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5279 /* psrlw */, X86::PSRLWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5279 /* psrlw */, X86::MMX_PSRLWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR64 }, 0},
  { 5279 /* psrlw */, X86::PSRLWri, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32 }, 0},
  { 5279 /* psrlw */, X86::PSRLWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5279 /* psrlw */, X86::MMX_PSRLWrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5285 /* psubb */, X86::MMX_PSUBBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5285 /* psubb */, X86::MMX_PSUBBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5285 /* psubb */, X86::MMX_PSUBBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5285 /* psubb */, X86::PSUBBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5285 /* psubb */, X86::PSUBBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5285 /* psubb */, X86::PSUBBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5285 /* psubb */, X86::PSUBBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5285 /* psubb */, X86::MMX_PSUBBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5291 /* psubd */, X86::MMX_PSUBDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5291 /* psubd */, X86::MMX_PSUBDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5291 /* psubd */, X86::MMX_PSUBDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5291 /* psubd */, X86::PSUBDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5291 /* psubd */, X86::PSUBDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5291 /* psubd */, X86::PSUBDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5291 /* psubd */, X86::PSUBDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5291 /* psubd */, X86::MMX_PSUBDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5297 /* psubq */, X86::MMX_PSUBQirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5297 /* psubq */, X86::MMX_PSUBQirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5297 /* psubq */, X86::MMX_PSUBQirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5297 /* psubq */, X86::PSUBQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5297 /* psubq */, X86::PSUBQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5297 /* psubq */, X86::PSUBQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5297 /* psubq */, X86::PSUBQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5297 /* psubq */, X86::MMX_PSUBQirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5303 /* psubsb */, X86::MMX_PSUBSBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5303 /* psubsb */, X86::MMX_PSUBSBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5303 /* psubsb */, X86::MMX_PSUBSBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5303 /* psubsb */, X86::PSUBSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5303 /* psubsb */, X86::PSUBSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5303 /* psubsb */, X86::PSUBSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5303 /* psubsb */, X86::PSUBSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5303 /* psubsb */, X86::MMX_PSUBSBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5310 /* psubsw */, X86::MMX_PSUBSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5310 /* psubsw */, X86::MMX_PSUBSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5310 /* psubsw */, X86::MMX_PSUBSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5310 /* psubsw */, X86::PSUBSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5310 /* psubsw */, X86::PSUBSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5310 /* psubsw */, X86::PSUBSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5310 /* psubsw */, X86::PSUBSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5310 /* psubsw */, X86::MMX_PSUBSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5317 /* psubusb */, X86::MMX_PSUBUSBirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5317 /* psubusb */, X86::MMX_PSUBUSBirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5317 /* psubusb */, X86::MMX_PSUBUSBirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5317 /* psubusb */, X86::PSUBUSBrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5317 /* psubusb */, X86::PSUBUSBrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5317 /* psubusb */, X86::PSUBUSBrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5317 /* psubusb */, X86::PSUBUSBrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5317 /* psubusb */, X86::MMX_PSUBUSBirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5325 /* psubusw */, X86::MMX_PSUBUSWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5325 /* psubusw */, X86::MMX_PSUBUSWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5325 /* psubusw */, X86::MMX_PSUBUSWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5325 /* psubusw */, X86::PSUBUSWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5325 /* psubusw */, X86::PSUBUSWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5325 /* psubusw */, X86::PSUBUSWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5325 /* psubusw */, X86::PSUBUSWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5325 /* psubusw */, X86::MMX_PSUBUSWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5333 /* psubw */, X86::MMX_PSUBWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5333 /* psubw */, X86::MMX_PSUBWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5333 /* psubw */, X86::MMX_PSUBWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5333 /* psubw */, X86::PSUBWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5333 /* psubw */, X86::PSUBWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5333 /* psubw */, X86::PSUBWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5333 /* psubw */, X86::PSUBWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5333 /* psubw */, X86::MMX_PSUBWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5339 /* pswapd */, X86::PSWAPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5339 /* pswapd */, X86::PSWAPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5339 /* pswapd */, X86::PSWAPDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5339 /* pswapd */, X86::PSWAPDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5346 /* ptest */, X86::PTESTrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5346 /* ptest */, X86::PTESTrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5346 /* ptest */, X86::PTESTrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5346 /* ptest */, X86::PTESTrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5352 /* punpckhbw */, X86::MMX_PUNPCKHBWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5352 /* punpckhbw */, X86::MMX_PUNPCKHBWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5352 /* punpckhbw */, X86::MMX_PUNPCKHBWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5352 /* punpckhbw */, X86::PUNPCKHBWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5352 /* punpckhbw */, X86::PUNPCKHBWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5352 /* punpckhbw */, X86::PUNPCKHBWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5352 /* punpckhbw */, X86::PUNPCKHBWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5352 /* punpckhbw */, X86::MMX_PUNPCKHBWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5362 /* punpckhdq */, X86::MMX_PUNPCKHDQirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5362 /* punpckhdq */, X86::MMX_PUNPCKHDQirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5362 /* punpckhdq */, X86::MMX_PUNPCKHDQirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5362 /* punpckhdq */, X86::PUNPCKHDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5362 /* punpckhdq */, X86::PUNPCKHDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5362 /* punpckhdq */, X86::PUNPCKHDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5362 /* punpckhdq */, X86::PUNPCKHDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5362 /* punpckhdq */, X86::MMX_PUNPCKHDQirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5372 /* punpckhqdq */, X86::PUNPCKHQDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5372 /* punpckhqdq */, X86::PUNPCKHQDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5372 /* punpckhqdq */, X86::PUNPCKHQDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5372 /* punpckhqdq */, X86::PUNPCKHQDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5383 /* punpckhwd */, X86::MMX_PUNPCKHWDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5383 /* punpckhwd */, X86::MMX_PUNPCKHWDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5383 /* punpckhwd */, X86::MMX_PUNPCKHWDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5383 /* punpckhwd */, X86::PUNPCKHWDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5383 /* punpckhwd */, X86::PUNPCKHWDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5383 /* punpckhwd */, X86::PUNPCKHWDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5383 /* punpckhwd */, X86::PUNPCKHWDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5383 /* punpckhwd */, X86::MMX_PUNPCKHWDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5393 /* punpcklbw */, X86::MMX_PUNPCKLBWirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5393 /* punpcklbw */, X86::MMX_PUNPCKLBWirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5393 /* punpcklbw */, X86::MMX_PUNPCKLBWirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5393 /* punpcklbw */, X86::PUNPCKLBWrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5393 /* punpcklbw */, X86::PUNPCKLBWrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5393 /* punpcklbw */, X86::PUNPCKLBWrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5393 /* punpcklbw */, X86::PUNPCKLBWrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5393 /* punpcklbw */, X86::MMX_PUNPCKLBWirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5403 /* punpckldq */, X86::MMX_PUNPCKLDQirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5403 /* punpckldq */, X86::MMX_PUNPCKLDQirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5403 /* punpckldq */, X86::MMX_PUNPCKLDQirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5403 /* punpckldq */, X86::PUNPCKLDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5403 /* punpckldq */, X86::PUNPCKLDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5403 /* punpckldq */, X86::PUNPCKLDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5403 /* punpckldq */, X86::PUNPCKLDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5403 /* punpckldq */, X86::MMX_PUNPCKLDQirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5413 /* punpcklqdq */, X86::PUNPCKLQDQrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5413 /* punpcklqdq */, X86::PUNPCKLQDQrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5413 /* punpcklqdq */, X86::PUNPCKLQDQrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5413 /* punpcklqdq */, X86::PUNPCKLQDQrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5424 /* punpcklwd */, X86::MMX_PUNPCKLWDirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5424 /* punpcklwd */, X86::MMX_PUNPCKLWDirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5424 /* punpcklwd */, X86::MMX_PUNPCKLWDirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5424 /* punpcklwd */, X86::PUNPCKLWDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5424 /* punpcklwd */, X86::PUNPCKLWDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5424 /* punpcklwd */, X86::PUNPCKLWDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5424 /* punpcklwd */, X86::PUNPCKLWDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5424 /* punpcklwd */, X86::MMX_PUNPCKLWDirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5434 /* push */, X86::PUSH16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 5434 /* push */, X86::PUSH16rmr, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 5434 /* push */, X86::PUSH32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 5434 /* push */, X86::PUSH32rmr, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 5434 /* push */, X86::PUSH64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 5434 /* push */, X86::PUSH64rmr, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 5434 /* push */, X86::PUSHCS16, Convert_NoOperands, Feature_In32BitMode, { MCK_CS }, 1},
  { 5434 /* push */, X86::PUSHCS32, Convert_NoOperands, Feature_In32BitMode, { MCK_CS }, 1},
  { 5434 /* push */, X86::PUSHDS16, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 1},
  { 5434 /* push */, X86::PUSHDS32, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 1},
  { 5434 /* push */, X86::PUSHSS16, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 1},
  { 5434 /* push */, X86::PUSHSS32, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 1},
  { 5434 /* push */, X86::PUSHES16, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 1},
  { 5434 /* push */, X86::PUSHES32, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 1},
  { 5434 /* push */, X86::PUSHFS32, Convert_NoOperands, Feature_In32BitMode, { MCK_FS }, 1},
  { 5434 /* push */, X86::PUSHFS16, Convert_NoOperands, 0, { MCK_FS }, 1},
  { 5434 /* push */, X86::PUSHFS64, Convert_NoOperands, 0, { MCK_FS }, 1},
  { 5434 /* push */, X86::PUSHGS32, Convert_NoOperands, Feature_In32BitMode, { MCK_GS }, 1},
  { 5434 /* push */, X86::PUSHGS16, Convert_NoOperands, 0, { MCK_GS }, 1},
  { 5434 /* push */, X86::PUSHGS64, Convert_NoOperands, 0, { MCK_GS }, 1},
  { 5434 /* push */, X86::PUSH64i8, Convert__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8 }, 1},
  { 5434 /* push */, X86::PUSHi8, Convert__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8 }, 1},
  { 5434 /* push */, X86::PUSH64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32 }, 1},
  { 5434 /* push */, X86::PUSH64i16, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5434 /* push */, X86::PUSHi16, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5434 /* push */, X86::PUSHi32, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5434 /* push */, X86::PUSH16rmm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5434 /* push */, X86::PUSH32rmm, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5434 /* push */, X86::PUSH64rmm, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5439 /* pusha */, X86::PUSHA32, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 5445 /* pushal */, X86::PUSHA32, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 5452 /* pushf */, X86::PUSHF16, Convert_NoOperands, 0, {  }, 1},
  { 5458 /* pushfd */, X86::PUSHF32, Convert_NoOperands, Feature_In32BitMode, {  }, 1},
  { 5465 /* pushfl */, X86::PUSHF32, Convert_NoOperands, Feature_In32BitMode, {  }, 0},
  { 5472 /* pushfq */, X86::PUSHF64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 5472 /* pushfq */, X86::PUSHF64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 5479 /* pushfw */, X86::PUSHF16, Convert_NoOperands, 0, {  }, 0},
  { 5486 /* pushl */, X86::PUSH32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 5486 /* pushl */, X86::PUSH32rmr, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 5486 /* pushl */, X86::PUSHCS32, Convert_NoOperands, Feature_In32BitMode, { MCK_CS }, 0},
  { 5486 /* pushl */, X86::PUSHDS32, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 0},
  { 5486 /* pushl */, X86::PUSHSS32, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 0},
  { 5486 /* pushl */, X86::PUSHES32, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 0},
  { 5486 /* pushl */, X86::PUSHFS32, Convert_NoOperands, Feature_In32BitMode, { MCK_FS }, 0},
  { 5486 /* pushl */, X86::PUSHGS32, Convert_NoOperands, Feature_In32BitMode, { MCK_GS }, 0},
  { 5486 /* pushl */, X86::PUSHi8, Convert__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8 }, 0},
  { 5486 /* pushl */, X86::PUSHi32, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 5486 /* pushl */, X86::PUSH32rmm, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5492 /* pushq */, X86::PUSH64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 5492 /* pushq */, X86::PUSH64rmr, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 5492 /* pushq */, X86::PUSHFS64, Convert_NoOperands, 0, { MCK_FS }, 0},
  { 5492 /* pushq */, X86::PUSHGS64, Convert_NoOperands, 0, { MCK_GS }, 0},
  { 5492 /* pushq */, X86::PUSH64i8, Convert__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8 }, 0},
  { 5492 /* pushq */, X86::PUSH64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32 }, 0},
  { 5492 /* pushq */, X86::PUSH64i16, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 5492 /* pushq */, X86::PUSH64rmm, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5498 /* pushw */, X86::PUSH16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 5498 /* pushw */, X86::PUSH16rmr, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 5498 /* pushw */, X86::PUSHCS16, Convert_NoOperands, Feature_In32BitMode, { MCK_CS }, 0},
  { 5498 /* pushw */, X86::PUSHDS16, Convert_NoOperands, Feature_In32BitMode, { MCK_DS }, 0},
  { 5498 /* pushw */, X86::PUSHSS16, Convert_NoOperands, Feature_In32BitMode, { MCK_SS }, 0},
  { 5498 /* pushw */, X86::PUSHES16, Convert_NoOperands, Feature_In32BitMode, { MCK_ES }, 0},
  { 5498 /* pushw */, X86::PUSHFS16, Convert_NoOperands, 0, { MCK_FS }, 0},
  { 5498 /* pushw */, X86::PUSHGS16, Convert_NoOperands, 0, { MCK_GS }, 0},
  { 5498 /* pushw */, X86::PUSHi16, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 5498 /* pushw */, X86::PUSH16rmm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5504 /* pxor */, X86::MMX_PXORirr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_VR64, MCK_VR64 }, 0},
  { 5504 /* pxor */, X86::MMX_PXORirr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_VR64, MCK_VR64 }, 1},
  { 5504 /* pxor */, X86::MMX_PXORirm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_VR64, MCK_Mem64 }, 1},
  { 5504 /* pxor */, X86::PXORrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5504 /* pxor */, X86::PXORrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5504 /* pxor */, X86::PXORrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5504 /* pxor */, X86::PXORrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5504 /* pxor */, X86::MMX_PXORirm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_VR64 }, 0},
  { 5509 /* rcl */, X86::RCL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 5509 /* rcl */, X86::RCL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 5509 /* rcl */, X86::RCL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 5509 /* rcl */, X86::RCL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 5509 /* rcl */, X86::RCL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5509 /* rcl */, X86::RCL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5509 /* rcl */, X86::RCL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5509 /* rcl */, X86::RCL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5509 /* rcl */, X86::RCL8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 5509 /* rcl */, X86::RCL8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 5509 /* rcl */, X86::RCL8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5513 /* rclb */, X86::RCL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 5513 /* rclb */, X86::RCL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5513 /* rclb */, X86::RCL8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 5513 /* rclb */, X86::RCL8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 5513 /* rclb */, X86::RCL8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5513 /* rclb */, X86::RCL8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5518 /* rcll */, X86::RCL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 5518 /* rcll */, X86::RCL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5518 /* rcll */, X86::RCL32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 5518 /* rcll */, X86::RCL32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 5518 /* rcll */, X86::RCL32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5518 /* rcll */, X86::RCL32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5523 /* rclq */, X86::RCL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 5523 /* rclq */, X86::RCL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5523 /* rclq */, X86::RCL64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 5523 /* rclq */, X86::RCL64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 5523 /* rclq */, X86::RCL64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 5523 /* rclq */, X86::RCL64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 5528 /* rclw */, X86::RCL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 5528 /* rclw */, X86::RCL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5528 /* rclw */, X86::RCL16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 5528 /* rclw */, X86::RCL16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 5528 /* rclw */, X86::RCL16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5528 /* rclw */, X86::RCL16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5533 /* rcpps */, X86::RCPPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5533 /* rcpps */, X86::RCPPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5533 /* rcpps */, X86::RCPPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5533 /* rcpps */, X86::RCPPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5539 /* rcpss */, X86::RCPSSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5539 /* rcpss */, X86::RCPSSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5539 /* rcpss */, X86::RCPSSm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 5539 /* rcpss */, X86::RCPSSm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 5545 /* rcr */, X86::RCR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 5545 /* rcr */, X86::RCR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 5545 /* rcr */, X86::RCR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 5545 /* rcr */, X86::RCR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 5545 /* rcr */, X86::RCR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5545 /* rcr */, X86::RCR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5545 /* rcr */, X86::RCR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5545 /* rcr */, X86::RCR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5545 /* rcr */, X86::RCR8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 5545 /* rcr */, X86::RCR8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 5545 /* rcr */, X86::RCR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5549 /* rcrb */, X86::RCR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 5549 /* rcrb */, X86::RCR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5549 /* rcrb */, X86::RCR8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 5549 /* rcrb */, X86::RCR8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 5549 /* rcrb */, X86::RCR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5549 /* rcrb */, X86::RCR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5554 /* rcrl */, X86::RCR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 5554 /* rcrl */, X86::RCR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5554 /* rcrl */, X86::RCR32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 5554 /* rcrl */, X86::RCR32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 5554 /* rcrl */, X86::RCR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5554 /* rcrl */, X86::RCR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5559 /* rcrq */, X86::RCR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 5559 /* rcrq */, X86::RCR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5559 /* rcrq */, X86::RCR64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 5559 /* rcrq */, X86::RCR64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 5559 /* rcrq */, X86::RCR64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 5559 /* rcrq */, X86::RCR64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 5564 /* rcrw */, X86::RCR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 5564 /* rcrw */, X86::RCR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5564 /* rcrw */, X86::RCR16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 5564 /* rcrw */, X86::RCR16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 5564 /* rcrw */, X86::RCR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5564 /* rcrw */, X86::RCR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5569 /* rdfsbase */, X86::RDFSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 5569 /* rdfsbase */, X86::RDFSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 5578 /* rdfsbasel */, X86::RDFSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 5588 /* rdfsbaseq */, X86::RDFSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 0},
  { 5598 /* rdgsbase */, X86::RDGSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 5598 /* rdgsbase */, X86::RDGSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 5607 /* rdgsbasel */, X86::RDGSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 5617 /* rdgsbaseq */, X86::RDGSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 0},
  { 5627 /* rdmsr */, X86::RDMSR, Convert_NoOperands, 0, {  }, 0},
  { 5627 /* rdmsr */, X86::RDMSR, Convert_NoOperands, 0, {  }, 1},
  { 5633 /* rdpmc */, X86::RDPMC, Convert_NoOperands, 0, {  }, 0},
  { 5633 /* rdpmc */, X86::RDPMC, Convert_NoOperands, 0, {  }, 1},
  { 5639 /* rdrand */, X86::RDRAND16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 5639 /* rdrand */, X86::RDRAND32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 5639 /* rdrand */, X86::RDRAND64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 5646 /* rdrandl */, X86::RDRAND32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 5654 /* rdrandq */, X86::RDRAND64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 5662 /* rdrandw */, X86::RDRAND16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 5670 /* rdtsc */, X86::RDTSC, Convert_NoOperands, 0, {  }, 0},
  { 5670 /* rdtsc */, X86::RDTSC, Convert_NoOperands, 0, {  }, 1},
  { 5676 /* rdtscp */, X86::RDTSCP, Convert_NoOperands, 0, {  }, 0},
  { 5676 /* rdtscp */, X86::RDTSCP, Convert_NoOperands, 0, {  }, 1},
  { 5683 /* rep */, X86::REP_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 5683 /* rep */, X86::REP_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 5687 /* repne */, X86::REPNE_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 5687 /* repne */, X86::REPNE_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 5693 /* ret */, X86::RET, Convert_NoOperands, 0, {  }, 0},
  { 5693 /* ret */, X86::RET, Convert_NoOperands, 0, {  }, 1},
  { 5693 /* ret */, X86::RETW, Convert_NoOperands, 0, {  }, 1},
  { 5693 /* ret */, X86::RETI, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 5693 /* ret */, X86::RETI, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5693 /* ret */, X86::RETIW, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5697 /* retf */, X86::LRETL, Convert_NoOperands, 0, {  }, 1},
  { 5697 /* retf */, X86::LRETQ, Convert_NoOperands, 0, {  }, 1},
  { 5697 /* retf */, X86::LRETW, Convert_NoOperands, 0, {  }, 1},
  { 5697 /* retf */, X86::LRETI, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5697 /* retf */, X86::LRETIW, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 5702 /* retw */, X86::RETW, Convert_NoOperands, 0, {  }, 0},
  { 5702 /* retw */, X86::RETIW, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 5707 /* rex64 */, X86::REX64_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 5707 /* rex64 */, X86::REX64_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 5713 /* rol */, X86::ROL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 5713 /* rol */, X86::ROL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 5713 /* rol */, X86::ROL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 5713 /* rol */, X86::ROL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 5713 /* rol */, X86::ROL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5713 /* rol */, X86::ROL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5713 /* rol */, X86::ROL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5713 /* rol */, X86::ROL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5713 /* rol */, X86::ROL8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK__PCT_cl }, 1},
  { 5713 /* rol */, X86::ROL64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 5713 /* rol */, X86::ROL8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 5713 /* rol */, X86::ROL8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5717 /* rolb */, X86::ROL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 5717 /* rolb */, X86::ROL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5717 /* rolb */, X86::ROL8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 5717 /* rolb */, X86::ROL8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 5717 /* rolb */, X86::ROL8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5717 /* rolb */, X86::ROL8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5722 /* roll */, X86::ROL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 5722 /* roll */, X86::ROL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5722 /* roll */, X86::ROL32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 5722 /* roll */, X86::ROL32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 5722 /* roll */, X86::ROL32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5722 /* roll */, X86::ROL32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5727 /* rolq */, X86::ROL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 5727 /* rolq */, X86::ROL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5727 /* rolq */, X86::ROL64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 5727 /* rolq */, X86::ROL64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 5727 /* rolq */, X86::ROL64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 5727 /* rolq */, X86::ROL64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 5732 /* rolw */, X86::ROL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 5732 /* rolw */, X86::ROL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5732 /* rolw */, X86::ROL16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 5732 /* rolw */, X86::ROL16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 5732 /* rolw */, X86::ROL16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5732 /* rolw */, X86::ROL16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5737 /* ror */, X86::ROR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 5737 /* ror */, X86::ROR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 5737 /* ror */, X86::ROR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 5737 /* ror */, X86::ROR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 5737 /* ror */, X86::ROR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5737 /* ror */, X86::ROR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5737 /* ror */, X86::ROR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5737 /* ror */, X86::ROR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5737 /* ror */, X86::ROR8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 5737 /* ror */, X86::ROR8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 5737 /* ror */, X86::ROR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5741 /* rorb */, X86::ROR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 5741 /* rorb */, X86::ROR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5741 /* rorb */, X86::ROR8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 5741 /* rorb */, X86::ROR8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 5741 /* rorb */, X86::ROR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5741 /* rorb */, X86::ROR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5746 /* rorl */, X86::ROR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 5746 /* rorl */, X86::ROR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5746 /* rorl */, X86::ROR32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 5746 /* rorl */, X86::ROR32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 5746 /* rorl */, X86::ROR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5746 /* rorl */, X86::ROR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5751 /* rorq */, X86::ROR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 5751 /* rorq */, X86::ROR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5751 /* rorq */, X86::ROR64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 5751 /* rorq */, X86::ROR64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 5751 /* rorq */, X86::ROR64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 5751 /* rorq */, X86::ROR64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 5756 /* rorw */, X86::ROR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 5756 /* rorw */, X86::ROR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5756 /* rorw */, X86::ROR16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 5756 /* rorw */, X86::ROR16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 5756 /* rorw */, X86::ROR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5756 /* rorw */, X86::ROR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5761 /* rorx */, X86::RORX32ri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_GR32, MCK_GR32, MCK_Imm }, 1},
  { 5761 /* rorx */, X86::RORX32mi, Convert__Reg1_0__Mem325_1__Imm1_2, 0, { MCK_GR32, MCK_Mem32, MCK_Imm }, 1},
  { 5761 /* rorx */, X86::RORX64ri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_GR64, MCK_GR64, MCK_Imm }, 1},
  { 5761 /* rorx */, X86::RORX64mi, Convert__Reg1_0__Mem645_1__Imm1_2, 0, { MCK_GR64, MCK_Mem64, MCK_Imm }, 1},
  { 5766 /* rorxl */, X86::RORX32ri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_GR32 }, 0},
  { 5766 /* rorxl */, X86::RORX32mi, Convert__Reg1_2__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32, MCK_GR32 }, 0},
  { 5772 /* rorxq */, X86::RORX64ri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64, MCK_GR64 }, 0},
  { 5772 /* rorxq */, X86::RORX64mi, Convert__Reg1_2__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64, MCK_GR64 }, 0},
  { 5778 /* roundpd */, X86::ROUNDPDr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5778 /* roundpd */, X86::ROUNDPDm, Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmSExti32i8 }, 1},
  { 5778 /* roundpd */, X86::ROUNDPDr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 5778 /* roundpd */, X86::ROUNDPDm, Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem128, MCK_FR32 }, 0},
  { 5786 /* roundps */, X86::ROUNDPSr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5786 /* roundps */, X86::ROUNDPSm, Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmSExti32i8 }, 1},
  { 5786 /* roundps */, X86::ROUNDPSr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 5786 /* roundps */, X86::ROUNDPSm, Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem128, MCK_FR32 }, 0},
  { 5794 /* roundsd */, X86::ROUNDSDr, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5794 /* roundsd */, X86::ROUNDSDm, Convert__Reg1_0__Tie0__Mem5_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem, MCK_ImmSExti32i8 }, 1},
  { 5794 /* roundsd */, X86::ROUNDSDr, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 5794 /* roundsd */, X86::ROUNDSDm, Convert__Reg1_2__Tie0__Mem5_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem, MCK_FR32 }, 0},
  { 5802 /* roundss */, X86::ROUNDSSr, Convert__Reg1_0__Tie0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 5802 /* roundss */, X86::ROUNDSSm, Convert__Reg1_0__Tie0__Mem5_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem, MCK_ImmSExti32i8 }, 1},
  { 5802 /* roundss */, X86::ROUNDSSr, Convert__Reg1_2__Tie0__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 5802 /* roundss */, X86::ROUNDSSm, Convert__Reg1_2__Tie0__Mem5_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem, MCK_FR32 }, 0},
  { 5810 /* rsm */, X86::RSM, Convert_NoOperands, 0, {  }, 0},
  { 5810 /* rsm */, X86::RSM, Convert_NoOperands, 0, {  }, 1},
  { 5814 /* rsqrtps */, X86::RSQRTPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5814 /* rsqrtps */, X86::RSQRTPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5814 /* rsqrtps */, X86::RSQRTPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 5814 /* rsqrtps */, X86::RSQRTPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 5822 /* rsqrtss */, X86::RSQRTSSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 5822 /* rsqrtss */, X86::RSQRTSSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 5822 /* rsqrtss */, X86::RSQRTSSm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 5822 /* rsqrtss */, X86::RSQRTSSm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 5830 /* sahf */, X86::SAHF, Convert_NoOperands, 0, {  }, 0},
  { 5830 /* sahf */, X86::SAHF, Convert_NoOperands, 0, {  }, 1},
  { 5835 /* sar */, X86::SAR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 5835 /* sar */, X86::SAR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 5835 /* sar */, X86::SAR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 5835 /* sar */, X86::SAR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 5835 /* sar */, X86::SAR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 5835 /* sar */, X86::SAR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 5835 /* sar */, X86::SAR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 5835 /* sar */, X86::SAR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5835 /* sar */, X86::SAR8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 5835 /* sar */, X86::SAR8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 5835 /* sar */, X86::SAR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5839 /* sarb */, X86::SAR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 5839 /* sarb */, X86::SAR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5839 /* sarb */, X86::SAR8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 5839 /* sarb */, X86::SAR8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 5839 /* sarb */, X86::SAR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5839 /* sarb */, X86::SAR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5844 /* sarl */, X86::SAR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 5844 /* sarl */, X86::SAR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 5844 /* sarl */, X86::SAR32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 5844 /* sarl */, X86::SAR32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 5844 /* sarl */, X86::SAR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5844 /* sarl */, X86::SAR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5849 /* sarq */, X86::SAR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 5849 /* sarq */, X86::SAR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 5849 /* sarq */, X86::SAR64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 5849 /* sarq */, X86::SAR64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 5849 /* sarq */, X86::SAR64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 5849 /* sarq */, X86::SAR64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 5854 /* sarw */, X86::SAR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 5854 /* sarw */, X86::SAR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 5854 /* sarw */, X86::SAR16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 5854 /* sarw */, X86::SAR16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 5854 /* sarw */, X86::SAR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5854 /* sarw */, X86::SAR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5859 /* sarx */, X86::SARX32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 5859 /* sarx */, X86::SARX32rm, Convert__Reg1_0__Mem325_1__Reg1_2, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 1},
  { 5859 /* sarx */, X86::SARX64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 5859 /* sarx */, X86::SARX64rm, Convert__Reg1_0__Mem645_1__Reg1_2, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 1},
  { 5864 /* sarxl */, X86::SARX32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 5864 /* sarxl */, X86::SARX32rm, Convert__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 0},
  { 5870 /* sarxq */, X86::SARX64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 5870 /* sarxq */, X86::SARX64rm, Convert__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 0},
  { 5876 /* sbb */, X86::SBB8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 5876 /* sbb */, X86::SBB8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 5876 /* sbb */, X86::SBB16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 5876 /* sbb */, X86::SBB16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 5876 /* sbb */, X86::SBB16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 5876 /* sbb */, X86::SBB32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 5876 /* sbb */, X86::SBB32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 5876 /* sbb */, X86::SBB32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 5876 /* sbb */, X86::SBB64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 5876 /* sbb */, X86::SBB64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 5876 /* sbb */, X86::SBB64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 5876 /* sbb */, X86::SBB64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 5876 /* sbb */, X86::SBB64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 5876 /* sbb */, X86::SBB16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 5876 /* sbb */, X86::SBB16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 5876 /* sbb */, X86::SBB16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 5876 /* sbb */, X86::SBB32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 5876 /* sbb */, X86::SBB32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 5876 /* sbb */, X86::SBB64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 5876 /* sbb */, X86::SBB64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 5876 /* sbb */, X86::SBB64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 5876 /* sbb */, X86::SBB8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 5876 /* sbb */, X86::SBB8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 5880 /* sbbb */, X86::SBB8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 5880 /* sbbb */, X86::SBB8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 5880 /* sbbb */, X86::SBB8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 5880 /* sbbb */, X86::SBB8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 5880 /* sbbb */, X86::SBB8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 5880 /* sbbb */, X86::SBB8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 5885 /* sbbl */, X86::SBB32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 5885 /* sbbl */, X86::SBB32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 5885 /* sbbl */, X86::SBB32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 5885 /* sbbl */, X86::SBB32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 5885 /* sbbl */, X86::SBB32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 5885 /* sbbl */, X86::SBB32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 5885 /* sbbl */, X86::SBB32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 5885 /* sbbl */, X86::SBB32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 5890 /* sbbq */, X86::SBB64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 5890 /* sbbq */, X86::SBB64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 5890 /* sbbq */, X86::SBB64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 5890 /* sbbq */, X86::SBB64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 5890 /* sbbq */, X86::SBB64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 5890 /* sbbq */, X86::SBB64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 5890 /* sbbq */, X86::SBB64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 5890 /* sbbq */, X86::SBB64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 5895 /* sbbw */, X86::SBB16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 5895 /* sbbw */, X86::SBB16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 5895 /* sbbw */, X86::SBB16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 5895 /* sbbw */, X86::SBB16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 5895 /* sbbw */, X86::SBB16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 5895 /* sbbw */, X86::SBB16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 5895 /* sbbw */, X86::SBB16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 5895 /* sbbw */, X86::SBB16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 5900 /* scasb */, X86::SCAS8, Convert_NoOperands, 0, {  }, 0},
  { 5900 /* scasb */, X86::SCAS8, Convert_NoOperands, 0, {  }, 1},
  { 5906 /* scasd */, X86::SCAS32, Convert_NoOperands, 0, {  }, 1},
  { 5912 /* scasl */, X86::SCAS32, Convert_NoOperands, 0, {  }, 0},
  { 5918 /* scasq */, X86::SCAS64, Convert_NoOperands, 0, {  }, 0},
  { 5918 /* scasq */, X86::SCAS64, Convert_NoOperands, 0, {  }, 1},
  { 5924 /* scasw */, X86::SCAS16, Convert_NoOperands, 0, {  }, 0},
  { 5924 /* scasw */, X86::SCAS16, Convert_NoOperands, 0, {  }, 1},
  { 5930 /* seta */, X86::SETAr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5930 /* seta */, X86::SETAr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5930 /* seta */, X86::SETAm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5930 /* seta */, X86::SETAm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5935 /* setae */, X86::SETAEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5935 /* setae */, X86::SETAEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5935 /* setae */, X86::SETAEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5935 /* setae */, X86::SETAEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5941 /* setb */, X86::SETBr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5941 /* setb */, X86::SETBr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5941 /* setb */, X86::SETBm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5941 /* setb */, X86::SETBm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5946 /* setbe */, X86::SETBEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5946 /* setbe */, X86::SETBEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5946 /* setbe */, X86::SETBEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5946 /* setbe */, X86::SETBEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5952 /* sete */, X86::SETEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5952 /* sete */, X86::SETEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5952 /* sete */, X86::SETEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5952 /* sete */, X86::SETEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5957 /* setg */, X86::SETGr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5957 /* setg */, X86::SETGr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5957 /* setg */, X86::SETGm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5957 /* setg */, X86::SETGm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5962 /* setge */, X86::SETGEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5962 /* setge */, X86::SETGEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5962 /* setge */, X86::SETGEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5962 /* setge */, X86::SETGEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5968 /* setl */, X86::SETLr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5968 /* setl */, X86::SETLr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5968 /* setl */, X86::SETLm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5968 /* setl */, X86::SETLm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5973 /* setle */, X86::SETLEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5973 /* setle */, X86::SETLEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5973 /* setle */, X86::SETLEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5973 /* setle */, X86::SETLEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5979 /* setne */, X86::SETNEr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5979 /* setne */, X86::SETNEr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5979 /* setne */, X86::SETNEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5979 /* setne */, X86::SETNEm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5985 /* setno */, X86::SETNOr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5985 /* setno */, X86::SETNOr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5985 /* setno */, X86::SETNOm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5985 /* setno */, X86::SETNOm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5991 /* setnp */, X86::SETNPr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5991 /* setnp */, X86::SETNPr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5991 /* setnp */, X86::SETNPm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5991 /* setnp */, X86::SETNPm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 5997 /* setns */, X86::SETNSr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 5997 /* setns */, X86::SETNSr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 5997 /* setns */, X86::SETNSm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 5997 /* setns */, X86::SETNSm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6003 /* seto */, X86::SETOr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 6003 /* seto */, X86::SETOr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 6003 /* seto */, X86::SETOm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 6003 /* seto */, X86::SETOm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6008 /* setp */, X86::SETPr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 6008 /* setp */, X86::SETPr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 6008 /* setp */, X86::SETPm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 6008 /* setp */, X86::SETPm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6013 /* sets */, X86::SETSr, Convert__Reg1_0, 0, { MCK_GR8 }, 0},
  { 6013 /* sets */, X86::SETSr, Convert__Reg1_0, 0, { MCK_GR8 }, 1},
  { 6013 /* sets */, X86::SETSm, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 6013 /* sets */, X86::SETSm, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6018 /* sfence */, X86::SFENCE, Convert_NoOperands, 0, {  }, 0},
  { 6018 /* sfence */, X86::SFENCE, Convert_NoOperands, 0, {  }, 1},
  { 6025 /* sgdt */, X86::SGDTm, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 6025 /* sgdt */, X86::SGDTm, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 6030 /* sgdtw */, X86::SGDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 0},
  { 6030 /* sgdtw */, X86::SGDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 1},
  { 6036 /* shl */, X86::SHL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 6036 /* shl */, X86::SHL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 6036 /* shl */, X86::SHL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 6036 /* shl */, X86::SHL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 6036 /* shl */, X86::SHL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6036 /* shl */, X86::SHL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 6036 /* shl */, X86::SHL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 6036 /* shl */, X86::SHL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6036 /* shl */, X86::SHL8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 6036 /* shl */, X86::SHL8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 6036 /* shl */, X86::SHL8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 6040 /* shlb */, X86::SHL8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 6040 /* shlb */, X86::SHL8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 6040 /* shlb */, X86::SHL8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 6040 /* shlb */, X86::SHL8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 6040 /* shlb */, X86::SHL8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 6040 /* shlb */, X86::SHL8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 6045 /* shld */, X86::SHLD16rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD16rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR16, MCK_GR16, MCK_Imm }, 1},
  { 6045 /* shld */, X86::SHLD32rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD32rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR32, MCK_GR32, MCK_Imm }, 1},
  { 6045 /* shld */, X86::SHLD64rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD64rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR64, MCK_GR64, MCK_Imm }, 1},
  { 6045 /* shld */, X86::SHLD16mrCL, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD16mri8, Convert__Mem165_0__Reg1_1__Imm1_2, 0, { MCK_Mem16, MCK_GR16, MCK_Imm }, 1},
  { 6045 /* shld */, X86::SHLD32mrCL, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD32mri8, Convert__Mem325_0__Reg1_1__Imm1_2, 0, { MCK_Mem32, MCK_GR32, MCK_Imm }, 1},
  { 6045 /* shld */, X86::SHLD64mrCL, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64, MCK_CL }, 1},
  { 6045 /* shld */, X86::SHLD64mri8, Convert__Mem645_0__Reg1_1__Imm1_2, 0, { MCK_Mem64, MCK_GR64, MCK_Imm }, 1},
  { 6050 /* shldl */, X86::SHLD32rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 6050 /* shldl */, X86::SHLD32rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 6050 /* shldl */, X86::SHLD32mrCL, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 6050 /* shldl */, X86::SHLD32mrCL, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6050 /* shldl */, X86::SHLD32rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR32, MCK_GR32 }, 0},
  { 6050 /* shldl */, X86::SHLD32mrCL, Convert__Mem325_2__Reg1_1, 0, { MCK_CL, MCK_GR32, MCK_Mem32 }, 0},
  { 6050 /* shldl */, X86::SHLD32rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_GR32 }, 0},
  { 6050 /* shldl */, X86::SHLD32mri8, Convert__Mem325_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_Mem32 }, 0},
  { 6056 /* shldq */, X86::SHLD64rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 6056 /* shldq */, X86::SHLD64rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 6056 /* shldq */, X86::SHLD64mrCL, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 6056 /* shldq */, X86::SHLD64mrCL, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6056 /* shldq */, X86::SHLD64rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR64, MCK_GR64 }, 0},
  { 6056 /* shldq */, X86::SHLD64mrCL, Convert__Mem645_2__Reg1_1, 0, { MCK_CL, MCK_GR64, MCK_Mem64 }, 0},
  { 6056 /* shldq */, X86::SHLD64rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64, MCK_GR64 }, 0},
  { 6056 /* shldq */, X86::SHLD64mri8, Convert__Mem645_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64, MCK_Mem64 }, 0},
  { 6062 /* shldw */, X86::SHLD16rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 6062 /* shldw */, X86::SHLD16rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 6062 /* shldw */, X86::SHLD16mrCL, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 6062 /* shldw */, X86::SHLD16mrCL, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6062 /* shldw */, X86::SHLD16rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR16, MCK_GR16 }, 0},
  { 6062 /* shldw */, X86::SHLD16mrCL, Convert__Mem165_2__Reg1_1, 0, { MCK_CL, MCK_GR16, MCK_Mem16 }, 0},
  { 6062 /* shldw */, X86::SHLD16rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16, MCK_GR16 }, 0},
  { 6062 /* shldw */, X86::SHLD16mri8, Convert__Mem165_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16, MCK_Mem16 }, 0},
  { 6068 /* shll */, X86::SHL32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 6068 /* shll */, X86::SHL32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 6068 /* shll */, X86::SHL32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 6068 /* shll */, X86::SHL32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 6068 /* shll */, X86::SHL32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 6068 /* shll */, X86::SHL32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 6073 /* shlq */, X86::SHL64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 6073 /* shlq */, X86::SHL64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 6073 /* shlq */, X86::SHL64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 6073 /* shlq */, X86::SHL64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 6073 /* shlq */, X86::SHL64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 6073 /* shlq */, X86::SHL64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 6078 /* shlw */, X86::SHL16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 6078 /* shlw */, X86::SHL16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6078 /* shlw */, X86::SHL16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 6078 /* shlw */, X86::SHL16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 6078 /* shlw */, X86::SHL16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 6078 /* shlw */, X86::SHL16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 6083 /* shlx */, X86::SHLX32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 6083 /* shlx */, X86::SHLX32rm, Convert__Reg1_0__Mem325_1__Reg1_2, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 1},
  { 6083 /* shlx */, X86::SHLX64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 6083 /* shlx */, X86::SHLX64rm, Convert__Reg1_0__Mem645_1__Reg1_2, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 1},
  { 6088 /* shlxl */, X86::SHLX32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 6088 /* shlxl */, X86::SHLX32rm, Convert__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 0},
  { 6094 /* shlxq */, X86::SHLX64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 6094 /* shlxq */, X86::SHLX64rm, Convert__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 0},
  { 6100 /* shr */, X86::SHR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 1},
  { 6100 /* shr */, X86::SHR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 1},
  { 6100 /* shr */, X86::SHR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 1},
  { 6100 /* shr */, X86::SHR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 1},
  { 6100 /* shr */, X86::SHR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6100 /* shr */, X86::SHR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 6100 /* shr */, X86::SHR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 6100 /* shr */, X86::SHR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 1},
  { 6100 /* shr */, X86::SHR8rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR8, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR16rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR16, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR32rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR32, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR64rCL, Convert__Reg1_0__Tie0, 0, { MCK_GR64, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR64ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR64, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR16mCL, Convert__Mem165_0, 0, { MCK_Mem16, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR32mCL, Convert__Mem325_0, 0, { MCK_Mem32, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR64mCL, Convert__Mem645_0, 0, { MCK_Mem64, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR64mi, Convert__Mem645_0__Imm1_1, 0, { MCK_Mem64, MCK_Imm }, 1},
  { 6100 /* shr */, X86::SHR8mCL, Convert__Mem85_0, 0, { MCK_Mem8, MCK_CL }, 1},
  { 6100 /* shr */, X86::SHR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 6104 /* shrb */, X86::SHR8r1, Convert__Reg1_0__Tie0, 0, { MCK_GR8 }, 0},
  { 6104 /* shrb */, X86::SHR8m1, Convert__Mem85_0, 0, { MCK_Mem8 }, 0},
  { 6104 /* shrb */, X86::SHR8rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR8 }, 0},
  { 6104 /* shrb */, X86::SHR8mCL, Convert__Mem85_1, 0, { MCK_CL, MCK_Mem8 }, 0},
  { 6104 /* shrb */, X86::SHR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 6104 /* shrb */, X86::SHR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 6109 /* shrd */, X86::SHRD16rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD16rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR16, MCK_GR16, MCK_Imm }, 1},
  { 6109 /* shrd */, X86::SHRD32rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD32rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR32, MCK_GR32, MCK_Imm }, 1},
  { 6109 /* shrd */, X86::SHRD64rrCL, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD64rri8, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_GR64, MCK_GR64, MCK_Imm }, 1},
  { 6109 /* shrd */, X86::SHRD16mrCL, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD16mri8, Convert__Mem165_0__Reg1_1__Imm1_2, 0, { MCK_Mem16, MCK_GR16, MCK_Imm }, 1},
  { 6109 /* shrd */, X86::SHRD32mrCL, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD32mri8, Convert__Mem325_0__Reg1_1__Imm1_2, 0, { MCK_Mem32, MCK_GR32, MCK_Imm }, 1},
  { 6109 /* shrd */, X86::SHRD64mrCL, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64, MCK_CL }, 1},
  { 6109 /* shrd */, X86::SHRD64mri8, Convert__Mem645_0__Reg1_1__Imm1_2, 0, { MCK_Mem64, MCK_GR64, MCK_Imm }, 1},
  { 6114 /* shrdl */, X86::SHRD32rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 6114 /* shrdl */, X86::SHRD32rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 6114 /* shrdl */, X86::SHRD32mrCL, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 6114 /* shrdl */, X86::SHRD32mrCL, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6114 /* shrdl */, X86::SHRD32rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR32, MCK_GR32 }, 0},
  { 6114 /* shrdl */, X86::SHRD32mrCL, Convert__Mem325_2__Reg1_1, 0, { MCK_CL, MCK_GR32, MCK_Mem32 }, 0},
  { 6114 /* shrdl */, X86::SHRD32rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_GR32 }, 0},
  { 6114 /* shrdl */, X86::SHRD32mri8, Convert__Mem325_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32, MCK_Mem32 }, 0},
  { 6120 /* shrdq */, X86::SHRD64rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 6120 /* shrdq */, X86::SHRD64rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 6120 /* shrdq */, X86::SHRD64mrCL, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 6120 /* shrdq */, X86::SHRD64mrCL, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6120 /* shrdq */, X86::SHRD64rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR64, MCK_GR64 }, 0},
  { 6120 /* shrdq */, X86::SHRD64mrCL, Convert__Mem645_2__Reg1_1, 0, { MCK_CL, MCK_GR64, MCK_Mem64 }, 0},
  { 6120 /* shrdq */, X86::SHRD64rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64, MCK_GR64 }, 0},
  { 6120 /* shrdq */, X86::SHRD64mri8, Convert__Mem645_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR64, MCK_Mem64 }, 0},
  { 6126 /* shrdw */, X86::SHRD16rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 6126 /* shrdw */, X86::SHRD16rrCL, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 6126 /* shrdw */, X86::SHRD16mrCL, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 6126 /* shrdw */, X86::SHRD16mrCL, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6126 /* shrdw */, X86::SHRD16rrCL, Convert__Reg1_2__Tie0__Reg1_1, 0, { MCK_CL, MCK_GR16, MCK_GR16 }, 0},
  { 6126 /* shrdw */, X86::SHRD16mrCL, Convert__Mem165_2__Reg1_1, 0, { MCK_CL, MCK_GR16, MCK_Mem16 }, 0},
  { 6126 /* shrdw */, X86::SHRD16rri8, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16, MCK_GR16 }, 0},
  { 6126 /* shrdw */, X86::SHRD16mri8, Convert__Mem165_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16, MCK_Mem16 }, 0},
  { 6132 /* shrl */, X86::SHR32r1, Convert__Reg1_0__Tie0, 0, { MCK_GR32 }, 0},
  { 6132 /* shrl */, X86::SHR32m1, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 6132 /* shrl */, X86::SHR32rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR32 }, 0},
  { 6132 /* shrl */, X86::SHR32mCL, Convert__Mem325_1, 0, { MCK_CL, MCK_Mem32 }, 0},
  { 6132 /* shrl */, X86::SHR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 6132 /* shrl */, X86::SHR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 6137 /* shrq */, X86::SHR64r1, Convert__Reg1_0__Tie0, 0, { MCK_GR64 }, 0},
  { 6137 /* shrq */, X86::SHR64m1, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 6137 /* shrq */, X86::SHR64rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR64 }, 0},
  { 6137 /* shrq */, X86::SHR64mCL, Convert__Mem645_1, 0, { MCK_CL, MCK_Mem64 }, 0},
  { 6137 /* shrq */, X86::SHR64ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR64 }, 0},
  { 6137 /* shrq */, X86::SHR64mi, Convert__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64 }, 0},
  { 6142 /* shrw */, X86::SHR16r1, Convert__Reg1_0__Tie0, 0, { MCK_GR16 }, 0},
  { 6142 /* shrw */, X86::SHR16m1, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6142 /* shrw */, X86::SHR16rCL, Convert__Reg1_1__Tie0, 0, { MCK_CL, MCK_GR16 }, 0},
  { 6142 /* shrw */, X86::SHR16mCL, Convert__Mem165_1, 0, { MCK_CL, MCK_Mem16 }, 0},
  { 6142 /* shrw */, X86::SHR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 6142 /* shrw */, X86::SHR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 6147 /* shrx */, X86::SHRX32rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 1},
  { 6147 /* shrx */, X86::SHRX32rm, Convert__Reg1_0__Mem325_1__Reg1_2, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 1},
  { 6147 /* shrx */, X86::SHRX64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 1},
  { 6147 /* shrx */, X86::SHRX64rm, Convert__Reg1_0__Mem645_1__Reg1_2, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 1},
  { 6152 /* shrxl */, X86::SHRX32rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32, MCK_GR32 }, 0},
  { 6152 /* shrxl */, X86::SHRX32rm, Convert__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32, MCK_GR32 }, 0},
  { 6158 /* shrxq */, X86::SHRX64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64, MCK_GR64 }, 0},
  { 6158 /* shrxq */, X86::SHRX64rm, Convert__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64, MCK_GR64 }, 0},
  { 6164 /* shufpd */, X86::SHUFPDrri, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6164 /* shufpd */, X86::SHUFPDrmi, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 6164 /* shufpd */, X86::SHUFPDrri, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 6164 /* shufpd */, X86::SHUFPDrmi, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 6171 /* shufps */, X86::SHUFPSrri, Convert__Reg1_0__Tie0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6171 /* shufps */, X86::SHUFPSrmi, Convert__Reg1_0__Tie0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 6171 /* shufps */, X86::SHUFPSrri, Convert__Reg1_2__Tie0__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 6171 /* shufps */, X86::SHUFPSrmi, Convert__Reg1_2__Tie0__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 6178 /* sidt */, X86::SIDTm, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 6178 /* sidt */, X86::SIDTm, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 6183 /* sidtw */, X86::SIDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 0},
  { 6183 /* sidtw */, X86::SIDT16m, Convert__Mem5_0, Feature_In32BitMode, { MCK_Mem }, 1},
  { 6189 /* skinit */, X86::SKINIT, Convert_NoOperands, 0, { MCK_EAX }, 0},
  { 6189 /* skinit */, X86::SKINIT, Convert_NoOperands, 0, { MCK_EAX }, 1},
  { 6196 /* sldt */, X86::SLDT16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 6196 /* sldt */, X86::SLDT32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 6196 /* sldt */, X86::SLDT64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 6196 /* sldt */, X86::SLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6196 /* sldt */, X86::SLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6196 /* sldt */, X86::SLDT64m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6196 /* sldt */, X86::SLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6201 /* sldtl */, X86::SLDT32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 6207 /* sldtq */, X86::SLDT64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 6207 /* sldtq */, X86::SLDT64m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6213 /* sldtw */, X86::SLDT16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 6213 /* sldtw */, X86::SLDT16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6219 /* smsw */, X86::SMSW16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 6219 /* smsw */, X86::SMSW32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 6219 /* smsw */, X86::SMSW64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 6219 /* smsw */, X86::SMSW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6224 /* smswl */, X86::SMSW32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 6230 /* smswq */, X86::SMSW64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 6236 /* smsww */, X86::SMSW16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 6236 /* smsww */, X86::SMSW16m, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6242 /* sqrtpd */, X86::SQRTPDr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6242 /* sqrtpd */, X86::SQRTPDr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6242 /* sqrtpd */, X86::SQRTPDm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6242 /* sqrtpd */, X86::SQRTPDm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6249 /* sqrtps */, X86::SQRTPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6249 /* sqrtps */, X86::SQRTPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6249 /* sqrtps */, X86::SQRTPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6249 /* sqrtps */, X86::SQRTPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6256 /* sqrtsd */, X86::SQRTSDr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6256 /* sqrtsd */, X86::SQRTSDr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6256 /* sqrtsd */, X86::SQRTSDm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6256 /* sqrtsd */, X86::SQRTSDm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6263 /* sqrtss */, X86::SQRTSSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6263 /* sqrtss */, X86::SQRTSSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6263 /* sqrtss */, X86::SQRTSSm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 6263 /* sqrtss */, X86::SQRTSSm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 6270 /* ss */, X86::SS_PREFIX, Convert_NoOperands, 0, {  }, 0},
  { 6270 /* ss */, X86::SS_PREFIX, Convert_NoOperands, 0, {  }, 1},
  { 6273 /* stc */, X86::STC, Convert_NoOperands, 0, {  }, 0},
  { 6273 /* stc */, X86::STC, Convert_NoOperands, 0, {  }, 1},
  { 6277 /* std */, X86::STD, Convert_NoOperands, 0, {  }, 0},
  { 6277 /* std */, X86::STD, Convert_NoOperands, 0, {  }, 1},
  { 6281 /* stgi */, X86::STGI, Convert_NoOperands, 0, {  }, 0},
  { 6281 /* stgi */, X86::STGI, Convert_NoOperands, 0, {  }, 1},
  { 6286 /* sti */, X86::STI, Convert_NoOperands, 0, {  }, 0},
  { 6286 /* sti */, X86::STI, Convert_NoOperands, 0, {  }, 1},
  { 6290 /* stmxcsr */, X86::STMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 6290 /* stmxcsr */, X86::STMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 6298 /* stosb */, X86::STOSB, Convert_NoOperands, 0, {  }, 0},
  { 6298 /* stosb */, X86::STOSB, Convert_NoOperands, 0, {  }, 1},
  { 6304 /* stosd */, X86::STOSD, Convert_NoOperands, 0, {  }, 1},
  { 6310 /* stosl */, X86::STOSD, Convert_NoOperands, 0, {  }, 0},
  { 6316 /* stosq */, X86::STOSQ, Convert_NoOperands, 0, {  }, 0},
  { 6316 /* stosq */, X86::STOSQ, Convert_NoOperands, 0, {  }, 1},
  { 6322 /* stosw */, X86::STOSW, Convert_NoOperands, 0, {  }, 0},
  { 6322 /* stosw */, X86::STOSW, Convert_NoOperands, 0, {  }, 1},
  { 6328 /* str */, X86::STR16r, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 6328 /* str */, X86::STR32r, Convert__Reg1_0, 0, { MCK_GR32 }, 1},
  { 6328 /* str */, X86::STR64r, Convert__Reg1_0, 0, { MCK_GR64 }, 1},
  { 6328 /* str */, X86::STRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 6332 /* strl */, X86::STR32r, Convert__Reg1_0, 0, { MCK_GR32 }, 0},
  { 6337 /* strq */, X86::STR64r, Convert__Reg1_0, 0, { MCK_GR64 }, 0},
  { 6342 /* strw */, X86::STR16r, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 6342 /* strw */, X86::STRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 6347 /* sub */, X86::SUB8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 6347 /* sub */, X86::SUB8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 6347 /* sub */, X86::SUB16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 6347 /* sub */, X86::SUB16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 6347 /* sub */, X86::SUB16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6347 /* sub */, X86::SUB32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 6347 /* sub */, X86::SUB32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 6347 /* sub */, X86::SUB32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6347 /* sub */, X86::SUB64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 6347 /* sub */, X86::SUB64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 6347 /* sub */, X86::SUB64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 6347 /* sub */, X86::SUB64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 6347 /* sub */, X86::SUB64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6347 /* sub */, X86::SUB16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 6347 /* sub */, X86::SUB16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 6347 /* sub */, X86::SUB16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 6347 /* sub */, X86::SUB32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 6347 /* sub */, X86::SUB32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 6347 /* sub */, X86::SUB64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 6347 /* sub */, X86::SUB64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 6347 /* sub */, X86::SUB64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 6347 /* sub */, X86::SUB8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 6347 /* sub */, X86::SUB8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 6351 /* subb */, X86::SUB8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 6351 /* subb */, X86::SUB8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 6351 /* subb */, X86::SUB8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 6351 /* subb */, X86::SUB8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 6351 /* subb */, X86::SUB8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 6351 /* subb */, X86::SUB8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 6356 /* subl */, X86::SUB32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 6356 /* subl */, X86::SUB32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 6356 /* subl */, X86::SUB32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 6356 /* subl */, X86::SUB32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 6356 /* subl */, X86::SUB32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 6356 /* subl */, X86::SUB32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 6356 /* subl */, X86::SUB32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 6356 /* subl */, X86::SUB32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 6361 /* subpd */, X86::SUBPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6361 /* subpd */, X86::SUBPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6361 /* subpd */, X86::SUBPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6361 /* subpd */, X86::SUBPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6367 /* subps */, X86::SUBPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6367 /* subps */, X86::SUBPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6367 /* subps */, X86::SUBPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6367 /* subps */, X86::SUBPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6373 /* subq */, X86::SUB64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 6373 /* subq */, X86::SUB64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 6373 /* subq */, X86::SUB64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 6373 /* subq */, X86::SUB64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 6373 /* subq */, X86::SUB64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 6373 /* subq */, X86::SUB64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 6373 /* subq */, X86::SUB64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 6373 /* subq */, X86::SUB64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 6378 /* subsd */, X86::SUBSDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6378 /* subsd */, X86::SUBSDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6378 /* subsd */, X86::SUBSDrm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6378 /* subsd */, X86::SUBSDrm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6384 /* subss */, X86::SUBSSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6384 /* subss */, X86::SUBSSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6384 /* subss */, X86::SUBSSrm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 6384 /* subss */, X86::SUBSSrm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 6390 /* subw */, X86::SUB16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 6390 /* subw */, X86::SUB16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 6390 /* subw */, X86::SUB16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 6390 /* subw */, X86::SUB16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 6390 /* subw */, X86::SUB16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 6390 /* subw */, X86::SUB16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 6390 /* subw */, X86::SUB16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 6390 /* subw */, X86::SUB16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 6395 /* swapgs */, X86::SWAPGS, Convert_NoOperands, 0, {  }, 0},
  { 6395 /* swapgs */, X86::SWAPGS, Convert_NoOperands, 0, {  }, 1},
  { 6402 /* syscall */, X86::SYSCALL, Convert_NoOperands, 0, {  }, 0},
  { 6402 /* syscall */, X86::SYSCALL, Convert_NoOperands, 0, {  }, 1},
  { 6410 /* sysenter */, X86::SYSENTER, Convert_NoOperands, 0, {  }, 0},
  { 6410 /* sysenter */, X86::SYSENTER, Convert_NoOperands, 0, {  }, 1},
  { 6419 /* sysexit */, X86::SYSEXIT64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 6419 /* sysexit */, X86::SYSEXIT, Convert_NoOperands, 0, {  }, 1},
  { 6427 /* sysexitl */, X86::SYSEXIT, Convert_NoOperands, 0, {  }, 0},
  { 6436 /* sysexitq */, X86::SYSEXIT64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 6445 /* sysret */, X86::SYSRET64, Convert_NoOperands, Feature_In64BitMode, {  }, 1},
  { 6445 /* sysret */, X86::SYSRET, Convert_NoOperands, 0, {  }, 1},
  { 6452 /* sysretl */, X86::SYSRET, Convert_NoOperands, 0, {  }, 0},
  { 6460 /* sysretq */, X86::SYSRET64, Convert_NoOperands, Feature_In64BitMode, {  }, 0},
  { 6468 /* test */, X86::TEST8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST8rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 6468 /* test */, X86::TEST8ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST8rm, Convert__Reg1_0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 6468 /* test */, X86::TEST16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 6468 /* test */, X86::TEST16ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6468 /* test */, X86::TEST32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 6468 /* test */, X86::TEST32ri, Convert__Reg1_0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6468 /* test */, X86::TEST64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 6468 /* test */, X86::TEST64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 6468 /* test */, X86::TEST64ri32, Convert__Reg1_0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 6468 /* test */, X86::TEST64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6468 /* test */, X86::TEST16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 6468 /* test */, X86::TEST64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 6468 /* test */, X86::TEST8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 6473 /* testb */, X86::TEST8rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 6473 /* testb */, X86::TEST8rm, Convert__Reg1_0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 6473 /* testb */, X86::TEST8rm, Convert__Reg1_0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 6473 /* testb */, X86::TEST8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 6473 /* testb */, X86::TEST8ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 6473 /* testb */, X86::TEST8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 6473 /* testb */, X86::TEST8rm, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 6479 /* testl */, X86::TEST32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 6479 /* testl */, X86::TEST32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 6479 /* testl */, X86::TEST32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6479 /* testl */, X86::TEST32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 6479 /* testl */, X86::TEST32ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 6479 /* testl */, X86::TEST32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 6479 /* testl */, X86::TEST32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 6485 /* testq */, X86::TEST64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 6485 /* testq */, X86::TEST64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 6485 /* testq */, X86::TEST64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6485 /* testq */, X86::TEST64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 6485 /* testq */, X86::TEST64ri32, Convert__Reg1_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 6485 /* testq */, X86::TEST64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 6485 /* testq */, X86::TEST64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 6491 /* testw */, X86::TEST16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 6491 /* testw */, X86::TEST16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 6491 /* testw */, X86::TEST16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6491 /* testw */, X86::TEST16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 6491 /* testw */, X86::TEST16ri, Convert__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 6491 /* testw */, X86::TEST16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 6491 /* testw */, X86::TEST16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 6497 /* tzcnt */, X86::TZCNT16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 6497 /* tzcnt */, X86::TZCNT16rm, Convert__Reg1_0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 6497 /* tzcnt */, X86::TZCNT32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 6497 /* tzcnt */, X86::TZCNT32rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 6497 /* tzcnt */, X86::TZCNT64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 6497 /* tzcnt */, X86::TZCNT64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 6503 /* tzcntl */, X86::TZCNT32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 6503 /* tzcntl */, X86::TZCNT32rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 6510 /* tzcntq */, X86::TZCNT64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 6510 /* tzcntq */, X86::TZCNT64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 6517 /* tzcntw */, X86::TZCNT16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 6517 /* tzcntw */, X86::TZCNT16rm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 6524 /* ucomisd */, X86::UCOMISDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6524 /* ucomisd */, X86::UCOMISDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6524 /* ucomisd */, X86::UCOMISDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6524 /* ucomisd */, X86::UCOMISDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6532 /* ucomiss */, X86::UCOMISSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6532 /* ucomiss */, X86::UCOMISSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6532 /* ucomiss */, X86::UCOMISSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 6532 /* ucomiss */, X86::UCOMISSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 6540 /* ud2 */, X86::TRAP, Convert_NoOperands, 0, {  }, 0},
  { 6540 /* ud2 */, X86::TRAP, Convert_NoOperands, 0, {  }, 1},
  { 6544 /* ud2b */, X86::UD2B, Convert_NoOperands, 0, {  }, 0},
  { 6544 /* ud2b */, X86::UD2B, Convert_NoOperands, 0, {  }, 1},
  { 6549 /* unpckhpd */, X86::UNPCKHPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6549 /* unpckhpd */, X86::UNPCKHPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6549 /* unpckhpd */, X86::UNPCKHPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6549 /* unpckhpd */, X86::UNPCKHPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6558 /* unpckhps */, X86::UNPCKHPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6558 /* unpckhps */, X86::UNPCKHPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6558 /* unpckhps */, X86::UNPCKHPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6558 /* unpckhps */, X86::UNPCKHPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6567 /* unpcklpd */, X86::UNPCKLPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6567 /* unpcklpd */, X86::UNPCKLPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6567 /* unpcklpd */, X86::UNPCKLPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6567 /* unpcklpd */, X86::UNPCKLPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6576 /* unpcklps */, X86::UNPCKLPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6576 /* unpcklps */, X86::UNPCKLPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6576 /* unpcklps */, X86::UNPCKLPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6576 /* unpcklps */, X86::UNPCKLPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6585 /* vaddpd */, X86::VADDPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6585 /* vaddpd */, X86::VADDPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6585 /* vaddpd */, X86::VADDPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6585 /* vaddpd */, X86::VADDPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6585 /* vaddpd */, X86::VADDPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6585 /* vaddpd */, X86::VADDPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6585 /* vaddpd */, X86::VADDPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6585 /* vaddpd */, X86::VADDPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6592 /* vaddps */, X86::VADDPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6592 /* vaddps */, X86::VADDPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6592 /* vaddps */, X86::VADDPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6592 /* vaddps */, X86::VADDPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6592 /* vaddps */, X86::VADDPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6592 /* vaddps */, X86::VADDPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6592 /* vaddps */, X86::VADDPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6592 /* vaddps */, X86::VADDPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6599 /* vaddsd */, X86::VADDSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6599 /* vaddsd */, X86::VADDSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6599 /* vaddsd */, X86::VADDSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 6599 /* vaddsd */, X86::VADDSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 6606 /* vaddss */, X86::VADDSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6606 /* vaddss */, X86::VADDSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6606 /* vaddss */, X86::VADDSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 6606 /* vaddss */, X86::VADDSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6613 /* vaddsubpd */, X86::VADDSUBPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6623 /* vaddsubps */, X86::VADDSUBPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6623 /* vaddsubps */, X86::VADDSUBPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6623 /* vaddsubps */, X86::VADDSUBPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6623 /* vaddsubps */, X86::VADDSUBPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6623 /* vaddsubps */, X86::VADDSUBPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6623 /* vaddsubps */, X86::VADDSUBPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6623 /* vaddsubps */, X86::VADDSUBPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6623 /* vaddsubps */, X86::VADDSUBPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6633 /* vaesdec */, X86::VAESDECrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6633 /* vaesdec */, X86::VAESDECrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6633 /* vaesdec */, X86::VAESDECrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6633 /* vaesdec */, X86::VAESDECrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6641 /* vaesdeclast */, X86::VAESDECLASTrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6641 /* vaesdeclast */, X86::VAESDECLASTrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6641 /* vaesdeclast */, X86::VAESDECLASTrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6641 /* vaesdeclast */, X86::VAESDECLASTrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6653 /* vaesenc */, X86::VAESENCrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6653 /* vaesenc */, X86::VAESENCrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6653 /* vaesenc */, X86::VAESENCrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6653 /* vaesenc */, X86::VAESENCrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6661 /* vaesenclast */, X86::VAESENCLASTrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6661 /* vaesenclast */, X86::VAESENCLASTrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6661 /* vaesenclast */, X86::VAESENCLASTrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6661 /* vaesenclast */, X86::VAESENCLASTrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6673 /* vaesimc */, X86::VAESIMCrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6673 /* vaesimc */, X86::VAESIMCrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6673 /* vaesimc */, X86::VAESIMCrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6673 /* vaesimc */, X86::VAESIMCrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6681 /* vaeskeygenassist */, X86::VAESKEYGENASSIST128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6681 /* vaeskeygenassist */, X86::VAESKEYGENASSIST128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 6681 /* vaeskeygenassist */, X86::VAESKEYGENASSIST128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 6681 /* vaeskeygenassist */, X86::VAESKEYGENASSIST128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 6698 /* vandnpd */, X86::VANDNPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6698 /* vandnpd */, X86::VFsANDNPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6698 /* vandnpd */, X86::VANDNPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6698 /* vandnpd */, X86::VFsANDNPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6698 /* vandnpd */, X86::VANDNPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6698 /* vandnpd */, X86::VFsANDNPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6698 /* vandnpd */, X86::VANDNPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6698 /* vandnpd */, X86::VANDNPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6698 /* vandnpd */, X86::VANDNPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6698 /* vandnpd */, X86::VANDNPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6698 /* vandnpd */, X86::VFsANDNPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6698 /* vandnpd */, X86::VANDNPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6706 /* vandnps */, X86::VANDNPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6706 /* vandnps */, X86::VFsANDNPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6706 /* vandnps */, X86::VANDNPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6706 /* vandnps */, X86::VFsANDNPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6706 /* vandnps */, X86::VANDNPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6706 /* vandnps */, X86::VFsANDNPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6706 /* vandnps */, X86::VANDNPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6706 /* vandnps */, X86::VANDNPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6706 /* vandnps */, X86::VANDNPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6706 /* vandnps */, X86::VANDNPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6706 /* vandnps */, X86::VFsANDNPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6706 /* vandnps */, X86::VANDNPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6714 /* vandpd */, X86::VANDPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6714 /* vandpd */, X86::VFsANDPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6714 /* vandpd */, X86::VANDPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6714 /* vandpd */, X86::VFsANDPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6714 /* vandpd */, X86::VANDPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6714 /* vandpd */, X86::VFsANDPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6714 /* vandpd */, X86::VANDPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6714 /* vandpd */, X86::VANDPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6714 /* vandpd */, X86::VANDPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6714 /* vandpd */, X86::VANDPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6714 /* vandpd */, X86::VFsANDPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6714 /* vandpd */, X86::VANDPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6721 /* vandps */, X86::VANDPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6721 /* vandps */, X86::VFsANDPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6721 /* vandps */, X86::VANDPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6721 /* vandps */, X86::VFsANDPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6721 /* vandps */, X86::VANDPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6721 /* vandps */, X86::VFsANDPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6721 /* vandps */, X86::VANDPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6721 /* vandps */, X86::VANDPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6721 /* vandps */, X86::VANDPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6721 /* vandps */, X86::VANDPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6721 /* vandps */, X86::VFsANDPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6721 /* vandps */, X86::VANDPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6728 /* vblendpd */, X86::VBLENDPDrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 6728 /* vblendpd */, X86::VBLENDPDrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 6728 /* vblendpd */, X86::VBLENDPDYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 6728 /* vblendpd */, X86::VBLENDPDYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 6728 /* vblendpd */, X86::VBLENDPDrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6728 /* vblendpd */, X86::VBLENDPDYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6728 /* vblendpd */, X86::VBLENDPDrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6728 /* vblendpd */, X86::VBLENDPDYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6737 /* vblendps */, X86::VBLENDPSrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 6737 /* vblendps */, X86::VBLENDPSrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 6737 /* vblendps */, X86::VBLENDPSYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 6737 /* vblendps */, X86::VBLENDPSYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 6737 /* vblendps */, X86::VBLENDPSrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6737 /* vblendps */, X86::VBLENDPSYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6737 /* vblendps */, X86::VBLENDPSrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6737 /* vblendps */, X86::VBLENDPSYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6746 /* vblendvpd */, X86::VBLENDVPDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6746 /* vblendvpd */, X86::VBLENDVPDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6746 /* vblendvpd */, X86::VBLENDVPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 6746 /* vblendvpd */, X86::VBLENDVPDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6746 /* vblendvpd */, X86::VBLENDVPDYrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6746 /* vblendvpd */, X86::VBLENDVPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6746 /* vblendvpd */, X86::VBLENDVPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 6746 /* vblendvpd */, X86::VBLENDVPDYrm, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6756 /* vblendvps */, X86::VBLENDVPSrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6756 /* vblendvps */, X86::VBLENDVPSrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6756 /* vblendvps */, X86::VBLENDVPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 6756 /* vblendvps */, X86::VBLENDVPSrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6756 /* vblendvps */, X86::VBLENDVPSYrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6756 /* vblendvps */, X86::VBLENDVPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6756 /* vblendvps */, X86::VBLENDVPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 6756 /* vblendvps */, X86::VBLENDVPSYrm, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6766 /* vbroadcastf128 */, X86::VBROADCASTF128, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 6766 /* vbroadcastf128 */, X86::VBROADCASTF128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 6781 /* vbroadcasti128 */, X86::VBROADCASTI128, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 6781 /* vbroadcasti128 */, X86::VBROADCASTI128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 6796 /* vbroadcastsd */, X86::VBROADCASTSDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 6796 /* vbroadcastsd */, X86::VBROADCASTSDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 6796 /* vbroadcastsd */, X86::VBROADCASTSDYrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR256, MCK_Mem64 }, 1},
  { 6796 /* vbroadcastsd */, X86::VBROADCASTSDYrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR256 }, 0},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 6809 /* vbroadcastss */, X86::VBROADCASTSSYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 6822 /* vcmp */, X86::VCMPPDrri, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPPDrri, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6822 /* vcmp */, X86::VCMPPDrmi, Convert__Reg1_2__Reg1_3__Mem1285_4__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6822 /* vcmp */, X86::VCMPPDYrri, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6822 /* vcmp */, X86::VCMPPDYrri, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6822 /* vcmp */, X86::VCMPPDYrmi, Convert__Reg1_2__Reg1_3__Mem2565_4__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6822 /* vcmp */, X86::VCMPPDrmi, Convert__Reg1_4__Reg1_3__Mem1285_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPPDYrmi, Convert__Reg1_4__Reg1_3__Mem2565_2__Imm1_0, 0, { MCK_Imm, MCK_pd, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6822 /* vcmp */, X86::VCMPPSrri, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPPSrri, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6822 /* vcmp */, X86::VCMPPSrmi, Convert__Reg1_2__Reg1_3__Mem1285_4__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 6822 /* vcmp */, X86::VCMPPSYrri, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6822 /* vcmp */, X86::VCMPPSYrri, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 6822 /* vcmp */, X86::VCMPPSYrmi, Convert__Reg1_2__Reg1_3__Mem2565_4__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 6822 /* vcmp */, X86::VCMPPSrmi, Convert__Reg1_4__Reg1_3__Mem1285_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPPSYrmi, Convert__Reg1_4__Reg1_3__Mem2565_2__Imm1_0, 0, { MCK_Imm, MCK_ps, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6822 /* vcmp */, X86::VCMPSDrr, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPSDrr, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6822 /* vcmp */, X86::VCMPSDrm, Convert__Reg1_2__Reg1_3__Mem645_4__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 6822 /* vcmp */, X86::VCMPSDrm, Convert__Reg1_4__Reg1_3__Mem645_2__Imm1_0, 0, { MCK_Imm, MCK_sd, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPSSrr, Convert__Reg1_4__Reg1_3__Reg1_2__Imm1_0, 0, { MCK_Imm, MCK_ss, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPSSrm, Convert__Reg1_4__Reg1_3__Mem325_2__Imm1_0, 0, { MCK_Imm, MCK_ss, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 6822 /* vcmp */, X86::VCMPSSrr, Convert__Reg1_2__Reg1_3__Reg1_4__Imm1_0, 0, { MCK_Imm, MCK_SS, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 6822 /* vcmp */, X86::VCMPSSrm, Convert__Reg1_2__Reg1_3__Mem325_4__Imm1_0, 0, { MCK_Imm, MCK_SS, MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 6827 /* vcmppd */, X86::VCMPPDrri_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6827 /* vcmppd */, X86::VCMPPDrmi_alt, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 6827 /* vcmppd */, X86::VCMPPDYrri_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 6827 /* vcmppd */, X86::VCMPPDYrmi_alt, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 6827 /* vcmppd */, X86::VCMPPDrri_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6827 /* vcmppd */, X86::VCMPPDYrri_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6827 /* vcmppd */, X86::VCMPPDrmi_alt, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6827 /* vcmppd */, X86::VCMPPDYrmi_alt, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6834 /* vcmpps */, X86::VCMPPSrri_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6834 /* vcmpps */, X86::VCMPPSrmi_alt, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 6834 /* vcmpps */, X86::VCMPPSYrri_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 6834 /* vcmpps */, X86::VCMPPSYrmi_alt, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 6834 /* vcmpps */, X86::VCMPPSrri_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6834 /* vcmpps */, X86::VCMPPSYrri_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 6834 /* vcmpps */, X86::VCMPPSrmi_alt, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 6834 /* vcmpps */, X86::VCMPPSYrmi_alt, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 6841 /* vcmpsd */, X86::VCMPSDrr_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6841 /* vcmpsd */, X86::VCMPSDrm_alt, Convert__Reg1_0__Reg1_1__Mem645_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_Imm }, 1},
  { 6841 /* vcmpsd */, X86::VCMPSDrr_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6841 /* vcmpsd */, X86::VCMPSDrm_alt, Convert__Reg1_3__Reg1_2__Mem645_1__Imm1_0, 0, { MCK_Imm, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 6848 /* vcmpss */, X86::VCMPSSrr_alt, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 6848 /* vcmpss */, X86::VCMPSSrm_alt, Convert__Reg1_0__Reg1_1__Mem325_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_Imm }, 1},
  { 6848 /* vcmpss */, X86::VCMPSSrr_alt, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 6848 /* vcmpss */, X86::VCMPSSrm_alt, Convert__Reg1_3__Reg1_2__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 6855 /* vcomisd */, X86::VCOMISDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6855 /* vcomisd */, X86::VCOMISDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6855 /* vcomisd */, X86::VCOMISDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6855 /* vcomisd */, X86::VCOMISDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6863 /* vcomiss */, X86::VCOMISSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6863 /* vcomiss */, X86::VCOMISSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6863 /* vcomiss */, X86::VCOMISSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6863 /* vcomiss */, X86::VCOMISSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 6871 /* vcvtdq2pd */, X86::VCVTDQ2PDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6881 /* vcvtdq2ps */, X86::VCVTDQ2PSYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_FR32, MCK_Mem256 }, 1},
  { 6891 /* vcvtpd2dq */, X86::VCVTPD2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 6901 /* vcvtpd2dqx */, X86::VCVTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6901 /* vcvtpd2dqx */, X86::VCVTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6901 /* vcvtpd2dqx */, X86::VCVTPD2DQXrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6901 /* vcvtpd2dqx */, X86::VCVTPD2DQXrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6912 /* vcvtpd2dqy */, X86::VCVTPD2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 6912 /* vcvtpd2dqy */, X86::VCVTPD2DQYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_FR32 }, 0},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_FR32, MCK_Mem256 }, 1},
  { 6923 /* vcvtpd2ps */, X86::VCVTPD2PSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 6933 /* vcvtpd2psx */, X86::VCVTPD2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6933 /* vcvtpd2psx */, X86::VCVTPD2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6933 /* vcvtpd2psx */, X86::VCVTPD2PSXrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6933 /* vcvtpd2psx */, X86::VCVTPD2PSXrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6944 /* vcvtpd2psy */, X86::VCVTPD2PSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 6944 /* vcvtpd2psy */, X86::VCVTPD2PSYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_FR32 }, 0},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 6955 /* vcvtph2ps */, X86::VCVTPH2PSrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 6965 /* vcvtps2dq */, X86::VCVTPS2DQYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 6975 /* vcvtps2pd */, X86::VCVTPS2PDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHYrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHmr, Convert__Mem645_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem64 }, 0},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHYrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_FR32 }, 0},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHYmr, Convert__Mem1285_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_Mem128 }, 0},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHYmr, Convert__Mem1285_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem128, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 6985 /* vcvtps2ph */, X86::VCVTPS2PHmr, Convert__Mem645_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 6995 /* vcvtsd2si */, X86::VCVTSD2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 6995 /* vcvtsd2si */, X86::VCVTSD2SIrm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 6995 /* vcvtsd2si */, X86::VCVTSD2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 6995 /* vcvtsd2si */, X86::VCVTSD2SI64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 7005 /* vcvtsd2sil */, X86::VCVTSD2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 7005 /* vcvtsd2sil */, X86::VCVTSD2SIrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 7016 /* vcvtsd2siq */, X86::VCVTSD2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 7016 /* vcvtsd2siq */, X86::VCVTSD2SI64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 7027 /* vcvtsd2ss */, X86::VCVTSD2SSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7027 /* vcvtsd2ss */, X86::VCVTSD2SSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7027 /* vcvtsd2ss */, X86::VCVTSD2SSrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7027 /* vcvtsd2ss */, X86::VCVTSD2SSrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_GR32 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_GR32 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SD64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_GR64 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SD64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7037 /* vcvtsi2sd */, X86::VCVTSI2SDrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7047 /* vcvtsi2sdl */, X86::VCVTSI2SDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 7047 /* vcvtsi2sdl */, X86::VCVTSI2SDrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7058 /* vcvtsi2sdq */, X86::VCVTSI2SD64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32, MCK_FR32 }, 0},
  { 7058 /* vcvtsi2sdq */, X86::VCVTSI2SD64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_GR32 }, 1},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SS64rr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_GR64 }, 1},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SS64rm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7069 /* vcvtsi2ss */, X86::VCVTSI2SSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7079 /* vcvtsi2ssq */, X86::VCVTSI2SS64rr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32, MCK_FR32 }, 0},
  { 7079 /* vcvtsi2ssq */, X86::VCVTSI2SS64rm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7090 /* vcvtss2sd */, X86::VCVTSS2SDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7090 /* vcvtss2sd */, X86::VCVTSS2SDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7090 /* vcvtss2sd */, X86::VCVTSS2SDrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7090 /* vcvtss2sd */, X86::VCVTSS2SDrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7100 /* vcvtss2si */, X86::VCVTSS2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 7100 /* vcvtss2si */, X86::VCVTSS2SIrm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR32, MCK_Mem }, 1},
  { 7100 /* vcvtss2si */, X86::VCVTSS2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 7100 /* vcvtss2si */, X86::VCVTSS2SI64rm, Convert__Reg1_0__Mem5_1, 0, { MCK_GR64, MCK_Mem }, 1},
  { 7110 /* vcvtss2sil */, X86::VCVTSS2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 7110 /* vcvtss2sil */, X86::VCVTSS2SIrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR32 }, 0},
  { 7121 /* vcvtss2siq */, X86::VCVTSS2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 7121 /* vcvtss2siq */, X86::VCVTSS2SI64rm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_GR64 }, 0},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_VR256 }, 1},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_FR32, MCK_Mem256 }, 1},
  { 7132 /* vcvttpd2dq */, X86::VCVTTPD2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 7143 /* vcvttpd2dqx */, X86::VCVTTPD2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 7143 /* vcvttpd2dqx */, X86::VCVTTPD2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 7143 /* vcvttpd2dqx */, X86::VCVTTPD2DQXrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 7143 /* vcvttpd2dqx */, X86::VCVTTPD2DQXrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 7155 /* vcvttpd2dqy */, X86::VCVTTPD2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_FR32 }, 0},
  { 7155 /* vcvttpd2dqy */, X86::VCVTTPD2DQYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_FR32 }, 0},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 7167 /* vcvttps2dq */, X86::VCVTTPS2DQYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR32, MCK_Mem64 }, 1},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SI64rm, Convert__Reg1_0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 7178 /* vcvttsd2si */, X86::VCVTTSD2SIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR32 }, 0},
  { 7189 /* vcvttsd2siq */, X86::VCVTTSD2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 7189 /* vcvttsd2siq */, X86::VCVTTSD2SI64rm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SI64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SI64rm, Convert__Reg1_0__Mem325_1, 0, { MCK_GR64, MCK_Mem32 }, 1},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 7201 /* vcvttss2si */, X86::VCVTTSS2SIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 7212 /* vcvttss2siq */, X86::VCVTTSS2SI64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 7212 /* vcvttss2siq */, X86::VCVTTSS2SI64rm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_GR64 }, 0},
  { 7224 /* vdivpd */, X86::VDIVPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7224 /* vdivpd */, X86::VDIVPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7224 /* vdivpd */, X86::VDIVPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7224 /* vdivpd */, X86::VDIVPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7224 /* vdivpd */, X86::VDIVPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7224 /* vdivpd */, X86::VDIVPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7224 /* vdivpd */, X86::VDIVPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7224 /* vdivpd */, X86::VDIVPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7231 /* vdivps */, X86::VDIVPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7231 /* vdivps */, X86::VDIVPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7231 /* vdivps */, X86::VDIVPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7231 /* vdivps */, X86::VDIVPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7231 /* vdivps */, X86::VDIVPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7231 /* vdivps */, X86::VDIVPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7231 /* vdivps */, X86::VDIVPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7231 /* vdivps */, X86::VDIVPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7238 /* vdivsd */, X86::VDIVSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7238 /* vdivsd */, X86::VDIVSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7238 /* vdivsd */, X86::VDIVSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7238 /* vdivsd */, X86::VDIVSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7245 /* vdivss */, X86::VDIVSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7245 /* vdivss */, X86::VDIVSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7245 /* vdivss */, X86::VDIVSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7245 /* vdivss */, X86::VDIVSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7252 /* vdppd */, X86::VDPPDrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 7252 /* vdppd */, X86::VDPPDrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 7252 /* vdppd */, X86::VDPPDrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7252 /* vdppd */, X86::VDPPDrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7258 /* vdpps */, X86::VDPPSrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 7258 /* vdpps */, X86::VDPPSrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 7258 /* vdpps */, X86::VDPPSYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 7258 /* vdpps */, X86::VDPPSYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 7258 /* vdpps */, X86::VDPPSrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7258 /* vdpps */, X86::VDPPSYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7258 /* vdpps */, X86::VDPPSrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7258 /* vdpps */, X86::VDPPSYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7264 /* verr */, X86::VERRr, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 7264 /* verr */, X86::VERRr, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 7264 /* verr */, X86::VERRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 7264 /* verr */, X86::VERRm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 7269 /* verw */, X86::VERWr, Convert__Reg1_0, 0, { MCK_GR16 }, 0},
  { 7269 /* verw */, X86::VERWr, Convert__Reg1_0, 0, { MCK_GR16 }, 1},
  { 7269 /* verw */, X86::VERWm, Convert__Mem165_0, 0, { MCK_Mem16 }, 0},
  { 7269 /* verw */, X86::VERWm, Convert__Mem165_0, 0, { MCK_Mem16 }, 1},
  { 7274 /* vextractf128 */, X86::VEXTRACTF128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_VR256, MCK_Imm }, 1},
  { 7274 /* vextractf128 */, X86::VEXTRACTF128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_FR32 }, 0},
  { 7274 /* vextractf128 */, X86::VEXTRACTF128mr, Convert__Mem1285_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_Mem128 }, 0},
  { 7274 /* vextractf128 */, X86::VEXTRACTF128mr, Convert__Mem1285_0__Reg1_1__Imm1_2, 0, { MCK_Mem128, MCK_VR256, MCK_Imm }, 1},
  { 7287 /* vextracti128 */, X86::VEXTRACTI128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_VR256, MCK_Imm }, 1},
  { 7287 /* vextracti128 */, X86::VEXTRACTI128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_FR32 }, 0},
  { 7287 /* vextracti128 */, X86::VEXTRACTI128mr, Convert__Mem1285_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_Mem128 }, 0},
  { 7287 /* vextracti128 */, X86::VEXTRACTI128mr, Convert__Mem1285_0__Reg1_1__Imm1_2, 0, { MCK_Mem128, MCK_VR256, MCK_Imm }, 1},
  { 7300 /* vextractps */, X86::VEXTRACTPSrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 7300 /* vextractps */, X86::VEXTRACTPSrr64, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 7300 /* vextractps */, X86::VEXTRACTPSrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 7300 /* vextractps */, X86::VEXTRACTPSrr64, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR64 }, 0},
  { 7300 /* vextractps */, X86::VEXTRACTPSmr, Convert__Mem325_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem32 }, 0},
  { 7300 /* vextractps */, X86::VEXTRACTPSmr, Convert__Mem325_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7311 /* vfmadd132pd */, X86::VFMADDPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7323 /* vfmadd132ps */, X86::VFMADDPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7335 /* vfmadd132sd */, X86::VFMADDSDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7335 /* vfmadd132sd */, X86::VFMADDSDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7335 /* vfmadd132sd */, X86::VFMADDSDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7335 /* vfmadd132sd */, X86::VFMADDSDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7347 /* vfmadd132ss */, X86::VFMADDSSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7347 /* vfmadd132ss */, X86::VFMADDSSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7347 /* vfmadd132ss */, X86::VFMADDSSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7347 /* vfmadd132ss */, X86::VFMADDSSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7359 /* vfmadd213pd */, X86::VFMADDPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7371 /* vfmadd213ps */, X86::VFMADDPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7383 /* vfmadd213sd */, X86::VFMADDSDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7383 /* vfmadd213sd */, X86::VFMADDSDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7383 /* vfmadd213sd */, X86::VFMADDSDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7383 /* vfmadd213sd */, X86::VFMADDSDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7395 /* vfmadd213ss */, X86::VFMADDSSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7395 /* vfmadd213ss */, X86::VFMADDSSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7395 /* vfmadd213ss */, X86::VFMADDSSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7395 /* vfmadd213ss */, X86::VFMADDSSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7407 /* vfmadd231pd */, X86::VFMADDPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7419 /* vfmadd231ps */, X86::VFMADDPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7431 /* vfmadd231sd */, X86::VFMADDSDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7431 /* vfmadd231sd */, X86::VFMADDSDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7431 /* vfmadd231sd */, X86::VFMADDSDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7431 /* vfmadd231sd */, X86::VFMADDSDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7443 /* vfmadd231ss */, X86::VFMADDSSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7443 /* vfmadd231ss */, X86::VFMADDSSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7443 /* vfmadd231ss */, X86::VFMADDSSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7443 /* vfmadd231ss */, X86::VFMADDSSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7455 /* vfmaddpd */, X86::VFMADDPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7464 /* vfmaddps */, X86::VFMADDPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7464 /* vfmaddps */, X86::VFMADDPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4mr, Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_FR32 }, 1},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4mr, Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7473 /* vfmaddsd */, X86::VFMADDSD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7482 /* vfmaddss */, X86::VFMADDSS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7482 /* vfmaddss */, X86::VFMADDSS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7482 /* vfmaddss */, X86::VFMADDSS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7482 /* vfmaddss */, X86::VFMADDSS4mr, Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_FR32 }, 1},
  { 7482 /* vfmaddss */, X86::VFMADDSS4mr, Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7482 /* vfmaddss */, X86::VFMADDSS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7491 /* vfmaddsub132pd */, X86::VFMADDSUBPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7506 /* vfmaddsub132ps */, X86::VFMADDSUBPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7521 /* vfmaddsub213pd */, X86::VFMADDSUBPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7536 /* vfmaddsub213ps */, X86::VFMADDSUBPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7551 /* vfmaddsub231pd */, X86::VFMADDSUBPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7566 /* vfmaddsub231ps */, X86::VFMADDSUBPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7581 /* vfmaddsubpd */, X86::VFMADDSUBPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7593 /* vfmaddsubps */, X86::VFMADDSUBPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7605 /* vfmsub132pd */, X86::VFMSUBPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7617 /* vfmsub132ps */, X86::VFMSUBPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7629 /* vfmsub132sd */, X86::VFMSUBSDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7629 /* vfmsub132sd */, X86::VFMSUBSDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7629 /* vfmsub132sd */, X86::VFMSUBSDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7629 /* vfmsub132sd */, X86::VFMSUBSDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7641 /* vfmsub132ss */, X86::VFMSUBSSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7641 /* vfmsub132ss */, X86::VFMSUBSSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7641 /* vfmsub132ss */, X86::VFMSUBSSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7641 /* vfmsub132ss */, X86::VFMSUBSSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7653 /* vfmsub213pd */, X86::VFMSUBPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7665 /* vfmsub213ps */, X86::VFMSUBPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7677 /* vfmsub213sd */, X86::VFMSUBSDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7677 /* vfmsub213sd */, X86::VFMSUBSDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7677 /* vfmsub213sd */, X86::VFMSUBSDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7677 /* vfmsub213sd */, X86::VFMSUBSDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7689 /* vfmsub213ss */, X86::VFMSUBSSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7689 /* vfmsub213ss */, X86::VFMSUBSSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7689 /* vfmsub213ss */, X86::VFMSUBSSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7689 /* vfmsub213ss */, X86::VFMSUBSSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7701 /* vfmsub231pd */, X86::VFMSUBPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7713 /* vfmsub231ps */, X86::VFMSUBPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7725 /* vfmsub231sd */, X86::VFMSUBSDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7725 /* vfmsub231sd */, X86::VFMSUBSDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7725 /* vfmsub231sd */, X86::VFMSUBSDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7725 /* vfmsub231sd */, X86::VFMSUBSDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7737 /* vfmsub231ss */, X86::VFMSUBSSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7737 /* vfmsub231ss */, X86::VFMSUBSSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7737 /* vfmsub231ss */, X86::VFMSUBSSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7737 /* vfmsub231ss */, X86::VFMSUBSSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7749 /* vfmsubadd132pd */, X86::VFMSUBADDPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7764 /* vfmsubadd132ps */, X86::VFMSUBADDPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7779 /* vfmsubadd213pd */, X86::VFMSUBADDPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7794 /* vfmsubadd213ps */, X86::VFMSUBADDPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7809 /* vfmsubadd231pd */, X86::VFMSUBADDPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7824 /* vfmsubadd231ps */, X86::VFMSUBADDPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7839 /* vfmsubaddpd */, X86::VFMSUBADDPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7851 /* vfmsubaddps */, X86::VFMSUBADDPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7863 /* vfmsubpd */, X86::VFMSUBPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7872 /* vfmsubps */, X86::VFMSUBPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4mr, Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_FR32 }, 1},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4mr, Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7881 /* vfmsubsd */, X86::VFMSUBSD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4mr, Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_FR32 }, 1},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4mr, Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7890 /* vfmsubss */, X86::VFMSUBSS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7899 /* vfnmadd132pd */, X86::VFNMADDPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7912 /* vfnmadd132ps */, X86::VFNMADDPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7925 /* vfnmadd132sd */, X86::VFNMADDSDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7925 /* vfnmadd132sd */, X86::VFNMADDSDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7925 /* vfnmadd132sd */, X86::VFNMADDSDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7925 /* vfnmadd132sd */, X86::VFNMADDSDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7938 /* vfnmadd132ss */, X86::VFNMADDSSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7938 /* vfnmadd132ss */, X86::VFNMADDSSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7938 /* vfnmadd132ss */, X86::VFNMADDSSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7938 /* vfnmadd132ss */, X86::VFNMADDSSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7951 /* vfnmadd213pd */, X86::VFNMADDPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 7964 /* vfnmadd213ps */, X86::VFNMADDPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 7977 /* vfnmadd213sd */, X86::VFNMADDSDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7977 /* vfnmadd213sd */, X86::VFNMADDSDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7977 /* vfnmadd213sd */, X86::VFNMADDSDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 7977 /* vfnmadd213sd */, X86::VFNMADDSDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 7990 /* vfnmadd213ss */, X86::VFNMADDSSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 7990 /* vfnmadd213ss */, X86::VFNMADDSSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 7990 /* vfnmadd213ss */, X86::VFNMADDSSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 7990 /* vfnmadd213ss */, X86::VFNMADDSSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8003 /* vfnmadd231pd */, X86::VFNMADDPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8016 /* vfnmadd231ps */, X86::VFNMADDPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8029 /* vfnmadd231sd */, X86::VFNMADDSDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8029 /* vfnmadd231sd */, X86::VFNMADDSDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8029 /* vfnmadd231sd */, X86::VFNMADDSDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8029 /* vfnmadd231sd */, X86::VFNMADDSDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8042 /* vfnmadd231ss */, X86::VFNMADDSSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8042 /* vfnmadd231ss */, X86::VFNMADDSSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8042 /* vfnmadd231ss */, X86::VFNMADDSSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8042 /* vfnmadd231ss */, X86::VFNMADDSSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8055 /* vfnmaddpd */, X86::VFNMADDPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8065 /* vfnmaddps */, X86::VFNMADDPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4mr, Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_FR32 }, 1},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4mr, Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8075 /* vfnmaddsd */, X86::VFNMADDSD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4mr, Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_FR32 }, 1},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4mr, Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8085 /* vfnmaddss */, X86::VFNMADDSS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8095 /* vfnmsub132pd */, X86::VFNMSUBPDr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8108 /* vfnmsub132ps */, X86::VFNMSUBPSr132mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8121 /* vfnmsub132sd */, X86::VFNMSUBSDr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8121 /* vfnmsub132sd */, X86::VFNMSUBSDr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8121 /* vfnmsub132sd */, X86::VFNMSUBSDr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8121 /* vfnmsub132sd */, X86::VFNMSUBSDr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8134 /* vfnmsub132ss */, X86::VFNMSUBSSr132r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8134 /* vfnmsub132ss */, X86::VFNMSUBSSr132r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8134 /* vfnmsub132ss */, X86::VFNMSUBSSr132m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8134 /* vfnmsub132ss */, X86::VFNMSUBSSr132m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8147 /* vfnmsub213pd */, X86::VFNMSUBPDr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8160 /* vfnmsub213ps */, X86::VFNMSUBPSr213mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8173 /* vfnmsub213sd */, X86::VFNMSUBSDr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8173 /* vfnmsub213sd */, X86::VFNMSUBSDr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8173 /* vfnmsub213sd */, X86::VFNMSUBSDr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8173 /* vfnmsub213sd */, X86::VFNMSUBSDr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8186 /* vfnmsub213ss */, X86::VFNMSUBSSr213r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8186 /* vfnmsub213ss */, X86::VFNMSUBSSr213r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8186 /* vfnmsub213ss */, X86::VFNMSUBSSr213m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8186 /* vfnmsub213ss */, X86::VFNMSUBSSr213m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8199 /* vfnmsub231pd */, X86::VFNMSUBPDr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231rY, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231rY, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231mY, Convert__Reg1_0__Tie0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8212 /* vfnmsub231ps */, X86::VFNMSUBPSr231mY, Convert__Reg1_2__Tie0__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8225 /* vfnmsub231sd */, X86::VFNMSUBSDr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8225 /* vfnmsub231sd */, X86::VFNMSUBSDr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8225 /* vfnmsub231sd */, X86::VFNMSUBSDr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8225 /* vfnmsub231sd */, X86::VFNMSUBSDr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8238 /* vfnmsub231ss */, X86::VFNMSUBSSr231r, Convert__Reg1_2__Tie0__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8238 /* vfnmsub231ss */, X86::VFNMSUBSSr231r, Convert__Reg1_0__Tie0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8238 /* vfnmsub231ss */, X86::VFNMSUBSSr231m, Convert__Reg1_0__Tie0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8238 /* vfnmsub231ss */, X86::VFNMSUBSSr231m, Convert__Reg1_2__Tie0__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8251 /* vfnmsubpd */, X86::VFNMSUBPD4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4mr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4mr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4mrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4mrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8261 /* vfnmsubps */, X86::VFNMSUBPS4rmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem645_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4mr, Convert__Reg1_0__Reg1_1__Mem645_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_FR32 }, 1},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4mr, Convert__Reg1_3__Reg1_2__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8271 /* vfnmsubsd */, X86::VFNMSUBSD4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4rr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4rr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4rm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem325_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4mr, Convert__Reg1_0__Reg1_1__Mem325_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_FR32 }, 1},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4mr, Convert__Reg1_3__Reg1_2__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8281 /* vfnmsubss */, X86::VFNMSUBSS4rm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8291 /* vfrczpd */, X86::VFRCZPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8291 /* vfrczpd */, X86::VFRCZPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8291 /* vfrczpd */, X86::VFRCZPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8291 /* vfrczpd */, X86::VFRCZPDrrY, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8291 /* vfrczpd */, X86::VFRCZPDrrY, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8291 /* vfrczpd */, X86::VFRCZPDrmY, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8291 /* vfrczpd */, X86::VFRCZPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8291 /* vfrczpd */, X86::VFRCZPDrmY, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8299 /* vfrczps */, X86::VFRCZPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8299 /* vfrczps */, X86::VFRCZPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8299 /* vfrczps */, X86::VFRCZPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8299 /* vfrczps */, X86::VFRCZPSrrY, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8299 /* vfrczps */, X86::VFRCZPSrrY, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8299 /* vfrczps */, X86::VFRCZPSrmY, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8299 /* vfrczps */, X86::VFRCZPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8299 /* vfrczps */, X86::VFRCZPSrmY, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8307 /* vfrczsd */, X86::VFRCZSDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8307 /* vfrczsd */, X86::VFRCZSDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8307 /* vfrczsd */, X86::VFRCZSDrm, Convert__Reg1_0__Mem5_1, 0, { MCK_FR32, MCK_Mem }, 1},
  { 8307 /* vfrczsd */, X86::VFRCZSDrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_FR32 }, 0},
  { 8315 /* vfrczss */, X86::VFRCZSSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8315 /* vfrczss */, X86::VFRCZSSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8315 /* vfrczss */, X86::VFRCZSSrm, Convert__Reg1_0__Mem5_1, 0, { MCK_FR32, MCK_Mem }, 1},
  { 8315 /* vfrczss */, X86::VFRCZSSrm, Convert__Reg1_1__Mem5_0, 0, { MCK_Mem, MCK_FR32 }, 0},
  { 8323 /* vgatherdpd */, X86::VGATHERDPDrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 0},
  { 8323 /* vgatherdpd */, X86::VGATHERDPDrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 1},
  { 8323 /* vgatherdpd */, X86::VGATHERDPDYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_VR256, MCK_MemVX64, MCK_VR256 }, 0},
  { 8323 /* vgatherdpd */, X86::VGATHERDPDYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_VR256, MCK_MemVX64, MCK_VR256 }, 1},
  { 8334 /* vgatherdps */, X86::VGATHERDPSrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 0},
  { 8334 /* vgatherdps */, X86::VGATHERDPSrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 1},
  { 8334 /* vgatherdps */, X86::VGATHERDPSYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1, 0, { MCK_VR256, MCK_MemVY32, MCK_VR256 }, 0},
  { 8334 /* vgatherdps */, X86::VGATHERDPSYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1, 0, { MCK_VR256, MCK_MemVY32, MCK_VR256 }, 1},
  { 8345 /* vgatherqpd */, X86::VGATHERQPDrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 0},
  { 8345 /* vgatherqpd */, X86::VGATHERQPDrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 1},
  { 8345 /* vgatherqpd */, X86::VGATHERQPDYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY645_1__Tie1, 0, { MCK_VR256, MCK_MemVY64, MCK_VR256 }, 0},
  { 8345 /* vgatherqpd */, X86::VGATHERQPDYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY645_1__Tie1, 0, { MCK_VR256, MCK_MemVY64, MCK_VR256 }, 1},
  { 8356 /* vgatherqps */, X86::VGATHERQPSrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 0},
  { 8356 /* vgatherqps */, X86::VGATHERQPSrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 1},
  { 8356 /* vgatherqps */, X86::VGATHERQPSYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1, 0, { MCK_FR32, MCK_MemVY32, MCK_FR32 }, 0},
  { 8356 /* vgatherqps */, X86::VGATHERQPSYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1, 0, { MCK_FR32, MCK_MemVY32, MCK_FR32 }, 1},
  { 8367 /* vhaddpd */, X86::VHADDPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8367 /* vhaddpd */, X86::VHADDPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8367 /* vhaddpd */, X86::VHADDPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8367 /* vhaddpd */, X86::VHADDPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8367 /* vhaddpd */, X86::VHADDPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8367 /* vhaddpd */, X86::VHADDPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8367 /* vhaddpd */, X86::VHADDPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8367 /* vhaddpd */, X86::VHADDPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8375 /* vhaddps */, X86::VHADDPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8375 /* vhaddps */, X86::VHADDPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8375 /* vhaddps */, X86::VHADDPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8375 /* vhaddps */, X86::VHADDPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8375 /* vhaddps */, X86::VHADDPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8375 /* vhaddps */, X86::VHADDPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8375 /* vhaddps */, X86::VHADDPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8375 /* vhaddps */, X86::VHADDPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8383 /* vhsubpd */, X86::VHSUBPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8383 /* vhsubpd */, X86::VHSUBPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8383 /* vhsubpd */, X86::VHSUBPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8383 /* vhsubpd */, X86::VHSUBPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8383 /* vhsubpd */, X86::VHSUBPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8383 /* vhsubpd */, X86::VHSUBPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8383 /* vhsubpd */, X86::VHSUBPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8383 /* vhsubpd */, X86::VHSUBPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8391 /* vhsubps */, X86::VHSUBPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8391 /* vhsubps */, X86::VHSUBPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8391 /* vhsubps */, X86::VHSUBPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8391 /* vhsubps */, X86::VHSUBPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8391 /* vhsubps */, X86::VHSUBPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8391 /* vhsubps */, X86::VHSUBPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8391 /* vhsubps */, X86::VHSUBPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8391 /* vhsubps */, X86::VHSUBPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8399 /* vinsertf128 */, X86::VINSERTF128rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_FR32, MCK_Imm }, 1},
  { 8399 /* vinsertf128 */, X86::VINSERTF128rm, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem128, MCK_Imm }, 1},
  { 8399 /* vinsertf128 */, X86::VINSERTF128rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 8399 /* vinsertf128 */, X86::VINSERTF128rm, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 8411 /* vinserti128 */, X86::VINSERTI128rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_FR32, MCK_Imm }, 1},
  { 8411 /* vinserti128 */, X86::VINSERTI128rm, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem128, MCK_Imm }, 1},
  { 8411 /* vinserti128 */, X86::VINSERTI128rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 8411 /* vinserti128 */, X86::VINSERTI128rm, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 8423 /* vinsertps */, X86::VINSERTPSrr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 8423 /* vinsertps */, X86::VINSERTPSrm, Convert__Reg1_0__Reg1_1__Mem325_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_ImmZExtu32u8 }, 1},
  { 8423 /* vinsertps */, X86::VINSERTPSrr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8423 /* vinsertps */, X86::VINSERTPSrm, Convert__Reg1_3__Reg1_2__Mem325_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8433 /* vlddqu */, X86::VLDDQUrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8433 /* vlddqu */, X86::VLDDQUYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8433 /* vlddqu */, X86::VLDDQUrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8433 /* vlddqu */, X86::VLDDQUYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8440 /* vldmxcsr */, X86::VLDMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 8440 /* vldmxcsr */, X86::VLDMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 8449 /* vmaskmovdqu */, X86::VMASKMOVDQU, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8449 /* vmaskmovdqu */, X86::VMASKMOVDQU64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8449 /* vmaskmovdqu */, X86::VMASKMOVDQU, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8449 /* vmaskmovdqu */, X86::VMASKMOVDQU64, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDmr, Convert__Mem1285_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 0},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDYmr, Convert__Mem2565_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 0},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDmr, Convert__Mem1285_0__Reg1_1__Reg1_2, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 1},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8461 /* vmaskmovpd */, X86::VMASKMOVPDYmr, Convert__Mem2565_0__Reg1_1__Reg1_2, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 1},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSmr, Convert__Mem1285_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 0},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSYmr, Convert__Mem2565_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 0},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSmr, Convert__Mem1285_0__Reg1_1__Reg1_2, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 1},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8472 /* vmaskmovps */, X86::VMASKMOVPSYmr, Convert__Mem2565_0__Reg1_1__Reg1_2, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 1},
  { 8483 /* vmaxpd */, X86::VMAXPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8483 /* vmaxpd */, X86::VMAXPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8483 /* vmaxpd */, X86::VMAXPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8483 /* vmaxpd */, X86::VMAXPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8483 /* vmaxpd */, X86::VMAXPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8483 /* vmaxpd */, X86::VMAXPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8483 /* vmaxpd */, X86::VMAXPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8483 /* vmaxpd */, X86::VMAXPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8490 /* vmaxps */, X86::VMAXPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8490 /* vmaxps */, X86::VMAXPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8490 /* vmaxps */, X86::VMAXPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8490 /* vmaxps */, X86::VMAXPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8490 /* vmaxps */, X86::VMAXPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8490 /* vmaxps */, X86::VMAXPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8490 /* vmaxps */, X86::VMAXPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8490 /* vmaxps */, X86::VMAXPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8497 /* vmaxsd */, X86::VMAXSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8497 /* vmaxsd */, X86::VMAXSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8497 /* vmaxsd */, X86::VMAXSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8497 /* vmaxsd */, X86::VMAXSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8504 /* vmaxss */, X86::VMAXSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8504 /* vmaxss */, X86::VMAXSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8504 /* vmaxss */, X86::VMAXSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8504 /* vmaxss */, X86::VMAXSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8511 /* vmcall */, X86::VMCALL, Convert_NoOperands, 0, {  }, 0},
  { 8511 /* vmcall */, X86::VMCALL, Convert_NoOperands, 0, {  }, 1},
  { 8518 /* vmclear */, X86::VMCLEARm, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 8518 /* vmclear */, X86::VMCLEARm, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 8526 /* vmfunc */, X86::VMFUNC, Convert_NoOperands, 0, {  }, 0},
  { 8526 /* vmfunc */, X86::VMFUNC, Convert_NoOperands, 0, {  }, 1},
  { 8533 /* vminpd */, X86::VMINPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8533 /* vminpd */, X86::VMINPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8533 /* vminpd */, X86::VMINPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8533 /* vminpd */, X86::VMINPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8533 /* vminpd */, X86::VMINPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8533 /* vminpd */, X86::VMINPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8533 /* vminpd */, X86::VMINPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8533 /* vminpd */, X86::VMINPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8540 /* vminps */, X86::VMINPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8540 /* vminps */, X86::VMINPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8540 /* vminps */, X86::VMINPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8540 /* vminps */, X86::VMINPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8540 /* vminps */, X86::VMINPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8540 /* vminps */, X86::VMINPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8540 /* vminps */, X86::VMINPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8540 /* vminps */, X86::VMINPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8547 /* vminsd */, X86::VMINSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8547 /* vminsd */, X86::VMINSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8547 /* vminsd */, X86::VMINSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8547 /* vminsd */, X86::VMINSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8554 /* vminss */, X86::VMINSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8554 /* vminss */, X86::VMINSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8554 /* vminss */, X86::VMINSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8554 /* vminss */, X86::VMINSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8561 /* vmlaunch */, X86::VMLAUNCH, Convert_NoOperands, 0, {  }, 0},
  { 8561 /* vmlaunch */, X86::VMLAUNCH, Convert_NoOperands, 0, {  }, 1},
  { 8570 /* vmload */, X86::VMLOAD32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 0},
  { 8570 /* vmload */, X86::VMLOAD32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 1},
  { 8570 /* vmload */, X86::VMLOAD64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 0},
  { 8570 /* vmload */, X86::VMLOAD64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 1},
  { 8577 /* vmmcall */, X86::VMMCALL, Convert_NoOperands, 0, {  }, 0},
  { 8577 /* vmmcall */, X86::VMMCALL, Convert_NoOperands, 0, {  }, 1},
  { 8585 /* vmovapd */, X86::FsVMOVAPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8585 /* vmovapd */, X86::FsVMOVAPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8585 /* vmovapd */, X86::VMOVAPDYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8585 /* vmovapd */, X86::VMOVAPDYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8593 /* vmovaps */, X86::FsVMOVAPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8593 /* vmovaps */, X86::FsVMOVAPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8593 /* vmovaps */, X86::VMOVAPSYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8593 /* vmovaps */, X86::VMOVAPSYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8601 /* vmovd */, X86::VMOVDI2PDIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVDI2SSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVZDI2PDIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVPDI2DIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVSS2DIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 8601 /* vmovd */, X86::VMOV64toPQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOV64toSDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVZQI2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVQd64rr_alt, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVPDI2DIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVSS2DIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVDI2PDIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVDI2SSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVZDI2PDIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVPQIto64rr, Convert__Reg1_1__Reg1_0, Feature_In64BitMode, { MCK_FR32, MCK_GR64 }, 0},
  { 8601 /* vmovd */, X86::VMOVQd64rr_alt, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8601 /* vmovd */, X86::VMOVSDto64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8601 /* vmovd */, X86::VMOVPDI2DImr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 8601 /* vmovd */, X86::VMOVSS2DImr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 8601 /* vmovd */, X86::VMOVDI2PDIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 8601 /* vmovd */, X86::VMOVDI2SSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 8601 /* vmovd */, X86::VMOVZDI2PDIrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 8601 /* vmovd */, X86::VMOVDI2PDIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVDI2SSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVZDI2PDIrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 8601 /* vmovd */, X86::VMOVPDI2DImr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 8601 /* vmovd */, X86::VMOVSS2DImr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 8607 /* vmovddup */, X86::VMOVDDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8607 /* vmovddup */, X86::VMOVDDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8607 /* vmovddup */, X86::VMOVDDUPrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 8607 /* vmovddup */, X86::VMOVDDUPYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8607 /* vmovddup */, X86::VMOVDDUPYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8607 /* vmovddup */, X86::VMOVDDUPYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8607 /* vmovddup */, X86::VMOVDDUPYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8607 /* vmovddup */, X86::VMOVDDUPrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQArr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQArr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8616 /* vmovdqa */, X86::VMOVDQAmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQArm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8616 /* vmovdqa */, X86::VMOVDQAYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQAYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8616 /* vmovdqa */, X86::VMOVDQAYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQAYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8616 /* vmovdqa */, X86::VMOVDQArm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQAmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8616 /* vmovdqa */, X86::VMOVDQAYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8616 /* vmovdqa */, X86::VMOVDQAYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8624 /* vmovdqu */, X86::VMOVDQUYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8624 /* vmovdqu */, X86::VMOVDQUYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8632 /* vmovhlps */, X86::VMOVHLPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8632 /* vmovhlps */, X86::VMOVHLPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8641 /* vmovhpd */, X86::VMOVHPDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8641 /* vmovhpd */, X86::VMOVHPDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8641 /* vmovhpd */, X86::VMOVHPDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8641 /* vmovhpd */, X86::VMOVHPDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8649 /* vmovhps */, X86::VMOVHPSmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8649 /* vmovhps */, X86::VMOVHPSmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8649 /* vmovhps */, X86::VMOVHPSrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8649 /* vmovhps */, X86::VMOVHPSrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8657 /* vmovlhps */, X86::VMOVLHPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8657 /* vmovlhps */, X86::VMOVLHPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8666 /* vmovlpd */, X86::VMOVLPDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8666 /* vmovlpd */, X86::VMOVLPDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8666 /* vmovlpd */, X86::VMOVLPDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8666 /* vmovlpd */, X86::VMOVLPDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8674 /* vmovlps */, X86::VMOVLPSmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8674 /* vmovlps */, X86::VMOVLPSmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8674 /* vmovlps */, X86::VMOVLPSrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8674 /* vmovlps */, X86::VMOVLPSrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_VR256 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR256 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR256 }, 1},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR32 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR64 }, 0},
  { 8682 /* vmovmskpd */, X86::VMOVMSKPDYrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR64 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYrr32, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_VR256 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR256 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYrr64, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR256 }, 1},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYrr32, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR32 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR64 }, 0},
  { 8692 /* vmovmskps */, X86::VMOVMSKPSYrr64, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR64 }, 0},
  { 8702 /* vmovntdq */, X86::VMOVNTDQmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8702 /* vmovntdq */, X86::VMOVNTDQYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8702 /* vmovntdq */, X86::VMOVNTDQmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8702 /* vmovntdq */, X86::VMOVNTDQYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8711 /* vmovntdqa */, X86::VMOVNTDQArm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8711 /* vmovntdqa */, X86::VMOVNTDQAYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8711 /* vmovntdqa */, X86::VMOVNTDQArm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8711 /* vmovntdqa */, X86::VMOVNTDQAYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8721 /* vmovntpd */, X86::VMOVNTPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8721 /* vmovntpd */, X86::VMOVNTPDYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8721 /* vmovntpd */, X86::VMOVNTPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8721 /* vmovntpd */, X86::VMOVNTPDYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8730 /* vmovntps */, X86::VMOVNTPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8730 /* vmovntps */, X86::VMOVNTPSYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8730 /* vmovntps */, X86::VMOVNTPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8730 /* vmovntps */, X86::VMOVNTPSYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8739 /* vmovq */, X86::VMOVPQIto64rr, Convert__Reg1_0__Reg1_1, Feature_In64BitMode, { MCK_GR64, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVQs64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVQd64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVSDto64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVQd64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 8739 /* vmovq */, X86::VMOV64toPQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 8739 /* vmovq */, X86::VMOV64toSDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 8739 /* vmovq */, X86::VMOVQs64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 8739 /* vmovq */, X86::VMOVZQI2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_GR64 }, 1},
  { 8739 /* vmovq */, X86::VMOVQxrxr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVZPQILo2PQIrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVQxrxr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVZPQILo2PQIrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVZPQILo2PQIrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8739 /* vmovq */, X86::VMOVLQ128mr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8739 /* vmovq */, X86::VMOVPQI2QImr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8739 /* vmovq */, X86::VMOVSDto64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8739 /* vmovq */, X86::VMOV64toSDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 8739 /* vmovq */, X86::VMOVQI2PQIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 8739 /* vmovq */, X86::VMOVZQI2PQIrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 8739 /* vmovq */, X86::VMOVZPQILo2PQIrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOV64toSDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVQI2PQIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVZQI2PQIrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 8739 /* vmovq */, X86::VMOVLQ128mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVPQI2QImr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8739 /* vmovq */, X86::VMOVSDto64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8745 /* vmovsd */, X86::VMOVSDmr, Convert__Mem645_1__Reg1_0, 0, { MCK_FR32, MCK_Mem64 }, 0},
  { 8745 /* vmovsd */, X86::VMOVSDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 8745 /* vmovsd */, X86::VMOVSDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 8745 /* vmovsd */, X86::VMOVSDmr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_FR32 }, 1},
  { 8745 /* vmovsd */, X86::VMOVSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8745 /* vmovsd */, X86::VMOVSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8752 /* vmovshdup */, X86::VMOVSHDUPYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8762 /* vmovsldup */, X86::VMOVSLDUPYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8772 /* vmovss */, X86::VMOVSSmr, Convert__Mem325_1__Reg1_0, 0, { MCK_FR32, MCK_Mem32 }, 0},
  { 8772 /* vmovss */, X86::VMOVSSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 8772 /* vmovss */, X86::VMOVSSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 8772 /* vmovss */, X86::VMOVSSmr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_FR32 }, 1},
  { 8772 /* vmovss */, X86::VMOVSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8772 /* vmovss */, X86::VMOVSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8779 /* vmovupd */, X86::VMOVUPDYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8779 /* vmovupd */, X86::VMOVUPDYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSmr, Convert__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSYmr, Convert__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSmr, Convert__Mem1285_0__Reg1_1, 0, { MCK_Mem128, MCK_FR32 }, 1},
  { 8787 /* vmovups */, X86::VMOVUPSYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8787 /* vmovups */, X86::VMOVUPSYmr, Convert__Mem2565_0__Reg1_1, 0, { MCK_Mem256, MCK_VR256 }, 1},
  { 8795 /* vmpsadbw */, X86::VMPSADBWrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 8795 /* vmpsadbw */, X86::VMPSADBWrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 8795 /* vmpsadbw */, X86::VMPSADBWYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 8795 /* vmpsadbw */, X86::VMPSADBWYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 8795 /* vmpsadbw */, X86::VMPSADBWrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8795 /* vmpsadbw */, X86::VMPSADBWYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8795 /* vmpsadbw */, X86::VMPSADBWrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8795 /* vmpsadbw */, X86::VMPSADBWYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8804 /* vmptrld */, X86::VMPTRLDm, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 8804 /* vmptrld */, X86::VMPTRLDm, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 8812 /* vmptrst */, X86::VMPTRSTm, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 8812 /* vmptrst */, X86::VMPTRSTm, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 8820 /* vmread */, X86::VMREAD32rr, Convert__Reg1_0__Reg1_1, Feature_In32BitMode, { MCK_GR32, MCK_GR32 }, 1},
  { 8820 /* vmread */, X86::VMREAD64rr, Convert__Reg1_0__Reg1_1, Feature_In64BitMode, { MCK_GR64, MCK_GR64 }, 1},
  { 8820 /* vmread */, X86::VMREAD32rm, Convert__Mem325_0__Reg1_1, Feature_In32BitMode, { MCK_Mem32, MCK_GR32 }, 1},
  { 8820 /* vmread */, X86::VMREAD64rm, Convert__Mem645_0__Reg1_1, Feature_In64BitMode, { MCK_Mem64, MCK_GR64 }, 1},
  { 8827 /* vmreadl */, X86::VMREAD32rr, Convert__Reg1_1__Reg1_0, Feature_In32BitMode, { MCK_GR32, MCK_GR32 }, 0},
  { 8827 /* vmreadl */, X86::VMREAD32rm, Convert__Mem325_1__Reg1_0, Feature_In32BitMode, { MCK_GR32, MCK_Mem32 }, 0},
  { 8835 /* vmreadq */, X86::VMREAD64rr, Convert__Reg1_1__Reg1_0, Feature_In64BitMode, { MCK_GR64, MCK_GR64 }, 0},
  { 8835 /* vmreadq */, X86::VMREAD64rm, Convert__Mem645_1__Reg1_0, Feature_In64BitMode, { MCK_GR64, MCK_Mem64 }, 0},
  { 8843 /* vmresume */, X86::VMRESUME, Convert_NoOperands, 0, {  }, 0},
  { 8843 /* vmresume */, X86::VMRESUME, Convert_NoOperands, 0, {  }, 1},
  { 8852 /* vmrun */, X86::VMRUN32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 0},
  { 8852 /* vmrun */, X86::VMRUN32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 1},
  { 8852 /* vmrun */, X86::VMRUN64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 0},
  { 8852 /* vmrun */, X86::VMRUN64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 1},
  { 8858 /* vmsave */, X86::VMSAVE32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 0},
  { 8858 /* vmsave */, X86::VMSAVE32, Convert_NoOperands, Feature_In32BitMode, { MCK_EAX }, 1},
  { 8858 /* vmsave */, X86::VMSAVE64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 0},
  { 8858 /* vmsave */, X86::VMSAVE64, Convert_NoOperands, Feature_In64BitMode, { MCK_RAX }, 1},
  { 8865 /* vmulpd */, X86::VMULPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8865 /* vmulpd */, X86::VMULPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8865 /* vmulpd */, X86::VMULPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8865 /* vmulpd */, X86::VMULPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8865 /* vmulpd */, X86::VMULPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8865 /* vmulpd */, X86::VMULPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8865 /* vmulpd */, X86::VMULPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8865 /* vmulpd */, X86::VMULPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8872 /* vmulps */, X86::VMULPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8872 /* vmulps */, X86::VMULPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8872 /* vmulps */, X86::VMULPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8872 /* vmulps */, X86::VMULPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8872 /* vmulps */, X86::VMULPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8872 /* vmulps */, X86::VMULPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8872 /* vmulps */, X86::VMULPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8872 /* vmulps */, X86::VMULPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8879 /* vmulsd */, X86::VMULSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8879 /* vmulsd */, X86::VMULSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8879 /* vmulsd */, X86::VMULSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 8879 /* vmulsd */, X86::VMULSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 8886 /* vmulss */, X86::VMULSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8886 /* vmulss */, X86::VMULSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8886 /* vmulss */, X86::VMULSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 8886 /* vmulss */, X86::VMULSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 8893 /* vmwrite */, X86::VMWRITE32rr, Convert__Reg1_0__Reg1_1, Feature_In32BitMode, { MCK_GR32, MCK_GR32 }, 1},
  { 8893 /* vmwrite */, X86::VMWRITE32rm, Convert__Reg1_0__Mem325_1, Feature_In32BitMode, { MCK_GR32, MCK_Mem32 }, 1},
  { 8893 /* vmwrite */, X86::VMWRITE64rr, Convert__Reg1_0__Reg1_1, Feature_In64BitMode, { MCK_GR64, MCK_GR64 }, 1},
  { 8893 /* vmwrite */, X86::VMWRITE64rm, Convert__Reg1_0__Mem645_1, Feature_In64BitMode, { MCK_GR64, MCK_Mem64 }, 1},
  { 8901 /* vmwritel */, X86::VMWRITE32rr, Convert__Reg1_1__Reg1_0, Feature_In32BitMode, { MCK_GR32, MCK_GR32 }, 0},
  { 8901 /* vmwritel */, X86::VMWRITE32rm, Convert__Reg1_1__Mem325_0, Feature_In32BitMode, { MCK_Mem32, MCK_GR32 }, 0},
  { 8910 /* vmwriteq */, X86::VMWRITE64rr, Convert__Reg1_1__Reg1_0, Feature_In64BitMode, { MCK_GR64, MCK_GR64 }, 0},
  { 8910 /* vmwriteq */, X86::VMWRITE64rm, Convert__Reg1_1__Mem645_0, Feature_In64BitMode, { MCK_Mem64, MCK_GR64 }, 0},
  { 8919 /* vmxoff */, X86::VMXOFF, Convert_NoOperands, 0, {  }, 0},
  { 8919 /* vmxoff */, X86::VMXOFF, Convert_NoOperands, 0, {  }, 1},
  { 8926 /* vmxon */, X86::VMXON, Convert__Mem645_0, 0, { MCK_Mem64 }, 0},
  { 8926 /* vmxon */, X86::VMXON, Convert__Mem645_0, 0, { MCK_Mem64 }, 1},
  { 8932 /* vorpd */, X86::VFsORPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8932 /* vorpd */, X86::VORPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8932 /* vorpd */, X86::VFsORPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8932 /* vorpd */, X86::VORPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8932 /* vorpd */, X86::VFsORPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8932 /* vorpd */, X86::VORPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8932 /* vorpd */, X86::VORPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8932 /* vorpd */, X86::VORPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8932 /* vorpd */, X86::VORPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8932 /* vorpd */, X86::VFsORPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8932 /* vorpd */, X86::VORPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8932 /* vorpd */, X86::VORPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8938 /* vorps */, X86::VFsORPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8938 /* vorps */, X86::VORPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8938 /* vorps */, X86::VFsORPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8938 /* vorps */, X86::VORPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8938 /* vorps */, X86::VFsORPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8938 /* vorps */, X86::VORPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8938 /* vorps */, X86::VORPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8938 /* vorps */, X86::VORPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8938 /* vorps */, X86::VORPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8938 /* vorps */, X86::VFsORPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8938 /* vorps */, X86::VORPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8938 /* vorps */, X86::VORPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8944 /* vpabsb */, X86::VPABSBrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8944 /* vpabsb */, X86::VPABSBrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8944 /* vpabsb */, X86::VPABSBrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8944 /* vpabsb */, X86::VPABSBrr256, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8944 /* vpabsb */, X86::VPABSBrr256, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8944 /* vpabsb */, X86::VPABSBrm256, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8944 /* vpabsb */, X86::VPABSBrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8944 /* vpabsb */, X86::VPABSBrm256, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8951 /* vpabsd */, X86::VPABSDrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8951 /* vpabsd */, X86::VPABSDrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8951 /* vpabsd */, X86::VPABSDrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8951 /* vpabsd */, X86::VPABSDrr256, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8951 /* vpabsd */, X86::VPABSDrr256, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8951 /* vpabsd */, X86::VPABSDrm256, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8951 /* vpabsd */, X86::VPABSDrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8951 /* vpabsd */, X86::VPABSDrm256, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8958 /* vpabsw */, X86::VPABSWrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 8958 /* vpabsw */, X86::VPABSWrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 8958 /* vpabsw */, X86::VPABSWrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 8958 /* vpabsw */, X86::VPABSWrr256, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 8958 /* vpabsw */, X86::VPABSWrr256, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 8958 /* vpabsw */, X86::VPABSWrm256, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 8958 /* vpabsw */, X86::VPABSWrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 8958 /* vpabsw */, X86::VPABSWrm256, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 8965 /* vpackssdw */, X86::VPACKSSDWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8965 /* vpackssdw */, X86::VPACKSSDWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8965 /* vpackssdw */, X86::VPACKSSDWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8965 /* vpackssdw */, X86::VPACKSSDWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8965 /* vpackssdw */, X86::VPACKSSDWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8965 /* vpackssdw */, X86::VPACKSSDWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8965 /* vpackssdw */, X86::VPACKSSDWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8965 /* vpackssdw */, X86::VPACKSSDWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8975 /* vpacksswb */, X86::VPACKSSWBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8975 /* vpacksswb */, X86::VPACKSSWBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8975 /* vpacksswb */, X86::VPACKSSWBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8975 /* vpacksswb */, X86::VPACKSSWBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8975 /* vpacksswb */, X86::VPACKSSWBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8975 /* vpacksswb */, X86::VPACKSSWBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8975 /* vpacksswb */, X86::VPACKSSWBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8975 /* vpacksswb */, X86::VPACKSSWBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8985 /* vpackusdw */, X86::VPACKUSDWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8985 /* vpackusdw */, X86::VPACKUSDWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8985 /* vpackusdw */, X86::VPACKUSDWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8985 /* vpackusdw */, X86::VPACKUSDWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8985 /* vpackusdw */, X86::VPACKUSDWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8985 /* vpackusdw */, X86::VPACKUSDWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8985 /* vpackusdw */, X86::VPACKUSDWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8985 /* vpackusdw */, X86::VPACKUSDWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 8995 /* vpackuswb */, X86::VPACKUSWBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 8995 /* vpackuswb */, X86::VPACKUSWBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 8995 /* vpackuswb */, X86::VPACKUSWBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 8995 /* vpackuswb */, X86::VPACKUSWBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 8995 /* vpackuswb */, X86::VPACKUSWBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 8995 /* vpackuswb */, X86::VPACKUSWBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 8995 /* vpackuswb */, X86::VPACKUSWBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 8995 /* vpackuswb */, X86::VPACKUSWBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9005 /* vpaddb */, X86::VPADDBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9005 /* vpaddb */, X86::VPADDBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9005 /* vpaddb */, X86::VPADDBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9005 /* vpaddb */, X86::VPADDBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9005 /* vpaddb */, X86::VPADDBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9005 /* vpaddb */, X86::VPADDBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9005 /* vpaddb */, X86::VPADDBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9005 /* vpaddb */, X86::VPADDBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9012 /* vpaddd */, X86::VPADDDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9012 /* vpaddd */, X86::VPADDDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9012 /* vpaddd */, X86::VPADDDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9012 /* vpaddd */, X86::VPADDDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9012 /* vpaddd */, X86::VPADDDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9012 /* vpaddd */, X86::VPADDDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9012 /* vpaddd */, X86::VPADDDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9012 /* vpaddd */, X86::VPADDDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9019 /* vpaddq */, X86::VPADDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9019 /* vpaddq */, X86::VPADDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9019 /* vpaddq */, X86::VPADDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9019 /* vpaddq */, X86::VPADDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9019 /* vpaddq */, X86::VPADDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9019 /* vpaddq */, X86::VPADDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9019 /* vpaddq */, X86::VPADDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9019 /* vpaddq */, X86::VPADDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9026 /* vpaddsb */, X86::VPADDSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9026 /* vpaddsb */, X86::VPADDSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9026 /* vpaddsb */, X86::VPADDSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9026 /* vpaddsb */, X86::VPADDSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9026 /* vpaddsb */, X86::VPADDSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9026 /* vpaddsb */, X86::VPADDSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9026 /* vpaddsb */, X86::VPADDSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9026 /* vpaddsb */, X86::VPADDSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9034 /* vpaddsw */, X86::VPADDSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9034 /* vpaddsw */, X86::VPADDSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9034 /* vpaddsw */, X86::VPADDSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9034 /* vpaddsw */, X86::VPADDSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9034 /* vpaddsw */, X86::VPADDSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9034 /* vpaddsw */, X86::VPADDSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9034 /* vpaddsw */, X86::VPADDSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9034 /* vpaddsw */, X86::VPADDSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9042 /* vpaddusb */, X86::VPADDUSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9042 /* vpaddusb */, X86::VPADDUSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9042 /* vpaddusb */, X86::VPADDUSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9042 /* vpaddusb */, X86::VPADDUSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9042 /* vpaddusb */, X86::VPADDUSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9042 /* vpaddusb */, X86::VPADDUSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9042 /* vpaddusb */, X86::VPADDUSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9042 /* vpaddusb */, X86::VPADDUSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9051 /* vpaddusw */, X86::VPADDUSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9051 /* vpaddusw */, X86::VPADDUSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9051 /* vpaddusw */, X86::VPADDUSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9051 /* vpaddusw */, X86::VPADDUSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9051 /* vpaddusw */, X86::VPADDUSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9051 /* vpaddusw */, X86::VPADDUSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9051 /* vpaddusw */, X86::VPADDUSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9051 /* vpaddusw */, X86::VPADDUSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9060 /* vpaddw */, X86::VPADDWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9060 /* vpaddw */, X86::VPADDWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9060 /* vpaddw */, X86::VPADDWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9060 /* vpaddw */, X86::VPADDWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9060 /* vpaddw */, X86::VPADDWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9060 /* vpaddw */, X86::VPADDWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9060 /* vpaddw */, X86::VPADDWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9060 /* vpaddw */, X86::VPADDWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9067 /* vpalignr */, X86::VPALIGNR128rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9067 /* vpalignr */, X86::VPALIGNR128rm, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9067 /* vpalignr */, X86::VPALIGNR256rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9067 /* vpalignr */, X86::VPALIGNR256rm, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9067 /* vpalignr */, X86::VPALIGNR128rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9067 /* vpalignr */, X86::VPALIGNR256rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9067 /* vpalignr */, X86::VPALIGNR128rm, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9067 /* vpalignr */, X86::VPALIGNR256rm, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9076 /* vpand */, X86::VPANDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9076 /* vpand */, X86::VPANDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9076 /* vpand */, X86::VPANDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9076 /* vpand */, X86::VPANDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9076 /* vpand */, X86::VPANDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9076 /* vpand */, X86::VPANDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9076 /* vpand */, X86::VPANDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9076 /* vpand */, X86::VPANDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9082 /* vpandn */, X86::VPANDNrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9082 /* vpandn */, X86::VPANDNrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9082 /* vpandn */, X86::VPANDNrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9082 /* vpandn */, X86::VPANDNYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9082 /* vpandn */, X86::VPANDNYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9082 /* vpandn */, X86::VPANDNYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9082 /* vpandn */, X86::VPANDNrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9082 /* vpandn */, X86::VPANDNYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9089 /* vpavgb */, X86::VPAVGBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9089 /* vpavgb */, X86::VPAVGBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9089 /* vpavgb */, X86::VPAVGBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9089 /* vpavgb */, X86::VPAVGBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9089 /* vpavgb */, X86::VPAVGBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9089 /* vpavgb */, X86::VPAVGBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9089 /* vpavgb */, X86::VPAVGBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9089 /* vpavgb */, X86::VPAVGBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9096 /* vpavgw */, X86::VPAVGWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9096 /* vpavgw */, X86::VPAVGWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9096 /* vpavgw */, X86::VPAVGWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9096 /* vpavgw */, X86::VPAVGWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9096 /* vpavgw */, X86::VPAVGWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9096 /* vpavgw */, X86::VPAVGWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9096 /* vpavgw */, X86::VPAVGWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9096 /* vpavgw */, X86::VPAVGWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9103 /* vpblendd */, X86::VPBLENDDrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 9103 /* vpblendd */, X86::VPBLENDDrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 9103 /* vpblendd */, X86::VPBLENDDYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 9103 /* vpblendd */, X86::VPBLENDDYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 9103 /* vpblendd */, X86::VPBLENDDrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9103 /* vpblendd */, X86::VPBLENDDYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9103 /* vpblendd */, X86::VPBLENDDrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9103 /* vpblendd */, X86::VPBLENDDYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9112 /* vpblendvb */, X86::VPBLENDVBrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9112 /* vpblendvb */, X86::VPBLENDVBrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9112 /* vpblendvb */, X86::VPBLENDVBrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9112 /* vpblendvb */, X86::VPBLENDVBrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9112 /* vpblendvb */, X86::VPBLENDVBYrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9112 /* vpblendvb */, X86::VPBLENDVBYrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9112 /* vpblendvb */, X86::VPBLENDVBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 9112 /* vpblendvb */, X86::VPBLENDVBYrm, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9122 /* vpblendw */, X86::VPBLENDWrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmZExtu32u8 }, 1},
  { 9122 /* vpblendw */, X86::VPBLENDWrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__ImmZExtu32u81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_ImmZExtu32u8 }, 1},
  { 9122 /* vpblendw */, X86::VPBLENDWYrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_ImmZExtu32u8 }, 1},
  { 9122 /* vpblendw */, X86::VPBLENDWYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__ImmZExtu32u81_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_ImmZExtu32u8 }, 1},
  { 9122 /* vpblendw */, X86::VPBLENDWrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9122 /* vpblendw */, X86::VPBLENDWYrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9122 /* vpblendw */, X86::VPBLENDWrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9122 /* vpblendw */, X86::VPBLENDWYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__ImmZExtu32u81_0, 0, { MCK_ImmZExtu32u8, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBrm, Convert__Reg1_0__Mem85_1, 0, { MCK_FR32, MCK_Mem8 }, 1},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBYrm, Convert__Reg1_0__Mem85_1, 0, { MCK_VR256, MCK_Mem8 }, 1},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBrm, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_FR32 }, 0},
  { 9131 /* vpbroadcastb */, X86::VPBROADCASTBYrm, Convert__Reg1_1__Mem85_0, 0, { MCK_Mem8, MCK_VR256 }, 0},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 9144 /* vpbroadcastd */, X86::VPBROADCASTDYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQYrm, Convert__Reg1_0__Mem645_1, 0, { MCK_VR256, MCK_Mem64 }, 1},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 9157 /* vpbroadcastq */, X86::VPBROADCASTQYrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_VR256 }, 0},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWrm, Convert__Reg1_0__Mem165_1, 0, { MCK_FR32, MCK_Mem16 }, 1},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWYrm, Convert__Reg1_0__Mem165_1, 0, { MCK_VR256, MCK_Mem16 }, 1},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_FR32 }, 0},
  { 9170 /* vpbroadcastw */, X86::VPBROADCASTWYrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_VR256 }, 0},
  { 9183 /* vpclmulhqhqdq */, X86::VPCLMULQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0__imm_17, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9183 /* vpclmulhqhqdq */, X86::VPCLMULQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2__imm_17, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9183 /* vpclmulhqhqdq */, X86::VPCLMULQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2__imm_17, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9183 /* vpclmulhqhqdq */, X86::VPCLMULQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0__imm_17, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9197 /* vpclmulhqlqdq */, X86::VPCLMULQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0__imm_1, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9197 /* vpclmulhqlqdq */, X86::VPCLMULQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2__imm_1, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9197 /* vpclmulhqlqdq */, X86::VPCLMULQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2__imm_1, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9197 /* vpclmulhqlqdq */, X86::VPCLMULQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0__imm_1, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9211 /* vpclmullqhqdq */, X86::VPCLMULQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0__imm_16, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9211 /* vpclmullqhqdq */, X86::VPCLMULQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2__imm_16, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9211 /* vpclmullqhqdq */, X86::VPCLMULQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2__imm_16, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9211 /* vpclmullqhqdq */, X86::VPCLMULQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0__imm_16, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9225 /* vpclmullqlqdq */, X86::VPCLMULQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0__imm_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9225 /* vpclmullqlqdq */, X86::VPCLMULQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2__imm_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9225 /* vpclmullqlqdq */, X86::VPCLMULQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2__imm_0, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9225 /* vpclmullqlqdq */, X86::VPCLMULQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0__imm_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9239 /* vpclmulqdq */, X86::VPCLMULQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9239 /* vpclmulqdq */, X86::VPCLMULQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9239 /* vpclmulqdq */, X86::VPCLMULQDQrr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9239 /* vpclmulqdq */, X86::VPCLMULQDQrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVrm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVmr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVmr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrrY, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVrmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVmrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256 }, 1},
  { 9250 /* vpcmov */, X86::VPCMOVmrY, Convert__Reg1_3__Reg1_2__Mem2565_1__Reg1_0, 0, { MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9250 /* vpcmov */, X86::VPCMOVrmY, Convert__Reg1_3__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9257 /* vpcmpeqb */, X86::VPCMPEQBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9266 /* vpcmpeqd */, X86::VPCMPEQDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9275 /* vpcmpeqq */, X86::VPCMPEQQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9284 /* vpcmpeqw */, X86::VPCMPEQWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9293 /* vpcmpestri */, X86::VPCMPESTRIrr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9293 /* vpcmpestri */, X86::VPCMPESTRIrm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9293 /* vpcmpestri */, X86::VPCMPESTRIrr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9293 /* vpcmpestri */, X86::VPCMPESTRIrm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9304 /* vpcmpestrm */, X86::VPCMPESTRM128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9304 /* vpcmpestrm */, X86::VPCMPESTRM128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9304 /* vpcmpestrm */, X86::VPCMPESTRM128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9304 /* vpcmpestrm */, X86::VPCMPESTRM128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9315 /* vpcmpgtb */, X86::VPCMPGTBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9324 /* vpcmpgtd */, X86::VPCMPGTDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9333 /* vpcmpgtq */, X86::VPCMPGTQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9342 /* vpcmpgtw */, X86::VPCMPGTWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9351 /* vpcmpistri */, X86::VPCMPISTRIrr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9351 /* vpcmpistri */, X86::VPCMPISTRIrm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9351 /* vpcmpistri */, X86::VPCMPISTRIrr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9351 /* vpcmpistri */, X86::VPCMPISTRIrm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9362 /* vpcmpistrm */, X86::VPCMPISTRM128rr, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9362 /* vpcmpistrm */, X86::VPCMPISTRM128rm, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9362 /* vpcmpistrm */, X86::VPCMPISTRM128rr, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9362 /* vpcmpistrm */, X86::VPCMPISTRM128rm, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9373 /* vpcomb */, X86::VPCOMBri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9373 /* vpcomb */, X86::VPCOMBmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9373 /* vpcomb */, X86::VPCOMBri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9373 /* vpcomb */, X86::VPCOMBmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9380 /* vpcomd */, X86::VPCOMDri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9380 /* vpcomd */, X86::VPCOMDmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9380 /* vpcomd */, X86::VPCOMDri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9380 /* vpcomd */, X86::VPCOMDmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9387 /* vpcomq */, X86::VPCOMQri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9387 /* vpcomq */, X86::VPCOMQmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9387 /* vpcomq */, X86::VPCOMQri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9387 /* vpcomq */, X86::VPCOMQmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9394 /* vpcomub */, X86::VPCOMUBri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9394 /* vpcomub */, X86::VPCOMUBmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9394 /* vpcomub */, X86::VPCOMUBri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9394 /* vpcomub */, X86::VPCOMUBmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9402 /* vpcomud */, X86::VPCOMUDri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9402 /* vpcomud */, X86::VPCOMUDmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9402 /* vpcomud */, X86::VPCOMUDri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9402 /* vpcomud */, X86::VPCOMUDmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9410 /* vpcomuq */, X86::VPCOMUQri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9410 /* vpcomuq */, X86::VPCOMUQmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9410 /* vpcomuq */, X86::VPCOMUQri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9410 /* vpcomuq */, X86::VPCOMUQmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9418 /* vpcomuw */, X86::VPCOMUWri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9418 /* vpcomuw */, X86::VPCOMUWmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9418 /* vpcomuw */, X86::VPCOMUWri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9418 /* vpcomuw */, X86::VPCOMUWmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9426 /* vpcomw */, X86::VPCOMWri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9426 /* vpcomw */, X86::VPCOMWmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9426 /* vpcomw */, X86::VPCOMWri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9426 /* vpcomw */, X86::VPCOMWmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9433 /* vperm2f128 */, X86::VPERM2F128rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9433 /* vperm2f128 */, X86::VPERM2F128rm, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9433 /* vperm2f128 */, X86::VPERM2F128rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9433 /* vperm2f128 */, X86::VPERM2F128rm, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9444 /* vperm2i128 */, X86::VPERM2I128rr, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9444 /* vperm2i128 */, X86::VPERM2I128rm, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9444 /* vperm2i128 */, X86::VPERM2I128rr, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9444 /* vperm2i128 */, X86::VPERM2I128rm, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9455 /* vpermd */, X86::VPERMDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9455 /* vpermd */, X86::VPERMDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9455 /* vpermd */, X86::VPERMDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9455 /* vpermd */, X86::VPERMDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDmr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDmrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256, MCK_Imm }, 1},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrr, Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDmr, Convert__Reg1_4__Reg1_3__Mem1285_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrrY, Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDmrY, Convert__Reg1_4__Reg1_3__Mem2565_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrm, Convert__Reg1_4__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9462 /* vpermil2pd */, X86::VPERMIL2PDrmY, Convert__Reg1_4__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSmr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3__Imm1_4, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrrY, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrmY, Convert__Reg1_0__Reg1_1__Reg1_2__Mem2565_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSmrY, Convert__Reg1_0__Reg1_1__Mem2565_2__Reg1_3__Imm1_4, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_VR256, MCK_Imm }, 1},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrr, Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSmr, Convert__Reg1_4__Reg1_3__Mem1285_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrrY, Convert__Reg1_4__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSmrY, Convert__Reg1_4__Reg1_3__Mem2565_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrm, Convert__Reg1_4__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9473 /* vpermil2ps */, X86::VPERMIL2PSrmY, Convert__Reg1_4__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9484 /* vpermilpd */, X86::VPERMILPDri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9484 /* vpermilpd */, X86::VPERMILPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9494 /* vpermilps */, X86::VPERMILPSri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9494 /* vpermilps */, X86::VPERMILPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9504 /* vpermpd */, X86::VPERMPDYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9504 /* vpermpd */, X86::VPERMPDYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9504 /* vpermpd */, X86::VPERMPDYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 9504 /* vpermpd */, X86::VPERMPDYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 9512 /* vpermps */, X86::VPERMPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9512 /* vpermps */, X86::VPERMPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9512 /* vpermps */, X86::VPERMPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9512 /* vpermps */, X86::VPERMPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9520 /* vpermq */, X86::VPERMQYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 9520 /* vpermq */, X86::VPERMQYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 9520 /* vpermq */, X86::VPERMQYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 9520 /* vpermq */, X86::VPERMQYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 9527 /* vpextrb */, X86::VPEXTRBrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9527 /* vpextrb */, X86::VPEXTRBrr64, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9527 /* vpextrb */, X86::VPEXTRBrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 9527 /* vpextrb */, X86::VPEXTRBrr64, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR64 }, 0},
  { 9527 /* vpextrb */, X86::VPEXTRBmr, Convert__Mem85_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem8 }, 0},
  { 9527 /* vpextrb */, X86::VPEXTRBmr, Convert__Mem85_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem8, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9535 /* vpextrd */, X86::VPEXTRDrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9535 /* vpextrd */, X86::VPEXTRDrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 9535 /* vpextrd */, X86::VPEXTRDmr, Convert__Mem325_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem32 }, 0},
  { 9535 /* vpextrd */, X86::VPEXTRDmr, Convert__Mem325_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9543 /* vpextrq */, X86::VPEXTRQrr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9543 /* vpextrq */, X86::VPEXTRQrr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR64 }, 0},
  { 9543 /* vpextrq */, X86::VPEXTRQmr, Convert__Mem645_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem64 }, 0},
  { 9543 /* vpextrq */, X86::VPEXTRQmr, Convert__Mem645_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem64, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9551 /* vpextrw */, X86::VPEXTRWri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_GR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9551 /* vpextrw */, X86::VPEXTRWri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_GR32 }, 0},
  { 9551 /* vpextrw */, X86::VPEXTRWmr, Convert__Mem165_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_Mem16 }, 0},
  { 9551 /* vpextrw */, X86::VPEXTRWmr, Convert__Mem165_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_Mem16, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 9559 /* vpgatherdd */, X86::VPGATHERDDrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 0},
  { 9559 /* vpgatherdd */, X86::VPGATHERDDrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 1},
  { 9559 /* vpgatherdd */, X86::VPGATHERDDYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1, 0, { MCK_VR256, MCK_MemVY32, MCK_VR256 }, 0},
  { 9559 /* vpgatherdd */, X86::VPGATHERDDYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1, 0, { MCK_VR256, MCK_MemVY32, MCK_VR256 }, 1},
  { 9570 /* vpgatherdq */, X86::VPGATHERDQrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 0},
  { 9570 /* vpgatherdq */, X86::VPGATHERDQrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 1},
  { 9570 /* vpgatherdq */, X86::VPGATHERDQYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_VR256, MCK_MemVX64, MCK_VR256 }, 0},
  { 9570 /* vpgatherdq */, X86::VPGATHERDQYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_VR256, MCK_MemVX64, MCK_VR256 }, 1},
  { 9581 /* vpgatherqd */, X86::VPGATHERQDrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 0},
  { 9581 /* vpgatherqd */, X86::VPGATHERQDrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX325_1__Tie1, 0, { MCK_FR32, MCK_MemVX32, MCK_FR32 }, 1},
  { 9581 /* vpgatherqd */, X86::VPGATHERQDYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY325_1__Tie1, 0, { MCK_FR32, MCK_MemVY32, MCK_FR32 }, 0},
  { 9581 /* vpgatherqd */, X86::VPGATHERQDYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY325_1__Tie1, 0, { MCK_FR32, MCK_MemVY32, MCK_FR32 }, 1},
  { 9592 /* vpgatherqq */, X86::VPGATHERQQrm, Convert__Reg1_2__Reg1_0__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 0},
  { 9592 /* vpgatherqq */, X86::VPGATHERQQrm, Convert__Reg1_0__Reg1_2__Tie0__MemVX645_1__Tie1, 0, { MCK_FR32, MCK_MemVX64, MCK_FR32 }, 1},
  { 9592 /* vpgatherqq */, X86::VPGATHERQQYrm, Convert__Reg1_2__Reg1_0__Tie0__MemVY645_1__Tie1, 0, { MCK_VR256, MCK_MemVY64, MCK_VR256 }, 0},
  { 9592 /* vpgatherqq */, X86::VPGATHERQQYrm, Convert__Reg1_0__Reg1_2__Tie0__MemVY645_1__Tie1, 0, { MCK_VR256, MCK_MemVY64, MCK_VR256 }, 1},
  { 9603 /* vphaddbd */, X86::VPHADDBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9603 /* vphaddbd */, X86::VPHADDBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9603 /* vphaddbd */, X86::VPHADDBDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9603 /* vphaddbd */, X86::VPHADDBDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9612 /* vphaddbq */, X86::VPHADDBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9612 /* vphaddbq */, X86::VPHADDBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9612 /* vphaddbq */, X86::VPHADDBQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9612 /* vphaddbq */, X86::VPHADDBQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9621 /* vphaddbw */, X86::VPHADDBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9621 /* vphaddbw */, X86::VPHADDBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9621 /* vphaddbw */, X86::VPHADDBWrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9621 /* vphaddbw */, X86::VPHADDBWrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9630 /* vphaddd */, X86::VPHADDDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9630 /* vphaddd */, X86::VPHADDDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9630 /* vphaddd */, X86::VPHADDDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9630 /* vphaddd */, X86::VPHADDDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9630 /* vphaddd */, X86::VPHADDDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9630 /* vphaddd */, X86::VPHADDDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9630 /* vphaddd */, X86::VPHADDDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9630 /* vphaddd */, X86::VPHADDDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9638 /* vphadddq */, X86::VPHADDDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9638 /* vphadddq */, X86::VPHADDDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9638 /* vphadddq */, X86::VPHADDDQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9638 /* vphadddq */, X86::VPHADDDQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9647 /* vphaddsw */, X86::VPHADDSWrr128, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9647 /* vphaddsw */, X86::VPHADDSWrr128, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9647 /* vphaddsw */, X86::VPHADDSWrm128, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9647 /* vphaddsw */, X86::VPHADDSWrr256, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9647 /* vphaddsw */, X86::VPHADDSWrr256, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9647 /* vphaddsw */, X86::VPHADDSWrm256, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9647 /* vphaddsw */, X86::VPHADDSWrm128, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9647 /* vphaddsw */, X86::VPHADDSWrm256, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9656 /* vphaddubd */, X86::VPHADDUBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9656 /* vphaddubd */, X86::VPHADDUBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9656 /* vphaddubd */, X86::VPHADDUBDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9656 /* vphaddubd */, X86::VPHADDUBDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9666 /* vphaddubq */, X86::VPHADDUBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9666 /* vphaddubq */, X86::VPHADDUBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9666 /* vphaddubq */, X86::VPHADDUBQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9666 /* vphaddubq */, X86::VPHADDUBQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9676 /* vphaddubw */, X86::VPHADDUBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9676 /* vphaddubw */, X86::VPHADDUBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9676 /* vphaddubw */, X86::VPHADDUBWrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9676 /* vphaddubw */, X86::VPHADDUBWrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9686 /* vphaddudq */, X86::VPHADDUDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9686 /* vphaddudq */, X86::VPHADDUDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9686 /* vphaddudq */, X86::VPHADDUDQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9686 /* vphaddudq */, X86::VPHADDUDQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9696 /* vphadduwd */, X86::VPHADDUWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9696 /* vphadduwd */, X86::VPHADDUWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9696 /* vphadduwd */, X86::VPHADDUWDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9696 /* vphadduwd */, X86::VPHADDUWDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9706 /* vphadduwq */, X86::VPHADDUWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9706 /* vphadduwq */, X86::VPHADDUWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9706 /* vphadduwq */, X86::VPHADDUWQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9706 /* vphadduwq */, X86::VPHADDUWQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9716 /* vphaddw */, X86::VPHADDWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9716 /* vphaddw */, X86::VPHADDWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9716 /* vphaddw */, X86::VPHADDWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9716 /* vphaddw */, X86::VPHADDWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9716 /* vphaddw */, X86::VPHADDWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9716 /* vphaddw */, X86::VPHADDWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9716 /* vphaddw */, X86::VPHADDWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9716 /* vphaddw */, X86::VPHADDWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9724 /* vphaddwd */, X86::VPHADDWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9724 /* vphaddwd */, X86::VPHADDWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9724 /* vphaddwd */, X86::VPHADDWDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9724 /* vphaddwd */, X86::VPHADDWDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9733 /* vphaddwq */, X86::VPHADDWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9733 /* vphaddwq */, X86::VPHADDWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9733 /* vphaddwq */, X86::VPHADDWQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9733 /* vphaddwq */, X86::VPHADDWQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9742 /* vphminposuw */, X86::VPHMINPOSUWrr128, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9742 /* vphminposuw */, X86::VPHMINPOSUWrr128, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9742 /* vphminposuw */, X86::VPHMINPOSUWrm128, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9742 /* vphminposuw */, X86::VPHMINPOSUWrm128, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9754 /* vphsubbw */, X86::VPHSUBBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9754 /* vphsubbw */, X86::VPHSUBBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9754 /* vphsubbw */, X86::VPHSUBBWrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9754 /* vphsubbw */, X86::VPHSUBBWrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9763 /* vphsubd */, X86::VPHSUBDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9763 /* vphsubd */, X86::VPHSUBDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9763 /* vphsubd */, X86::VPHSUBDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9763 /* vphsubd */, X86::VPHSUBDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9763 /* vphsubd */, X86::VPHSUBDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9763 /* vphsubd */, X86::VPHSUBDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9763 /* vphsubd */, X86::VPHSUBDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9763 /* vphsubd */, X86::VPHSUBDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9771 /* vphsubdq */, X86::VPHSUBDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9771 /* vphsubdq */, X86::VPHSUBDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9771 /* vphsubdq */, X86::VPHSUBDQrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9771 /* vphsubdq */, X86::VPHSUBDQrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrr128, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrr128, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrm128, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrr256, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrr256, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrm256, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrm128, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9780 /* vphsubsw */, X86::VPHSUBSWrm256, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9789 /* vphsubw */, X86::VPHSUBWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9789 /* vphsubw */, X86::VPHSUBWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9789 /* vphsubw */, X86::VPHSUBWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9789 /* vphsubw */, X86::VPHSUBWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9789 /* vphsubw */, X86::VPHSUBWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9789 /* vphsubw */, X86::VPHSUBWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9789 /* vphsubw */, X86::VPHSUBWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9789 /* vphsubw */, X86::VPHSUBWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9797 /* vphsubwd */, X86::VPHSUBWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 9797 /* vphsubwd */, X86::VPHSUBWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 9797 /* vphsubwd */, X86::VPHSUBWDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 9797 /* vphsubwd */, X86::VPHSUBWDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 9806 /* vpinsrb */, X86::VPINSRBrr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 9806 /* vpinsrb */, X86::VPINSRBrm, Convert__Reg1_0__Reg1_1__Mem85_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem8, MCK_ImmSExti32i8 }, 1},
  { 9806 /* vpinsrb */, X86::VPINSRBrr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 9806 /* vpinsrb */, X86::VPINSRBrm, Convert__Reg1_3__Reg1_2__Mem85_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem8, MCK_FR32, MCK_FR32 }, 0},
  { 9814 /* vpinsrd */, X86::VPINSRDrr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 9814 /* vpinsrd */, X86::VPINSRDrm, Convert__Reg1_0__Reg1_1__Mem325_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 9814 /* vpinsrd */, X86::VPINSRDrr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 9814 /* vpinsrd */, X86::VPINSRDrm, Convert__Reg1_3__Reg1_2__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 9822 /* vpinsrq */, X86::VPINSRQrr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_GR64, MCK_ImmSExti32i8 }, 1},
  { 9822 /* vpinsrq */, X86::VPINSRQrm, Convert__Reg1_0__Reg1_1__Mem645_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem64, MCK_ImmSExti32i8 }, 1},
  { 9822 /* vpinsrq */, X86::VPINSRQrr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR64, MCK_FR32, MCK_FR32 }, 0},
  { 9822 /* vpinsrq */, X86::VPINSRQrm, Convert__Reg1_3__Reg1_2__Mem645_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 9830 /* vpinsrw */, X86::VPINSRWrri, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 9830 /* vpinsrw */, X86::VPINSRWrr64i, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_GR64, MCK_ImmSExti32i8 }, 1},
  { 9830 /* vpinsrw */, X86::VPINSRWrmi, Convert__Reg1_0__Reg1_1__Mem165_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem16, MCK_ImmSExti32i8 }, 1},
  { 9830 /* vpinsrw */, X86::VPINSRWrri, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32, MCK_FR32, MCK_FR32 }, 0},
  { 9830 /* vpinsrw */, X86::VPINSRWrr64i, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR64, MCK_FR32, MCK_FR32 }, 0},
  { 9830 /* vpinsrw */, X86::VPINSRWrmi, Convert__Reg1_3__Reg1_2__Mem165_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem16, MCK_FR32, MCK_FR32 }, 0},
  { 9838 /* vpmacsdd */, X86::VPMACSDDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9838 /* vpmacsdd */, X86::VPMACSDDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9838 /* vpmacsdd */, X86::VPMACSDDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9838 /* vpmacsdd */, X86::VPMACSDDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9847 /* vpmacsdqh */, X86::VPMACSDQHrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9847 /* vpmacsdqh */, X86::VPMACSDQHrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9847 /* vpmacsdqh */, X86::VPMACSDQHrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9847 /* vpmacsdqh */, X86::VPMACSDQHrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9857 /* vpmacsdql */, X86::VPMACSDQLrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9857 /* vpmacsdql */, X86::VPMACSDQLrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9857 /* vpmacsdql */, X86::VPMACSDQLrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9857 /* vpmacsdql */, X86::VPMACSDQLrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9867 /* vpmacssdd */, X86::VPMACSSDDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9867 /* vpmacssdd */, X86::VPMACSSDDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9867 /* vpmacssdd */, X86::VPMACSSDDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9867 /* vpmacssdd */, X86::VPMACSSDDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9877 /* vpmacssdqh */, X86::VPMACSSDQHrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9877 /* vpmacssdqh */, X86::VPMACSSDQHrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9877 /* vpmacssdqh */, X86::VPMACSSDQHrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9877 /* vpmacssdqh */, X86::VPMACSSDQHrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9888 /* vpmacssdql */, X86::VPMACSSDQLrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9888 /* vpmacssdql */, X86::VPMACSSDQLrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9888 /* vpmacssdql */, X86::VPMACSSDQLrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9888 /* vpmacssdql */, X86::VPMACSSDQLrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9899 /* vpmacsswd */, X86::VPMACSSWDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9899 /* vpmacsswd */, X86::VPMACSSWDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9899 /* vpmacsswd */, X86::VPMACSSWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9899 /* vpmacsswd */, X86::VPMACSSWDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9909 /* vpmacssww */, X86::VPMACSSWWrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9909 /* vpmacssww */, X86::VPMACSSWWrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9909 /* vpmacssww */, X86::VPMACSSWWrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9909 /* vpmacssww */, X86::VPMACSSWWrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9919 /* vpmacswd */, X86::VPMACSWDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9919 /* vpmacswd */, X86::VPMACSWDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9919 /* vpmacswd */, X86::VPMACSWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9919 /* vpmacswd */, X86::VPMACSWDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9928 /* vpmacsww */, X86::VPMACSWWrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9928 /* vpmacsww */, X86::VPMACSWWrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9928 /* vpmacsww */, X86::VPMACSWWrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9928 /* vpmacsww */, X86::VPMACSWWrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9937 /* vpmadcsswd */, X86::VPMADCSSWDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9937 /* vpmadcsswd */, X86::VPMADCSSWDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9937 /* vpmadcsswd */, X86::VPMADCSSWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9937 /* vpmadcsswd */, X86::VPMADCSSWDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9948 /* vpmadcswd */, X86::VPMADCSWDrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9948 /* vpmadcswd */, X86::VPMADCSWDrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9948 /* vpmadcswd */, X86::VPMADCSWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 9948 /* vpmadcswd */, X86::VPMADCSWDrm, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrr128, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrr128, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrm128, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrr256, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrr256, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrm256, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrm128, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9958 /* vpmaddubsw */, X86::VPMADDUBSWrm256, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9969 /* vpmaddwd */, X86::VPMADDWDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 9969 /* vpmaddwd */, X86::VPMADDWDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 9969 /* vpmaddwd */, X86::VPMADDWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9969 /* vpmaddwd */, X86::VPMADDWDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 9969 /* vpmaddwd */, X86::VPMADDWDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 9969 /* vpmaddwd */, X86::VPMADDWDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9969 /* vpmaddwd */, X86::VPMADDWDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9969 /* vpmaddwd */, X86::VPMADDWDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDmr, Convert__Mem1285_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 0},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDYmr, Convert__Mem2565_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 0},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDmr, Convert__Mem1285_0__Reg1_1__Reg1_2, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 1},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9978 /* vpmaskmovd */, X86::VPMASKMOVDYmr, Convert__Mem2565_0__Reg1_1__Reg1_2, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 1},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQmr, Convert__Mem1285_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 0},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQYmr, Convert__Mem2565_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 0},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQmr, Convert__Mem1285_0__Reg1_1__Reg1_2, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 1},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 9989 /* vpmaskmovq */, X86::VPMASKMOVQYmr, Convert__Mem2565_0__Reg1_1__Reg1_2, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 1},
  { 10000 /* vpmaxsb */, X86::VPMAXSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10000 /* vpmaxsb */, X86::VPMAXSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10000 /* vpmaxsb */, X86::VPMAXSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10000 /* vpmaxsb */, X86::VPMAXSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10000 /* vpmaxsb */, X86::VPMAXSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10000 /* vpmaxsb */, X86::VPMAXSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10000 /* vpmaxsb */, X86::VPMAXSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10000 /* vpmaxsb */, X86::VPMAXSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10008 /* vpmaxsd */, X86::VPMAXSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10008 /* vpmaxsd */, X86::VPMAXSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10008 /* vpmaxsd */, X86::VPMAXSDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10008 /* vpmaxsd */, X86::VPMAXSDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10008 /* vpmaxsd */, X86::VPMAXSDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10008 /* vpmaxsd */, X86::VPMAXSDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10008 /* vpmaxsd */, X86::VPMAXSDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10008 /* vpmaxsd */, X86::VPMAXSDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10016 /* vpmaxsw */, X86::VPMAXSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10016 /* vpmaxsw */, X86::VPMAXSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10016 /* vpmaxsw */, X86::VPMAXSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10016 /* vpmaxsw */, X86::VPMAXSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10016 /* vpmaxsw */, X86::VPMAXSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10016 /* vpmaxsw */, X86::VPMAXSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10016 /* vpmaxsw */, X86::VPMAXSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10016 /* vpmaxsw */, X86::VPMAXSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10024 /* vpmaxub */, X86::VPMAXUBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10024 /* vpmaxub */, X86::VPMAXUBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10024 /* vpmaxub */, X86::VPMAXUBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10024 /* vpmaxub */, X86::VPMAXUBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10024 /* vpmaxub */, X86::VPMAXUBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10024 /* vpmaxub */, X86::VPMAXUBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10024 /* vpmaxub */, X86::VPMAXUBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10024 /* vpmaxub */, X86::VPMAXUBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10032 /* vpmaxud */, X86::VPMAXUDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10032 /* vpmaxud */, X86::VPMAXUDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10032 /* vpmaxud */, X86::VPMAXUDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10032 /* vpmaxud */, X86::VPMAXUDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10032 /* vpmaxud */, X86::VPMAXUDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10032 /* vpmaxud */, X86::VPMAXUDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10032 /* vpmaxud */, X86::VPMAXUDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10032 /* vpmaxud */, X86::VPMAXUDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10040 /* vpmaxuw */, X86::VPMAXUWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10040 /* vpmaxuw */, X86::VPMAXUWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10040 /* vpmaxuw */, X86::VPMAXUWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10040 /* vpmaxuw */, X86::VPMAXUWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10040 /* vpmaxuw */, X86::VPMAXUWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10040 /* vpmaxuw */, X86::VPMAXUWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10040 /* vpmaxuw */, X86::VPMAXUWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10040 /* vpmaxuw */, X86::VPMAXUWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10048 /* vpminsb */, X86::VPMINSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10048 /* vpminsb */, X86::VPMINSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10048 /* vpminsb */, X86::VPMINSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10048 /* vpminsb */, X86::VPMINSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10048 /* vpminsb */, X86::VPMINSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10048 /* vpminsb */, X86::VPMINSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10048 /* vpminsb */, X86::VPMINSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10048 /* vpminsb */, X86::VPMINSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10056 /* vpminsd */, X86::VPMINSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10056 /* vpminsd */, X86::VPMINSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10056 /* vpminsd */, X86::VPMINSDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10056 /* vpminsd */, X86::VPMINSDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10056 /* vpminsd */, X86::VPMINSDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10056 /* vpminsd */, X86::VPMINSDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10056 /* vpminsd */, X86::VPMINSDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10056 /* vpminsd */, X86::VPMINSDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10064 /* vpminsw */, X86::VPMINSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10064 /* vpminsw */, X86::VPMINSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10064 /* vpminsw */, X86::VPMINSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10064 /* vpminsw */, X86::VPMINSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10064 /* vpminsw */, X86::VPMINSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10064 /* vpminsw */, X86::VPMINSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10064 /* vpminsw */, X86::VPMINSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10064 /* vpminsw */, X86::VPMINSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10072 /* vpminub */, X86::VPMINUBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10072 /* vpminub */, X86::VPMINUBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10072 /* vpminub */, X86::VPMINUBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10072 /* vpminub */, X86::VPMINUBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10072 /* vpminub */, X86::VPMINUBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10072 /* vpminub */, X86::VPMINUBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10072 /* vpminub */, X86::VPMINUBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10072 /* vpminub */, X86::VPMINUBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10080 /* vpminud */, X86::VPMINUDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10080 /* vpminud */, X86::VPMINUDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10080 /* vpminud */, X86::VPMINUDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10080 /* vpminud */, X86::VPMINUDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10080 /* vpminud */, X86::VPMINUDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10080 /* vpminud */, X86::VPMINUDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10080 /* vpminud */, X86::VPMINUDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10080 /* vpminud */, X86::VPMINUDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10088 /* vpminuw */, X86::VPMINUWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10088 /* vpminuw */, X86::VPMINUWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10088 /* vpminuw */, X86::VPMINUWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10088 /* vpminuw */, X86::VPMINUWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10088 /* vpminuw */, X86::VPMINUWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10088 /* vpminuw */, X86::VPMINUWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10088 /* vpminuw */, X86::VPMINUWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10088 /* vpminuw */, X86::VPMINUWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_FR32 }, 1},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_VR256 }, 1},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_FR32 }, 1},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBYr64r, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_VR256 }, 1},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR32 }, 0},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_GR64 }, 0},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR32 }, 0},
  { 10096 /* vpmovmskb */, X86::VPMOVMSKBYr64r, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_GR64 }, 0},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 10106 /* vpmovsxbd */, X86::VPMOVSXBDYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQrm, Convert__Reg1_0__Mem165_1, 0, { MCK_FR32, MCK_Mem16 }, 1},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQYrm, Convert__Reg1_0__Mem165_1, 0, { MCK_VR256, MCK_Mem16 }, 1},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_FR32 }, 0},
  { 10116 /* vpmovsxbq */, X86::VPMOVSXBQYrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_VR256 }, 0},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10126 /* vpmovsxbw */, X86::VPMOVSXBWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10136 /* vpmovsxdq */, X86::VPMOVSXDQrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10146 /* vpmovsxwd */, X86::VPMOVSXWDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 10156 /* vpmovsxwq */, X86::VPMOVSXWQYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 10166 /* vpmovzxbd */, X86::VPMOVZXBDYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQrm, Convert__Reg1_0__Mem165_1, 0, { MCK_FR32, MCK_Mem16 }, 1},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQYrm, Convert__Reg1_0__Mem165_1, 0, { MCK_VR256, MCK_Mem16 }, 1},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_FR32 }, 0},
  { 10176 /* vpmovzxbq */, X86::VPMOVZXBQYrm, Convert__Reg1_1__Mem165_0, 0, { MCK_Mem16, MCK_VR256 }, 0},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10186 /* vpmovzxbw */, X86::VPMOVZXBWrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10196 /* vpmovzxdq */, X86::VPMOVZXDQrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDYrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_VR256, MCK_Mem128 }, 1},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDYrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256 }, 0},
  { 10206 /* vpmovzxwd */, X86::VPMOVZXWDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256 }, 0},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_FR32 }, 1},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQYrm, Convert__Reg1_0__Mem325_1, 0, { MCK_VR256, MCK_Mem32 }, 1},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 10216 /* vpmovzxwq */, X86::VPMOVZXWQYrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_VR256 }, 0},
  { 10226 /* vpmuldq */, X86::VPMULDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10226 /* vpmuldq */, X86::VPMULDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10226 /* vpmuldq */, X86::VPMULDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10226 /* vpmuldq */, X86::VPMULDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10226 /* vpmuldq */, X86::VPMULDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10226 /* vpmuldq */, X86::VPMULDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10226 /* vpmuldq */, X86::VPMULDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10226 /* vpmuldq */, X86::VPMULDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrr128, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrr128, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrm128, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrr256, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrr256, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrm256, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrm128, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10234 /* vpmulhrsw */, X86::VPMULHRSWrm256, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10244 /* vpmulhuw */, X86::VPMULHUWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10244 /* vpmulhuw */, X86::VPMULHUWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10244 /* vpmulhuw */, X86::VPMULHUWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10244 /* vpmulhuw */, X86::VPMULHUWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10244 /* vpmulhuw */, X86::VPMULHUWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10244 /* vpmulhuw */, X86::VPMULHUWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10244 /* vpmulhuw */, X86::VPMULHUWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10244 /* vpmulhuw */, X86::VPMULHUWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10253 /* vpmulhw */, X86::VPMULHWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10253 /* vpmulhw */, X86::VPMULHWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10253 /* vpmulhw */, X86::VPMULHWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10253 /* vpmulhw */, X86::VPMULHWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10253 /* vpmulhw */, X86::VPMULHWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10253 /* vpmulhw */, X86::VPMULHWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10253 /* vpmulhw */, X86::VPMULHWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10253 /* vpmulhw */, X86::VPMULHWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10261 /* vpmulld */, X86::VPMULLDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10261 /* vpmulld */, X86::VPMULLDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10261 /* vpmulld */, X86::VPMULLDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10261 /* vpmulld */, X86::VPMULLDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10261 /* vpmulld */, X86::VPMULLDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10261 /* vpmulld */, X86::VPMULLDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10261 /* vpmulld */, X86::VPMULLDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10261 /* vpmulld */, X86::VPMULLDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10269 /* vpmullw */, X86::VPMULLWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10269 /* vpmullw */, X86::VPMULLWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10269 /* vpmullw */, X86::VPMULLWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10269 /* vpmullw */, X86::VPMULLWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10269 /* vpmullw */, X86::VPMULLWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10269 /* vpmullw */, X86::VPMULLWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10269 /* vpmullw */, X86::VPMULLWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10269 /* vpmullw */, X86::VPMULLWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10277 /* vpmuludq */, X86::VPMULUDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10277 /* vpmuludq */, X86::VPMULUDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10277 /* vpmuludq */, X86::VPMULUDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10277 /* vpmuludq */, X86::VPMULUDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10277 /* vpmuludq */, X86::VPMULUDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10277 /* vpmuludq */, X86::VPMULUDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10277 /* vpmuludq */, X86::VPMULUDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10277 /* vpmuludq */, X86::VPMULUDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10286 /* vpor */, X86::VPORrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10286 /* vpor */, X86::VPORrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10286 /* vpor */, X86::VPORrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10286 /* vpor */, X86::VPORYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10286 /* vpor */, X86::VPORYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10286 /* vpor */, X86::VPORYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10286 /* vpor */, X86::VPORrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10286 /* vpor */, X86::VPORYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10291 /* vpperm */, X86::VPPERMrr, Convert__Reg1_3__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10291 /* vpperm */, X86::VPPERMrr, Convert__Reg1_0__Reg1_1__Reg1_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10291 /* vpperm */, X86::VPPERMrm, Convert__Reg1_0__Reg1_1__Reg1_2__Mem1285_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10291 /* vpperm */, X86::VPPERMmr, Convert__Reg1_0__Reg1_1__Mem1285_2__Reg1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10291 /* vpperm */, X86::VPPERMmr, Convert__Reg1_3__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10291 /* vpperm */, X86::VPPERMrm, Convert__Reg1_3__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10298 /* vprotb */, X86::VPROTBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10298 /* vprotb */, X86::VPROTBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10298 /* vprotb */, X86::VPROTBri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10298 /* vprotb */, X86::VPROTBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10298 /* vprotb */, X86::VPROTBmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10298 /* vprotb */, X86::VPROTBmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10298 /* vprotb */, X86::VPROTBmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10298 /* vprotb */, X86::VPROTBri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10298 /* vprotb */, X86::VPROTBmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10298 /* vprotb */, X86::VPROTBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10305 /* vprotd */, X86::VPROTDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10305 /* vprotd */, X86::VPROTDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10305 /* vprotd */, X86::VPROTDri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10305 /* vprotd */, X86::VPROTDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10305 /* vprotd */, X86::VPROTDmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10305 /* vprotd */, X86::VPROTDmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10305 /* vprotd */, X86::VPROTDmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10305 /* vprotd */, X86::VPROTDri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10305 /* vprotd */, X86::VPROTDmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10305 /* vprotd */, X86::VPROTDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10312 /* vprotq */, X86::VPROTQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10312 /* vprotq */, X86::VPROTQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10312 /* vprotq */, X86::VPROTQri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10312 /* vprotq */, X86::VPROTQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10312 /* vprotq */, X86::VPROTQmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10312 /* vprotq */, X86::VPROTQmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10312 /* vprotq */, X86::VPROTQmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10312 /* vprotq */, X86::VPROTQri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10312 /* vprotq */, X86::VPROTQmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10312 /* vprotq */, X86::VPROTQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10319 /* vprotw */, X86::VPROTWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10319 /* vprotw */, X86::VPROTWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10319 /* vprotw */, X86::VPROTWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10319 /* vprotw */, X86::VPROTWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10319 /* vprotw */, X86::VPROTWmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10319 /* vprotw */, X86::VPROTWmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10319 /* vprotw */, X86::VPROTWmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10319 /* vprotw */, X86::VPROTWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10319 /* vprotw */, X86::VPROTWmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10319 /* vprotw */, X86::VPROTWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10326 /* vpsadbw */, X86::VPSADBWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10326 /* vpsadbw */, X86::VPSADBWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10326 /* vpsadbw */, X86::VPSADBWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10326 /* vpsadbw */, X86::VPSADBWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10326 /* vpsadbw */, X86::VPSADBWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10326 /* vpsadbw */, X86::VPSADBWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10326 /* vpsadbw */, X86::VPSADBWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10326 /* vpsadbw */, X86::VPSADBWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10334 /* vpshab */, X86::VPSHABrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10334 /* vpshab */, X86::VPSHABrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10334 /* vpshab */, X86::VPSHABrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10334 /* vpshab */, X86::VPSHABmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10334 /* vpshab */, X86::VPSHABmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10334 /* vpshab */, X86::VPSHABrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10341 /* vpshad */, X86::VPSHADrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10341 /* vpshad */, X86::VPSHADrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10341 /* vpshad */, X86::VPSHADrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10341 /* vpshad */, X86::VPSHADmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10341 /* vpshad */, X86::VPSHADmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10341 /* vpshad */, X86::VPSHADrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10348 /* vpshaq */, X86::VPSHAQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10348 /* vpshaq */, X86::VPSHAQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10348 /* vpshaq */, X86::VPSHAQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10348 /* vpshaq */, X86::VPSHAQmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10348 /* vpshaq */, X86::VPSHAQmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10348 /* vpshaq */, X86::VPSHAQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10355 /* vpshaw */, X86::VPSHAWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10355 /* vpshaw */, X86::VPSHAWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10355 /* vpshaw */, X86::VPSHAWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10355 /* vpshaw */, X86::VPSHAWmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10355 /* vpshaw */, X86::VPSHAWmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10355 /* vpshaw */, X86::VPSHAWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10362 /* vpshlb */, X86::VPSHLBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10362 /* vpshlb */, X86::VPSHLBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10362 /* vpshlb */, X86::VPSHLBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10362 /* vpshlb */, X86::VPSHLBmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10362 /* vpshlb */, X86::VPSHLBmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10362 /* vpshlb */, X86::VPSHLBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10369 /* vpshld */, X86::VPSHLDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10369 /* vpshld */, X86::VPSHLDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10369 /* vpshld */, X86::VPSHLDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10369 /* vpshld */, X86::VPSHLDmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10369 /* vpshld */, X86::VPSHLDmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10369 /* vpshld */, X86::VPSHLDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10376 /* vpshlq */, X86::VPSHLQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10376 /* vpshlq */, X86::VPSHLQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10376 /* vpshlq */, X86::VPSHLQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10376 /* vpshlq */, X86::VPSHLQmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10376 /* vpshlq */, X86::VPSHLQmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10376 /* vpshlq */, X86::VPSHLQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10383 /* vpshlw */, X86::VPSHLWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10383 /* vpshlw */, X86::VPSHLWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10383 /* vpshlw */, X86::VPSHLWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10383 /* vpshlw */, X86::VPSHLWmr, Convert__Reg1_2__Mem1285_1__Reg1_0, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 0},
  { 10383 /* vpshlw */, X86::VPSHLWmr, Convert__Reg1_0__Mem1285_1__Reg1_2, 0, { MCK_FR32, MCK_Mem128, MCK_FR32 }, 1},
  { 10383 /* vpshlw */, X86::VPSHLWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10390 /* vpshufb */, X86::VPSHUFBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10390 /* vpshufb */, X86::VPSHUFBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10390 /* vpshufb */, X86::VPSHUFBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10390 /* vpshufb */, X86::VPSHUFBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10390 /* vpshufb */, X86::VPSHUFBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10390 /* vpshufb */, X86::VPSHUFBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10390 /* vpshufb */, X86::VPSHUFBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10390 /* vpshufb */, X86::VPSHUFBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10398 /* vpshufd */, X86::VPSHUFDri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10398 /* vpshufd */, X86::VPSHUFDmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10398 /* vpshufd */, X86::VPSHUFDYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 10398 /* vpshufd */, X86::VPSHUFDYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 10398 /* vpshufd */, X86::VPSHUFDri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10398 /* vpshufd */, X86::VPSHUFDYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 10398 /* vpshufd */, X86::VPSHUFDmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10398 /* vpshufd */, X86::VPSHUFDYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 10406 /* vpshufhw */, X86::VPSHUFHWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10406 /* vpshufhw */, X86::VPSHUFHWmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10406 /* vpshufhw */, X86::VPSHUFHWYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 10406 /* vpshufhw */, X86::VPSHUFHWYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 10406 /* vpshufhw */, X86::VPSHUFHWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10406 /* vpshufhw */, X86::VPSHUFHWYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 10406 /* vpshufhw */, X86::VPSHUFHWmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10406 /* vpshufhw */, X86::VPSHUFHWYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 10415 /* vpshuflw */, X86::VPSHUFLWri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10415 /* vpshuflw */, X86::VPSHUFLWmi, Convert__Reg1_0__Mem1285_1__Imm1_2, 0, { MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10415 /* vpshuflw */, X86::VPSHUFLWYri, Convert__Reg1_0__Reg1_1__Imm1_2, 0, { MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 10415 /* vpshuflw */, X86::VPSHUFLWYmi, Convert__Reg1_0__Mem2565_1__Imm1_2, 0, { MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 10415 /* vpshuflw */, X86::VPSHUFLWri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32 }, 0},
  { 10415 /* vpshuflw */, X86::VPSHUFLWYri, Convert__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256 }, 0},
  { 10415 /* vpshuflw */, X86::VPSHUFLWmi, Convert__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32 }, 0},
  { 10415 /* vpshuflw */, X86::VPSHUFLWYmi, Convert__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256 }, 0},
  { 10424 /* vpsignb */, X86::VPSIGNBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10424 /* vpsignb */, X86::VPSIGNBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10424 /* vpsignb */, X86::VPSIGNBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10424 /* vpsignb */, X86::VPSIGNBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10424 /* vpsignb */, X86::VPSIGNBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10424 /* vpsignb */, X86::VPSIGNBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10424 /* vpsignb */, X86::VPSIGNBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10424 /* vpsignb */, X86::VPSIGNBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10432 /* vpsignd */, X86::VPSIGNDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10432 /* vpsignd */, X86::VPSIGNDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10432 /* vpsignd */, X86::VPSIGNDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10432 /* vpsignd */, X86::VPSIGNDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10432 /* vpsignd */, X86::VPSIGNDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10432 /* vpsignd */, X86::VPSIGNDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10432 /* vpsignd */, X86::VPSIGNDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10432 /* vpsignd */, X86::VPSIGNDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10440 /* vpsignw */, X86::VPSIGNWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10440 /* vpsignw */, X86::VPSIGNWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10440 /* vpsignw */, X86::VPSIGNWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10440 /* vpsignw */, X86::VPSIGNWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10440 /* vpsignw */, X86::VPSIGNWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10440 /* vpsignw */, X86::VPSIGNWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10440 /* vpsignw */, X86::VPSIGNWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10440 /* vpsignw */, X86::VPSIGNWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10448 /* vpslld */, X86::VPSLLDri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10448 /* vpslld */, X86::VPSLLDYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10455 /* vpslldq */, X86::VPSLLDQri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10455 /* vpslldq */, X86::VPSLLDQYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10455 /* vpslldq */, X86::VPSLLDQri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10455 /* vpslldq */, X86::VPSLLDQYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10463 /* vpsllq */, X86::VPSLLQri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10463 /* vpsllq */, X86::VPSLLQYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10470 /* vpsllvd */, X86::VPSLLVDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10470 /* vpsllvd */, X86::VPSLLVDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10470 /* vpsllvd */, X86::VPSLLVDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10470 /* vpsllvd */, X86::VPSLLVDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10470 /* vpsllvd */, X86::VPSLLVDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10470 /* vpsllvd */, X86::VPSLLVDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10470 /* vpsllvd */, X86::VPSLLVDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10470 /* vpsllvd */, X86::VPSLLVDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10478 /* vpsllvq */, X86::VPSLLVQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10478 /* vpsllvq */, X86::VPSLLVQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10478 /* vpsllvq */, X86::VPSLLVQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10478 /* vpsllvq */, X86::VPSLLVQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10478 /* vpsllvq */, X86::VPSLLVQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10478 /* vpsllvq */, X86::VPSLLVQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10478 /* vpsllvq */, X86::VPSLLVQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10478 /* vpsllvq */, X86::VPSLLVQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10486 /* vpsllw */, X86::VPSLLWri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10486 /* vpsllw */, X86::VPSLLWYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10493 /* vpsrad */, X86::VPSRADri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10493 /* vpsrad */, X86::VPSRADYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10500 /* vpsravd */, X86::VPSRAVDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10500 /* vpsravd */, X86::VPSRAVDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10500 /* vpsravd */, X86::VPSRAVDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10500 /* vpsravd */, X86::VPSRAVDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10500 /* vpsravd */, X86::VPSRAVDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10500 /* vpsravd */, X86::VPSRAVDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10500 /* vpsravd */, X86::VPSRAVDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10500 /* vpsravd */, X86::VPSRAVDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10508 /* vpsraw */, X86::VPSRAWri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10508 /* vpsraw */, X86::VPSRAWYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10515 /* vpsrld */, X86::VPSRLDri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10515 /* vpsrld */, X86::VPSRLDYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10522 /* vpsrldq */, X86::VPSRLDQri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10522 /* vpsrldq */, X86::VPSRLDQYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10522 /* vpsrldq */, X86::VPSRLDQri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10522 /* vpsrldq */, X86::VPSRLDQYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10530 /* vpsrlq */, X86::VPSRLQri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10530 /* vpsrlq */, X86::VPSRLQYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10537 /* vpsrlvd */, X86::VPSRLVDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10537 /* vpsrlvd */, X86::VPSRLVDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10537 /* vpsrlvd */, X86::VPSRLVDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10537 /* vpsrlvd */, X86::VPSRLVDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10537 /* vpsrlvd */, X86::VPSRLVDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10537 /* vpsrlvd */, X86::VPSRLVDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10537 /* vpsrlvd */, X86::VPSRLVDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10537 /* vpsrlvd */, X86::VPSRLVDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10545 /* vpsrlvq */, X86::VPSRLVQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10545 /* vpsrlvq */, X86::VPSRLVQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10545 /* vpsrlvq */, X86::VPSRLVQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10545 /* vpsrlvq */, X86::VPSRLVQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10545 /* vpsrlvq */, X86::VPSRLVQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10545 /* vpsrlvq */, X86::VPSRLVQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10545 /* vpsrlvq */, X86::VPSRLVQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10545 /* vpsrlvq */, X86::VPSRLVQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_VR256, MCK_VR256 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_FR32 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWYri, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWYrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem128 }, 1},
  { 10553 /* vpsrlw */, X86::VPSRLWri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWYri, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10553 /* vpsrlw */, X86::VPSRLWYrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_VR256, MCK_VR256 }, 0},
  { 10560 /* vpsubb */, X86::VPSUBBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10560 /* vpsubb */, X86::VPSUBBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10560 /* vpsubb */, X86::VPSUBBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10560 /* vpsubb */, X86::VPSUBBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10560 /* vpsubb */, X86::VPSUBBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10560 /* vpsubb */, X86::VPSUBBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10560 /* vpsubb */, X86::VPSUBBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10560 /* vpsubb */, X86::VPSUBBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10567 /* vpsubd */, X86::VPSUBDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10567 /* vpsubd */, X86::VPSUBDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10567 /* vpsubd */, X86::VPSUBDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10567 /* vpsubd */, X86::VPSUBDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10567 /* vpsubd */, X86::VPSUBDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10567 /* vpsubd */, X86::VPSUBDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10567 /* vpsubd */, X86::VPSUBDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10567 /* vpsubd */, X86::VPSUBDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10574 /* vpsubq */, X86::VPSUBQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10574 /* vpsubq */, X86::VPSUBQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10574 /* vpsubq */, X86::VPSUBQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10574 /* vpsubq */, X86::VPSUBQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10574 /* vpsubq */, X86::VPSUBQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10574 /* vpsubq */, X86::VPSUBQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10574 /* vpsubq */, X86::VPSUBQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10574 /* vpsubq */, X86::VPSUBQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10581 /* vpsubsb */, X86::VPSUBSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10581 /* vpsubsb */, X86::VPSUBSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10581 /* vpsubsb */, X86::VPSUBSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10581 /* vpsubsb */, X86::VPSUBSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10581 /* vpsubsb */, X86::VPSUBSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10581 /* vpsubsb */, X86::VPSUBSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10581 /* vpsubsb */, X86::VPSUBSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10581 /* vpsubsb */, X86::VPSUBSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10589 /* vpsubsw */, X86::VPSUBSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10589 /* vpsubsw */, X86::VPSUBSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10589 /* vpsubsw */, X86::VPSUBSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10589 /* vpsubsw */, X86::VPSUBSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10589 /* vpsubsw */, X86::VPSUBSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10589 /* vpsubsw */, X86::VPSUBSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10589 /* vpsubsw */, X86::VPSUBSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10589 /* vpsubsw */, X86::VPSUBSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10597 /* vpsubusb */, X86::VPSUBUSBrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10597 /* vpsubusb */, X86::VPSUBUSBrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10597 /* vpsubusb */, X86::VPSUBUSBrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10597 /* vpsubusb */, X86::VPSUBUSBYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10597 /* vpsubusb */, X86::VPSUBUSBYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10597 /* vpsubusb */, X86::VPSUBUSBYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10597 /* vpsubusb */, X86::VPSUBUSBrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10597 /* vpsubusb */, X86::VPSUBUSBYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10606 /* vpsubusw */, X86::VPSUBUSWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10606 /* vpsubusw */, X86::VPSUBUSWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10606 /* vpsubusw */, X86::VPSUBUSWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10606 /* vpsubusw */, X86::VPSUBUSWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10606 /* vpsubusw */, X86::VPSUBUSWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10606 /* vpsubusw */, X86::VPSUBUSWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10606 /* vpsubusw */, X86::VPSUBUSWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10606 /* vpsubusw */, X86::VPSUBUSWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10615 /* vpsubw */, X86::VPSUBWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10615 /* vpsubw */, X86::VPSUBWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10615 /* vpsubw */, X86::VPSUBWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10615 /* vpsubw */, X86::VPSUBWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10615 /* vpsubw */, X86::VPSUBWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10615 /* vpsubw */, X86::VPSUBWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10615 /* vpsubw */, X86::VPSUBWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10615 /* vpsubw */, X86::VPSUBWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10622 /* vptest */, X86::VPTESTrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10622 /* vptest */, X86::VPTESTrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10622 /* vptest */, X86::VPTESTrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10622 /* vptest */, X86::VPTESTYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10622 /* vptest */, X86::VPTESTYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10622 /* vptest */, X86::VPTESTYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10622 /* vptest */, X86::VPTESTrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10622 /* vptest */, X86::VPTESTYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10629 /* vpunpckhbw */, X86::VPUNPCKHBWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10640 /* vpunpckhdq */, X86::VPUNPCKHDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10651 /* vpunpckhqdq */, X86::VPUNPCKHQDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10663 /* vpunpckhwd */, X86::VPUNPCKHWDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10674 /* vpunpcklbw */, X86::VPUNPCKLBWYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10685 /* vpunpckldq */, X86::VPUNPCKLDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10696 /* vpunpcklqdq */, X86::VPUNPCKLQDQYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10708 /* vpunpcklwd */, X86::VPUNPCKLWDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10719 /* vpxor */, X86::VPXORrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10719 /* vpxor */, X86::VPXORrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10719 /* vpxor */, X86::VPXORrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10719 /* vpxor */, X86::VPXORYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10719 /* vpxor */, X86::VPXORYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10719 /* vpxor */, X86::VPXORYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10719 /* vpxor */, X86::VPXORrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10719 /* vpxor */, X86::VPXORYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10725 /* vrcpps */, X86::VRCPPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10725 /* vrcpps */, X86::VRCPPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10725 /* vrcpps */, X86::VRCPPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10725 /* vrcpps */, X86::VRCPPSYr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10725 /* vrcpps */, X86::VRCPPSYr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10725 /* vrcpps */, X86::VRCPPSYm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10725 /* vrcpps */, X86::VRCPPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10725 /* vrcpps */, X86::VRCPPSYm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10732 /* vrcpss */, X86::VRCPSSr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10732 /* vrcpss */, X86::VRCPSSr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10732 /* vrcpss */, X86::VRCPSSm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 10732 /* vrcpss */, X86::VRCPSSm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 10739 /* vroundpd */, X86::VROUNDPDr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10739 /* vroundpd */, X86::VROUNDPDm, Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmSExti32i8 }, 1},
  { 10739 /* vroundpd */, X86::VROUNDYPDr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10739 /* vroundpd */, X86::VROUNDYPDm, Convert__Reg1_0__Mem2565_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_Mem256, MCK_ImmSExti32i8 }, 1},
  { 10739 /* vroundpd */, X86::VROUNDPDr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10739 /* vroundpd */, X86::VROUNDYPDr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10739 /* vroundpd */, X86::VROUNDPDm, Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem128, MCK_FR32 }, 0},
  { 10739 /* vroundpd */, X86::VROUNDYPDm, Convert__Reg1_2__Mem2565_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem256, MCK_VR256 }, 0},
  { 10748 /* vroundps */, X86::VROUNDPSr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10748 /* vroundps */, X86::VROUNDPSm, Convert__Reg1_0__Mem1285_1__ImmSExti32i81_2, 0, { MCK_FR32, MCK_Mem128, MCK_ImmSExti32i8 }, 1},
  { 10748 /* vroundps */, X86::VROUNDYPSr, Convert__Reg1_0__Reg1_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_VR256, MCK_ImmSExti32i8 }, 1},
  { 10748 /* vroundps */, X86::VROUNDYPSm, Convert__Reg1_0__Mem2565_1__ImmSExti32i81_2, 0, { MCK_VR256, MCK_Mem256, MCK_ImmSExti32i8 }, 1},
  { 10748 /* vroundps */, X86::VROUNDPSr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32 }, 0},
  { 10748 /* vroundps */, X86::VROUNDYPSr, Convert__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_VR256, MCK_VR256 }, 0},
  { 10748 /* vroundps */, X86::VROUNDPSm, Convert__Reg1_2__Mem1285_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem128, MCK_FR32 }, 0},
  { 10748 /* vroundps */, X86::VROUNDYPSm, Convert__Reg1_2__Mem2565_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem256, MCK_VR256 }, 0},
  { 10757 /* vroundsd */, X86::VROUNDSDr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10757 /* vroundsd */, X86::VROUNDSDm, Convert__Reg1_0__Reg1_1__Mem5_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem, MCK_ImmSExti32i8 }, 1},
  { 10757 /* vroundsd */, X86::VROUNDSDr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10757 /* vroundsd */, X86::VROUNDSDm, Convert__Reg1_3__Reg1_2__Mem5_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem, MCK_FR32, MCK_FR32 }, 0},
  { 10766 /* vroundss */, X86::VROUNDSSr, Convert__Reg1_0__Reg1_1__Reg1_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_ImmSExti32i8 }, 1},
  { 10766 /* vroundss */, X86::VROUNDSSm, Convert__Reg1_0__Reg1_1__Mem5_2__ImmSExti32i81_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem, MCK_ImmSExti32i8 }, 1},
  { 10766 /* vroundss */, X86::VROUNDSSr, Convert__Reg1_3__Reg1_2__Reg1_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10766 /* vroundss */, X86::VROUNDSSm, Convert__Reg1_3__Reg1_2__Mem5_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem, MCK_FR32, MCK_FR32 }, 0},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSYr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSYr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSYm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10775 /* vrsqrtps */, X86::VRSQRTPSYm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10784 /* vrsqrtss */, X86::VRSQRTSSr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10784 /* vrsqrtss */, X86::VRSQRTSSr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10784 /* vrsqrtss */, X86::VRSQRTSSm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 10784 /* vrsqrtss */, X86::VRSQRTSSm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 10793 /* vshufpd */, X86::VSHUFPDrri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10793 /* vshufpd */, X86::VSHUFPDYrri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10793 /* vshufpd */, X86::VSHUFPDrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10793 /* vshufpd */, X86::VSHUFPDYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10801 /* vshufps */, X86::VSHUFPSrri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_FR32, MCK_Imm }, 1},
  { 10801 /* vshufps */, X86::VSHUFPSrmi, Convert__Reg1_0__Reg1_1__Mem1285_2__Imm1_3, 0, { MCK_FR32, MCK_FR32, MCK_Mem128, MCK_Imm }, 1},
  { 10801 /* vshufps */, X86::VSHUFPSYrri, Convert__Reg1_0__Reg1_1__Reg1_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_VR256, MCK_Imm }, 1},
  { 10801 /* vshufps */, X86::VSHUFPSYrmi, Convert__Reg1_0__Reg1_1__Mem2565_2__Imm1_3, 0, { MCK_VR256, MCK_VR256, MCK_Mem256, MCK_Imm }, 1},
  { 10801 /* vshufps */, X86::VSHUFPSrri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10801 /* vshufps */, X86::VSHUFPSYrri, Convert__Reg1_3__Reg1_2__Reg1_1__Imm1_0, 0, { MCK_Imm, MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10801 /* vshufps */, X86::VSHUFPSrmi, Convert__Reg1_3__Reg1_2__Mem1285_1__Imm1_0, 0, { MCK_Imm, MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10801 /* vshufps */, X86::VSHUFPSYrmi, Convert__Reg1_3__Reg1_2__Mem2565_1__Imm1_0, 0, { MCK_Imm, MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10809 /* vsqrtpd */, X86::VSQRTPDr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10809 /* vsqrtpd */, X86::VSQRTPDr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10809 /* vsqrtpd */, X86::VSQRTPDm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10809 /* vsqrtpd */, X86::VSQRTPDYr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10809 /* vsqrtpd */, X86::VSQRTPDYr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10809 /* vsqrtpd */, X86::VSQRTPDYm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10809 /* vsqrtpd */, X86::VSQRTPDm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10809 /* vsqrtpd */, X86::VSQRTPDYm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10817 /* vsqrtps */, X86::VSQRTPSr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10817 /* vsqrtps */, X86::VSQRTPSr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10817 /* vsqrtps */, X86::VSQRTPSm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10817 /* vsqrtps */, X86::VSQRTPSYr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10817 /* vsqrtps */, X86::VSQRTPSYr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10817 /* vsqrtps */, X86::VSQRTPSYm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10817 /* vsqrtps */, X86::VSQRTPSm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10817 /* vsqrtps */, X86::VSQRTPSYm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10825 /* vsqrtsd */, X86::VSQRTSDr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10825 /* vsqrtsd */, X86::VSQRTSDr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10825 /* vsqrtsd */, X86::VSQRTSDm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 10825 /* vsqrtsd */, X86::VSQRTSDm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 10833 /* vsqrtss */, X86::VSQRTSSr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10833 /* vsqrtss */, X86::VSQRTSSr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10833 /* vsqrtss */, X86::VSQRTSSm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 10833 /* vsqrtss */, X86::VSQRTSSm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 10841 /* vstmxcsr */, X86::VSTMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 0},
  { 10841 /* vstmxcsr */, X86::VSTMXCSR, Convert__Mem325_0, 0, { MCK_Mem32 }, 1},
  { 10850 /* vsubpd */, X86::VSUBPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10850 /* vsubpd */, X86::VSUBPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10850 /* vsubpd */, X86::VSUBPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10850 /* vsubpd */, X86::VSUBPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10850 /* vsubpd */, X86::VSUBPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10850 /* vsubpd */, X86::VSUBPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10850 /* vsubpd */, X86::VSUBPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10850 /* vsubpd */, X86::VSUBPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10857 /* vsubps */, X86::VSUBPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10857 /* vsubps */, X86::VSUBPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10857 /* vsubps */, X86::VSUBPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10857 /* vsubps */, X86::VSUBPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10857 /* vsubps */, X86::VSUBPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10857 /* vsubps */, X86::VSUBPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10857 /* vsubps */, X86::VSUBPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10857 /* vsubps */, X86::VSUBPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10864 /* vsubsd */, X86::VSUBSDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10864 /* vsubsd */, X86::VSUBSDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10864 /* vsubsd */, X86::VSUBSDrm, Convert__Reg1_0__Reg1_1__Mem645_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem64 }, 1},
  { 10864 /* vsubsd */, X86::VSUBSDrm, Convert__Reg1_2__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32, MCK_FR32 }, 0},
  { 10871 /* vsubss */, X86::VSUBSSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10871 /* vsubss */, X86::VSUBSSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10871 /* vsubss */, X86::VSUBSSrm, Convert__Reg1_0__Reg1_1__Mem325_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem32 }, 1},
  { 10871 /* vsubss */, X86::VSUBSSrm, Convert__Reg1_2__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32, MCK_FR32 }, 0},
  { 10878 /* vtestpd */, X86::VTESTPDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10878 /* vtestpd */, X86::VTESTPDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10878 /* vtestpd */, X86::VTESTPDrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10878 /* vtestpd */, X86::VTESTPDYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10878 /* vtestpd */, X86::VTESTPDYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10878 /* vtestpd */, X86::VTESTPDYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10878 /* vtestpd */, X86::VTESTPDrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10878 /* vtestpd */, X86::VTESTPDYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10886 /* vtestps */, X86::VTESTPSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10886 /* vtestps */, X86::VTESTPSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10886 /* vtestps */, X86::VTESTPSrm, Convert__Reg1_0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 10886 /* vtestps */, X86::VTESTPSYrr, Convert__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256 }, 0},
  { 10886 /* vtestps */, X86::VTESTPSYrr, Convert__Reg1_0__Reg1_1, 0, { MCK_VR256, MCK_VR256 }, 1},
  { 10886 /* vtestps */, X86::VTESTPSYrm, Convert__Reg1_0__Mem2565_1, 0, { MCK_VR256, MCK_Mem256 }, 1},
  { 10886 /* vtestps */, X86::VTESTPSrm, Convert__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 10886 /* vtestps */, X86::VTESTPSYrm, Convert__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256 }, 0},
  { 10894 /* vucomisd */, X86::VUCOMISDrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10894 /* vucomisd */, X86::VUCOMISDrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10894 /* vucomisd */, X86::VUCOMISDrm, Convert__Reg1_0__Mem645_1, 0, { MCK_FR32, MCK_Mem64 }, 1},
  { 10894 /* vucomisd */, X86::VUCOMISDrm, Convert__Reg1_1__Mem645_0, 0, { MCK_Mem64, MCK_FR32 }, 0},
  { 10903 /* vucomiss */, X86::VUCOMISSrr, Convert__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 10903 /* vucomiss */, X86::VUCOMISSrr, Convert__Reg1_0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 10903 /* vucomiss */, X86::VUCOMISSrm, Convert__Reg1_0__Mem325_1, 0, { MCK_FR32, MCK_Mem32 }, 1},
  { 10903 /* vucomiss */, X86::VUCOMISSrm, Convert__Reg1_1__Mem325_0, 0, { MCK_Mem32, MCK_FR32 }, 0},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10912 /* vunpckhpd */, X86::VUNPCKHPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10922 /* vunpckhps */, X86::VUNPCKHPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10932 /* vunpcklpd */, X86::VUNPCKLPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10942 /* vunpcklps */, X86::VUNPCKLPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10952 /* vxorpd */, X86::VFsXORPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10952 /* vxorpd */, X86::VXORPDrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10952 /* vxorpd */, X86::VFsXORPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10952 /* vxorpd */, X86::VXORPDrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10952 /* vxorpd */, X86::VFsXORPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10952 /* vxorpd */, X86::VXORPDrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10952 /* vxorpd */, X86::VXORPDYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10952 /* vxorpd */, X86::VXORPDYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10952 /* vxorpd */, X86::VXORPDYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10952 /* vxorpd */, X86::VFsXORPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10952 /* vxorpd */, X86::VXORPDrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10952 /* vxorpd */, X86::VXORPDYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10959 /* vxorps */, X86::VFsXORPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10959 /* vxorps */, X86::VXORPSrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 0},
  { 10959 /* vxorps */, X86::VFsXORPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10959 /* vxorps */, X86::VXORPSrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_FR32, MCK_FR32, MCK_FR32 }, 1},
  { 10959 /* vxorps */, X86::VFsXORPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10959 /* vxorps */, X86::VXORPSrm, Convert__Reg1_0__Reg1_1__Mem1285_2, 0, { MCK_FR32, MCK_FR32, MCK_Mem128 }, 1},
  { 10959 /* vxorps */, X86::VXORPSYrr, Convert__Reg1_2__Reg1_1__Reg1_0, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 0},
  { 10959 /* vxorps */, X86::VXORPSYrr, Convert__Reg1_0__Reg1_1__Reg1_2, 0, { MCK_VR256, MCK_VR256, MCK_VR256 }, 1},
  { 10959 /* vxorps */, X86::VXORPSYrm, Convert__Reg1_0__Reg1_1__Mem2565_2, 0, { MCK_VR256, MCK_VR256, MCK_Mem256 }, 1},
  { 10959 /* vxorps */, X86::VFsXORPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10959 /* vxorps */, X86::VXORPSrm, Convert__Reg1_2__Reg1_1__Mem1285_0, 0, { MCK_Mem128, MCK_FR32, MCK_FR32 }, 0},
  { 10959 /* vxorps */, X86::VXORPSYrm, Convert__Reg1_2__Reg1_1__Mem2565_0, 0, { MCK_Mem256, MCK_VR256, MCK_VR256 }, 0},
  { 10966 /* vzeroall */, X86::VZEROALL, Convert_NoOperands, 0, {  }, 0},
  { 10966 /* vzeroall */, X86::VZEROALL, Convert_NoOperands, 0, {  }, 1},
  { 10975 /* vzeroupper */, X86::VZEROUPPER, Convert_NoOperands, 0, {  }, 0},
  { 10975 /* vzeroupper */, X86::VZEROUPPER, Convert_NoOperands, 0, {  }, 1},
  { 10986 /* wait */, X86::WAIT, Convert_NoOperands, 0, {  }, 0},
  { 10986 /* wait */, X86::WAIT, Convert_NoOperands, 0, {  }, 1},
  { 10991 /* wbinvd */, X86::WBINVD, Convert_NoOperands, 0, {  }, 0},
  { 10991 /* wbinvd */, X86::WBINVD, Convert_NoOperands, 0, {  }, 1},
  { 10998 /* wrfsbase */, X86::WRFSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 10998 /* wrfsbase */, X86::WRFSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 11007 /* wrfsbasel */, X86::WRFSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 11017 /* wrfsbaseq */, X86::WRFSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 0},
  { 11027 /* wrgsbase */, X86::WRGSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 1},
  { 11027 /* wrgsbase */, X86::WRGSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 1},
  { 11036 /* wrgsbasel */, X86::WRGSBASE, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32 }, 0},
  { 11046 /* wrgsbaseq */, X86::WRGSBASE64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR64 }, 0},
  { 11056 /* wrmsr */, X86::WRMSR, Convert_NoOperands, 0, {  }, 0},
  { 11056 /* wrmsr */, X86::WRMSR, Convert_NoOperands, 0, {  }, 1},
  { 11062 /* xabort */, X86::XABORT, Convert__Imm1_0, 0, { MCK_Imm }, 0},
  { 11062 /* xabort */, X86::XABORT, Convert__Imm1_0, 0, { MCK_Imm }, 1},
  { 11069 /* xadd */, X86::XADD8rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 11069 /* xadd */, X86::XADD16rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 11069 /* xadd */, X86::XADD32rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 11069 /* xadd */, X86::XADD64rr, Convert__Reg1_0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 11069 /* xadd */, X86::XADD16rm, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 11069 /* xadd */, X86::XADD32rm, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 11069 /* xadd */, X86::XADD64rm, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 11069 /* xadd */, X86::XADD8rm, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 11074 /* xaddb */, X86::XADD8rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 11074 /* xaddb */, X86::XADD8rm, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 11080 /* xaddl */, X86::XADD32rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 11080 /* xaddl */, X86::XADD32rm, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 11086 /* xaddq */, X86::XADD64rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 11086 /* xaddq */, X86::XADD64rm, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 11092 /* xaddw */, X86::XADD16rr, Convert__Reg1_1__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 11092 /* xaddw */, X86::XADD16rm, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 11098 /* xbegin */, X86::XBEGIN_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 0},
  { 11098 /* xbegin */, X86::XBEGIN_4, Convert__AbsMem1_0, 0, { MCK_AbsMem }, 1},
  { 11105 /* xchg */, X86::XCHG8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 11105 /* xchg */, X86::XCHG16ar, Convert__Reg1_1, 0, { MCK_AX, MCK_GR16 }, 1},
  { 11105 /* xchg */, X86::XCHG16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 11105 /* xchg */, X86::XCHG32ar64, Convert__Reg1_1, Feature_In64BitMode, { MCK_EAX, MCK_GR32_NOAX }, 1},
  { 11105 /* xchg */, X86::XCHG32ar, Convert__Reg1_1, Feature_In32BitMode, { MCK_EAX, MCK_GR32 }, 1},
  { 11105 /* xchg */, X86::XCHG32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 11105 /* xchg */, X86::XCHG64ar, Convert__Reg1_1, 0, { MCK_RAX, MCK_GR64 }, 1},
  { 11105 /* xchg */, X86::XCHG64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 11105 /* xchg */, X86::XCHG16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 11105 /* xchg */, X86::XCHG32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 11105 /* xchg */, X86::XCHG64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 11105 /* xchg */, X86::XCHG8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 11110 /* xchgb */, X86::XCHG8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 11110 /* xchgb */, X86::XCHG8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 11110 /* xchgb */, X86::XCHG8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 11110 /* xchgb */, X86::XCHG8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 11116 /* xchgl */, X86::XCHG32ar64, Convert__Reg1_1, Feature_In64BitMode, { MCK__PCT_eax, MCK_GR32_NOAX }, 1},
  { 11116 /* xchgl */, X86::XCHG32ar, Convert__Reg1_1, Feature_In32BitMode, { MCK__PCT_eax, MCK_GR32 }, 1},
  { 11116 /* xchgl */, X86::XCHG32ar64, Convert__Reg1_1, Feature_In64BitMode, { MCK_EAX, MCK_GR32_NOAX }, 0},
  { 11116 /* xchgl */, X86::XCHG32ar, Convert__Reg1_1, Feature_In32BitMode, { MCK_EAX, MCK_GR32 }, 0},
  { 11116 /* xchgl */, X86::XCHG32ar64, Convert__Reg1_0, Feature_In64BitMode, { MCK_GR32_NOAX, MCK_EAX }, 0},
  { 11116 /* xchgl */, X86::XCHG32ar, Convert__Reg1_0, Feature_In32BitMode, { MCK_GR32, MCK_EAX }, 0},
  { 11116 /* xchgl */, X86::XCHG32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 11116 /* xchgl */, X86::XCHG32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 11116 /* xchgl */, X86::XCHG32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 11116 /* xchgl */, X86::XCHG32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 11122 /* xchgq */, X86::XCHG64ar, Convert__Reg1_1, 0, { MCK__PCT_rax, MCK_GR64 }, 1},
  { 11122 /* xchgq */, X86::XCHG64ar, Convert__Reg1_1, 0, { MCK_RAX, MCK_GR64 }, 0},
  { 11122 /* xchgq */, X86::XCHG64ar, Convert__Reg1_0, 0, { MCK_GR64, MCK_RAX }, 0},
  { 11122 /* xchgq */, X86::XCHG64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 11122 /* xchgq */, X86::XCHG64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 11122 /* xchgq */, X86::XCHG64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 11122 /* xchgq */, X86::XCHG64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 11128 /* xchgw */, X86::XCHG16ar, Convert__Reg1_1, 0, { MCK__PCT_ax, MCK_GR16 }, 1},
  { 11128 /* xchgw */, X86::XCHG16ar, Convert__Reg1_1, 0, { MCK_AX, MCK_GR16 }, 0},
  { 11128 /* xchgw */, X86::XCHG16ar, Convert__Reg1_0, 0, { MCK_GR16, MCK_AX }, 0},
  { 11128 /* xchgw */, X86::XCHG16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 11128 /* xchgw */, X86::XCHG16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 11128 /* xchgw */, X86::XCHG16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 11128 /* xchgw */, X86::XCHG16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 11134 /* xcryptcbc */, X86::XCRYPTCBC, Convert_NoOperands, 0, {  }, 0},
  { 11134 /* xcryptcbc */, X86::XCRYPTCBC, Convert_NoOperands, 0, {  }, 1},
  { 11144 /* xcryptcfb */, X86::XCRYPTCFB, Convert_NoOperands, 0, {  }, 0},
  { 11144 /* xcryptcfb */, X86::XCRYPTCFB, Convert_NoOperands, 0, {  }, 1},
  { 11154 /* xcryptctr */, X86::XCRYPTCTR, Convert_NoOperands, 0, {  }, 0},
  { 11154 /* xcryptctr */, X86::XCRYPTCTR, Convert_NoOperands, 0, {  }, 1},
  { 11164 /* xcryptecb */, X86::XCRYPTECB, Convert_NoOperands, 0, {  }, 0},
  { 11164 /* xcryptecb */, X86::XCRYPTECB, Convert_NoOperands, 0, {  }, 1},
  { 11174 /* xcryptofb */, X86::XCRYPTOFB, Convert_NoOperands, 0, {  }, 0},
  { 11174 /* xcryptofb */, X86::XCRYPTOFB, Convert_NoOperands, 0, {  }, 1},
  { 11184 /* xend */, X86::XEND, Convert_NoOperands, 0, {  }, 0},
  { 11184 /* xend */, X86::XEND, Convert_NoOperands, 0, {  }, 1},
  { 11189 /* xgetbv */, X86::XGETBV, Convert_NoOperands, 0, {  }, 0},
  { 11189 /* xgetbv */, X86::XGETBV, Convert_NoOperands, 0, {  }, 1},
  { 11196 /* xlatb */, X86::XLAT, Convert_NoOperands, 0, {  }, 0},
  { 11196 /* xlatb */, X86::XLAT, Convert_NoOperands, 0, {  }, 1},
  { 11202 /* xor */, X86::XOR8i8, Convert__Imm1_1, 0, { MCK_AL, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR8rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR8, MCK_GR8 }, 1},
  { 11202 /* xor */, X86::XOR8ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR8, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR8rm, Convert__Reg1_0__Tie0__Mem85_1, 0, { MCK_GR8, MCK_Mem8 }, 1},
  { 11202 /* xor */, X86::XOR16i16, Convert__Imm1_1, 0, { MCK_AX, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR16rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR16, MCK_GR16 }, 1},
  { 11202 /* xor */, X86::XOR16ri8, Convert__Reg1_0__Tie0__ImmSExti16i81_1, 0, { MCK_GR16, MCK_ImmSExti16i8 }, 1},
  { 11202 /* xor */, X86::XOR16ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR16, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR16rm, Convert__Reg1_0__Tie0__Mem165_1, 0, { MCK_GR16, MCK_Mem16 }, 1},
  { 11202 /* xor */, X86::XOR32i32, Convert__Imm1_1, 0, { MCK_EAX, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR32rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR32, MCK_GR32 }, 1},
  { 11202 /* xor */, X86::XOR32ri8, Convert__Reg1_0__Tie0__ImmSExti32i81_1, 0, { MCK_GR32, MCK_ImmSExti32i8 }, 1},
  { 11202 /* xor */, X86::XOR32ri, Convert__Reg1_0__Tie0__Imm1_1, 0, { MCK_GR32, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR32rm, Convert__Reg1_0__Tie0__Mem325_1, 0, { MCK_GR32, MCK_Mem32 }, 1},
  { 11202 /* xor */, X86::XOR64i32, Convert__ImmSExti64i321_1, 0, { MCK_RAX, MCK_ImmSExti64i32 }, 1},
  { 11202 /* xor */, X86::XOR64rr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_GR64, MCK_GR64 }, 1},
  { 11202 /* xor */, X86::XOR64ri8, Convert__Reg1_0__Tie0__ImmSExti64i81_1, 0, { MCK_GR64, MCK_ImmSExti64i8 }, 1},
  { 11202 /* xor */, X86::XOR64ri32, Convert__Reg1_0__Tie0__ImmSExti64i321_1, 0, { MCK_GR64, MCK_ImmSExti64i32 }, 1},
  { 11202 /* xor */, X86::XOR64rm, Convert__Reg1_0__Tie0__Mem645_1, 0, { MCK_GR64, MCK_Mem64 }, 1},
  { 11202 /* xor */, X86::XOR16mr, Convert__Mem165_0__Reg1_1, 0, { MCK_Mem16, MCK_GR16 }, 1},
  { 11202 /* xor */, X86::XOR16mi8, Convert__Mem165_0__ImmSExti16i81_1, 0, { MCK_Mem16, MCK_ImmSExti16i8 }, 1},
  { 11202 /* xor */, X86::XOR16mi, Convert__Mem165_0__Imm1_1, 0, { MCK_Mem16, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR32mr, Convert__Mem325_0__Reg1_1, 0, { MCK_Mem32, MCK_GR32 }, 1},
  { 11202 /* xor */, X86::XOR32mi8, Convert__Mem325_0__ImmSExti32i81_1, 0, { MCK_Mem32, MCK_ImmSExti32i8 }, 1},
  { 11202 /* xor */, X86::XOR32mi, Convert__Mem325_0__Imm1_1, 0, { MCK_Mem32, MCK_Imm }, 1},
  { 11202 /* xor */, X86::XOR64mr, Convert__Mem645_0__Reg1_1, 0, { MCK_Mem64, MCK_GR64 }, 1},
  { 11202 /* xor */, X86::XOR64mi8, Convert__Mem645_0__ImmSExti64i81_1, 0, { MCK_Mem64, MCK_ImmSExti64i8 }, 1},
  { 11202 /* xor */, X86::XOR64mi32, Convert__Mem645_0__ImmSExti64i321_1, 0, { MCK_Mem64, MCK_ImmSExti64i32 }, 1},
  { 11202 /* xor */, X86::XOR8mr, Convert__Mem85_0__Reg1_1, 0, { MCK_Mem8, MCK_GR8 }, 1},
  { 11202 /* xor */, X86::XOR8mi, Convert__Mem85_0__Imm1_1, 0, { MCK_Mem8, MCK_Imm }, 1},
  { 11206 /* xorb */, X86::XOR8rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR8, MCK_GR8 }, 0},
  { 11206 /* xorb */, X86::XOR8mr, Convert__Mem85_1__Reg1_0, 0, { MCK_GR8, MCK_Mem8 }, 0},
  { 11206 /* xorb */, X86::XOR8i8, Convert__Imm1_0, 0, { MCK_Imm, MCK_AL }, 0},
  { 11206 /* xorb */, X86::XOR8ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR8 }, 0},
  { 11206 /* xorb */, X86::XOR8mi, Convert__Mem85_1__Imm1_0, 0, { MCK_Imm, MCK_Mem8 }, 0},
  { 11206 /* xorb */, X86::XOR8rm, Convert__Reg1_1__Tie0__Mem85_0, 0, { MCK_Mem8, MCK_GR8 }, 0},
  { 11211 /* xorl */, X86::XOR32rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR32, MCK_GR32 }, 0},
  { 11211 /* xorl */, X86::XOR32mr, Convert__Mem325_1__Reg1_0, 0, { MCK_GR32, MCK_Mem32 }, 0},
  { 11211 /* xorl */, X86::XOR32ri8, Convert__Reg1_1__Tie0__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_GR32 }, 0},
  { 11211 /* xorl */, X86::XOR32mi8, Convert__Mem325_1__ImmSExti32i81_0, 0, { MCK_ImmSExti32i8, MCK_Mem32 }, 0},
  { 11211 /* xorl */, X86::XOR32i32, Convert__Imm1_0, 0, { MCK_Imm, MCK_EAX }, 0},
  { 11211 /* xorl */, X86::XOR32ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR32 }, 0},
  { 11211 /* xorl */, X86::XOR32mi, Convert__Mem325_1__Imm1_0, 0, { MCK_Imm, MCK_Mem32 }, 0},
  { 11211 /* xorl */, X86::XOR32rm, Convert__Reg1_1__Tie0__Mem325_0, 0, { MCK_Mem32, MCK_GR32 }, 0},
  { 11216 /* xorpd */, X86::FsXORPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 11216 /* xorpd */, X86::XORPDrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 11216 /* xorpd */, X86::FsXORPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 11216 /* xorpd */, X86::XORPDrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 11216 /* xorpd */, X86::FsXORPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 11216 /* xorpd */, X86::XORPDrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 11216 /* xorpd */, X86::FsXORPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 11216 /* xorpd */, X86::XORPDrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 11222 /* xorps */, X86::FsXORPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 11222 /* xorps */, X86::XORPSrr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_FR32, MCK_FR32 }, 0},
  { 11222 /* xorps */, X86::FsXORPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 11222 /* xorps */, X86::XORPSrr, Convert__Reg1_0__Tie0__Reg1_1, 0, { MCK_FR32, MCK_FR32 }, 1},
  { 11222 /* xorps */, X86::FsXORPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 11222 /* xorps */, X86::XORPSrm, Convert__Reg1_0__Tie0__Mem1285_1, 0, { MCK_FR32, MCK_Mem128 }, 1},
  { 11222 /* xorps */, X86::FsXORPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 11222 /* xorps */, X86::XORPSrm, Convert__Reg1_1__Tie0__Mem1285_0, 0, { MCK_Mem128, MCK_FR32 }, 0},
  { 11228 /* xorq */, X86::XOR64rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR64, MCK_GR64 }, 0},
  { 11228 /* xorq */, X86::XOR64mr, Convert__Mem645_1__Reg1_0, 0, { MCK_GR64, MCK_Mem64 }, 0},
  { 11228 /* xorq */, X86::XOR64ri8, Convert__Reg1_1__Tie0__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_GR64 }, 0},
  { 11228 /* xorq */, X86::XOR64mi8, Convert__Mem645_1__ImmSExti64i81_0, 0, { MCK_ImmSExti64i8, MCK_Mem64 }, 0},
  { 11228 /* xorq */, X86::XOR64i32, Convert__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_RAX }, 0},
  { 11228 /* xorq */, X86::XOR64ri32, Convert__Reg1_1__Tie0__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_GR64 }, 0},
  { 11228 /* xorq */, X86::XOR64mi32, Convert__Mem645_1__ImmSExti64i321_0, 0, { MCK_ImmSExti64i32, MCK_Mem64 }, 0},
  { 11228 /* xorq */, X86::XOR64rm, Convert__Reg1_1__Tie0__Mem645_0, 0, { MCK_Mem64, MCK_GR64 }, 0},
  { 11233 /* xorw */, X86::XOR16rr, Convert__Reg1_1__Tie0__Reg1_0, 0, { MCK_GR16, MCK_GR16 }, 0},
  { 11233 /* xorw */, X86::XOR16mr, Convert__Mem165_1__Reg1_0, 0, { MCK_GR16, MCK_Mem16 }, 0},
  { 11233 /* xorw */, X86::XOR16ri8, Convert__Reg1_1__Tie0__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_GR16 }, 0},
  { 11233 /* xorw */, X86::XOR16mi8, Convert__Mem165_1__ImmSExti16i81_0, 0, { MCK_ImmSExti16i8, MCK_Mem16 }, 0},
  { 11233 /* xorw */, X86::XOR16i16, Convert__Imm1_0, 0, { MCK_Imm, MCK_AX }, 0},
  { 11233 /* xorw */, X86::XOR16ri, Convert__Reg1_1__Tie0__Imm1_0, 0, { MCK_Imm, MCK_GR16 }, 0},
  { 11233 /* xorw */, X86::XOR16mi, Convert__Mem165_1__Imm1_0, 0, { MCK_Imm, MCK_Mem16 }, 0},
  { 11233 /* xorw */, X86::XOR16rm, Convert__Reg1_1__Tie0__Mem165_0, 0, { MCK_Mem16, MCK_GR16 }, 0},
  { 11238 /* xrstor */, X86::XRSTOR, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 11238 /* xrstor */, X86::XRSTOR, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 11245 /* xrstorq */, X86::XRSTOR64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 0},
  { 11245 /* xrstorq */, X86::XRSTOR64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 1},
  { 11253 /* xsave */, X86::XSAVE, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 11253 /* xsave */, X86::XSAVE, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 11259 /* xsaveopt */, X86::XSAVEOPT, Convert__Mem5_0, 0, { MCK_Mem }, 0},
  { 11259 /* xsaveopt */, X86::XSAVEOPT, Convert__Mem5_0, 0, { MCK_Mem }, 1},
  { 11268 /* xsaveoptq */, X86::XSAVEOPT64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 0},
  { 11268 /* xsaveoptq */, X86::XSAVEOPT64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 1},
  { 11278 /* xsaveq */, X86::XSAVE64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 0},
  { 11278 /* xsaveq */, X86::XSAVE64, Convert__Mem5_0, Feature_In64BitMode, { MCK_Mem }, 1},
  { 11285 /* xsetbv */, X86::XSETBV, Convert_NoOperands, 0, {  }, 0},
  { 11285 /* xsetbv */, X86::XSETBV, Convert_NoOperands, 0, {  }, 1},
  { 11292 /* xsha1 */, X86::XSHA1, Convert_NoOperands, 0, {  }, 0},
  { 11292 /* xsha1 */, X86::XSHA1, Convert_NoOperands, 0, {  }, 1},
  { 11298 /* xsha256 */, X86::XSHA256, Convert_NoOperands, 0, {  }, 0},
  { 11298 /* xsha256 */, X86::XSHA256, Convert_NoOperands, 0, {  }, 1},
  { 11306 /* xstore */, X86::XSTORE, Convert_NoOperands, 0, {  }, 0},
  { 11306 /* xstore */, X86::XSTORE, Convert_NoOperands, 0, {  }, 1},
  { 11313 /* xstorerng */, X86::XSTORE, Convert_NoOperands, 0, {  }, 0},
  { 11313 /* xstorerng */, X86::XSTORE, Convert_NoOperands, 0, {  }, 1},
};

bool X86AsmParser::
mnemonicIsValid(StringRef Mnemonic) {
  // Search the table.
  std::pair<const MatchEntry*, const MatchEntry*> MnemonicRange =
    std::equal_range(MatchTable, MatchTable+7830, Mnemonic, LessOpcode());
  return MnemonicRange.first != MnemonicRange.second;
}

unsigned X86AsmParser::
MatchInstructionImpl(const SmallVectorImpl<MCParsedAsmOperand*> &Operands,
                     MCInst &Inst,
unsigned &ErrorInfo, bool matchingInlineAsm, unsigned VariantID) {
  // Eliminate obvious mismatches.
  if (Operands.size() > 6) {
    ErrorInfo = 6;
    return Match_InvalidOperand;
  }

  // Get the current feature set.
  unsigned AvailableFeatures = getAvailableFeatures();

  // Get the instruction mnemonic, which is the first token.
  StringRef Mnemonic = ((X86Operand*)Operands[0])->getToken();

  // Process all MnemonicAliases to remap the mnemonic.
  // FIXME : Add an entry in AsmParserVariant to check this.
  if (!VariantID)
    applyMnemonicAliases(Mnemonic, AvailableFeatures);

  // Some state to try to produce better error messages.
  bool HadMatchOtherThanFeatures = false;
  bool HadMatchOtherThanPredicate = false;
  unsigned RetCode = Match_InvalidOperand;
  unsigned MissingFeatures = ~0U;
  // Set ErrorInfo to the operand that mismatches if it is
  // wrong for all instances of the instruction.
  ErrorInfo = ~0U;
  // Search the table.
  std::pair<const MatchEntry*, const MatchEntry*> MnemonicRange =
    std::equal_range(MatchTable, MatchTable+7830, Mnemonic, LessOpcode());

  // Return a more specific error code if no mnemonics match.
  if (MnemonicRange.first == MnemonicRange.second)
    return Match_MnemonicFail;

  for (const MatchEntry *it = MnemonicRange.first, *ie = MnemonicRange.second;
       it != ie; ++it) {
    // equal_range guarantees that instruction mnemonic matches.
    assert(Mnemonic == it->getMnemonic());
    if (VariantID != it->AsmVariantID) continue;
    bool OperandsValid = true;
    for (unsigned i = 0; i != 5; ++i) {
      if (i + 1 >= Operands.size()) {
        OperandsValid = (it->Classes[i] == InvalidMatchClass);
        if (!OperandsValid) ErrorInfo = i + 1;
        break;
      }
      unsigned Diag = validateOperandClass(Operands[i+1],
                                           (MatchClassKind)it->Classes[i]);
      if (Diag == Match_Success)
        continue;
      // If this operand is broken for all of the instances of this
      // mnemonic, keep track of it so we can report loc info.
      // If we already had a match that only failed due to a
      // target predicate, that diagnostic is preferred.
      if (!HadMatchOtherThanPredicate &&
          (it == MnemonicRange.first || ErrorInfo <= i+1)) {
        ErrorInfo = i+1;
        // InvalidOperand is the default. Prefer specificity.
        if (Diag != Match_InvalidOperand)
          RetCode = Diag;
      }
      // Otherwise, just reject this instance of the mnemonic.
      OperandsValid = false;
      break;
    }

    if (!OperandsValid) continue;
    if ((AvailableFeatures & it->RequiredFeatures) != it->RequiredFeatures) {
      HadMatchOtherThanFeatures = true;
      unsigned NewMissingFeatures = it->RequiredFeatures & ~AvailableFeatures;
      if (CountPopulation_32(NewMissingFeatures) <=
          CountPopulation_32(MissingFeatures))
        MissingFeatures = NewMissingFeatures;
      continue;
    }

    if (matchingInlineAsm) {
      Inst.setOpcode(it->Opcode);
      convertToMapAndConstraints(it->ConvertFn, Operands);
      return Match_Success;
    }

    // We have selected a definite instruction, convert the parsed
    // operands into the appropriate MCInst.
    convertToMCInst(it->ConvertFn, Inst, it->Opcode, Operands);

    // We have a potential match. Check the target predicate to
    // handle any context sensitive constraints.
    unsigned MatchResult;
    if ((MatchResult = checkTargetMatchPredicate(Inst)) != Match_Success) {
      Inst.clear();
      RetCode = MatchResult;
      HadMatchOtherThanPredicate = true;
      continue;
    }

    return Match_Success;
  }

  // Okay, we had no match.  Try to return a useful error code.
  if (HadMatchOtherThanPredicate || !HadMatchOtherThanFeatures)
    return RetCode;

  // Missing feature matches return which features were missing
  ErrorInfo = MissingFeatures;
  return Match_MissingFeature;
}

#endif // GET_MATCHER_IMPLEMENTATION

