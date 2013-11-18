#ifndef _cellml_api_cxx_support_hpp
#define _cellml_api_cxx_support_hpp

#include "cda_compiler_support.h"
#include "Ifacexpcom.hxx"
#include <stdlib.h>
#include <cstring>
#ifdef WIN32
#include <windows.h>
#endif

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

// A wrapper for a mutex...
class CDAMutex
{
public:
  CDAMutex()
  {
#ifdef WIN32
    InitializeCriticalSection(&mMutex);
#else
    pthread_mutex_init(&mMutex, NULL);
#endif
  }

  ~CDAMutex()
  {
#ifdef WIN32
    DeleteCriticalSection(&mMutex);
#else
    pthread_mutex_destroy(&mMutex);
#endif
  }

  void Lock()
  {
#ifdef WIN32
    EnterCriticalSection(&mMutex);
#else
    pthread_mutex_lock(&mMutex);
#endif
  }

  void Unlock()
  {
#ifdef WIN32
    LeaveCriticalSection(&mMutex);
#else
    pthread_mutex_unlock(&mMutex);
#endif
  }
private:
#ifdef WIN32
  CRITICAL_SECTION mMutex;
#else
  pthread_mutex_t mMutex;
#endif
};

// A class to provide a scoped lock...
class CDALock
{
public:
  CDALock(CDAMutex& m)
    : mutex(m)
  {
    mutex.Lock();
  }

  ~CDALock()
  {
    mutex.Unlock();
  }
private:
  CDAMutex& mutex;
};

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

class CDA_RefCount
{
public:
  CDA_RefCount()
    : mRefcount(1)
  {
  }

  operator uint32_t()
  {
#if !(defined(WIN32) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4))
    CDALock l(mMutex);
#endif
    return mRefcount;
  }

  CDA_RefCount& operator=(const uint32_t& aValue)
  {
#if !(defined(WIN32) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4))
    CDALock l(mMutex);
#endif
    mRefcount = aValue;
    return *this;
  }

  void operator++()
  {
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
    __sync_fetch_and_add(&mRefcount, 1);
#elif defined(WIN32)
    InterlockedIncrement((volatile long int*)&mRefcount);
#else
    CDALock l(mMutex);
    mRefcount++;
#endif
  }

  bool operator--()
  {
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
    return __sync_sub_and_fetch(&mRefcount, 1) != 0;
#elif defined(WIN32)
    return InterlockedDecrement((volatile long int*)&mRefcount) != 0;
#else
    CDALock l(mMutex);
    mRefcount--;
    return (mRefcount != 0);
#endif
  }

private:
#if !(defined(WIN32) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4))
  CDAMutex mMutex;
#endif
  uint32_t mRefcount;
};

#define CDA_IMPL_REFCOUNT \
  private: \
    CDA_RefCount _cda_refcount; \
  public: \
    void add_ref() \
      throw() \
    { \
      ++_cda_refcount; \
    } \
    void release_ref() \
      throw() \
    { \
      if (!--_cda_refcount) \
        delete this; \
    }

#define CDA_IMPL_QI0 \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      return v; \
    }

#define CDA_IMPL_QI1(c1) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      return v; \
    }

#define CDA_IMPL_QI2(c1, c2) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      return v; \
    }

#define CDA_IMPL_QI3(c1, c2, c3) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      return v; \
    }

#define CDA_IMPL_QI4(c1, c2, c3, c4) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      else if (id == #c4) \
      { \
        add_ref(); \
        return static_cast< iface::c4* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      v.push_back(#c4); \
      return v; \
    }

#define CDA_IMPL_QI5(c1, c2, c3, c4, c5) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      else if (id == #c4) \
      { \
        add_ref(); \
        return static_cast< iface::c4* >(this); \
      } \
      else if (id == #c5) \
      { \
        add_ref(); \
        return static_cast< iface::c5* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      v.push_back(#c4); \
      v.push_back(#c5); \
      return v; \
    }

#define CDA_IMPL_QI6(c1, c2, c3, c4, c5, c6) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      else if (id == #c4) \
      { \
        add_ref(); \
        return static_cast< iface::c4* >(this); \
      } \
      else if (id == #c5) \
      { \
        add_ref(); \
        return static_cast< iface::c5* >(this); \
      } \
      else if (id == #c6) \
      { \
        add_ref(); \
        return static_cast< iface::c6* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      v.push_back(#c4); \
      v.push_back(#c5); \
      v.push_back(#c6); \
      return v; \
    }

#define CDA_IMPL_QI7(c1, c2, c3, c4, c5, c6, c7) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      else if (id == #c4) \
      { \
        add_ref(); \
        return static_cast< iface::c4* >(this); \
      } \
      else if (id == #c5) \
      { \
        add_ref(); \
        return static_cast< iface::c5* >(this); \
      } \
      else if (id == #c6) \
      { \
        add_ref(); \
        return static_cast< iface::c6* >(this); \
      } \
      else if (id == #c7) \
      { \
        add_ref(); \
        return static_cast< iface::c7* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      v.push_back(#c4); \
      v.push_back(#c5); \
      v.push_back(#c6); \
      v.push_back(#c7); \
      return v; \
    }

#define CDA_IMPL_QI8(c1, c2, c3, c4, c5, c6, c7, c8) \
    void* query_interface(const std::string& id) \
      throw(std::exception&) \
    { \
      if (id == "XPCOM::IObject") \
      { \
        add_ref(); \
        return static_cast<iface::XPCOM::IObject*>(this); \
      } \
      else if (id == #c1) \
      { \
        add_ref(); \
        return static_cast< iface::c1* >(this); \
      } \
      else if (id == #c2) \
      { \
        add_ref(); \
        return static_cast< iface::c2* >(this); \
      } \
      else if (id == #c3) \
      { \
        add_ref(); \
        return static_cast< iface::c3* >(this); \
      } \
      else if (id == #c4) \
      { \
        add_ref(); \
        return static_cast< iface::c4* >(this); \
      } \
      else if (id == #c5) \
      { \
        add_ref(); \
        return static_cast< iface::c5* >(this); \
      } \
      else if (id == #c6) \
      { \
        add_ref(); \
        return static_cast< iface::c6* >(this); \
      } \
      else if (id == #c7) \
      { \
        add_ref(); \
        return static_cast< iface::c7* >(this); \
      } \
      else if (id == #c8) \
      { \
        add_ref(); \
        return static_cast< iface::c8* >(this); \
      } \
      return NULL; \
    } \
    std::vector<std::string> supported_interfaces() throw() \
    { \
      std::vector<std::string> v; \
      v.push_back("XPCOM::IObject"); \
      v.push_back(#c1); \
      v.push_back(#c2); \
      v.push_back(#c3); \
      v.push_back(#c4); \
      v.push_back(#c5); \
      v.push_back(#c6); \
      v.push_back(#c7); \
      v.push_back(#c8); \
      return v; \
    }

#endif // ifndef _cellml_api_cxx_support_hpp
