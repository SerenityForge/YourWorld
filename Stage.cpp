#include "Stage.h"

Stage::Stage(class World* MyWorld) : MyWorld(MyWorld)
{
	Gravity = b2Vec2(0,-10);
	World = new b2World(Gravity,true);
	
	b2BodyDef GroundBodyDef;
	GroundBodyDef.position.Set(0,0);
	GroundBodyDef.angle = 0;
	
	GroundBody = World->CreateBody(&GroundBodyDef);
	
	GroundBody->SetUserData(new BodyData(true,b2Vec2_zero,"trans.png",false));
}

Stage::~Stage()
{
	for(b2Body* Body = World->GetBodyList(); Body; Body=Body->GetNext())
	{
		for(b2Fixture* Fixture = Body->GetFixtureList(); Fixture; Fixture=Fixture->GetNext())
		{
			delete (FixtureData*)Fixture->GetUserData();
		}
		
		delete (BodyData*)Body->GetUserData();
	}
	
	delete World;
}

void Stage::Update(float Time)
{
	World->Step(Time, 10, 10);
	
	for(b2Body* Body = World->GetBodyList(); Body; Body=Body->GetNext())
	{
		BodyData* Data = (BodyData*)Body->GetUserData();
		
		if(Data->Scroll)
		{
			Body->SetTransform(b2Vec2(Data->Pos.x-Data->ScrollRate*MyWorld->GetCenter().x,Data->Pos.y),Body->GetAngle());
		}
	}
}

void Stage::Load(std::string BaseFolder, std::string StageName)
{
	std::string StageFolder = BaseFolder + "/" + StageName + ".stage";
	
	InfoReader Reader;
	Reader.Load(StageFolder + "/stage.info");
	
	for(int i=0,end=Reader.GetOptionsNum(); i<end; i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "size")
		{
			Size.x = GetOption.GetFloat(0);
			Size.y = GetOption.GetFloat(1);
			
			SetWall(AddLine(GroundBody,b2Vec2(0,0),b2Vec2(Size.x,0)));
			SetWall(AddLine(GroundBody,b2Vec2(0,0),b2Vec2(0,Size.y)));
			SetWall(AddLine(GroundBody,b2Vec2(Size.x,0),b2Vec2(Size.x,Size.y)));
			SetWall(AddLine(GroundBody,b2Vec2(0,Size.y),b2Vec2(Size.x,Size.y)));
			
			
		}
		else if(GetOption.GetName() == "body")
		{
			AddBody(StageFolder,GetOption.GetString(0));
		}
	}
}

void Stage::AddBody(std::string BaseFolder, std::string BodyName)
{
	std::string BodyFolder = BaseFolder + "/" + BodyName;
	
	InfoReader Reader;
	Reader.Load(BodyFolder + "/body.info");
	
	bool Static,Scroll;
	b2Vec2 Pos;
	float Angle,ScrollRate;
	
	std::string SpriteName;
	
	std::vector<std::string> Fixtures;
	
	for(int i=0,end=Reader.GetOptionsNum(); i<end; i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "pos")
		{
			Pos.x = GetOption.GetFloat(0);
			Pos.y = GetOption.GetFloat(1);
		}
		else if(GetOption.GetName() == "angle")
		{
			Angle = GetOption.GetFloat(0);
		}
		else if(GetOption.GetName() == "static")
		{
			Static = GetOption.GetBool(0);
		}
		else if(GetOption.GetName() == "sprite")
		{
			SpriteName = GetOption.GetString(0);
		}
		else if(GetOption.GetName() == "fixture")
		{
			Fixtures.push_back(GetOption.GetString(0));
		}
		else if(GetOption.GetName() == "scroll")
		{
			Scroll = GetOption.GetBool(0);
		}
		else if(GetOption.GetName() == "scrollrate")
		{
			ScrollRate = GetOption.GetFloat(0);
		}
	}
	b2Body* Body;
	
	if(!Static) 
	{	
		Body = MakeBody(Pos, Angle, Static, SpriteName, Scroll, ScrollRate);
	}
	else 
	{	
		Body = GroundBody;
	}
	
	for(int i=0; i<Fixtures.size(); i++)
	{
		AddFixture(BodyFolder,Fixtures.at(i),Body);
	}
}

