#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <OpenGL/gl.h>

#include <vector>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

#include "InfoReader.h"
#include "InfoWriter.h"

struct BackgroundTile {
	BackgroundTile(int Depth, float ScrollRate, float X, float Y, std::string ImageName) : Depth(Depth), ScrollRate(ScrollRate), X(X), Y(Y), ImageName(ImageName) {
		
		Image.LoadFromFile("sprites/"+ImageName);
		Sprite.SetImage(Image);
	}
	
	std::string ImageName;
	
	sf::Image Image;
	sf::Sprite Sprite;
	
	int Depth;
	
	float ScrollRate;
	
	float X,Y;
};

class Background{
public:
	Background();
	~Background();
	
	void Load(std::string BaseFolder, std::string BackgroundName);
	void AddBackground(std::string BaseFolder, std::string BackgroundTileName);
	
	void Save(std::string BaseFolder, std::string BackgroundName);
	void SaveBackground(std::string BaseFolder, std::string BackgroundTileName, int Background);
	
	std::vector<BackgroundTile*>* GetBackgrounds() { return &Backgrounds; }
	
private:
	std::vector<BackgroundTile*> Backgrounds;
};

#endif