#include "InfoWriter.h"

WriteOption* WriteOption::Int(const int Number)
{
	std::stringstream Converter;
	
	Converter<<Number;
	
	std::string ToUse;
	
	Converter>>ToUse;
	
	Values.push_back(ToUse);
	
	return this;
}

WriteOption* WriteOption::Float(const float Number)
{
	std::stringstream Converter;
	
	Converter<<Number;
	
	std::string ToUse;
	
	Converter>>ToUse;
	
	Values.push_back(ToUse);
	
	return this;
}
WriteOption* WriteOption::Bool(const bool Boolean)
{
	Values.push_back((Boolean?"true":"false"));
	
	return this;
}
WriteOption* WriteOption::String(const std::string String)
{
	Values.push_back(String);
	
	return this;
}

WriteOption* InfoWriter::AddOption(std::string Name)
{
	WriteOption NewOption;
	
	NewOption.Name = Name;
	
	Options.push_back(NewOption);
	
	return &Options[Options.size()-1];
}

void InfoWriter::Write(std::string InfoName)
{
	std::fstream InfoFile;
	
	InfoFile.open(InfoName.c_str(), std::fstream::out | std::fstream::trunc);
	
	if(!InfoFile.is_open())
		return;
	
	for(int i=0; i<Options.size(); i++)
	{
		InfoFile<<Options[i].Name<<"=";
		
		for(int j=0,end=Options[i].Values.size(); j<end; j++)
		{
			InfoFile<<Options[i].Values[j];
			if(j<end-1) InfoFile<<",";
		}
		
		InfoFile<<std::endl;
	}
}
