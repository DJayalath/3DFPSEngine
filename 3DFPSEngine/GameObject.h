#pragma once

#include <vector>
#include "GameComponent.h"
#include "Transform.h"
using namespace std;

class GameObject
{
private:
	vector<GameObject*> children;
	vector<GameComponent*> components;
	Transform transform;

public:
	GameObject();

	void AddChild(GameObject& child); // MAYBE BY REFERENCE?
	void AddComponent(GameComponent& component);

	void Input();
	void Update();
	void Render();

	Transform& GetTransform();
};

