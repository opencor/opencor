
#ifdef GET_GROUPS
#endif // GET_GROUPS


#ifdef GET_PACKAGES
PACKAGE("alpha", -1, false)
PACKAGE("alpha.core", -1, true)
PACKAGE("alpha.cplusplus", -1, true)
PACKAGE("alpha.deadcode", -1, true)
PACKAGE("alpha.osx", -1, true)
PACKAGE("alpha.osx.cocoa", -1, true)
PACKAGE("alpha.security", -1, true)
PACKAGE("alpha.security.taint", -1, true)
PACKAGE("alpha.unix", -1, true)
PACKAGE("alpha.unix.cstring", -1, true)
PACKAGE("core", -1, false)
PACKAGE("core.builtin", -1, false)
PACKAGE("core.uninitialized", -1, false)
PACKAGE("cplusplus", -1, false)
PACKAGE("deadcode", -1, false)
PACKAGE("debug", -1, false)
PACKAGE("llvm", -1, false)
PACKAGE("osx", -1, false)
PACKAGE("osx.cocoa", -1, false)
PACKAGE("osx.coreFoundation", -1, false)
PACKAGE("osx.coreFoundation.containers", -1, false)
PACKAGE("security", -1, false)
PACKAGE("security.insecureAPI", -1, false)
PACKAGE("unix", -1, false)
PACKAGE("unix.cstring", -1, true)
#endif // GET_PACKAGES


