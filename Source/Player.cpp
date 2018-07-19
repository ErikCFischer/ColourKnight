#include <Player.hpp>

Player::Player() { // Make the weird pentagon thing

	this->body = sf::VertexArray( sf::TriangleFan, 7 );
	this->setOrigin( 10, 10 );

	this->color = sf::Color::Magenta;

	this->body[0].position = sf::Vector2f( 10, 10 );
	this->body[1].position = sf::Vector2f( 10,  0 );
	this->body[2].position = sf::Vector2f( 20, 10 );
	this->body[3].position = sf::Vector2f( 20, 20 );
	this->body[4].position = sf::Vector2f(  0, 20 );
	this->body[5].position = sf::Vector2f(  0, 10 );
	this->body[6].position = sf::Vector2f( 10,  0 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;

}

sf::Color Player::getColor() { // Technically never used
	return this->color;
}

void Player::setColor( // Once again, never used.
	sf::Color set_color
) {
	this->color = set_color;

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;
}

//Making your own entities is weird, I have to define my own bounding boxes
sf::FloatRect Player::getBoundingBox() {
	sf::FloatRect bounder = this->body.getBounds();
	bounder.left += this->getPosition().x;
	bounder.left -= bounder.width/2;  // Origin is in the middle of the thing
	bounder.top += this->getPosition().y;
	bounder.top -= bounder.height/2;
	return bounder;
}

float Player::getLeft() {
	return getBoundingBox().left;
};
float Player::getRight() {
	return getBoundingBox().left + getBoundingBox().width;
};
float Player::getTop() {
	return getBoundingBox().top;
};
float Player::getBottom() {
	return getBoundingBox().top + getBoundingBox().height;
};
