#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <set> 
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Tile.h"
#include "Random.h"
// init functions, they should only be called once and never inside any sort of loop
void initTiles(std::vector<Tile> &tiles);
void initTextures(std::map<std::string, sf::Texture> &Textures);
void generateRandBombs(std::vector<Tile> &tiles);

void drawTiles(std::vector<Tile> &tiles, std::map<std::string, sf::Texture> &Textures, sf::RenderWindow &window, bool debug, bool won, bool lost);
void drawSpecialButtons(std::map<std::string, sf::Texture> &Textures, sf::RenderWindow &window, bool lostGame, bool wonGame);
void drawDigits(int const flagCount, sf::RenderWindow &window, std::map<std::string, sf::Texture> &Textures);
void getDigit(std::string digit, std::vector<std::string> &draw);

// will find which tile on the board was clicked and return index of tile clicked :: only for main board //if no tile was clicked return -1
int tileClicked(std::vector<Tile> &tiles, int xCoorMouse, int yCoorMouse);
/// will find if a special button is clicked
int specialClicked(int xCoorMouse, int yCoorMouse);
//loads preset bombs, sets tiles that need to be bombs to true
void loadBombs(std::string choice, std::vector<int> &bombs);

void revealNeighbors(std::vector<Tile> &tiles, int selectedTile);
void setRevaleadTextures(std::vector<Tile> &tiles, int selectedTile);

int bombTotal(std::vector<Tile> &tiles);

bool wonGame(std::vector<Tile> &tiles);

int main()
{

	//basic vars will be used throught like mouse
	sf::Mouse mouse;
	int flagCount = 0;
	int bombCount = 0;
	bool lostGame = false;
	bool won = false;
	bool revealBombs = false;
	
	std::vector<int> bombs;
	 
	

	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
	// always draw from this, making the necessary adjustments based on when/where user clicks
	std::vector<Tile> tiles;
	// this map will associate the texture string of tiles with the correct texture, which will be initilazied at the begining of the game
	std::map<std::string, sf::Texture> Textures;
	// going to initialize tiles to all be hidden, along with initializing their coords, 
	// the only thing that will be changed from this  point is the bool for whether the tile has a bomb and there sprite,'
	//but the sprite shouldnt be changed until that tile is clicked
	initTiles(tiles);
	initTextures(Textures);
	generateRandBombs(tiles);
	//defines each tiles neighbors, a tiles neighbor never changes
	tiles.at(0).setNeighbors(tiles);
	//gives each tile an id corresponding to its index in the vector of tiles
	
	//temp vars to test functions , delete all of this within comments when done
	///////////////////////////
	
	int counter = 0;
	
	/////////////////////////////////////

	while (window.isOpen())
	{



		sf::Event event;


		bombCount = bombTotal(tiles);
		won = wonGame(tiles);

		

		window.clear();
		drawTiles(tiles, Textures, window, revealBombs, won, lostGame);
		drawSpecialButtons(Textures, window, lostGame,won);
		if (won) {
			drawDigits(0, window, Textures);
		}
		else {
			drawDigits(bombCount - flagCount, window, Textures);
		}
		
		
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			
			if (event.type == sf::Event::MouseButtonPressed) {
				// checks for right click, only 2 actions can happen as result, place flag, remove flag
				if (event.mouseButton.button == sf::Mouse::Right && !lostGame && !won) {
					int flagTile = tileClicked(tiles, mouse.getPosition(window).x, mouse.getPosition(window).y);
					bool clickSuccess = false;
					if (flagTile >= 0 && !tiles.at(flagTile).getReavealed()) {
						tiles.at(flagTile).setFlag(!tiles.at(flagTile).getFlag());
						clickSuccess = true;
					}
					if (clickSuccess && tiles.at(flagTile).getFlag()) {
						flagCount++;	
						tiles.at(flagTile).setDraw(true);
					}
					else if(clickSuccess) {
						flagCount--;
						
					}
				
					
				}
				// there are 3 main categories of action
				//1. clicked on board with the following sub actions
				// -clicked on bomb - reveal bomb+ (rest of board) and lose game
				// clicked on safe tile (reveal num of bombs near + tiles without bombs in contact)
				// 2. clicked on special button
				//-happy button - new board
				// -  debug - reveal board
				// - test 1-3, load test board 1-3
				// 3. clicked on space that isnt a button

				else if (event.mouseButton.button == sf::Mouse::Left) {

					int selectedTile = tileClicked(tiles, mouse.getPosition(window).x, mouse.getPosition(window).y);
					bool isFlag;
					if (!lostGame && !won) {
						if (selectedTile >= 0) {
							isFlag = tiles.at(selectedTile).getFlag();
						}

						if (selectedTile >= 0 && tiles.at(selectedTile).getBombStatus() && !isFlag) {
							tiles.at(selectedTile).setRevealed(true);
							tiles.at(selectedTile).setTexture("bomb");
							lostGame = true;

						}
						else if (selectedTile >= 0 && !tiles.at(selectedTile).getBombStatus() && !isFlag) {

							
							
							setRevaleadTextures(tiles, selectedTile);
							revealNeighbors(tiles, selectedTile);
							
							
						}
						// then check special buttons
					}
					int selectedSpecial = specialClicked(mouse.getPosition(window).x, mouse.getPosition(window).y);
					if (selectedSpecial == 1) {
						bombs.clear();
						loadBombs("boards/testboard1.brd", bombs);

					}
					else if (selectedSpecial == 2) {
						bombs.clear();
						loadBombs("boards/testboard2.brd", bombs);
					}
					else if (selectedSpecial == 3) {
						bombs.clear();
						loadBombs("boards/testboard3.brd", bombs);
					}
					else if (selectedSpecial == 10) {

						revealBombs = !revealBombs;
						
						

					}
					
					
					

					
					if (0 <= selectedSpecial && selectedSpecial <= 3) {
						
						lostGame = false;
						won = false;

						
						flagCount = 0;
						initTiles(tiles);
						tiles.at(0).setNeighbors(tiles);
						if (selectedSpecial == 0) {
							generateRandBombs(tiles);
						}
						
				
					}
					// may need to add in feature that first ensures all tiles are first reset before doing this
					if (selectedSpecial >= 1 && selectedSpecial <= 3) {
						for (unsigned int i = 0; i < tiles.size(); i++) {
							if (bombs.at(i) == 1) {
								tiles.at(i).setBomb(true);
								tiles.at(i).setTexture("hidden");
							}
							else {
								tiles.at(i).setBomb(false);
								tiles.at(i).setTexture("hidden");
							}
						}

					}
				}
			}
		}
		
		
		
		window.display();
	}

	return 0;
}


