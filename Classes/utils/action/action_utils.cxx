// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Class
#include "action_utils.hxx"

Action* ActionUtils::fadeIn(const float delay, const float duration) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  return fadeInInternal(props);
}

Action* ActionUtils::fadeOut(const float delay, const float duration) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  return fadeOutInternal(props);
}

Action* ActionUtils::fadeTo(const float delay, const float duration, const int opacity, const float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__OPACITY__, opacity);
  props.emplace(__RATE__, rate);
  return fadeToInternal(props);
}

Action* ActionUtils::easeIn(float delay, float duration, float pos_x, float pos_y, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__POSX__, pos_x);
  props.emplace(__POSY__, pos_y);
  props.emplace(__RATE__, rate);
  return easeInInternal(props);
}

Action* ActionUtils::easeOut(float delay, float duration, float pos_x, float pos_y, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__POSX__, pos_x);
  props.emplace(__POSY__, pos_y);
  props.emplace(__RATE__, rate);
  return easeOutInternal(props);
}

Action* ActionUtils::easeInOut(float delay, float duration, float pos_x, float pos_y, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__POSX__, pos_x);
  props.emplace(__POSY__, pos_y);
  props.emplace(__RATE__, rate);
  return easeInOutInternal(props);
}

Action* ActionUtils::moveTo(float delay, float duration, float pos_x, float pos_y) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__POSX__, pos_x);
  props.emplace(__POSY__, pos_y);
  return moveToInternal(props);
}

Action* ActionUtils::moveBy(float delay, float duration, float pos_x, float pos_y) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__POSX__, pos_x);
  props.emplace(__POSY__, pos_y);
  return moveByInternal(props);
}

Action* ActionUtils::rotateTo(float delay, float duration, float degree, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__DEGREE__, degree);
  props.emplace(__RATE__, rate);
  return rotateToInternal(props);
}

Action* ActionUtils::rotateBy(float delay, float duration, float degree, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__DEGREE__, degree);
  props.emplace(__RATE__, rate);
  return rotateByInternal(props);
}

Action* ActionUtils::zoom(float delay, float duration, float scale, float rate) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__SCALE__, scale);
  props.emplace(__RATE__, rate);
  return zoomInternal(props);
}

Action* ActionUtils::blink(float delay, float duration, int repeat) {
  ValueMap props;
  props.emplace(__DELAY__, delay);
  props.emplace(__DURATION__, duration);
  props.emplace(__REPEAT__, repeat);
  return blinkInternal(props);
}

Action* ActionUtils::spawn(initializer_list<Action*> actions) {
  Vector<FiniteTimeAction*> actions_vec;
  for_each(actions.begin(), actions.end(), [&](const auto& action) {
    if (action != nullptr) actions_vec.pushBack(toFiniteTimeAction(action));
  });

  return Spawn::create(actions_vec);
}

Action* ActionUtils::sequence(initializer_list<Action*> actions) {
  Vector<FiniteTimeAction*> actions_vec;
  for_each(actions.begin(), actions.end(), [&](const auto& action) {
    if (action != nullptr) actions_vec.pushBack(toFiniteTimeAction(action));
  });

  return Sequence::create(actions_vec);
}

Action* ActionUtils::makeAction(ValueMap props) {
  if (not props.at(__NAME__).isNull()) return createActionMap().at(props.at(__NAME__).asString())(props);
  return nullptr;
}

FiniteTimeAction* ActionUtils::toFiniteTimeAction(Action* action) {
  return dynamic_cast<FiniteTimeAction*>(action);
}

bool ActionUtils::isRunningAction(const Node* node) { return node->getNumberOfRunningActions() != 0; }

bool ActionUtils::isRunningAction(initializer_list<Node*> nodes) {
  // for_each(nodes.begin(), nodes.end(), [&](const auto& node) { if (isRunningAction(node)) return true; });
  for (const auto& node : nodes) {
    if (isRunningAction(node)) return true;
  }
  return false;
}

Action* ActionUtils::fadeInInternal(ValueMap props) {
  return Sequence::createWithTwoActions(DelayTime::create(props.at(__DELAY__).asFloat()),
                                        FadeIn::create(props.at(__DURATION__).asFloat()));
}

Action* ActionUtils::fadeOutInternal(ValueMap props) {
  return Sequence::createWithTwoActions(DelayTime::create(props.at(__DELAY__).asFloat()),
                                        FadeOut::create(props.at(__DURATION__).asFloat()));
}

Action* ActionUtils::fadeToInternal(ValueMap props) {
  return Sequence::createWithTwoActions(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseInOut::create(
          FadeTo::create(props.at(__DURATION__).asFloat(), GLubyte(props.at(__OPACITY__).asFloat())),
          props.at(__RATE__).asFloat()));
}

