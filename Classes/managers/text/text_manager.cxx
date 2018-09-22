// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

#include "text_manager.hxx"
#include "engines/language/language_engine.hxx"

TextManager::TextManager(const ValueMap& info, Layer& layer) : BaseManager(layer) {
  // Run some assertions on the passed info.
  CCASSERT(info.find(__FILE_NAME__) not_eq info.end(), "TextManager: for filename in info specified...");
  CCASSERT(info.find(__TEXT__) not_eq info.end(), "TextManager: for text array in info specified...");

  // Caches strings with the language engine
  LanguageEngine::getInstance()->cacheWords(info.at(__FILE_NAME__).asString());

  // Reference ValueVector and get the font path
  const auto& text_array = info.at(__TEXT__).asValueVector();
  const auto font_path = LanguageEngine::getInstance()->getFontPath();

  // Load all the data into the NodeMap for this manager
  for_each(text_array.begin(), text_array.end(), [&](const Value& data) -> void {
    const auto& map = data.asValueMap();

    // Get preliminary properties for creating the label
    const auto name = map.at(__NAME__).asString();
    const auto size = map.at(__SIZE__).asInt() * 4;
    const auto font = font_path + map.at(__FONT__).asString() + ".ttf";
    const auto h_alignment = map.find(__H_ALIGNMENT__) not_eq map.end()
                                 ? getHAlignment(map.at(__H_ALIGNMENT__).asString())
                                 : getHAlignment();
    const auto v_alignment = map.find(__V_ALIGNMENT__) not_eq map.end()
                                 ? getVAlignment(map.at(__V_ALIGNMENT__).asString())
                                 : getVAlignment();

    const auto text = LanguageEngine::getInstance()->getStringForKey(name);

    // Create a label with the above properties and set other properties.
    auto label = Label::createWithTTF(text, font, size, cocos2d::Size::ZERO, h_alignment, v_alignment);
    label->setName(name);
    label->setPosition(Globals::getScreenPosition(map.at(__POSX__).asFloat(), map.at(__POSY__).asFloat()));
    label->setOpacity(GLubyte(map.at(__OPACITY__).asInt()));
    label->setScale(0.25f);
    label->setLocalZOrder(map.at(__ZINDEX__).asInt());
    label->setCascadeOpacityEnabled(true);
    label->setMaxLineWidth(2100);

    // Set optional label properties from the properties value map.
    if (map.find(__ACP_X__) not_eq map.end() and map.find(__ACP_Y__) not_eq map.end())
      label->setAnchorPoint(Vec2(map.at(__ACP_X__).asFloat(), map.at(__ACP_Y__).asFloat()));

    // Auto add to layer if set
    if (map.find(__ADD_TO_LAYER__) not_eq map.end() and map.at(__ADD_TO_LAYER__).asBool())
      main_layer_.addChild(label);

    // Insert into the node map
    node_map_.insert(name, label);

    // If the props has an action then run it on the sprite
    addActionFromProps(label, map);
  });
}

TextManager::~TextManager() = default;

Label* TextManager::findLabel(const string& key) { return dynamic_cast<Label*>(findNode(key)); }

string TextManager::getFont(const Fonts font) {
  switch (font) {
    case LIGHT:
      return "Light.ttf";
    case BOLD:
      return "Bold.ttf";
    case THIN:
      return "Thin.ttf";
    case LIGHT_ITA:
      return "Light-Italic.ttf";
    case BOLD_ITA:
      return "Bold-Italic.ttf";
  }

  return string();
}

TextHAlignment TextManager::getHAlignment(const string& align) {
  switch (Globals::sshash(align.c_str())) {
    case Globals::sshash("left"):
      return TextHAlignment::LEFT;
    case Globals::sshash("right"):
      return TextHAlignment::RIGHT;
    default:
      return TextHAlignment::CENTER;
  }
}

TextVAlignment TextManager::getVAlignment(const string& align) {
  switch (Globals::sshash(align.c_str())) {
    case Globals::sshash("top"):
      return TextVAlignment::TOP;
    case Globals::sshash("bottom"):
      return TextVAlignment::BOTTOM;
    default:
      return TextVAlignment::CENTER;
  }
}