void initTiles(std::vector<Tile> &tiles) {
	tiles.clear();
	// to start the game all tiles will have the hidden tile texture,
	// they will be preset to "not have a  bomb", however this only because the location of the bombs has not let been decided,
	//there will/is be/a function that will generate the locations as a list of 1 and 0s, 
	//then it will retiertate throw the list of tiles and adjust the bool value accordingly
	std::string texture = "hidden";
	int x = 0;
	int y = 0;
	bool bomb = false;
	
	int count = 0; // used to determine when its time to shift to the next row,
	// i.e. if you have added 32 to the x coor 25 times,then you've filled at the row
	// you should then shift to the lower row, reset  x to 0, and increase y in order to actual shift down a row

	for (unsigned int i = 0; i < 400; i++) {
		Tile tile(texture, bomb, x, y, false, false, 0);
		
		tiles.push_back(tile);
		x += 32;
		count++;

		if (count == 25) {
			y += 32;
			count = 0;
			x = 0;
		}
	}
	for (unsigned int j = 0; j <= 399; j++) {
		tiles.at(j).setID(j);
	}
}


void initTextures(std::map<std::string, sf::Texture> &Textures) {

	//only for general cases
	//this will not handle case for bomb count
	//number_1.png
	sf::Texture hidden;
	sf::Texture revealed;
	sf::Texture bomb;
	sf::Texture flag;
	sf::Texture faceHappy;
	sf::Texture faceLose;
	sf::Texture faceWin;
	sf::Texture debug;
	sf::Texture testOne;
	sf::Texture testTwo;
	sf::Texture testThree;
	

	
	hidden.loadFromFile("images/tile_hidden.png");
	revealed.loadFromFile("images/tile_revealed.png");
	bomb.loadFromFile("images/mine.png");
	flag.loadFromFile("images/flag.png");
	faceHappy.loadFromFile("images/face_happy.png");
	faceLose.loadFromFile("images/face_lose.png");
	faceWin.loadFromFile("images/face_win.png");
	debug.loadFromFile("images/debug.png");
	testOne.loadFromFile("images/test_1.png");
	testTwo.loadFromFile("images/test_2.png");
	testThree.loadFromFile("images/test_3.png");
	
	sf::Texture digits_0;
	sf::Texture digits_1;
	sf::Texture digits_2;
	sf::Texture digits_3;
	sf::Texture digits_4;
	sf::Texture digits_5;
	sf::Texture digits_6;
	sf::Texture digits_7;
	sf::Texture digits_8;
	sf::Texture digits_9;
	sf::Texture digits_;
	
	


	digits_0.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
	digits_1.loadFromFile("images/digits.png", sf::IntRect(21, 0, 21, 32));
	digits_2.loadFromFile("images/digits.png", sf::IntRect(42, 0, 21, 32));
	digits_3.loadFromFile("images/digits.png", sf::IntRect(63, 0, 21, 32));
	digits_4.loadFromFile("images/digits.png", sf::IntRect(84, 0, 21, 32));
	digits_5.loadFromFile("images/digits.png", sf::IntRect(105, 0, 21, 32));
	digits_6.loadFromFile("images/digits.png", sf::IntRect(126, 0, 21, 32));
	digits_7.loadFromFile("images/digits.png", sf::IntRect(147, 0, 21, 32));
	digits_8.loadFromFile("images/digits.png", sf::IntRect(168, 0, 21, 32));
	digits_9.loadFromFile("images/digits.png", sf::IntRect(189, 0, 21, 32));
	digits_.loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
	

	sf::Texture number_1;
	sf::Texture number_2;
	sf::Texture number_3;
	sf::Texture number_4;
	sf::Texture number_5;
	sf::Texture number_6;
	sf::Texture number_7;
	sf::Texture number_8;
	
	
	number_1.loadFromFile("images/number_1.png");
	number_2.loadFromFile("images/number_2.png");
	number_3.loadFromFile("images/number_3.png");
	number_4.loadFromFile("images/number_4.png");
	number_5.loadFromFile("images/number_5.png");
	number_6.loadFromFile("images/number_6.png");
	number_7.loadFromFile("images/number_7.png");
	number_8.loadFromFile("images/number_8.png");
	
	
	Textures.emplace("hidden", hidden);
	Textures.emplace("revealed", revealed);
	Textures.emplace("bomb", bomb);
	Textures.emplace("flag", flag);
	Textures.emplace("Happy", faceHappy);
	Textures.emplace("lose", faceLose);
	Textures.emplace("win", faceWin);
	Textures.emplace("debug", debug);
	Textures.emplace("testOne", testOne);
	Textures.emplace("testTwo", testTwo);
	Textures.emplace("testThree", testThree);
	Textures.emplace("1", number_1);
	Textures.emplace("2", number_2);
	Textures.emplace("3", number_3);
	Textures.emplace("4", number_4);
	Textures.emplace("5", number_5);
	Textures.emplace("6", number_6);
	Textures.emplace("7", number_7);
	Textures.emplace("8", number_8);
	Textures.emplace("0-dig", digits_0);
	Textures.emplace("1-dig", digits_1);
	Textures.emplace("2-dig", digits_2);
	Textures.emplace("3-dig", digits_3);
	Textures.emplace("4-dig", digits_4);
	Textures.emplace("5-dig", digits_5);
	Textures.emplace("6-dig", digits_6);
	Textures.emplace("7-dig", digits_7);
	Textures.emplace("8-dig", digits_8);
	Textures.emplace("9-dig", digits_9);
	Textures.emplace("-dig", digits_);
}


