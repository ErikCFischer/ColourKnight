#pragma once

#include <SFML/Graphics.hpp>

struct SharedSpace;

class SplitE : public sf::Drawable, public sf::Transformable {

	sf::VertexArray body;
	sf::Color color;
	float speed = 0.00025;
	float* d_time;
	bool has_sibling = false;
	SplitE* sibling = nullptr;

public:

	SplitE();
	SplitE( sf::Color, float* );
	void setColor( sf::Color );
	sf::Color getColor();
	void setSibling( SplitE* );
	SplitE* getSibling();
	bool hasSibling();
	void killSibling();
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
