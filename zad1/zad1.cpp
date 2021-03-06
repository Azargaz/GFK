#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Drawing.h"
#include "font.h"

class Menu : public sf::Drawable
{
private:
	const wchar_t* currentOptionsText = L"";

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

	sf::Texture *picked_colors_texture;
	sf::Sprite *picked_colors_sprite;
	sf::Uint8 *picked_colors_pixels;
	const unsigned int picked_colors_size_x = 35;
	const unsigned int picked_colors_size_y = 60;
	inline void draw_to_picked_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
	{
		picked_colors_pixels[4 * (y * picked_colors_size_x + x) + 0] = r;
		picked_colors_pixels[4 * (y * picked_colors_size_x + x) + 1] = g;
		picked_colors_pixels[4 * (y * picked_colors_size_x + x) + 2] = b;
		picked_colors_pixels[4 * (y * picked_colors_size_x + x) + 3] = 255;
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

		picked_colors_pixels = nullptr;
		picked_colors_texture = nullptr;
		picked_colors_sprite = nullptr;
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
		target.draw(*picked_colors_sprite);
	}

	void changeCurrentOptionText(const wchar_t* txt)
	{
		currentOptionsText = txt;
	}

	// Returns color from color palette
	sf::Color getColorFromPosition(unsigned int x, unsigned int y)
	{		
		if (x > 764 || y > 60)
		{
			return sf::Color(0, 0, 0, 0);
		}

		sf::Uint8 r = colors_pixels[4 * (y * colors_size_x + x) + 0];
		sf::Uint8 g = colors_pixels[4 * (y * colors_size_x + x) + 1];
		sf::Uint8 b = colors_pixels[4 * (y * colors_size_x + x) + 2];
		return sf::Color(r, g, b);		
	}

	void UpdatePickedColors(sf::Color fgColor, sf::Color bgColor)
	{
		delete[] picked_colors_pixels;
		delete picked_colors_texture;
		delete picked_colors_sprite;

		unsigned int x, y;
		picked_colors_pixels = new sf::Uint8[picked_colors_size_x * picked_colors_size_y * 4];
		for (x = 0; x < 35; x++)
			for (y = 0; y < 30; y++)
			{
				draw_to_picked_color_pixels(x, y, fgColor.r, fgColor.g, fgColor.b);
				draw_to_picked_color_pixels(x, y+30, bgColor.r, bgColor.g, bgColor.b);
			}

		picked_colors_texture = new sf::Texture();
		picked_colors_texture->create(picked_colors_size_x, picked_colors_size_y);
		picked_colors_texture->update(picked_colors_pixels);

		picked_colors_sprite = new sf::Sprite();
		picked_colors_sprite->setTexture(*picked_colors_texture);
		picked_colors_sprite->setPosition(765, 1);
	}
};

// Available shapes to store in which order they must be drawn
enum DrawableShape { LineShape, RectangleShape, RectangleNoFillShape, CircleShape };

// Available options
enum Options { DrawLine, DrawRectangle, DrawRectangleNoFill, DrawCircle, PickFGColor, PickBGColor };

