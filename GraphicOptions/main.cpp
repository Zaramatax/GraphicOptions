#ifndef MAIN_CPP
#define MAIN_CPP

#include "OptionsController.h"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> split(const string &s, char delim) 
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) 
	{
        tokens.push_back(item);
    }
    return tokens;
}

void main()
{
	string line;
	OptionsController * graphic_controller = new OptionsController();
	
	while (getline(cin, line) && line != "exit")
    {
		vector<string> tokens = split(line, ' ');
		graphic_controller->ProcessCommand(tokens);
    }

	delete graphic_controller;
}

#endif