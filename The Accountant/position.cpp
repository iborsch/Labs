#include "position.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

bool Position::operator == (const Position& position) const {
	return x_ == position.x_ && y_ == position.y_;
}

std::ostream& operator << (std::ostream& os, const Position& position) {
	os << position.x_ << " " << position.y_;
	return os;
}

double Position::Distance(const Position& position) const {
	return sqrt((x_ - position.x_)*(x_ - position.x_)
		+ (y_ - position.y_)*(y_ - position.y_));
}

int Position::GetClosestPositionIndex(const std::vector<Position>& positions) const {
	auto iter = std::min_element(positions.begin(), positions.end(),
		[this](const Position& first, const Position& second) {
		return Distance(first) < Distance(second);
	});
	return (iter - positions.begin());
}

Position Position::GoInDirection(const Position& position, int step_length) const {
	double distance = Distance(position);
	int x = int(floor(step_length * x_ / distance
		- (step_length - distance) * position.x_ / distance));
	int y = int(floor(step_length * y_ / distance
		- (step_length - distance) * position.y_ / distance));
	x = std::min(std::max(0, x), MAX_X);
	y = std::min(std::max(0, y), MAX_Y);
	return Position(x, y);
}

Position Position::GoWithAngle(double x_rad, int step_length) const {
	double x_db = cos(x_rad)*step_length + x_;
	double y_db = sin(x_rad)*step_length + y_;
	int x = std::min(std::max(0, int(floor(x_db))), MAX_X);
	int y = std::min(std::max(0, int(floor(y_db))), MAX_Y);
	return Position(x, y);
}