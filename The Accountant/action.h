#pragma once

#include "position.h"

class Action {
public:
	enum ActionType {
		MOVE = 0,
		SHOOT,
	};

	Action() : action_type_(MOVE), position_(Position(0, 0)) {}
	Action(const Position& position) : action_type_(MOVE), position_(position) {}
	Action(int enemy_id) : enemy_id_(enemy_id) {};

	ActionType GetActionType() const { return action_type_; }
	Position GetPosition() const { return position_; }
	int GetEnemyId() const { return enemy_id_;  }

	bool operator < (const Action& action) const {
		return action_type_ < action.action_type_;
	}

	std::string ToString() const {
		switch (action_type_) {
		case MOVE:
			return "MOVE" + position_.ToString();
		case SHOOT:
			return "SHOOT" + std::to_string(enemy_id_);
		default:
			return "";
		} 
	}

private:
	ActionType action_type_;
	Position position_;
	int enemy_id_;

};