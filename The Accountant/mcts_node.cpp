#include "hero.h"
#include "mcts_node.h"
#include "world.h"

#include <algorithm>
#include <map>
#include <memory>
#include <random>
#include <vector>
#include <string>

int MCTSNode::BasicHeroScore() const {
	World basic_world = cur_world_;
	while (!basic_world.IsTheEnd()) {
		auto closest_data_points = basic_world.MoveEnemies();
		Action action = basic_world.BasicHero();
		bool safe = basic_world.PerfromHeroAction(action);
		if (!safe) {
			return 0;
		}
		basic_world.CollectData(closest_data_points);
	}
	return basic_world.ComputeScore();
}

int MCTSNode::Visit() {
	int cur_score = 0;
	if (visited_ == 0 || next_worlds_.empty()) {
		cur_score = BasicHeroScore();
		auto all_actions = cur_world_.GetAllPossibleActions();
		for (const Action& action : all_actions) {
			World new_world = cur_world_;
			bool safe = new_world.PredictWorld(action);
			if (safe) {
				auto new_node = std::unique_ptr<MCTSNode>(new MCTSNode(new_world));
				next_worlds_[new_node.get()] = action;
			}
		}
	}
	else {
		cur_score = ChooseNextBranch().first->Visit();
	}
	best_score_ = std::max(best_score_, cur_score);
	average_best_score_ = (average_best_score_*visited_ + best_score_) / (visited_ + 1);
	visited_++;
	return best_score_;
}

MCTSNode::ValueType MCTSNode::BestBranch() const {
	auto max_iter = std::max_element(next_worlds_.begin(), next_worlds_.end(),
		[](const ValueType& first, const ValueType& second) {
		return first.first->best_score_ < second.first->best_score_;
	});
	int best_score = max_iter->first->best_score_;
	std::vector<ValueType> max_elements;
	for (const ValueType& world : next_worlds_) {
		if (world.first->best_score_ == best_score) {
			max_elements.push_back(world);
		}
	}
	auto found_iter = find_if(max_elements.begin(), max_elements.end(), [](const ValueType& world) {
		return world.second.GetActionType() == Action::SHOOT;
	});
	if (found_iter == max_elements.end()) {
		return max_elements[0];
	}
	else {
		return *found_iter;
		std::cerr << "SHOOT choosed." << std::endl;
	}
}

MCTSNode::ValueType MCTSNode::ChooseNextBranch() const {
	std::map<ValueType, double> cum_sums;
	double cur_score = 0;
	for (const auto& world : next_worlds_) {
		cur_score += world.first->CalculateUCB1(visited_, average_best_score_);
		cum_sums[world] = cur_score;
	}

	int random_bound = rand() % (int(cur_score) + 1);
	auto min_iter = std::min_element(cum_sums.begin(), cum_sums.end(),
		[&random_bound](const std::pair<ValueType, double>& first,
			const std::pair<ValueType, double>& second) {
		return abs(first.second - random_bound) < abs(second.second - random_bound);
	});
	return min_iter->first;
}