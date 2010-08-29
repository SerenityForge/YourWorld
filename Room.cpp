#include "Room.h"

Room::Room(sf::RenderWindow *AppWindow, World* MyWorld) : AppWindow(AppWindow),MyWorld(MyWorld)
{
	
}

Room::~Room()
{
	delete MyStage;
	delete MyPlayer;
	delete MyBackground;
}

void Room::HandleEvent(sf::Event Event)
{
	MyPlayer->HandleEvent(Event);
}

void Room::Load(std::string RoomName)
{
	std::string RoomFolder = RoomName + ".room";
	
	MyStage = new Stage(MyWorld);
	
	MyStage->Load(RoomFolder,"stage");
	
	MyPlayer = new Player;
	
	MyPlayer->Load(RoomFolder,"player",MyStage);
	
	MyBackground = new Background;
	
	MyBackground->Load(RoomFolder,"background");
}

void Room::Save(std::string RoomName)
{
	std::string RoomFolder = RoomName + ".room";
	
	mkdir(RoomFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	MyStage->Save(RoomFolder,"stage");
	MyPlayer->Save(RoomFolder,"player");
	MyBackground->Save(RoomFolder,"background");
}

void Room::Update()
{
	MyStage->Update(1.0/60.0);
	
	MyPlayer->Update(1.0/60.0);
}

void Room::Draw(b2Vec2 CenterPos)
{
	
}