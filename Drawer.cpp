#include "Drawer.h"

Drawer::~Drawer()
{
	
}

void Drawer::Draw(Stage *Stage, bool Outline)
{
	b2World *World = Stage->GetWorld();
	
	for(b2Body *Body = World->GetBodyList(); Body; Body=Body->GetNext())
	{
		if(Body->IsActive())
		{
			if(!Outline)
			{
				BodyData* Data = (BodyData*)Body->GetUserData();
				
				sf::Vector2f BodyPos = MyWorld->GetScreenPoint(Body->GetPosition());
				sf::Vector2f Size = sf::Vector2f(Data->Sprite.GetSize().x/2,Data->Sprite.GetSize().y/2);
				
				
				
				Data->Sprite.SetPosition(BodyPos-Size);
				Data->Sprite.SetRotation(Body->GetAngle()*180/3.14159);
				
				AppWindow->Draw(Data->Sprite);
			}
				
			else {
				for(b2Fixture *Fixture = Body->GetFixtureList(); Fixture; Fixture=Fixture->GetNext())
				{
					if(Fixture->GetType()==b2Shape::e_polygon)
					{
						b2PolygonShape *Shape = (b2PolygonShape*)Fixture->GetShape();
						
						sf::Shape DrawShape;
						
						if(Shape->m_vertexCount > 2) {
							for(int i=0; i<Shape->m_vertexCount; i++)
							{
								DrawShape.AddPoint(Shape->m_vertices[i].x*25,Shape->m_vertices[i].y*-25, sf::Color::White, sf::Color::White);
							}
						}
						
						else
						{
							DrawShape = sf::Shape::Line(Shape->m_vertices[0].x*25,Shape->m_vertices[0].y*-25,Shape->m_vertices[1].x*25,Shape->m_vertices[1].y*-25,0.01,sf::Color::White,0.01,sf::Color::White);
						}
						
						DrawShape.SetPosition(MyWorld->GetScreenPoint(Body->GetPosition()));
						
						DrawShape.SetRotation(Body->GetAngle()*180/3.14159);
						
						DrawShape.EnableFill(false);
						DrawShape.EnableOutline(true);
						DrawShape.SetOutlineWidth(1);
						
						AppWindow->Draw(DrawShape);
					}
					else if(Fixture->GetType()==b2Shape::e_circle)
					{
						b2CircleShape *Shape = (b2CircleShape*)Fixture->GetShape();
						
						sf::Shape DrawShape = sf::Shape::Circle(0,0,Shape->m_radius,sf::Color::White,1,sf::Color::White);
						
						DrawShape.SetPosition(MyWorld->GetScreenPoint(Body->GetPosition()));
						
						DrawShape.SetRotation(Body->GetAngle());
						
						DrawShape.EnableFill(false);
						DrawShape.EnableOutline(true);
						DrawShape.SetOutlineWidth(1);
						
						AppWindow->Draw(DrawShape);
					}
				}
			}
		}
	}
}

void Drawer::Draw(Editor* Editor)
{
	DrawText(10, 10,  "1:Create", (Editor->GetSelectedMenu()==0));
	
	if(Editor->GetSelectedMenu()==0)
	{
		DrawText(10, 35, "1:Box (Dynamic)", (Editor->GetSelectedOption()==0));
		DrawText(10, 60, "2:Box (Static)", (Editor->GetSelectedOption()==1));
		DrawText(10, 85, "3:Line (Static)", (Editor->GetSelectedOption()==2));
	}
	
	DrawText(100, 10, "2:Edit",   (Editor->GetSelectedMenu()==1));
	
	if(Editor->GetSelectedMenu()==1)
	{
		DrawText(100, 35, "1:Rotate", (Editor->GetSelectedOption()==3));
		DrawText(100, 60, "2:Move", (Editor->GetSelectedOption()==4));
	}
	
	DrawText(190, 10, "3:Destroy",(Editor->GetSelectedMenu()==2));
	
	if(Editor->GetSelectedMenu()==2)
	{
		DrawText(190, 35, "1:Destroy", (Editor->GetSelectedOption()==5));
	}
}

void Drawer::DrawText(float X, float Y, std::string Text, bool Selected)
{
	sf::String DrawString;
	
	DrawString.SetText(Text);
	DrawString.SetSize(15);
	DrawString.SetPosition(X,Y);
	
	sf::FloatRect Rect = DrawString.GetRect();
	
	sf::Shape Box = sf::Shape::Rectangle(Rect.Left-1,Rect.Top+1,Rect.Right+1,Rect.Bottom+2,sf::Color::Black,1,(Selected?sf::Color::White:sf::Color::Black));
	
	AppWindow->Draw(Box);
	AppWindow->Draw(DrawString);
}

bool BackgroundSort(BackgroundTile* Tile1, BackgroundTile* Tile2)
{
	return Tile1->Depth > Tile2->Depth;
}

void Drawer::DrawBackground(Background *Background)
{
	std::vector<BackgroundTile*> *Backgrounds = Background->GetBackgrounds();
	
	std::vector<BackgroundTile*> OrderedBackground; 
	
	for(int i=0; i<Backgrounds->size(); i++)
	{
		if(Backgrounds->at(i)->Depth >= 0)
		{
			OrderedBackground.push_back(Backgrounds->at(i));
		}
	}
	
	std::sort(OrderedBackground.begin(),OrderedBackground.end(),BackgroundSort);
	
	for(int i=0; i<OrderedBackground.size(); i++)
	{
		DrawBGTile(OrderedBackground.at(i));
	}
}

void Drawer::DrawForeground(Background *Background)
{
	std::vector<BackgroundTile*> *Backgrounds = Background->GetBackgrounds();
	
	std::vector<BackgroundTile*> OrderedBackground;
	
	for(int i=0; i<Backgrounds->size(); i++)
	{
		if(Backgrounds->at(i)->Depth < 0)
		{
			OrderedBackground.push_back(Backgrounds->at(i));
		}
	}
	
	std::sort(OrderedBackground.begin(),OrderedBackground.end(),BackgroundSort);
	
	for(int i=0; i<OrderedBackground.size(); i++)
	{
		DrawBGTile(OrderedBackground.at(i));
	}
}

void Drawer::DrawBGTile(BackgroundTile *Tile)
{
	sf::Vector2f CenterPos(MyWorld->GetCenter().x*25,MyWorld->GetCenter().y*25);
	
	sf::Vector2f Offset(CenterPos.x * Tile->ScrollRate, CenterPos.y * Tile->ScrollRate);
	
	Tile->Sprite.SetPosition(sf::Vector2f(Tile->X-Offset.x,Tile->Y+Offset.y));
	
	AppWindow->Draw(Tile->Sprite);
}
