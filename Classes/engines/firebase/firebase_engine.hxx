/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __FIREBASE_ENGINE_HXX__
#define __FIREBASE_ENGINE_HXX__

/**
 * Includes
 */
#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/database.h"
#include "firebase/util.h"
#include "globals.hxx"

#if (CC_TARGET_PLATFORM not_eq CC_PLATFORM_ANDROID and CC_TARGET_PLATFORM not_eq CC_PLATFORM_IOS)
#define __FIREBASE_CONFIG__ "configs/desktop.json"
#endif

/**
 * Aliases
 */
using FirebaseAuthCallback = firebase::Future<firebase::auth::User*>::TypedCompletionCallback;

class FirebaseEngine {
  /**
   * Constructor.
   */
  explicit FirebaseEngine();

  /**
   * Destructor.
   */
  virtual ~FirebaseEngine();

 public:
  /**
   * Gets singleton instance of this class. If the instance is null, it will initialize it.
   *
   * @returns ~ The singleton instance of the FirebaseEngine.
   *
   * @note ~ This was implemented using acquire and release fences. Since the instance is wraped
   * in a C++11 atomic type, we can manipulate it using relaxed atomic operations.
   */
  static FirebaseEngine* getInstance();

  /**
   * Authenticates the user with facebook. If this is the users first time, the users firebase id will get
   * generated.
   *
   * @param authtoken ~ Authentication toekn from Facebook.
   * @param callback  ~ Callback when the login has completed.
   */
  void authenticateWithFacebook(const string& authtoken, const FirebaseAuthCallback& callback) const;

 protected:
  /**
   * Creates an initializer for the Firebase Authentication module.
   *
   * @returns ~  Firebase Authentication module initializer function.
   */
  static firebase::ModuleInitializer::InitializerFn getAuthModule();

  /**
   * Creates an initializer for the Firebase Authentication module.
   *
   * @returns ~  Firebase Realtime Database module initializer function.
   */
  static firebase::ModuleInitializer::InitializerFn getDatabaseModule();

 private:
  // Firebase App module.
  firebase::App* core_{};

  // Firebase authentication module.
  firebase::auth::Auth* auth_{};

  // Firebase database module.
  firebase::database::Database* database_{};

  // Singleton instance of this class.
  static atomic<FirebaseEngine*> instance_;

  // Mutex to make accessing the singleton thread-safe
  static mutex instance_mutex_;

  __DISALLOW_COPY_AND_ASSIGN__(FirebaseEngine)
};

#endif  // __AUDIO_UTILS_HXX__
