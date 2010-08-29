#ifndef STAGE_H
#define STAGE_H

#include <Box2d/box2d.h>
#include <iostream>
#include <sys/stat.h>

#include "Object.h"
#include "World.h"

#include "InfoReader.h"
#include "InfoWriter.h"

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& Point)
	{
		MyPoint = Point;
		MyFixture = NULL;
	}
	bool ReportFixture(b2Fixture* Fixture){
		if(Fixture->TestPoint(MyPoint))
		{
			MyFixture = Fixture;
			
			return false;
		}
		return true;
	}
	b2Vec2 MyPoint;
	b2Fixture* MyFixture;
};

class Stage
{
public:
	Stage(class World* MyWorld);
	~Stage();
	
	void Update(float Time);
	
	b2Body* MakeBody(b2Vec2 Pos, float Angle, bool Static, std::string SpriteName="testbox.png", bool Scroll=false, float ScrollRate=0);
	
	b2Fixture* AddLine(b2Body* Body, b2Vec2 FirstPoint, b2Vec2 SecondPoint);
	b2Fixture* AddPlatform(b2Body* Body, float Width, float Height, b2Vec2 Pos=b2Vec2_zero, float Angle=0);
	
	b2Body* SelectBody(float X, float Y);
	b2Fixture* SelectFixture(float X, float Y);
	
	void Load(std::string BaseFolder, std::string StageName);
	void AddBody(std::string BaseFolder, std::string BodyName);
	void AddFixture(std::string BaseFolder, std::string FixtureName, b2Body* Body);
	
	void Save(std::string BaseFolder, std::string StageName);
	void SaveBody(std::string BaseFolder, std::string BodyName, b2Body* Body);
	void SaveFixture(std::string BaseFolder, std::string FixtureName, b2Fixture* Fixture);
	
	b2World* GetWorld() { return World; }
	
	b2Vec2 GetSize() { return Size; }
	
	b2Body* GetGround() { return GroundBody; }
	
private:
	b2Fixture* SetWall(b2Fixture* Wall);
	
	b2Vec2 Gravity;
	b2World *World;
	
	class World* MyWorld;
	
	b2Vec2 Size;
	
	b2Body* GroundBody;
};

#endif