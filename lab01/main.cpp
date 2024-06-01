//Uwaga! Co najmniej C++17!!!
//Project-> ... Properties->Configuration Properties->General->C++ Language Standard = ISO C++ 17 Standard (/std:c++17)

#include "SFML/Graphics.hpp"
#include <fstream>
#include <iostream>

enum class Field { VOID, FLOOR, WALL, BOX, PARK, PLAYER };

class Sokoban : public sf::Drawable
{
public:
	void LoadMapFromFile(std::string fileName);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void SetDrawParameters(sf::Vector2u draw_area_size);
	void Move_Player_Left();
	void Move_Player_Right();
	void Move_Player_Up();
	void Move_Player_Down();
	bool Is_Victory() const;

private:
	std::vector<std::vector<Field>> map;
	sf::Vector2f shift, tile_size;
	sf::Vector2i player_position;
	std::vector<sf::Vector2i> park_positions;

	void move_player(int dx, int dy);
};

void Sokoban::LoadMapFromFile(std::string fileName)
{
	std::string str;
	std::vector<std::string> vos;

	std::ifstream in(fileName.c_str());
	while (std::getline(in, str)) { vos.push_back(str); }
	in.close();

	map.clear();
	map.resize(vos.size(), std::vector<Field>(vos[0].size()));
	for (auto [row, row_end, y] = std::tuple{ vos.cbegin(), vos.cend(), 0 }; row != row_end; ++row, ++y)
		for (auto [element, end, x] = std::tuple{ row->begin(), row->end(), 0 }; element != end; ++element, ++x)
			switch (*element)
			{
			case 'X': map[y][x] = Field::WALL; break;
			case '*': map[y][x] = Field::VOID; break;
			case ' ': map[y][x] = Field::FLOOR; break;
			case 'B': map[y][x] = Field::BOX; break;
			case 'P': map[y][x] = Field::PARK; park_positions.push_back(sf::Vector2i(x, y));  break;
			case 'S': map[y][x] = Field::PLAYER; player_position = sf::Vector2i(x, y);  break;
			}
}
//
//void sokoban::draw(sf::rendertarget& target, sf::renderstates states) const
//{
//	// tu niew¹tpliwie powinno coœ byæ : -) tu nale¿y narysowaæ wszystko. o tak jakoœ :
//	//target.draw(....);
//
//	//przydatna mo¿e byæ pêtla :
//	for (int y = 0; y < map.size(); ++y)
//		for (int x = 0; x < map[y].size(); ++x)
//		{
//			//teraz map[y][x] mówi nam co mamy narysowaæ.
//		}
//}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	sf::RectangleShape tile(tile_size);

	for (int y = 0; y < map.size(); ++y)
		for (int x = 0; x < map[y].size(); ++x)
		{
			tile.setPosition(sf::Vector2f(shift.x + x * tile_size.x, shift.y + y * tile_size.y));

			switch (map[y][x]) {
				case Field::WALL: tile.setFillColor(sf::Color(90, 20, 20)); break;
				case Field::VOID: tile.setFillColor(sf::Color(0,0,0)); break;
				case Field::FLOOR: tile.setFillColor(sf::Color(200, 125, 20)); break;
				case Field::BOX: tile.setFillColor(sf::Color(100, 200, 200)); break;
				case Field::PARK: tile.setFillColor(sf::Color(0,255,50)); break;
				case Field::PLAYER: tile.setFillColor(sf::Color(255,0,255)); break;
			}

			target.draw(tile);
		}
}

//background.setTextureRect(sf::IntRect(0, 0, 212, 213));
//floor.setTextureRect(sf::IntRect(0, 0, 300, 300));
//wall.setTextureRect(sf::IntRect(0, 0, 208, 202));
//box.setTextureRect(sf::IntRect(0, 0, 216, 211));
//place.setTextureRect(sf::IntRect(0, 0, 217, 207));
//player.setTextureRect(sf::IntRect(0, 0, 212, 212));

//tile.setScale(tile_size.x / background_texture.getSize().x, tile_size.y / background_texture.getSize().y);


