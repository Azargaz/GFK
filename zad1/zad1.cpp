#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Drawing.h"
#include "font.h"

class Menu : public sf::Drawable
{
private:
	const char* currentOptionsText = "";

	sf::Font font;
	sf::Text *text;
	sf::RectangleShape *rectangle;

	sf::Texture *colors_texture;
	sf::Sprite *colors_sprite;
	sf::Uint8 *colors_pixels;
	const unsigned int colors_size_x = 765;
	const unsigned int colors_size_y = 60;
	inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
	{
		colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
		colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
		colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
		colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
	}

public:
	Menu()
	{
		font.loadFromMemory(font_data, font_data_size);
		text = new sf::Text;
		text->setFont(font);
		text->setCharacterSize(12);
		text->setFillColor(sf::Color::White);

		rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
		rectangle->setFillColor(sf::Color::Transparent);
		rectangle->setOutlineColor(sf::Color::White);
		rectangle->setOutlineThickness(1.0f);
		rectangle->setPosition(2, 62);

		unsigned int x, y;
		colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
		for (x = 0; x < 255; x++)
			for (y = 0; y < 30; y++)
			{
				draw_to_color_pixels(x, y, x, 255, 0);
				draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
				draw_to_color_pixels(x + 510, y, 255, 0, x);
				draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
				draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
				draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
			}

		colors_texture = new sf::Texture();
		colors_texture->create(colors_size_x, colors_size_y);
		colors_texture->update(colors_pixels);

		colors_sprite = new sf::Sprite();
		colors_sprite->setTexture(*colors_texture);
		colors_sprite->setPosition(1, 1);
	}

	void outtextxy(sf::RenderTarget& target, float x, float y, sf::String str) const
	{
		text->setPosition(x, y);
		text->setString(str);
		target.draw(*text);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		outtextxy(target, 5, 600, L"f - wybór koloru rysowania");
		outtextxy(target, 5, 615, L"b - wybór koloru wypełniania");
		outtextxy(target, 5, 630, L"l - rysowanie linii");

		outtextxy(target, 200, 600, L"r - rysowanie prostokąta");
		outtextxy(target, 200, 615, L"a - rysowanie wypełnionego prostokąta");
		outtextxy(target, 200, 630, L"c - rysowanie okręgu");

		outtextxy(target, 470, 600, L"w - zapis do pliku");
		outtextxy(target, 470, 615, L"o - odczyt z pliku");
		outtextxy(target, 470, 630, L"esc - wyjście");

		outtextxy(target, 650, 600, L"Aktualne:");
		outtextxy(target, 650, 615, currentOptionsText);

		target.draw(*rectangle);
		target.draw(*colors_sprite);
	}

	void changeCurrentOptionText(const char* txt)
	{
		currentOptionsText = txt;
	}
};

enum ChosenOption { Line, Rectangle, RectangleNoFill, Circle, PickFGColor, PickBGColor };
const char* OptionsText[] =
{
	"rysowanie linii",
	"rysowanie wypełnionego\nprostokąta",
	"rysowanie prostokąta",
	"rysowanie okręgu",
	"wybór koloru rysowania",
	"wybór koloru wypełniania",
	"zapis do pliku",
	"odczyt z pliku"
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 650), "SFML");
	sf::Event event;
	Menu menu;

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

	sf::CircleShape circle(0);
	circle.setFillColor(sf::Color(255, 0, 0));

	sf::CircleShape* circles = new sf::CircleShape[0];
	int circlesCount = 0;

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(0, 0))
	};
	for (int i = 0; i < 2; i++)
	{
		line[i].color = sf::Color(255, 0, 0);
	}

	sf::Vertex* lines = new sf::Vertex[0];
	int linesCount = 0;

	ChosenOption chosenOption = ChosenOption::Line;
	menu.changeCurrentOptionText(OptionsText[chosenOption]);
	bool drawing = false;

	while (window.isOpen())
	{
		window.clear(sf::Color::Black);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			
			// TEST POBIERANIE KOLORU
			if (event.type == sf::Event::MouseButtonPressed)
			{

			}
			//

			if (!drawing && event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::F:
					chosenOption = ChosenOption::PickFGColor;
					break;
				case sf::Keyboard::B:
					chosenOption = ChosenOption::PickBGColor;
					break;
				case sf::Keyboard::L:
					chosenOption = ChosenOption::Line;
					break;
				case sf::Keyboard::R:
					chosenOption = ChosenOption::RectangleNoFill;
					break;
				case sf::Keyboard::A:
					chosenOption = ChosenOption::Rectangle;
					break;
				case sf::Keyboard::C:
					chosenOption = ChosenOption::Circle;
					break;
				case sf::Keyboard::W:
					// zapis do pliku
					break;
				case sf::Keyboard::O:
					// odczyt z pliku
					break;
				}

				menu.changeCurrentOptionText(OptionsText[chosenOption]);
			}

			if (!drawing && event.type == sf::Event::MouseButtonPressed)
			{
				switch (chosenOption)
				{
				case ChosenOption::Line:
					StartDrawingShape(line, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::Rectangle:
					StartDrawingShape(&rectangle, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::RectangleNoFill:
					StartDrawingShape(&rectangleNoFill, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::Circle:
					StartDrawingShape(&circle, sf::Mouse::getPosition(window));
					break;
				}				
				drawing = true;
			}

			if (drawing && event.type == sf::Event::MouseMoved)
			{
				switch (chosenOption)
				{
				case ChosenOption::Line:
					DrawShape(line+1, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::Rectangle:
					DrawShape(&rectangle, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::RectangleNoFill:
					DrawShape(&rectangleNoFill, sf::Mouse::getPosition(window));
					break;
				case ChosenOption::Circle:
					DrawShape(&circle, sf::Mouse::getPosition(window));
					break;
				}
			}

			if (drawing && event.type == sf::Event::MouseButtonReleased)
			{				
				switch (chosenOption)
				{
				case ChosenOption::Line:
					FinishDrawingShape<sf::Vertex>(line[0], &lines, &linesCount);
					FinishDrawingShape<sf::Vertex>(line[1], &lines, &linesCount);
					break;
				case ChosenOption::Rectangle:
					FinishDrawingShape<sf::RectangleShape>(rectangle, &rectangles, &rectanglesCount);
					break;
				case ChosenOption::RectangleNoFill:
					FinishDrawingShape<sf::RectangleShape>(rectangleNoFill, &rectanglesNoFill, &rectanglesNoFillCount);
					break;
				case ChosenOption::Circle:
					FinishDrawingShape<sf::CircleShape>(circle, &circles, &circlesCount);
					break;
				}
				drawing = false;
			}
		}

		for (int i = 0; i < rectanglesCount; i++)
			window.draw(rectangles[i]);

		for (int i = 0; i < rectanglesNoFillCount; i++)
			window.draw(rectanglesNoFill[i]);

		for (int i = 0; i < circlesCount; i++)
			window.draw(circles[i]);

		for (int i = 0; i < linesCount; i+=2)
			window.draw(lines+i, 2, sf::Lines);

		if (drawing)
		{
			switch (chosenOption)
			{
			case ChosenOption::Line:
				window.draw(line, 2, sf::Lines);
				break;
			case ChosenOption::Rectangle:
				window.draw(rectangle);
				break;
			case ChosenOption::RectangleNoFill:
				window.draw(rectangleNoFill);
				break;
			case ChosenOption::Circle:
				window.draw(circle);
				break;
			}
		}
		window.draw(menu);
		window.display();
	}

	return 0;
}