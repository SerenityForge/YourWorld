#ifndef DRAWER_H
#define DRAWER_H

#include <SFML/Graphics.hpp>
#include <Box2d/box2d.h>

#include "Stage.h"
#include "Editor.h"
#include "Background.h"

#include "World.h"

class Drawer
{
public:
	Drawer(sf::RenderWindow *AppWindow, class World* MyWorld) : AppWindow(AppWindow),MyWorld(MyWorld) {}
	~Drawer();
	
	void Draw(class Stage* Stage, bool Outline);
	void Draw(class Editor* Editor);
	
	void DrawForeground(Background *Background);
	void DrawBackground(Background *Background);
	
private:
	void DrawBGTile(BackgroundTile *Tile);
	
	void DrawText(float X, float Y, std::string Text, bool Selected=false);
	
	sf::RenderWindow *AppWindow;
	class World *MyWorld;
};

#endif