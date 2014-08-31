PROJECT(LLVMPlugin)

MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve some LLVM settings

    IF(WIN32)
        SET(LLVM_DEFINITIONS)
    ELSE()
        SET(LLVM_DEFINITIONS
            __STDC_CONSTANT_MACROS
            __STDC_LIMIT_MACROS
        )

        IF(NOT APPLE)
            LIST(APPEND LLVM_DEFINITIONS
                __STDC_FORMAT_MACROS
                # Note: this is to address a couple of issues with PRIx64 in
                #       [LLVM]/lib/CodeGen/AsmPrinter/AsmPrinter.cpp...
            )
        ENDIF()
    ENDIF()

    # Ignore some warnings on Windows, as well as allow for big objects
    # Note: we have nothing to do with those warnings, so...

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4267")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /wd4180 /wd4244 /wd4355 /wd4291 /wd4351 /wd4624 /wd4722 /wd4996")
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

SET(CONFIG_FILES
    include/llvm/Config/DISTRIB_DIR/config.h
    include/llvm/Config/DISTRIB_DIR/llvm-config.h

    include/llvm/Support/DISTRIB_DIR/DataTypes.h
)

FOREACH(CONFIG_FILE ${CONFIG_FILES})
    STRING(REPLACE "DISTRIB_DIR/" "${DISTRIB_DIR}/" CONFIG_FILE_ORIG "${CONFIG_FILE}")
    STRING(REPLACE "DISTRIB_DIR/" "" CONFIG_FILE_DEST "${CONFIG_FILE}")

    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/${CONFIG_FILE_ORIG}
                   ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_DEST}
                   COPYONLY)
ENDFOREACH()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LLVM_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll 1d8a21b24c5613adc16cac21e9a9f5b6f88f7ac9)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib b861a9e66bb3901fe66d0d91e59ec49e4e3c7eff)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll 1d4621b059d7f826e49cf2cf254942960eece795)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib 6ee73e97a58ccbfdd71b850fd5381d042ed803c1)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll e2e08b014103cbf9c36716de1f1e9906c2bcd15d)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib c1be02f4d222c68627cd6124f03a51f78d2dae7b)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll 6e0882cb032640bf20c9ad399a0e8a2c48285682)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib eb37f2252d4e0a6346c5b71d6cb14ff81d8ded84)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLLVM.dylib ba21faa0a2e2b793e9d2553b663c68110488a6e2)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLLVM.so 4c3cc5c459d8c1d029349c2397e7eb33863925de)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLLVM.so 448541f15ed7bdd79f63c3fac071accb879bc04f)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, must refer to the distribution directory as
    #       opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(LLVM_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}LLVM${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(LLVM_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}LLVM${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(LLVM
        INCLUDE_DIRS
            include
            lib/Target/X86
            src
            tools/clang/include
        QT_LIBRARIES
            QtCore
    )
