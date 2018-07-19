#include <logic.hpp>

#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SharedSpace.hxx>

using std::cout;
using std::endl;
using std::vector;

//Readjust the Shared Space if the local vector changes
void realloc(std::vector<NormalE*>*, std::vector<NormalE>*);
void realloc(std::vector<ShooterE*>*, std::vector<ShooterE>*);
void realloc(std::vector<Shot*>*, std::vector<Shot>*);
void realloc(std::vector<SplitterE*>*, std::vector<SplitterE>*);
void realloc(std::vector<SplitE*>*, std::vector<SplitE>*);

void logicThread(
	SharedSpace* shared_space
) {

	sf::View* view = shared_space->view;
	MapDemo* drawmap = shared_space->drawmap;
	//Basically creating local aliases for the shared space
	char* key_state = shared_space->key_state;
	/* * * * 0000 0001 = D
	 * * * * 0000 0010 = S
	 * * * * 0000 0100 = A
	 * * * * 0000 1000 = W
	 * * * * 0001 0000 = Right Click
	 * * * * 0010 0000 = Left Click
	 * * * * 1000 0000 = Game Close
	 * * * */
	char* player_state = shared_space->player_state;
 	/* * * * 0000 0001 = dash menu
 	 * * * * 0000 1000 = dash up
 	 * * * * 0000 1010 = dash right
 	 * * * * 0000 1100 = dash left
 	 * * * * 0000 1110 = dash down
 	 * * * * 0001 0000 = paused
 	 * * * * 1000 0000 = dead
 	 * * * */
	sf::Vector2f* mouse_position = shared_space->mouse_position;
	Player* player = shared_space->player;
	sf::RectangleShape* dash_trail = shared_space->dash_trail;
	sf::Sprite* dash_wheel_sprite = shared_space->dash_wheel_sprite;
	std::vector<NormalE*>* normal_enemies = shared_space->normal_enemies;
	std::vector<ShooterE*>* shooter_enemies = shared_space->shooter_enemies;
	std::vector<Shot*>* shot = shared_space->shot;
	std::vector<SplitterE*>* splitter_enemies = shared_space->splitter_enemies;
	std::vector<SplitE*>* split_enemies = shared_space->split_enemies;

	//These are for syncing speeds across different computers
	float movespeed;
	float dashspeed;
	float deltatime;

	*player_state |= 0b00010000; //Start paused
	bool pointmade = false; //This is so you can only die once, then you're immortal

	//Where the player is looking
	sf::Vector2f player_aim;
	float atan_player_aim = 0;
	float mag_player_aim = 0;

	//Where the mouse is in relation to the dash menu
	sf::Vector2f menu_to_mouse;
	float atan_menu_to_mouse = 0;
	float mag_menu_to_mouse = 0;

	//Dash menu position
	sf::Vector2f menu_position;

	//Clock
	sf::Clock clock;

	//Seed random with time (random is for colours when making baddies)
	srand (time(NULL));
	//The colours
	sf::Color CMAG(255, 063, 127);
	sf::Color CBLU(000, 000, 255);
	sf::Color CGRN(000, 255, 000);
	sf::Color CORG(255, 127, 063);

	//Local vectors to actually store the enemy objects as they're made
	vector<NormalE> vnormal;
	vector<ShooterE> vshooter;
	vector<Shot> vshot;
	vector<SplitterE> vsplitter;
	vector<SplitE> vsplit;

	//forever
	for(;;) {

		//This makes sure that speeds will be computer speed independant
		deltatime = clock.restart().asMicroseconds();
		movespeed = 0.0005f * deltatime;
		dashspeed = 0.001f * deltatime;

		if( *key_state & 0b01000000 && !(*player_state & 0b10000000) ) { // Space is Pressed and not dead

			if(*player_state & 0b00010000) { 	// If paused
				*player_state &= 0b11101111; 	// Unpause
			} else {							// If not paused
				*player_state |= 0b00010000;	// Pause
			}
			*key_state &= 0b10111111;			// Unset spacebar so it goes once per press

		}

		if(!(*player_state & 0b00010000) && !(*player_state & 0b10000000)) { // Not paused and not dead

			if( !(*player_state & 0b1000) ) { // Not Dashing

				if( *key_state & 0b1000 ) { // W is Pressed

					if(player->getTop() > drawmap->getTop()) { // If you're not colliding with the top
						dash_trail->move( 0, -movespeed );     // Move the trail up
						player->move( 0, -movespeed );         // Oh and the player too
					}

				}
				if( *key_state & 0b0100 ) { // A is Pressed

					if(player->getLeft() > drawmap->getLeft()) { //Okay you get it
						dash_trail->move( -movespeed, 0 );
						player->move( -movespeed, 0 );
					}

				}
				if( *key_state & 0b0010 ) { // S is Pressed

					if(player->getBottom() < drawmap->getBottom()) {
						dash_trail->move( 0, movespeed );
						player->move( 0, movespeed );
					}

				}
				if( *key_state & 0b0001 ) { // D is Pressed

					if(player->getRight() < drawmap->getRight()) {
						dash_trail->move( movespeed, 0 );
						player->move( movespeed, 0 );
					}

				}

				if( *key_state & 0b00100000 ) { // Left Click

					if( !(*player_state & 0b0001) ) { // Dash menu isn't open

						//Open it
						*player_state |= 0b0001;

						//Set the menu position
						menu_position = *mouse_position;
						dash_wheel_sprite->setPosition( menu_position );
					}

				}
				if( *key_state & 0b00010000 ) { // Right Click

					if( *player_state & 0b0001 ) {	 // If dash menu is open
						*player_state &= 0b11111110; // Close it
						*key_state &= 0b11011111;    // Also unset left click
					}

				}

			}

			if( *player_state & 0b0001 ) { // Dash Menu is Open

				//The player is now looking at where the menu was openned
				player_aim = menu_position - player->getPosition();

				mag_player_aim =
					sqrt( player_aim.x * player_aim.x
						+ player_aim.y * player_aim.y );

				//Where the mouse is related to the dash menu
				menu_to_mouse = *mouse_position - menu_position;

				mag_menu_to_mouse =
					sqrt( menu_to_mouse.x * menu_to_mouse.x
						+ menu_to_mouse.y * menu_to_mouse.y );

				atan_menu_to_mouse =
					atan2( menu_to_mouse.y, menu_to_mouse.x ) * 180 / 3.14159;

				atan_menu_to_mouse += 45; // Tilt offset

				if( !(*key_state & 0b00100000) ) { // On Left Click release

					//Turn dash menu off
					*player_state &= 0b11111110;

					if( 10 <= mag_menu_to_mouse ) { // Minimum distance to register

						if( // Left
							(-180 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= -90)
							|| 180 < atan_menu_to_mouse
						) {

							*player_state |= 0b00001100;

						} else if( // Up
							-90 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 0
						) {

							*player_state |= 0b00001000;

						} else if( // Right
							0 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 90
						) {

							*player_state |= 0b00001010;

						} else if( // Down
							90 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 180
						) {

							*player_state |= 0b00001110;

						}

					}

				} else { // While left click is being held down

					if( 10 <= mag_menu_to_mouse ) { // Minimum distance to register

						if( // Left
							(-180 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= -90)
							|| 180 < atan_menu_to_mouse
						) {

							// Magenta Selection
							dash_wheel_sprite->setTextureRect(
								sf::IntRect(72, 0, 72, 72)
							);

						} else if( // Up
							-90 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 0
						) {

							//Blue Selection
							dash_wheel_sprite->setTextureRect(
								sf::IntRect(144, 0, 72, 72)
							);

						} else if( // Right
							0 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 90
						) {

							//Green Selection
							dash_wheel_sprite->setTextureRect(
								sf::IntRect(216, 0, 72, 72)
							);

						} else if( // Down
							90 < atan_menu_to_mouse
							&& atan_menu_to_mouse <= 180
						) {

							//Orange Selection
							dash_wheel_sprite->setTextureRect(
								sf::IntRect(288, 0, 72, 72)
							);

						}
					} else { // While under minimum distance

						//Default unlit wheel
						dash_wheel_sprite->setTextureRect(
							sf::IntRect(0, 0, 72, 72)
						);

					}

				}

			} else if( *player_state & 0b1000 ) { // Dashing

				//Player is looking where the menu was
				player_aim = menu_position - player->getPosition();

				mag_player_aim =
					sqrt( player_aim.x * player_aim.x
						+ player_aim.y * player_aim.y );

				//Colour Switch
				switch( *player_state & 0b1110 ) {
					case 0b1100 : // Left
						dash_trail->setFillColor(CMAG);
						break;
					case 0b1000 : // Up
						dash_trail->setFillColor(CBLU);
						break;
					case 0b1010 : // Right
						dash_trail->setFillColor(CGRN);
						break;
					case 0b1110 : // Down
						dash_trail->setFillColor(CORG);
						break;
				}

				//If the player is at the final destination
				if( player->getPosition() == menu_position ) {

					//Turn off the dash
					*player_state &= 0b11110001;
					continue;

				}

				//If the player is closer to the destination than a single movement
				if( fabs( mag_player_aim ) <= dashspeed ) {

					//Put the player AT the destination and turn off the dash
					player->setPosition( menu_position );
					*player_state &= 0b11110001;
					continue;

				}

				//Move the player towards the destination
				player->move(
					(player_aim.x * dashspeed)/mag_player_aim,
					(player_aim.y * dashspeed)/mag_player_aim
				);

			} else { // While not dashing and the menu isn't up

				//Player is looking at the mouse
				player_aim = *mouse_position - player->getPosition();
				mag_player_aim =
					sqrt( player_aim.x * player_aim.x
						+ player_aim.y * player_aim.y );

				//Dash trail is invisible
				dash_trail->setFillColor( sf::Color::Transparent );

			}

			//Rotate player based on where the player is looking
			atan_player_aim = atan2( player_aim.y, player_aim.x ) * 180 / 3.14159;
			player->setRotation( atan_player_aim + 90 );

			//Resize and move dash trail based on player location
			dash_trail->setSize( sf::Vector2f( 20,  mag_player_aim) );
			dash_trail->setPosition( player->getPosition() );
			dash_trail->setRotation( player->getRotation() - 180);

			//Hitbox stuff
			sf::FloatRect player_bounds = player->getBoundingBox();
			sf::FloatRect dash_bounds = dash_trail->getGlobalBounds();
			sf::FloatRect collision_bounds;

			for(int i = 0; i < vnormal.size(); i++) { // For each normal enemy

				vnormal[i].update(*shared_space); // Think
				collision_bounds = vnormal[i].getBoundingBox();

				if(player_bounds.intersects(collision_bounds)) { // If touching the player

					if(dash_trail->getFillColor() == vnormal[i].getColor()) { // If dashing the same colour

						vnormal.erase(vnormal.begin() + i); // baddie got killed
						realloc(normal_enemies, &vnormal);
						i--;

					} else if( !(*player_state & 0b1000) && !pointmade ) { // But if not dashing and you havn't died yet...
						*player_state |= 0b10000000; // You ded.
					}
				}

			}

			for(int i = 0; i < vshooter.size(); i++) { // For each shooter enemy

				if( vshooter[i].update(*shared_space) == 2) { // Think, and if it's time to shoot projectile
					vshot.push_back( Shot(&deltatime, player->getPosition()) ); // shoot a projectile
					vshot.back().setPosition(vshooter[i].getPosition());
					realloc(shot, &vshot);
				}

				collision_bounds = vshooter[i].getBoundingBox();

				if(player_bounds.intersects(collision_bounds)) { // Hitbox yada yada you get it
					if(dash_trail->getFillColor() == vshooter[i].getColor()) {

						vshooter.erase(vshooter.begin() + i);
						realloc(shooter_enemies, &vshooter);
						i--;

					} else if( !(*player_state & 0b1000) && !pointmade ) {
						*player_state |= 0b10000000;
					}
				}

			}

			for(int i = 0; i < vshot.size(); i++) { // Projectiles

				if( vshot[i].update(*shared_space) == 2 ) { // Think, if time to die...
					vshot.erase(vshot.begin() + i); // Die
					realloc(shot, &vshot);
					i--;
					continue; // No need to do anything else, it's dead.
				}

				collision_bounds = vshot[i].getBoundingBox();

				if(player_bounds.intersects(collision_bounds)) {
					if( !(*player_state & 0b1000) && !pointmade ) {
						*player_state |= 0b10000000;
					}
				}
			}

			for(int i = 0; i < vsplitter.size(); i++) { // Splitter enemies

				vsplitter[i].update(*shared_space);
				collision_bounds = vsplitter[i].getBoundingBox();

				if(player_bounds.intersects(collision_bounds)) {

					if(dash_trail->getFillColor() == vsplitter[i].getColor()) { // Upon death...

						//This part part assigns a random colour to both splits
						//But makes sure neither are same collour as the Splitter that spawned them
						//Nor are they the same as eachother
						vector<sf::Color> wheel = {CMAG, CBLU, CGRN, CORG};
						sf::Color spc1, spc2;
						int spin = rand() % 4;
						if(wheel[spin] == vsplitter[i].getColor()) {
							wheel.erase(wheel.begin() + spin);
							spin = rand() % 3;
							spc1 = wheel[spin];
							wheel.erase(wheel.begin() + spin);
							spin = rand() % 2;
							spc2 = wheel[spin];
						} else {
							spc1 = wheel[spin];
							wheel.erase(wheel.begin() + spin);
							spin = rand() % 3;
							if(wheel[spin] == vsplitter[i].getColor()) {
								wheel.erase(wheel.begin() + spin);
								spin = rand() % 2;
								spc2 = wheel[spin];
							} else {
								spc2 = wheel[spin];
							}
						}

						//Places the splits between 5 and 15 units away from centre
						sf::Vector2f spl(rand() % 11 + 5, rand() % 11 + 5);

						vsplit.push_back( SplitE(spc1, &deltatime) );
						vsplit.back().setPosition(vsplitter[i].getPosition() + spl); // Offset

						vsplit.push_back( SplitE(spc2, &deltatime) );
						vsplit.back().setPosition(vsplitter[i].getPosition() - spl); // Negative offset, other side of splitter

						//Make them siblings
						vsplit[vsplit.size()-1].setSibling(&vsplit[vsplit.size()-2]);
						vsplit[vsplit.size()-2].setSibling(&vsplit[vsplit.size()-1]);

						realloc(split_enemies, &vsplit);

						vsplitter.erase(vsplitter.begin() + i); // Oh yeah also actually kill the splitter
						realloc(splitter_enemies, &vsplitter);
						i--;

					} else if( !(*player_state & 0b1000) && !pointmade ) {
						*player_state |= 0b10000000;
					}
				}
			}

			for(int i = 0; i < vsplit.size(); i++) { // Split enemies

				vsplit[i].update(*shared_space);
				collision_bounds = vsplit[i].getBoundingBox();

				if(player_bounds.intersects(collision_bounds)) {

					if(dash_trail->getFillColor() == vsplit[i].getColor()) {

						if(vsplit[i].hasSibling()) {
							vsplit[i].getSibling()->killSibling();
						}
						vsplit.erase(vsplit.begin() + i);
						realloc(split_enemies, &vsplit);
						i--;

					} else if( !(*player_state & 0b1000) && !pointmade) {
						*player_state |= 0b10000000;
					}
				}
			}

		} else if(*player_state & 0b10000000 && !pointmade) { // Dead for the first time

			if( *key_state & 0b01000000 ) {  // If you press spacebar
				*player_state &= 0b01111111; // unkill yourself
				*key_state &= 0b10111111;    // unset spacebar
				pointmade = true;            // Immortality
			}


		} else { // If paused


			if( *key_state & 0b1000 ) { // W is Pressed

				view->zoom(0.9f); // Zoom in
				*key_state &= 0b11110111;

			}
			if( *key_state & 0b0010 ) { // S is Pressed

				view->zoom(1.1f); // Zoom out
				*key_state &= 0b11111101;

			}


			if( *key_state & 0b0100 ) { // A is Pressed

				//Random coloured normal enemy at cursor
				vector<sf::Color> wheel = {CMAG, CBLU, CGRN, CORG};
				int spin = rand() % 4;

				vnormal.push_back( NormalE(wheel[spin], &deltatime) );
				vnormal.back().setPosition(*mouse_position);
				realloc(normal_enemies, &vnormal);
				*key_state &= 0b11111011;

			}
			if( *key_state & 0b0001 ) { // D is Pressed

				//Random coloured shooter enemy at cursor
				vector<sf::Color> wheel = {CMAG, CBLU, CGRN, CORG};
				int spin = rand() % 4;

				vshooter.push_back( ShooterE(wheel[spin], &deltatime) );
				vshooter.back().setPosition(*mouse_position);
				realloc(shooter_enemies, &vshooter);
				*key_state &= 0b11111110;

			}
			if( *key_state & 0b00010000 ) { // Right Click

				//Random coloured splitter enemy at cursor
				vector<sf::Color> wheel = {CMAG, CBLU, CGRN, CORG};
				int spin = rand() % 4;

				vsplitter.push_back( SplitterE(wheel[spin], &deltatime) );
				vsplitter.back().setPosition(*mouse_position);
				realloc(splitter_enemies, &vsplitter);

				*key_state &= 0b11101111;

			}

		}

		//This section just adjusts the view to center on the player
		//Unless you're too close to the edge of the map, it won't show the parts that aren't there
		float viewx = player->getPosition().x;
		if( viewx < (drawmap->getLeft() + (view->getSize().x / 2)) ) {
			viewx = drawmap->getLeft() + (view->getSize().x / 2);
		} else if( viewx > (drawmap->getRight() - (view->getSize().x / 2)) ) {
			viewx = drawmap->getRight() - (view->getSize().x / 2);
		}

		float viewy = player->getPosition().y;
		if( viewy < (drawmap->getTop() + (view->getSize().y / 2)) ) {
			viewy = drawmap->getTop() + (view->getSize().y / 2);
		} else if( viewy > (drawmap->getBottom() - (view->getSize().y / 2)) ) {
			viewy = drawmap->getBottom() - (view->getSize().y / 2);
		}

		view->setCenter( viewx, viewy );


		if( *key_state & 0b10000000 ) // Exited
			break;

	}

}


