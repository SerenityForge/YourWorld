#ifndef INFOREADER_H
#define INFOREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class InfoReader;

class Option
{
public:
	std::string GetName() { return Name; }
	int GetValueAmount() { return ValueAmount; }
	
	std::string GetString(int Number) { return Values[Number]; }
	bool GetBool(int Number);
	float GetFloat(int Number);
	int GetInt(int Number);
private:
	std::string Name;
	
	int ValueAmount;
	std::string *Values;
	
	friend class InfoReader;
};

class InfoReader
{
public:
	InfoReader();
	~InfoReader();
	
	bool Load(std::string InfoFile);
	
	int GetOptionsNum();
	Option GetOption(int OptionNumber);
private:
	std::vector<Option> Options;
};

#endif