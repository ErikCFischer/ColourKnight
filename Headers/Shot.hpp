#pragma once

#include <SFML/Graphics.hpp>

struct SharedSpace;

class Shot : public sf::Drawable, public sf::Transformable {

	sf::VertexArray body;
	sf::Color color;
	sf::Vector2f target;
	float speed = 0.001;
	float* d_time;
	int lifetime = 4000;

public:

	Shot();
	Shot( float*, sf::Vector2f );
	char update( SharedSpace );
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
