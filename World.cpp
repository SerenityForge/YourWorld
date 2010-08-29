#include "World.h"

World::World(sf::RenderWindow *AppWindow)  : AppWindow(AppWindow)
{
	MyDrawer = new Drawer(AppWindow, this);
	
	MyEditor = new Editor(AppWindow, this);
	CurrentRoom = NULL;
	Editing = false;
}

World::~World()
{
	delete MyEditor;
	delete MyDrawer;
	delete CurrentRoom;
}

void World::Load(std::string RoomName) {
	if(CurrentRoom)
	{
		delete CurrentRoom;
	}
	
	CurrentRoom = new Room(AppWindow,this);
	CurrentRoom->Load(RoomName);
	
	CurrentRoomName = RoomName;
	
	MyEditor->SetStage(CurrentRoom->MyStage);
	
	CenterPos = b2Vec2(0,0);
	
	Center();
}

void World::HandleEvent(sf::Event Event)
{
	if(Event.Key.Code == sf::Key::P && Event.Type == sf::Event::KeyPressed)
	{
		if(Editing)
		{
			CurrentRoom->Save(CurrentRoomName);
		}
		Load(CurrentRoomName);
		Editing = !Editing;
	}
	else if(Editing)
	{
		MyEditor->HandleEvent(Event);
	}
	else {
		CurrentRoom->HandleEvent(Event);
	}
}

void World::Update() {
	if(!Editing) {
		CurrentRoom->Update();
		Center();
	}
	else {
		MyEditor->Update();
	}
}

void World::Draw() {
	MyDrawer->DrawBackground(CurrentRoom->MyBackground);
	MyDrawer->Draw(CurrentRoom->MyStage, Editing);
	if(!Editing) MyDrawer->Draw(CurrentRoom->MyStage, true);
	MyDrawer->DrawForeground(CurrentRoom->MyBackground);
	if(Editing) MyDrawer->Draw(MyEditor);
}

void World::Center()
{
	b2Vec2 Size = CurrentRoom->MyStage->GetSize();
	b2Vec2 Position = CurrentRoom->MyPlayer->GetPosition();
	
	CenterPos.x = (Position.x < 16)?16:(Position.x > Size.x-16)?Size.x-16:Position.x;
	CenterPos.y = (Position.y < 12)?12:(Position.y > Size.y-12)?Size.y-12:Position.y;
}

b2Vec2 World::GetStagePoint(int ScreenX, int ScreenY)
{
	float X = ((float)ScreenX-400.0)/25.0+CenterPos.x;
	float Y = (300.0-(float)ScreenY)/25.0+CenterPos.y;
	
	return b2Vec2(X,Y);
}

b2Vec2 World::GetStagePoint(int ScreenX, int ScreenY, b2Vec2 Center)
{
	float X = ((float)ScreenX-400.0)/25.0+Center.x;
	float Y = (300.0-(float)ScreenY)/25.0+Center.y;
	
	return b2Vec2(X,Y);
}

sf::Vector2f World::GetScreenPoint(b2Vec2 Pos)
{
	return sf::Vector2f(25.0*(Pos.x-CenterPos.x)+400.0,-25.0*(Pos.y-CenterPos.y)+300.0);
}

sf::Vector2f World::GetScreenPoint(b2Vec2 Pos, b2Vec2 Center)
{
	return sf::Vector2f(25.0*(Pos.x-Center.x)+400.0,-25.0*(Pos.y-Center.y)+300.0);
}