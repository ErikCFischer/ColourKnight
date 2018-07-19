#pragma once

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable, public sf::Transformable {

	sf::VertexArray body;
	sf::Color color;

public:

	Player();
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
