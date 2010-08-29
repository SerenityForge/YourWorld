#include "Background.h"

Background::Background()
{
	Backgrounds.clear();
}

Background::~Background()
{
	for(int i=0; i<Backgrounds.size(); i++)
	{
		delete Backgrounds.at(i);
	}
}

void Background::Load(std::string BaseFolder, std::string BackgroundName)
{
	std::string BGFolder = BaseFolder + "/" + BackgroundName + ".bg";
	
	InfoReader Reader;
	Reader.Load(BGFolder + "/background.info");
	
	for(int i=0,end=Reader.GetOptionsNum(); i<end; i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "background")
		{
			AddBackground(BGFolder,GetOption.GetString(0));
		}
	}
}

void Background::AddBackground(std::string BaseFolder, std::string BackgroundTileName)
{
	std::string BackgroundTileFolder = BaseFolder + "/" + BackgroundTileName;
	
	InfoReader Reader;
	Reader.Load(BackgroundTileFolder + "/backgroundTile.info");
	
	int depth;
	float ScrollRate, X, Y;
	
	std::string SpriteName;
	
	for(int i=0,end=Reader.GetOptionsNum(); i<end; i++)
	{
		Option GetOption = Reader.GetOption(i);
		
		if(GetOption.GetName() == "depth")
		{
			depth = GetOption.GetInt(0);
		}
		else if(GetOption.GetName() == "pos")
		{
			X = GetOption.GetFloat(0);
			Y = GetOption.GetFloat(1);
		}
		else if(GetOption.GetName() == "scrollrate")
		{
			ScrollRate = GetOption.GetFloat(0);
		}
		else if(GetOption.GetName() == "image")
		{
			SpriteName = GetOption.GetString(0);
		}
	}
	Backgrounds.push_back(new BackgroundTile(depth,ScrollRate,X,Y, SpriteName));
}

void Background::Save(std::string BaseFolder, std::string BackgroundName)
{
	std::string BGFolder = BaseFolder + "/" + BackgroundName + ".bg";
	
	mkdir(BGFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	InfoWriter Writer;
	
	for(int i=0; i<Backgrounds.size(); i++)
	{
		std::stringstream Stream;
		Stream << "Background" << i;
		
		Writer.AddOption("background")->String(Stream.str());
		
		SaveBackground(BGFolder, Stream.str(), i);
	}
	
	Writer.Write(BGFolder + "/background.info");
}

void Background::SaveBackground(std::string BaseFolder, std::string BackgroundTileName, int Background)
{
	std::string BackgroundTileFolder = BaseFolder + "/" + BackgroundTileName;
	
	mkdir(BackgroundTileFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	InfoWriter Writer;
	
	BackgroundTile* Tile = Backgrounds.at(Background);
	
	Writer.AddOption("depth")->Int(Tile->Depth);
	Writer.AddOption("pos")->Float(Tile->X)->Float(Tile->Y);
	Writer.AddOption("scrollrate")->Float(Tile->ScrollRate);
	Writer.AddOption("image")->String(Tile->ImageName);
	
	Writer.Write(BackgroundTileFolder+"/backgroundTile.info");
}