void Stage::AddFixture(std::string BaseFolder, std::string FixtureName, b2Body* Body)
{
	InfoReader Reader;
	Reader.Load(BaseFolder+"/"+FixtureName+".info");
	
	int Type;
	
	b2Vec2 Pos,SecondPos;
	float Angle,Width,Height;
	
	for(int i=0,end=Reader.GetOptionsNum(); i<end; i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "type")
		{
			Type = GetOption.GetInt(0);
		}
		else if(GetOption.GetName() == "pos")
		{
			Pos.x = GetOption.GetFloat(0);
			Pos.y = GetOption.GetFloat(1);
		}
		else if(GetOption.GetName() == "secondpos")
		{
			SecondPos.x = GetOption.GetFloat(0);
			SecondPos.y = GetOption.GetFloat(1);
		}
		else if(GetOption.GetName() == "angle")
		{
			Angle = GetOption.GetFloat(0);
		}
		else if(GetOption.GetName() == "size")
		{
			Width = GetOption.GetFloat(0);
			Height = GetOption.GetFloat(1);
		}
	}
	
	switch (Type) {
		case 0:
			AddPlatform(Body, Width, Height, Pos, Angle);
			break;
		case 1:
			AddLine(Body, Pos, SecondPos);
			break;
		default:
			break;
	}
}


