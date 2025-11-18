#pragma once
#include "Components.h"


class MovementComponent : public Component {

public:
	int speed = 4;
	int acceleration = 0;
	bool canMove = true;

	MovementComponent() = default;
	MovementComponent(int s) : speed(s) {};

	void init() override {};
	void update() override {};

};