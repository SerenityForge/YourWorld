#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics.hpp>
#include <sys/stat.h>

#include "Stage.h"
#include "Drawer.h"
#include "Player.h"
#include "Background.h"
#include "World.h"

#include "InfoReader.h"
#include "InfoWriter.h"

class Room
{
public:
	Room(sf::RenderWindow *AppWindow, class World* MyWorld);
	~Room();
	
	void Load(std::string RoomName);
	void Save(std::string RoomName);
	
	void HandleEvent(sf::Event Event);
	
	void Update();
	void Draw(b2Vec2 CenterPos);
	
private:
	sf::RenderWindow *AppWindow;
	
	class World* MyWorld;
	
	Stage* MyStage;
	class Player* MyPlayer;
	Background* MyBackground;
	
	friend class World;
};

#endif	