/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __PARTICLE_UTILS_HXX__
#define __PARTICLE_UTILS_HXX__

/**
 * Includes
 */
#include "globals.hxx"
#include "ui/CocosGUI.h"

/**
 * Namespaces
 */
using namespace ui;

class ParticleUtils {
 public:
  /**
   * Loads a particle system and auto sets it position.
   *
   * @param filename ~ Name of the file in the archive.
   * @param x        ~ X coordinate.
   * @param y        ~ Y coordinate.
   * @param absolute ~ if we should place it in absolute positioning of div positioning.
   *
   * @returns ~ New paritcle system.
   */
  static ParticleSystemQuad* load(const string& filename, float x = 2, float y = 2, bool absolute = false);

  /**
   * Loads all pariticels in the list and add them to a node container, creating a "compound" particle.
   *
   * @param list     ~ List on names of particle files
   * @param x        ~ X coordinate.
   * @param y        ~ Y coordinate.
   * @param absolute ~ if we should place it in absolute positioning of div positioning.
   *
   * @returns ~ Container paritcle system
   */
  static Layout* load(initializer_list<string> list, float x = 2, float y = 2, bool absolute = false);

  /**
   * Stop all pariticels in the node container.
   *
   * @param container ~ Node containing more than one particle system.
   */
  static void stop(Node* container);

 private:
  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(ParticleUtils)
};

#endif  // __PARTICLE_UTILS_HXX__
