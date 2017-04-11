
#ifdef GET_GROUPS
#endif // GET_GROUPS


#ifdef GET_PACKAGES
PACKAGE("alpha", -1, false)
PACKAGE("alpha.clone", -1, true)
PACKAGE("alpha.core", -1, true)
PACKAGE("alpha.cplusplus", -1, true)
PACKAGE("alpha.deadcode", -1, true)
PACKAGE("alpha.osx", -1, true)
PACKAGE("alpha.osx.cocoa", -1, true)
PACKAGE("alpha.osx.cocoa.localizability", -1, true)
PACKAGE("alpha.security", -1, true)
PACKAGE("alpha.security.taint", -1, true)
PACKAGE("alpha.unix", -1, true)
PACKAGE("alpha.unix.cstring", -1, true)
PACKAGE("alpha.valist", -1, true)
PACKAGE("apiModeling", -1, true)
PACKAGE("apiModeling.google", -1, true)
PACKAGE("core", -1, false)
PACKAGE("core.builtin", -1, false)
PACKAGE("core.uninitialized", -1, false)
PACKAGE("cplusplus", -1, false)
PACKAGE("deadcode", -1, false)
PACKAGE("debug", -1, false)
PACKAGE("llvm", -1, false)
PACKAGE("nullability", -1, false)
PACKAGE("optin", -1, false)
PACKAGE("optin.cplusplus", -1, false)
PACKAGE("optin.mpi", -1, false)
PACKAGE("optin.osx", -1, false)
PACKAGE("optin.osx.cocoa", -1, false)
PACKAGE("optin.osx.cocoa.localizability", -1, false)
PACKAGE("optin.performance", -1, false)
PACKAGE("osx", -1, false)
PACKAGE("osx.cocoa", -1, false)
PACKAGE("osx.coreFoundation", -1, false)
PACKAGE("osx.coreFoundation.containers", -1, false)
PACKAGE("security", -1, false)
PACKAGE("security.insecureAPI", -1, false)
PACKAGE("unix", -1, false)
PACKAGE("unix.cstring", -1, true)
PACKAGE("valist", -1, false)
#endif // GET_PACKAGES