void generateRandBombs(std::vector<Tile> &tiles) {
	std::set<int> randomBombs;
	while (randomBombs.size() != 50) {
		randomBombs.insert(Random::randomInteger(0, 399));
	}
	for (auto itr = randomBombs.begin(); itr != randomBombs.end(); itr++) {
		tiles.at(*itr).setBomb(true);
	}
}

void drawTiles(std::vector<Tile> &tiles, std::map<std::string, sf::Texture> &Textures, sf::RenderWindow &window, bool reveal, bool won, bool lost) {
	sf::Sprite revealed(Textures.find("revealed")->second);
	sf::Sprite flag(Textures.find("flag")->second);
	sf::Sprite debug(Textures.find("bomb")->second);
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (tiles.at(i).getReavealed() && !tiles.at(i).getBombStatus() && tiles.at(i).getBombCount() == 0) {
			revealNeighbors(tiles, i);
		}
		else if(tiles.at(i).getReavealed() ) {
			
  		    revealed.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
			window.draw(revealed);
			
		}
		
		std::string stringTextCurr = tiles.at(i).getTexture();
		sf::Texture textCurr = Textures.find(stringTextCurr)->second;
		sf::Sprite curr(textCurr);
		curr.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
		
		window.draw(curr);
		if (tiles.at(i).getFlag() || (won && tiles.at(i).getBombStatus())) {
			
			
			flag.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
			window.draw(flag);
		}
		if (lost && tiles.at(i).getBombStatus()) {
			revealed.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
			debug.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
			window.draw(revealed);
			window.draw(debug);
		}
		
		if ( tiles.at(i).getBombStatus() && reveal) {
			
			debug.setPosition(tiles.at(i).getXCoor(), tiles.at(i).getYCoor());
			window.draw(debug);
		}
		
		
	}

	

}

