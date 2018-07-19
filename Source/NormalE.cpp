#include <SharedSpace.hxx>

#include <math.h>

NormalE::NormalE() { // never really used tbh...

	this->body = sf::VertexArray( sf::Triangles, 3 );

	this->color = sf::Color::White;

	this->body[0].position = sf::Vector2f( 10, 0 );
	this->body[1].position = sf::Vector2f( 0, 20 );
	this->body[2].position = sf::Vector2f( 20, 20 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;

}

NormalE::NormalE( // the actually used constructor
	sf::Color set_color,
	float* set_d_time
) {

	this->body = sf::VertexArray( sf::Triangles, 3 );
	this->setOrigin( 10, 10 );

	this->color = set_color;
	this->d_time = set_d_time;

	this->body[0].position = sf::Vector2f( 10, 0 );
	this->body[1].position = sf::Vector2f( 0, 20 );
	this->body[2].position = sf::Vector2f( 20, 20 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;

}

//A.I. time oh boy
char NormalE::update(
	SharedSpace shared_space
) {

	sf::Vector2f player_position = shared_space.player->getPosition();
	player_position -= this->getPosition(); // Vector from this to player
	float dist =
		sqrt( player_position.x * player_position.x
			+ player_position.y * player_position.y );
	player_position *= (dist) ? (1/dist) : dist; // unit vector
	player_position *= this->speed;
	player_position *= *(this->d_time);

	//Turn to face player
	float rot = atan2( player_position.y, player_position.x ) * 180 / 3.14159 + 90;
	this->setRotation(rot);

	//hitbox stuff
	sf::FloatRect copy_of_this = getBoundingBox();
	copy_of_this.left += player_position.x;
	copy_of_this.top += player_position.y;

	//Map bounds
	if(copy_of_this.top < shared_space.drawmap->getTop()) {
		player_position.y = 0;
	}
	if(copy_of_this.left < shared_space.drawmap->getLeft()) {
		player_position.x = 0;
	}
	if(copy_of_this.left + copy_of_this.width > shared_space.drawmap->getRight()) {
		player_position.x = 0;
	}
	if(copy_of_this.top + copy_of_this.height > shared_space.drawmap->getBottom()) {
		player_position.y = 0;
	}

	//If you run into another enemy, back up a bit
	for(auto x : *(shared_space.normal_enemies)) {
		if(x == this) continue;
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.shooter_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.split_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.splitter_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}

	this->move( player_position ); // Actually move

	return 1;

}

sf::Color NormalE::getColor() {
	return this->color;
}

void NormalE::setColor(
	sf::Color set_color
) {
	this->color = set_color;

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
}

//Making your own entities is weird, I have to define my own bounding boxes
sf::FloatRect NormalE::getBoundingBox() {
	sf::FloatRect bounder = this->body.getBounds();
	bounder.left += this->getPosition().x;
	bounder.left -= bounder.width/2;  // Origin is in the middle of the thing
	bounder.top += this->getPosition().y;
	bounder.top -= bounder.height/2;
	return bounder;
}

float NormalE::getLeft() {
	return getBoundingBox().left;
};
float NormalE::getRight() {
	return getBoundingBox().left + getBoundingBox().width;
};
float NormalE::getTop() {
	return getBoundingBox().top;
};
float NormalE::getBottom() {
	return getBoundingBox().top + getBoundingBox().height;
};
