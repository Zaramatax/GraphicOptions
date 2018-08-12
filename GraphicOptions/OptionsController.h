#ifndef OPTIONS_CONTROLLER_H
#define OPTIONS_CONTROLLER_H

#include "OptionsContainer.h"
#include <list>

class OptionsController
{
public:
	OptionsController();
	~OptionsController();

	void ProcessCommand(std::vector<std::string> _command);
	void ShowErrorMessage(const char * _message) const;
private:
	std::list<OptionsContainer*> options_history;
	std::list<OptionsContainer*>::iterator current_option_iterator;
	bool binary_save_format;
};
#endif