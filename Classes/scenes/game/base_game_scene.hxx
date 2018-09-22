/**  Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __BASE_GAME_LAYER_HXX__
#define __BASE_GAME_LAYER_HXX__

/**
 * Includes
 */
#include "gestures/swipe/swipe_gesture.hxx"
#include "scenes/base_scene.hxx"

/**
 * Tags
 */
#define __FOLLOW_TAG__ 1000

/**
 * Particles
 */
#define __PARTICLE_EXIT__ \
  { "particles/game_exit.plist", "particles/game_exit_inner.plist" }

/**
 * Forward Declarations
 */
class BaseEntity;
class BaseLluma;
class BaseLlurker;

class BaseGameScene : public BaseScene {
  /**
   * Friend Declarations
   */
  friend BaseEntity;
  friend BaseLluma;
  friend BaseLlurker;

 protected:
  /**
   * Constructor.
   *
   * @param params ~ Parameters needed to initialize the scene.
   * @param bg     ~ Pointer to the constant background layer.
   */
  explicit BaseGameScene(const ValueMap& params, BackgroundLayer* bg);

  /**
   * Destructor.
   */
  virtual ~BaseGameScene();
  
  /**
   * @see BaseScene::beforeInitialize()
   */
  void beforeInitialized() override;

  /**
   * @see BaseScene::onInitialized()
   */
  bool onInitialized() override;

  /**
   * @see BaseScene::update()
   */
  void update(float delta) override;

  /**
   * Initializes the UI of the scene.
   *
   * @note ~ Required by all children since it is a pure virtual function
   */
  virtual void initializeUi() = 0;

  /**
   * Initializes the menu in the scene.
   *
   * @note ~ Required by all children since it is a pure virtual function
   */
  virtual void initializeMenu() = 0;

  /**
   * Initializes the text in the scene.
   *
   * @note ~ Required by all children since it is a pure virtual function
   */
  virtual void initializeText() = 0;

  /**
   * Initializes the Player.
   *
   * @note ~ Required by all children since it is a pure virtual function
   */
  virtual void initializePlayer() = 0;

  /**
   * Central logic that is preformed every time the player moves a tile.
   *
   * @returns If the game logic should stop.
   */
  virtual bool logic() = 0;

  /**
   * Callback when a collision is detected
   */
  virtual void onCollision() = 0;

  /**
   * Central logic that is preformed every time the player moves a tile.
   *
   * @returns If the game logic should stop.
   */
  virtual void finish(Scenes scene, CallFunc* function) = 0;

  /**
   * Initializes a TMXTiledMap.
   *
   * @param path ~ The paths to the tmx file in the archive.
   * @param name ~ The name of the tmx file.
   */
  virtual void initializeMap(const string& path, const string& name);

  /**
   * Initialize the collisition detector for physics object on the game scene (llumas)
   */
  void initializeCollisionDetection();

  /**
   * Pans the map to a specific vec2 and return back
   *
   * @param to            ~ Vec2 location to pan to.
   * @param to_function   ~ CallFunc function to run after `to` location is reached.
   * @param back_function ~ CallFunc function to run after players location is reached back from the `to`
   * @param to_duration   ~ Speed to move to the `to` location.
   * @param to_rate       ~ Rate to move to the `to` location.
   * @param delay         ~ delay between the `to` and player location.
   * @param back_duration ~ Speed to move back to the players location.
   * @param back_rate     ~ Rate to move back to the players location.
   */
  void pan(const Vec2& to, CallFunc& to_function, CallFunc& back_function, float to_duration = 1.5f,
           float to_rate = 3.5f, float delay = 2.0f, float back_duration = 1.25f,
           float back_rate = 3.5f) const;

  // The TMX Tiled Map that for the game.
  TMXTiledMap* map_{};

  // Displays the info and objective of the current level. This is only in the beginning of the level.
  LayerColor* loading_layer_{};

  // Displays the menu, and various other elements that  need to be over the game layer. (example: Time,
  // lives, etc..).
  Layer* ui_layer_{};

  // The player (Lluma).
  BaseLluma* player_{};

  // List of llurkers.
  list<BaseLlurker*> llurkers_{};

  // A Swipe Listener that is responsible for detected all swipes on the screen
  SwipeGesture* swipe_gesture_{};

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(BaseGameScene)
};

#endif  // __BASE_GAME_LAYER_HXX__
