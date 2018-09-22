// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "app_delegate.hxx"
#include "engines/firebase/firebase_engine.hxx"
#include "engines/fmod/fmod_engine.hxx"
#include "engines/language/language_engine.hxx"
#include "scenes/intro/intro_scene.hxx"
#include "utils/data/data_utils.hxx"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "utils/archive/archive_utils.hxx"
#endif

AppDelegate::AppDelegate() = default;

AppDelegate::~AppDelegate() = default;

void AppDelegate::initGLContextAttrs() {
  // Set OpenGL context attributions, now can only set six attributions:
  // Red, Green, Blue, Alpha, Depth, Stencil
  GLContextAttrs gl_context_attrs = {8, 8, 8, 8, 24, 8, 0};
  GLView::setGLContextAttrs(gl_context_attrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
  initZipUtils();
  initOpenGl();
  initDirector();
  initDatabase();
  initFirebase();
  copyLoadAssets();
  createAndRunScene();
  return true;
}

void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();
  AudioUtils::getInstance()->pauseMixer();

  // Destroy the instance of the language engine just in case the user changes the
  // system language
  LanguageEngine::destroyInstance();
}

void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();
  AudioUtils::getInstance()->resumeMixer();
}

void AppDelegate::initZipUtils() {
  // Set Up ZipUtils to use Key for sprites.
  ZipUtils::setPvrEncryptionKeyPart(0, 0x3a701f23);
  ZipUtils::setPvrEncryptionKeyPart(1, 0xa2af6677);
  ZipUtils::setPvrEncryptionKeyPart(2, 0x5248175b);
  ZipUtils::setPvrEncryptionKeyPart(3, 0xe404f08e);
}

void AppDelegate::initOpenGl() {
  const auto& director = Director::getInstance();
  auto gl_view = director->getOpenGLView();

  if (not gl_view) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX or \
     CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_MAIN__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_MAIN_LARGE__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_MAIN_SMALL__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_TABLET__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_TABLET_LARGE__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_TABLET_SMALL__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_GALAXY_S8__);
    gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_GALAXY_S8_SMALL__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_LG_G6__);
    // gl_view = GLViewImpl::createWithRect(__APP_DELEGATE_NAME__, __APP_DELEGATE_RECT_LG_G6_SMALL__);
#else
    gl_view = GLViewImpl::create(__APP_DELEGATE_NAME__);
#endif
  }

  // Sets the internal resolution of the game. This allows to be played at the same resolution no matter what
  // device we are currently on.
  gl_view->setDesignResolutionSize(__APP_DELEGATE_RESOLUTION__.width, __APP_DELEGATE_RESOLUTION__.height,
                                   ResolutionPolicy::NO_BORDER);

  // Set the GLView
  director->setOpenGLView(gl_view);
}

void AppDelegate::initDirector() {
  const auto& director = Director::getInstance();
  director->setAnimationInterval(1.0f / 144.0f);

#ifdef COCOS2D_DEBUG
  director->setDisplayStats(true);
#endif

  FileUtils::getInstance()->addSearchPath("data", true);
  FileUtils::getInstance()->addSearchPath("fonts");
}

void AppDelegate::initDatabase() { DataUtils::initDatabase(); }

void AppDelegate::initFirebase() { FirebaseEngine::getInstance(); }

void AppDelegate::copyLoadAssets() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
  auto data = FileUtils::getInstance()->getDataFromFile(
      FileUtils::getInstance()->fullPathForFilename(__ARCHIVE_NAME__));
  FileUtils::getInstance()->writeDataToFile(data,
                                            FileUtils::getInstance()->getWritablePath() + __ARCHIVE_NAME__);
#endif
}

void AppDelegate::createAndRunScene() {
  Director::getInstance()->runWithScene(IntroScene::create(IntroScene::generateParams(), nullptr));
}
