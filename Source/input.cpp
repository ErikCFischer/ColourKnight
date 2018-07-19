#include <thread>
#include <vector>
#include <iostream>

#include <math.h>

#include <SFML/Graphics.hpp>

#include <SharedSpace.hxx>

#include <logic.hpp>
#include <render.hpp>

/*
 *
 *
 *
 */

//Height and width of the window
#define WIDTH 800
#define HEIGHT 600

//Literally just for debug, the final product wont have a console
using std::cout;
using std::endl;
using std::vector;

//Main Function, Execution starts here
int main() {

	//Create a shared memory space between the threads
	SharedSpace shared_space;

	//This is just for 8x Anti-Aliasing
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Create the window
	sf::RenderWindow window;
	window.create(
		sf::VideoMode( WIDTH, HEIGHT ),	//WIDTH x HEIGHT
		"Colour Knight",				//Window Name
		sf::Style::Default,				//Top bar of window
		settings						//Window Context Settings, aka 8xAA
	);

	//60 FPS cause I feel like it
	window.setFramerateLimit( 60 );
	//This has to be done to thread rendering
	window.setActive( false );

	sf::View view = window.getDefaultView();
	window.setView(view);

	//Add the window and view to the shared space
	shared_space.window = &window;
	shared_space.view = &view;

	//Create the player
	Player player;
	player.setPosition( 100, 100 );

	//Create the dash trail
	sf::RectangleShape dash_trail( sf::Vector2f( 20, 100 ) );
	dash_trail.setOrigin( 10, 0 );
	dash_trail.setPosition( 100, 100 );
	dash_trail.setFillColor( sf::Color::Transparent );

	shared_space.player = &player;
	shared_space.dash_trail = &dash_trail;

	//Load the Dash Wheel texture
	sf::Texture dash_wheel_texture;
	if( !dash_wheel_texture.loadFromFile( "Assets/dash_wheel.png" ) ) {

		cout << "could not load dashwheel" << endl;

	}
	dash_wheel_texture.setSmooth( true );

	//Create Dash Wheel sprite
	sf::Sprite dash_wheel_sprite;
	dash_wheel_sprite.setTexture( dash_wheel_texture );
	dash_wheel_sprite.setTextureRect(
		sf::IntRect( 0, 0, 72, 72 )
	);
	dash_wheel_sprite.setOrigin( 36, 36 );

	//Only have to add Sprite to shared space as sprite is bound to texture
	shared_space.dash_wheel_sprite = &dash_wheel_sprite;

	//Key presses and player state
	char key_state = 0b00000000;
	char player_state = 0b00000000;

	shared_space.key_state = &key_state;
	shared_space.player_state = &player_state;

	//Mouse position in the window
	sf::Vector2f mouse_position;
	mouse_position = (sf::Vector2f)sf::Mouse::getPosition( window );
	shared_space.mouse_position = &mouse_position;

	//Initialize map
	MapDemo drawmap;
	shared_space.drawmap = &drawmap;

	//Start the render thread
	std::thread rendT(
		renderThread,
		&shared_space
	);
	rendT.detach();

	//Start the logic thread
	std::thread logT(
		logicThread,
		&shared_space
	);
	logT.detach();

	//This is the main loop, runs until window is closed
	while( window.isOpen() ) {

		//Constantly set mouse position
		mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		//Event loop
		sf::Event event;
		while( window.pollEvent(event) ) {

			//Detect and record keyboard key presses
			if( event.type == sf::Event::KeyPressed ) {

				switch( event.key.code ) {
				case sf::Keyboard::W:
			        key_state |= 0b00001000;
					break;
				case sf::Keyboard::A:
			        key_state |= 0b00000100;
					break;
				case sf::Keyboard::S:
			        key_state |= 0b00000010;
					break;
				case sf::Keyboard::D:
			        key_state |= 0b00000001;
					break;
				case sf::Keyboard::Space:
			        key_state |= 0b01000000;
					break;
			    }

			}

			//Detect and record releasing keyboard keys
			if( event.type == sf::Event::KeyReleased ) {

				switch( event.key.code ) {
				case sf::Keyboard::W:
			        key_state &= 0b11110111;
					break;
				case sf::Keyboard::A:
			        key_state &= 0b11111011;
					break;
				case sf::Keyboard::S:
			        key_state &= 0b11111101;
					break;
				case sf::Keyboard::D:
			        key_state &= 0b11111110;
					break;
				case sf::Keyboard::Space:
			        key_state &= 0b10111111;
					break;
			    }

			}

			//Mouse presses
			if( event.type == sf::Event::MouseButtonPressed ) {

			    switch( event.mouseButton.button ) {
				case sf::Mouse::Left:
					key_state |= 0b00100000;
					break;
			    case sf::Mouse::Right:
					key_state |= 0b00010000;
					break;
			    }

			}

			//Mouse releases
			if( event.type == sf::Event::MouseButtonReleased ) {

				switch( event.mouseButton.button ) {
				case sf::Mouse::Left:
					key_state &= 0b11011111;
					break;
			    case sf::Mouse::Right:
					key_state &= 0b11101111;
					break;
			    }

			}

			//If the window is resized, expand the view to fit the new window size
			if (event.type == sf::Event::Resized) {
		        view.setSize(event.size.width, event.size.height);
		        window.setView(view);
		    }

			//If the red X in top right corner is pressed
			if( event.type == sf::Event::Closed ) {

				key_state |= 0b10000000;
				window.close();

			}

		}

	}

	return 0;

}
