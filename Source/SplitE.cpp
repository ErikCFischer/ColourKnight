#include <SharedSpace.hxx>

#include <math.h>

SplitE::SplitE() { // but actually why did I even make these?

	this->body = sf::VertexArray( sf::TriangleFan, 8 );

	this->color = sf::Color::White;

	this->body[0].position = sf::Vector2f( 10, 10 );
	this->body[1].position = sf::Vector2f( 10,  5 );
	this->body[2].position = sf::Vector2f( 20, 15 );
	this->body[3].position = sf::Vector2f( 15, 20 );
	this->body[4].position = sf::Vector2f( 10, 15 );
	this->body[5].position = sf::Vector2f(  5, 20 );
	this->body[6].position = sf::Vector2f(  0, 15 );
	this->body[7].position = sf::Vector2f( 10,  5 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;
	this->body[7].color = this->color;

}

SplitE::SplitE( // I literally only ever use these ones...
	sf::Color set_color,
	float* set_d_time
) {

	this->body = sf::VertexArray( sf::TriangleFan, 8 );
	this->setOrigin( 10, 10 );

	this->color = set_color;
	this->d_time = set_d_time;

	this->body[0].position = sf::Vector2f( 10, 10 );
	this->body[1].position = sf::Vector2f( 10,  5 );
	this->body[2].position = sf::Vector2f( 20, 15 );
	this->body[3].position = sf::Vector2f( 15, 20 );
	this->body[4].position = sf::Vector2f( 10, 15 );
	this->body[5].position = sf::Vector2f(  5, 20 );
	this->body[6].position = sf::Vector2f(  0, 15 );
	this->body[7].position = sf::Vector2f( 10,  5 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;
	this->body[7].color = this->color;

}

void SplitE::setColor(
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
	this->body[7].color = this->color;
}

sf::Color SplitE::getColor() {
	return this->color;
}

//Guess you CAN choose your family...
void SplitE::setSibling(
	SplitE* set_sibling
) {
	this->has_sibling = true;
	this->sibling = set_sibling;
}
SplitE* SplitE::getSibling() {
	return this->sibling;
}
bool SplitE::hasSibling() {
	return this->has_sibling;
}
void SplitE::killSibling() {   // :(
	this->has_sibling = false;
	this->sibling = nullptr;
}

char SplitE::update(
	SharedSpace shared_space
) {

	sf::Vector2f player_position = shared_space.player->getPosition();

	if(this->has_sibling) {

		sf::Vector2f sibling_position = this->getPosition() - sibling->getPosition(); // vector from sibling to this
		float sib_dist =
			sqrt( sibling_position.x * sibling_position.x
				+ sibling_position.y * sibling_position.y );

		sibling_position /= sib_dist; // unit vector

		sf::Vector2f mp_position = player_position - this->getPosition(); // vector from this to player
		float mp_dist =
			sqrt( mp_position.x * mp_position.x
				+ mp_position.y * mp_position.y );

		sf::Vector2f sp_position = player_position - sibling->getPosition(); // vector from sibling to player
		float sp_dist =
			sqrt( sp_position.x * sp_position.x
				+ sp_position.y * sp_position.y );

		sibling_position *= (mp_dist + sp_dist) - sib_dist; // give it size
		player_position += sibling_position; // add it to where the player is

	}

	player_position -= this->getPosition(); // vector towards goal

	float dist =
		sqrt( player_position.x * player_position.x
			+ player_position.y * player_position.y );

	player_position *= (dist) ? (1/dist) : dist; // unit vector
	player_position *= this->speed;
	player_position *= *(this->d_time);

	//LOOK AT ME!
	float rot = atan2( player_position.y, player_position.x ) * 180 / 3.14159 + 90;
	this->setRotation(rot);

	//Hitbox stuff
	sf::FloatRect copy_of_this = getBoundingBox();
	copy_of_this.left += player_position.x;
	copy_of_this.top += player_position.y;

	//Map boundaries
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

	//if about to hit an enemy, back up a bit
	for(auto x : *(shared_space.normal_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.shooter_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.split_enemies)) {
		if(x == this) continue;
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.splitter_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}

	this->move( player_position ); // actually move

	return 1;

}

//Making your own entities is weird, I have to define my own bounding boxes
sf::FloatRect SplitE::getBoundingBox() {
	sf::FloatRect bounder = this->body.getBounds();
	bounder.left += this->getPosition().x;
	bounder.left -= bounder.width/2; // Origin is in the middle of the thing
	bounder.top += this->getPosition().y;
	bounder.top -= bounder.height/2;
	return bounder;
}

float SplitE::getLeft() {
	return getBoundingBox().left;
};
float SplitE::getRight() {
	return getBoundingBox().left + getBoundingBox().width;
};
float SplitE::getTop() {
	return getBoundingBox().top;
};
float SplitE::getBottom() {
	return getBoundingBox().top + getBoundingBox().height;
};
