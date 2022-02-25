#ifndef DF_PLAYER_H
#define DF_PLAYER_H

#include "Object.h"

class Player : public df::Object {

private:

	int ID{};
	df::Vector acceleration;
	float jumpspeed{};
	int shoot_countdown{};
	int shoot_slowdown{};
	int walkingcountdown{};

public:

	Player();

	Player(int ID);

	int eventHandler(const df::Event *p_e) override;

	bool onGround();

	int jump();

	void shoot(df::Vector target);

	int getID() const;

	void setID(int id);
};

#endif //DF_PLAYER_H
