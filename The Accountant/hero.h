#pragma once

#include "data_point.h"
#include "enemy.h"
#include "position.h"

#include <string>
#include <unordered_map>

class Hero {
public:
	Hero() {}
	Hero(const Position& position) : position_(position) {}

	Hero operator = (const Hero& hero) {
		position_ = hero.position_;
		return *this;
	}

	const Position GetPosition() const { return position_; }

	void SetPosition(const Position& position) { position_ = position; }

	bool operator == (const Hero& hero) const {
		return position_ == hero.position_;
	}

	int CalculateDamage(Enemy enemy) const {
		return int(round(125000.0 / pow(position_.Distance(enemy.position), 1.2)));
	}

	std::vector<Position> GetAllMoves() const;

	Position GetWayToEscape(const std::unordered_map<int, Enemy>& enemies) const;

	bool IsSafe(const std::unordered_map<int, Enemy>& enemies) const;
private:
	Position position_;

	const int num_sectors = 8;
	const int step_length = 1000;
};
