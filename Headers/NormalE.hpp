#pragma once

#include <SFML/Graphics.hpp>

struct SharedSpace;

class NormalE : public sf::Drawable, public sf::Transformable {

	sf::VertexArray body;
	sf::Color color;
	float speed = 0.00015;
	float* d_time;

public:

	NormalE();
	NormalE( sf::Color, float* );
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
