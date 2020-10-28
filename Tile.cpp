#include "Tile.h"
#include <iostream>
//Constructor
Tile::Tile(std::string texture, bool  bomb, int xCoor, int yCoor, bool hasFlag, bool revealed, int  bombCount)
{
	this->texture = texture;
	this->bomb = bomb;
	this->xCoor = xCoor;
	this->yCoor = yCoor;
	this->hasFlag = hasFlag;
	this->revealed = revealed;
	this->bombsCount = bombCount;
}


// Accessors

std::string Tile::getTexture()
{
	return texture;
}

void Tile::setTexture(std::string texture)
{
	this->texture = texture;
}

int Tile::getXCoor()
{
	return xCoor;
}

int Tile::getYCoor()
{
	return yCoor;
}



void Tile::setFlag(bool status)
{
	this->hasFlag = status;
}

bool Tile::getFlag()
{
	return hasFlag;
}

void Tile::setBomb(bool status)
{
	this->bomb = status;
}

bool Tile::getBombStatus()
{
	return bomb;
}

void Tile::setNeighbors(std::vector<Tile> &tiles)
{
	Tile* bottom = nullptr;
	Tile* bottomLeft = nullptr;
	Tile* bottomRight = nullptr;

	Tile* top = nullptr;
	Tile* topLeft = nullptr;
	Tile* topRight = nullptr;

	Tile* left = nullptr;
	Tile *right = nullptr;

	Tile* add = nullptr;
	//top left corner
	add = &tiles.at(1);
	tiles.at(0).neighbors.push_back(add);
	add = &tiles.at(25);
	tiles.at(0).neighbors.push_back(add);
	add = &tiles.at(26);
	tiles.at(0).neighbors.push_back(add);

	//top right corner

	add = &tiles.at(23);
	tiles.at(24).neighbors.push_back(add);
	add = &tiles.at(48);
	tiles.at(24).neighbors.push_back(add);
	add = &tiles.at(49);
	tiles.at(24).neighbors.push_back(add);

	//bottom left corner

	add = &tiles.at(350);
	tiles.at(375).neighbors.push_back(add);
	add = &tiles.at(351);
	tiles.at(375).neighbors.push_back(add);
	add = &tiles.at(376);
	tiles.at(375).neighbors.push_back(add);


	//bottom right corner

	add = &tiles.at(398);
	tiles.at(399).neighbors.push_back(add);
	add = &tiles.at(374);
	tiles.at(399).neighbors.push_back(add);
	add = &tiles.at(373);
	tiles.at(399).neighbors.push_back(add);

	//top row
	int i;
	for (i = 1; i < 24; i++) {
		left = &tiles.at(i - 1);
		right = &tiles.at(i + 1);

		bottom = &tiles.at(i + 25);
		bottomLeft = &tiles.at(i + 24);
		bottomRight = &tiles.at(i + 26);

		tiles.at(i).neighbors.push_back(left);
		tiles.at(i).neighbors.push_back(right);
		tiles.at(i).neighbors.push_back(bottom);
		tiles.at(i).neighbors.push_back(bottomLeft);
		tiles.at(i).neighbors.push_back(bottomRight);

	}

	//bottom row
	for (i = 376; i < 399; i++) {
		left = &tiles.at(i - 1);
		right = &tiles.at(i + 1);

		top = &tiles.at(i - 25);
		topLeft = &tiles.at(i - 24);
		topRight = &tiles.at(i - 26);


		tiles.at(i).neighbors.push_back(left);
		tiles.at(i).neighbors.push_back(right);
		tiles.at(i).neighbors.push_back(top);
		tiles.at(i).neighbors.push_back(topLeft);
		tiles.at(i).neighbors.push_back(topRight);
	}
	
	for (i = 0; i < 400; i++) {
		//left side
		if (i != 0 && i != 375 && (i % 25 == 0)) {
			right = &tiles.at(i + 1);
			
			top = &tiles.at(i - 25);
			topRight = &tiles.at(i - 24);

			bottom = &tiles.at(i + 25);
			bottomRight = &tiles.at(i + 26);

			
			tiles.at(i).neighbors.push_back(right);
			tiles.at(i).neighbors.push_back(top);
			tiles.at(i).neighbors.push_back(topRight);
			tiles.at(i).neighbors.push_back(bottom);
			tiles.at(i).neighbors.push_back(bottomRight);

		}
		//right side
		else if (i != 24 && i != 399 && ((i + 1) % 25 == 0)) {
			left = &tiles.at(i - 1);

			top = &tiles.at(i - 25);
			topLeft = &tiles.at(i - 26);

			bottom = &tiles.at(i + 25);
			bottomLeft = &tiles.at(i + 24);

			
			tiles.at(i).neighbors.push_back(left);

			tiles.at(i).neighbors.push_back(top);
			tiles.at(i).neighbors.push_back(topLeft);

			tiles.at(i).neighbors.push_back(bottom);
			tiles.at(i).neighbors.push_back(bottomLeft);

			
		}// middle
		else if ( (i > 24) && (i < 375) && !((i + 1) % 25 == 0) && !(i % 25 == 0)){
			left = &tiles.at(i - 1);
			right = &tiles.at(i + 1);
			
			top = &tiles.at(i - 25);
			topLeft = &tiles.at(i - 26);
			topRight = &tiles.at(i - 24);

			bottom = &tiles.at(i + 25);
			bottomLeft = &tiles.at(i + 24);
			bottomRight = &tiles.at(i + 26);


			tiles.at(i).neighbors.push_back(right);
			tiles.at(i).neighbors.push_back(left);

			tiles.at(i).neighbors.push_back(top);
			tiles.at(i).neighbors.push_back(topRight);
			tiles.at(i).neighbors.push_back(topLeft);

			tiles.at(i).neighbors.push_back(bottom);
			tiles.at(i).neighbors.push_back(bottomRight);
			tiles.at(i).neighbors.push_back(bottomLeft);
		}
	}
	

}

std::vector<Tile*> Tile::getNeighbors()
{
	
	return neighbors;
}

// tester functions delete when done
void Tile::setID(int x)
{
	this->id = x;

}

int Tile::getID()
{
	return id;
}
void Tile::calculateBombs(std::vector<Tile*>&  neighbors) 
{
	int count = 0;
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors.at(i)->getBombStatus()) {
			count++;
		}
	}
	
	this->bombsCount = count;
}

int Tile::getBombCount()
{
	return bombsCount;
}
void Tile::setRevealed(bool status)
{
	this->revealed = status;
}
bool Tile::getReavealed()
{
	return revealed;
}
void Tile::setDraw(bool status)
{
	this->draw = status;
}
bool Tile::getDraw()
{
	return draw;
}
////////

//Info








