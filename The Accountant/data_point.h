#pragma once

#include "position.h"

#include <unordered_map>

struct DataPoint {
	int id;
	Position position;

	bool operator == (const DataPoint& data_point) const {
		return (id == data_point.id) && (position == data_point.position);
	}
};

std::unordered_map<int, DataPoint>::const_iterator
	GetClosestData(const Position& position,
		const std::unordered_map<int, DataPoint>& data_points);