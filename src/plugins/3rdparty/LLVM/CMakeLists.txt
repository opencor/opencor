PROJECT(LLVMPlugin)

MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve LLVM settings

    IF(MSVC)
        SET(LLVM_MSVC_DEFINITIONS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_NONSTDC_NO_WARNINGS

            _CRT_SECURE_NO_DEPRECATE
            _CRT_SECURE_NO_WARNINGS

            _SCL_SECURE_NO_DEPRECATE
            _SCL_SECURE_NO_WARNINGS
        )
    ELSE()
        SET(LLVM_MSVC_DEFINITIONS)
    ENDIF()

    SET(LLVM_DEFINITIONS
        ${LLVM_MSVC_DEFINITIONS}

        __STDC_CONSTANT_MACROS
        __STDC_LIMIT_MACROS
    )

    IF(MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355 /wd4503")
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Config)
SET(CONFIG_HEADER_FILES
    config.h
    llvm-config.h
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Support)
SET(CONFIG_HEADER_FILES
    DataTypes.h
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

# Retrieve some LLVM settings

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(LLVM
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        lib/Support/Allocator.cpp
        lib/Support/APFloat.cpp
        lib/Support/APInt.cpp
        lib/Support/APSInt.cpp
        lib/Support/Atomic.cpp
        lib/Support/BlockFrequency.cpp
        lib/Support/BranchProbability.cpp
        lib/Support/circular_raw_ostream.cpp
        lib/Support/CommandLine.cpp
        lib/Support/ConstantRange.cpp
        lib/Support/CrashRecoveryContext.cpp
        lib/Support/DAGDeltaAlgorithm.cpp
        lib/Support/DataExtractor.cpp
        lib/Support/Debug.cpp
        lib/Support/DeltaAlgorithm.cpp
        lib/Support/Disassembler.cpp
        lib/Support/Dwarf.cpp
        lib/Support/DynamicLibrary.cpp
        lib/Support/Errno.cpp
        lib/Support/ErrorHandling.cpp
        lib/Support/FileUtilities.cpp
        lib/Support/FoldingSet.cpp
        lib/Support/FormattedStream.cpp
        lib/Support/GraphWriter.cpp
        lib/Support/Host.cpp
        lib/Support/IncludeFile.cpp
        lib/Support/IntEqClasses.cpp
        lib/Support/IntervalMap.cpp
        lib/Support/IsInf.cpp
        lib/Support/IsNAN.cpp
        lib/Support/ManagedStatic.cpp
        lib/Support/Memory.cpp
        lib/Support/MemoryBuffer.cpp
        lib/Support/MemoryObject.cpp
        lib/Support/Mutex.cpp
        lib/Support/Path.cpp
        lib/Support/PathV2.cpp
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
        lib/Support/StringExtras.cpp
        lib/Support/StringMap.cpp
        lib/Support/StringPool.cpp
        lib/Support/StringRef.cpp
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
        lib/Support/Valgrind.cpp

        lib/Support/Unix/Host.inc
        lib/Support/Unix/Memory.inc
        lib/Support/Unix/Mutex.inc
        lib/Support/Unix/Path.inc
        lib/Support/Unix/PathV2.inc
        lib/Support/Unix/Process.inc
        lib/Support/Unix/Program.inc
        lib/Support/Unix/RWMutex.inc
        lib/Support/Unix/Signals.inc
        lib/Support/Unix/system_error.inc
        lib/Support/Unix/ThreadLocal.inc
        lib/Support/Unix/TimeValue.inc

        lib/Support/Windows/DynamicLibrary.inc
        lib/Support/Windows/Host.inc
        lib/Support/Windows/Memory.inc
        lib/Support/Windows/Mutex.inc
        lib/Support/Windows/Path.inc
        lib/Support/Windows/PathV2.inc
        lib/Support/Windows/Process.inc
        lib/Support/Windows/Program.inc
        lib/Support/Windows/RWMutex.inc
        lib/Support/Windows/Signals.inc
        lib/Support/Windows/system_error.inc
        lib/Support/Windows/ThreadLocal.inc
        lib/Support/Windows/TimeValue.inc

        lib/VMCore/AsmWriter.cpp
        lib/VMCore/Attributes.cpp
        lib/VMCore/AutoUpgrade.cpp
        lib/VMCore/BasicBlock.cpp
        lib/VMCore/ConstantFold.cpp
        lib/VMCore/Constants.cpp
        lib/VMCore/Core.cpp
        lib/VMCore/DebugInfoProbe.cpp
        lib/VMCore/DebugLoc.cpp
        lib/VMCore/Dominators.cpp
        lib/VMCore/Function.cpp
        lib/VMCore/GCOV.cpp
        lib/VMCore/Globals.cpp
        lib/VMCore/GVMaterializer.cpp
        lib/VMCore/InlineAsm.cpp
        lib/VMCore/Instruction.cpp
        lib/VMCore/Instructions.cpp
        lib/VMCore/IntrinsicInst.cpp
        lib/VMCore/IRBuilder.cpp
        lib/VMCore/LeakDetector.cpp
        lib/VMCore/LLVMContext.cpp
        lib/VMCore/LLVMContextImpl.cpp
        lib/VMCore/Metadata.cpp
        lib/VMCore/Module.cpp
        lib/VMCore/Pass.cpp
        lib/VMCore/PassManager.cpp
        lib/VMCore/PassRegistry.cpp
        lib/VMCore/PrintModulePass.cpp
        lib/VMCore/Type.cpp
        lib/VMCore/Use.cpp
        lib/VMCore/User.cpp
        lib/VMCore/Value.cpp
        lib/VMCore/ValueSymbolTable.cpp
        lib/VMCore/ValueTypes.cpp
        lib/VMCore/Verifier.cpp

        src/llvmplugin.cpp
    HEADERS_MOC
        src/llvmplugin.h
    INCLUDE_DIRS
        include
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    QT_DEPENDENCIES
        QtCore
)
