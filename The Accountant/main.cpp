#include "action.h"
#include "data_point.h"
#include "enemy.h"
#include "hero.h"
#include "mcts.h"
#include "position.h"
#include "world.h"

#include <ctime>
#include <iostream>
#include <unordered_map> 

/**
* Shoot enemies before they collect all the incriminating data!
* The closer you are to an enemy, the more damage you do but don't get too close or you'll get killed.
**/

double GetDurationMS(clock_t start, clock_t finish) {
	return (finish - start) * 1000.0 / CLOCKS_PER_SEC;
}

int main()
{
	World predicted_world;
	std::unique_ptr<MCTS> tree_search;
	bool first_round = true;
	int timeout_ms = 1000;
	// game loop
	while (1) {
		auto start_turn = clock();
		int x;
		int y;
		std::cin >> x >> y;
		std::cin.ignore();

		Hero wolff({ Position(x, y) });

		int dataCount;
		std::cin >> dataCount;
		std::cin.ignore();
		std::unordered_map<int, DataPoint> data_points;
		for (int i = 0; i < dataCount; i++) {
			int dataId;
			int dataX;
			int dataY;
			std::cin >> dataId >> dataX >> dataY;
			std::cin.ignore();
			data_points[dataId] = DataPoint({ dataId, Position(dataX, dataY) });
		}
		int enemyCount;
		std::cin >> enemyCount;
		std::cin.ignore();
		std::unordered_map<int, Enemy> enemies;
		for (int i = 0; i < enemyCount; i++) {
			int enemyId;
			int enemyX;
			int enemyY;
			int enemyLife;
			std::cin >> enemyId >> enemyX >> enemyY >> enemyLife;
			std::cin.ignore();
			enemies[enemyId] = Enemy({ enemyId, Position(enemyX, enemyY), enemyLife });
		}

		World actual_world(wolff, enemies, data_points);

		if (first_round) {
			int startEnemyLifes = 0;
			for (const std::pair<int, Enemy>& enemy : enemies) {
				startEnemyLifes += enemy.second.life;
			}
			World new_world = actual_world;
			new_world.SetStartEnemyLifes(startEnemyLifes);
			new_world.SetStartEnemyNum(enemies.size());
			tree_search = std::unique_ptr<MCTS>(new MCTS(new_world));
			first_round = false;
			timeout_ms = 100;

			predicted_world.CopyWorld(actual_world);
			predicted_world.SetStartEnemyLifes(startEnemyLifes);
			predicted_world.SetStartEnemyNum(enemies.size());
		}


		if (tree_search->GetCurrentWorld() == actual_world) {
			std::cerr << "OK" << std::endl;
		}
		else {
			std::cerr << "BAD" << std::endl;
			predicted_world.CopyWorld(actual_world);
		}

		// Write an action using cout. DON'T FORGET THE "<< std::endl"
		// To debug: std::cerr << "Debug messages..." << std::endl;

		double previous_explore_duration = 0;
		auto current_time = clock();
		while (GetDurationMS(start_turn, current_time) <
			timeout_ms - (previous_explore_duration + 5)) {
			auto before_explore = clock();
			tree_search->Explore();
			current_time = clock();
			previous_explore_duration = GetDurationMS(before_explore, current_time);
		}

		Action action = tree_search->Decide();
		std::cout << action.ToString() << std::endl; // MOVE x y or SHOOT enemyId 

		if (predicted_world.IsTheEnd()) {
			std::cerr << "THE END." << std::endl;
			std::cerr << "Score: " << predicted_world.ComputeScore() << std::endl;
		}

		auto finish_turn = clock();
		auto duration = finish_turn - start_turn;
	}
}