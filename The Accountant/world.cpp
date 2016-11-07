#include "data_point.h"
#include "enemy.h"
#include "hero.h"
#include "position.h"
#include "world.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

bool World::operator == (const World& world) const {
	bool hero = (wolff_ == world.wolff_);
	bool data = (data_points_ == world.data_points_);
	bool enemies = (enemies_ == world.enemies_);
	return hero && data && enemies;
}

std::unordered_map<int, int> World::MoveEnemies() {
	std::unordered_map<int, int> closest_data_points;
	for (const std::pair<int, Enemy>& enemy : enemies_) {
		Position position = enemy.second.position;
		DataPoint closest_data_point
			= GetClosestData(position, data_points_)->second;
		closest_data_points[enemy.first] = closest_data_point.id;
		if (closest_data_point.position.Distance(position) <= Enemy::step_length) {
			enemies_[enemy.first].position = closest_data_point.position;
		}
		else {
			enemies_[enemy.first].position
				= closest_data_point.position.GoInDirection(position, Enemy::step_length);
		}
	}
	return closest_data_points;
}

void World::CollectData(const std::unordered_map<int, int>& closest_data_points) {
	std::unordered_set<int> data_to_erase;
	for (const std::pair<int, int>& pair_id : closest_data_points) {
		if (enemies_.find(pair_id.first) != enemies_.end()) {
			if (enemies_[pair_id.first].position == data_points_[pair_id.second].position) {
				data_to_erase.insert(pair_id.second);
			}
		}
	}
	for (int id : data_to_erase) {
		data_points_.erase(id);
	}
}
bool World::PerfromHeroAction(const Action& action) {
	bool safe = true;
	switch (action.GetActionType()) {
	case Action::MOVE: {
		wolff_.SetPosition(action.GetPosition());
		if (!wolff_.IsSafe(enemies_)) {
			safe = false;
		}
		break;
	}
	case Action::SHOOT: {
		if (!wolff_.IsSafe(enemies_)) {
			safe = false;
		}
		else {
			statistics_.shoots++;
			int enemy_id = action.GetEnemyId();
			int damage = wolff_.CalculateDamage(enemies_[enemy_id]);
			if (enemies_[enemy_id].life <= damage) {
				enemies_.erase(enemy_id);
			}
			else {
				enemies_[enemy_id].life -= damage;
			}
		}
		break;
	}
	default:
		std::cerr << "Something wrong with the world today." << std::endl;
		safe = false;
	}
	return safe;
}

Action World::BasicHero() const {
	if (enemies_.empty()) {
		return Action(Position(0, 0));
	}
	if (wolff_.IsSafe(enemies_)) {
		// Shoot.
		int closest_enemy_id = GetClosestEnemy(wolff_.GetPosition(), enemies_)->first;
		return Action(closest_enemy_id);
	}
	else {
		// We need to escape.
		Position escape = wolff_.GetWayToEscape(enemies_);
		return Action(escape);
	}
}

bool World::PredictWorld(const Action& action) {
	if (data_points_.empty() || enemies_.empty()) {
		return true;
	}
	auto closest_data_points = MoveEnemies();
	if (!PerfromHeroAction(action)) {
		return false;
	};
	CollectData(closest_data_points);
	return true;
}

bool World::IsTheEnd() const {
	return (enemies_.empty() || data_points_.empty());
}

int World::ComputeScore() const {
	int killed = statistics_.start_enemy_num - enemies_.size();
	return 100 * data_points_.size() + 10 * killed +
		data_points_.size() * std::max(0,
			(statistics_.start_enemy_lifes - 3 * statistics_.shoots)) * 3;
}

std::vector<Action> World::GetAllPossibleActions() const {
	std::vector<Action> all_actions;
	for (const std::pair<int, Enemy>& enemy : enemies_) {
		all_actions.push_back(Action(enemy.first));
	}
	auto all_moves = wolff_.GetAllMoves();
	for (const Position& position : all_moves) {
		all_actions.push_back(Action(position));
	}
	//auto engine = std::default_random_engine{};
	//std::shuffle(all_actions.begin(), all_actions.end(), engine);
	return all_actions;
}