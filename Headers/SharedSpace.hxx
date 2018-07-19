#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include <Player.hpp>
#include <NormalE.hpp>
#include <ShooterE.hpp>
#include <Shot.hpp>
#include <SplitterE.hpp>
#include <SplitE.hpp>
#include <MapDemo.hpp>

struct SharedSpace {

	sf::RenderWindow* window;
	sf::View* view;
	MapDemo* drawmap;
	char* key_state;
	char* player_state;
	sf::Vector2f* mouse_position;
	Player* player;
	sf::RectangleShape* dash_trail;
	sf::Sprite* dash_wheel_sprite;
	std::vector<NormalE*>* normal_enemies = new std::vector<NormalE*>();
	std::vector<ShooterE*>* shooter_enemies = new std::vector<ShooterE*>();
	std::vector<Shot*>* shot = new std::vector<Shot*>();
	std::vector<SplitterE*>* splitter_enemies = new std::vector<SplitterE*>();
	std::vector<SplitE*>* split_enemies = new std::vector<SplitE*>();

};
