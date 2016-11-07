#pragma once

#include "action.h"
#include "mcts_node.h"
#include "world.h"

#include <memory>
#include <string>

class MCTS {
public:
	MCTS(const World& world) {
		root = std::unique_ptr<MCTSNode>(new MCTSNode(world));
		cur_node = root.get();
	}

	World GetCurrentWorld() const {
		return cur_node->GetCurrentWorld();
	}

	Action Decide();
	void Explore() const;
private:
	std::unique_ptr<MCTSNode> root;
	MCTSNode* cur_node;
};