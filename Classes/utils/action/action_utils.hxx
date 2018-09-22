/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __ACTION_UTILS_HXX__
#define __ACTION_UTILS_HXX__

/**
 * Includes
 */
#include "globals.hxx"

class ActionUtils {
 public:
  /**
   * All action public to the rest of the application
   */
  static Action* fadeIn(float delay, float duration);

  static Action* fadeOut(float delay, float duration);

  static Action* fadeTo(float delay, float duration, int opacity, float rate);

  static Action* easeIn(float delay, float duration, float pos_x, float pos_y, float rate);

  static Action* easeOut(float delay, float duration, float pos_x, float pos_y, float rate);

  static Action* easeInOut(float delay, float duration, float pos_x, float pos_y, float rate);

  static Action* moveTo(float delay, float duration, float pos_x, float pos_y);

  static Action* moveBy(float delay, float duration, float pos_x, float pos_y);

  static Action* rotateTo(float delay, float duration, float degree, float rate);

  static Action* rotateBy(float delay, float duration, float degree, float rate);

  static Action* zoom(float delay, float duration, float scale, float rate);

  static Action* blink(float delay, float duration, int repeat);

  static Action* spawn(initializer_list<Action*> actions);

  static Action* sequence(initializer_list<Action*> actions);

  /**
   * Maps a defined string in the `props` parameter to the appropriate action function.
   *
   * @param props ~ properties needed to create action.
   *
   * @returns ~ New Action*.
   */
  static Action* makeAction(ValueMap props);

  /**
   * Converts an Action* to a FiniteTimeAction*.
   *
   * @param action ~ Action to be converted.
   *
   * @returns ~ Converted action.
   */
  static FiniteTimeAction* toFiniteTimeAction(Action* action);

  /**
   * Checks a given node to verify if there is actions on it
   *
   * @param node ~ Node to be checked
   *
   * @returns ~ If the node is running any actions
   */
  static bool isRunningAction(const Node* node);

  /**
   * Checks a given node to verify if there is actions on it
   *
   * @param nodes ~ Nodes to be checked
   *
   * @returns ~ If any node is running any actions
   */
  static bool isRunningAction(initializer_list<Node*> nodes);

 protected:
  /**
   * Internal implementations of the public actions. This is also used by the
   */
  static Action* fadeInInternal(ValueMap props);

  static Action* fadeOutInternal(ValueMap props);

  static Action* fadeToInternal(ValueMap props);

  static Action* moveToInternal(ValueMap props);

  static Action* moveByInternal(ValueMap props);

  static Action* rotateToInternal(ValueMap props);

  static Action* rotateByInternal(ValueMap props);

  static Action* easeInInternal(ValueMap props);

  static Action* easeOutInternal(ValueMap props);

  static Action* easeInOutInternal(ValueMap props);

  static Action* zoomInternal(ValueMap props);

  static Action* blinkInternal(ValueMap props);

  static Action* sequenceInternal(ValueMap props);

  static Action* spawnInternal(ValueMap props);

  static Action* repeatInternal(ValueMap props);

  /**
   * Creates multiple actions. This will the be used by spawn or sequenced actions
   * to either play at the same time or one after another.
   *
   * @param props ~ ValueMap containing multiple other maps for actions.
   *
   * @returns ~ A vector of FiniteTimeActions
   */
  static Vector<FiniteTimeAction*> createActionVector(ValueMap& props);

  /**
   * Gets the current instance of this class.
   *
   * @returns ~ temparay map to actions.
   */
  static unordered_map<string, function<Action*(ValueMap)>> createActionMap();

 private:
  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(ActionUtils)
};

#endif  // __ACTION_UTILS_HXX__
