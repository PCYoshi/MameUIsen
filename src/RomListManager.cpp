//
// Created by joseph on 16/11/15.
//

#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include "RomListManager.h"

using namespace std;

//Constructors
RomListManager::RomListManager()
{
	loadCategories();
	loadRoms();
}

//Getters
RomList& RomListManager::getPreviousRomList()
{
	updateCurrentRomSetIndex('-');
	return romListsArray[currentRomSetIndex-1];
}

RomList& RomListManager::getNextRomList()
{
	updateCurrentRomSetIndex('+');
	return romListsArray[currentRomSetIndex-1];
}

int RomListManager::getCurrentRomSetIndex()
{
	return currentRomSetIndex;
}

int RomListManager::getRomSetNumber()
{
	return (int) romListsArray.size();
}

//Member Functions
void RomListManager::loadCategories()
{
	ifstream categoriesConfFile("../config/categories.cfg", ios::in);
	if(!categoriesConfFile)
	{
		cerr << "Can't open config/categories.cfg : " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		int nbLines = 0;
		string line;

		while(getline(categoriesConfFile, line))
		{
			if(line[0] != '#')
			{
				nbLines++;
			}
		}

		romListsArray.resize((unsigned long) nbLines);
		categoriesConfFile.clear();
		categoriesConfFile.seekg(0);

		while(getline(categoriesConfFile, line))
		{
			if(line[0] != '#')
			{
				size_t* pos = new size_t;
				int categoryNumber = stoi(line, pos);
				romListsArray[categoryNumber] = RomList(line.substr(*pos+1, string::npos));
				delete pos;
			}

		}
	}
}

void RomListManager::loadRoms()
{
	ifstream romsConfFile("../config/games.cfg", ios::in);
	if(!romsConfFile)
	{
		cerr << "Can't open config/games.cfg : " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		string line;
		int nbLines = 0;

		while(getline(romsConfFile, line))
		{
			if(line[0] != '#')
			{
				nbLines++;
			}
		}

		//Set final vector capacity, because we will use pointer on it's content, so we don't want it's content moved in memory.
		roms.resize((unsigned long) nbLines);
		romsConfFile.clear();
		romsConfFile.seekg(0);

		string romProp;
		while(getline(romsConfFile, line))
		{
			vector<string> romProperties;
			if(line[0] != '#')
			{
				istringstream iss(line);
				while(getline(iss, romProp, ';'))
				{
					romProperties.push_back(romProp);
				}

				if(romProperties.size() < 4)
				{
					cerr << "Syntax error in file config/games.cfg with line : " << line;
					exit(EXIT_FAILURE);
				}

				Rom rom(romProperties[0], romProperties[1], romProperties[2], romProperties[3]);
				roms.push_back(rom);
				Rom* romsPointer = &roms.back();
				for(unsigned int i=4; i<romProperties.size(); i++)
				{
					romListsArray[atoi(romProperties[i].c_str())].addRom(romsPointer);
				}
			}
		}
	}
}

void RomListManager::updateCurrentRomSetIndex(char sign)
{
	if(sign == '+')
	{
		if(currentRomSetIndex + 1 > (int) romListsArray.size())
		{
			currentRomSetIndex = 1;
		}
		else
		{
			currentRomSetIndex++;
		}
	}
	else if(sign == '-')
	{
		if(currentRomSetIndex - 1 < 1)
		{
			currentRomSetIndex = (int) romListsArray.size();
		}
		else
		{
			currentRomSetIndex--;
		}
	}
}