#include "GameObject.h"

GameObject::GameObject()
{

}

void GameObject::AddChild(GameObject& child)
{
	children.push_back(&child);
}

void GameObject::AddComponent(GameComponent& component)
{
	components.push_back(&component);
}

void GameObject::Input()
{
	for (auto& component : components)
		component->Input(transform);

	for (auto& child : children)
		child->Input();
}

void GameObject::Update()
{
	for (auto& component : components)
		component->Update(transform);

	for (auto& child : children)
		child->Update();
}

void GameObject::Render()
{
	for (auto& component : components)
		component->Render(transform);

	for (auto& child : children)
		child->Render();
}

Transform& GameObject::GetTransform()
{
	return transform;
}
