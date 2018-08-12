#ifndef OPTIONS_CONTROLLER_CPP
#define OPTIONS_CONTROLLER_CPP

#include "OptionsController.h"

using namespace std;

OptionsController::OptionsController()
{
	options_history.push_back(new OptionsContainer());
	current_option_iterator = options_history.begin();
	binary_save_format = false;
}

OptionsController::~OptionsController()
{
	for (auto it = options_history.begin(); it != options_history.end(); ++it)
	{
		delete *it;
	}

	options_history.clear();
}

void OptionsController::ShowErrorMessage(const char * _message) const
{
	cout << _message << endl;
}

void OptionsController::ProcessCommand(vector<string> _command)
{
	if (_command.size() < 1)
	{
		return;
	}
	
	if (_command[0] == "change")
	{
		if (_command.size() == 3)
		{
			//Copy instance of current options and try to change it.
			OptionsContainer * new_options = new OptionsContainer(**current_option_iterator); 
			if (new_options->CreateChangedOptions(_command[1].c_str(), _command[2].c_str()))
			{
				//If option name and value are correct add it to history and set as current.
				options_history.erase(++current_option_iterator, options_history.end());
				options_history.push_back(new_options);
				current_option_iterator = --options_history.end();
			}
			//If change process failed, delete instance.
			else
			{
				delete new_options;
			}
			return;
		}
	}

	if (_command[0] == "print")
	{
		if (_command.size() == 1)
		{
			(*current_option_iterator)->PrintAll();
			return;
		}
	}

	if (_command[0] == "get")
	{
		if (_command.size() == 2)
		{
			(*current_option_iterator)->Print(_command[1].c_str());
			return;
		}
	}

	if (_command[0] == "format")
	{
		if (_command.size() == 2)
		{
			if (_command[1] == "xml")
			{
				binary_save_format = false;
				return;
			}

			if (_command[1] == "bin")
			{
				binary_save_format = true;
				return;
			}
		}
	}

	if (_command[0] == "save")
	{
		if (_command.size() == 1)
		{
			(*current_option_iterator)->Save(binary_save_format);
			return;
		}
	}

	if (_command[0] == "undo")
	{
		if (current_option_iterator != options_history.begin())
		{
			--current_option_iterator;
		}
		return;
	}

	if (_command[0] == "redo")
	{
		if (current_option_iterator != --options_history.end())
		{
			++current_option_iterator;
		}
		return;
	}

	ShowErrorMessage("Unknown command format");
}

#endif