Action* ActionUtils::moveToInternal(ValueMap props) {
  return MoveTo::create(
      props.at(__DURATION__).asFloat(),
      Globals::getScreenPosition(props.at(__POSX__).asFloat(), props.at(__POSY__).asFloat()));
}

Action* ActionUtils::moveByInternal(ValueMap props) {
  return MoveBy::create(props.at(__DURATION__).asFloat(),
                        Vec2(props.at(__POSX__).asFloat(), props.at(__POSY__).asFloat()));
}

Action* ActionUtils::rotateToInternal(ValueMap props) {
  return Sequence::createWithTwoActions(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseInOut::create(RotateTo::create(props.at(__DURATION__).asFloat(), props.at(__DEGREE__).asFloat()),
                        props.at(__RATE__).asFloat()));
}

Action* ActionUtils::rotateByInternal(ValueMap props) {
  return Sequence::createWithTwoActions(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseInOut::create(RotateBy::create(props.at(__DURATION__).asFloat(), props.at(__DEGREE__).asFloat()),
                        props.at(__RATE__).asFloat()));
}

Action* ActionUtils::easeInInternal(ValueMap props) {
  return Sequence::create(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseIn::create(dynamic_cast<MoveTo*>(moveToInternal(props)), props.at(__RATE__).asFloat()), nullptr);
}

Action* ActionUtils::easeOutInternal(ValueMap props) {
  return Sequence::create(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseOut::create(dynamic_cast<MoveTo*>(moveToInternal(props)), props.at(__RATE__).asFloat()), nullptr);
}

Action* ActionUtils::easeInOutInternal(ValueMap props) {
  return Sequence::create(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseInOut::create(dynamic_cast<MoveTo*>(moveToInternal(props)), props.at(__RATE__).asFloat()), nullptr);
}

Action* ActionUtils::zoomInternal(ValueMap props) {
  return Sequence::create(
      DelayTime::create(props.at(__DELAY__).asFloat()),
      EaseInOut::create(ScaleTo::create(props.at(__DURATION__).asFloat(), props.at(__SCALE__).asFloat()),
                        props.at(__RATE__).asFloat()),
      nullptr);
}

Action* ActionUtils::blinkInternal(ValueMap props) {
  Vector<FiniteTimeAction*> actions;
  actions.pushBack(DelayTime::create(props.at(__DELAY__).asFloat()));
  actions.pushBack(FadeIn::create(0));
  actions.pushBack(Blink::create(props.at(__DURATION__).asFloat(), props.at(__REPEAT__).asInt()));
  return Sequence::create(actions);
}

Action* ActionUtils::sequenceInternal(ValueMap props) { return Sequence::create(createActionVector(props)); }

Action* ActionUtils::spawnInternal(ValueMap props) { return Spawn::create(createActionVector(props)); }

Action* ActionUtils::repeatInternal(ValueMap props) {
  const auto repeat = dynamic_cast<ActionInterval*>(createActionVector(props).at(0));
  return RepeatForever::create(repeat);
}

Vector<FiniteTimeAction*> ActionUtils::createActionVector(ValueMap& props) {
  Vector<FiniteTimeAction*> actions_vec;

  auto& actions_ref = props.at(__ACTIONS__).asValueVector();
  for_each(actions_ref.begin(), actions_ref.end(),
           [&](auto& action) { actions_vec.pushBack(toFiniteTimeAction(makeAction(action.asValueMap()))); });

  return actions_vec;
}

unordered_map<string, function<Action*(ValueMap)>> ActionUtils::createActionMap() {
  // Create an actionMap instance and add all the values to it
  unordered_map<string, function<Action*(ValueMap)>> action_map;
  action_map.emplace("fadein", fadeInInternal);
  action_map.emplace("fadeout", fadeOutInternal);
  action_map.emplace("fadeto", fadeToInternal);
  action_map.emplace("moveto", moveToInternal);
  action_map.emplace("moveby", moveByInternal);
  action_map.emplace("rotateTo", rotateToInternal);
  action_map.emplace("rotateBy", rotateByInternal);
  action_map.emplace("easein", easeInInternal);
  action_map.emplace("easeout", easeOutInternal);
  action_map.emplace("easeinout", easeInOutInternal);
  action_map.emplace("blink", blinkInternal);
  action_map.emplace("sequence", sequenceInternal);
  action_map.emplace("spawn", spawnInternal);
  action_map.emplace("repeat", repeatInternal);
  action_map.emplace("zoom", zoomInternal);

  // Return ActionMap
  return action_map;
}