void Stage::Save(std::string BaseFolder, std::string StageName)
{
	std::string StageFolder = BaseFolder + "/" + StageName + ".stage";
	
	mkdir(StageFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	InfoWriter Writer;
	
	Writer.AddOption("size")->Float(Size.x)->Float(Size.y);
	
	int i=0;
	
	for(b2Body* Body=World->GetBodyList(); Body != NULL; Body=Body->GetNext())
	{
		if(((BodyData*)Body->GetUserData())->Save)
		{
			std::stringstream Stream;
			Stream << "Body" << i;
			
			Writer.AddOption("body")->String(Stream.str());
			
			SaveBody(StageFolder, Stream.str(), Body);
			
			i++;
		}
	}
	
	Writer.Write(StageFolder+"/stage.info");
}

void Stage::SaveBody(std::string BaseFolder, std::string BodyName, b2Body* Body)
{
	std::string BodyFolder = BaseFolder + "/" + BodyName;
	
	mkdir(BodyFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	InfoWriter Writer;
	
	BodyData* Data = (BodyData*)Body->GetUserData();
	
	Writer.AddOption("pos")->Float(Data->Pos.x)->Float(Data->Pos.y);
	Writer.AddOption("angle")->Float(Data->Angle);
	Writer.AddOption("static")->Bool(Data->Static);
	Writer.AddOption("sprite")->String(Data->SpriteName);
	Writer.AddOption("scroll")->Bool(Data->Scroll);
	Writer.AddOption("scrollrate")->Float(Data->ScrollRate);
	
	int i=0;
	
	for(b2Fixture* Fixture=Body->GetFixtureList(); Fixture; Fixture=Fixture->GetNext())
	{
		if(((FixtureData*)Fixture->GetUserData())->Type != -1)
		{
			std::stringstream Stream;
			Stream << BodyName << "Fixture"<<i;
			
			Writer.AddOption("fixture")->String(Stream.str());
			
			SaveFixture(BodyFolder,Stream.str(),Fixture);
			
			i++;
		}
			
	}
	
	Writer.Write(BodyFolder + "/body.info");
}

void Stage::SaveFixture(std::string BaseFolder, std::string FixtureName, b2Fixture* Fixture)
{
	InfoWriter Writer;
	
	FixtureData* Data = (FixtureData*)Fixture->GetUserData();
	
	Writer.AddOption("type")->Int(Data->Type);
	
	switch (Data->Type) {
		case 0:
			Writer.AddOption("pos")->Float(Data->FirstPos.x)->Float(Data->FirstPos.y);
			Writer.AddOption("angle")->Float(Data->Angle);
			Writer.AddOption("size")->Float(Data->Width)->Float(Data->Height);
			break;
		case 1:
			Writer.AddOption("pos")->Float(Data->FirstPos.x)->Float(Data->FirstPos.y);
			Writer.AddOption("secondpos")->Float(Data->SecondPos.x)->Float(Data->SecondPos.y);
			break;
		default:
			break;
	}
	
	Writer.Write(BaseFolder + "/" + FixtureName + ".info");
}



b2Body* Stage::MakeBody(b2Vec2 Pos, float Angle, bool Static,std::string SpriteName,bool Scroll,float ScrollRate)
{
	b2BodyDef BodyDef;
	if(!Static) BodyDef.type = b2_dynamicBody;
	BodyDef.position = Pos;
	BodyDef.angle = Angle;
	
	b2Body* Body = World->CreateBody(&BodyDef);
	
	Body->SetUserData(new BodyData(Static,Pos,SpriteName,Scroll,Angle,ScrollRate));
	
	return Body;
}

b2Fixture* Stage::AddLine(b2Body* Body, b2Vec2 FirstPoint, b2Vec2 SecondPoint)
{
	b2PolygonShape NewBodyShape;
	
	b2Vec2 Center = FirstPoint+SecondPoint;
	Center *= 0.5;
	
	NewBodyShape.SetAsBox((SecondPoint-FirstPoint).Length()/2, 0.02, Center, atan2(SecondPoint.y-FirstPoint.y, SecondPoint.x-FirstPoint.x));
	
	b2FixtureDef NewBodyFixture;
	NewBodyFixture.shape = &NewBodyShape;
	NewBodyFixture.density = 1.0;
	NewBodyFixture.friction = 0.2;
	
	NewBodyFixture.restitution = 0;
	
	b2Fixture* Fixture = Body->CreateFixture(&NewBodyFixture);
	
	Fixture->SetUserData(new FixtureData(1,FirstPoint,SecondPoint));
	
	return Fixture;
}

b2Fixture* Stage::AddPlatform(b2Body* Body, float Width, float Height, b2Vec2 Pos, float Angle) {
	
	b2PolygonShape NewBodyShape;
	NewBodyShape.SetAsBox(Width/2.0, Height/2.0, Pos, Angle);
	
	b2FixtureDef NewBodyFixture;
	NewBodyFixture.shape = &NewBodyShape;
	NewBodyFixture.density = 1.0;
	NewBodyFixture.friction = 0.2;
	
	NewBodyFixture.restitution = 0.8;
	
	b2Fixture* Fixture = Body->CreateFixture(&NewBodyFixture);
	
	Fixture->SetUserData(new FixtureData(0,Pos,Angle,Width,Height));
	
	return Fixture;
}



b2Fixture* Stage::SelectFixture(float X, float Y){
	b2Vec2 Point(X,Y);
	
	b2AABB QueryArea;
	b2Vec2 Offset;
	Offset.Set(0.001f, 0.001f);
	QueryArea.lowerBound = Point - Offset;
	QueryArea.upperBound = Point + Offset;
	
	// Query the world for overlapping shapes.
	QueryCallback Callback(Point);
	World->QueryAABB(&Callback, QueryArea);
	
	return Callback.MyFixture;
}

b2Body* Stage::SelectBody(float X, float Y)
{
	b2Fixture* Fixture = SelectFixture(X,Y);
	
	if(Fixture) return Fixture->GetBody();
	else return NULL;
}

b2Fixture* Stage::SetWall(b2Fixture* Wall)
{
	delete (FixtureData*)Wall->GetUserData();
	
	Wall->SetUserData(new FixtureData(-1));
	
	return Wall;
}