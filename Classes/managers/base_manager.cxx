// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "base_manager.hxx"

BaseManager::BaseManager(Layer& layer) : main_layer_(layer) {}

BaseManager::~BaseManager() {
  // Unload all the node in the node map. first, cancel all action on the node if any
  for_each(node_map_.begin(), node_map_.end(), [&](const auto& node) { node.second->stopAllActions(); });

  // Clear action map
  node_map_.clear();
}

void BaseManager::addActionFromProps(Node* node, ValueMap props) {
  // Check if properties contains an action key. If true, we set the described action and pass it to node.
  if (props.find(__ACTION__) not_eq props.end())
    node->runAction(ActionUtils::makeAction(props.at(__ACTION__).asValueMap()));
}

Node* BaseManager::findNode(const string& name) {
  // When searching for a node, We expect the node to exist so asset this is true
  const auto itr = find_if(node_map_.begin(), node_map_.end(),
                           [name](const pair<string, Node*> pair) { return name == pair.first; });
  CCASSERT(itr != node_map_.end(), "Cannot find node in node map");

  // Return pointer to this node
  return itr->second;
}
