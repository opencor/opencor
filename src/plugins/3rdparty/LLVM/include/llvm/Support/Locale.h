#ifndef LLVM_SUPPORT_LOCALE
#define LLVM_SUPPORT_LOCALE

#include "llvm/ADT/StringRef.h"
//---OPENCOR--- BEGIN
#include "llvmglobal.h"
//---OPENCOR--- END

namespace llvm {
namespace sys {
namespace locale {

/*---OPENCOR---
int columnWidth(StringRef s);
bool isPrint(int c);
*/
//---OPENCOR--- BEGIN
int LLVM_EXPORT columnWidth(StringRef s);
bool LLVM_EXPORT isPrint(int c);
//---OPENCOR--- END

}
}
}

#endif // LLVM_SUPPORT_LOCALE
