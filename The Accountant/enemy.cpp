#include "enemy.h"
#include "position.h"

#include <algorithm>
#include <unordered_map>

std::unordered_map<int, Enemy>::const_iterator 
			GetClosestEnemy(const Position& position,
				const std::unordered_map<int, Enemy>& enemies) {
	auto iter = std::min_element(enemies.begin(), enemies.end(),
		[position](const std::pair<int, Enemy>& first,
			const std::pair<int, Enemy>& second) {
		return position.Distance(first.second.position) <
			position.Distance(second.second.position);
	});

	return iter;
}