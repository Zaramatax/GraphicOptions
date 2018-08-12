#ifndef OPTIONS_CONTAINER_H
#define OPTIONS_CONTAINER_H

#include "Options.h"

#include <vector>
#include <iostream>
#include <fstream>

class OptionsContainer
{
public:
	OptionsContainer();
	OptionsContainer(const OptionsContainer & _options);
	~OptionsContainer();

	void PrintOption(const char * _option_name) const;
	Option * GetOption(const char * _option_name);
	bool CreateChangedOptions(const char * _option_name, const char * _value);
	void PrintAll() const;
	void Print(const char * _option_name) const;
	void Save(const bool _is_binary);
private:
	std::vector<Option*> options;
};
#endif