//I'm just gonna explain one, they're all the same.
//It takes the shared vector and local vector
void realloc(std::vector<NormalE*>* pve, std::vector<NormalE>* ve) {
	pve->clear();			 // Wipe the shared vector
	pve->resize(ve->size()); // set it up for local vector
	for(int i = 0; i < pve->size(); i++) {
		(*pve)[i] = &((*ve)[i]); // Fill it with the local addresses
	}
}
void realloc(std::vector<ShooterE*>* pve, std::vector<ShooterE>* ve) {
	pve->clear();
	pve->resize(ve->size());
	for(int i = 0; i < pve->size(); i++) {
		(*pve)[i] = &((*ve)[i]);
	}
}
void realloc(std::vector<Shot*>* pve, std::vector<Shot>* ve) {
	pve->clear();
	pve->resize(ve->size());
	for(int i = 0; i < pve->size(); i++) {
		(*pve)[i] = &((*ve)[i]);
	}
}
void realloc(std::vector<SplitterE*>* pve, std::vector<SplitterE>* ve) {
	pve->clear();
	pve->resize(ve->size());
	for(int i = 0; i < pve->size(); i++) {
		(*pve)[i] = &((*ve)[i]);
	}
}
void realloc(std::vector<SplitE*>* pve, std::vector<SplitE>* ve) {
	pve->clear();
	pve->resize(ve->size());
	for(int i = 0; i < pve->size(); i++) {
		(*pve)[i] = &((*ve)[i]);
	}
}