#ifdef GET_CHECKERS
CHECKER("debug.AnalysisOrder", AnalysisOrderChecker, AnalysisOrder.cpp, "Print callbacks that are called during analysis in order", -1, false)
CHECKER("debug.Stats", AnalyzerStatsChecker, AnalyzerStatsChecker.cpp, "Emit warnings with analyzer statistics", -1, false)
CHECKER("alpha.security.ArrayBound", ArrayBoundChecker, ArrayBoundChecker.cpp, "Warn about buffer overflows (older checker)", -1, true)
CHECKER("alpha.security.ArrayBoundV2", ArrayBoundCheckerV2, ArrayBoundCheckerV2.cpp, "Warn about buffer overflows (newer checker)", -1, true)
CHECKER("alpha.unix.BlockInCriticalSection", BlockInCriticalSectionChecker, BlockInCriticalSectionChecker.cpp, "Check for calls to blocking functions inside a critical section", -1, true)
CHECKER("alpha.core.BoolAssignment", BoolAssignmentChecker, BoolAssignmentChecker.cpp, "Warn about assigning non-{0,1} values to Boolean variables", -1, true)
CHECKER("debug.DumpBugHash", BugHashDumper, DebugCheckers.cpp, "Dump the bug hash for all statements.", -1, false)
CHECKER("core.builtin.BuiltinFunctions", BuiltinFunctionChecker, BuiltinFunctionChecker.cpp, "Evaluate compiler builtin functions (e.g., alloca())", -1, false)
CHECKER("osx.coreFoundation.CFError", CFErrorChecker, NSErrorChecker.cpp, "Check usage of CFErrorRef* parameters", -1, false)
CHECKER("debug.DumpCFG", CFGDumper, DebugCheckers.cpp, "Display Control-Flow Graphs", -1, false)
CHECKER("debug.ViewCFG", CFGViewer, DebugCheckers.cpp, "View Control-Flow Graphs using GraphViz", -1, false)
CHECKER("osx.coreFoundation.CFNumber", CFNumberChecker, BasicObjCFoundationChecks.cpp, "Check for proper uses of CFNumber APIs", -1, false)
CHECKER("osx.coreFoundation.CFRetainRelease", CFRetainReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for null arguments to CFRetain/CFRelease/CFMakeCollectable", -1, false)
CHECKER("alpha.unix.cstring.BufferOverlap", CStringBufferOverlap, CStringChecker.cpp, "Checks for overlap in two buffer arguments", -1, true)
CHECKER("alpha.unix.cstring.NotNullTerminated", CStringNotNullTerm, CStringChecker.cpp, "Check for arguments which are not null-terminating strings", -1, true)
CHECKER("unix.cstring.NullArg", CStringNullArg, CStringChecker.cpp, "Check for null pointers being passed as arguments to C string functions", -1, true)
CHECKER("alpha.unix.cstring.OutOfBounds", CStringOutOfBounds, CStringChecker.cpp, "Check for out-of-bounds access in string functions", -1, true)
CHECKER("unix.cstring.BadSizeArg", CStringSyntaxChecker, CStringSyntaxChecker.cpp, "Check the size argument passed into C string functions for common erroneous patterns", -1, true)
CHECKER("cplusplus.SelfAssignment", CXXSelfAssignmentChecker, CXXSelfAssignmentChecker.cpp, "Checks C++ copy and move assignment operators for self assignment", -1, false)
CHECKER("core.CallAndMessage", CallAndMessageChecker, CallAndMessageChecker.cpp, "Check for logical errors for function calls and Objective-C message expressions (e.g., uninitialized arguments, null function pointers)", -1, false)
CHECKER("alpha.core.CallAndMessageUnInitRefArg", CallAndMessageUnInitRefArg, CallAndMessageChecker.cpp, "Check for logical errors for function calls and Objective-C message expressions (e.g., uninitialized arguments, null function pointers, and pointer to undefined variables)", -1, true)
CHECKER("debug.DumpCalls", CallDumper, TraversalChecker.cpp, "Print calls as they are traversed by the engine", -1, false)
CHECKER("debug.DumpCallGraph", CallGraphDumper, DebugCheckers.cpp, "Display Call Graph", -1, false)
CHECKER("debug.ViewCallGraph", CallGraphViewer, DebugCheckers.cpp, "View Call Graph using GraphViz", -1, false)
CHECKER("alpha.core.CastSize", CastSizeChecker, CastSizeChecker.cpp, "Check when casting a malloc'ed type T, whether the size is a multiple of the size of T", -1, true)
CHECKER("alpha.core.CastToStruct", CastToStructChecker, CastToStructChecker.cpp, "Check for cast from non-struct pointer to struct pointer", -1, true)
CHECKER("alpha.unix.Chroot", ChrootChecker, ChrootChecker.cpp, "Check improper use of chroot", -1, true)
CHECKER("osx.cocoa.ClassRelease", ClassReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for sending 'retain', 'release', or 'autorelease' directly to a Class", -1, false)
CHECKER("alpha.clone.CloneChecker", CloneChecker, CloneChecker.cpp, "Reports similar pieces of code.", -1, true)
CHECKER("debug.ConfigDumper", ConfigDumper, DebugCheckers.cpp, "Dump config table", -1, false)
CHECKER("alpha.core.Conversion", ConversionChecker, ConversionChecker.cpp, "Loss of sign/precision in implicit conversions", -1, true)
CHECKER("alpha.valist.CopyToSelf", CopyToSelfChecker, ValistChecker.cpp, "Check for va_lists which are copied onto itself.", -1, true)
CHECKER("deadcode.DeadStores", DeadStoresChecker, DeadStoresChecker.cpp, "Check for values stored to variables that are never read afterwards", -1, false)
CHECKER("core.NullDereference", DereferenceChecker, DereferenceChecker.cpp, "Check for dereferences of null pointers", -1, false)
CHECKER("alpha.osx.cocoa.DirectIvarAssignment", DirectIvarAssignment, DirectIvarAssignment.cpp, "Check for direct assignments to instance variables", -1, true)
CHECKER("alpha.osx.cocoa.DirectIvarAssignmentForAnnotatedFunctions", DirectIvarAssignmentForAnnotatedFunctions, DirectIvarAssignment.cpp, "Check for direct assignments to instance variables in the methods annotated with objc_no_direct_instance_variable_assignment", -1, true)
CHECKER("core.DivideZero", DivZeroChecker, DivZeroChecker.cpp, "Check for division by zero", -1, false)
CHECKER("debug.DumpDominators", DominatorsTreeDumper, DebugCheckers.cpp, "Print the dominance tree for a given CFG", -1, false)
CHECKER("alpha.core.DynamicTypeChecker", DynamicTypeChecker, DynamicTypeChecker.cpp, "Check for cases where the dynamic and the static type of an object are unrelated.", -1, true)
CHECKER("core.DynamicTypePropagation", DynamicTypePropagation, DynamicTypePropagation.cpp, "Generate dynamic type information", -1, false)
CHECKER("optin.osx.cocoa.localizability.EmptyLocalizationContextChecker", EmptyLocalizationContextChecker, LocalizationChecker.cpp, "Check that NSLocalizedString macros include a comment for context", -1, false)
CHECKER("debug.ViewExplodedGraph", ExplodedGraphViewer, DebugCheckers.cpp, "View Exploded Graphs using GraphViz", -1, false)
CHECKER("debug.ExprInspection", ExprInspectionChecker, ExprInspectionChecker.cpp, "Check the analyzer's understanding of expressions", -1, false)
CHECKER("alpha.core.FixedAddr", FixedAddressChecker, FixedAddressChecker.cpp, "Check for assignment of a fixed address to a pointer", -1, true)
CHECKER("security.FloatLoopCounter", FloatLoopCounter, CheckSecuritySyntaxOnly.cpp, "Warn on using a floating point value as a loop counter (CERT: FLP30-C, FLP30-CPP)", -1, false)
CHECKER("apiModeling.google.GTest", GTestChecker, GTestChecker.cpp, "Model gtest assertion APIs", -1, true)
CHECKER("alpha.security.taint.TaintPropagation", GenericTaintChecker, GenericTaintChecker.cpp, "Generate taint information used by other checkers", -1, true)
CHECKER("alpha.core.IdenticalExpr", IdenticalExprChecker, IdenticalExprChecker.cpp, "Warn about unintended use of identical expressions in operators", -1, true)
CHECKER("alpha.osx.cocoa.InstanceVariableInvalidation", InstanceVariableInvalidation, IvarInvalidationChecker.cpp, "Check that the invalidatable instance variables are invalidated in the methods annotated with objc_instance_variable_invalidator", -1, true)
CHECKER("alpha.cplusplus.IteratorPastEnd", IteratorPastEndChecker, IteratorPastEndChecker.cpp, "Check iterators used past end", -1, true)
CHECKER("llvm.Conventions", LLVMConventionsChecker, LLVMConventionsChecker.cpp, "Check code for LLVM codebase conventions", -1, false)
CHECKER("debug.DumpLiveVars", LiveVariablesDumper, DebugCheckers.cpp, "Print results of live variable analysis", -1, false)
CHECKER("optin.mpi.MPI-Checker", MPIChecker, MPIChecker.cpp, "Checks MPI code", -1, false)
CHECKER("osx.SecKeychainAPI", MacOSKeychainAPIChecker, MacOSKeychainAPIChecker.cpp, "Check for proper uses of Secure Keychain APIs", -1, false)
CHECKER("osx.API", MacOSXAPIChecker, MacOSXAPIChecker.cpp, "Check for proper uses of various Apple APIs", -1, false)
CHECKER("unix.Malloc", MallocChecker, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems. Traces memory managed by malloc()/free().", -1, false)
CHECKER("alpha.security.MallocOverflow", MallocOverflowSecurityChecker, MallocOverflowSecurityChecker.cpp, "Check for overflows in the arguments to malloc()", -1, true)
CHECKER("unix.MallocSizeof", MallocSizeofChecker, MallocSizeofChecker.cpp, "Check for dubious malloc arguments involving sizeof", -1, false)
CHECKER("unix.MismatchedDeallocator", MismatchedDeallocatorChecker, MallocChecker.cpp, "Check for mismatched deallocators.", -1, false)
CHECKER("alpha.osx.cocoa.MissingInvalidationMethod", MissingInvalidationMethod, IvarInvalidationChecker.cpp, "Check that the invalidation methods are present in classes that contain invalidatable instance variables", -1, true)
CHECKER("osx.cocoa.NSAutoreleasePool", NSAutoreleasePoolChecker, NSAutoreleasePoolChecker.cpp, "Warn for suboptimal uses of NSAutoreleasePool in Objective-C GC mode", -1, false)
CHECKER("osx.cocoa.NSError", NSErrorChecker, NSErrorChecker.cpp, "Check usage of NSError** parameters", -1, false)
CHECKER("cplusplus.NewDelete", NewDeleteChecker, MallocChecker.cpp, "Check for double-free and use-after-free problems. Traces memory managed by new/delete.", -1, false)
CHECKER("cplusplus.NewDeleteLeaks", NewDeleteLeaksChecker, MallocChecker.cpp, "Check for memory leaks. Traces memory managed by new/delete.", -1, false)
CHECKER("osx.cocoa.NilArg", NilArgChecker, BasicObjCFoundationChecks.cpp, "Check for prohibited nil arguments to ObjC method calls", -1, false)
CHECKER("core.builtin.NoReturnFunctions", NoReturnFunctionChecker, NoReturnFunctionChecker.cpp, "Evaluate \"panic\" functions that are known to not return to the caller", -1, false)
CHECKER("optin.osx.cocoa.localizability.NonLocalizedStringChecker", NonLocalizedStringChecker, LocalizationChecker.cpp, "Warns about uses of non-localized NSStrings passed to UI methods expecting localized NSStrings", -1, false)
CHECKER("core.NonNullParamChecker", NonNullParamChecker, NonNullParamChecker.cpp, "Check for null pointers passed as arguments to a function whose arguments are references or marked with the 'nonnull' attribute", -1, false)
CHECKER("nullability.NullPassedToNonnull", NullPassedToNonnullChecker, NullabilityChecker.cpp, "Warns when a null pointer is passed to a pointer which has a _Nonnull type.", -1, false)
CHECKER("nullability.NullReturnedFromNonnull", NullReturnedFromNonnullChecker, NullabilityChecker.cpp, "Warns when a null pointer is returned from a function that has _Nonnull return type.", -1, false)
CHECKER("nullability.NullableDereferenced", NullableDereferencedChecker, NullabilityChecker.cpp, "Warns when a nullable pointer is dereferenced.", -1, false)
CHECKER("nullability.NullablePassedToNonnull", NullablePassedToNonnullChecker, NullabilityChecker.cpp, "Warns when a nullable pointer is passed to a pointer which has a _Nonnull type.", -1, false)
CHECKER("nullability.NullableReturnedFromNonnull", NullableReturnedFromNonnullChecker, NullabilityChecker.cpp, "Warns when a nullable pointer is returned from a function that has _Nonnull return type.", -1, false)
CHECKER("osx.NumberObjectConversion", NumberObjectConversionChecker, NumberObjectConversionChecker.cpp, "Check for erroneous conversions of objects representing numbers into numbers", -1, false)
CHECKER("osx.cocoa.AtSync", ObjCAtSyncChecker, ObjCAtSyncChecker.cpp, "Check for nil pointers used as mutexes for @synchronized", -1, false)
CHECKER("osx.coreFoundation.containers.PointerSizedValues", ObjCContainersASTChecker, ObjCContainersASTChecker.cpp, "Warns if 'CFArray', 'CFDictionary', 'CFSet' are created with non-pointer-size values", -1, false)
CHECKER("osx.coreFoundation.containers.OutOfBounds", ObjCContainersChecker, ObjCContainersChecker.cpp, "Checks for index out-of-bounds when using 'CFArray' API", -1, false)
CHECKER("osx.cocoa.Dealloc", ObjCDeallocChecker, CheckObjCDealloc.cpp, "Warn about Objective-C classes that lack a correct implementation of -dealloc", -1, false)
CHECKER("osx.cocoa.ObjCGenerics", ObjCGenericsChecker, DynamicTypePropagation.cpp, "Check for type errors when using Objective-C generics", -1, false)
CHECKER("osx.cocoa.Loops", ObjCLoopChecker, BasicObjCFoundationChecks.cpp, "Improved modeling of loops using Cocoa collection types", -1, false)
CHECKER("osx.cocoa.IncompatibleMethodTypes", ObjCMethSigsChecker, CheckObjCInstMethSignature.cpp, "Warn about Objective-C method signatures with type incompatibilities", -1, false)
CHECKER("osx.cocoa.NonNilReturnValue", ObjCNonNilReturnValueChecker, BasicObjCFoundationChecks.cpp, "Model the APIs that are guaranteed to return a non-nil value", -1, false)
CHECKER("osx.ObjCProperty", ObjCPropertyChecker, ObjCPropertyChecker.cpp, "Check for proper uses of Objective-C properties", -1, false)
CHECKER("osx.cocoa.SelfInit", ObjCSelfInitChecker, ObjCSelfInitChecker.cpp, "Check that 'self' is properly initialized inside an initializer method", -1, false)
CHECKER("osx.cocoa.MissingSuperCall", ObjCSuperCallChecker, ObjCMissingSuperCallChecker.cpp, "Warn about Objective-C methods that lack a necessary call to super", -1, false)
CHECKER("osx.cocoa.SuperDealloc", ObjCSuperDeallocChecker, ObjCSuperDeallocChecker.cpp, "Warn about improper use of '[super dealloc]' in Objective-C", -1, false)
CHECKER("osx.cocoa.UnusedIvars", ObjCUnusedIvarsChecker, ObjCUnusedIVarsChecker.cpp, "Warn about private ivars that are never used", -1, false)
CHECKER("optin.performance.Padding", PaddingChecker, PaddingChecker.cpp, "Check for excessively padded structs.", -1, false)
CHECKER("alpha.osx.cocoa.localizability.PluralMisuseChecker", PluralMisuseChecker, LocalizationChecker.cpp, "Warns against using one vs. many plural pattern in code when generating localized strings.", -1, true)
CHECKER("alpha.core.PointerArithm", PointerArithChecker, PointerArithChecker, "Check for pointer arithmetic on locations other than array elements", -1, true)
CHECKER("alpha.core.PointerSub", PointerSubChecker, PointerSubChecker, "Check for pointer subtractions on two pointers pointing to different memory chunks", -1, true)
CHECKER("alpha.unix.PthreadLock", PthreadLockChecker, PthreadLockChecker.cpp, "Simple lock -> unlock checker", -1, true)
CHECKER("osx.cocoa.RetainCount", RetainCountChecker, RetainCountChecker.cpp, "Check for leaks and improper reference count management", -1, false)
CHECKER("alpha.security.ReturnPtrRange", ReturnPointerRangeChecker, ReturnPointerRangeChecker.cpp, "Check for an out-of-bound pointer being returned to callers", -1, true)
CHECKER("core.uninitialized.UndefReturn", ReturnUndefChecker, ReturnUndefChecker.cpp, "Check for uninitialized values being returned to the caller", -1, false)
CHECKER("alpha.unix.SimpleStream", SimpleStreamChecker, SimpleStreamChecker.cpp, "Check for misuses of stream APIs", -1, true)
CHECKER("alpha.core.SizeofPtr", SizeofPointerChecker, CheckSizeofPointer.cpp, "Warn about unintended use of sizeof() on pointer expressions", -1, true)
CHECKER("core.StackAddressEscape", StackAddrEscapeChecker, StackAddrEscapeChecker.cpp, "Check that addresses to stack memory do not escape the function", -1, false)
CHECKER("unix.StdCLibraryFunctions", StdCLibraryFunctionsChecker, StdLibraryFunctionsChecker.cpp, "Improve modeling of the C standard library functions", -1, false)
CHECKER("alpha.unix.Stream", StreamChecker, StreamChecker.cpp, "Check stream handling functions", -1, true)
CHECKER("debug.TaintTest", TaintTesterChecker, TaintTesterChecker.cpp, "Mark tainted symbols as such.", -1, false)
CHECKER("alpha.core.TestAfterDivZero", TestAfterDivZeroChecker, TestAfterDivZeroChecker.cpp, "Check for division by variable that is later compared against 0. Either the comparison is useless or there is division by zero.", -1, true)
CHECKER("debug.DumpTraversal", TraversalDumper, TraversalChecker.cpp, "Print branch conditions as they are traversed by the engine", -1, false)
CHECKER("security.insecureAPI.UncheckedReturn", UncheckedReturn, CheckSecuritySyntaxOnly.cpp, "Warn on uses of functions whose return values must be always checked", -1, false)
CHECKER("core.uninitialized.Branch", UndefBranchChecker, UndefBranchChecker.cpp, "Check for uninitialized values used as branch conditions", -1, false)
CHECKER("core.uninitialized.CapturedBlockVariable", UndefCapturedBlockVarChecker, UndefCapturedBlockVarChecker.cpp, "Check for blocks that capture uninitialized values", -1, false)
CHECKER("core.UndefinedBinaryOperatorResult", UndefResultChecker, UndefResultChecker.cpp, "Check for undefined results of binary operators", -1, false)
CHECKER("core.uninitialized.ArraySubscript", UndefinedArraySubscriptChecker, UndefinedArraySubscriptChecker.cpp, "Check for uninitialized values used as array subscripts", -1, false)
CHECKER("core.uninitialized.Assign", UndefinedAssignmentChecker, UndefinedAssignmentChecker.cpp, "Check for assigning uninitialized values", -1, false)
CHECKER("alpha.valist.Uninitialized", UninitializedChecker, ValistChecker.cpp, "Check for usages of uninitialized (or already released) va_lists.", -1, true)
CHECKER("unix.API", UnixAPIChecker, UnixAPIChecker.cpp, "Check calls to various UNIX/Posix functions", -1, false)
CHECKER("alpha.deadcode.UnreachableCode", UnreachableCodeChecker, UnreachableCodeChecker.cpp, "Check unreachable code", -1, true)
CHECKER("alpha.valist.Unterminated", UnterminatedChecker, ValistChecker.cpp, "Check for va_lists which are not released by a va_end call.", -1, true)
CHECKER("core.VLASize", VLASizeChecker, VLASizeChecker.cpp, "Check for declarations of VLA of undefined or zero size", -1, false)
CHECKER("osx.cocoa.VariadicMethodTypes", VariadicMethodTypeChecker, BasicObjCFoundationChecks.cpp, "Check for passing non-Objective-C types to variadic collection initialization methods that expect only Objective-C types", -1, false)
CHECKER("unix.Vfork", VforkChecker, VforkChecker.cpp, "Check for proper usage of vfork", -1, false)
CHECKER("optin.cplusplus.VirtualCall", VirtualCallChecker, VirtualCallChecker.cpp, "Check virtual function calls during construction or destruction", -1, false)
CHECKER("security.insecureAPI.getpw", getpw, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'getpw' function", -1, false)
CHECKER("security.insecureAPI.gets", gets, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'gets' function", -1, false)
CHECKER("security.insecureAPI.mkstemp", mkstemp, CheckSecuritySyntaxOnly.cpp, "Warn when 'mkstemp' is passed fewer than 6 X's in the format string", -1, false)
CHECKER("security.insecureAPI.mktemp", mktemp, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'mktemp' function", -1, false)
CHECKER("security.insecureAPI.rand", rand, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'rand', 'random', and related functions", -1, false)
CHECKER("security.insecureAPI.strcpy", strcpy, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'strcpy' and 'strcat' functions", -1, false)
CHECKER("security.insecureAPI.vfork", vfork, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'vfork' function", -1, false)
#endif // GET_CHECKERS


#ifdef GET_MEMBER_ARRAYS
static const short SubPackageArray0[] = { 3, 16, 45, 18, 28, 35, 20, 1, -1 };
static const short SubPackageArray1[] = { 2, -1 };
static const short CheckerArray2[] = { 28, -1 };
static const short SubPackageArray3[] = { 4, 6, 7, 8, 11, 10, 12, 13, 14, 5, 15, 9, -1 };
static const short CheckerArray4[] = { 5, -1 };
static const short CheckerArray5[] = { 20, -1 };
static const short CheckerArray6[] = { 24, -1 };
static const short CheckerArray7[] = { 25, -1 };
static const short CheckerArray8[] = { 30, -1 };
static const short CheckerArray9[] = { 38, -1 };
static const short CheckerArray10[] = { 43, -1 };
static const short CheckerArray11[] = { 47, -1 };
static const short CheckerArray12[] = { 89, -1 };
static const short CheckerArray13[] = { 90, -1 };
static const short CheckerArray14[] = { 96, -1 };
static const short CheckerArray15[] = { 101, -1 };
static const short SubPackageArray16[] = { 17, -1 };
static const short CheckerArray17[] = { 49, -1 };
static const short SubPackageArray18[] = { 19, -1 };
static const short CheckerArray19[] = { 111, -1 };
static const short SubPackageArray20[] = { 21, -1 };
static const short SubPackageArray21[] = { 26, 24, 25, 22, 23, -1 };
static const short CheckerArray22[] = { 34, -1 };
static const short CheckerArray23[] = { 35, -1 };
static const short CheckerArray24[] = { 48, -1 };
static const short CheckerArray25[] = { 59, -1 };
static const short SubPackageArray26[] = { 27, -1 };
static const short CheckerArray27[] = { 88, -1 };
static const short SubPackageArray28[] = { 33, 29, 30, 32, 31, -1 };
static const short CheckerArray29[] = { 2, -1 };
static const short CheckerArray30[] = { 3, -1 };
static const short CheckerArray31[] = { 56, -1 };
static const short CheckerArray32[] = { 93, -1 };
static const short SubPackageArray33[] = { 34, -1 };
static const short CheckerArray34[] = { 46, -1 };
static const short SubPackageArray35[] = { 41, 37, 38, 40, 39, 36, -1 };
static const short CheckerArray36[] = { 4, -1 };
static const short CheckerArray37[] = { 26, -1 };
static const short CheckerArray38[] = { 91, -1 };
static const short CheckerArray39[] = { 95, -1 };
static const short CheckerArray40[] = { 99, -1 };
static const short SubPackageArray41[] = { 44, 42, 43, -1 };
static const short CheckerArray42[] = { 13, -1 };
static const short CheckerArray43[] = { 14, -1 };
static const short CheckerArray44[] = { 16, -1 };
static const short SubPackageArray45[] = { 47, 48, 46, -1 };
static const short CheckerArray46[] = { 31, -1 };
static const short CheckerArray47[] = { 109, -1 };
static const short CheckerArray48[] = { 112, -1 };
static const short SubPackageArray49[] = { 50, -1 };
static const short SubPackageArray50[] = { 51, -1 };
static const short CheckerArray51[] = { 45, -1 };
static const short SubPackageArray52[] = { 61, 64, 57, 53, 56, 60, 54, 59, 58, 55, -1 };
static const short CheckerArray53[] = { 19, -1 };
static const short CheckerArray54[] = { 36, -1 };
static const short CheckerArray55[] = { 39, -1 };
static const short CheckerArray56[] = { 67, -1 };
static const short CheckerArray57[] = { 33, -1 };
static const short CheckerArray58[] = { 97, -1 };
static const short CheckerArray59[] = { 106, -1 };
static const short CheckerArray60[] = { 113, -1 };
static const short SubPackageArray61[] = { 63, 62, -1 };
static const short CheckerArray62[] = { 7, -1 };
static const short CheckerArray63[] = { 65, -1 };
static const short SubPackageArray64[] = { 65, 66, 67, 68, 69, -1 };
static const short CheckerArray65[] = { 107, -1 };
static const short CheckerArray66[] = { 108, -1 };
static const short CheckerArray67[] = { 104, -1 };
static const short CheckerArray68[] = { 105, -1 };
static const short CheckerArray69[] = { 94, -1 };
static const short SubPackageArray70[] = { 71, 72, 73, -1 };
static const short CheckerArray71[] = { 62, -1 };
static const short CheckerArray72[] = { 63, -1 };
static const short CheckerArray73[] = { 18, -1 };
static const short SubPackageArray74[] = { 75, -1 };
static const short CheckerArray75[] = { 32, -1 };
static const short SubPackageArray76[] = { 77, 83, 84, 89, 80, 90, 81, 78, 85, 82, 87, 88, 86, 91, 79, -1 };
static const short CheckerArray77[] = { 0, -1 };
static const short CheckerArray78[] = { 29, -1 };
static const short CheckerArray79[] = { 6, -1 };
static const short CheckerArray80[] = { 9, -1 };
static const short CheckerArray81[] = { 22, -1 };
static const short CheckerArray82[] = { 21, -1 };
static const short CheckerArray83[] = { 37, -1 };
static const short CheckerArray84[] = { 51, -1 };
static const short CheckerArray85[] = { 102, -1 };
static const short CheckerArray86[] = { 42, -1 };
static const short CheckerArray87[] = { 1, -1 };
static const short CheckerArray88[] = { 100, -1 };
static const short CheckerArray89[] = { 10, -1 };
static const short CheckerArray90[] = { 23, -1 };
static const short CheckerArray91[] = { 41, -1 };
static const short SubPackageArray92[] = { 93, -1 };
static const short CheckerArray93[] = { 50, -1 };
static const short SubPackageArray94[] = { 95, 96, 97, 98, 99, -1 };
static const short CheckerArray95[] = { 68, -1 };
static const short CheckerArray96[] = { 69, -1 };
static const short CheckerArray97[] = { 70, -1 };
static const short CheckerArray98[] = { 71, -1 };
static const short CheckerArray99[] = { 72, -1 };
static const short SubPackageArray100[] = { 101, 110, 105, 103, -1 };
static const short SubPackageArray101[] = { 102, -1 };
static const short CheckerArray102[] = { 116, -1 };
static const short SubPackageArray103[] = { 104, -1 };
static const short CheckerArray104[] = { 52, -1 };
static const short SubPackageArray105[] = { 106, -1 };
static const short SubPackageArray106[] = { 107, -1 };
static const short SubPackageArray107[] = { 109, 108, -1 };
static const short CheckerArray108[] = { 40, -1 };
static const short CheckerArray109[] = { 66, -1 };
static const short SubPackageArray110[] = { 111, -1 };
static const short CheckerArray111[] = { 87, -1 };
static const short SubPackageArray112[] = { 117, 134, 114, 113, 116, 115, -1 };
static const short CheckerArray113[] = { 54, -1 };
static const short CheckerArray114[] = { 73, -1 };
static const short CheckerArray115[] = { 82, -1 };
static const short CheckerArray116[] = { 53, -1 };
static const short SubPackageArray117[] = { 118, 126, 119, 133, 124, 121, 132, 130, 122, 127, 123, 125, 129, 128, 120, 131, -1 };
static const short CheckerArray118[] = { 74, -1 };
static const short CheckerArray119[] = { 27, -1 };
static const short CheckerArray120[] = { 77, -1 };
static const short CheckerArray121[] = { 80, -1 };
static const short CheckerArray122[] = { 79, -1 };
static const short CheckerArray123[] = { 84, -1 };
static const short CheckerArray124[] = { 60, -1 };
static const short CheckerArray125[] = { 61, -1 };
static const short CheckerArray126[] = { 64, -1 };
static const short CheckerArray127[] = { 81, -1 };
static const short CheckerArray128[] = { 78, -1 };
static const short CheckerArray129[] = { 92, -1 };
static const short CheckerArray130[] = { 83, -1 };
static const short CheckerArray131[] = { 85, -1 };
static const short CheckerArray132[] = { 86, -1 };
static const short CheckerArray133[] = { 114, -1 };
static const short SubPackageArray134[] = { 138, 136, 137, 135, -1 };
static const short CheckerArray135[] = { 8, -1 };
static const short CheckerArray136[] = { 11, -1 };
static const short CheckerArray137[] = { 12, -1 };
static const short SubPackageArray138[] = { 140, 139, -1 };
static const short CheckerArray139[] = { 76, -1 };
static const short CheckerArray140[] = { 75, -1 };
static const short SubPackageArray141[] = { 143, 142, -1 };
static const short CheckerArray142[] = { 44, -1 };
static const short SubPackageArray143[] = { 146, 145, 148, 147, 149, 150, 151, 144, -1 };
static const short CheckerArray144[] = { 103, -1 };
static const short CheckerArray145[] = { 117, -1 };
static const short CheckerArray146[] = { 118, -1 };
static const short CheckerArray147[] = { 119, -1 };
static const short CheckerArray148[] = { 120, -1 };
static const short CheckerArray149[] = { 121, -1 };
static const short CheckerArray150[] = { 122, -1 };
static const short CheckerArray151[] = { 123, -1 };
static const short SubPackageArray152[] = { 159, 153, 154, 155, 156, 158, 157, -1 };
static const short CheckerArray153[] = { 110, -1 };
static const short CheckerArray154[] = { 55, -1 };
static const short CheckerArray155[] = { 57, -1 };
static const short CheckerArray156[] = { 58, -1 };
static const short CheckerArray157[] = { 98, -1 };
static const short CheckerArray158[] = { 115, -1 };
static const short SubPackageArray159[] = { 161, 160, -1 };
static const short CheckerArray160[] = { 17, -1 };
static const short CheckerArray161[] = { 15, -1 };
#endif // GET_MEMBER_ARRAYS


#ifdef GET_CHECKNAME_TABLE
  { "alpha",                                                          0, SubPackageArray0, false },
  { "alpha.clone",                                                    0, SubPackageArray1, true },
  { "alpha.clone.CloneChecker",                                       CheckerArray2, 0, false },
  { "alpha.core",                                                     0, SubPackageArray3, true },
  { "alpha.core.BoolAssignment",                                      CheckerArray4, 0, false },
  { "alpha.core.CallAndMessageUnInitRefArg",                          CheckerArray5, 0, false },
  { "alpha.core.CastSize",                                            CheckerArray6, 0, false },
  { "alpha.core.CastToStruct",                                        CheckerArray7, 0, false },
  { "alpha.core.Conversion",                                          CheckerArray8, 0, false },
  { "alpha.core.DynamicTypeChecker",                                  CheckerArray9, 0, false },
  { "alpha.core.FixedAddr",                                           CheckerArray10, 0, false },
  { "alpha.core.IdenticalExpr",                                       CheckerArray11, 0, false },
  { "alpha.core.PointerArithm",                                       CheckerArray12, 0, false },
  { "alpha.core.PointerSub",                                          CheckerArray13, 0, false },
  { "alpha.core.SizeofPtr",                                           CheckerArray14, 0, false },
  { "alpha.core.TestAfterDivZero",                                    CheckerArray15, 0, false },
  { "alpha.cplusplus",                                                0, SubPackageArray16, true },
  { "alpha.cplusplus.IteratorPastEnd",                                CheckerArray17, 0, false },
  { "alpha.deadcode",                                                 0, SubPackageArray18, true },
  { "alpha.deadcode.UnreachableCode",                                 CheckerArray19, 0, false },
  { "alpha.osx",                                                      0, SubPackageArray20, true },
  { "alpha.osx.cocoa",                                                0, SubPackageArray21, true },
  { "alpha.osx.cocoa.DirectIvarAssignment",                           CheckerArray22, 0, false },
  { "alpha.osx.cocoa.DirectIvarAssignmentForAnnotatedFunctions",      CheckerArray23, 0, false },
  { "alpha.osx.cocoa.InstanceVariableInvalidation",                   CheckerArray24, 0, false },
  { "alpha.osx.cocoa.MissingInvalidationMethod",                      CheckerArray25, 0, false },
  { "alpha.osx.cocoa.localizability",                                 0, SubPackageArray26, true },
  { "alpha.osx.cocoa.localizability.PluralMisuseChecker",             CheckerArray27, 0, false },
  { "alpha.security",                                                 0, SubPackageArray28, true },
  { "alpha.security.ArrayBound",                                      CheckerArray29, 0, false },
  { "alpha.security.ArrayBoundV2",                                    CheckerArray30, 0, false },
  { "alpha.security.MallocOverflow",                                  CheckerArray31, 0, false },
  { "alpha.security.ReturnPtrRange",                                  CheckerArray32, 0, false },
  { "alpha.security.taint",                                           0, SubPackageArray33, true },
  { "alpha.security.taint.TaintPropagation",                          CheckerArray34, 0, false },
  { "alpha.unix",                                                     0, SubPackageArray35, true },
  { "alpha.unix.BlockInCriticalSection",                              CheckerArray36, 0, false },
  { "alpha.unix.Chroot",                                              CheckerArray37, 0, false },
  { "alpha.unix.PthreadLock",                                         CheckerArray38, 0, false },
  { "alpha.unix.SimpleStream",                                        CheckerArray39, 0, false },
  { "alpha.unix.Stream",                                              CheckerArray40, 0, false },
  { "alpha.unix.cstring",                                             0, SubPackageArray41, true },
  { "alpha.unix.cstring.BufferOverlap",                               CheckerArray42, 0, false },
  { "alpha.unix.cstring.NotNullTerminated",                           CheckerArray43, 0, false },
  { "alpha.unix.cstring.OutOfBounds",                                 CheckerArray44, 0, false },
  { "alpha.valist",                                                   0, SubPackageArray45, true },
  { "alpha.valist.CopyToSelf",                                        CheckerArray46, 0, false },
  { "alpha.valist.Uninitialized",                                     CheckerArray47, 0, false },
  { "alpha.valist.Unterminated",                                      CheckerArray48, 0, false },
  { "apiModeling",                                                    0, SubPackageArray49, true },
  { "apiModeling.google",                                             0, SubPackageArray50, true },
  { "apiModeling.google.GTest",                                       CheckerArray51, 0, false },
  { "core",                                                           0, SubPackageArray52, false },
  { "core.CallAndMessage",                                            CheckerArray53, 0, false },
  { "core.DivideZero",                                                CheckerArray54, 0, false },
  { "core.DynamicTypePropagation",                                    CheckerArray55, 0, false },
  { "core.NonNullParamChecker",                                       CheckerArray56, 0, false },
  { "core.NullDereference",                                           CheckerArray57, 0, false },
  { "core.StackAddressEscape",                                        CheckerArray58, 0, false },
  { "core.UndefinedBinaryOperatorResult",                             CheckerArray59, 0, false },
  { "core.VLASize",                                                   CheckerArray60, 0, false },
  { "core.builtin",                                                   0, SubPackageArray61, false },
  { "core.builtin.BuiltinFunctions",                                  CheckerArray62, 0, false },
  { "core.builtin.NoReturnFunctions",                                 CheckerArray63, 0, false },
  { "core.uninitialized",                                             0, SubPackageArray64, false },
  { "core.uninitialized.ArraySubscript",                              CheckerArray65, 0, false },
  { "core.uninitialized.Assign",                                      CheckerArray66, 0, false },
  { "core.uninitialized.Branch",                                      CheckerArray67, 0, false },
  { "core.uninitialized.CapturedBlockVariable",                       CheckerArray68, 0, false },
  { "core.uninitialized.UndefReturn",                                 CheckerArray69, 0, false },
  { "cplusplus",                                                      0, SubPackageArray70, false },
  { "cplusplus.NewDelete",                                            CheckerArray71, 0, false },
  { "cplusplus.NewDeleteLeaks",                                       CheckerArray72, 0, false },
  { "cplusplus.SelfAssignment",                                       CheckerArray73, 0, false },
  { "deadcode",                                                       0, SubPackageArray74, false },
  { "deadcode.DeadStores",                                            CheckerArray75, 0, false },
  { "debug",                                                          0, SubPackageArray76, false },
  { "debug.AnalysisOrder",                                            CheckerArray77, 0, false },
  { "debug.ConfigDumper",                                             CheckerArray78, 0, false },
  { "debug.DumpBugHash",                                              CheckerArray79, 0, false },
  { "debug.DumpCFG",                                                  CheckerArray80, 0, false },
  { "debug.DumpCallGraph",                                            CheckerArray81, 0, false },
  { "debug.DumpCalls",                                                CheckerArray82, 0, false },
  { "debug.DumpDominators",                                           CheckerArray83, 0, false },
  { "debug.DumpLiveVars",                                             CheckerArray84, 0, false },
  { "debug.DumpTraversal",                                            CheckerArray85, 0, false },
  { "debug.ExprInspection",                                           CheckerArray86, 0, false },
  { "debug.Stats",                                                    CheckerArray87, 0, false },
  { "debug.TaintTest",                                                CheckerArray88, 0, false },
  { "debug.ViewCFG",                                                  CheckerArray89, 0, false },
  { "debug.ViewCallGraph",                                            CheckerArray90, 0, false },
  { "debug.ViewExplodedGraph",                                        CheckerArray91, 0, false },
  { "llvm",                                                           0, SubPackageArray92, false },
  { "llvm.Conventions",                                               CheckerArray93, 0, false },
  { "nullability",                                                    0, SubPackageArray94, false },
  { "nullability.NullPassedToNonnull",                                CheckerArray95, 0, false },
  { "nullability.NullReturnedFromNonnull",                            CheckerArray96, 0, false },
  { "nullability.NullableDereferenced",                               CheckerArray97, 0, false },
  { "nullability.NullablePassedToNonnull",                            CheckerArray98, 0, false },
  { "nullability.NullableReturnedFromNonnull",                        CheckerArray99, 0, false },
  { "optin",                                                          0, SubPackageArray100, false },
  { "optin.cplusplus",                                                0, SubPackageArray101, false },
  { "optin.cplusplus.VirtualCall",                                    CheckerArray102, 0, false },
  { "optin.mpi",                                                      0, SubPackageArray103, false },
  { "optin.mpi.MPI-Checker",                                          CheckerArray104, 0, false },
  { "optin.osx",                                                      0, SubPackageArray105, false },
  { "optin.osx.cocoa",                                                0, SubPackageArray106, false },
  { "optin.osx.cocoa.localizability",                                 0, SubPackageArray107, false },
  { "optin.osx.cocoa.localizability.EmptyLocalizationContextChecker", CheckerArray108, 0, false },
  { "optin.osx.cocoa.localizability.NonLocalizedStringChecker",       CheckerArray109, 0, false },
  { "optin.performance",                                              0, SubPackageArray110, false },
  { "optin.performance.Padding",                                      CheckerArray111, 0, false },
  { "osx",                                                            0, SubPackageArray112, false },
  { "osx.API",                                                        CheckerArray113, 0, false },
  { "osx.NumberObjectConversion",                                     CheckerArray114, 0, false },
  { "osx.ObjCProperty",                                               CheckerArray115, 0, false },
  { "osx.SecKeychainAPI",                                             CheckerArray116, 0, false },
  { "osx.cocoa",                                                      0, SubPackageArray117, false },
  { "osx.cocoa.AtSync",                                               CheckerArray118, 0, false },
  { "osx.cocoa.ClassRelease",                                         CheckerArray119, 0, false },
  { "osx.cocoa.Dealloc",                                              CheckerArray120, 0, false },
  { "osx.cocoa.IncompatibleMethodTypes",                              CheckerArray121, 0, false },
  { "osx.cocoa.Loops",                                                CheckerArray122, 0, false },
  { "osx.cocoa.MissingSuperCall",                                     CheckerArray123, 0, false },
  { "osx.cocoa.NSAutoreleasePool",                                    CheckerArray124, 0, false },
  { "osx.cocoa.NSError",                                              CheckerArray125, 0, false },
  { "osx.cocoa.NilArg",                                               CheckerArray126, 0, false },
  { "osx.cocoa.NonNilReturnValue",                                    CheckerArray127, 0, false },
  { "osx.cocoa.ObjCGenerics",                                         CheckerArray128, 0, false },
  { "osx.cocoa.RetainCount",                                          CheckerArray129, 0, false },
  { "osx.cocoa.SelfInit",                                             CheckerArray130, 0, false },
  { "osx.cocoa.SuperDealloc",                                         CheckerArray131, 0, false },
  { "osx.cocoa.UnusedIvars",                                          CheckerArray132, 0, false },
  { "osx.cocoa.VariadicMethodTypes",                                  CheckerArray133, 0, false },
  { "osx.coreFoundation",                                             0, SubPackageArray134, false },
  { "osx.coreFoundation.CFError",                                     CheckerArray135, 0, false },
  { "osx.coreFoundation.CFNumber",                                    CheckerArray136, 0, false },
  { "osx.coreFoundation.CFRetainRelease",                             CheckerArray137, 0, false },
  { "osx.coreFoundation.containers",                                  0, SubPackageArray138, false },
  { "osx.coreFoundation.containers.OutOfBounds",                      CheckerArray139, 0, false },
  { "osx.coreFoundation.containers.PointerSizedValues",               CheckerArray140, 0, false },
  { "security",                                                       0, SubPackageArray141, false },
  { "security.FloatLoopCounter",                                      CheckerArray142, 0, false },
  { "security.insecureAPI",                                           0, SubPackageArray143, false },
  { "security.insecureAPI.UncheckedReturn",                           CheckerArray144, 0, false },
  { "security.insecureAPI.getpw",                                     CheckerArray145, 0, false },
  { "security.insecureAPI.gets",                                      CheckerArray146, 0, false },
  { "security.insecureAPI.mkstemp",                                   CheckerArray147, 0, false },
  { "security.insecureAPI.mktemp",                                    CheckerArray148, 0, false },
  { "security.insecureAPI.rand",                                      CheckerArray149, 0, false },
  { "security.insecureAPI.strcpy",                                    CheckerArray150, 0, false },
  { "security.insecureAPI.vfork",                                     CheckerArray151, 0, false },
  { "unix",                                                           0, SubPackageArray152, false },
  { "unix.API",                                                       CheckerArray153, 0, false },
  { "unix.Malloc",                                                    CheckerArray154, 0, false },
  { "unix.MallocSizeof",                                              CheckerArray155, 0, false },
  { "unix.MismatchedDeallocator",                                     CheckerArray156, 0, false },
  { "unix.StdCLibraryFunctions",                                      CheckerArray157, 0, false },
  { "unix.Vfork",                                                     CheckerArray158, 0, false },
  { "unix.cstring",                                                   0, SubPackageArray159, true },
  { "unix.cstring.BadSizeArg",                                        CheckerArray160, 0, false },
  { "unix.cstring.NullArg",                                           CheckerArray161, 0, false },
  { "valist",                                                         0, 0, false },
#endif // GET_CHECKNAME_TABLE

