
#pragma once
#include <string>
#include <vector>

class Tile
{
	
private:
	
	std::string texture;
	
	int xCoor;
	int yCoor;
	int bombsCount;
	int id;
	std::vector<Tile*> neighbors;
	bool hasFlag;
	bool bomb;
	bool revealed;
	bool draw; 




public:
	Tile(std::string texture, bool bomb, int xCoor, int yCoor, bool hasFlag, bool revealed, int  bombCount);

	//Acessors

	std::string getTexture();
	void setTexture(std::string texture);
	int getXCoor();
	int getYCoor();

	//Info

	void setFlag(bool status);
	bool getFlag();


	void setBomb(bool status);
	bool getBombStatus();

	void setNeighbors(std::vector<Tile> &tiles);
	std::vector<Tile*> getNeighbors();


	void setID(int x);
	int getID();




	void calculateBombs(std::vector<Tile*>& neighbors)  ;
	int getBombCount();

	void setRevealed(bool status);
	bool getReavealed();


	void setDraw(bool status);
	bool getDraw();












};

