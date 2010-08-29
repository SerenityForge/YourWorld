#ifndef EDITOR_H
#define EDITOR_H

#include <SFML/Graphics.hpp>
#include <Box2d/box2d.h>
#include <cmath>

#include "Stage.h"
#include "World.h"

class Editor
{
public:
	Editor(sf::RenderWindow *AppWindow, class World *MyWorld);
	~Editor();
	
	void SetStage(class Stage* Stage);
	
	void Update();
	void HandleEvent(sf::Event Event);
	
	int GetSelectedMenu() { return SelectedMenu; }
	int GetSelectedOption() { return SelectedOption; }
private:
	enum Options {
		Create=0,
		Edit=1,
		Destroy=2
	};
	
	enum CreateOptions {
		DPlat=0,
		SPlat=1,
		SLine=2
	};
	
	enum EditOptions {
		Rotate=3,
		Move=4
	};
	
	enum DestroyOptions {
		BDestroy=5
	};
	
	void SetRotate(float Angle);
	void FinalizeRotate(float Angle);
	
	void SetMove(b2Vec2 Position);
	void FinalizeMove(b2Vec2 Position);
	
	sf::RenderWindow *AppWindow;
	class World* MyWorld;
	Stage* MyStage;
	
	int SelectedMenu,SelectedOption;
	bool UseFixture;
	
	bool FirstClick,IsDragging;
	
	b2Body* SelectedBody;
	b2Fixture* SelectedFixture;
	
	bool Fixture;
	
	b2Vec2 Position,GrabPoint;
};

#endif