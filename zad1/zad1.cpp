#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>

template <class T>
void StartDrawingShape(T* shape, sf::Vector2i mousePos)
{
	shape->setPosition(mousePos.x, mousePos.y);
	shape->setSize(sf::Vector2f(0, 0));
}

template <class T>
void DrawShape(T* shape, sf::Vector2i mousePos)
{
	sf::Vector2i recSize = mousePos - sf::Vector2i(shape->getPosition());
	shape->setSize(sf::Vector2f(recSize.x, recSize.y));
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

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML");
	sf::Event event;

	window.setFramerateLimit(60);

	sf::RectangleShape rectangle(sf::Vector2f(0, 0));
	InitializeShape<sf::RectangleShape>(&rectangle);

	sf::RectangleShape* rectangles = new sf::RectangleShape[0];
	int rectanglesCount = 0;

	sf::RectangleShape rectangleNoFill(sf::Vector2f(0, 0));
	rectangleNoFill.setFillColor(sf::Color(0, 0, 0, 0));
	rectangleNoFill.setOutlineColor(sf::Color(255, 0, 0));
	rectangleNoFill.setOutlineThickness(1);

	sf::RectangleShape* rectanglesNoFill = new sf::RectangleShape[0];
	int rectanglesNoFillCount = 0;

	bool drawing = false;

	window.clear(sf::Color::Black);

	while (window.isOpen())
	{
		window.clear();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (!drawing && event.type == sf::Event::MouseButtonPressed)
			{
				StartDrawingShape<sf::RectangleShape>(&rectangle, sf::Mouse::getPosition(window));
				drawing = true;
			}

			if (drawing && event.type == sf::Event::MouseMoved)
			{
				DrawShape<sf::RectangleShape>(&rectangle, sf::Mouse::getPosition(window));
			}

			if (drawing && event.type == sf::Event::MouseButtonReleased)
			{		
				FinishDrawingShape<sf::RectangleShape>(rectangle, &rectangles, &rectanglesCount);
				drawing = false;
			}
		}

		if (drawing)
			window.draw(rectangle);

		for (int i = 0; i < rectanglesCount; i++)
		{
			window.draw(rectangles[i]);
		}
		window.display();
	}

	return 0;
}