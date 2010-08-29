#include "Player.h"

Player::Player()
{
	Climbing = false;
	
	U=L=R=D=Jump=false;
}

Player::~Player()
{
	
}

void Player::Save(std::string BaseFolder, std::string PlayerName)
{
	InfoWriter Writer;
	
	Writer.AddOption("pos")->Float(MyPos.x)->Float(MyPos.y);
	
	Writer.Write(BaseFolder + "/" + PlayerName + ".info");
}

void Player::Load(std::string BaseFolder, std::string PlayerName, Stage* Stage)
{
	InfoReader Reader;
	
	Reader.Load(BaseFolder + "/" + PlayerName + ".info");
	
	for(int i=0; i<Reader.GetOptionsNum(); i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "pos")
		{
			MyPos.x = GetOption.GetFloat(0);
			MyPos.y = GetOption.GetFloat(1);
		}
	}
	
	b2BodyDef DynamicBodyDef;
	DynamicBodyDef.type = b2_dynamicBody;
	DynamicBodyDef.position = MyPos;
	DynamicBodyDef.fixedRotation = true;
	
	DynamicBody = Stage->GetWorld()->CreateBody(&DynamicBodyDef);
	
	DynamicBody->SetUserData(new BodyData("player.png",false));
	
	b2BodyDef KinematicBodyDef;
	KinematicBodyDef.type = b2_kinematicBody;
	KinematicBodyDef.position.Set(0,0);
	KinematicBodyDef.fixedRotation = true;
	
	KinematicBody = Stage->GetWorld()->CreateBody(&KinematicBodyDef);
	
	KinematicBody->SetUserData(new BodyData("player.png",false));
	
	KinematicBody->SetActive(false);
	
	b2PolygonShape NormalBodyShape;
	NormalBodyShape.SetAsBox(.5, 1, b2Vec2(0,0),0);
	
	b2PolygonShape GroundTesterShape;
	GroundTesterShape.SetAsBox(0.35, 0.15, b2Vec2(0,-0.9), 0);
	
	b2PolygonShape ClimbTesterShape;
	ClimbTesterShape.SetAsBox(.7, 0.05, b2Vec2(0,.7), 0);
	
	b2PolygonShape HitTestLShape;
	HitTestLShape.SetAsBox(.1, .65, b2Vec2(-.5,0), 0);
	
	b2PolygonShape HitTestRShape;
	HitTestRShape.SetAsBox(.1, .65, b2Vec2(.5,0), 0);
	
	b2FixtureDef NewBodyFixture;
	NewBodyFixture.shape = &NormalBodyShape;
	NewBodyFixture.density = .1;
	NewBodyFixture.friction = 0.5;
	NewBodyFixture.restitution = 0.0;
	
	BodyFixture = DynamicBody->CreateFixture(&NewBodyFixture);
	
	NewBodyFixture.isSensor = true;
	
	KinematicBody->CreateFixture(&NewBodyFixture);
	
	NewBodyFixture.shape = &GroundTesterShape;
	
	GroundTester = DynamicBody->CreateFixture(&NewBodyFixture);
	KGroundTester = KinematicBody->CreateFixture(&NewBodyFixture);
	
	NewBodyFixture.shape = &ClimbTesterShape;
	
	ClimbTester = DynamicBody->CreateFixture(&NewBodyFixture);
	
	NewBodyFixture.shape = &HitTestLShape;
	
	HitTesterL = DynamicBody->CreateFixture(&NewBodyFixture);
	
	NewBodyFixture.shape = &HitTestRShape;
	
	HitTesterR = DynamicBody->CreateFixture(&NewBodyFixture);
}



void Player::HandleEvent(sf::Event Event)
{
	if(Event.Type == sf::Event::KeyPressed)
	{
		if(Event.Key.Code == sf::Key::A)
		{
			L = true;
		}
		else if(Event.Key.Code == sf::Key::D)
		{
			R = true;
		}
		else if(Event.Key.Code == sf::Key::W)
		{
			U = true;
		}
		else if(Event.Key.Code == sf::Key::S)
		{
			D = true;
		}
		else if(Event.Key.Code == sf::Key::Space)
		{
			Jump = true;
		}
	}
	else if(Event.Type == sf::Event::KeyReleased)
	{
		if(Event.Key.Code == sf::Key::A)
		{
			L = false;
		}
		else if(Event.Key.Code == sf::Key::D)
		{
			R = false;
		}
		else if(Event.Key.Code == sf::Key::W)
		{
			U = false;
		}
		else if(Event.Key.Code == sf::Key::S)
		{
			D = false;
		}
		else if(Event.Key.Code == sf::Key::Space)
		{
			Jump = false;
		}
	}
}



bool Player::isTouching(b2Fixture* Test) {
	for(b2ContactEdge* ContactEdge = DynamicBody->GetContactList(); ContactEdge; ContactEdge = ContactEdge->next)
	{
		b2Contact* Contact = ContactEdge->contact;
		
		b2Fixture* OtherFixture = (Contact->GetFixtureA()==Test)?Contact->GetFixtureB():((Contact->GetFixtureB()==Test)?Contact->GetFixtureA():NULL);
		
		if(OtherFixture && OtherFixture!=BodyFixture)
		{
			if(Contact->IsTouching())
			{
				return true;
			}
		}
	}
	
	return false;
}



void Player::Update(float Time)
{
	if(!Climbing)
	{
		if(isTouching(GroundTester))
		{
			b2Vec2 Velocity = DynamicBody->GetLinearVelocity();
			
			if(R) {
				DynamicBody->SetLinearVelocity(b2Vec2(5,Velocity.y/4.));
			}
			else if(L) {
				DynamicBody->SetLinearVelocity(b2Vec2(-5,Velocity.y/4.));
			}
			else {
				DynamicBody->SetLinearVelocity(b2Vec2(0,Velocity.y/4.));
			}
		}
		
		else {
			if(R) {
				if(DynamicBody->GetLinearVelocity().x < 6) DynamicBody->ApplyForce(b2Vec2(2,0),b2Vec2(0,0));
			}
			else if(L) {
				if(DynamicBody->GetLinearVelocity().x > -6) DynamicBody->ApplyForce(b2Vec2(-2,0),b2Vec2(0,0));
			}
		}
		
		if(Jump) {
			if((isTouching(HitTesterL) || isTouching(HitTesterR)) && !isTouching(ClimbTester))
			{
				Climbing = true;
				ClimbDirection = isTouching(HitTesterR);
				
				KinematicBody->SetTransform(DynamicBody->GetPosition(),0);
				KinematicBody->SetLinearVelocity(b2Vec2(ClimbDirection?1:-1,1.7));
				ClimbTime = 1.0;
				
				DynamicBody->SetTransform(b2Vec2(0,0), 0);
				
				KinematicBody->SetActive(true);
				DynamicBody->SetActive(false);
				
			}
			else if(isTouching(GroundTester))
				DynamicBody->ApplyForce(b2Vec2(0,40), b2Vec2(0,0));
		}
	}
	
	else {
		ClimbTime -= Time;
		
		std::cout<<isTouching(KGroundTester)<<std::endl;
		
		if(ClimbTime < 0) {
			Climbing = false;
			DynamicBody->SetTransform(KinematicBody->GetPosition(), 0);
			DynamicBody->SetLinearVelocity(b2Vec2(0,0));
			
			KinematicBody->SetTransform(b2Vec2(0,0), 0);
			
			KinematicBody->SetActive(false);
			DynamicBody->SetActive(true);
		}
	}
}