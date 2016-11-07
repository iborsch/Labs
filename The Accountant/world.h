#pragma once

#include "action.h"
#include "data_point.h"
#include "enemy.h"
#include "hero.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map> 

class World {
public:
	struct Statistics {
		int shoots = 0;
		int start_enemy_num = 0;
		int start_enemy_lifes = 0;
	};

	World() {}
	World(const Hero& wolff, const std::unordered_map<int, Enemy>& enemies,
		const std::unordered_map<int, DataPoint>& data_points)
		: wolff_(wolff), enemies_(enemies), data_points_(data_points) {};

	void CopyWorld(const World& world) {
		wolff_ = world.wolff_;
		enemies_ = world.enemies_;
		data_points_ = world.data_points_;
	}

	Hero GetHero() const { return wolff_; }
	std::unordered_map<int, Enemy> GetEnemies() const { return enemies_; }
	std::unordered_map<int, DataPoint> GetDataPoints() const { return data_points_; }
	Statistics GetStatistics() const { return statistics_; }

	void SetStartEnemyLifes(int start_enemy_lifes) {
		statistics_.start_enemy_lifes = start_enemy_lifes;
	}
	void SetStartEnemyNum(int start_enemy_num) {
		statistics_.start_enemy_num = start_enemy_num;
	}

	bool operator == (const World& world) const;

	std::unordered_map<int, int> MoveEnemies();
	void CollectData(const std::unordered_map<int, int>& closest_data_points);
	bool PerfromHeroAction(const Action& action);
	bool PredictWorld(const Action& action);

	Action BasicHero() const;

	bool IsTheEnd() const;

	int ComputeScore() const;

	std::vector<Action> GetAllPossibleActions() const;
private:
	Hero wolff_;
	std::unordered_map<int, Enemy> enemies_;
	std::unordered_map<int, DataPoint> data_points_;

	Statistics statistics_;

};
