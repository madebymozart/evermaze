/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __MACROS_HXX__
#define __MACROS_HXX__

/**
 * Creates a scenes in cocos2d-x.
 *
 * @param __TYPE__   ~ Class type to add create(), such as Layer in cocos2d-x game engine.
 * @param __PARAMS__ ~ The parameters associated with this class. This is usually a cocos2d::ValueMap
 * @param __BG__     ~ Pointer to the background layer
 */
#define __CREATE_COCOS__(__TYPE__, __PARAMS__, __BG__)    \
  static __TYPE__* create(__PARAMS__ params, __BG__ bg) { \
    auto* pRet = new (nothrow) __TYPE__(params, bg);      \
    if (pRet) {                                           \
      pRet->initialize();                                 \
      pRet->setName(params.at("name").asString());        \
      return pRet;                                        \
    } else {                                              \
      delete pRet;                                        \
      pRet = nullptr;                                     \
      return nullptr;                                     \
    }                                                     \
  }

/**
 * Creates a scene with physics in cocos2d-x.
 *
 * @param __TYPE__   ~ Class type to add create(), such as Layer in cocos2d-x game engine.
 * @param __PARAMS__ ~ The parameters associated with this class. This is usually a cocos2d::ValueMap
 * @param __BG__     ~ Pointer to the background layer
 */
#define __CREATE_COCOS_PHYSICS__(__TYPE__, __PARAMS__, __BG__) \
  static __TYPE__* create(__PARAMS__ params, __BG__ bg) {      \
    __TYPE__* pRet = new (std::nothrow) __TYPE__(params, bg);  \
    if (pRet and pRet->initWithPhysics()) {                    \
      pRet->initialize();                                      \
      pRet->setName(params.at("name").asString());             \
      return pRet;                                             \
    } else {                                                   \
      delete pRet;                                             \
      pRet = nullptr;                                          \
      return nullptr;                                          \
    }                                                          \
  }

/**
 * A macro to disallow the copy constructor and operator= functions.
 *
 * @note This should be used in the private: declarations for a class that wants to prevent
 * anyone from instantiating it.
 */
#define __DISALLOW_COPY_AND_ASSIGN__(TypeName) \
  TypeName(TypeName&&) = delete;               \
  TypeName(const TypeName&) = delete;          \
  TypeName& operator=(TypeName&&) = delete;    \
  TypeName& operator=(const TypeName&) = delete;

/**
 * A macro to disallow all the implicit constructors, namely the default constructor,
 * copy constructor and operator= functions.
 *
 * @note This should be used in the private: declarations for a class that wants to prevent
 * anyone from instantiating it. This one is specifically useful for classes containing only
 * static methods.
 */
#define __DISALLOW_IMPLICIT_CONSTRUCTORS__(TypeName) \
  TypeName() = delete;                               \
  ~TypeName() = delete;                              \
  __DISALLOW_COPY_AND_ASSIGN__(TypeName)

#endif  // __MACROS_HXX__
