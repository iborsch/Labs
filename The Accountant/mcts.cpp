#include "action.h"
#include "mcts.h"

#include <string>

Action MCTS::Decide() {
	auto best_branch = cur_node->BestBranch();
	cur_node = best_branch.first;
	return best_branch.second;
}

void MCTS::Explore() const {
	cur_node->Visit();
}