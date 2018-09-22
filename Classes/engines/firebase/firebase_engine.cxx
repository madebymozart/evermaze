// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "firebase_engine.hxx"
#include "firebase/util.h"
#include "utils/archive/archive_utils.hxx"

// Singleton Declaration
atomic<FirebaseEngine*> FirebaseEngine::instance_;
mutex FirebaseEngine::instance_mutex_;

FirebaseEngine::FirebaseEngine() {
  // Initializing firebase
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  core_ = firebase::App::Create(JniHelper::getEnv(), JniHelper::getActivity());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  core_ = firebase::App::Create();
#else
  // Desktop initialization is requires to load the json yourself create the options manually.
  const auto config = ArchiveUtils::loadString(__FIREBASE_CONFIG__);
  const auto options = firebase::AppOptions::LoadFromJsonConfig(config.c_str());
  core_ = firebase::App::Create(*options);
#endif

  // Using the ModuleInitializer to initialize all modules at once.
  firebase::ModuleInitializer initializer;

  // Defining modules. It's important that these are in order based on your initializer list
  void* targets[] = {&auth_, &database_};
  firebase::ModuleInitializer::InitializerFn initializers[] = {getAuthModule(), getDatabaseModule()};

  // Initialize all modules
  initializer.Initialize(core_, targets, initializers, sizeof initializers / sizeof initializers[0])
      .OnCompletion([](const firebase::FutureBase& future) {
        if (future.status() not_eq firebase::kFutureStatusComplete or future.error() != 0) {
          CCLOG("Firebase error : %s", future.error_message());
        }
        CCLOG("Firebase Engine: Successfully initialized Firebase Engine...");
      });
}

FirebaseEngine::~FirebaseEngine() { CC_SAFE_DELETE(core_); }

FirebaseEngine* FirebaseEngine::getInstance() {
  auto init = instance_.load(memory_order_relaxed);
  atomic_thread_fence(memory_order_acquire);

  if (init == nullptr) {
    // Invoke lock guards.
    lock_guard<mutex> lock(instance_mutex_);
    init = instance_.load(memory_order_relaxed);

    // Initialize a new instance if we are still null
    if (init == nullptr) {
      init = new (nothrow) FirebaseEngine();

      // Release guards
      atomic_thread_fence(memory_order_release);

      // Store new FirebaseEngine Instance
      instance_.store(init, memory_order_relaxed);
    }
  }

  // Return
  return init;
}

void FirebaseEngine::authenticateWithFacebook(const string& authtoken,
                                              const FirebaseAuthCallback& callback) const {
  // FirebaseAuthCallback should not be null.
  CCASSERT(callback != nullptr, "Firebase Engine: FirebaseAuthCallback should not be null...");

  // Initialize Authorization with a completion.
  const auto credential = firebase::auth::FacebookAuthProvider::GetCredential(authtoken.c_str());
  auth_->SignInWithCredential(credential).OnCompletion(callback, nullptr);
}

firebase::ModuleInitializer::InitializerFn FirebaseEngine::getAuthModule() {
  // Initializer function for Firebase Authentication.
  return [](firebase::App* app, void* data) {
    CCLOG("Firebase Engine: Attempt to initialize Firebase Authentication");
    const auto targets = reinterpret_cast<void**>(data);
    firebase::InitResult result;
    *reinterpret_cast<firebase::auth::Auth**>(targets[0]) = firebase::auth::Auth::GetAuth(app, &result);
    return result;
  };
}

firebase::ModuleInitializer::InitializerFn FirebaseEngine::getDatabaseModule() {
  // Initializer for Firebase Realtime Database.
  return [](firebase::App* app, void* data) {
    CCLOG("Firebase Engine: Attempt to initialize Realtime Database");
    const auto targets = reinterpret_cast<void**>(data);
    firebase::InitResult result;
    *reinterpret_cast<firebase::database::Database**>(targets[1]) =
        firebase::database::Database::GetInstance(app, &result);
    return result;
  };
}
