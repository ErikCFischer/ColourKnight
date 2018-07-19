#pragma once

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>


class MapDemo : public sf::Drawable, public sf::Transformable {

	sf::VertexArray mapping;
	sf::Texture tiles;
	int width, height, t_width, t_height;

public:

	MapDemo();
	float getLeft();
	float getRight();
	float getTop();
	float getBottom();

private:

	//This is basically ripped straight from the SFML tutorial, I just cleaned it up slightly
	virtual void draw(
		sf::RenderTarget& target,
		sf::RenderStates states
	) const {

        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &tiles;

        // draw the vertex array
        target.draw(mapping, states);
    }

};
