#include <MapDemo.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Heavily inspired from SFML tutorial
MapDemo::MapDemo() {

	//Load the texture
    tiles.loadFromFile("Assets/TileMap.png");

	//Seed random
	srand (time(NULL));

	width = 100;	// 100 tiles wide
	height = 50;	//  50 tiles tall
	t_width = 30;	// Tiles are 30 pixels wide
	t_height = 30;  // Tiles are 30 pixels tall

	//Make all the points for the squares
	mapping.setPrimitiveType(sf::Quads);
    mapping.resize(width * height * 4); // Squares have 4 corners

	//This is the tile map
	std::vector<int> tilemap;
	tilemap.resize(width * height);

	for(int i = 0; i < (width * height); i++) {
		tilemap[i] = 2;				// Everything is stone
		if( !(rand() % 30) ) {		// 1/30 chance to turn stone to dirt
			tilemap[i] = 1;
			if( !(rand() % 6) ) {	// 1/6 chance to turn dirt to grass
				tilemap[i] = 0;
			}
		}

	}

/*  This is technically working code of how you would be able to use an array from a text file

	std::fstream raw_tile_map;
	raw_tile_map.open("Assets/demomap.tilemap");
	for(int i = 0; i < (width * height); i++) {
		raw_tile_map >> tilemap[i];
	}
	raw_tile_map.close();
*/

    for (int w = 0; w < width; w++) {
        for (int h = 0; h < height; h++) {
            // get the current tile number
            int t_num = tilemap[w + (h * width)];

            // find its position in the tileset texture
            int tw = t_num % (tiles.getSize().x / t_width);
            int th = t_num / (tiles.getSize().x / t_width);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &mapping[(w + (h * width)) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(w * t_width, h * t_height);
            quad[1].position = sf::Vector2f((w + 1) * t_width, h * t_height);
            quad[2].position = sf::Vector2f((w + 1) * t_width, (h + 1) * t_height);
            quad[3].position = sf::Vector2f(w * t_width, (h + 1) * t_height);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tw * t_width, th * t_height);
            quad[1].texCoords = sf::Vector2f((tw + 1) * t_width, th * t_height);
            quad[2].texCoords = sf::Vector2f((tw + 1) * t_width, (th + 1) * t_height);
            quad[3].texCoords = sf::Vector2f(tw * t_width, (th + 1) * t_height);
        }
	}

}

float MapDemo::getLeft() {
	return this->mapping.getBounds().left;
};
float MapDemo::getRight() {
	return this->mapping.getBounds().left + this->mapping.getBounds().width;
};
float MapDemo::getTop() {
	return this->mapping.getBounds().top;
};
float MapDemo::getBottom() {
	return this->mapping.getBounds().top + this->mapping.getBounds().height;
};
