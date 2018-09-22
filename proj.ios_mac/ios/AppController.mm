/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "cocos2d.h"
#import "app/app_delegate.hxx"
#import "RootViewController.h"

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  
  cocos2d::Application *app = cocos2d::Application::getInstance();
  
  // Initialize the GLView attributes
  app->initGLContextAttrs();
  cocos2d::GLViewImpl::convertAttrs();
  
  // Add the view controller's view to the window and display.
  window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
  
  // Use RootViewController to manage CCEAGLView
  _viewController = [[RootViewController alloc]init];
  
  // Set RootViewController to window
  if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
  {
    // warning: addSubView doesn't work on iOS6
    [window addSubview: _viewController.view];
  }
  else
  {
    // use this method on ios6
    [window setRootViewController:_viewController];
  }
  
  [window makeKeyAndVisible];
  
  // IMPORTANT: Setting the GLView should be done after creating the RootViewController
  cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);
  cocos2d::Director::getInstance()->setOpenGLView(glview);
  
  //run the cocos2d-x game scene
  app->run();
  
  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {}

- (void)applicationDidBecomeActive:(UIApplication *)application {}

- (void)applicationDidEnterBackground:(UIApplication *)application {
  cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
  cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {}

#if __has_feature(objc_arc)
#else
- (void)dealloc {
  [window release];
  [_viewController release];
  [super dealloc];
}
#endif

@end
