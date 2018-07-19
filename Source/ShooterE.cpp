#include <SharedSpace.hxx>

#include <math.h>

ShooterE::ShooterE() { // never used

	this->body = sf::VertexArray( sf::TriangleFan, 11 );

	this->color = sf::Color::White;

	this->body[0].position = sf::Vector2f( 10, 10 );
	this->body[1].position = sf::Vector2f( 10,  0 );
	this->body[2].position = sf::Vector2f( 15, 10 );
	this->body[3].position = sf::Vector2f( 20, 10 );
	this->body[4].position = sf::Vector2f( 20, 15 );
	this->body[5].position = sf::Vector2f( 15, 20 );
	this->body[6].position = sf::Vector2f(  5, 20 );
	this->body[7].position = sf::Vector2f(  0, 15 );
	this->body[8].position = sf::Vector2f(  0, 10 );
	this->body[9].position = sf::Vector2f(  5, 10 );
	this->body[10].position = sf::Vector2f(10,  0 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;
	this->body[7].color = this->color;
	this->body[8].color = this->color;
	this->body[9].color = this->color;
	this->body[10].color = this->color;

}

ShooterE::ShooterE( // Actually used constructer
	sf::Color set_color,
	float* set_d_time
) {

	this->body = sf::VertexArray( sf::TriangleFan, 11 );
	this->setOrigin( 10, 10 );

	this->color = set_color;
	this->d_time = set_d_time;

	this->body[0].position = sf::Vector2f( 10, 10 );
	this->body[1].position = sf::Vector2f( 10,  0 );
	this->body[2].position = sf::Vector2f( 15, 10 );
	this->body[3].position = sf::Vector2f( 20, 10 );
	this->body[4].position = sf::Vector2f( 20, 15 );
	this->body[5].position = sf::Vector2f( 15, 20 );
	this->body[6].position = sf::Vector2f(  5, 20 );
	this->body[7].position = sf::Vector2f(  0, 15 );
	this->body[8].position = sf::Vector2f(  0, 10 );
	this->body[9].position = sf::Vector2f(  5, 10 );
	this->body[10].position = sf::Vector2f(10,  0 );

	this->body[0].color = this->color;
	this->body[1].color = this->color;
	this->body[2].color = this->color;
	this->body[3].color = this->color;
	this->body[4].color = this->color;
	this->body[5].color = this->color;
	this->body[6].color = this->color;
	this->body[7].color = this->color;
	this->body[8].color = this->color;
	this->body[9].color = this->color;
	this->body[10].color = this->color;

}

char ShooterE::update(
	SharedSpace shared_space
) {

	sf::Vector2f player_position = shared_space.player->getPosition();
	player_position -= this->getPosition(); // Vector from this to player

	//Face player
	float rot = atan2( player_position.y, player_position.x ) * 180 / 3.14159 + 90;
	this->setRotation(rot);

	float dist =
		sqrt( player_position.x * player_position.x
			+ player_position.y * player_position.y );
	player_position *= (dist) ? (1/dist) : dist; // Unit vector
	//Go back if too close
	if(dist < 400) {
		player_position *= -1.f;
	//Stop if comfortable zone
	} else if(dist < 600) {
		player_position *= 0.f;
	}
	player_position *= this->speed;
	player_position *= *(this->d_time);

	//hitbox stuff
	sf::FloatRect copy_of_this = getBoundingBox();
	copy_of_this.left += player_position.x;
	copy_of_this.top += player_position.y;

	//map boundaries
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

	//if running into an enemy, back up a bit
	for(auto x : *(shared_space.normal_enemies)) {
		if(x->getBoundingBox().intersects(copy_of_this))
			player_position *= -1.f;
	}
	for(auto x : *(shared_space.shooter_enemies)) {
		if(x == this) continue;
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

	this->move( player_position ); // actually move

	fire_rate--; // closer to firing projectile
	if(fire_rate <= 0) {  // Bang
		fire_rate = 5000; // RELOAD!
		return 2;		  // You did it
	}

	return 1;

}

sf::Color ShooterE::getColor() {
	return this->color;
}

void ShooterE::setColor(
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
	this->body[8].color = this->color;
	this->body[9].color = this->color;
	this->body[10].color = this->color;
}

//Making your own entities is weird, I have to define my own bounding boxes
sf::FloatRect ShooterE::getBoundingBox() {
	sf::FloatRect bounder = this->body.getBounds();
	bounder.left += this->getPosition().x;
	bounder.left -= bounder.width/2; // Origin is in the middle of the thing
	bounder.top += this->getPosition().y;
	bounder.top -= bounder.height/2;
	return bounder;
}

float ShooterE::getLeft() {
	return getBoundingBox().left;
};
float ShooterE::getRight() {
	return getBoundingBox().left + getBoundingBox().width;
};
float ShooterE::getTop() {
	return getBoundingBox().top;
};
float ShooterE::getBottom() {
	return getBoundingBox().top + getBoundingBox().height;
};
