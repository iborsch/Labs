#pragma once

#include <iostream>
#include <string>
#include <vector>

class Position {
public:
	Position() : x_(-1), y_(-1) {}
	Position(int x, int y) : x_(x), y_(y) {}
	Position(const Position& position) {
		x_ = position.x_;
		y_ = position.y_;
	}

	int GetX() const { return x_; }
	int GetY() const { return y_; }

	Position operator = (const Position& position) {
		x_ = position.x_;
		y_ = position.y_;
		return *this;
	}

	bool operator == (const Position& position) const;

	friend  std::ostream& operator << (std::ostream& os, const Position& position);
	std::string ToString() const {
		return std::to_string(x_) + " " + std::to_string(y_);
	}

	double Distance(const Position& position) const;
	int GetClosestPositionIndex(const std::vector<Position>& positions) const;

	// Goes from position to (*this).
	Position GoInDirection(const Position& position, int step_length) const;
	Position GoWithAngle(double x_rad, int step_length) const;
private:
	int x_;
	int y_;

	const int MAX_X = 15999;
	const int MAX_Y = 8999;
};