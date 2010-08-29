#ifndef OBJECT_H
#define OBJECT_H

#include <Box2d/box2d.h>
#include <SFML/Graphics.hpp>

struct FixtureData {
	FixtureData() {}
	FixtureData(int Type) : Type(Type) { }
	FixtureData(int Type, b2Vec2 Pos, float Angle, float Width, float Height) : Type(Type),FirstPos(Pos),SecondPos(b2Vec2_zero),Angle(Angle),Width(Width),Height(Height) {}
	FixtureData(int Type, b2Vec2 FirstPos, b2Vec2 SecondPos) : Type(Type),FirstPos(FirstPos),SecondPos(SecondPos),Angle(0),Width(0),Height(0) {}
	
	b2Fixture* Fixture;
	
	int Type;
	b2Vec2 FirstPos,SecondPos;
	float Angle,Width,Height;
};

struct BodyData {
	BodyData(std::string SpriteName, bool Save=true) : Save(Save),SpriteName(SpriteName),Scroll(false) {LoadSprite();}
	BodyData(bool Static, b2Vec2 Pos, std::string SpriteName, bool Scroll, float Angle=0, float ScrollRate=0, bool Save=true) : Static(Static),Scroll(Scroll),Pos(Pos),Angle(Angle),Save(Save),ScrollRate(ScrollRate),SpriteName(SpriteName) {LoadSprite();}
	
	std::string SpriteName;
	sf::Image Image;
	sf::Sprite Sprite;
	
	b2Body* Body;
	
	bool Scroll;
	float ScrollRate;
	
	bool Save;
	bool Static;
	b2Vec2 Pos;
	float Angle;
	
	void LoadSprite() {
		Image.LoadFromFile("sprites/"+SpriteName);
		Sprite.SetImage(Image);
	}
};

#endif