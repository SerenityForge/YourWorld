#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "World.h"

int main()
{	
	sf::RenderWindow App(sf::VideoMode(800, 600), "Your World - Version 0.1");
	
	glOrtho(-16.f, 16.f, -12.f, 12.f, 2.0f, -2.0f);
	
	App.SetFramerateLimit(60);
	
	World YourWorld(&App);
	
	YourWorld.Load("main");
	
	while (App.IsOpened())
	{
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			if(Event.Type == sf::Event::Closed) App.Close();
			else {
				YourWorld.HandleEvent(Event);
			}
		}
		
		YourWorld.Update();
		
		App.Clear();
		
		YourWorld.Draw();

		App.Display();
	}

	return EXIT_SUCCESS;
}