#ifdef GET_CHECKERS
CHECKER("debug.Stats", AnalyzerStatsChecker, AnalyzerStatsChecker.cpp, "Emit warnings with analyzer statistics", -1, false)
CHECKER("alpha.security.ArrayBound", ArrayBoundChecker, ArrayBoundChecker.cpp, "Warn about buffer overflows (older checker)", -1, true)
CHECKER("alpha.security.ArrayBoundV2", ArrayBoundCheckerV2, ArrayBoundCheckerV2.cpp, "Warn about buffer overflows (newer checker)", -1, true)
CHECKER("alpha.core.BoolAssignment", BoolAssignmentChecker, BoolAssignmentChecker.cpp, "Warn about assigning non-{0,1} values to Boolean variables", -1, true)
CHECKER("core.builtin.BuiltinFunctions", BuiltinFunctionChecker, BuiltinFunctionChecker.cpp, "Evaluate compiler builtin functions (e.g., alloca())", -1, false)
CHECKER("osx.coreFoundation.CFError", CFErrorChecker, NSErrorChecker.cpp, "Check usage of CFErrorRef* parameters", -1, false)
CHECKER("debug.DumpCFG", CFGDumper, DebugCheckers.cpp, "Display Control-Flow Graphs", -1, false)
CHECKER("debug.ViewCFG", CFGViewer, DebugCheckers.cpp, "View Control-Flow Graphs using GraphViz", -1, false)
CHECKER("osx.coreFoundation.CFNumber", CFNumberCreateChecker, BasicObjCFoundationChecks.cpp, "Check for proper uses of CFNumberCreate", -1, false)
CHECKER("osx.coreFoundation.CFRetainRelease", CFRetainReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for null arguments to CFRetain/CFRelease/CFMakeCollectable", -1, false)
CHECKER("alpha.unix.cstring.BufferOverlap", CStringBufferOverlap, CStringChecker.cpp, "Checks for overlap in two buffer arguments", -1, true)
CHECKER("alpha.unix.cstring.NotNullTerminated", CStringNotNullTerm, CStringChecker.cpp, "Check for arguments which are not null-terminating strings", -1, true)
CHECKER("unix.cstring.NullArg", CStringNullArg, CStringChecker.cpp, "Check for null pointers being passed as arguments to C string functions", -1, true)
CHECKER("alpha.unix.cstring.OutOfBounds", CStringOutOfBounds, CStringChecker.cpp, "Check for out-of-bounds access in string functions", -1, true)
CHECKER("unix.cstring.BadSizeArg", CStringSyntaxChecker, CStringSyntaxChecker.cpp, "Check the size argument passed into C string functions for common erroneous patterns", -1, true)
CHECKER("core.CallAndMessage", CallAndMessageChecker, CallAndMessageChecker.cpp, "Check for logical errors for function calls and Objective-C message expressions (e.g., uninitialized arguments, null function pointers)", -1, false)
CHECKER("debug.DumpCalls", CallDumper, TraversalChecker.cpp, "Print calls as they are traversed by the engine", -1, false)
CHECKER("debug.DumpCallGraph", CallGraphDumper, DebugCheckers.cpp, "Display Call Graph", -1, false)
CHECKER("debug.ViewCallGraph", CallGraphViewer, DebugCheckers.cpp, "View Call Graph using GraphViz", -1, false)
CHECKER("alpha.core.CastSize", CastSizeChecker, CastSizeChecker.cpp, "Check when casting a malloc'ed type T, whether the size is a multiple of the size of T", -1, true)
CHECKER("alpha.core.CastToStruct", CastToStructChecker, CastToStructChecker.cpp, "Check for cast from non-struct pointer to struct pointer", -1, true)
CHECKER("alpha.unix.Chroot", ChrootChecker, ChrootChecker.cpp, "Check improper use of chroot", -1, true)
CHECKER("osx.cocoa.ClassRelease", ClassReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for sending 'retain', 'release', or 'autorelease' directly to a Class", -1, false)
CHECKER("debug.ConfigDumper", ConfigDumper, DebugCheckers.cpp, "Dump config table", -1, false)
CHECKER("deadcode.DeadStores", DeadStoresChecker, DeadStoresChecker.cpp, "Check for values stored to variables that are never read afterwards", -1, false)
CHECKER("core.NullDereference", DereferenceChecker, DereferenceChecker.cpp, "Check for dereferences of null pointers", -1, false)
CHECKER("alpha.osx.cocoa.DirectIvarAssignment", DirectIvarAssignment, DirectIvarAssignment.cpp, "Check for direct assignments to instance variables", -1, true)
CHECKER("alpha.osx.cocoa.DirectIvarAssignmentForAnnotatedFunctions", DirectIvarAssignmentForAnnotatedFunctions, DirectIvarAssignment.cpp, "Check for direct assignments to instance variables in the methods annotated with objc_no_direct_instance_variable_assignment", -1, true)
CHECKER("core.DivideZero", DivZeroChecker, DivZeroChecker.cpp, "Check for division by zero", -1, false)
CHECKER("debug.DumpDominators", DominatorsTreeDumper, DebugCheckers.cpp, "Print the dominance tree for a given CFG", -1, false)
CHECKER("core.DynamicTypePropagation", DynamicTypePropagation, DynamicTypePropagation.cpp, "Generate dynamic type information", -1, false)
CHECKER("debug.ViewExplodedGraph", ExplodedGraphViewer, DebugCheckers.cpp, "View Exploded Graphs using GraphViz", -1, false)
CHECKER("debug.ExprInspection", ExprInspectionChecker, ExprInspectionChecker.cpp, "Check the analyzer's understanding of expressions", -1, false)
CHECKER("alpha.core.FixedAddr", FixedAddressChecker, FixedAddressChecker.cpp, "Check for assignment of a fixed address to a pointer", -1, true)
CHECKER("security.FloatLoopCounter", FloatLoopCounter, CheckSecuritySyntaxOnly.cpp, "Warn on using a floating point value as a loop counter (CERT: FLP30-C, FLP30-CPP)", -1, false)
CHECKER("alpha.security.taint.TaintPropagation", GenericTaintChecker, GenericTaintChecker.cpp, "Generate taint information used by other checkers", -1, true)
CHECKER("alpha.deadcode.IdempotentOperations", IdempotentOperationChecker, IdempotentOperationChecker.cpp, "Warn about idempotent operations", -1, true)
CHECKER("alpha.core.IdenticalExpr", IdenticalExprChecker, IdenticalExprChecker.cpp, "Warn about unintended use of identical expressions in operators", -1, true)
CHECKER("alpha.osx.cocoa.InstanceVariableInvalidation", InstanceVariableInvalidation, IvarInvalidationChecker.cpp, "Check that the invalidatable instance variables are invalidated in the methods annotated with objc_instance_variable_invalidator", -1, true)
CHECKER("llvm.Conventions", LLVMConventionsChecker, LLVMConventionsChecker.cpp, "Check code for LLVM codebase conventions", -1, false)
CHECKER("debug.DumpLiveVars", LiveVariablesDumper, DebugCheckers.cpp, "Print results of live variable analysis", -1, false)
CHECKER("osx.SecKeychainAPI", MacOSKeychainAPIChecker, MacOSKeychainAPIChecker.cpp, "Check for proper uses of Secure Keychain APIs", -1, false)
CHECKER("osx.API", MacOSXAPIChecker, MacOSXAPIChecker.cpp, "Check for proper uses of various Apple APIs", -1, false)
CHECKER("alpha.unix.MallocWithAnnotations", MallocOptimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems. Traces memory managed by malloc()/free(). Assumes that all user-defined functions which might free a pointer are annotated.", -1, true)
CHECKER("alpha.security.MallocOverflow", MallocOverflowSecurityChecker, MallocOverflowSecurityChecker.cpp, "Check for overflows in the arguments to malloc()", -1, true)
CHECKER("unix.Malloc", MallocPessimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems. Traces memory managed by malloc()/free().", -1, false)
CHECKER("unix.MallocSizeof", MallocSizeofChecker, MallocSizeofChecker.cpp, "Check for dubious malloc arguments involving sizeof", -1, false)
CHECKER("unix.MismatchedDeallocator", MismatchedDeallocatorChecker, MallocChecker.cpp, "Check for mismatched deallocators.", -1, false)
CHECKER("alpha.osx.cocoa.MissingInvalidationMethod", MissingInvalidationMethod, IvarInvalidationChecker.cpp, "Check that the invalidation methods are present in classes that contain invalidatable instance variables", -1, true)
CHECKER("osx.cocoa.NSAutoreleasePool", NSAutoreleasePoolChecker, NSAutoreleasePoolChecker.cpp, "Warn for suboptimal uses of NSAutoreleasePool in Objective-C GC mode", -1, false)
CHECKER("osx.cocoa.NSError", NSErrorChecker, NSErrorChecker.cpp, "Check usage of NSError** parameters", -1, false)
CHECKER("cplusplus.NewDelete", NewDeleteChecker, MallocChecker.cpp, "Check for double-free and use-after-free problems. Traces memory managed by new/delete.", -1, false)
CHECKER("alpha.cplusplus.NewDeleteLeaks", NewDeleteLeaksChecker, MallocChecker.cpp, "Check for memory leaks. Traces memory managed by new/delete.", -1, true)
CHECKER("osx.cocoa.NilArg", NilArgChecker, BasicObjCFoundationChecks.cpp, "Check for prohibited nil arguments to ObjC method calls", -1, false)
CHECKER("core.builtin.NoReturnFunctions", NoReturnFunctionChecker, NoReturnFunctionChecker.cpp, "Evaluate \"panic\" functions that are known to not return to the caller", -1, false)
CHECKER("core.NonNullParamChecker", NonNullParamChecker, NonNullParamChecker.cpp, "Check for null pointers passed as arguments to a function whose arguments are references or marked with the 'nonnull' attribute", -1, false)
CHECKER("osx.cocoa.AtSync", ObjCAtSyncChecker, ObjCAtSyncChecker.cpp, "Check for nil pointers used as mutexes for @synchronized", -1, false)
CHECKER("osx.coreFoundation.containers.PointerSizedValues", ObjCContainersASTChecker, ObjCContainersASTChecker.cpp, "Warns if 'CFArray', 'CFDictionary', 'CFSet' are created with non-pointer-size values", -1, false)
CHECKER("osx.coreFoundation.containers.OutOfBounds", ObjCContainersChecker, ObjCContainersChecker.cpp, "Checks for index out-of-bounds when using 'CFArray' API", -1, false)
CHECKER("alpha.osx.cocoa.Dealloc", ObjCDeallocChecker, CheckObjCDealloc.cpp, "Warn about Objective-C classes that lack a correct implementation of -dealloc", -1, true)
CHECKER("osx.cocoa.Loops", ObjCLoopChecker, BasicObjCFoundationChecks.cpp, "Improved modeling of loops using Cocoa collection types", -1, false)
CHECKER("osx.cocoa.IncompatibleMethodTypes", ObjCMethSigsChecker, CheckObjCInstMethSignature.cpp, "Warn about Objective-C method signatures with type incompatibilities", -1, false)
CHECKER("osx.cocoa.NonNilReturnValue", ObjCNonNilReturnValueChecker, BasicObjCFoundationChecks.cpp, "Model the APIs that are guaranteed to return a non-nil value", -1, false)
CHECKER("osx.cocoa.SelfInit", ObjCSelfInitChecker, ObjCSelfInitChecker.cpp, "Check that 'self' is properly initialized inside an initializer method", -1, false)
CHECKER("alpha.osx.cocoa.MissingSuperCall", ObjCSuperCallChecker, ObjCMissingSuperCallChecker.cpp, "Warn about Objective-C methods that lack a necessary call to super", -1, true)
CHECKER("osx.cocoa.UnusedIvars", ObjCUnusedIvarsChecker, ObjCUnusedIVarsChecker.cpp, "Warn about private ivars that are never used", -1, false)
CHECKER("alpha.core.PointerArithm", PointerArithChecker, PointerArithChecker, "Check for pointer arithmetic on locations other than array elements", -1, true)
CHECKER("alpha.core.PointerSub", PointerSubChecker, PointerSubChecker, "Check for pointer subtractions on two pointers pointing to different memory chunks", -1, true)
CHECKER("alpha.unix.PthreadLock", PthreadLockChecker, PthreadLockChecker.cpp, "Simple lock -> unlock checker", -1, true)
CHECKER("osx.cocoa.RetainCount", RetainCountChecker, RetainCountChecker.cpp, "Check for leaks and improper reference count management", -1, false)
CHECKER("alpha.security.ReturnPtrRange", ReturnPointerRangeChecker, ReturnPointerRangeChecker.cpp, "Check for an out-of-bound pointer being returned to callers", -1, true)
CHECKER("core.uninitialized.UndefReturn", ReturnUndefChecker, ReturnUndefChecker.cpp, "Check for uninitialized values being returned to the caller", -1, false)
CHECKER("alpha.unix.SimpleStream", SimpleStreamChecker, SimpleStreamChecker.cpp, "Check for misuses of stream APIs", -1, true)
CHECKER("alpha.core.SizeofPtr", SizeofPointerChecker, CheckSizeofPointer.cpp, "Warn about unintended use of sizeof() on pointer expressions", -1, true)
CHECKER("core.StackAddressEscape", StackAddrEscapeChecker, StackAddrEscapeChecker.cpp, "Check that addresses to stack memory do not escape the function", -1, false)
CHECKER("alpha.unix.Stream", StreamChecker, StreamChecker.cpp, "Check stream handling functions", -1, true)
CHECKER("debug.TaintTest", TaintTesterChecker, TaintTesterChecker.cpp, "Mark tainted symbols as such.", -1, false)
CHECKER("debug.DumpTraversal", TraversalDumper, TraversalChecker.cpp, "Print branch conditions as they are traversed by the engine", -1, false)
CHECKER("security.insecureAPI.UncheckedReturn", UncheckedReturn, CheckSecuritySyntaxOnly.cpp, "Warn on uses of functions whose return values must be always checked", -1, false)
CHECKER("core.uninitialized.Branch", UndefBranchChecker, UndefBranchChecker.cpp, "Check for uninitialized values used as branch conditions", -1, false)
CHECKER("core.uninitialized.CapturedBlockVariable", UndefCapturedBlockVarChecker, UndefCapturedBlockVarChecker.cpp, "Check for blocks that capture uninitialized values", -1, false)
CHECKER("core.UndefinedBinaryOperatorResult", UndefResultChecker, UndefResultChecker.cpp, "Check for undefined results of binary operators", -1, false)
CHECKER("core.uninitialized.ArraySubscript", UndefinedArraySubscriptChecker, UndefinedArraySubscriptChecker.cpp, "Check for uninitialized values used as array subscripts", -1, false)
CHECKER("core.uninitialized.Assign", UndefinedAssignmentChecker, UndefinedAssignmentChecker.cpp, "Check for assigning uninitialized values", -1, false)
CHECKER("unix.API", UnixAPIChecker, UnixAPIChecker.cpp, "Check calls to various UNIX/Posix functions", -1, false)
CHECKER("alpha.deadcode.UnreachableCode", UnreachableCodeChecker, UnreachableCodeChecker.cpp, "Check unreachable code", -1, true)
CHECKER("core.VLASize", VLASizeChecker, VLASizeChecker.cpp, "Check for declarations of VLA of undefined or zero size", -1, false)
CHECKER("osx.cocoa.VariadicMethodTypes", VariadicMethodTypeChecker, BasicObjCFoundationChecks.cpp, "Check for passing non-Objective-C types to variadic collection initialization methods that expect only Objective-C types", -1, false)
CHECKER("alpha.cplusplus.VirtualCall", VirtualCallChecker, VirtualCallChecker.cpp, "Check virtual function calls during construction or destruction", -1, true)
CHECKER("security.insecureAPI.getpw", getpw, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'getpw' function", -1, false)
CHECKER("security.insecureAPI.gets", gets, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'gets' function", -1, false)
CHECKER("security.insecureAPI.mkstemp", mkstemp, CheckSecuritySyntaxOnly.cpp, "Warn when 'mkstemp' is passed fewer than 6 X's in the format string", -1, false)
CHECKER("security.insecureAPI.mktemp", mktemp, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'mktemp' function", -1, false)
CHECKER("security.insecureAPI.rand", rand, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'rand', 'random', and related functions", -1, false)
CHECKER("security.insecureAPI.strcpy", strcpy, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'strcpy' and 'strcat' functions", -1, false)
CHECKER("security.insecureAPI.vfork", vfork, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'vfork' function", -1, false)
#endif // GET_CHECKERS


#ifdef GET_MEMBER_ARRAYS
static const short SubPackageArray0[] = { 1, 10, 13, 24, 31, 16, -1 };
static const short SubPackageArray1[] = { 2, 3, 4, 6, 5, 7, 8, 9, -1 };
static const short CheckerArray2[] = { 3, -1 };
static const short CheckerArray3[] = { 19, -1 };
static const short CheckerArray4[] = { 20, -1 };
static const short CheckerArray5[] = { 33, -1 };
static const short CheckerArray6[] = { 37, -1 };
static const short CheckerArray7[] = { 66, -1 };
static const short CheckerArray8[] = { 67, -1 };
static const short CheckerArray9[] = { 73, -1 };
static const short SubPackageArray10[] = { 12, 11, -1 };
static const short CheckerArray11[] = { 52, -1 };
static const short CheckerArray12[] = { 88, -1 };
static const short SubPackageArray13[] = { 14, 15, -1 };
static const short CheckerArray14[] = { 36, -1 };
static const short CheckerArray15[] = { 85, -1 };
static const short SubPackageArray16[] = { 17, -1 };
static const short SubPackageArray17[] = { 18, 21, 22, 19, 20, 23, -1 };
static const short CheckerArray18[] = { 59, -1 };
static const short CheckerArray19[] = { 26, -1 };
static const short CheckerArray20[] = { 27, -1 };
static const short CheckerArray21[] = { 38, -1 };
static const short CheckerArray22[] = { 48, -1 };
static const short CheckerArray23[] = { 64, -1 };
static const short SubPackageArray24[] = { 29, 25, 26, 28, 27, -1 };
static const short CheckerArray25[] = { 1, -1 };
static const short CheckerArray26[] = { 2, -1 };
static const short CheckerArray27[] = { 44, -1 };
static const short CheckerArray28[] = { 70, -1 };
static const short SubPackageArray29[] = { 30, -1 };
static const short CheckerArray30[] = { 35, -1 };
static const short SubPackageArray31[] = { 37, 32, 33, 34, 36, 35, -1 };
static const short CheckerArray32[] = { 21, -1 };
static const short CheckerArray33[] = { 43, -1 };
static const short CheckerArray34[] = { 68, -1 };
static const short CheckerArray35[] = { 72, -1 };
static const short CheckerArray36[] = { 75, -1 };
static const short SubPackageArray37[] = { 40, 38, 39, -1 };
static const short CheckerArray38[] = { 10, -1 };
static const short CheckerArray39[] = { 11, -1 };
static const short CheckerArray40[] = { 13, -1 };
static const short SubPackageArray41[] = { 50, 53, 46, 42, 45, 49, 43, 48, 47, 44, -1 };
static const short CheckerArray42[] = { 15, -1 };
static const short CheckerArray43[] = { 28, -1 };
static const short CheckerArray44[] = { 30, -1 };
static const short CheckerArray45[] = { 55, -1 };
static const short CheckerArray46[] = { 25, -1 };
static const short CheckerArray47[] = { 74, -1 };
static const short CheckerArray48[] = { 81, -1 };
static const short CheckerArray49[] = { 86, -1 };
static const short SubPackageArray50[] = { 52, 51, -1 };
static const short CheckerArray51[] = { 4, -1 };
static const short CheckerArray52[] = { 54, -1 };
static const short SubPackageArray53[] = { 54, 55, 56, 57, 58, -1 };
static const short CheckerArray54[] = { 82, -1 };
static const short CheckerArray55[] = { 83, -1 };
static const short CheckerArray56[] = { 79, -1 };
static const short CheckerArray57[] = { 80, -1 };
static const short CheckerArray58[] = { 71, -1 };
static const short SubPackageArray59[] = { 60, -1 };
static const short CheckerArray60[] = { 51, -1 };
static const short SubPackageArray61[] = { 62, -1 };
static const short CheckerArray62[] = { 24, -1 };
static const short SubPackageArray63[] = { 68, 69, 74, 65, 75, 66, 64, 70, 67, 72, 73, 71, 76, -1 };
static const short CheckerArray64[] = { 23, -1 };
static const short CheckerArray65[] = { 6, -1 };
static const short CheckerArray66[] = { 17, -1 };
static const short CheckerArray67[] = { 16, -1 };
static const short CheckerArray68[] = { 29, -1 };
static const short CheckerArray69[] = { 40, -1 };
static const short CheckerArray70[] = { 77, -1 };
static const short CheckerArray71[] = { 32, -1 };
static const short CheckerArray72[] = { 0, -1 };
static const short CheckerArray73[] = { 76, -1 };
static const short CheckerArray74[] = { 7, -1 };
static const short CheckerArray75[] = { 18, -1 };
static const short CheckerArray76[] = { 31, -1 };
static const short SubPackageArray77[] = { 78, -1 };
static const short CheckerArray78[] = { 39, -1 };
static const short SubPackageArray79[] = { 82, 95, 80, 81, -1 };
static const short CheckerArray80[] = { 42, -1 };
static const short CheckerArray81[] = { 41, -1 };
static const short SubPackageArray82[] = { 83, 89, 84, 94, 87, 85, 93, 92, 86, 90, 88, 91, -1 };
static const short CheckerArray83[] = { 56, -1 };
static const short CheckerArray84[] = { 22, -1 };
static const short CheckerArray85[] = { 61, -1 };
static const short CheckerArray86[] = { 60, -1 };
static const short CheckerArray87[] = { 49, -1 };
static const short CheckerArray88[] = { 50, -1 };
static const short CheckerArray89[] = { 53, -1 };
static const short CheckerArray90[] = { 62, -1 };
static const short CheckerArray91[] = { 69, -1 };
static const short CheckerArray92[] = { 63, -1 };
static const short CheckerArray93[] = { 65, -1 };
static const short CheckerArray94[] = { 87, -1 };
static const short SubPackageArray95[] = { 99, 97, 98, 96, -1 };
static const short CheckerArray96[] = { 5, -1 };
static const short CheckerArray97[] = { 8, -1 };
static const short CheckerArray98[] = { 9, -1 };
static const short SubPackageArray99[] = { 101, 100, -1 };
static const short CheckerArray100[] = { 58, -1 };
static const short CheckerArray101[] = { 57, -1 };
static const short SubPackageArray102[] = { 104, 103, -1 };
static const short CheckerArray103[] = { 34, -1 };
static const short SubPackageArray104[] = { 107, 106, 109, 108, 110, 111, 112, 105, -1 };
static const short CheckerArray105[] = { 78, -1 };
static const short CheckerArray106[] = { 89, -1 };
static const short CheckerArray107[] = { 90, -1 };
static const short CheckerArray108[] = { 91, -1 };
static const short CheckerArray109[] = { 92, -1 };
static const short CheckerArray110[] = { 93, -1 };
static const short CheckerArray111[] = { 94, -1 };
static const short CheckerArray112[] = { 95, -1 };
static const short SubPackageArray113[] = { 118, 114, 115, 116, 117, -1 };
static const short CheckerArray114[] = { 84, -1 };
static const short CheckerArray115[] = { 45, -1 };
static const short CheckerArray116[] = { 46, -1 };
static const short CheckerArray117[] = { 47, -1 };
static const short SubPackageArray118[] = { 120, 119, -1 };
static const short CheckerArray119[] = { 14, -1 };
static const short CheckerArray120[] = { 12, -1 };
#endif // GET_MEMBER_ARRAYS


#ifdef GET_CHECKNAME_TABLE
  { "alpha",                                                     0, SubPackageArray0, false },
  { "alpha.core",                                                0, SubPackageArray1, true },
  { "alpha.core.BoolAssignment",                                 CheckerArray2, 0, false },
  { "alpha.core.CastSize",                                       CheckerArray3, 0, false },
  { "alpha.core.CastToStruct",                                   CheckerArray4, 0, false },
  { "alpha.core.FixedAddr",                                      CheckerArray5, 0, false },
  { "alpha.core.IdenticalExpr",                                  CheckerArray6, 0, false },
  { "alpha.core.PointerArithm",                                  CheckerArray7, 0, false },
  { "alpha.core.PointerSub",                                     CheckerArray8, 0, false },
  { "alpha.core.SizeofPtr",                                      CheckerArray9, 0, false },
  { "alpha.cplusplus",                                           0, SubPackageArray10, true },
  { "alpha.cplusplus.NewDeleteLeaks",                            CheckerArray11, 0, false },
  { "alpha.cplusplus.VirtualCall",                               CheckerArray12, 0, false },
  { "alpha.deadcode",                                            0, SubPackageArray13, true },
  { "alpha.deadcode.IdempotentOperations",                       CheckerArray14, 0, false },
  { "alpha.deadcode.UnreachableCode",                            CheckerArray15, 0, false },
  { "alpha.osx",                                                 0, SubPackageArray16, true },
  { "alpha.osx.cocoa",                                           0, SubPackageArray17, true },
  { "alpha.osx.cocoa.Dealloc",                                   CheckerArray18, 0, false },
  { "alpha.osx.cocoa.DirectIvarAssignment",                      CheckerArray19, 0, false },
  { "alpha.osx.cocoa.DirectIvarAssignmentForAnnotatedFunctions", CheckerArray20, 0, false },
  { "alpha.osx.cocoa.InstanceVariableInvalidation",              CheckerArray21, 0, false },
  { "alpha.osx.cocoa.MissingInvalidationMethod",                 CheckerArray22, 0, false },
  { "alpha.osx.cocoa.MissingSuperCall",                          CheckerArray23, 0, false },
  { "alpha.security",                                            0, SubPackageArray24, true },
  { "alpha.security.ArrayBound",                                 CheckerArray25, 0, false },
  { "alpha.security.ArrayBoundV2",                               CheckerArray26, 0, false },
  { "alpha.security.MallocOverflow",                             CheckerArray27, 0, false },
  { "alpha.security.ReturnPtrRange",                             CheckerArray28, 0, false },
  { "alpha.security.taint",                                      0, SubPackageArray29, true },
  { "alpha.security.taint.TaintPropagation",                     CheckerArray30, 0, false },
  { "alpha.unix",                                                0, SubPackageArray31, true },
  { "alpha.unix.Chroot",                                         CheckerArray32, 0, false },
  { "alpha.unix.MallocWithAnnotations",                          CheckerArray33, 0, false },
  { "alpha.unix.PthreadLock",                                    CheckerArray34, 0, false },
  { "alpha.unix.SimpleStream",                                   CheckerArray35, 0, false },
  { "alpha.unix.Stream",                                         CheckerArray36, 0, false },
  { "alpha.unix.cstring",                                        0, SubPackageArray37, true },
  { "alpha.unix.cstring.BufferOverlap",                          CheckerArray38, 0, false },
  { "alpha.unix.cstring.NotNullTerminated",                      CheckerArray39, 0, false },
  { "alpha.unix.cstring.OutOfBounds",                            CheckerArray40, 0, false },
  { "core",                                                      0, SubPackageArray41, false },
  { "core.CallAndMessage",                                       CheckerArray42, 0, false },
  { "core.DivideZero",                                           CheckerArray43, 0, false },
  { "core.DynamicTypePropagation",                               CheckerArray44, 0, false },
  { "core.NonNullParamChecker",                                  CheckerArray45, 0, false },
  { "core.NullDereference",                                      CheckerArray46, 0, false },
  { "core.StackAddressEscape",                                   CheckerArray47, 0, false },
  { "core.UndefinedBinaryOperatorResult",                        CheckerArray48, 0, false },
  { "core.VLASize",                                              CheckerArray49, 0, false },
  { "core.builtin",                                              0, SubPackageArray50, false },
  { "core.builtin.BuiltinFunctions",                             CheckerArray51, 0, false },
  { "core.builtin.NoReturnFunctions",                            CheckerArray52, 0, false },
  { "core.uninitialized",                                        0, SubPackageArray53, false },
  { "core.uninitialized.ArraySubscript",                         CheckerArray54, 0, false },
  { "core.uninitialized.Assign",                                 CheckerArray55, 0, false },
  { "core.uninitialized.Branch",                                 CheckerArray56, 0, false },
  { "core.uninitialized.CapturedBlockVariable",                  CheckerArray57, 0, false },
  { "core.uninitialized.UndefReturn",                            CheckerArray58, 0, false },
  { "cplusplus",                                                 0, SubPackageArray59, false },
  { "cplusplus.NewDelete",                                       CheckerArray60, 0, false },
  { "deadcode",                                                  0, SubPackageArray61, false },
  { "deadcode.DeadStores",                                       CheckerArray62, 0, false },
  { "debug",                                                     0, SubPackageArray63, false },
  { "debug.ConfigDumper",                                        CheckerArray64, 0, false },
  { "debug.DumpCFG",                                             CheckerArray65, 0, false },
  { "debug.DumpCallGraph",                                       CheckerArray66, 0, false },
  { "debug.DumpCalls",                                           CheckerArray67, 0, false },
  { "debug.DumpDominators",                                      CheckerArray68, 0, false },
  { "debug.DumpLiveVars",                                        CheckerArray69, 0, false },
  { "debug.DumpTraversal",                                       CheckerArray70, 0, false },
  { "debug.ExprInspection",                                      CheckerArray71, 0, false },
  { "debug.Stats",                                               CheckerArray72, 0, false },
  { "debug.TaintTest",                                           CheckerArray73, 0, false },
  { "debug.ViewCFG",                                             CheckerArray74, 0, false },
  { "debug.ViewCallGraph",                                       CheckerArray75, 0, false },
  { "debug.ViewExplodedGraph",                                   CheckerArray76, 0, false },
  { "llvm",                                                      0, SubPackageArray77, false },
  { "llvm.Conventions",                                          CheckerArray78, 0, false },
  { "osx",                                                       0, SubPackageArray79, false },
  { "osx.API",                                                   CheckerArray80, 0, false },
  { "osx.SecKeychainAPI",                                        CheckerArray81, 0, false },
  { "osx.cocoa",                                                 0, SubPackageArray82, false },
  { "osx.cocoa.AtSync",                                          CheckerArray83, 0, false },
  { "osx.cocoa.ClassRelease",                                    CheckerArray84, 0, false },
  { "osx.cocoa.IncompatibleMethodTypes",                         CheckerArray85, 0, false },
  { "osx.cocoa.Loops",                                           CheckerArray86, 0, false },
  { "osx.cocoa.NSAutoreleasePool",                               CheckerArray87, 0, false },
  { "osx.cocoa.NSError",                                         CheckerArray88, 0, false },
  { "osx.cocoa.NilArg",                                          CheckerArray89, 0, false },
  { "osx.cocoa.NonNilReturnValue",                               CheckerArray90, 0, false },
  { "osx.cocoa.RetainCount",                                     CheckerArray91, 0, false },
  { "osx.cocoa.SelfInit",                                        CheckerArray92, 0, false },
  { "osx.cocoa.UnusedIvars",                                     CheckerArray93, 0, false },
  { "osx.cocoa.VariadicMethodTypes",                             CheckerArray94, 0, false },
  { "osx.coreFoundation",                                        0, SubPackageArray95, false },
  { "osx.coreFoundation.CFError",                                CheckerArray96, 0, false },
  { "osx.coreFoundation.CFNumber",                               CheckerArray97, 0, false },
  { "osx.coreFoundation.CFRetainRelease",                        CheckerArray98, 0, false },
  { "osx.coreFoundation.containers",                             0, SubPackageArray99, false },
  { "osx.coreFoundation.containers.OutOfBounds",                 CheckerArray100, 0, false },
  { "osx.coreFoundation.containers.PointerSizedValues",          CheckerArray101, 0, false },
  { "security",                                                  0, SubPackageArray102, false },
  { "security.FloatLoopCounter",                                 CheckerArray103, 0, false },
  { "security.insecureAPI",                                      0, SubPackageArray104, false },
  { "security.insecureAPI.UncheckedReturn",                      CheckerArray105, 0, false },
  { "security.insecureAPI.getpw",                                CheckerArray106, 0, false },
  { "security.insecureAPI.gets",                                 CheckerArray107, 0, false },
  { "security.insecureAPI.mkstemp",                              CheckerArray108, 0, false },
  { "security.insecureAPI.mktemp",                               CheckerArray109, 0, false },
  { "security.insecureAPI.rand",                                 CheckerArray110, 0, false },
  { "security.insecureAPI.strcpy",                               CheckerArray111, 0, false },
  { "security.insecureAPI.vfork",                                CheckerArray112, 0, false },
  { "unix",                                                      0, SubPackageArray113, false },
  { "unix.API",                                                  CheckerArray114, 0, false },
  { "unix.Malloc",                                               CheckerArray115, 0, false },
  { "unix.MallocSizeof",                                         CheckerArray116, 0, false },
  { "unix.MismatchedDeallocator",                                CheckerArray117, 0, false },
  { "unix.cstring",                                              0, SubPackageArray118, true },
  { "unix.cstring.BadSizeArg",                                   CheckerArray119, 0, false },
  { "unix.cstring.NullArg",                                      CheckerArray120, 0, false },
#endif // GET_CHECKNAME_TABLE