void Sokoban::SetDrawParameters(sf::Vector2u draw_area_size)
{
	this->tile_size = sf::Vector2f(
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size())),
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size()))
	);
	this->shift = sf::Vector2f(
		((float)draw_area_size.x - this->tile_size.x * map[0].size()) / 2.0f,
		((float)draw_area_size.y - this->tile_size.y * map.size()) / 2.0f
	);
}

void Sokoban::Move_Player_Left()
{
	move_player(-1, 0);
}

void Sokoban::Move_Player_Right()
{
	move_player(1, 0);
}

void Sokoban::Move_Player_Up()
{
	move_player(0, -1);
}

void Sokoban::Move_Player_Down()
{
	move_player(0, 1);
}

void Sokoban::move_player(int dx, int dy)
{
	bool allow_move = false; // Pesymistyczne za³ó¿my, ¿e gracz nie mo¿e siê poruszyæ.
	sf::Vector2i new_pp(player_position.x + dx, player_position.y + dy); //Potencjalna nowa pozycja gracza.
	Field fts = map[new_pp.y][new_pp.x]; //Element na miejscu na które gracz zamierza przejœæ.
	Field ftsa = map[new_pp.y + dy][new_pp.x + dx]; //Element na miejscu ZA miejscem na które gracz zamierza przejœæ. :-D

	//Gracz mo¿e siê poruszyæ jeœli pole na którym ma stan¹æ to pod³oga lub miejsce na skrzynki.
	if (fts == Field::FLOOR || fts == Field::PARK) allow_move = true;
	//Jeœli pole na które chce siê poruszyæ gracz zawiera skrzynkê to mo¿e siê on poruszyæ jedynie jeœli kolejne pole jest puste lub zawiera miejsce na skrzynkê  - bo wtedy mo¿e przepchn¹æ skrzynkê.
	if (fts == Field::BOX && (ftsa == Field::FLOOR || ftsa == Field::PARK))
	{
		allow_move = true;
		//Przepychamy skrzynkê.
		map[new_pp.y + dy][new_pp.x + dx] = Field::BOX;
		//Oczywiœcie pole na którym sta³a skrzynka staje siê teraz pod³og¹.
		map[new_pp.y][new_pp.x] = Field::FLOOR;
	}

	if (allow_move)
	{
		//Przesuwamy gracza.
		map[player_position.y][player_position.x] = Field::FLOOR;
		player_position = new_pp;
		map[player_position.y][player_position.x] = Field::PLAYER;
	}

	//Niestety w czasie ruchu mog³y „ucierpieæ” miejsca na skrzynkê. ;-(
	for (auto park_position : park_positions) if (map[park_position.y][park_position.x] == Field::FLOOR) map[park_position.y][park_position.x] = Field::PARK;
}

bool Sokoban::Is_Victory() const
{
	//Tym razem dla odmiany optymistycznie zak³adamy, ¿e gracz wygra³.
	//No ale jeœli na którymkolwiek miejscu na skrzynki nie ma skrzynki to chyba za³o¿enie by³o zbyt optymistyczne... : -/
	for (auto park_position : park_positions) if (map[park_position.y][park_position.x] != Field::BOX) return false;
	return true;
}





int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	Sokoban sokoban;

	sokoban.LoadMapFromFile("plansza.txt");
	sokoban.SetDrawParameters(window.getSize());

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Resized)
			{
				float width = static_cast<float>(event.size.width);
				float height = static_cast<float>(event.size.height);
				window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
				sokoban.SetDrawParameters(window.getSize());
			}

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					sokoban.Move_Player_Left();
					break;
				case sf::Keyboard::Right:
					sokoban.Move_Player_Right();
					break;
				case sf::Keyboard::Up:
					sokoban.Move_Player_Up();
					break;
				case sf::Keyboard::Down:
					sokoban.Move_Player_Down();
					break;
				default:
					break;
				}

				if (sokoban.Is_Victory())
				{
					std::cout << "Gratulacje! Wygra³eœ!" << std::endl;
					window.close(); // Po wygranej zamykamy okno
				}
			}
		}

		window.draw(sokoban);
		window.display();
	}

	return 0;
}