#include "cda_compiler_support.h"
#include "IfaceDOM_APISPEC.hxx"

#include "cda_compiler_support.h"

#ifdef IN_DOM_MODULE
#define DOM_PUBLIC_PRE CDA_EXPORT_PRE
#define DOM_PUBLIC_POST CDA_EXPORT_POST
#else
#define DOM_PUBLIC_PRE CDA_IMPORT_PRE
#define DOM_PUBLIC_POST CDA_IMPORT_POST
#endif

/*
 * Note: Everything in this file is specific to locally hosted C++(with no
 *   CORBA ORB involved). Use sparingly or the reusability of your code will be
 *   harmed.
 */

/**
 * This class is non-standard, and should only be used by the API
 * implementation. The base class iface::dom::DOMImplementation is standard,
 * and can be used by applications.
 */
class CellML_DOMImplementationBase
  : public virtual iface::dom::DOMImplementation
{
public:
  // A non-standard function used by the bootstrap code to load documents...
  virtual iface::dom::Document* loadDocument(const wchar_t* aURL,
                                             std::wstring& aErrorMessage)
    throw(std::exception&) = 0;

  virtual iface::dom::Document* loadDocumentFromText(const wchar_t* aText,
                                                     std::wstring& aErrorMessage)
    throw(std::exception&) = 0;
};

// Get a DOM Implementation. This is a C++ specific method, not part of the
// standard interface. Applications should only access methods on the base
// class.
DOM_PUBLIC_PRE CellML_DOMImplementationBase* CreateDOMImplementation() DOM_PUBLIC_POST;
