/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __APP_DELEGATE_HXX__
#define __APP_DELEGATE_HXX__

/**
 * Game Resolution (Internal, 720p)
 */
#define __APP_DELEGATE_RESOLUTION__ Size(768, 1136)

/**
 * Game Name
 */
#define __APP_DELEGATE_NAME__ "Evermaze"

/**
 * Aspect Ratio | 16:9 (Most Phones)
 */
#define __APP_DELEGATE_RECT_MAIN__ Rect(Vec2(0, 0), Size(720, 1280))
#define __APP_DELEGATE_RECT_MAIN_LARGE__ Rect(Vec2(0, 0), Size(1080, 1920))
#define __APP_DELEGATE_RECT_MAIN_SMALL__ Rect(Vec2(0, 0), Size(360, 640))

/**
 * Aspect Ratio | 4:3 (iPad / Other Tablets)
 */
#define __APP_DELEGATE_RECT_TABLET__ Rect(Vec2(0, 0), Size(1152, 1536))
#define __APP_DELEGATE_RECT_TABLET_LARGE__ Rect(Vec2(0, 0), Size(1536, 2048))
#define __APP_DELEGATE_RECT_TABLET_SMALL__ Rect(Vec2(0, 0), Size(768, 1024))

/**
 * Special Aspect Ratios
 * 18.5:9 (Samsung Galaxy S8 / S8+)
 * 18:9   (LG G6)
 */
#define __APP_DELEGATE_RECT_GALAXY_S8__ Rect(Vec2(0, 0), Size(720, 1480))
#define __APP_DELEGATE_RECT_GALAXY_S8_SMALL__ Rect(Vec2(0, 0), Size(360, 740))
#define __APP_DELEGATE_RECT_LG_G6__ Rect(Vec2(0, 0), Size(720, 1440))
#define __APP_DELEGATE_RECT_LG_G6_SMALL__ Rect(Vec2(0, 0), Size(360, 720))

/**
 * Includes
 */
#include "globals.hxx"

class AppDelegate : Application {
 public:
  /**
   * Constructor.
   */
  AppDelegate();

  /**
   * Destructor.
   */
  virtual ~AppDelegate();

 protected:
  /**
   * The values of glContextAttrs that determine the context of the game. This will takes effect
   * on all platforms.
   */
  void initGLContextAttrs() override;

  /**
   * Implementation of the Director and Scene.
   */
  bool applicationDidFinishLaunching() override;

  /**
   * Called when the application enters the background. For example, this will be invoked when
   * you get a phone call.
   */
  void applicationDidEnterBackground() override;

  /**
   * Called when the application enters the foreground.
   */
  void applicationWillEnterForeground() override;

  /**
   * Initialize ZipUtils.
   */
  static void initZipUtils();

  /**
   * Initialize native OpenGL on the host device. OpenGL 2.0+ is supported.
   */
  static void initOpenGl();

  /**
   * Initialize cocos2d::Director with attributes for Diligence.
   */
  static void initDirector();

  /**
   * Initialize SQLite3 Database
   */
  static void initDatabase();

  /**
   * Initializes Firebase
   */
  static void initFirebase();

  /**
   * Copies encrypted game loading scripts to a writeable path in android.
   */
  static void copyLoadAssets();

  /**
   * Creates and instance of scenes and runs the application.
   */
  static void createAndRunScene();

 private:
  __DISALLOW_COPY_AND_ASSIGN__(AppDelegate)
};

#endif  // __APP_DELEGATE_HXX__
