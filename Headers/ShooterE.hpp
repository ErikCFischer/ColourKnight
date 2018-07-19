#pragma once

#include <SFML/Graphics.hpp>

struct SharedSpace;

class ShooterE : public sf::Drawable, public sf::Transformable {

	sf::VertexArray body;
	sf::Color color;
	float speed = 0.00005;
	float* d_time;
	int fire_rate = 0;

public:

	ShooterE();
	ShooterE( sf::Color, float* );
	char update( SharedSpace );
	sf::Color getColor();
	void setColor( sf::Color );
	sf::FloatRect getBoundingBox();
	float getLeft();
	float getRight();
	float getTop();
	float getBottom();

private:

	//This is basically ripped straight from the SFML tutorial, I just cleaned it up slightly
	virtual void draw(
		sf::RenderTarget& render,
		sf::RenderStates transformation
	) const {

        transformation.transform *= getTransform();
        transformation.texture = NULL;

        render.draw( body, transformation );

    }

};
