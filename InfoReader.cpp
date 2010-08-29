#include "InfoReader.h"

int Option::GetInt(int Number) {
	std::stringstream Converter;
	
	Converter<<Values[Number];
	
	int ToReturn;
	
	Converter>>ToReturn;
	
	return ToReturn;
}

float Option::GetFloat(int Number) {
	std::stringstream Converter;
	
	Converter<<Values[Number];
	
	float ToReturn;
	
	Converter>>ToReturn;
	
	return ToReturn;
}

bool Option::GetBool(int Number) {
	if(Values[Number]=="true")
		return true;
	else
		return false;
}

InfoReader::InfoReader() {
	
}

InfoReader::~InfoReader() {
	for(int i=0; i<Options.size(); i++) {
		delete[] Options[i].Values;
	}
}

bool InfoReader::Load(std::string InfoName) {
	std::fstream InfoFile(InfoName.c_str());
	
	if(!InfoFile.is_open())
		return false;
	
	std::string LineInput;
	
	getline(InfoFile,LineInput);
	
	while(!InfoFile.eof())
	{
		if(LineInput[0] != '#' && LineInput.size() > 0){
			Option NewOption;
			
			std::vector<int> Commas;
			
			int EqualPlace = LineInput.find("=");
			NewOption.Name = LineInput.substr(0,EqualPlace);
			
			Commas.push_back(EqualPlace);
			
			int CommaPlace;
			
			do {
				int SubstrCommaPlace = LineInput.substr(Commas[Commas.size()-1]+1).find(",");
				CommaPlace = (SubstrCommaPlace<0)?std::string::npos:SubstrCommaPlace+Commas[Commas.size()-1]+1;
				Commas.push_back(CommaPlace);
				
			} while(CommaPlace != std::string::npos);
			
			NewOption.ValueAmount = Commas.size() - 1;
			NewOption.Values = new std::string[NewOption.ValueAmount];
			
			for(int i=0; i<NewOption.ValueAmount; i++) {
				NewOption.Values[i] = LineInput.substr(Commas[i]+1, Commas[i+1]-Commas[i]-1);
			}
			
			Options.push_back(NewOption);
		}
		
		getline(InfoFile,LineInput);
	}
	
	return true;
}

int InfoReader::GetOptionsNum() {
	return Options.size();
}

Option InfoReader::GetOption(int OptionNumber) {
	return Options[OptionNumber];
}