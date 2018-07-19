#include <render.hpp>

#include <SharedSpace.hxx>

//Render Thread
void renderThread(
	SharedSpace* shared_space
) {

	//Basically creating local aliases for the shared space
	sf::RenderWindow* window = shared_space->window;
	sf::View* view = shared_space->view;
	MapDemo* drawmap = shared_space->drawmap;
	char* player_state = shared_space->player_state;
	Player* player = shared_space->player;
	sf::RectangleShape* dash_trail = shared_space->dash_trail;
	sf::Sprite* dash_wheel_sprite = shared_space->dash_wheel_sprite;
	std::vector<NormalE*>* normal_enemies = shared_space->normal_enemies;
	std::vector<ShooterE*>* shooter_enemies = shared_space->shooter_enemies;
	std::vector<Shot*>* shot = shared_space->shot;
	std::vector<SplitterE*>* splitter_enemies = shared_space->splitter_enemies;
	std::vector<SplitE*>* split_enemies = shared_space->split_enemies;

	sf::RectangleShape paused(sf::Vector2f(200*30, 100*30));
	paused.setFillColor(sf::Color(0,0,0,64)); // grey tint

	sf::RectangleShape dead(sf::Vector2f(200*30, 100*30));
	dead.setFillColor(sf::Color(255,0,0,64)); // red tint


	//While the window is open
	while( window->isOpen() ) {

		//Clear the window
		window->clear();

		//Set View
		window->setView(*view);

		//Draw Map
		window->draw( *drawmap );

		//Draw player and trail
		window->draw( *dash_trail );
		window->draw( *player );

		if( *player_state & 0b0001 ) { // Dash Menu

			//Draw it
			window->draw( *dash_wheel_sprite );

		}

		//Draw all the enemies
		for(auto x : *normal_enemies) {
			window->draw( *x );
		}
		for(auto x : *shooter_enemies) {
			window->draw( *x );
		}
		for(auto x : *shot) {
			window->draw( *x );
		}
		for(auto x : *splitter_enemies) {
			window->draw( *x );
		}
		for(auto x : *split_enemies) {
			window->draw( *x );
		}

		if(*player_state & 0b00010000) { // Paused
			window->draw(paused);
		}

		if(*player_state & 0b10000000) { // Dead
			window->draw(dead);
		}


		//Show render on screen
		window->display();

	}

}
