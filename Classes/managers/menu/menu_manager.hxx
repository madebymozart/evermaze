/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __MENU_MANAGER_HXX__
#define __MENU_MANAGER_HXX__

/**
 * Includes
 */
#include "managers/base_manager.hxx"

class MenuManager : public BaseManager {
 public:
  /**
   * Constructor.
   *
   * @param info  ~ ValueMap of info for the initialization of the MenuManager
   * @param layer ~ Reference to the main layer associated to the scene.
   */
  explicit MenuManager(const ValueMap& info, Layer& layer);

  /**
   * Destructor.
   */
  virtual ~MenuManager();

  /**
   * Searches & converts the result of the call to BaseManager#findNode into a cocos2d#MenuItemToggle
   *
   * @param key ~ Key of the menu item you are searching for.
   *
   * @returns ~ Converted node to {@link cocos2d#MenuItemToggle}.
   *
   * @note ~ This will throw an error if the node cannot be found.
   */
  MenuItemToggle* findItem(const string& key);

  /**
   * Gets the menu object.
   *
   * @returns ~ Immutable menu object
   */
  Menu* getMenu() const;

 protected:
  /**
   * Initialize the menu for the manager.
   *
   * @param items ~ ValueVector containing all the of the menu items.
   *
   * @note This manager is tried to the scene which created it and will create a menu
   *       once initialized.
   */
  void initializeMenu(ValueVector items);

  // Menu object made by this manager. This object will stay alive for the duration of the managers life
  Menu* menu_;

 private:
  /**
   * __DISALLOW_COPY_AND_ASSIGN__
   */
  __DISALLOW_COPY_AND_ASSIGN__(MenuManager)
};

#endif  // __MENU_MANAGER_HXX__
