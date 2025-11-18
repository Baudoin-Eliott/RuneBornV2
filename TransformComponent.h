#pragma once
#include "Components.h"
#include "vector2D.h"

class TransformComponent : public Component {



public:

	Vector2D position;
	Vector2D velocity;

	int height = 16;
	int width = 16;
	float scale = 2.0f;

	TransformComponent() {

		position.Zero();
	}

	TransformComponent(int sc) {
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;

	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override {
		velocity.Zero();
	}

	void update() override {};
};