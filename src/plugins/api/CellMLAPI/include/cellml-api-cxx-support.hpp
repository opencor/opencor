#ifndef _cellml_api_cxx_support_hpp
#define _cellml_api_cxx_support_hpp

#include "cda_compiler_support.h"
#include "Ifacexpcom.hxx"
#include <stdlib.h>
#include <cstring>

HEADER_INLINE int
CDA_objcmp(iface::XPCOM::IObject* o1, iface::XPCOM::IObject* o2)
  throw()
{
  std::string s1, s2;
  bool vs1 = true, vs2 = true;
  try
  {
    s1 = o1->objid();
  }
  catch (...)
  {
    vs1 = false;
  }
  try
  {
    s2 = o2->objid();
  }
  catch (...)
  {
    vs2 = false;
  }

  int cmp;
  if (vs1 && vs2)
  {
    cmp = strcmp(s1.c_str(), s2.c_str());
  }
  // if we have a dead object, we can't compare them so easily, so we just
  // follow a basic rule. This can break ordering relationships, but once
  // they are dead, it is the best we can do.
  else if (!vs1 && !vs2)
    // Dead objects are automatically identical.
    cmp = 0;
  else if (!vs1)
    // First is dead, second is alive, so treat as -1...
    cmp = -1;
  else
    cmp = 1;

  return cmp;
}

class DoQueryInterface
{
public:
  DoQueryInterface(iface::XPCOM::IObject* aObj)
    : mObj(aObj)
  {
  }

  iface::XPCOM::IObject* mObj;
};

template<class T>
class ObjRef
{
public:
  ObjRef()
    : mPtr(NULL)
  {
  }

  ObjRef(const ObjRef<T>& aPtr)
  {
    mPtr = aPtr.getPointer();
    if (mPtr != NULL)
      mPtr->add_ref();
  }

  ObjRef(T* aPtr)
    : mPtr(aPtr)
  {
    if (mPtr != NULL)
      mPtr->add_ref();
  }

  ObjRef(const already_AddRefd<T> aar)
  {
    mPtr = aar.getPointer();
  }

  ObjRef(DoQueryInterface dqi)
  {
    if (dqi.mObj == NULL)
    {
      mPtr = NULL;
      return;
    }
    mPtr = reinterpret_cast<T*>(dqi.mObj->query_interface(T::INTERFACE_NAME()));
    dqi.mObj->release_ref();
    dqi.mObj = NULL;
  }

  ~ObjRef()
  {
    if (mPtr != NULL)
      mPtr->release_ref();
  }

  T* operator-> () const
  {
    return mPtr;
  }

  T* getPointer() const
  {
    return mPtr;
  }

  operator T* () const
  {
    return mPtr;
  }

  void operator= (T* newAssign)
  {
    if (mPtr == newAssign)
      return;
    if (mPtr)
      mPtr->release_ref();
    mPtr = newAssign;
    if (newAssign != NULL)
      mPtr->add_ref();
  }

  // We need these explicit forms or the default overloads the templates below.
  void operator= (const already_AddRefd<T>& newAssign)
  {
    T* nap = newAssign.getPointer();
    if (mPtr)
      mPtr->release_ref();
    mPtr = nap;
  }

  void operator= (const ObjRef<T>& newAssign)
  {
    T* nap = newAssign.getPointer();
    if (mPtr == nap)
      return;
    if (mPtr)
      mPtr->release_ref();
    mPtr = nap;
    if (mPtr != NULL)
      mPtr->add_ref();
  }

  template<class U>
  void operator= (const already_AddRefd<U>& newAssign)
  {
    T* nap = newAssign.getPointer();
    if (mPtr == nap)
      return;
    if (mPtr)
      mPtr->release_ref();
    mPtr = nap;
  }

  template<class U>
  void operator= (const ObjRef<U>& newAssign)
  {
    T* nap = newAssign.getPointer();
    if (mPtr == nap)
      return;
    if (mPtr)
      mPtr->release_ref();
    mPtr = nap;
    if (mPtr != NULL)
      mPtr->add_ref();
  }

  void operator=(DoQueryInterface dqi)
  {
    T* tmp = mPtr;

    if (dqi.mObj == NULL)
      mPtr = NULL;
    else
    {
      mPtr = reinterpret_cast<T*>(dqi.mObj->query_interface(T::INTERFACE_NAME()));
      dqi.mObj->release_ref();
      dqi.mObj = NULL;
    }

    if (tmp)
      tmp->release_ref();
  }
private:
  T* mPtr;
};

template<class C>
DoQueryInterface
QueryInterface(C* qi)
{
  if (qi)
    qi->add_ref();
  return DoQueryInterface(qi);
}

template<class C>
DoQueryInterface
QueryInterface(already_AddRefd<C> qi)
{
  return DoQueryInterface(qi.getPointer());
}

template<class C>
DoQueryInterface
QueryInterface(ObjRef<C> qi)
{
  return QueryInterface(static_cast<C*>(qi));
}

template<class T, class U> bool
operator==(const ObjRef<T>& lhs, const ObjRef<U>& rhs)
{
  return (lhs.getPointer() == rhs.getPointer());
}

template<class T, class U> bool
operator!=(const ObjRef<T>& lhs, const ObjRef<U>& rhs)
{
  return (lhs.getPointer() != rhs.getPointer());
}

#define RETURN_INTO_WSTRING(lhs, rhs) \
  std::wstring lhs(rhs);
#define RETURN_INTO_OBJREF(lhs, type, rhs) \
  ObjRef<type> lhs \
  ( \
    already_AddRefd<type> \
    ( \
      static_cast<type*> \
      ( \
        rhs \
      ) \
    )\
  )

#define RETURN_INTO_OBJREFD(lhs, type, rhs) \
  ObjRef<type> lhs \
  ( \
    already_AddRefd<type> \
    ( \
      dynamic_cast<type*> \
      ( \
        rhs \
      ) \
    )\
  )

#define QUERY_INTERFACE(lhs, rhs, type) \
  if (rhs != NULL) \
  { \
    void* _qicast_obj = rhs->query_interface(#type); \
    if (_qicast_obj != NULL) \
    { \
      lhs = already_AddRefd<iface::type>(reinterpret_cast<iface::type*>(_qicast_obj)); \
    } \
    else \
      lhs = NULL; \
  } \
  else \
    lhs = NULL;

#define QUERY_INTERFACE_REPLACE(lhs, rhs, type) \
  QUERY_INTERFACE(lhs, rhs, type) \
  if (rhs != NULL) \
  { \
    rhs->release_ref(); \
    rhs = NULL; \
  }

#define DECLARE_QUERY_INTERFACE(lhs, rhs, type) \
  iface::type* lhs; \
  QUERY_INTERFACE(lhs, rhs, type)

#define DECLARE_QUERY_INTERFACE_REPLACE(lhs, rhs, type) \
  iface::type* lhs; \
  QUERY_INTERFACE_REPLACE(lhs, rhs, type)

#define DECLARE_QUERY_INTERFACE_OBJREF(lhs, rhs, type) \
  ObjRef<iface::type> lhs; \
  QUERY_INTERFACE(lhs, rhs, type)

#endif // ifndef _cellml_api_cxx_support_hpp