ELSE()
    # Retrieve some include files that were generated on each of our target
    # platforms

    SET(CONFIG_FILES
        lib/Target/X86/DISTRIB_DIR/X86GenAsmMatcher.inc
    )

    FOREACH(CONFIG_FILE ${CONFIG_FILES})
        STRING(REPLACE "DISTRIB_DIR/" "${DISTRIB_DIR}/" CONFIG_FILE_ORIG "${CONFIG_FILE}")
        STRING(REPLACE "DISTRIB_DIR/" "" CONFIG_FILE_DEST "${CONFIG_FILE}")

        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/${CONFIG_FILE_ORIG}
                       ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_DEST}
                       COPYONLY)
    ENDFOREACH()

    # Build a special assembly file on 64-bit Windows

    IF(WIN32 AND ${ARCHITECTURE} EQUAL 64)
        ENABLE_LANGUAGE(ASM_MASM)

        ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/X86CompilationCallback_Win64.obj
                           MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/lib/Target/X86/X86CompilationCallback_Win64.asm
                           COMMAND ${CMAKE_ASM_MASM_COMPILER} /Fo ${CMAKE_CURRENT_BINARY_DIR}/X86CompilationCallback_Win64.obj /c ${PROJECT_SOURCE_DIR}/lib/Target/X86/X86CompilationCallback_Win64.asm
        )

        SET(ASM_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/X86CompilationCallback_Win64.obj)
    ELSE()
        SET(ASM_OBJECT)
    ENDIF()

    # Retrieve some LLVM settings

    RETRIEVE_LLVM_SETTINGS()

    # Add the plugin

    SET(LLVM_PLUGIN LLVM PARENT_SCOPE)

    ADD_PLUGIN(LLVM
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            ${ASM_OBJECT}

            lib/Analysis/AliasAnalysis.cpp
            lib/Analysis/AliasAnalysisCounter.cpp
            lib/Analysis/AliasAnalysisEvaluator.cpp
            lib/Analysis/AliasDebugger.cpp
            lib/Analysis/AliasSetTracker.cpp
            lib/Analysis/Analysis.cpp
            lib/Analysis/BasicAliasAnalysis.cpp
            lib/Analysis/BlockFrequencyInfo.cpp
            lib/Analysis/BranchProbabilityInfo.cpp
            lib/Analysis/CaptureTracking.cpp
            lib/Analysis/CFG.cpp
            lib/Analysis/CFGPrinter.cpp
            lib/Analysis/CodeMetrics.cpp
            lib/Analysis/ConstantFolding.cpp
            lib/Analysis/CostModel.cpp
            lib/Analysis/Delinearization.cpp
            lib/Analysis/DependenceAnalysis.cpp
            lib/Analysis/DominanceFrontier.cpp
            lib/Analysis/DomPrinter.cpp
            lib/Analysis/InstCount.cpp
            lib/Analysis/InstructionSimplify.cpp
            lib/Analysis/Interval.cpp
            lib/Analysis/IntervalPartition.cpp
            lib/Analysis/IVUsers.cpp
            lib/Analysis/LazyValueInfo.cpp
            lib/Analysis/LibCallAliasAnalysis.cpp
            lib/Analysis/LibCallSemantics.cpp
            lib/Analysis/Lint.cpp
            lib/Analysis/Loads.cpp
            lib/Analysis/LoopInfo.cpp
            lib/Analysis/LoopPass.cpp
            lib/Analysis/MemDepPrinter.cpp
            lib/Analysis/MemoryBuiltins.cpp
            lib/Analysis/MemoryDependenceAnalysis.cpp
            lib/Analysis/ModuleDebugInfoPrinter.cpp
            lib/Analysis/NoAliasAnalysis.cpp
            lib/Analysis/PHITransAddr.cpp
            lib/Analysis/PostDominators.cpp
            lib/Analysis/PtrUseVisitor.cpp
            lib/Analysis/RegionInfo.cpp
            lib/Analysis/RegionPass.cpp
            lib/Analysis/RegionPrinter.cpp
            lib/Analysis/ScalarEvolution.cpp
            lib/Analysis/ScalarEvolutionAliasAnalysis.cpp
            lib/Analysis/ScalarEvolutionExpander.cpp
            lib/Analysis/ScalarEvolutionNormalization.cpp
            lib/Analysis/SparsePropagation.cpp
            lib/Analysis/TargetTransformInfo.cpp
            lib/Analysis/Trace.cpp
            lib/Analysis/TypeBasedAliasAnalysis.cpp
            lib/Analysis/ValueTracking.cpp

            lib/Analysis/IPA/CallGraph.cpp
            lib/Analysis/IPA/CallGraphSCCPass.cpp
            lib/Analysis/IPA/CallPrinter.cpp
            lib/Analysis/IPA/FindUsedTypes.cpp
            lib/Analysis/IPA/GlobalsModRef.cpp
            lib/Analysis/IPA/InlineCost.cpp
            lib/Analysis/IPA/IPA.cpp

            lib/AsmParser/LLLexer.cpp
            lib/AsmParser/LLParser.cpp
            lib/AsmParser/Parser.cpp

            lib/Bitcode/Reader/BitcodeReader.cpp
            lib/Bitcode/Reader/BitReader.cpp
            lib/Bitcode/Reader/BitstreamReader.cpp

            lib/Bitcode/Writer/BitcodeWriter.cpp
            lib/Bitcode/Writer/BitcodeWriterPass.cpp
            lib/Bitcode/Writer/BitWriter.cpp
            lib/Bitcode/Writer/ValueEnumerator.cpp

            lib/CodeGen/AggressiveAntiDepBreaker.cpp
            lib/CodeGen/AllocationOrder.cpp
            lib/CodeGen/Analysis.cpp
            lib/CodeGen/BasicTargetTransformInfo.cpp
            lib/CodeGen/BranchFolding.cpp
            lib/CodeGen/CalcSpillWeights.cpp
            lib/CodeGen/CallingConvLower.cpp
            lib/CodeGen/CodeGen.cpp
            lib/CodeGen/CriticalAntiDepBreaker.cpp
            lib/CodeGen/DeadMachineInstructionElim.cpp
            lib/CodeGen/DFAPacketizer.cpp
            lib/CodeGen/DwarfEHPrepare.cpp
            lib/CodeGen/EarlyIfConversion.cpp
            lib/CodeGen/EdgeBundles.cpp
            lib/CodeGen/ErlangGC.cpp
            lib/CodeGen/ExecutionDepsFix.cpp
            lib/CodeGen/ExpandISelPseudos.cpp
            lib/CodeGen/ExpandPostRAPseudos.cpp
            lib/CodeGen/GCMetadata.cpp
            lib/CodeGen/GCMetadataPrinter.cpp
            lib/CodeGen/GCStrategy.cpp
            lib/CodeGen/IfConversion.cpp
            lib/CodeGen/InlineSpiller.cpp
            lib/CodeGen/InterferenceCache.cpp
            lib/CodeGen/IntrinsicLowering.cpp
            lib/CodeGen/JITCodeEmitter.cpp
            lib/CodeGen/LatencyPriorityQueue.cpp
            lib/CodeGen/LexicalScopes.cpp
            lib/CodeGen/LiveDebugVariables.cpp
            lib/CodeGen/LiveInterval.cpp
            lib/CodeGen/LiveIntervalAnalysis.cpp
            lib/CodeGen/LiveIntervalUnion.cpp
            lib/CodeGen/LiveRangeCalc.cpp
            lib/CodeGen/LiveRangeEdit.cpp
            lib/CodeGen/LiveRegMatrix.cpp
            lib/CodeGen/LiveRegUnits.cpp
            lib/CodeGen/LiveStackAnalysis.cpp
            lib/CodeGen/LiveVariables.cpp
            lib/CodeGen/LLVMTargetMachine.cpp
            lib/CodeGen/LocalStackSlotAllocation.cpp
            lib/CodeGen/MachineBasicBlock.cpp
            lib/CodeGen/MachineBlockFrequencyInfo.cpp
            lib/CodeGen/MachineBlockPlacement.cpp
            lib/CodeGen/MachineBranchProbabilityInfo.cpp
            lib/CodeGen/MachineCodeEmitter.cpp
            lib/CodeGen/MachineCopyPropagation.cpp
            lib/CodeGen/MachineCSE.cpp
            lib/CodeGen/MachineDominators.cpp
            lib/CodeGen/MachineFunction.cpp
            lib/CodeGen/MachineFunctionAnalysis.cpp
            lib/CodeGen/MachineFunctionPass.cpp
            lib/CodeGen/MachineFunctionPrinterPass.cpp
            lib/CodeGen/MachineInstr.cpp
            lib/CodeGen/MachineInstrBundle.cpp
            lib/CodeGen/MachineLICM.cpp
            lib/CodeGen/MachineLoopInfo.cpp
            lib/CodeGen/MachineModuleInfo.cpp
            lib/CodeGen/MachineModuleInfoImpls.cpp
            lib/CodeGen/MachinePassRegistry.cpp
            lib/CodeGen/MachinePostDominators.cpp
            lib/CodeGen/MachineRegisterInfo.cpp
            lib/CodeGen/MachineScheduler.cpp
            lib/CodeGen/MachineSink.cpp
            lib/CodeGen/MachineSSAUpdater.cpp
            lib/CodeGen/MachineTraceMetrics.cpp
            lib/CodeGen/MachineVerifier.cpp
            lib/CodeGen/OcamlGC.cpp
            lib/CodeGen/OptimizePHIs.cpp
            lib/CodeGen/Passes.cpp
            lib/CodeGen/PeepholeOptimizer.cpp
            lib/CodeGen/PHIElimination.cpp
            lib/CodeGen/PHIEliminationUtils.cpp
            lib/CodeGen/PostRASchedulerList.cpp
            lib/CodeGen/ProcessImplicitDefs.cpp
            lib/CodeGen/PrologEpilogInserter.cpp
            lib/CodeGen/PseudoSourceValue.cpp
            lib/CodeGen/RegAllocBase.cpp
            lib/CodeGen/RegAllocBasic.cpp
            lib/CodeGen/RegAllocFast.cpp
            lib/CodeGen/RegAllocGreedy.cpp
            lib/CodeGen/RegAllocPBQP.cpp
            lib/CodeGen/RegisterClassInfo.cpp
            lib/CodeGen/RegisterCoalescer.cpp
            lib/CodeGen/RegisterPressure.cpp
            lib/CodeGen/RegisterScavenging.cpp
            lib/CodeGen/ScheduleDAG.cpp
            lib/CodeGen/ScheduleDAGInstrs.cpp
            lib/CodeGen/ScheduleDAGPrinter.cpp
            lib/CodeGen/ScoreboardHazardRecognizer.cpp
            lib/CodeGen/ShadowStackGC.cpp
            lib/CodeGen/SjLjEHPrepare.cpp
            lib/CodeGen/SlotIndexes.cpp
            lib/CodeGen/Spiller.cpp
            lib/CodeGen/SpillPlacement.cpp
            lib/CodeGen/SplitKit.cpp
            lib/CodeGen/StackColoring.cpp
            lib/CodeGen/StackMaps.cpp
            lib/CodeGen/StackProtector.cpp
            lib/CodeGen/StackSlotColoring.cpp
            lib/CodeGen/TailDuplication.cpp
            lib/CodeGen/TargetFrameLoweringImpl.cpp
            lib/CodeGen/TargetInstrInfo.cpp
            lib/CodeGen/TargetLoweringBase.cpp
            lib/CodeGen/TargetLoweringObjectFileImpl.cpp
            lib/CodeGen/TargetOptionsImpl.cpp
            lib/CodeGen/TargetRegisterInfo.cpp
            lib/CodeGen/TargetSchedule.cpp
            lib/CodeGen/TwoAddressInstructionPass.cpp
            lib/CodeGen/UnreachableBlockElim.cpp
            lib/CodeGen/VirtRegMap.cpp

            lib/CodeGen/AsmPrinter/ARMException.cpp
            lib/CodeGen/AsmPrinter/AsmPrinter.cpp
            lib/CodeGen/AsmPrinter/AsmPrinterDwarf.cpp
            lib/CodeGen/AsmPrinter/AsmPrinterInlineAsm.cpp
            lib/CodeGen/AsmPrinter/DIE.cpp
            lib/CodeGen/AsmPrinter/DIEHash.cpp
            lib/CodeGen/AsmPrinter/DwarfAccelTable.cpp
            lib/CodeGen/AsmPrinter/DwarfCFIException.cpp
            lib/CodeGen/AsmPrinter/DwarfCompileUnit.cpp
            lib/CodeGen/AsmPrinter/DwarfDebug.cpp
            lib/CodeGen/AsmPrinter/DwarfException.cpp
            lib/CodeGen/AsmPrinter/ErlangGCPrinter.cpp
            lib/CodeGen/AsmPrinter/OcamlGCPrinter.cpp
            lib/CodeGen/AsmPrinter/Win64Exception.cpp

            lib/CodeGen/SelectionDAG/DAGCombiner.cpp
            lib/CodeGen/SelectionDAG/FastISel.cpp
            lib/CodeGen/SelectionDAG/FunctionLoweringInfo.cpp
            lib/CodeGen/SelectionDAG/InstrEmitter.cpp
            lib/CodeGen/SelectionDAG/LegalizeDAG.cpp
            lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp
            lib/CodeGen/SelectionDAG/LegalizeIntegerTypes.cpp
            lib/CodeGen/SelectionDAG/LegalizeTypes.cpp
            lib/CodeGen/SelectionDAG/LegalizeTypesGeneric.cpp
            lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp
            lib/CodeGen/SelectionDAG/LegalizeVectorTypes.cpp
            lib/CodeGen/SelectionDAG/ResourcePriorityQueue.cpp
            lib/CodeGen/SelectionDAG/ScheduleDAGFast.cpp
            lib/CodeGen/SelectionDAG/ScheduleDAGRRList.cpp
            lib/CodeGen/SelectionDAG/ScheduleDAGSDNodes.cpp
            lib/CodeGen/SelectionDAG/ScheduleDAGVLIW.cpp
            lib/CodeGen/SelectionDAG/SelectionDAG.cpp
            lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp
            lib/CodeGen/SelectionDAG/SelectionDAGDumper.cpp
            lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp
            lib/CodeGen/SelectionDAG/SelectionDAGPrinter.cpp
            lib/CodeGen/SelectionDAG/TargetLowering.cpp
            lib/CodeGen/SelectionDAG/TargetSelectionDAGInfo.cpp

            lib/DebugInfo/DIContext.cpp
            lib/DebugInfo/DWARFAbbreviationDeclaration.cpp
            lib/DebugInfo/DWARFCompileUnit.cpp
            lib/DebugInfo/DWARFContext.cpp
            lib/DebugInfo/DWARFDebugAbbrev.cpp
            lib/DebugInfo/DWARFDebugAranges.cpp
            lib/DebugInfo/DWARFDebugArangeSet.cpp
            lib/DebugInfo/DWARFDebugFrame.cpp
            lib/DebugInfo/DWARFDebugInfoEntry.cpp
            lib/DebugInfo/DWARFDebugLine.cpp
            lib/DebugInfo/DWARFDebugLoc.cpp
            lib/DebugInfo/DWARFDebugRangeList.cpp
            lib/DebugInfo/DWARFFormValue.cpp
            lib/DebugInfo/DWARFTypeUnit.cpp
            lib/DebugInfo/DWARFUnit.cpp

            lib/ExecutionEngine/ExecutionEngine.cpp
            lib/ExecutionEngine/ExecutionEngineBindings.cpp
            lib/ExecutionEngine/RTDyldMemoryManager.cpp
            lib/ExecutionEngine/TargetSelect.cpp

#            lib/ExecutionEngine/IntelJITEvents/IntelJITEventListener.cpp
#            lib/ExecutionEngine/IntelJITEvents/jitprofiling.c
# Note: we don't need the above, but we leave them so we know about them next
#       time we update LLVM. Indeed, Intel JIT API support is only available on
#       Linux and Windows, and it requires additional features that may or not
#       be present on the build machine, so...

            lib/ExecutionEngine/Interpreter/Execution.cpp
            lib/ExecutionEngine/Interpreter/ExternalFunctions.cpp
            lib/ExecutionEngine/Interpreter/Interpreter.cpp

            lib/ExecutionEngine/JIT/JIT.cpp
            lib/ExecutionEngine/JIT/JITEmitter.cpp
            lib/ExecutionEngine/JIT/JITMemoryManager.cpp

            lib/ExecutionEngine/MCJIT/MCJIT.cpp
            lib/ExecutionEngine/MCJIT/SectionMemoryManager.cpp

#            lib/ExecutionEngine/OProfileJIT/OProfileJITEventListener.cpp
#            lib/ExecutionEngine/OProfileJIT/OProfileWrapper.cpp
# Note: we don't need the above, but we leave them so we know about them next
#       time we update LLVM. Indeed, OProfile support is only available on
#       Linux, and it requires additional features that may or not be present
#       on the build machine, so...

            lib/ExecutionEngine/RuntimeDyld/GDBRegistrar.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyld.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldELF.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldMachO.cpp

            lib/IR/AsmWriter.cpp
            lib/IR/Attributes.cpp
            lib/IR/AutoUpgrade.cpp
            lib/IR/BasicBlock.cpp
            lib/IR/ConstantFold.cpp
            lib/IR/Constants.cpp
            lib/IR/Core.cpp
            lib/IR/DataLayout.cpp
            lib/IR/DebugInfo.cpp
            lib/IR/DebugLoc.cpp
            lib/IR/DIBuilder.cpp
            lib/IR/Dominators.cpp
            lib/IR/Function.cpp
            lib/IR/GCOV.cpp
            lib/IR/Globals.cpp
            lib/IR/GVMaterializer.cpp
            lib/IR/InlineAsm.cpp
            lib/IR/Instruction.cpp
            lib/IR/Instructions.cpp
            lib/IR/IntrinsicInst.cpp
            lib/IR/IRBuilder.cpp
            lib/IR/LeakDetector.cpp
            lib/IR/LegacyPassManager.cpp
            lib/IR/LLVMContext.cpp
            lib/IR/LLVMContextImpl.cpp
            lib/IR/Metadata.cpp
            lib/IR/Module.cpp
            lib/IR/Pass.cpp
            lib/IR/PassManager.cpp
            lib/IR/PassRegistry.cpp
            lib/IR/PrintModulePass.cpp
            lib/IR/Type.cpp
            lib/IR/TypeFinder.cpp
            lib/IR/Use.cpp
            lib/IR/User.cpp
            lib/IR/Value.cpp
            lib/IR/ValueSymbolTable.cpp
            lib/IR/ValueTypes.cpp
            lib/IR/Verifier.cpp

            lib/IRReader/IRReader.cpp

            lib/Linker/LinkModules.cpp

            lib/LTO/LTOCodeGenerator.cpp
            lib/LTO/LTOModule.cpp

            lib/MC/ELFObjectWriter.cpp
            lib/MC/MachObjectWriter.cpp
            lib/MC/MCAsmBackend.cpp
            lib/MC/MCAsmInfo.cpp
            lib/MC/MCAsmInfoCOFF.cpp
            lib/MC/MCAsmInfoDarwin.cpp
            lib/MC/MCAsmInfoELF.cpp
            lib/MC/MCAsmStreamer.cpp
            lib/MC/MCAssembler.cpp
            lib/MC/MCAtom.cpp
            lib/MC/MCCodeEmitter.cpp
            lib/MC/MCCodeGenInfo.cpp
            lib/MC/MCContext.cpp
            lib/MC/MCDisassembler.cpp
            lib/MC/MCDwarf.cpp
            lib/MC/MCELF.cpp
            lib/MC/MCELFObjectTargetWriter.cpp
            lib/MC/MCELFStreamer.cpp
            lib/MC/MCExpr.cpp
            lib/MC/MCExternalSymbolizer.cpp
            lib/MC/MCFunction.cpp
            lib/MC/MCInst.cpp
            lib/MC/MCInstPrinter.cpp
            lib/MC/MCInstrAnalysis.cpp
            lib/MC/MCLabel.cpp
            lib/MC/MCMachObjectTargetWriter.cpp
            lib/MC/MCMachOStreamer.cpp
            lib/MC/MCModule.cpp
            lib/MC/MCModuleYAML.cpp
            lib/MC/MCNullStreamer.cpp
            lib/MC/MCObjectDisassembler.cpp
            lib/MC/MCObjectFileInfo.cpp
            lib/MC/MCObjectStreamer.cpp
            lib/MC/MCObjectSymbolizer.cpp
            lib/MC/MCObjectWriter.cpp
            lib/MC/MCPureStreamer.cpp
            lib/MC/MCRegisterInfo.cpp
            lib/MC/MCRelocationInfo.cpp
            lib/MC/MCSection.cpp
            lib/MC/MCSectionCOFF.cpp
            lib/MC/MCSectionELF.cpp
            lib/MC/MCSectionMachO.cpp
            lib/MC/MCStreamer.cpp
            lib/MC/MCSubtargetInfo.cpp
            lib/MC/MCSymbol.cpp
            lib/MC/MCSymbolizer.cpp
            lib/MC/MCValue.cpp
            lib/MC/MCWin64EH.cpp
            lib/MC/SubtargetFeature.cpp
            lib/MC/WinCOFFObjectWriter.cpp
            lib/MC/WinCOFFStreamer.cpp

            lib/MC/MCDisassembler/Disassembler.cpp

            lib/MC/MCParser/AsmLexer.cpp
            lib/MC/MCParser/AsmParser.cpp
            lib/MC/MCParser/COFFAsmParser.cpp
            lib/MC/MCParser/DarwinAsmParser.cpp
            lib/MC/MCParser/ELFAsmParser.cpp
            lib/MC/MCParser/MCAsmLexer.cpp
            lib/MC/MCParser/MCAsmParser.cpp
            lib/MC/MCParser/MCAsmParserExtension.cpp
            lib/MC/MCParser/MCTargetAsmParser.cpp

            lib/Object/Archive.cpp
            lib/Object/Binary.cpp
            lib/Object/COFFObjectFile.cpp
            lib/Object/COFFYAML.cpp
            lib/Object/ELF.cpp
            lib/Object/ELFObjectFile.cpp
            lib/Object/ELFYAML.cpp
            lib/Object/Error.cpp
            lib/Object/MachOObjectFile.cpp
            lib/Object/MachOUniversal.cpp
            lib/Object/Object.cpp
            lib/Object/ObjectFile.cpp
            lib/Object/YAML.cpp

            lib/Option/Arg.cpp
            lib/Option/ArgList.cpp
            lib/Option/Option.cpp
            lib/Option/OptTable.cpp

            lib/Support/Allocator.cpp
            lib/Support/APFloat.cpp
            lib/Support/APInt.cpp
            lib/Support/APSInt.cpp
            lib/Support/Atomic.cpp
            lib/Support/BlockFrequency.cpp
            lib/Support/BranchProbability.cpp
            lib/Support/circular_raw_ostream.cpp
            lib/Support/CommandLine.cpp
            lib/Support/Compression.cpp
            lib/Support/ConstantRange.cpp
            lib/Support/ConvertUTF.c
            lib/Support/ConvertUTFWrapper.cpp
            lib/Support/CrashRecoveryContext.cpp
            lib/Support/DAGDeltaAlgorithm.cpp
            lib/Support/DataExtractor.cpp
            lib/Support/DataStream.cpp
            lib/Support/Debug.cpp
            lib/Support/DeltaAlgorithm.cpp
            lib/Support/Disassembler.cpp
            lib/Support/Dwarf.cpp
            lib/Support/DynamicLibrary.cpp
            lib/Support/Errno.cpp
            lib/Support/ErrorHandling.cpp
            lib/Support/FileOutputBuffer.cpp
            lib/Support/FileUtilities.cpp
            lib/Support/FoldingSet.cpp
            lib/Support/FormattedStream.cpp
            lib/Support/GraphWriter.cpp
            lib/Support/Hashing.cpp
            lib/Support/Host.cpp
            lib/Support/IncludeFile.cpp
            lib/Support/IntEqClasses.cpp
            lib/Support/IntervalMap.cpp
            lib/Support/IntrusiveRefCntPtr.cpp
            lib/Support/IsInf.cpp
            lib/Support/IsNAN.cpp
            lib/Support/Locale.cpp
            lib/Support/LockFileManager.cpp
            lib/Support/ManagedStatic.cpp
            lib/Support/MD5.cpp
            lib/Support/Memory.cpp
            lib/Support/MemoryBuffer.cpp
            lib/Support/MemoryObject.cpp
            lib/Support/Mutex.cpp
            lib/Support/Path.cpp
            lib/Support/PluginLoader.cpp
            lib/Support/PrettyStackTrace.cpp
            lib/Support/Process.cpp
            lib/Support/Program.cpp
            lib/Support/raw_os_ostream.cpp
            lib/Support/raw_ostream.cpp
            lib/Support/regcomp.c
            lib/Support/regerror.c
            lib/Support/Regex.cpp
            lib/Support/regexec.c
            lib/Support/regfree.c
            lib/Support/regstrlcpy.c
            lib/Support/RWMutex.cpp
            lib/Support/SearchForAddressOfSpecialSymbol.cpp
            lib/Support/Signals.cpp
            lib/Support/SmallPtrSet.cpp
            lib/Support/SmallVector.cpp
            lib/Support/SourceMgr.cpp
            lib/Support/Statistic.cpp
            lib/Support/StreamableMemoryObject.cpp
            lib/Support/StringExtras.cpp
            lib/Support/StringMap.cpp
            lib/Support/StringPool.cpp
            lib/Support/StringRef.cpp
            lib/Support/StringRefMemoryObject.cpp
            lib/Support/system_error.cpp
            lib/Support/SystemUtils.cpp
            lib/Support/TargetRegistry.cpp
            lib/Support/Threading.cpp
            lib/Support/ThreadLocal.cpp
            lib/Support/Timer.cpp
            lib/Support/TimeValue.cpp
            lib/Support/ToolOutputFile.cpp
            lib/Support/Triple.cpp
            lib/Support/Twine.cpp
            lib/Support/Unicode.cpp
            lib/Support/Valgrind.cpp
            lib/Support/Watchdog.cpp
            lib/Support/YAMLParser.cpp
            lib/Support/YAMLTraits.cpp

            lib/TableGen/Error.cpp
            lib/TableGen/Main.cpp
            lib/TableGen/Record.cpp
            lib/TableGen/StringMatcher.cpp
            lib/TableGen/TableGenBackend.cpp
            lib/TableGen/TGLexer.cpp
            lib/TableGen/TGParser.cpp

            lib/Target/Mangler.cpp
            lib/Target/Target.cpp
            lib/Target/TargetIntrinsicInfo.cpp
            lib/Target/TargetJITInfo.cpp
            lib/Target/TargetLibraryInfo.cpp
            lib/Target/TargetLoweringObjectFile.cpp
            lib/Target/TargetMachine.cpp
            lib/Target/TargetMachineC.cpp
            lib/Target/TargetSubtargetInfo.cpp

            lib/Target/X86/X86AsmPrinter.cpp
            lib/Target/X86/X86CodeEmitter.cpp
            lib/Target/X86/X86COFFMachineModuleInfo.cpp
            lib/Target/X86/X86FastISel.cpp
            lib/Target/X86/X86FixupLEAs.cpp
            lib/Target/X86/X86FloatingPoint.cpp
            lib/Target/X86/X86FrameLowering.cpp
            lib/Target/X86/X86InstrInfo.cpp
            lib/Target/X86/X86ISelDAGToDAG.cpp
            lib/Target/X86/X86ISelLowering.cpp
            lib/Target/X86/X86JITInfo.cpp
            lib/Target/X86/X86MachineFunctionInfo.cpp
            lib/Target/X86/X86MCInstLower.cpp
            lib/Target/X86/X86PadShortFunction.cpp
            lib/Target/X86/X86RegisterInfo.cpp
            lib/Target/X86/X86SelectionDAGInfo.cpp
            lib/Target/X86/X86Subtarget.cpp
            lib/Target/X86/X86TargetMachine.cpp
            lib/Target/X86/X86TargetObjectFile.cpp
            lib/Target/X86/X86TargetTransformInfo.cpp
            lib/Target/X86/X86VZeroUpper.cpp

            lib/Target/X86/AsmParser/X86AsmParser.cpp

            lib/Target/X86/Disassembler/X86Disassembler.cpp
            lib/Target/X86/Disassembler/X86DisassemblerDecoder.c

            lib/Target/X86/InstPrinter/X86ATTInstPrinter.cpp
            lib/Target/X86/InstPrinter/X86InstComments.cpp
            lib/Target/X86/InstPrinter/X86IntelInstPrinter.cpp

            lib/Target/X86/MCTargetDesc/X86AsmBackend.cpp
            lib/Target/X86/MCTargetDesc/X86ELFObjectWriter.cpp
            lib/Target/X86/MCTargetDesc/X86ELFRelocationInfo.cpp
            lib/Target/X86/MCTargetDesc/X86MachObjectWriter.cpp
            lib/Target/X86/MCTargetDesc/X86MachORelocationInfo.cpp
            lib/Target/X86/MCTargetDesc/X86MCAsmInfo.cpp
            lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp
            lib/Target/X86/MCTargetDesc/X86MCTargetDesc.cpp
            lib/Target/X86/MCTargetDesc/X86WinCOFFObjectWriter.cpp

            lib/Target/X86/TargetInfo/X86TargetInfo.cpp

            lib/Target/X86/Utils/X86ShuffleDecode.cpp

            lib/Transforms/Hello/Hello.cpp

            lib/Transforms/InstCombine/InstCombineAddSub.cpp
            lib/Transforms/InstCombine/InstCombineAndOrXor.cpp
            lib/Transforms/InstCombine/InstCombineCalls.cpp
            lib/Transforms/InstCombine/InstCombineCasts.cpp
            lib/Transforms/InstCombine/InstCombineCompares.cpp
            lib/Transforms/InstCombine/InstCombineLoadStoreAlloca.cpp
            lib/Transforms/InstCombine/InstCombineMulDivRem.cpp
            lib/Transforms/InstCombine/InstCombinePHI.cpp
            lib/Transforms/InstCombine/InstCombineSelect.cpp
            lib/Transforms/InstCombine/InstCombineShifts.cpp
            lib/Transforms/InstCombine/InstCombineSimplifyDemanded.cpp
            lib/Transforms/InstCombine/InstCombineVectorOps.cpp
            lib/Transforms/InstCombine/InstructionCombining.cpp

            lib/Transforms/Instrumentation/AddressSanitizer.cpp
            lib/Transforms/Instrumentation/BoundsChecking.cpp
            lib/Transforms/Instrumentation/DataFlowSanitizer.cpp
            lib/Transforms/Instrumentation/DebugIR.cpp
            lib/Transforms/Instrumentation/GCOVProfiling.cpp
            lib/Transforms/Instrumentation/Instrumentation.cpp
            lib/Transforms/Instrumentation/MemorySanitizer.cpp
            lib/Transforms/Instrumentation/ThreadSanitizer.cpp

            lib/Transforms/IPO/ArgumentPromotion.cpp
            lib/Transforms/IPO/BarrierNoopPass.cpp
            lib/Transforms/IPO/ConstantMerge.cpp
            lib/Transforms/IPO/DeadArgumentElimination.cpp
            lib/Transforms/IPO/ExtractGV.cpp
            lib/Transforms/IPO/FunctionAttrs.cpp
            lib/Transforms/IPO/GlobalDCE.cpp
            lib/Transforms/IPO/GlobalOpt.cpp
            lib/Transforms/IPO/InlineAlways.cpp
            lib/Transforms/IPO/Inliner.cpp
            lib/Transforms/IPO/InlineSimple.cpp
            lib/Transforms/IPO/Internalize.cpp
            lib/Transforms/IPO/IPConstantPropagation.cpp
            lib/Transforms/IPO/IPO.cpp
            lib/Transforms/IPO/LoopExtractor.cpp
            lib/Transforms/IPO/MergeFunctions.cpp
            lib/Transforms/IPO/PartialInlining.cpp
            lib/Transforms/IPO/PassManagerBuilder.cpp
            lib/Transforms/IPO/PruneEH.cpp
            lib/Transforms/IPO/StripDeadPrototypes.cpp
            lib/Transforms/IPO/StripSymbols.cpp

            lib/Transforms/ObjCARC/DependencyAnalysis.cpp
            lib/Transforms/ObjCARC/ObjCARC.cpp
            lib/Transforms/ObjCARC/ObjCARCAliasAnalysis.cpp
            lib/Transforms/ObjCARC/ObjCARCAPElim.cpp
            lib/Transforms/ObjCARC/ObjCARCContract.cpp
            lib/Transforms/ObjCARC/ObjCARCExpand.cpp
            lib/Transforms/ObjCARC/ObjCARCOpts.cpp
            lib/Transforms/ObjCARC/ObjCARCUtil.cpp
            lib/Transforms/ObjCARC/ProvenanceAnalysis.cpp

            lib/Transforms/Scalar/ADCE.cpp
            lib/Transforms/Scalar/CodeGenPrepare.cpp
            lib/Transforms/Scalar/ConstantProp.cpp
            lib/Transforms/Scalar/CorrelatedValuePropagation.cpp
            lib/Transforms/Scalar/DCE.cpp
            lib/Transforms/Scalar/DeadStoreElimination.cpp
            lib/Transforms/Scalar/EarlyCSE.cpp
            lib/Transforms/Scalar/FlattenCFGPass.cpp
            lib/Transforms/Scalar/GlobalMerge.cpp
            lib/Transforms/Scalar/GVN.cpp
            lib/Transforms/Scalar/IndVarSimplify.cpp
            lib/Transforms/Scalar/JumpThreading.cpp
            lib/Transforms/Scalar/LICM.cpp
            lib/Transforms/Scalar/LoopDeletion.cpp
            lib/Transforms/Scalar/LoopIdiomRecognize.cpp
            lib/Transforms/Scalar/LoopInstSimplify.cpp
            lib/Transforms/Scalar/LoopRerollPass.cpp
            lib/Transforms/Scalar/LoopRotation.cpp
            lib/Transforms/Scalar/LoopStrengthReduce.cpp
            lib/Transforms/Scalar/LoopUnrollPass.cpp
            lib/Transforms/Scalar/LoopUnswitch.cpp
            lib/Transforms/Scalar/LowerAtomic.cpp
            lib/Transforms/Scalar/MemCpyOptimizer.cpp
            lib/Transforms/Scalar/PartiallyInlineLibCalls.cpp
            lib/Transforms/Scalar/Reassociate.cpp
            lib/Transforms/Scalar/Reg2Mem.cpp
            lib/Transforms/Scalar/SampleProfile.cpp
            lib/Transforms/Scalar/Scalar.cpp
            lib/Transforms/Scalar/ScalarReplAggregates.cpp
            lib/Transforms/Scalar/SCCP.cpp
            lib/Transforms/Scalar/SimplifyCFGPass.cpp
            lib/Transforms/Scalar/Sink.cpp
            lib/Transforms/Scalar/SROA.cpp
            lib/Transforms/Scalar/StructurizeCFG.cpp
            lib/Transforms/Scalar/TailRecursionElimination.cpp

            lib/Transforms/Utils/BasicBlockUtils.cpp
            lib/Transforms/Utils/BreakCriticalEdges.cpp
            lib/Transforms/Utils/BuildLibCalls.cpp
            lib/Transforms/Utils/BypassSlowDivision.cpp
            lib/Transforms/Utils/CloneFunction.cpp
            lib/Transforms/Utils/CloneModule.cpp
            lib/Transforms/Utils/CmpInstAnalysis.cpp
            lib/Transforms/Utils/CodeExtractor.cpp
            lib/Transforms/Utils/DemoteRegToStack.cpp
            lib/Transforms/Utils/FlattenCFG.cpp
            lib/Transforms/Utils/GlobalStatus.cpp
            lib/Transforms/Utils/InlineFunction.cpp
            lib/Transforms/Utils/InstructionNamer.cpp
            lib/Transforms/Utils/IntegerDivision.cpp
            lib/Transforms/Utils/LCSSA.cpp
            lib/Transforms/Utils/Local.cpp
            lib/Transforms/Utils/LoopSimplify.cpp
            lib/Transforms/Utils/LoopUnroll.cpp
            lib/Transforms/Utils/LoopUnrollRuntime.cpp
            lib/Transforms/Utils/LowerExpectIntrinsic.cpp
            lib/Transforms/Utils/LowerInvoke.cpp
            lib/Transforms/Utils/LowerSwitch.cpp
            lib/Transforms/Utils/Mem2Reg.cpp
            lib/Transforms/Utils/MetaRenamer.cpp
            lib/Transforms/Utils/ModuleUtils.cpp
            lib/Transforms/Utils/PromoteMemoryToRegister.cpp
            lib/Transforms/Utils/SimplifyCFG.cpp
            lib/Transforms/Utils/SimplifyIndVar.cpp
            lib/Transforms/Utils/SimplifyInstructions.cpp
            lib/Transforms/Utils/SimplifyLibCalls.cpp
            lib/Transforms/Utils/SpecialCaseList.cpp
            lib/Transforms/Utils/SSAUpdater.cpp
            lib/Transforms/Utils/UnifyFunctionExitNodes.cpp
            lib/Transforms/Utils/Utils.cpp
            lib/Transforms/Utils/ValueMapper.cpp

            lib/Transforms/Vectorize/BBVectorize.cpp
            lib/Transforms/Vectorize/LoopVectorize.cpp
            lib/Transforms/Vectorize/SLPVectorizer.cpp
            lib/Transforms/Vectorize/Vectorize.cpp

            src/llvmplugin.cpp

            tools/clang/lib/Analysis/AnalysisDeclContext.cpp
            tools/clang/lib/Analysis/BodyFarm.cpp
            tools/clang/lib/Analysis/CallGraph.cpp
            tools/clang/lib/Analysis/CFG.cpp
            tools/clang/lib/Analysis/CFGReachabilityAnalysis.cpp
            tools/clang/lib/Analysis/CFGStmtMap.cpp
            tools/clang/lib/Analysis/CocoaConventions.cpp
            tools/clang/lib/Analysis/Consumed.cpp
            tools/clang/lib/Analysis/Dominators.cpp
            tools/clang/lib/Analysis/FormatString.cpp
            tools/clang/lib/Analysis/LiveVariables.cpp
            tools/clang/lib/Analysis/ObjCNoReturn.cpp
            tools/clang/lib/Analysis/PostOrderCFGView.cpp
            tools/clang/lib/Analysis/PrintfFormatString.cpp
            tools/clang/lib/Analysis/ProgramPoint.cpp
            tools/clang/lib/Analysis/PseudoConstantAnalysis.cpp
            tools/clang/lib/Analysis/ReachableCode.cpp
            tools/clang/lib/Analysis/ScanfFormatString.cpp
            tools/clang/lib/Analysis/ThreadSafety.cpp
            tools/clang/lib/Analysis/UninitializedValues.cpp

            tools/clang/lib/ARCMigrate/ARCMT.cpp
            tools/clang/lib/ARCMigrate/ARCMTActions.cpp
            tools/clang/lib/ARCMigrate/FileRemapper.cpp
            tools/clang/lib/ARCMigrate/ObjCMT.cpp
            tools/clang/lib/ARCMigrate/PlistReporter.cpp
            tools/clang/lib/ARCMigrate/TransAPIUses.cpp
            tools/clang/lib/ARCMigrate/TransARCAssign.cpp
            tools/clang/lib/ARCMigrate/TransAutoreleasePool.cpp
            tools/clang/lib/ARCMigrate/TransBlockObjCVariable.cpp
            tools/clang/lib/ARCMigrate/TransEmptyStatementsAndDealloc.cpp
            tools/clang/lib/ARCMigrate/TransformActions.cpp
            tools/clang/lib/ARCMigrate/Transforms.cpp
            tools/clang/lib/ARCMigrate/TransGCAttrs.cpp
            tools/clang/lib/ARCMigrate/TransGCCalls.cpp
            tools/clang/lib/ARCMigrate/TransProperties.cpp
            tools/clang/lib/ARCMigrate/TransProtectedScope.cpp
            tools/clang/lib/ARCMigrate/TransRetainReleaseDealloc.cpp
            tools/clang/lib/ARCMigrate/TransUnbridgedCasts.cpp
            tools/clang/lib/ARCMigrate/TransUnusedInitDelegate.cpp
            tools/clang/lib/ARCMigrate/TransZeroOutPropsInDealloc.cpp

            tools/clang/lib/AST/APValue.cpp
            tools/clang/lib/AST/ASTConsumer.cpp
            tools/clang/lib/AST/ASTContext.cpp
            tools/clang/lib/AST/ASTDiagnostic.cpp
            tools/clang/lib/AST/ASTDumper.cpp
            tools/clang/lib/AST/ASTImporter.cpp
            tools/clang/lib/AST/ASTTypeTraits.cpp
            tools/clang/lib/AST/AttrImpl.cpp
            tools/clang/lib/AST/Comment.cpp
            tools/clang/lib/AST/CommentBriefParser.cpp
            tools/clang/lib/AST/CommentCommandTraits.cpp
            tools/clang/lib/AST/CommentLexer.cpp
            tools/clang/lib/AST/CommentParser.cpp
            tools/clang/lib/AST/CommentSema.cpp
            tools/clang/lib/AST/CXXInheritance.cpp
            tools/clang/lib/AST/Decl.cpp
            tools/clang/lib/AST/DeclarationName.cpp
            tools/clang/lib/AST/DeclBase.cpp
            tools/clang/lib/AST/DeclCXX.cpp
            tools/clang/lib/AST/DeclFriend.cpp
            tools/clang/lib/AST/DeclGroup.cpp
            tools/clang/lib/AST/DeclObjC.cpp
            tools/clang/lib/AST/DeclOpenMP.cpp
            tools/clang/lib/AST/DeclPrinter.cpp
            tools/clang/lib/AST/DeclTemplate.cpp
            tools/clang/lib/AST/Expr.cpp
            tools/clang/lib/AST/ExprClassification.cpp
            tools/clang/lib/AST/ExprConstant.cpp
            tools/clang/lib/AST/ExprCXX.cpp
            tools/clang/lib/AST/ExternalASTSource.cpp
            tools/clang/lib/AST/InheritViz.cpp
            tools/clang/lib/AST/ItaniumCXXABI.cpp
            tools/clang/lib/AST/ItaniumMangle.cpp
            tools/clang/lib/AST/Mangle.cpp
            tools/clang/lib/AST/MangleNumberingContext.cpp
            tools/clang/lib/AST/MicrosoftCXXABI.cpp
            tools/clang/lib/AST/MicrosoftMangle.cpp
            tools/clang/lib/AST/NestedNameSpecifier.cpp
            tools/clang/lib/AST/NSAPI.cpp
            tools/clang/lib/AST/ParentMap.cpp
            tools/clang/lib/AST/RawCommentList.cpp
            tools/clang/lib/AST/RecordLayout.cpp
            tools/clang/lib/AST/RecordLayoutBuilder.cpp
            tools/clang/lib/AST/SelectorLocationsKind.cpp
            tools/clang/lib/AST/Stmt.cpp
            tools/clang/lib/AST/StmtIterator.cpp
            tools/clang/lib/AST/StmtPrinter.cpp
            tools/clang/lib/AST/StmtProfile.cpp
            tools/clang/lib/AST/StmtViz.cpp
            tools/clang/lib/AST/TemplateBase.cpp
            tools/clang/lib/AST/TemplateName.cpp
            tools/clang/lib/AST/Type.cpp
            tools/clang/lib/AST/TypeLoc.cpp
            tools/clang/lib/AST/TypePrinter.cpp
            tools/clang/lib/AST/VTableBuilder.cpp
            tools/clang/lib/AST/VTTBuilder.cpp

            tools/clang/lib/ASTMatchers/ASTMatchersInternal.cpp
            tools/clang/lib/ASTMatchers/ASTMatchFinder.cpp
            tools/clang/lib/ASTMatchers/Dynamic/Diagnostics.cpp
            tools/clang/lib/ASTMatchers/Dynamic/Parser.cpp
            tools/clang/lib/ASTMatchers/Dynamic/Registry.cpp
            tools/clang/lib/ASTMatchers/Dynamic/VariantValue.cpp
            tools/clang/lib/Basic/Builtins.cpp
            tools/clang/lib/Basic/CharInfo.cpp
            tools/clang/lib/Basic/Diagnostic.cpp
            tools/clang/lib/Basic/DiagnosticIDs.cpp
            tools/clang/lib/Basic/FileManager.cpp
            tools/clang/lib/Basic/FileSystemStatCache.cpp
            tools/clang/lib/Basic/IdentifierTable.cpp
            tools/clang/lib/Basic/LangOptions.cpp
            tools/clang/lib/Basic/Module.cpp
            tools/clang/lib/Basic/ObjCRuntime.cpp
            tools/clang/lib/Basic/OpenMPKinds.cpp
            tools/clang/lib/Basic/OperatorPrecedence.cpp
            tools/clang/lib/Basic/SourceLocation.cpp
            tools/clang/lib/Basic/SourceManager.cpp
            tools/clang/lib/Basic/TargetInfo.cpp
            tools/clang/lib/Basic/Targets.cpp
            tools/clang/lib/Basic/TokenKinds.cpp
            tools/clang/lib/Basic/Version.cpp
            tools/clang/lib/Basic/VersionTuple.cpp

            tools/clang/lib/CodeGen/BackendUtil.cpp
            tools/clang/lib/CodeGen/CGAtomic.cpp
            tools/clang/lib/CodeGen/CGBlocks.cpp
            tools/clang/lib/CodeGen/CGBuiltin.cpp
            tools/clang/lib/CodeGen/CGCall.cpp
            tools/clang/lib/CodeGen/CGClass.cpp
            tools/clang/lib/CodeGen/CGCleanup.cpp
            tools/clang/lib/CodeGen/CGCUDANV.cpp
            tools/clang/lib/CodeGen/CGCUDARuntime.cpp
            tools/clang/lib/CodeGen/CGCXX.cpp
            tools/clang/lib/CodeGen/CGCXXABI.cpp
            tools/clang/lib/CodeGen/CGDebugInfo.cpp
            tools/clang/lib/CodeGen/CGDecl.cpp
            tools/clang/lib/CodeGen/CGDeclCXX.cpp
            tools/clang/lib/CodeGen/CGException.cpp
            tools/clang/lib/CodeGen/CGExpr.cpp
            tools/clang/lib/CodeGen/CGExprAgg.cpp
            tools/clang/lib/CodeGen/CGExprComplex.cpp
            tools/clang/lib/CodeGen/CGExprConstant.cpp
            tools/clang/lib/CodeGen/CGExprCXX.cpp
            tools/clang/lib/CodeGen/CGExprScalar.cpp
            tools/clang/lib/CodeGen/CGObjC.cpp
            tools/clang/lib/CodeGen/CGObjCGNU.cpp
            tools/clang/lib/CodeGen/CGObjCMac.cpp
            tools/clang/lib/CodeGen/CGObjCRuntime.cpp
            tools/clang/lib/CodeGen/CGOpenCLRuntime.cpp
            tools/clang/lib/CodeGen/CGRecordLayoutBuilder.cpp
            tools/clang/lib/CodeGen/CGRTTI.cpp
            tools/clang/lib/CodeGen/CGStmt.cpp
            tools/clang/lib/CodeGen/CGVTables.cpp
            tools/clang/lib/CodeGen/CGVTT.cpp
            tools/clang/lib/CodeGen/CodeGenABITypes.cpp
            tools/clang/lib/CodeGen/CodeGenAction.cpp
            tools/clang/lib/CodeGen/CodeGenFunction.cpp
            tools/clang/lib/CodeGen/CodeGenModule.cpp
            tools/clang/lib/CodeGen/CodeGenTBAA.cpp
            tools/clang/lib/CodeGen/CodeGenTypes.cpp
            tools/clang/lib/CodeGen/ItaniumCXXABI.cpp
            tools/clang/lib/CodeGen/MicrosoftCXXABI.cpp
            tools/clang/lib/CodeGen/MicrosoftVBTables.cpp
            tools/clang/lib/CodeGen/ModuleBuilder.cpp
            tools/clang/lib/CodeGen/TargetInfo.cpp

            tools/clang/lib/Driver/Action.cpp
            tools/clang/lib/Driver/CC1AsOptions.cpp
            tools/clang/lib/Driver/Compilation.cpp
            tools/clang/lib/Driver/Driver.cpp
            tools/clang/lib/Driver/DriverOptions.cpp
            tools/clang/lib/Driver/Job.cpp
            tools/clang/lib/Driver/Phases.cpp
            tools/clang/lib/Driver/SanitizerArgs.cpp
            tools/clang/lib/Driver/Tool.cpp
            tools/clang/lib/Driver/ToolChain.cpp
            tools/clang/lib/Driver/ToolChains.cpp
            tools/clang/lib/Driver/Tools.cpp
            tools/clang/lib/Driver/Types.cpp
            tools/clang/lib/Driver/WindowsToolChain.cpp

            tools/clang/lib/Edit/Commit.cpp
            tools/clang/lib/Edit/EditedSource.cpp
            tools/clang/lib/Edit/RewriteObjCFoundationAPI.cpp

            tools/clang/lib/Format/BreakableToken.cpp
            tools/clang/lib/Format/ContinuationIndenter.cpp
            tools/clang/lib/Format/Format.cpp
            tools/clang/lib/Format/FormatToken.cpp
            tools/clang/lib/Format/TokenAnnotator.cpp
            tools/clang/lib/Format/UnwrappedLineParser.cpp
            tools/clang/lib/Format/WhitespaceManager.cpp

            tools/clang/lib/Frontend/ASTConsumers.cpp
            tools/clang/lib/Frontend/ASTMerge.cpp
            tools/clang/lib/Frontend/ASTUnit.cpp
            tools/clang/lib/Frontend/CacheTokens.cpp
            tools/clang/lib/Frontend/ChainedDiagnosticConsumer.cpp
            tools/clang/lib/Frontend/ChainedIncludesSource.cpp
            tools/clang/lib/Frontend/CompilerInstance.cpp
            tools/clang/lib/Frontend/CompilerInvocation.cpp
            tools/clang/lib/Frontend/CreateInvocationFromCommandLine.cpp
            tools/clang/lib/Frontend/DependencyFile.cpp
            tools/clang/lib/Frontend/DependencyGraph.cpp
            tools/clang/lib/Frontend/DiagnosticRenderer.cpp
            tools/clang/lib/Frontend/FrontendAction.cpp
            tools/clang/lib/Frontend/FrontendActions.cpp
            tools/clang/lib/Frontend/FrontendOptions.cpp
            tools/clang/lib/Frontend/HeaderIncludeGen.cpp
            tools/clang/lib/Frontend/InitHeaderSearch.cpp
            tools/clang/lib/Frontend/InitPreprocessor.cpp
            tools/clang/lib/Frontend/LangStandards.cpp
            tools/clang/lib/Frontend/LayoutOverrideSource.cpp
            tools/clang/lib/Frontend/LogDiagnosticPrinter.cpp
            tools/clang/lib/Frontend/MultiplexConsumer.cpp
            tools/clang/lib/Frontend/PrintPreprocessedOutput.cpp
            tools/clang/lib/Frontend/SerializedDiagnosticPrinter.cpp
            tools/clang/lib/Frontend/TextDiagnostic.cpp
            tools/clang/lib/Frontend/TextDiagnosticBuffer.cpp
            tools/clang/lib/Frontend/TextDiagnosticPrinter.cpp
            tools/clang/lib/Frontend/VerifyDiagnosticConsumer.cpp
            tools/clang/lib/Frontend/Warnings.cpp

            tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp
            tools/clang/lib/Index/CommentToXML.cpp
            tools/clang/lib/Index/USRGeneration.cpp
            tools/clang/lib/Lex/HeaderMap.cpp
            tools/clang/lib/Lex/HeaderSearch.cpp
            tools/clang/lib/Lex/Lexer.cpp
            tools/clang/lib/Lex/LiteralSupport.cpp
            tools/clang/lib/Lex/MacroArgs.cpp
            tools/clang/lib/Lex/MacroInfo.cpp
            tools/clang/lib/Lex/ModuleMap.cpp
            tools/clang/lib/Lex/PPCaching.cpp
            tools/clang/lib/Lex/PPCallbacks.cpp
            tools/clang/lib/Lex/PPConditionalDirectiveRecord.cpp
            tools/clang/lib/Lex/PPDirectives.cpp
            tools/clang/lib/Lex/PPExpressions.cpp
            tools/clang/lib/Lex/PPLexerChange.cpp
            tools/clang/lib/Lex/PPMacroExpansion.cpp
            tools/clang/lib/Lex/Pragma.cpp
            tools/clang/lib/Lex/PreprocessingRecord.cpp
            tools/clang/lib/Lex/Preprocessor.cpp
            tools/clang/lib/Lex/PreprocessorLexer.cpp
            tools/clang/lib/Lex/PTHLexer.cpp
            tools/clang/lib/Lex/ScratchBuffer.cpp
            tools/clang/lib/Lex/TokenConcatenation.cpp
            tools/clang/lib/Lex/TokenLexer.cpp

            tools/clang/lib/Parse/ParseAST.cpp
            tools/clang/lib/Parse/ParseCXXInlineMethods.cpp
            tools/clang/lib/Parse/ParseDecl.cpp
            tools/clang/lib/Parse/ParseDeclCXX.cpp
            tools/clang/lib/Parse/ParseExpr.cpp
            tools/clang/lib/Parse/ParseExprCXX.cpp
            tools/clang/lib/Parse/ParseInit.cpp
            tools/clang/lib/Parse/ParseObjc.cpp
            tools/clang/lib/Parse/ParseOpenMP.cpp
            tools/clang/lib/Parse/ParsePragma.cpp
            tools/clang/lib/Parse/Parser.cpp
            tools/clang/lib/Parse/ParseStmt.cpp
            tools/clang/lib/Parse/ParseTemplate.cpp
            tools/clang/lib/Parse/ParseTentative.cpp

            tools/clang/lib/Rewrite/Core/DeltaTree.cpp
            tools/clang/lib/Rewrite/Core/HTMLRewrite.cpp
            tools/clang/lib/Rewrite/Core/Rewriter.cpp
            tools/clang/lib/Rewrite/Core/RewriteRope.cpp
            tools/clang/lib/Rewrite/Core/TokenRewriter.cpp

            tools/clang/lib/Rewrite/Frontend/FixItRewriter.cpp
            tools/clang/lib/Rewrite/Frontend/FrontendActions.cpp
            tools/clang/lib/Rewrite/Frontend/HTMLPrint.cpp
            tools/clang/lib/Rewrite/Frontend/InclusionRewriter.cpp
            tools/clang/lib/Rewrite/Frontend/RewriteMacros.cpp
            tools/clang/lib/Rewrite/Frontend/RewriteModernObjC.cpp
            tools/clang/lib/Rewrite/Frontend/RewriteObjC.cpp
            tools/clang/lib/Rewrite/Frontend/RewriteTest.cpp

            tools/clang/lib/Sema/AnalysisBasedWarnings.cpp
            tools/clang/lib/Sema/AttributeList.cpp
            tools/clang/lib/Sema/CodeCompleteConsumer.cpp
            tools/clang/lib/Sema/DeclSpec.cpp
            tools/clang/lib/Sema/DelayedDiagnostic.cpp
            tools/clang/lib/Sema/IdentifierResolver.cpp
            tools/clang/lib/Sema/JumpDiagnostics.cpp
            tools/clang/lib/Sema/MultiplexExternalSemaSource.cpp
            tools/clang/lib/Sema/Scope.cpp
            tools/clang/lib/Sema/ScopeInfo.cpp
            tools/clang/lib/Sema/Sema.cpp
            tools/clang/lib/Sema/SemaAccess.cpp
            tools/clang/lib/Sema/SemaAttr.cpp
            tools/clang/lib/Sema/SemaCast.cpp
            tools/clang/lib/Sema/SemaChecking.cpp
            tools/clang/lib/Sema/SemaCodeComplete.cpp
            tools/clang/lib/Sema/SemaConsumer.cpp
            tools/clang/lib/Sema/SemaCXXScopeSpec.cpp
            tools/clang/lib/Sema/SemaDecl.cpp
            tools/clang/lib/Sema/SemaDeclAttr.cpp
            tools/clang/lib/Sema/SemaDeclCXX.cpp
            tools/clang/lib/Sema/SemaDeclObjC.cpp
            tools/clang/lib/Sema/SemaExceptionSpec.cpp
            tools/clang/lib/Sema/SemaExpr.cpp
            tools/clang/lib/Sema/SemaExprCXX.cpp
            tools/clang/lib/Sema/SemaExprMember.cpp
            tools/clang/lib/Sema/SemaExprObjC.cpp
            tools/clang/lib/Sema/SemaFixItUtils.cpp
            tools/clang/lib/Sema/SemaInit.cpp
            tools/clang/lib/Sema/SemaLambda.cpp
            tools/clang/lib/Sema/SemaLookup.cpp
            tools/clang/lib/Sema/SemaObjCProperty.cpp
            tools/clang/lib/Sema/SemaOpenMP.cpp
            tools/clang/lib/Sema/SemaOverload.cpp
            tools/clang/lib/Sema/SemaPseudoObject.cpp
            tools/clang/lib/Sema/SemaStmt.cpp
            tools/clang/lib/Sema/SemaStmtAsm.cpp
            tools/clang/lib/Sema/SemaStmtAttr.cpp
            tools/clang/lib/Sema/SemaTemplate.cpp
            tools/clang/lib/Sema/SemaTemplateDeduction.cpp
            tools/clang/lib/Sema/SemaTemplateInstantiate.cpp
            tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp
            tools/clang/lib/Sema/SemaTemplateVariadic.cpp
            tools/clang/lib/Sema/SemaType.cpp
            tools/clang/lib/Sema/TargetAttributesSema.cpp
            tools/clang/lib/Sema/TypeLocBuilder.cpp
            tools/clang/lib/Serialization/ASTCommon.cpp
            tools/clang/lib/Serialization/ASTReader.cpp
            tools/clang/lib/Serialization/ASTReaderDecl.cpp
            tools/clang/lib/Serialization/ASTReaderStmt.cpp
            tools/clang/lib/Serialization/ASTWriter.cpp
            tools/clang/lib/Serialization/ASTWriterDecl.cpp
            tools/clang/lib/Serialization/ASTWriterStmt.cpp
            tools/clang/lib/Serialization/GeneratePCH.cpp
            tools/clang/lib/Serialization/GlobalModuleIndex.cpp
            tools/clang/lib/Serialization/Module.cpp
            tools/clang/lib/Serialization/ModuleManager.cpp

            tools/clang/lib/StaticAnalyzer/Checkers/AllocationDiagnostics.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/AnalyzerStatsChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ArrayBoundChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ArrayBoundCheckerV2.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/BasicObjCFoundationChecks.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/BoolAssignmentChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/BuiltinFunctionChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CallAndMessageChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CastSizeChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CastToStructChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CheckerDocumentation.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CheckObjCDealloc.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CheckObjCInstMethSignature.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CheckSecuritySyntaxOnly.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CheckSizeofPointer.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ChrootChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ClangCheckers.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CStringChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/CStringSyntaxChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DeadStoresChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DebugCheckers.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DereferenceChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DirectIvarAssignment.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DivZeroChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/DynamicTypePropagation.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ExprInspectionChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/FixedAddressChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/GenericTaintChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/IdempotentOperationChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/IdenticalExprChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/IvarInvalidationChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/LLVMConventionsChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/MacOSKeychainAPIChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/MacOSXAPIChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/MallocChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/MallocOverflowSecurityChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/MallocSizeofChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/NonNullParamChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/NoReturnFunctionChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/NSAutoreleasePoolChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/NSErrorChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCAtSyncChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCContainersASTChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCContainersChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCMissingSuperCallChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCSelfInitChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ObjCUnusedIVarsChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/PointerArithChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/PointerSubChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/PthreadLockChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/RetainCountChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ReturnPointerRangeChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/ReturnUndefChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/SimpleStreamChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/StackAddrEscapeChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/StreamChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/TaintTesterChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/TraversalChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UndefBranchChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UndefCapturedBlockVarChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UndefinedArraySubscriptChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UndefinedAssignmentChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UndefResultChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UnixAPIChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/UnreachableCodeChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/VirtualCallChecker.cpp
            tools/clang/lib/StaticAnalyzer/Checkers/VLASizeChecker.cpp

            tools/clang/lib/StaticAnalyzer/Core/AnalysisManager.cpp
            tools/clang/lib/StaticAnalyzer/Core/AnalyzerOptions.cpp
            tools/clang/lib/StaticAnalyzer/Core/APSIntType.cpp
            tools/clang/lib/StaticAnalyzer/Core/BasicValueFactory.cpp
            tools/clang/lib/StaticAnalyzer/Core/BlockCounter.cpp
            tools/clang/lib/StaticAnalyzer/Core/BugReporter.cpp
            tools/clang/lib/StaticAnalyzer/Core/BugReporterVisitors.cpp
            tools/clang/lib/StaticAnalyzer/Core/CallEvent.cpp
            tools/clang/lib/StaticAnalyzer/Core/Checker.cpp
            tools/clang/lib/StaticAnalyzer/Core/CheckerContext.cpp
            tools/clang/lib/StaticAnalyzer/Core/CheckerHelpers.cpp
            tools/clang/lib/StaticAnalyzer/Core/CheckerManager.cpp
            tools/clang/lib/StaticAnalyzer/Core/CheckerRegistry.cpp
            tools/clang/lib/StaticAnalyzer/Core/CommonBugCategories.cpp
            tools/clang/lib/StaticAnalyzer/Core/ConstraintManager.cpp
            tools/clang/lib/StaticAnalyzer/Core/CoreEngine.cpp
            tools/clang/lib/StaticAnalyzer/Core/Environment.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExplodedGraph.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExprEngine.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExprEngineC.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExprEngineCallAndReturn.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExprEngineCXX.cpp
            tools/clang/lib/StaticAnalyzer/Core/ExprEngineObjC.cpp
            tools/clang/lib/StaticAnalyzer/Core/FunctionSummary.cpp
            tools/clang/lib/StaticAnalyzer/Core/HTMLDiagnostics.cpp
            tools/clang/lib/StaticAnalyzer/Core/MemRegion.cpp
            tools/clang/lib/StaticAnalyzer/Core/PathDiagnostic.cpp
            tools/clang/lib/StaticAnalyzer/Core/PlistDiagnostics.cpp
            tools/clang/lib/StaticAnalyzer/Core/ProgramState.cpp
            tools/clang/lib/StaticAnalyzer/Core/RangeConstraintManager.cpp
            tools/clang/lib/StaticAnalyzer/Core/RegionStore.cpp
            tools/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cpp
            tools/clang/lib/StaticAnalyzer/Core/SimpleSValBuilder.cpp
            tools/clang/lib/StaticAnalyzer/Core/Store.cpp
            tools/clang/lib/StaticAnalyzer/Core/SubEngine.cpp
            tools/clang/lib/StaticAnalyzer/Core/SValBuilder.cpp
            tools/clang/lib/StaticAnalyzer/Core/SVals.cpp
            tools/clang/lib/StaticAnalyzer/Core/SymbolManager.cpp

            tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp
            tools/clang/lib/StaticAnalyzer/Frontend/CheckerRegistration.cpp
            tools/clang/lib/StaticAnalyzer/Frontend/FrontendActions.cpp

            tools/clang/lib/Tooling/ArgumentsAdjusters.cpp
            tools/clang/lib/Tooling/CommonOptionsParser.cpp
            tools/clang/lib/Tooling/CompilationDatabase.cpp
            tools/clang/lib/Tooling/FileMatchTrie.cpp
            tools/clang/lib/Tooling/JSONCompilationDatabase.cpp
            tools/clang/lib/Tooling/Refactoring.cpp
            tools/clang/lib/Tooling/RefactoringCallbacks.cpp
            tools/clang/lib/Tooling/Tooling.cpp
        HEADERS_MOC
            src/llvmplugin.h
        INCLUDE_DIRS
            include
            lib/Target/X86
            src
            tools/clang/include
        DEFINITIONS
            ${LLVM_DEFINITIONS}
        QT_MODULES
            Core
        QT_LIBRARIES
            QtCore
    )
ENDIF()
