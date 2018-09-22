// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

// Includes
#include "particle_utils.hxx"
#include "utils/archive/archive_utils.hxx"

ParticleSystemQuad* ParticleUtils::load(const string& filename, const float x, const float y,
                                        const bool absolute) {
  // Load the data from the encrypted archive
  auto dict = ArchiveUtils::loadValueMap(filename);

  // Initialize new particle emitter
  auto particle = ParticleSystemQuad::create();
  particle->initWithDictionary(dict);

  // Set position to either absolute or screen
  if (absolute)
    particle->setPosition(x, y);
  else
    particle->setPosition(Globals::getScreenPosition(x, y));

  // Set properties for this particle
  particle->setCascadeOpacityEnabled(true);
  particle->setOpacityModifyRGB(true);
  particle->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);
  return particle;
}

Layout* ParticleUtils::load(initializer_list<string> list, float x, float y, bool absolute) {
  // Creating a container for particles.
  auto container = Layout::create();
  container->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

  // Set position to either absolute or screen
  if (absolute)
    container->setPosition(Vec2(x, y));
  else
    container->setPosition(Globals::getScreenPosition(x, y));

  // Create a particle for each node in the list and added it to the particle container
  for_each(list.begin(), list.end(), [&](const string& file) {
    // Configure particle
    auto particle = load(file, x, y, absolute);
    particle->setPosition(container->getContentSize().width / 2, container->getContentSize().height / 2);
    particle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    // Add particle to container
    container->addChild(particle, 0, file);
  });

  // Return container
  return container;
}

void ParticleUtils::stop(Node* container) {
  for_each(container->getChildren().begin(), container->getChildren().end(),
           [](Node* particle) { dynamic_cast<ParticleSystemQuad*>(particle)->stopSystem(); });
}
