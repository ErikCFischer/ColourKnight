#include <SharedSpace.hxx>

#include <math.h>

Shot::Shot() { // never used

	this->body = sf::VertexArray( sf::Quads, 4 );
	this->setOrigin( 2, 6 );

	this->color = sf::Color::Black;

	this->body[0].position = sf::Vector2f( 0,  0 );
	this->body[1].position = sf::Vector2f( 4,  0 );
	this->body[2].position = sf::Vector2f( 4, 12 );
	this->body[3].position = sf::Vector2f( 0, 12 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;

}

Shot::Shot( // actually used
	float* set_d_time,
	sf::Vector2f set_target
) {

	this->body = sf::VertexArray( sf::Quads, 4 );
	this->setOrigin( 2, 6 );

	this->d_time = set_d_time;
	target = set_target; // Where the player was, bullets can't turn
	this->color = sf::Color::Black;

	this->body[0].position = sf::Vector2f( 0,  0 );
	this->body[1].position = sf::Vector2f( 4,  0 );
	this->body[2].position = sf::Vector2f( 4, 12 );
	this->body[3].position = sf::Vector2f( 0, 12 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;

}

char Shot::update(
	SharedSpace shared_space
) {

	sf::Vector2f player_position = target;
	player_position -= this->getPosition(); // Vector from this to target
	float dist =
		sqrt( player_position.x * player_position.x
			+ player_position.y * player_position.y );
	player_position *= (dist) ? (1/dist) : dist; // unit
	player_position *= this->speed;
	player_position *= *(this->d_time);

	target += player_position; // aim to infinity and beyond

	//Face the target
	float rot = atan2( player_position.y, player_position.x ) * 180 / 3.14159 + 90;
	this->setRotation(rot);

	this->move( player_position ); // move
	lifetime--; // die a little

	if(!lifetime) { // if dead
		return 2;   // let someone know
	}

	return 1;

}

//Making your own entities is weird, I have to define my own bounding boxes
sf::FloatRect Shot::getBoundingBox() {
	sf::FloatRect bounder = this->body.getBounds();
	bounder.left += this->getPosition().x;
	bounder.left -= bounder.width/2; // Origin is in the middle of the thing
	bounder.top += this->getPosition().y;
	bounder.top -= bounder.height/2;
	return bounder;
}

float Shot::getLeft() {
	return getBoundingBox().left;
};
float Shot::getRight() {
	return getBoundingBox().left + getBoundingBox().width;
};
float Shot::getTop() {
	return getBoundingBox().top;
};
float Shot::getBottom() {
	return getBoundingBox().top + getBoundingBox().height;
};