int tileClicked(std::vector<Tile> &tiles, int xCoorMouse, int yCoorMouse) {
	
	for (unsigned int i = 0; i < tiles.size(); i++) {
		int xMax = tiles.at(i).getXCoor() + 32;
		int yMax = tiles.at(i).getYCoor() + 32;
		if (xCoorMouse <= xMax && yCoorMouse <= yMax) {
			
			return i;
			
		}
	}


	return -1;
}



void drawSpecialButtons(std::map<std::string, sf::Texture> &Textures, sf::RenderWindow &window, bool lostGame, bool wonGame) {
	sf::Sprite face(Textures.find("Happy")->second);
	sf::Sprite lost(Textures.find("lose")->second);
	sf::Sprite win(Textures.find("win")->second);
	sf::Sprite debug(Textures.find("debug")->second);
	sf::Sprite testOne(Textures.find("testOne")->second);
	sf::Sprite testTwo(Textures.find("testTwo")->second);
	sf::Sprite testThree(Textures.find("testThree")->second);
	if (!lostGame && !wonGame){
		face.setPosition(368, 512);
		window.draw(face);
	}
	if (lostGame) {
		lost.setPosition(368, 512);
		window.draw(lost);
	}
	if (wonGame) {
		win.setPosition(368, 512);
		window.draw(win);
	}
	


	debug.setPosition(496, 512);
	testOne.setPosition(560, 512);
	testTwo.setPosition(624, 512);
	testThree.setPosition(688, 512);
	
	window.draw(debug);
	window.draw(testOne);
	window.draw(testTwo);
	window.draw(testThree);


}

int specialClicked(int xCoorMouse, int yCoorMouse) {


	int baseX = 368;
	int topX = 432;
	int baseY = 512;
	int topY = 576;

	bool yValue = yCoorMouse >= baseY;
	bool yValueTop = yCoorMouse <= topY;
	
	if (!yValue || !yValueTop) {
		return -1;
	}
	
	//xValue in between whether its between xBase and XTop
	bool xValue = xCoorMouse >= baseX;
	bool xValueTop = xCoorMouse <= topX;
	
	if (xValue && xValueTop) {
		
		return 0;
	}
	
	
	baseX += 128;
	topX += 128;
	xValue = xCoorMouse >= baseX;
	xValueTop = xCoorMouse <= topX;

	if (xValue && xValueTop) {
		return 10;
	}

	baseX += 64;
	topX += 64;
	xValue = xCoorMouse >= baseX;
	xValueTop = xCoorMouse <= topX;

	if (xValue && xValueTop) {
		return 1;
	}

	baseX += 64;
	topX += 64;
	xValue = xCoorMouse >= baseX;
	xValueTop = xCoorMouse <= topX;

	if (xValue && xValueTop) {
		return 2;
	}

	baseX += 64;
	topX += 64;
	xValue = xCoorMouse >= baseX;
	xValueTop = xCoorMouse <= topX;

	if (xValue && xValueTop) {
		return 3;
	}
	

	return -1; 

}


void loadBombs(std::string choice, std::vector<int> &bombs) {
		std::ifstream input;
		input.open(choice);
		std::string storage;
		int bombStatus;
		for (unsigned int j = 0; j < 16; j++) {
			getline(input, storage);

			for (unsigned int i = 0; i < storage.length(); i++) {

				bombStatus = ((int)storage.at(i) - 48);
				bombs.push_back(bombStatus);
				

			}
		}
		
		
}



