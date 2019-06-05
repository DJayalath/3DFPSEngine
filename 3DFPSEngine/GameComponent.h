#pragma once

#include "Transform.h"
#include <iostream>

class GameComponent
{
public:

	virtual void Input(Transform transform) {}

	virtual void Update(Transform transform) {}

	virtual void Render(Transform transform) { /*std::cout << "GC Rendering..." << std::endl;*/ }
};