// Text to display when certain option is chosen
const wchar_t* OptionsText[] =
{
	L"rysowanie linii",
	L"rysowanie wypełnionego\nprostokąta",
	L"rysowanie prostokąta",
	L"rysowanie okręgu",
	L"wybór koloru rysowania",
	L"wybór koloru wypełniania",
	L"zapis do pliku",
	L"odczyt z pliku"
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 650), "SFML");
	sf::Event event;
	Menu menu;
	sf::Texture loadedImage;
	sf::Sprite loadedImgSprite;
	const char* savedImgName = "obrazek.png";

	bool drawing = false;
	bool saveToFile = false;

	// Array of order in which shapes have to be drawn
	DrawableShape* shapesOrder = new DrawableShape[0];
	int shapesDrawnCount = 0;

	// Setting up rectangles' variables
	sf::RectangleShape rectangle(sf::Vector2f(0, 0));
	sf::RectangleShape* rectangles = new sf::RectangleShape[0];
	int rectanglesCount = 0;

	sf::RectangleShape rectangleNoFill(sf::Vector2f(0, 0));
	rectangleNoFill.setFillColor(sf::Color(0, 0, 0, 0));
	rectangleNoFill.setOutlineThickness(1);
	sf::RectangleShape* rectanglesNoFill = new sf::RectangleShape[0];
	int rectanglesNoFillCount = 0;

	// Setting up circles' variables
	sf::CircleShape circle(0);
	sf::CircleShape* circles = new sf::CircleShape[0];
	int circlesCount = 0;

	// Setting up lines' variables
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(0, 0))
	};
	sf::Vertex* lines = new sf::Vertex[0];
	int linesCount = 0;
	
	// Setting up colors
	sf::Color foregroundColor(sf::Color::White);
	sf::Color backgroundColor(sf::Color::White);
	SetShapesFillColor<sf::RectangleShape>(&rectangle, foregroundColor);
	SetShapesOutlineColor<sf::RectangleShape>(&rectangle, backgroundColor);
	SetShapesOutlineColor<sf::RectangleShape>(&rectangleNoFill, foregroundColor);
	SetShapesFillColor<sf::CircleShape>(&circle, foregroundColor);
	SetShapesOutlineColor<sf::CircleShape>(&circle, backgroundColor);
	SetLinesColor(line, foregroundColor, backgroundColor);
	menu.UpdatePickedColors(foregroundColor, backgroundColor);
	
	// Options like drawing, picking colors, saving/loading image
	Options chosenOption = Options::DrawLine;
	menu.changeCurrentOptionText(OptionsText[chosenOption]);

	window.setFramerateLimit(60);

	// Main loop
	while (window.isOpen())
	{
		window.clear(sf::Color::Black);

		// Events
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			// Switch current option by pressing specified Key
			if (!drawing && event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				default:
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::F:
					chosenOption = Options::PickFGColor;
					break;
				case sf::Keyboard::B:
					chosenOption = Options::PickBGColor;
					break;
				case sf::Keyboard::L:
					chosenOption = Options::DrawLine;
					break;
				case sf::Keyboard::R:
					chosenOption = Options::DrawRectangleNoFill;
					break;
				case sf::Keyboard::A:
					chosenOption = Options::DrawRectangle;
					break;
				case sf::Keyboard::C:
					chosenOption = Options::DrawCircle;
					break;
				case sf::Keyboard::W:
					saveToFile = true;
					break;
				case sf::Keyboard::O:
					loadedImage.loadFromFile(savedImgName);
					loadedImgSprite.setTexture(loadedImage);
					break;
				}
				menu.changeCurrentOptionText(OptionsText[chosenOption]);
			}

			// Start drawing or pick a color if not already drawing
			if (!drawing && event.type == sf::Event::MouseButtonPressed)
			{
				switch (chosenOption)
				{
				default:
					break;
				case Options::DrawLine:
					StartDrawingShape(line, sf::Mouse::getPosition(window));
					break;
				case Options::DrawRectangle:
					StartDrawingShape(&rectangle, sf::Mouse::getPosition(window));
					break;
				case Options::DrawRectangleNoFill:
					StartDrawingShape(&rectangleNoFill, sf::Mouse::getPosition(window));
					break;
				case Options::DrawCircle:
					StartDrawingShape(&circle, sf::Mouse::getPosition(window));
					break;
				case Options::PickFGColor:
					foregroundColor = menu.getColorFromPosition(mousePos.x, mousePos.y);
					SetShapesFillColor<sf::RectangleShape>(&rectangle, foregroundColor);
					SetShapesOutlineColor<sf::RectangleShape>(&rectangle, backgroundColor);
					SetShapesOutlineColor<sf::RectangleShape>(&rectangleNoFill, foregroundColor);
					SetShapesFillColor<sf::CircleShape>(&circle, foregroundColor);
					SetShapesOutlineColor<sf::CircleShape>(&circle, backgroundColor);
					SetLinesColor(line, foregroundColor, backgroundColor);
					menu.UpdatePickedColors(foregroundColor, backgroundColor);
					break;				
				case Options::PickBGColor:
					backgroundColor = menu.getColorFromPosition(mousePos.x, mousePos.y);
					SetShapesFillColor<sf::RectangleShape>(&rectangle, foregroundColor);
					SetShapesOutlineColor<sf::RectangleShape>(&rectangle, backgroundColor);
					SetShapesOutlineColor<sf::RectangleShape>(&rectangleNoFill, foregroundColor);
					SetShapesFillColor<sf::CircleShape>(&circle, foregroundColor);
					SetShapesOutlineColor<sf::CircleShape>(&circle, backgroundColor);
					SetLinesColor(line, foregroundColor, backgroundColor);
					menu.UpdatePickedColors(foregroundColor, backgroundColor);
					break;				
				}
				drawing = true;
			}

			// While moving mouse and started drwaing - change size of shape
			if (drawing && event.type == sf::Event::MouseMoved)
			{
				switch (chosenOption)
				{
				default:
					break;
				case Options::DrawLine:
					DrawShape(line+1, mousePos);
					break;
				case Options::DrawRectangle:
					DrawShape(&rectangle, mousePos);
					break;
				case Options::DrawRectangleNoFill:
					DrawShape(&rectangleNoFill, mousePos);
					break;
				case Options::DrawCircle:
					DrawShape(&circle, mousePos);
					break;
				}
			}

			// If drawing and mouse button is realesed - finish drawing and add shape to array to draw it later
			if (drawing && event.type == sf::Event::MouseButtonReleased)
			{			
				IncreaseShapeArraySize<DrawableShape>(&shapesOrder, ++shapesDrawnCount);
				switch (chosenOption)
				{
				default:
					break;
				case Options::DrawLine:
					FinishDrawingShape<sf::Vertex>(line[0], &lines, &linesCount);
					FinishDrawingShape<sf::Vertex>(line[1], &lines, &linesCount);
					shapesOrder[shapesDrawnCount - 1] = DrawableShape::LineShape;
					break;
				case Options::DrawRectangle:
					FinishDrawingShape<sf::RectangleShape>(rectangle, &rectangles, &rectanglesCount);
					shapesOrder[shapesDrawnCount - 1] = DrawableShape::RectangleShape;
					break;
				case Options::DrawRectangleNoFill:
					FinishDrawingShape<sf::RectangleShape>(rectangleNoFill, &rectanglesNoFill, &rectanglesNoFillCount);
					shapesOrder[shapesDrawnCount - 1] = DrawableShape::RectangleNoFillShape;
					break;
				case Options::DrawCircle:
					FinishDrawingShape<sf::CircleShape>(circle, &circles, &circlesCount);
					shapesOrder[shapesDrawnCount - 1] = DrawableShape::CircleShape;
					break;
				}				
				drawing = false;
			}
		}

		// Draw loaded img
		window.draw(loadedImgSprite);

		// Draw all shapes in the order they were created
		int recs = 0;
		int recsNF = 0;
		int circs = 0;
		int lins = 0;
		for (int i = 0; i < shapesDrawnCount; i++)
		{
			switch (shapesOrder[i])
			{
			default:
				break;
			case DrawableShape::LineShape:
				window.draw(lines + lins, 2, sf::Lines);
				lins += 2;
				break;
			case DrawableShape::RectangleShape:
				window.draw(rectangles[recs]);
				recs++;
				break;
			case DrawableShape::RectangleNoFillShape:
				window.draw(rectanglesNoFill[recsNF]);
				recsNF++;
				break;
			case DrawableShape::CircleShape:
				window.draw(circles[circs]);
				circs++;
				break;
			}
		}

		// If drawing, draw shape on top of older shapes
		if (drawing)
		{
			switch (chosenOption)
			{
			default:
				break;
			case Options::DrawLine:
				window.draw(line, 2, sf::Lines);
				break;
			case Options::DrawRectangle:
				window.draw(rectangle);
				break;
			case Options::DrawRectangleNoFill:
				window.draw(rectangleNoFill);
				break;
			case Options::DrawCircle:
				window.draw(circle);
				break;
			}
		}

		// If specified key was pressed, save current img to file
		if (saveToFile)
		{
			saveToFile = false;
			sf::Texture currentImage;
			currentImage.create(window.getSize().x, window.getSize().y);
			currentImage.update(window);
			sf::Image img = currentImage.copyToImage();
			img.saveToFile(savedImgName);
		}
		
		window.draw(menu);
		window.display();
	}

	return 0;
}