void revealNeighbors(std::vector<Tile> &tiles, int selectedTile) {


	tiles.at(selectedTile).setRevealed(true);
	

	std::vector<Tile*> neighbors = tiles.at(selectedTile).getNeighbors();


	tiles.at(selectedTile).calculateBombs(neighbors);
	int bombs = tiles.at(selectedTile).getBombCount();
	if (bombs == 0) {
		for (unsigned int i = 0; i < neighbors.size(); i++) {

			if (!neighbors.at(i)->getBombStatus() && !neighbors.at(i)->getFlag() && !neighbors.at(i)->getDraw()) {

				if (!neighbors.at(i)->getReavealed()) {
					neighbors.at(i)->setRevealed(true);
					setRevaleadTextures(tiles, neighbors.at(i)->getID());
				}

			}
			

		}
	}



	neighbors.clear();

}


void setRevaleadTextures(std::vector<Tile> &tiles, int selectedTile) {

	std::vector<Tile*> neighbors = tiles.at(selectedTile).getNeighbors();
	tiles.at(selectedTile).calculateBombs(neighbors);
	int bombs = tiles.at(selectedTile).getBombCount();
	

	switch (bombs)
	{
	case 1:
		tiles.at(selectedTile).setTexture("1");
		break;
	case 2:
		tiles.at(selectedTile).setTexture("2");
		break;
	case 3:
		tiles.at(selectedTile).setTexture("3");
		break;
	case 4:
		tiles.at(selectedTile).setTexture("4");
		break;
	case 5:
		tiles.at(selectedTile).setTexture("5");
		break;
	case 6:
		tiles.at(selectedTile).setTexture("6");
		break;
	case 7:
		tiles.at(selectedTile).setTexture("7");
		break;
	case 8:
		tiles.at(selectedTile).setTexture("8");
		break;

	default:
		tiles.at(selectedTile).setTexture("revealed");
		break;
	}

	if (tiles.at(selectedTile).getBombStatus()) {
		tiles.at(selectedTile).setTexture("bomb");
	}
}


void drawDigits(int const flagCount, sf::RenderWindow &window, std::map<std::string, sf::Texture> &Textures) {
	std::string count = std::to_string(flagCount);

	int yCoor = 512;
	int xCoor = 0;
	

	std::vector<std::string> draw;

	
	if (flagCount >= 0) {
		switch (count.size())
		{
		case 1: 
			draw.push_back("0-dig");
			draw.push_back("0-dig");
			getDigit(count, draw);
			break;
		case 2:
			draw.push_back("0-dig");
			getDigit(count, draw);
			break;
		case 3:
			getDigit(count, draw);
			break;
		}
	}
	if (flagCount < 0) {
		count = count.substr(1);
		switch (count.size())
		{
		case 1:
			draw.push_back("-dig");
			draw.push_back("0-dig");
			draw.push_back("0-dig");
			getDigit(count, draw);
			break;
		case 2:
			draw.push_back("-dig");		
			draw.push_back("0-dig");
			getDigit(count, draw);
			break;
		case 3:
			draw.push_back("-dig");
			getDigit(count, draw);
			break;
		default:
			break;
		}

	}




	for (unsigned int j = 0; j < draw.size(); j++) {

		sf::Sprite digit(Textures.find(draw.at(j))->second);
		digit.setPosition(xCoor, yCoor);
		xCoor += 21;
		window.draw(digit);
	}




}


void getDigit(std::string digit, std::vector<std::string> &draw) {
	
	for (unsigned int i = 0; i < digit.size(); i++) {
		switch (digit.at(i))
		{
		
		case '0':
			draw.push_back("0-dig");

			break;
		case '1':
			draw.push_back("1-dig");

			break;
		case '2':
			draw.push_back("2-dig");

			break;
		case '3':
			draw.push_back("3-dig");

			break;
		case '4':
			draw.push_back("4-dig");

			break;
		case '5':
			draw.push_back("5-dig");

			break;
		case '6':
			draw.push_back("6-dig");

			break;
		case '7':
			draw.push_back("7-dig");

			break;
		case '8':
			draw.push_back("8-dig");

			break;
		case '9':
			draw.push_back("9-dig");

			break;
		default:
			break;
		}


	}
}


bool wonGame(std::vector<Tile> &tiles) {
	int bomb = bombTotal(tiles);
	int revealedTotal = 0;
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (tiles.at(i).getReavealed() && !tiles.at(i).getBombStatus()) {
			revealedTotal++;
		}
	}
	if (revealedTotal + bomb == 400) {
		return true;	
	}
	return false;
}



int bombTotal(std::vector<Tile> &tiles) {
	int result = 0;
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (tiles.at(i).getBombStatus()) {
			result++;
		}
	}
	return result;
}


