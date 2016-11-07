#include "data_point.h"
#include "position.h"

#include <algorithm>
#include <unordered_map>

std::unordered_map<int, DataPoint>::const_iterator
	GetClosestData(const Position& position,
		const std::unordered_map<int, DataPoint>& data_points) {
	auto iter = std::min_element(data_points.begin(), data_points.end(),
		[position](const std::pair<int, DataPoint>& first,
			const std::pair<int, DataPoint>& second) {
		return position.Distance(first.second.position) <
			position.Distance(second.second.position);
	});
	return iter;
}