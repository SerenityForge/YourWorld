#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <Box2d/box2d.h>

#include "Room.h"
#include "Editor.h"

#include "InfoReader.h"
#include "InfoWriter.h"

class World
{
public:
	World(sf::RenderWindow *AppWindow);
	~World();
	
	void Load(std::string RoomName);
	
	void HandleEvent(sf::Event Event);
	
	void Update();
	void Draw();
	
	b2Vec2 GetStagePoint(int ScreenX, int ScreenY);
	b2Vec2 GetStagePoint(int ScreenX, int ScreenY, b2Vec2 Center);
	
	sf::Vector2f GetScreenPoint(b2Vec2 Pos);
	sf::Vector2f GetScreenPoint(b2Vec2 Pos, b2Vec2 Center);
	
	b2Vec2 GetCenter() { return CenterPos; }
	
private:
	void Center();
	
	sf::RenderWindow *AppWindow;
	
	bool Editing;
	
	b2Vec2 CenterPos;
	
	class Editor* MyEditor;
	class Drawer* MyDrawer;
	
	class Room *CurrentRoom;
	std::string CurrentRoomName;
	
	friend class Editor;
};

#endif