#include "data_point.h"
#include "enemy.h"
#include "hero.h"
#include "position.h"

#include <algorithm>
#include <unordered_map>

std::vector<Position> Hero::GetAllMoves() const {
	std::vector<Position> all_moves;
	double pi = atan(1) * 4;
	for (int coeff = 0; coeff < num_sectors; ++coeff) {
		double angle = 2 * pi / num_sectors * coeff;
		all_moves.push_back(
			position_.GoWithAngle(angle, step_length));
	}
	return all_moves;
}

Position Hero::GetWayToEscape(const std::unordered_map<int, Enemy>& enemies) const {
	std::vector<Position> all_moves = GetAllMoves();
	auto iter = std::max_element(all_moves.begin(), all_moves.end(),
		[enemies](const Position& first, const Position& second) {
		auto first_closest = GetClosestEnemy(first, enemies);
		auto second_closest = GetClosestEnemy(second, enemies);
		return first.Distance(first_closest->second.position)
			< second.Distance(second_closest->second.position);
	});
	return *iter;
}

bool Hero::IsSafe(const std::unordered_map<int, Enemy>& enemies) const {
	if (enemies.empty()) {
		return true;
	}
	auto closest_enemy = GetClosestEnemy(position_, enemies);
	return position_.Distance(closest_enemy->second.position)
			> Enemy::notice_length;
}