#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

void StartDrawingShape(sf::CircleShape* shape, sf::Vector2i mousePos)
{
	shape->setPosition(mousePos.x, mousePos.y);
	shape->setRadius(0);
}

void StartDrawingShape(sf::RectangleShape* shape, sf::Vector2i mousePos)
{
	shape->setPosition(mousePos.x, mousePos.y);
	shape->setSize(sf::Vector2f(0, 0));
}

void StartDrawingShape(sf::Vertex* shape, sf::Vector2i mousePos)
{
	shape->position = sf::Vector2f(mousePos);
	(shape+1)->position = sf::Vector2f(mousePos);
}

void DrawShape(sf::RectangleShape* shape, sf::Vector2i mousePos)
{
	sf::Vector2i size = mousePos - sf::Vector2i(shape->getPosition());
	shape->setSize(sf::Vector2f(size.x, size.y));
}

void DrawShape(sf::CircleShape* shape, sf::Vector2i mousePos)
{
	sf::Vector2f relativeMousePos = sf::Vector2f(mousePos) - shape->getPosition();
	float relativeMousePosLength = sqrt(relativeMousePos.x*relativeMousePos.x + relativeMousePos.y*relativeMousePos.y);
	float radius = 0.5f * relativeMousePosLength;
	float angle = atan2(relativeMousePos.y, relativeMousePos.x) * (180.0f / (atan(1) * 4)) - 45.0f; // atan(1) * 4 = PI, pi z math.h nie działa?
	shape->setRotation(angle);
	shape->setRadius(radius);
}

void DrawShape(sf::Vertex* shape, sf::Vector2i mousePos)
{
	shape->position = sf::Vector2f(mousePos);
}

template <class T>
void IncreaseShapeArraySize(T** shapes, int newSize)
{
	T* copy = *shapes;
	*shapes = new T[newSize];
	for (int i = 0; i < newSize - 1; i++)
	{
		(*shapes)[i] = copy[i];
	}
	delete[] copy;
}

template<class T>
void FinishDrawingShape(T& shape, T** shapes, int* shapesCount)
{
	(*shapesCount)++;
	IncreaseShapeArraySize<T>(shapes, *shapesCount);
	(*shapes)[(*shapesCount) - 1] = T(shape);
}

template<class T>
void InitializeShape(T* shape)
{
	shape->setFillColor(sf::Color(255, 0, 0));
}