#pragma once

#include "position.h"

#include <unordered_map>

struct Enemy {
	int id;
	Position position;
	int life;
	bool operator == (const Enemy& enemy) const {
		return (id == enemy.id) && (position == enemy.position)
			&& (life == enemy.life);
	}
	static const int step_length = 500;
	static const int notice_length = 2000;
};

std::unordered_map<int, Enemy>::const_iterator
		GetClosestEnemy(const Position& position,
			const std::unordered_map<int, Enemy>& enemies);