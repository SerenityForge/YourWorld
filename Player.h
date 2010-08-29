#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <Box2d/box2d.h>

#include "Stage.h"
#include "Object.h"

#include "InfoReader.h"
#include "InfoWriter.h"

class Player
{
public:
	Player();
	~Player();
	
	void Load(std::string BaseFolder, std::string PlayerName, class Stage* Stage);
	void Save(std::string BaseFolder, std::string PlayerName);
	
	void HandleEvent(sf::Event Event);
	
	void Update(float Time);
	
	b2Vec2 GetPosition() {
		return GetPlayerBody()->GetPosition();
	}
private:
	b2Vec2 MyPos;
	
	bool isTouching(b2Fixture* Test);
	
	b2Body* GetPlayerBody() {
		return (Climbing)?KinematicBody:DynamicBody;
	}
	
	b2Body *DynamicBody,*KinematicBody;
	
	bool R,L,U,D, Jump;
	
	bool Climbing;
	float ClimbTime;
	bool ClimbDirection;
	
	b2Fixture *GroundTester, *HitTesterL, *HitTesterR, *ClimbTester, *BodyFixture, *KGroundTester;
};

#endif