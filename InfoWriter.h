#ifndef INFOWRITER_H
#define INFOWRITER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdarg>

class InfoWriter;

class WriteOption
{
public:
	WriteOption* Int(const int Number);
	WriteOption* Float(const float Number);
	WriteOption* Bool(const bool Boolean);
	WriteOption* String(const std::string String);
private:
	std::string Name;
	std::vector<std::string> Values;
	
	friend class InfoWriter;
};

class InfoWriter
{
public:
	InfoWriter(){
		
	}
	~InfoWriter(){
		
	}
	
	WriteOption* AddOption(std::string Name);
	
	void Write(std::string InfoName);
	
private:
	std::vector<WriteOption> Options;
};

#endif