#pragma once

#include "action.h"
#include "world.h"

#include <map>
#include <memory>
#include <string>

class MCTSNode {
public:
	typedef std::pair<MCTSNode*, Action> ValueType;
	MCTSNode() {};
	MCTSNode(const World& world) : cur_world_(world) {}

	World GetCurrentWorld() const { return cur_world_; }

	int Visit();
	int BasicHeroScore() const;

	double CalculateUCB1(int total_visits, double average_best_score) {
		if (visited_ == 0) {
			return INT_MAX;
		}
		return best_score_ / average_best_score + sqrt(2 * log(total_visits) / visited_);
	}

	ValueType BestBranch() const;
	ValueType ChooseNextBranch() const;
private:
	std::map<MCTSNode*, Action> next_worlds_;
	World cur_world_;
	int best_score_ = 0;
	double average_best_score_ = 0;
	int visited_ = 0;
};
