// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

#include "menu_manager.hxx"
#include "utils/archive/archive_utils.hxx"

MenuManager::MenuManager(const ValueMap& info, Layer& layer) : BaseManager(layer), menu_(Menu::create()) {
  // Add all the menu items to the menu object
  initializeMenu(info.at(__MENU_ITEMS__).asValueVector());

  // Add properties of the menu
  menu_->setLocalZOrder(info.at(__ZINDEX__).asInt());
  menu_->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
  menu_->setPosition(0, 0);
  menu_->setCascadeOpacityEnabled(true);

  // Auto add to layer if set
  if (info.find(__ADD_TO_LAYER__) not_eq info.end() and info.at(__ADD_TO_LAYER__).asBool())
    main_layer_.addChild(menu_);
}

MenuManager::~MenuManager() {
  menu_->removeAllChildrenWithCleanup(true);
  menu_->removeFromParentAndCleanup(true);
}

MenuItemToggle* MenuManager::findItem(const string& key) {
  return dynamic_cast<MenuItemToggle*>(findNode(key));
}

Menu* MenuManager::getMenu() const { return menu_; }

void MenuManager::initializeMenu(ValueVector items) {
  const auto& cache = SpriteFrameCache::getInstance();
  for_each(items.begin(), items.end(), [&](const Value& data) {
    // Getting a reference to the item as a ValueMap
    const auto& props = data.asValueMap();
    const auto& name = props.at(__FILE_NAME__).asString();

    // Create top level menu item that will encapsulate all the images for the menu item
    auto item = MenuItemImage::create();
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setCascadeOpacityEnabled(true);
    item->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));

    // Now convert the menu item into a MenuItemToggle since this is much better suited for all menus.
    auto toggle = MenuItemToggle::create(item);
    toggle->setPosition(
        Globals::getScreenPosition(props.at(__POSX__).asFloat(), props.at(__POSY__).asFloat()));
    toggle->setOpacity(static_cast<GLubyte>(props.at(__OPACITY__).asInt()));
    toggle->setLocalZOrder(props.at(__ZINDEX__).asInt());
    toggle->setScale(props.find(__SCALE__) != props.end() ? props.at(__SCALE__).asFloat() : 1.0f);
    toggle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    toggle->setCascadeOpacityEnabled(true);

    // Add item to the menu
    menu_->addChild(toggle);

    // Create a new node object and add to the menu map.
    node_map_.insert(name, toggle);

    // If the props has an action then run it on the sprite
    addActionFromProps(toggle, props);
  });
}
