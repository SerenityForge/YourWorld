#include "Editor.h"

Editor::Editor(sf::RenderWindow *AppWindow, World* MyWorld) : AppWindow(AppWindow), MyWorld(MyWorld),SelectedMenu(-1),SelectedOption(-1),SelectedBody(NULL),MyStage(NULL),FirstClick(false),IsDragging(false) { }

Editor::~Editor() { }

void Editor::SetStage(Stage* Stage)
{
	MyStage = Stage;
}

void Editor::Update()
{
	b2Vec2 MousePos = MyWorld->GetStagePoint(AppWindow->GetInput().GetMouseX(),AppWindow->GetInput().GetMouseY());
	
	b2Vec2 BodyPos;
	if(SelectedBody) BodyPos = SelectedBody->GetPosition();
	
	if(IsDragging) {
		b2Vec2 StaticMousePos = MyWorld->GetStagePoint(AppWindow->GetInput().GetMouseX(),AppWindow->GetInput().GetMouseY(),b2Vec2(0,0));
		
		MyWorld->CenterPos = MyWorld->CenterPos - StaticMousePos + GrabPoint;
		
		GrabPoint = StaticMousePos;
		
		std::cout<<MyWorld->CenterPos.x<<","<<MyWorld->CenterPos.y<<std::endl;
	}
	
	else if(SelectedMenu == Edit && SelectedBody)
	{
		switch (SelectedOption) {
			case Rotate:
				SelectedBody->SetTransform(BodyPos, atan2(MousePos.y-BodyPos.y,MousePos.x-BodyPos.x));
				break;
			case Move:
				SelectedBody->SetTransform(MousePos-Position, SelectedBody->GetAngle());
				break;
			default:
				break;
		}
	}
}

