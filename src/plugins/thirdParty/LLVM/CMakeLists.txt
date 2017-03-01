PROJECT(LLVMPlugin)

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    include/llvm/Config/PLATFORM_DIR/config.h
    include/llvm/Config/PLATFORM_DIR/llvm-config.h

    include/llvm/Support/PLATFORM_DIR/DataTypes.h

    tools/clang/include/clang/Config/PLATFORM_DIR/config.h
)

# Retrieve LLVM's definitions
# Note: check the calls to the add_llvm_definitions() macro in the original
#       [LLVM]/cmake/modules/HandleLLVMOptions.cmake file...

SET(DEFINITIONS
    __STDC_CONSTANT_MACROS
    __STDC_FORMAT_MACROS
    __STDC_LIMIT_MACROS
)

IF(WIN32)
    LIST(APPEND DEFINITIONS
        _CRT_SECURE_NO_DEPRECATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_DEPRECATE
        _CRT_NONSTDC_NO_WARNINGS
        _SCL_SECURE_NO_DEPRECATE
        _SCL_SECURE_NO_WARNINGS
    )
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LLVM_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll 4fe7c7baf63b8f5624dfee17587f052618c627c3)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib b59a63f0750c92aa084cc78fccc6ef46c719b5b2)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.dll df48883b54093cbe0698c7e0fe5a9d87548f1ea1)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LLVM.lib 421d607dca8bf251d7a808af2254a3b91cecc3a7)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLLVM.dylib eff380da305b0912e44b915e4ae3b93d8b561e52)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLLVM.so f4f0113d27f28af3eb42e73be31c79fa1f36b3a2)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(LLVM_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}LLVM${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(LLVM_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}LLVM${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(LLVM
        INCLUDE_DIRS
            include
            lib/Target/X86
            src
            tools/clang/include
    )
ELSE()
    # Retrieve some include files that were generated on each of our target
    # platforms

    SET(CONFIG_FILES
        lib/Target/X86/PLATFORM_DIR/X86GenAsmMatcher.inc
    )

    FOREACH(CONFIG_FILE ${CONFIG_FILES})
        STRING(REPLACE "PLATFORM_DIR/" "${PLATFORM_DIR}/"
               CONFIG_FILE_ORIG "${CONFIG_FILE}")
        STRING(REPLACE "PLATFORM_DIR/" ""
               CONFIG_FILE_DEST "${CONFIG_FILE}")

        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_ORIG}
                                                         ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_DEST})
    ENDFOREACH()

    # Ignore some warnings on Windows, as well as allow for big objects

    IF(WIN32)
        SET(COMPILATION_FLAGS "/bigobj /wd4146 /wd4180 /wd4244 /wd4258 /wd4267 /wd4291 /wd4345 /wd4351 /wd4355 /wd4456 /wd4457 /wd4458 /wd4459 /wd4503 /wd4624 /wd4722 /wd4800 /wd4805 /w14062 /we4238")

        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILATION_FLAGS}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILATION_FLAGS}")
    ENDIF()

    # On Windows, link to Version.dll

    IF(WIN32)
        SET(SYSTEM_BINARIES
            Version
        )
    ELSE()
        SET(SYSTEM_BINARIES)
    ENDIF()

    # Add the plugin

    SET(LLVM_PLUGIN LLVM PARENT_SCOPE)

    ADD_PLUGIN(LLVM
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            lib/Analysis/AliasAnalysis.cpp
            lib/Analysis/AliasAnalysisEvaluator.cpp
            lib/Analysis/AliasAnalysisSummary.cpp
            lib/Analysis/AliasSetTracker.cpp
            lib/Analysis/Analysis.cpp
            lib/Analysis/AssumptionCache.cpp
            lib/Analysis/BasicAliasAnalysis.cpp
            lib/Analysis/BlockFrequencyInfo.cpp
            lib/Analysis/BlockFrequencyInfoImpl.cpp
            lib/Analysis/BranchProbabilityInfo.cpp
            lib/Analysis/CallGraph.cpp
            lib/Analysis/CallGraphSCCPass.cpp
            lib/Analysis/CallPrinter.cpp
            lib/Analysis/CaptureTracking.cpp
            lib/Analysis/CFG.cpp
            lib/Analysis/CFGPrinter.cpp
            lib/Analysis/CFLAndersAliasAnalysis.cpp
            lib/Analysis/CFLSteensAliasAnalysis.cpp
            lib/Analysis/CGSCCPassManager.cpp
            lib/Analysis/CodeMetrics.cpp
            lib/Analysis/ConstantFolding.cpp
            lib/Analysis/CostModel.cpp
            lib/Analysis/Delinearization.cpp
            lib/Analysis/DemandedBits.cpp
            lib/Analysis/DependenceAnalysis.cpp
            lib/Analysis/DivergenceAnalysis.cpp
            lib/Analysis/DominanceFrontier.cpp
            lib/Analysis/DomPrinter.cpp
            lib/Analysis/EHPersonalities.cpp
            lib/Analysis/GlobalsModRef.cpp
            lib/Analysis/IndirectCallPromotionAnalysis.cpp
            lib/Analysis/InlineCost.cpp
            lib/Analysis/InstCount.cpp
            lib/Analysis/InstructionSimplify.cpp
            lib/Analysis/Interval.cpp
            lib/Analysis/IntervalPartition.cpp
            lib/Analysis/IteratedDominanceFrontier.cpp
            lib/Analysis/IVUsers.cpp
            lib/Analysis/LazyBlockFrequencyInfo.cpp
            lib/Analysis/LazyCallGraph.cpp
            lib/Analysis/LazyValueInfo.cpp
            lib/Analysis/Lint.cpp
            lib/Analysis/Loads.cpp
            lib/Analysis/LoopAccessAnalysis.cpp
            lib/Analysis/LoopInfo.cpp
            lib/Analysis/LoopPass.cpp
            lib/Analysis/LoopPassManager.cpp
            lib/Analysis/LoopUnrollAnalyzer.cpp
            lib/Analysis/MemDepPrinter.cpp
            lib/Analysis/MemDerefPrinter.cpp
            lib/Analysis/MemoryBuiltins.cpp
            lib/Analysis/MemoryDependenceAnalysis.cpp
            lib/Analysis/MemoryLocation.cpp
            lib/Analysis/ModuleDebugInfoPrinter.cpp
            lib/Analysis/ModuleSummaryAnalysis.cpp
            lib/Analysis/ObjCARCAliasAnalysis.cpp
            lib/Analysis/ObjCARCAnalysisUtils.cpp
            lib/Analysis/ObjCARCInstKind.cpp
            lib/Analysis/OptimizationDiagnosticInfo.cpp
            lib/Analysis/OrderedBasicBlock.cpp
            lib/Analysis/PHITransAddr.cpp
            lib/Analysis/PostDominators.cpp
            lib/Analysis/ProfileSummaryInfo.cpp
            lib/Analysis/PtrUseVisitor.cpp
            lib/Analysis/RegionInfo.cpp
            lib/Analysis/RegionPass.cpp
            lib/Analysis/RegionPrinter.cpp
            lib/Analysis/ScalarEvolution.cpp
            lib/Analysis/ScalarEvolutionAliasAnalysis.cpp
            lib/Analysis/ScalarEvolutionExpander.cpp
            lib/Analysis/ScalarEvolutionNormalization.cpp
            lib/Analysis/ScopedNoAliasAA.cpp
            lib/Analysis/SparsePropagation.cpp
            lib/Analysis/TargetLibraryInfo.cpp
            lib/Analysis/TargetTransformInfo.cpp
            lib/Analysis/Trace.cpp
            lib/Analysis/TypeBasedAliasAnalysis.cpp
            lib/Analysis/TypeMetadataUtils.cpp
            lib/Analysis/ValueTracking.cpp
            lib/Analysis/VectorUtils.cpp

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
            lib/CodeGen/AtomicExpandPass.cpp
            lib/CodeGen/BasicTargetTransformInfo.cpp
            lib/CodeGen/BranchFolding.cpp
            lib/CodeGen/BuiltinGCs.cpp
            lib/CodeGen/CalcSpillWeights.cpp
            lib/CodeGen/CallingConvLower.cpp
            lib/CodeGen/CodeGen.cpp
            lib/CodeGen/CodeGenPrepare.cpp
            lib/CodeGen/CriticalAntiDepBreaker.cpp
            lib/CodeGen/DeadMachineInstructionElim.cpp
            lib/CodeGen/DetectDeadLanes.cpp
            lib/CodeGen/DFAPacketizer.cpp
            lib/CodeGen/DwarfEHPrepare.cpp
            lib/CodeGen/EarlyIfConversion.cpp
            lib/CodeGen/EdgeBundles.cpp
            lib/CodeGen/ExecutionDepsFix.cpp
            lib/CodeGen/ExpandISelPseudos.cpp
            lib/CodeGen/ExpandPostRAPseudos.cpp
            lib/CodeGen/FaultMaps.cpp
            lib/CodeGen/FuncletLayout.cpp
            lib/CodeGen/GCMetadata.cpp
            lib/CodeGen/GCMetadataPrinter.cpp
            lib/CodeGen/GCRootLowering.cpp
            lib/CodeGen/GCStrategy.cpp
            lib/CodeGen/GlobalMerge.cpp
            lib/CodeGen/IfConversion.cpp
            lib/CodeGen/ImplicitNullChecks.cpp
            lib/CodeGen/InlineSpiller.cpp
            lib/CodeGen/InterferenceCache.cpp
            lib/CodeGen/InterleavedAccessPass.cpp
            lib/CodeGen/IntrinsicLowering.cpp
            lib/CodeGen/LatencyPriorityQueue.cpp
            lib/CodeGen/LexicalScopes.cpp
            lib/CodeGen/LiveDebugValues.cpp
            lib/CodeGen/LiveDebugVariables.cpp
            lib/CodeGen/LiveInterval.cpp
            lib/CodeGen/LiveIntervalAnalysis.cpp
            lib/CodeGen/LiveIntervalUnion.cpp
            lib/CodeGen/LivePhysRegs.cpp
            lib/CodeGen/LiveRangeCalc.cpp
            lib/CodeGen/LiveRangeEdit.cpp
            lib/CodeGen/LiveRegMatrix.cpp
            lib/CodeGen/LiveStackAnalysis.cpp
            lib/CodeGen/LiveVariables.cpp
            lib/CodeGen/LLVMTargetMachine.cpp
            lib/CodeGen/LocalStackSlotAllocation.cpp
            lib/CodeGen/LowerEmuTLS.cpp
            lib/CodeGen/MachineBasicBlock.cpp
            lib/CodeGen/MachineBlockFrequencyInfo.cpp
            lib/CodeGen/MachineBlockPlacement.cpp
            lib/CodeGen/MachineBranchProbabilityInfo.cpp
            lib/CodeGen/MachineCombiner.cpp
            lib/CodeGen/MachineCopyPropagation.cpp
            lib/CodeGen/MachineCSE.cpp
            lib/CodeGen/MachineDominanceFrontier.cpp
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
            lib/CodeGen/MachineRegionInfo.cpp
            lib/CodeGen/MachineRegisterInfo.cpp
            lib/CodeGen/MachineScheduler.cpp
            lib/CodeGen/MachineSink.cpp
            lib/CodeGen/MachineSSAUpdater.cpp
            lib/CodeGen/MachineTraceMetrics.cpp
            lib/CodeGen/MachineVerifier.cpp
            lib/CodeGen/MIRPrinter.cpp
            lib/CodeGen/MIRPrintingPass.cpp
            lib/CodeGen/OptimizePHIs.cpp
            lib/CodeGen/ParallelCG.cpp
            lib/CodeGen/PatchableFunction.cpp
            lib/CodeGen/PeepholeOptimizer.cpp
            lib/CodeGen/PHIElimination.cpp
            lib/CodeGen/PHIEliminationUtils.cpp
            lib/CodeGen/PostRAHazardRecognizer.cpp
            lib/CodeGen/PostRASchedulerList.cpp
            lib/CodeGen/PreISelIntrinsicLowering.cpp
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
            lib/CodeGen/RegisterUsageInfo.cpp
            lib/CodeGen/RegUsageInfoCollector.cpp
            lib/CodeGen/RegUsageInfoPropagate.cpp
            lib/CodeGen/RenameIndependentSubregs.cpp
            lib/CodeGen/SafeStack.cpp
            lib/CodeGen/SafeStackColoring.cpp
            lib/CodeGen/SafeStackLayout.cpp
            lib/CodeGen/ScheduleDAG.cpp
            lib/CodeGen/ScheduleDAGInstrs.cpp
            lib/CodeGen/ScheduleDAGPrinter.cpp
            lib/CodeGen/ScoreboardHazardRecognizer.cpp
            lib/CodeGen/ShadowStackGCLowering.cpp
            lib/CodeGen/ShrinkWrap.cpp
            lib/CodeGen/SjLjEHPrepare.cpp
            lib/CodeGen/SlotIndexes.cpp
            lib/CodeGen/SpillPlacement.cpp
            lib/CodeGen/SplitKit.cpp
            lib/CodeGen/StackColoring.cpp
            lib/CodeGen/StackMapLivenessAnalysis.cpp
            lib/CodeGen/StackMaps.cpp
            lib/CodeGen/StackProtector.cpp
            lib/CodeGen/StackSlotColoring.cpp
            lib/CodeGen/TailDuplication.cpp
            lib/CodeGen/TailDuplicator.cpp
            lib/CodeGen/TargetFrameLoweringImpl.cpp
            lib/CodeGen/TargetInstrInfo.cpp
            lib/CodeGen/TargetLoweringBase.cpp
            lib/CodeGen/TargetLoweringObjectFileImpl.cpp
            lib/CodeGen/TargetOptionsImpl.cpp
            lib/CodeGen/TargetPassConfig.cpp
            lib/CodeGen/TargetRegisterInfo.cpp
            lib/CodeGen/TargetSchedule.cpp
            lib/CodeGen/TwoAddressInstructionPass.cpp
            lib/CodeGen/UnreachableBlockElim.cpp
            lib/CodeGen/VirtRegMap.cpp
            lib/CodeGen/WinEHPrepare.cpp
            lib/CodeGen/XRayInstrumentation.cpp

            lib/CodeGen/AsmPrinter/AddressPool.cpp
            lib/CodeGen/AsmPrinter/ARMException.cpp
            lib/CodeGen/AsmPrinter/AsmPrinter.cpp
            lib/CodeGen/AsmPrinter/AsmPrinterDwarf.cpp
            lib/CodeGen/AsmPrinter/AsmPrinterInlineAsm.cpp
            lib/CodeGen/AsmPrinter/CodeViewDebug.cpp
            lib/CodeGen/AsmPrinter/DbgValueHistoryCalculator.cpp
            lib/CodeGen/AsmPrinter/DebugHandlerBase.cpp
            lib/CodeGen/AsmPrinter/DebugLocStream.cpp
            lib/CodeGen/AsmPrinter/DIE.cpp
            lib/CodeGen/AsmPrinter/DIEHash.cpp
            lib/CodeGen/AsmPrinter/DwarfAccelTable.cpp
            lib/CodeGen/AsmPrinter/DwarfCFIException.cpp
            lib/CodeGen/AsmPrinter/DwarfCompileUnit.cpp
            lib/CodeGen/AsmPrinter/DwarfDebug.cpp
            lib/CodeGen/AsmPrinter/DwarfExpression.cpp
            lib/CodeGen/AsmPrinter/DwarfFile.cpp
            lib/CodeGen/AsmPrinter/DwarfStringPool.cpp
            lib/CodeGen/AsmPrinter/DwarfUnit.cpp
            lib/CodeGen/AsmPrinter/EHStreamer.cpp
            lib/CodeGen/AsmPrinter/ErlangGCPrinter.cpp
            lib/CodeGen/AsmPrinter/OcamlGCPrinter.cpp
            lib/CodeGen/AsmPrinter/WinException.cpp

            lib/CodeGen/GlobalISel/GlobalISel.cpp
            lib/CodeGen/GlobalISel/IRTranslator.cpp
            lib/CodeGen/GlobalISel/MachineIRBuilder.cpp
            lib/CodeGen/GlobalISel/RegBankSelect.cpp
            lib/CodeGen/GlobalISel/RegisterBank.cpp
            lib/CodeGen/GlobalISel/RegisterBankInfo.cpp

            lib/CodeGen/MIRParser/MILexer.cpp
            lib/CodeGen/MIRParser/MIParser.cpp
            lib/CodeGen/MIRParser/MIRParser.cpp

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
            lib/CodeGen/SelectionDAG/SelectionDAGTargetInfo.cpp
            lib/CodeGen/SelectionDAG/StatepointLowering.cpp
            lib/CodeGen/SelectionDAG/TargetLowering.cpp

            lib/DebugInfo/CodeView/ByteStream.cpp
            lib/DebugInfo/CodeView/CodeViewError.cpp
            lib/DebugInfo/CodeView/CVTypeVisitor.cpp
            lib/DebugInfo/CodeView/EnumTables.cpp
            lib/DebugInfo/CodeView/FieldListRecordBuilder.cpp
            lib/DebugInfo/CodeView/Line.cpp
            lib/DebugInfo/CodeView/ListRecordBuilder.cpp
            lib/DebugInfo/CodeView/MemoryTypeTableBuilder.cpp
            lib/DebugInfo/CodeView/MethodListRecordBuilder.cpp
            lib/DebugInfo/CodeView/ModuleSubstream.cpp
            lib/DebugInfo/CodeView/ModuleSubstreamVisitor.cpp
            lib/DebugInfo/CodeView/RecordSerialization.cpp
            lib/DebugInfo/CodeView/StreamReader.cpp
            lib/DebugInfo/CodeView/StreamWriter.cpp
            lib/DebugInfo/CodeView/SymbolDumper.cpp
            lib/DebugInfo/CodeView/TypeDumper.cpp
            lib/DebugInfo/CodeView/TypeRecord.cpp
            lib/DebugInfo/CodeView/TypeRecordBuilder.cpp
            lib/DebugInfo/CodeView/TypeStreamMerger.cpp
            lib/DebugInfo/CodeView/TypeTableBuilder.cpp

            lib/DebugInfo/DWARF/DWARFAbbreviationDeclaration.cpp
            lib/DebugInfo/DWARF/DWARFAcceleratorTable.cpp
            lib/DebugInfo/DWARF/DWARFCompileUnit.cpp
            lib/DebugInfo/DWARF/DWARFContext.cpp
            lib/DebugInfo/DWARF/DWARFDebugAbbrev.cpp
            lib/DebugInfo/DWARF/DWARFDebugAranges.cpp
            lib/DebugInfo/DWARF/DWARFDebugArangeSet.cpp
            lib/DebugInfo/DWARF/DWARFDebugFrame.cpp
            lib/DebugInfo/DWARF/DWARFDebugInfoEntry.cpp
            lib/DebugInfo/DWARF/DWARFDebugLine.cpp
            lib/DebugInfo/DWARF/DWARFDebugLoc.cpp
            lib/DebugInfo/DWARF/DWARFDebugMacro.cpp
            lib/DebugInfo/DWARF/DWARFDebugRangeList.cpp
            lib/DebugInfo/DWARF/DWARFFormValue.cpp
            lib/DebugInfo/DWARF/DWARFTypeUnit.cpp
            lib/DebugInfo/DWARF/DWARFUnit.cpp
            lib/DebugInfo/DWARF/DWARFUnitIndex.cpp
            lib/DebugInfo/DWARF/SyntaxHighlighting.cpp

            lib/DebugInfo/PDB/GenericError.cpp
            lib/DebugInfo/PDB/IPDBSourceFile.cpp
            lib/DebugInfo/PDB/PDB.cpp
            lib/DebugInfo/PDB/PDBContext.cpp
            lib/DebugInfo/PDB/PDBExtras.cpp
            lib/DebugInfo/PDB/PDBInterfaceAnchors.cpp
            lib/DebugInfo/PDB/PDBSymDumper.cpp
            lib/DebugInfo/PDB/PDBSymbol.cpp
            lib/DebugInfo/PDB/PDBSymbolAnnotation.cpp
            lib/DebugInfo/PDB/PDBSymbolBlock.cpp
            lib/DebugInfo/PDB/PDBSymbolCompiland.cpp
            lib/DebugInfo/PDB/PDBSymbolCompilandDetails.cpp
            lib/DebugInfo/PDB/PDBSymbolCompilandEnv.cpp
            lib/DebugInfo/PDB/PDBSymbolCustom.cpp
            lib/DebugInfo/PDB/PDBSymbolData.cpp
            lib/DebugInfo/PDB/PDBSymbolExe.cpp
            lib/DebugInfo/PDB/PDBSymbolFunc.cpp
            lib/DebugInfo/PDB/PDBSymbolFuncDebugEnd.cpp
            lib/DebugInfo/PDB/PDBSymbolFuncDebugStart.cpp
            lib/DebugInfo/PDB/PDBSymbolLabel.cpp
            lib/DebugInfo/PDB/PDBSymbolPublicSymbol.cpp
            lib/DebugInfo/PDB/PDBSymbolThunk.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeArray.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeBaseClass.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeBuiltin.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeCustom.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeDimension.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeEnum.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeFriend.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeFunctionArg.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeFunctionSig.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeManaged.cpp
            lib/DebugInfo/PDB/PDBSymbolTypePointer.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeTypedef.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeUDT.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeVTable.cpp
            lib/DebugInfo/PDB/PDBSymbolTypeVTableShape.cpp
            lib/DebugInfo/PDB/PDBSymbolUnknown.cpp
            lib/DebugInfo/PDB/PDBSymbolUsingNamespace.cpp

#            lib/DebugInfo/PDB/DIA/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM. Indeed, the DIA SDK is only available on Windows, and it
#       may or not be present on a build machine...

            lib/DebugInfo/PDB/Raw/DbiStream.cpp
            lib/DebugInfo/PDB/Raw/DbiStreamBuilder.cpp
            lib/DebugInfo/PDB/Raw/EnumTables.cpp
            lib/DebugInfo/PDB/Raw/Hash.cpp
            lib/DebugInfo/PDB/Raw/IndexedStreamData.cpp
            lib/DebugInfo/PDB/Raw/InfoStream.cpp
            lib/DebugInfo/PDB/Raw/InfoStreamBuilder.cpp
            lib/DebugInfo/PDB/Raw/MappedBlockStream.cpp
            lib/DebugInfo/PDB/Raw/ModInfo.cpp
            lib/DebugInfo/PDB/Raw/ModStream.cpp
            lib/DebugInfo/PDB/Raw/MsfBuilder.cpp
            lib/DebugInfo/PDB/Raw/MsfCommon.cpp
            lib/DebugInfo/PDB/Raw/NameHashTable.cpp
            lib/DebugInfo/PDB/Raw/NameMap.cpp
            lib/DebugInfo/PDB/Raw/NameMapBuilder.cpp
            lib/DebugInfo/PDB/Raw/PDBFile.cpp
            lib/DebugInfo/PDB/Raw/PDBFileBuilder.cpp
            lib/DebugInfo/PDB/Raw/PublicsStream.cpp
            lib/DebugInfo/PDB/Raw/RawError.cpp
            lib/DebugInfo/PDB/Raw/RawSession.cpp
            lib/DebugInfo/PDB/Raw/SymbolStream.cpp
            lib/DebugInfo/PDB/Raw/TpiStream.cpp

            lib/DebugInfo/Symbolize/DIPrinter.cpp
            lib/DebugInfo/Symbolize/SymbolizableObjectFile.cpp
            lib/DebugInfo/Symbolize/Symbolize.cpp

            lib/ExecutionEngine/ExecutionEngine.cpp
            lib/ExecutionEngine/ExecutionEngineBindings.cpp
            lib/ExecutionEngine/GDBRegistrationListener.cpp
            lib/ExecutionEngine/SectionMemoryManager.cpp
            lib/ExecutionEngine/TargetSelect.cpp

#            lib/ExecutionEngine/IntelJITEvents/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM. Indeed, Intel JIT API support is only available on Linux
#       and Windows, and it requires additional features that may or not be
#       present on a build machine...

            lib/ExecutionEngine/Interpreter/Execution.cpp
            lib/ExecutionEngine/Interpreter/ExternalFunctions.cpp
            lib/ExecutionEngine/Interpreter/Interpreter.cpp

            lib/ExecutionEngine/MCJIT/MCJIT.cpp

#            lib/ExecutionEngine/OProfileJIT/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM. Indeed, OProfile support is only available on Linux,
#       and it requires additional features that may or not be present on a
#       build machine...

            lib/ExecutionEngine/Orc/ExecutionUtils.cpp
            lib/ExecutionEngine/Orc/IndirectionUtils.cpp
            lib/ExecutionEngine/Orc/NullResolver.cpp
            lib/ExecutionEngine/Orc/OrcABISupport.cpp
            lib/ExecutionEngine/Orc/OrcCBindings.cpp
            lib/ExecutionEngine/Orc/OrcError.cpp
            lib/ExecutionEngine/Orc/OrcMCJITReplacement.cpp
            lib/ExecutionEngine/Orc/OrcRemoteTargetRPCAPI.cpp

            lib/ExecutionEngine/RuntimeDyld/RTDyldMemoryManager.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyld.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldChecker.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldCOFF.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldELF.cpp
            lib/ExecutionEngine/RuntimeDyld/RuntimeDyldMachO.cpp

#            lib/Fuzzer/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM. Indeed, the Fuzzer library is to be moved to
#       http://llvm.org/docs/LibFuzzer.html, so it's not part of LLVM as such...

            lib/IR/AsmWriter.cpp
            lib/IR/Attributes.cpp
            lib/IR/AutoUpgrade.cpp
            lib/IR/BasicBlock.cpp
            lib/IR/Comdat.cpp
            lib/IR/ConstantFold.cpp
            lib/IR/ConstantRange.cpp
            lib/IR/Constants.cpp
            lib/IR/Core.cpp
            lib/IR/DataLayout.cpp
            lib/IR/DebugInfo.cpp
            lib/IR/DebugInfoMetadata.cpp
            lib/IR/DebugLoc.cpp
            lib/IR/DiagnosticInfo.cpp
            lib/IR/DiagnosticPrinter.cpp
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
            lib/IR/IRPrintingPasses.cpp
            lib/IR/LegacyPassManager.cpp
            lib/IR/LLVMContext.cpp
            lib/IR/LLVMContextImpl.cpp
            lib/IR/Mangler.cpp
            lib/IR/MDBuilder.cpp
            lib/IR/Metadata.cpp
            lib/IR/Module.cpp
            lib/IR/ModuleSummaryIndex.cpp
            lib/IR/Operator.cpp
            lib/IR/OptBisect.cpp
            lib/IR/Pass.cpp
            lib/IR/PassManager.cpp
            lib/IR/PassRegistry.cpp
            lib/IR/ProfileSummary.cpp
            lib/IR/Statepoint.cpp
            lib/IR/Type.cpp
            lib/IR/TypeFinder.cpp
            lib/IR/Use.cpp
            lib/IR/User.cpp
            lib/IR/Value.cpp
            lib/IR/ValueSymbolTable.cpp
            lib/IR/ValueTypes.cpp
            lib/IR/Verifier.cpp

            lib/IRReader/IRReader.cpp

            lib/LibDriver/LibDriver.cpp

            lib/LineEditor/LineEditor.cpp

            lib/Linker/IRMover.cpp
            lib/Linker/LinkModules.cpp

            lib/LTO/LTO.cpp
            lib/LTO/LTOCodeGenerator.cpp
            lib/LTO/LTOModule.cpp
            lib/LTO/ThinLTOCodeGenerator.cpp
            lib/LTO/UpdateCompilerUsed.cpp

            lib/MC/ConstantPools.cpp
            lib/MC/ELFObjectWriter.cpp
            lib/MC/MachObjectWriter.cpp
            lib/MC/MCAsmBackend.cpp
            lib/MC/MCAsmInfo.cpp
            lib/MC/MCAsmInfoCOFF.cpp
            lib/MC/MCAsmInfoDarwin.cpp
            lib/MC/MCAsmInfoELF.cpp
            lib/MC/MCAsmStreamer.cpp
            lib/MC/MCAssembler.cpp
            lib/MC/MCCodeEmitter.cpp
            lib/MC/MCCodeView.cpp
            lib/MC/MCContext.cpp
            lib/MC/MCDwarf.cpp
            lib/MC/MCELFObjectTargetWriter.cpp
            lib/MC/MCELFStreamer.cpp
            lib/MC/MCExpr.cpp
            lib/MC/MCFragment.cpp
            lib/MC/MCInst.cpp
            lib/MC/MCInstPrinter.cpp
            lib/MC/MCInstrAnalysis.cpp
            lib/MC/MCInstrDesc.cpp
            lib/MC/MCLabel.cpp
            lib/MC/MCLinkerOptimizationHint.cpp
            lib/MC/MCMachObjectTargetWriter.cpp
            lib/MC/MCMachOStreamer.cpp
            lib/MC/MCNullStreamer.cpp
            lib/MC/MCObjectFileInfo.cpp
            lib/MC/MCObjectStreamer.cpp
            lib/MC/MCObjectWriter.cpp
            lib/MC/MCRegisterInfo.cpp
            lib/MC/MCSchedule.cpp
            lib/MC/MCSection.cpp
            lib/MC/MCSectionCOFF.cpp
            lib/MC/MCSectionELF.cpp
            lib/MC/MCSectionMachO.cpp
            lib/MC/MCStreamer.cpp
            lib/MC/MCSubtargetInfo.cpp
            lib/MC/MCSymbol.cpp
            lib/MC/MCSymbolELF.cpp
            lib/MC/MCTargetOptions.cpp
            lib/MC/MCValue.cpp
            lib/MC/MCWin64EH.cpp
            lib/MC/MCWinEH.cpp
            lib/MC/StringTableBuilder.cpp
            lib/MC/SubtargetFeature.cpp
            lib/MC/WinCOFFObjectWriter.cpp
            lib/MC/WinCOFFStreamer.cpp

            lib/MC/MCDisassembler/Disassembler.cpp
            lib/MC/MCDisassembler/MCDisassembler.cpp
            lib/MC/MCDisassembler/MCExternalSymbolizer.cpp
            lib/MC/MCDisassembler/MCRelocationInfo.cpp
            lib/MC/MCDisassembler/MCSymbolizer.cpp

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
            lib/Object/ArchiveWriter.cpp
            lib/Object/Binary.cpp
            lib/Object/COFFObjectFile.cpp
            lib/Object/ELF.cpp
            lib/Object/ELFObjectFile.cpp
            lib/Object/Error.cpp
            lib/Object/IRObjectFile.cpp
            lib/Object/MachOObjectFile.cpp
            lib/Object/MachOUniversal.cpp
            lib/Object/Object.cpp
            lib/Object/ObjectFile.cpp
            lib/Object/RecordStreamer.cpp
            lib/Object/SymbolicFile.cpp
            lib/Object/ModuleSummaryIndexObjectFile.cpp
            lib/Object/SymbolSize.cpp

            lib/ObjectYAML/COFFYAML.cpp
            lib/ObjectYAML/ELFYAML.cpp
            lib/ObjectYAML/MachOYAML.cpp
            lib/ObjectYAML/ObjectYAML.cpp
            lib/ObjectYAML/YAML.cpp

            lib/Option/Arg.cpp
            lib/Option/ArgList.cpp
            lib/Option/Option.cpp
            lib/Option/OptTable.cpp

            lib/Passes/PassBuilder.cpp

            lib/ProfileData/InstrProf.cpp
            lib/ProfileData/InstrProfReader.cpp
            lib/ProfileData/InstrProfWriter.cpp
            lib/ProfileData/ProfileSummaryBuilder.cpp
            lib/ProfileData/SampleProf.cpp
            lib/ProfileData/SampleProfReader.cpp
            lib/ProfileData/SampleProfWriter.cpp

            lib/ProfileData/Coverage/CoverageMapping.cpp
            lib/ProfileData/Coverage/CoverageMappingReader.cpp
            lib/ProfileData/Coverage/CoverageMappingWriter.cpp

            lib/Support/Allocator.cpp
            lib/Support/APFloat.cpp
            lib/Support/APInt.cpp
            lib/Support/APSInt.cpp
            lib/Support/ARMBuildAttrs.cpp
            lib/Support/ARMWinEH.cpp
            lib/Support/Atomic.cpp
            lib/Support/BlockFrequency.cpp
            lib/Support/BranchProbability.cpp
            lib/Support/CachePruning.cpp
            lib/Support/circular_raw_ostream.cpp
            lib/Support/COM.cpp
            lib/Support/CommandLine.cpp
            lib/Support/Compression.cpp
            lib/Support/ConvertUTF.c
            lib/Support/ConvertUTFWrapper.cpp
            lib/Support/CrashRecoveryContext.cpp
            lib/Support/DAGDeltaAlgorithm.cpp
            lib/Support/DataExtractor.cpp
            lib/Support/DataStream.cpp
            lib/Support/Debug.cpp
            lib/Support/DeltaAlgorithm.cpp
            lib/Support/Dwarf.cpp
            lib/Support/DynamicLibrary.cpp
            lib/Support/Errno.cpp
            lib/Support/Error.cpp
            lib/Support/ErrorHandling.cpp
            lib/Support/FileOutputBuffer.cpp
            lib/Support/FileUtilities.cpp
            lib/Support/FoldingSet.cpp
            lib/Support/FormattedStream.cpp
            lib/Support/GraphWriter.cpp
            lib/Support/Hashing.cpp
            lib/Support/Host.cpp
            lib/Support/IntEqClasses.cpp
            lib/Support/IntervalMap.cpp
            lib/Support/IntrusiveRefCntPtr.cpp
            lib/Support/JamCRC.cpp
            lib/Support/LEB128.cpp
            lib/Support/LineIterator.cpp
            lib/Support/Locale.cpp
            lib/Support/LockFileManager.cpp
            lib/Support/ManagedStatic.cpp
            lib/Support/MathExtras.cpp
            lib/Support/MD5.cpp
            lib/Support/Memory.cpp
            lib/Support/MemoryBuffer.cpp
            lib/Support/MemoryObject.cpp
            lib/Support/Mutex.cpp
            lib/Support/Options.cpp
            lib/Support/Path.cpp
            lib/Support/PluginLoader.cpp
            lib/Support/PrettyStackTrace.cpp
            lib/Support/Process.cpp
            lib/Support/Program.cpp
            lib/Support/RandomNumberGenerator.cpp
            lib/Support/raw_os_ostream.cpp
            lib/Support/raw_ostream.cpp
            lib/Support/regcomp.c
            lib/Support/regerror.c
            lib/Support/Regex.cpp
            lib/Support/regexec.c
            lib/Support/regfree.c
            lib/Support/regstrlcpy.c
            lib/Support/RWMutex.cpp
            lib/Support/ScaledNumber.cpp
            lib/Support/ScopedPrinter.cpp
            lib/Support/SearchForAddressOfSpecialSymbol.cpp
            lib/Support/SHA1.cpp
            lib/Support/Signals.cpp
            lib/Support/SmallPtrSet.cpp
            lib/Support/SmallVector.cpp
            lib/Support/SourceMgr.cpp
            lib/Support/SpecialCaseList.cpp
            lib/Support/Statistic.cpp
            lib/Support/StreamingMemoryObject.cpp
            lib/Support/StringExtras.cpp
            lib/Support/StringMap.cpp
            lib/Support/StringPool.cpp
            lib/Support/StringRef.cpp
            lib/Support/StringSaver.cpp
            lib/Support/SystemUtils.cpp
            lib/Support/TargetParser.cpp
            lib/Support/TargetRegistry.cpp
            lib/Support/Threading.cpp
            lib/Support/ThreadLocal.cpp
            lib/Support/ThreadPool.cpp
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
            lib/TableGen/SetTheory.cpp
            lib/TableGen/StringMatcher.cpp
            lib/TableGen/TableGenBackend.cpp
            lib/TableGen/TGLexer.cpp
            lib/TableGen/TGParser.cpp

            lib/Target/Target.cpp
            lib/Target/TargetIntrinsicInfo.cpp
            lib/Target/TargetLoweringObjectFile.cpp
            lib/Target/TargetMachine.cpp
            lib/Target/TargetMachineC.cpp
            lib/Target/TargetRecip.cpp
            lib/Target/TargetSubtargetInfo.cpp

            lib/Target/X86/X86AsmPrinter.cpp
            lib/Target/X86/X86CallFrameOptimization.cpp
            lib/Target/X86/X86ExpandPseudo.cpp
            lib/Target/X86/X86FastISel.cpp
            lib/Target/X86/X86FixupBWInsts.cpp
            lib/Target/X86/X86FixupLEAs.cpp
            lib/Target/X86/X86FixupSetCC.cpp
            lib/Target/X86/X86FloatingPoint.cpp
            lib/Target/X86/X86FrameLowering.cpp
            lib/Target/X86/X86InstrInfo.cpp
            lib/Target/X86/X86ISelDAGToDAG.cpp
            lib/Target/X86/X86ISelLowering.cpp
            lib/Target/X86/X86MachineFunctionInfo.cpp
            lib/Target/X86/X86MCInstLower.cpp
            lib/Target/X86/X86OptimizeLEAs.cpp
            lib/Target/X86/X86PadShortFunction.cpp
            lib/Target/X86/X86RegisterInfo.cpp
            lib/Target/X86/X86SelectionDAGInfo.cpp
            lib/Target/X86/X86ShuffleDecodeConstantPool.cpp
            lib/Target/X86/X86Subtarget.cpp
            lib/Target/X86/X86TargetMachine.cpp
            lib/Target/X86/X86TargetObjectFile.cpp
            lib/Target/X86/X86TargetTransformInfo.cpp
            lib/Target/X86/X86VZeroUpper.cpp
            lib/Target/X86/X86WinAllocaExpander.cpp
            lib/Target/X86/X86WinEHState.cpp

            lib/Target/X86/AsmParser/X86AsmInstrumentation.cpp
            lib/Target/X86/AsmParser/X86AsmParser.cpp

            lib/Target/X86/Disassembler/X86Disassembler.cpp
            lib/Target/X86/Disassembler/X86DisassemblerDecoder.cpp

            lib/Target/X86/InstPrinter/X86ATTInstPrinter.cpp
            lib/Target/X86/InstPrinter/X86InstComments.cpp
            lib/Target/X86/InstPrinter/X86IntelInstPrinter.cpp

            lib/Target/X86/MCTargetDesc/X86AsmBackend.cpp
            lib/Target/X86/MCTargetDesc/X86ELFObjectWriter.cpp
            lib/Target/X86/MCTargetDesc/X86MachObjectWriter.cpp
            lib/Target/X86/MCTargetDesc/X86MCAsmInfo.cpp
            lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp
            lib/Target/X86/MCTargetDesc/X86MCTargetDesc.cpp
            lib/Target/X86/MCTargetDesc/X86WinCOFFObjectWriter.cpp
            lib/Target/X86/MCTargetDesc/X86WinCOFFStreamer.cpp

            lib/Target/X86/TargetInfo/X86TargetInfo.cpp

            lib/Target/X86/Utils/X86ShuffleDecode.cpp

#            lib/Transforms/Hello/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM...

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
            lib/Transforms/Instrumentation/EfficiencySanitizer.cpp
            lib/Transforms/Instrumentation/GCOVProfiling.cpp
            lib/Transforms/Instrumentation/IndirectCallPromotion.cpp
            lib/Transforms/Instrumentation/InstrProfiling.cpp
            lib/Transforms/Instrumentation/Instrumentation.cpp
            lib/Transforms/Instrumentation/MemorySanitizer.cpp
            lib/Transforms/Instrumentation/PGOInstrumentation.cpp
            lib/Transforms/Instrumentation/SanitizerCoverage.cpp
            lib/Transforms/Instrumentation/ThreadSanitizer.cpp

            lib/Transforms/IPO/ArgumentPromotion.cpp
            lib/Transforms/IPO/BarrierNoopPass.cpp
            lib/Transforms/IPO/ConstantMerge.cpp
            lib/Transforms/IPO/CrossDSOCFI.cpp
            lib/Transforms/IPO/DeadArgumentElimination.cpp
            lib/Transforms/IPO/ElimAvailExtern.cpp
            lib/Transforms/IPO/ExtractGV.cpp
            lib/Transforms/IPO/ForceFunctionAttrs.cpp
            lib/Transforms/IPO/FunctionAttrs.cpp
            lib/Transforms/IPO/FunctionImport.cpp
            lib/Transforms/IPO/GlobalDCE.cpp
            lib/Transforms/IPO/GlobalOpt.cpp
            lib/Transforms/IPO/InferFunctionAttrs.cpp
            lib/Transforms/IPO/InlineAlways.cpp
            lib/Transforms/IPO/Inliner.cpp
            lib/Transforms/IPO/InlineSimple.cpp
            lib/Transforms/IPO/Internalize.cpp
            lib/Transforms/IPO/IPConstantPropagation.cpp
            lib/Transforms/IPO/IPO.cpp
            lib/Transforms/IPO/LoopExtractor.cpp
            lib/Transforms/IPO/LowerTypeTests.cpp
            lib/Transforms/IPO/MergeFunctions.cpp
            lib/Transforms/IPO/PartialInlining.cpp
            lib/Transforms/IPO/PassManagerBuilder.cpp
            lib/Transforms/IPO/PruneEH.cpp
            lib/Transforms/IPO/SampleProfile.cpp
            lib/Transforms/IPO/StripDeadPrototypes.cpp
            lib/Transforms/IPO/StripSymbols.cpp
            lib/Transforms/IPO/WholeProgramDevirt.cpp

            lib/Transforms/ObjCARC/DependencyAnalysis.cpp
            lib/Transforms/ObjCARC/ObjCARC.cpp
            lib/Transforms/ObjCARC/ObjCARCAPElim.cpp
            lib/Transforms/ObjCARC/ObjCARCContract.cpp
            lib/Transforms/ObjCARC/ObjCARCExpand.cpp
            lib/Transforms/ObjCARC/ObjCARCOpts.cpp
            lib/Transforms/ObjCARC/ProvenanceAnalysis.cpp
            lib/Transforms/ObjCARC/ProvenanceAnalysisEvaluator.cpp
            lib/Transforms/ObjCARC/PtrState.cpp

            lib/Transforms/Scalar/ADCE.cpp
            lib/Transforms/Scalar/AlignmentFromAssumptions.cpp
            lib/Transforms/Scalar/BDCE.cpp
            lib/Transforms/Scalar/ConstantHoisting.cpp
            lib/Transforms/Scalar/ConstantProp.cpp
            lib/Transforms/Scalar/CorrelatedValuePropagation.cpp
            lib/Transforms/Scalar/DCE.cpp
            lib/Transforms/Scalar/DeadStoreElimination.cpp
            lib/Transforms/Scalar/EarlyCSE.cpp
            lib/Transforms/Scalar/FlattenCFGPass.cpp
            lib/Transforms/Scalar/Float2Int.cpp
            lib/Transforms/Scalar/GuardWidening.cpp
            lib/Transforms/Scalar/GVN.cpp
            lib/Transforms/Scalar/GVNHoist.cpp
            lib/Transforms/Scalar/InductiveRangeCheckElimination.cpp
            lib/Transforms/Scalar/IndVarSimplify.cpp
            lib/Transforms/Scalar/JumpThreading.cpp
            lib/Transforms/Scalar/LICM.cpp
            lib/Transforms/Scalar/LoadCombine.cpp
            lib/Transforms/Scalar/LoopDataPrefetch.cpp
            lib/Transforms/Scalar/LoopDeletion.cpp
            lib/Transforms/Scalar/LoopDistribute.cpp
            lib/Transforms/Scalar/LoopIdiomRecognize.cpp
            lib/Transforms/Scalar/LoopInstSimplify.cpp
            lib/Transforms/Scalar/LoopInterchange.cpp
            lib/Transforms/Scalar/LoopLoadElimination.cpp
            lib/Transforms/Scalar/LoopRerollPass.cpp
            lib/Transforms/Scalar/LoopRotation.cpp
            lib/Transforms/Scalar/LoopSimplifyCFG.cpp
            lib/Transforms/Scalar/LoopStrengthReduce.cpp
            lib/Transforms/Scalar/LoopUnrollPass.cpp
            lib/Transforms/Scalar/LoopUnswitch.cpp
            lib/Transforms/Scalar/LoopVersioningLICM.cpp
            lib/Transforms/Scalar/LowerAtomic.cpp
            lib/Transforms/Scalar/LowerExpectIntrinsic.cpp
            lib/Transforms/Scalar/LowerGuardIntrinsic.cpp
            lib/Transforms/Scalar/MemCpyOptimizer.cpp
            lib/Transforms/Scalar/MergedLoadStoreMotion.cpp
            lib/Transforms/Scalar/NaryReassociate.cpp
            lib/Transforms/Scalar/PartiallyInlineLibCalls.cpp
            lib/Transforms/Scalar/PlaceSafepoints.cpp
            lib/Transforms/Scalar/Reassociate.cpp
            lib/Transforms/Scalar/Reg2Mem.cpp
            lib/Transforms/Scalar/RewriteStatepointsForGC.cpp
            lib/Transforms/Scalar/Scalar.cpp
            lib/Transforms/Scalar/Scalarizer.cpp
            lib/Transforms/Scalar/SCCP.cpp
            lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp
            lib/Transforms/Scalar/SimplifyCFGPass.cpp
            lib/Transforms/Scalar/Sink.cpp
            lib/Transforms/Scalar/SpeculativeExecution.cpp
            lib/Transforms/Scalar/SROA.cpp
            lib/Transforms/Scalar/StraightLineStrengthReduce.cpp
            lib/Transforms/Scalar/StructurizeCFG.cpp
            lib/Transforms/Scalar/TailRecursionElimination.cpp

            lib/Transforms/Utils/AddDiscriminators.cpp
            lib/Transforms/Utils/ASanStackFrameLayout.cpp
            lib/Transforms/Utils/BasicBlockUtils.cpp
            lib/Transforms/Utils/BreakCriticalEdges.cpp
            lib/Transforms/Utils/BuildLibCalls.cpp
            lib/Transforms/Utils/BypassSlowDivision.cpp
            lib/Transforms/Utils/CloneFunction.cpp
            lib/Transforms/Utils/CloneModule.cpp
            lib/Transforms/Utils/CmpInstAnalysis.cpp
            lib/Transforms/Utils/CodeExtractor.cpp
            lib/Transforms/Utils/CtorUtils.cpp
            lib/Transforms/Utils/DemoteRegToStack.cpp
            lib/Transforms/Utils/Evaluator.cpp
            lib/Transforms/Utils/FlattenCFG.cpp
            lib/Transforms/Utils/FunctionImportUtils.cpp
            lib/Transforms/Utils/GlobalStatus.cpp
            lib/Transforms/Utils/InlineFunction.cpp
            lib/Transforms/Utils/InstructionNamer.cpp
            lib/Transforms/Utils/IntegerDivision.cpp
            lib/Transforms/Utils/LCSSA.cpp
            lib/Transforms/Utils/Local.cpp
            lib/Transforms/Utils/LoopSimplify.cpp
            lib/Transforms/Utils/LoopUnroll.cpp
            lib/Transforms/Utils/LoopUnrollRuntime.cpp
            lib/Transforms/Utils/LoopUtils.cpp
            lib/Transforms/Utils/LoopVersioning.cpp
            lib/Transforms/Utils/LowerInvoke.cpp
            lib/Transforms/Utils/LowerSwitch.cpp
            lib/Transforms/Utils/Mem2Reg.cpp
            lib/Transforms/Utils/MemorySSA.cpp
            lib/Transforms/Utils/MetaRenamer.cpp
            lib/Transforms/Utils/ModuleUtils.cpp
            lib/Transforms/Utils/NameAnonFunctions.cpp
            lib/Transforms/Utils/PromoteMemoryToRegister.cpp
            lib/Transforms/Utils/SanitizerStats.cpp
            lib/Transforms/Utils/SimplifyCFG.cpp
            lib/Transforms/Utils/SimplifyIndVar.cpp
            lib/Transforms/Utils/SimplifyInstructions.cpp
            lib/Transforms/Utils/SimplifyLibCalls.cpp
            lib/Transforms/Utils/SplitModule.cpp
            lib/Transforms/Utils/SSAUpdater.cpp
            lib/Transforms/Utils/SymbolRewriter.cpp
            lib/Transforms/Utils/UnifyFunctionExitNodes.cpp
            lib/Transforms/Utils/Utils.cpp
            lib/Transforms/Utils/ValueMapper.cpp

            lib/Transforms/Vectorize/BBVectorize.cpp
            lib/Transforms/Vectorize/LoadStoreVectorizer.cpp
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
            tools/clang/lib/Analysis/CodeInjector.cpp
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
            tools/clang/lib/Analysis/ThreadSafetyCommon.cpp
            tools/clang/lib/Analysis/ThreadSafetyLogical.cpp
            tools/clang/lib/Analysis/ThreadSafetyTIL.cpp
            tools/clang/lib/Analysis/UninitializedValues.cpp

#            tools/clang/lib/ARCMigrate/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM...

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
            tools/clang/lib/AST/ExprObjC.cpp
            tools/clang/lib/AST/ExternalASTSource.cpp
            tools/clang/lib/AST/InheritViz.cpp
            tools/clang/lib/AST/ItaniumCXXABI.cpp
            tools/clang/lib/AST/ItaniumMangle.cpp
            tools/clang/lib/AST/Mangle.cpp
            tools/clang/lib/AST/MicrosoftCXXABI.cpp
            tools/clang/lib/AST/MicrosoftMangle.cpp
            tools/clang/lib/AST/NestedNameSpecifier.cpp
            tools/clang/lib/AST/NSAPI.cpp
            tools/clang/lib/AST/OpenMPClause.cpp
            tools/clang/lib/AST/ParentMap.cpp
            tools/clang/lib/AST/RawCommentList.cpp
            tools/clang/lib/AST/RecordLayout.cpp
            tools/clang/lib/AST/RecordLayoutBuilder.cpp
            tools/clang/lib/AST/SelectorLocationsKind.cpp
            tools/clang/lib/AST/Stmt.cpp
            tools/clang/lib/AST/StmtCXX.cpp
            tools/clang/lib/AST/StmtIterator.cpp
            tools/clang/lib/AST/StmtObjC.cpp
            tools/clang/lib/AST/StmtOpenMP.cpp
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

            tools/clang/lib/Basic/Attributes.cpp
            tools/clang/lib/Basic/Builtins.cpp
            tools/clang/lib/Basic/CharInfo.cpp
            tools/clang/lib/Basic/Cuda.cpp
            tools/clang/lib/Basic/Diagnostic.cpp
            tools/clang/lib/Basic/DiagnosticIDs.cpp
            tools/clang/lib/Basic/DiagnosticOptions.cpp
            tools/clang/lib/Basic/FileManager.cpp
            tools/clang/lib/Basic/FileSystemStatCache.cpp
            tools/clang/lib/Basic/IdentifierTable.cpp
            tools/clang/lib/Basic/LangOptions.cpp
            tools/clang/lib/Basic/Module.cpp
            tools/clang/lib/Basic/ObjCRuntime.cpp
            tools/clang/lib/Basic/OpenMPKinds.cpp
            tools/clang/lib/Basic/OperatorPrecedence.cpp
            tools/clang/lib/Basic/SanitizerBlacklist.cpp
            tools/clang/lib/Basic/Sanitizers.cpp
            tools/clang/lib/Basic/SourceLocation.cpp
            tools/clang/lib/Basic/SourceManager.cpp
            tools/clang/lib/Basic/TargetInfo.cpp
            tools/clang/lib/Basic/Targets.cpp
            tools/clang/lib/Basic/TokenKinds.cpp
            tools/clang/lib/Basic/Version.cpp
            tools/clang/lib/Basic/VersionTuple.cpp
            tools/clang/lib/Basic/VirtualFileSystem.cpp
            tools/clang/lib/Basic/Warnings.cpp

            tools/clang/lib/CodeGen/BackendUtil.cpp
            tools/clang/lib/CodeGen/CGAtomic.cpp
            tools/clang/lib/CodeGen/CGBlocks.cpp
            tools/clang/lib/CodeGen/CGBuiltin.cpp
            tools/clang/lib/CodeGen/CGCall.cpp
            tools/clang/lib/CodeGen/CGClass.cpp
            tools/clang/lib/CodeGen/CGCleanup.cpp
            tools/clang/lib/CodeGen/CGCUDABuiltin.cpp
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
            tools/clang/lib/CodeGen/CGLoopInfo.cpp
            tools/clang/lib/CodeGen/CGObjC.cpp
            tools/clang/lib/CodeGen/CGObjCGNU.cpp
            tools/clang/lib/CodeGen/CGObjCMac.cpp
            tools/clang/lib/CodeGen/CGObjCRuntime.cpp
            tools/clang/lib/CodeGen/CGOpenCLRuntime.cpp
            tools/clang/lib/CodeGen/CGOpenMPRuntime.cpp
            tools/clang/lib/CodeGen/CGOpenMPRuntimeNVPTX.cpp
            tools/clang/lib/CodeGen/CGRecordLayoutBuilder.cpp
            tools/clang/lib/CodeGen/CGStmt.cpp
            tools/clang/lib/CodeGen/CGStmtOpenMP.cpp
            tools/clang/lib/CodeGen/CGVTables.cpp
            tools/clang/lib/CodeGen/CGVTT.cpp
            tools/clang/lib/CodeGen/CodeGenABITypes.cpp
            tools/clang/lib/CodeGen/CodeGenAction.cpp
            tools/clang/lib/CodeGen/CodeGenFunction.cpp
            tools/clang/lib/CodeGen/CodeGenModule.cpp
            tools/clang/lib/CodeGen/CodeGenPGO.cpp
            tools/clang/lib/CodeGen/CodeGenTBAA.cpp
            tools/clang/lib/CodeGen/CodeGenTypes.cpp
            tools/clang/lib/CodeGen/CoverageMappingGen.cpp
            tools/clang/lib/CodeGen/ItaniumCXXABI.cpp
            tools/clang/lib/CodeGen/MicrosoftCXXABI.cpp
            tools/clang/lib/CodeGen/ModuleBuilder.cpp
            tools/clang/lib/CodeGen/ObjectFilePCHContainerOperations.cpp
            tools/clang/lib/CodeGen/SanitizerMetadata.cpp
            tools/clang/lib/CodeGen/SwiftCallingConv.cpp
            tools/clang/lib/CodeGen/TargetInfo.cpp

            tools/clang/lib/Driver/Action.cpp
            tools/clang/lib/Driver/Compilation.cpp
            tools/clang/lib/Driver/CrossWindowsToolChain.cpp
            tools/clang/lib/Driver/Driver.cpp
            tools/clang/lib/Driver/DriverOptions.cpp
            tools/clang/lib/Driver/Job.cpp
            tools/clang/lib/Driver/MinGWToolChain.cpp
            tools/clang/lib/Driver/MSVCToolChain.cpp
            tools/clang/lib/Driver/Multilib.cpp
            tools/clang/lib/Driver/Phases.cpp
            tools/clang/lib/Driver/SanitizerArgs.cpp
            tools/clang/lib/Driver/Tool.cpp
            tools/clang/lib/Driver/ToolChain.cpp
            tools/clang/lib/Driver/ToolChains.cpp
            tools/clang/lib/Driver/Tools.cpp
            tools/clang/lib/Driver/Types.cpp

            tools/clang/lib/Edit/Commit.cpp
            tools/clang/lib/Edit/EditedSource.cpp
            tools/clang/lib/Edit/RewriteObjCFoundationAPI.cpp

            tools/clang/lib/Format/AffectedRangeManager.cpp
            tools/clang/lib/Format/BreakableToken.cpp
            tools/clang/lib/Format/ContinuationIndenter.cpp
            tools/clang/lib/Format/Format.cpp
            tools/clang/lib/Format/FormatToken.cpp
            tools/clang/lib/Format/FormatTokenLexer.cpp
            tools/clang/lib/Format/SortJavaScriptImports.cpp
            tools/clang/lib/Format/TokenAnalyzer.cpp
            tools/clang/lib/Format/TokenAnnotator.cpp
            tools/clang/lib/Format/UnwrappedLineFormatter.cpp
            tools/clang/lib/Format/UnwrappedLineParser.cpp
            tools/clang/lib/Format/WhitespaceManager.cpp

            tools/clang/lib/Frontend/ASTConsumers.cpp
            tools/clang/lib/Frontend/ASTMerge.cpp
            tools/clang/lib/Frontend/ASTUnit.cpp
            tools/clang/lib/Frontend/CacheTokens.cpp
            tools/clang/lib/Frontend/ChainedDiagnosticConsumer.cpp
            tools/clang/lib/Frontend/ChainedIncludesSource.cpp
            tools/clang/lib/Frontend/CodeGenOptions.cpp
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
            tools/clang/lib/Frontend/ModuleDependencyCollector.cpp
            tools/clang/lib/Frontend/MultiplexConsumer.cpp
            tools/clang/lib/Frontend/PCHContainerOperations.cpp
            tools/clang/lib/Frontend/PrintPreprocessedOutput.cpp
            tools/clang/lib/Frontend/SerializedDiagnosticPrinter.cpp
            tools/clang/lib/Frontend/SerializedDiagnosticReader.cpp
            tools/clang/lib/Frontend/TestModuleFileExtension.cpp
            tools/clang/lib/Frontend/TextDiagnostic.cpp
            tools/clang/lib/Frontend/TextDiagnosticBuffer.cpp
            tools/clang/lib/Frontend/TextDiagnosticPrinter.cpp
            tools/clang/lib/Frontend/VerifyDiagnosticConsumer.cpp

            tools/clang/lib/Frontend/Rewrite/FixItRewriter.cpp
            tools/clang/lib/Frontend/Rewrite/FrontendActions.cpp
            tools/clang/lib/Frontend/Rewrite/HTMLPrint.cpp
            tools/clang/lib/Frontend/Rewrite/InclusionRewriter.cpp
            tools/clang/lib/Frontend/Rewrite/RewriteMacros.cpp
            tools/clang/lib/Frontend/Rewrite/RewriteModernObjC.cpp
            tools/clang/lib/Frontend/Rewrite/RewriteObjC.cpp
            tools/clang/lib/Frontend/Rewrite/RewriteTest.cpp

            tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp

            tools/clang/lib/Index/CodegenNameGenerator.cpp
            tools/clang/lib/Index/CommentToXML.cpp
            tools/clang/lib/Index/IndexBody.cpp
            tools/clang/lib/Index/IndexDecl.cpp
            tools/clang/lib/Index/IndexingAction.cpp
            tools/clang/lib/Index/IndexingContext.cpp
            tools/clang/lib/Index/IndexSymbol.cpp
            tools/clang/lib/Index/IndexTypeSourceInfo.cpp
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
            tools/clang/lib/Parse/ParseStmtAsm.cpp
            tools/clang/lib/Parse/ParseTemplate.cpp
            tools/clang/lib/Parse/ParseTentative.cpp

            tools/clang/lib/Rewrite/DeltaTree.cpp
            tools/clang/lib/Rewrite/HTMLRewrite.cpp
            tools/clang/lib/Rewrite/Rewriter.cpp
            tools/clang/lib/Rewrite/RewriteRope.cpp
            tools/clang/lib/Rewrite/TokenRewriter.cpp

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
            tools/clang/lib/Sema/SemaCoroutine.cpp
            tools/clang/lib/Sema/SemaCUDA.cpp
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
            tools/clang/lib/Serialization/ModuleFileExtension.cpp
            tools/clang/lib/Serialization/ModuleManager.cpp

#            tools/clang/lib/StaticAnalyzer/...
# Note: we don't need the above, but we leave it so we know about it next time
#       we update LLVM...

            tools/clang/lib/Tooling/ArgumentsAdjusters.cpp
            tools/clang/lib/Tooling/CommonOptionsParser.cpp
            tools/clang/lib/Tooling/CompilationDatabase.cpp
            tools/clang/lib/Tooling/FileMatchTrie.cpp
            tools/clang/lib/Tooling/FixIt.cpp
            tools/clang/lib/Tooling/JSONCompilationDatabase.cpp
            tools/clang/lib/Tooling/Refactoring.cpp
            tools/clang/lib/Tooling/RefactoringCallbacks.cpp
            tools/clang/lib/Tooling/Tooling.cpp

            tools/clang/lib/Tooling/Core/Lookup.cpp
            tools/clang/lib/Tooling/Core/QualTypeNames.cpp
            tools/clang/lib/Tooling/Core/Replacement.cpp
        HEADERS_MOC
            src/llvmplugin.h
        INCLUDE_DIRS
            include
            lib/Target/X86
            src
            tools/clang/include
        DEFINITIONS
            ${DEFINITIONS}
        QT_MODULES
            Core
        SYSTEM_BINARIES
            ${SYSTEM_BINARIES}
    )
ENDIF()