void Editor::HandleEvent(sf::Event Event)
{
	if(Event.Type == sf::Event::KeyPressed)
	{
		if(Event.Key.Code == sf::Key::Escape)
		{
			SelectedMenu = -1;
			SelectedOption = -1;
			SelectedBody = NULL;
			SelectedFixture = NULL;
		}
		else if(Event.Key.Code == sf::Key::Num1)
		{
			if(SelectedMenu == -1)
			{
				SelectedMenu = Create;
			}
			else {
				switch(SelectedMenu)
				{
					case Create:
						SelectedOption=DPlat;
						break;
					case Edit:
						SelectedOption=Rotate;
						break;
					case Destroy:
						SelectedOption=BDestroy;
						break;
					default:
						break;
				}
			}
		}
		else if(Event.Key.Code == sf::Key::Num2)
		{
			if(SelectedMenu == -1)
			{
				SelectedMenu = Edit;
			}
			else {
				switch(SelectedMenu)
				{
					case Create:
						SelectedOption=SPlat;
						break;
					case Edit:
						SelectedOption=Move;
						break;
					default:
						break;
				}
			}
		}
		else if(Event.Key.Code == sf::Key::Num3)
		{
			if(SelectedMenu == -1)
			{
				SelectedMenu = Destroy;
			}
			else {
				switch(SelectedMenu)
				{
					case Create:
						SelectedOption=SLine;
						break;
					default:
						break;
				}
			}
		}
	}
	else if(Event.Type == sf::Event::MouseButtonPressed)
	{
		if(Event.MouseButton.Button == sf::Mouse::Left)
		{
			b2Vec2 MousePos = MyWorld->GetStagePoint(Event.MouseButton.X,Event.MouseButton.Y);
			
			if(AppWindow->GetInput().IsKeyDown(sf::Key::Space))
			{
				GrabPoint = MyWorld->GetStagePoint(AppWindow->GetInput().GetMouseX(),AppWindow->GetInput().GetMouseY(),b2Vec2(0,0));
				IsDragging = true;
			}
			
			else if(SelectedOption == SPlat || SelectedOption == DPlat || SelectedOption == SLine)
			{
				if(!FirstClick)
				{
					Position = MousePos;
					FirstClick = true;
				}
				
				else
				{
					b2Vec2 SecondPos = MousePos;
					
					b2Vec2 Center = Position + SecondPos;
					Center *= 0.5;
					
					if(SelectedOption == DPlat)
					{
						b2Body* CreatedBody = MyStage->MakeBody(Center,0,false);
						
						MyStage->AddPlatform(CreatedBody,fabs(Position.x-SecondPos.x), fabs(Position.y-SecondPos.y));
					}
					
					else if(SelectedOption == SPlat)
					{
						MyStage->AddPlatform(MyStage->GetGround(),fabs(Position.x-SecondPos.x), fabs(Position.y-SecondPos.y),Center);
					}
					
					else
					{
						MyStage->AddLine(MyStage->GetGround(), Position, SecondPos);
					}
					
					SelectedMenu = -1;
					SelectedOption = -1;
					FirstClick = false;
				}
			}
			
			else if(SelectedOption == Move || SelectedOption == Rotate)
			{
				if(!FirstClick)
				{
					SelectedBody = MyStage->SelectBody(MousePos.x, MousePos.y);
					if(SelectedBody)
					{
						Position = MousePos - SelectedBody->GetPosition();
						FirstClick = true;
					}
				}
			}
			
			else if(SelectedOption == BDestroy)
			{
				SelectedBody = MyStage->SelectBody(MousePos.x,MousePos.y);
				if(SelectedBody != NULL)
				{
					std::cout<<"Found body..."<<std::endl;
					
					if(SelectedBody == MyStage->GetGround())
					{
						SelectedFixture = MyStage->SelectFixture(MousePos.x, MousePos.y);
						
						if(SelectedFixture)
						{
							delete (FixtureData*)SelectedFixture->GetUserData();
							SelectedFixture->GetBody()->DestroyFixture(SelectedFixture);
							
							SelectedBody = NULL;
							SelectedFixture = NULL;
							
							SelectedMenu = -1;
							SelectedOption = -1;
							FirstClick = false;
						}
					}
					
					else {
						for(b2Fixture* Fixture=SelectedBody->GetFixtureList(); Fixture; Fixture=Fixture->GetNext())
						{
							delete (FixtureData*)Fixture->GetUserData();
						}
						
						delete (BodyData*)SelectedBody->GetUserData();
						MyStage->GetWorld()->DestroyBody(SelectedBody);
						SelectedBody = NULL;
						
						SelectedMenu = -1;
						SelectedOption = -1;
						FirstClick = false;
					}
				}
			}
		}
	}
	
	else if(Event.Type == sf::Event::MouseButtonReleased)
	{
		b2Vec2 MousePos = MyWorld->GetStagePoint(AppWindow->GetInput().GetMouseX(),AppWindow->GetInput().GetMouseY());
		
		if(IsDragging)
		{
			IsDragging = false;
		}
		else if((SelectedOption == Move || SelectedOption == Rotate) && FirstClick)
		{
			BodyData *Data = (BodyData*)SelectedBody->GetUserData();
			
			if(SelectedOption == Move)
			{
				Data->Pos = MousePos-Position;
			}
			
			else if(SelectedOption == Rotate)
			{
				Data->Angle = atan2(MousePos.y-SelectedBody->GetPosition().y,MousePos.x-SelectedBody->GetPosition().x);
			}
			
			SelectedBody = NULL;
			
			SelectedMenu = -1;
			SelectedOption = -1;
			FirstClick = false;
		}
	}
}

void Editor::SetRotate(float Angle) {
	if(Fixture)
	{
		sf::Shape NewShape;
		
		//SelectedFixture->SetTransform(BodyPos, atan2(MousePos.y-BodyPos.y,MousePos.x-BodyPos.x));
	}
	else
	{
		b2Vec2 BodyPos = SelectedBody->GetPosition();
		
		SelectedBody->SetTransform(BodyPos, Angle);
	}
}

void FinalizeRotate(float Angle);

void SetMove(b2Vec2 Position);
void FinalizeMove(b2Vec2 Position);
