#ifndef OPTIONS_CPP
#define OPTIONS_CPP

#include "Options.h"

#include <iostream>
#include <string>

using namespace std;

Option::Option(const char* _option_name)
{
	option_name = _option_name;
}

Option::~Option()
{
}

void Option::ShowErrorMessage() const
{
	cout << "Invalid " << option_name << " value" << endl;
}

Option * Option::GetOptionByName(const char * _option_name)
{
	if (option_name == _option_name)
		return this;

	return NULL;
}

string Option::GetOptionName()
{
	return option_name;
}

void Option::Print() const
{
	cout << option_name << " = " << string_value << endl;
}

void Option::Save(std::ofstream& file, bool _is_binary) const
{
	if (!_is_binary)
	{
		file << "<";
	}

	file << option_name;

	_is_binary ? file << " " : file << " value = \"";

	file << string_value;

	if (!_is_binary)
	{
		file << "\"/>";
	}

	file << endl;
}

bool Option::SetOption(const char * _option_name, const char * _value)
{
	if (_value == string_value)
	{
		return true;
	}

	if (CheckAndSet(_value))
	{
		string_value = _value;
		return true;
	}

	ShowErrorMessage();
	return false;
}

void Option::SetStringValue(const char * _value)
{
	string_value = _value;
}

//////////////////////////////////////////////////////////////////////////////////////

GraphicsQuality::GraphicsQuality(const char * _value) : Option("graphics_quality")
{
	vector<string> s1; s1.push_back("low"); s1.push_back("false"); s1.push_back("low");
	vector<string> s2; s2.push_back("medium"); s2.push_back("false"); s2.push_back("medium");
	vector<string> s3; s3.push_back("heigh"); s3.push_back("true"); s3.push_back("heigh");

	dependent_values.insert(make_pair(EGQ_LOW, s1));
	dependent_values.insert(make_pair(EGQ_MEDIUM, s2));
	dependent_values.insert(make_pair(EGQ_HIGH, s3));

	dependent_options.push_back(new TextureQuality());
	dependent_options.push_back(new DynamicLightning());
	dependent_options.push_back(new ShadowsQuality());

	SetOption(option_name.c_str(), _value);

	UpdateOption();
}

GraphicsQuality::GraphicsQuality(const GraphicsQuality & _right) : Option("graphics_quality")
{
	dependent_values = _right.dependent_values;

	TextureQuality* o1 = dynamic_cast<TextureQuality*>(_right.dependent_options[0]);
	dependent_options.push_back(new TextureQuality(*o1));

	DynamicLightning* o2 = dynamic_cast<DynamicLightning*>(_right.dependent_options[1]);
	dependent_options.push_back(new DynamicLightning(*o2));

	ShadowsQuality* o3 = dynamic_cast<ShadowsQuality*>(_right.dependent_options[2]);
	dependent_options.push_back(new ShadowsQuality(*o3));

	SetOption(option_name.c_str(), _right.string_value.c_str());

	UpdateOption();
}

GraphicsQuality::~GraphicsQuality()
{
	for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it)
	{
		delete *it;
	}
	dependent_options.clear();
}

void GraphicsQuality::UpdateOption()
{
	if (current_value == EGQ_CUSTOM)
	{
		return;
	}

	int i = 0;
	for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it, ++i)
	{
		string s = dependent_values[current_value][i];
		(*it)->SetOption((*it)->GetOptionName().c_str(), s.c_str());
	}
}

Option * GraphicsQuality::GetOptionByName(const char * _option_name)
{
	if (GetOptionName() == _option_name)
		return this;

	for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it)
	{
		if ((*it)->GetOptionName() == _option_name)
			return this;
	}

	return NULL;
}

bool GraphicsQuality::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "low")
	{
		current_value = EGQ_LOW;
		return true;
	}

	if (value == "medium")
	{
		current_value = EGQ_MEDIUM;
		return true;
	}

	if (value == "heigh")
	{
		current_value = EGQ_HIGH;
		return true;
	}

	if (value == "custom")
	{
		current_value = EGQ_CUSTOM;
		return true;
	}

	return false;
}

bool GraphicsQuality::SetOption(const char * _option_name, const char * _value)
{
	if (_value == string_value && _option_name == option_name)
	{
		return true;
	}

	if (_option_name == option_name)
	{
		if (CheckAndSet(_value))
		{
			string_value = _value;
			UpdateOption();
			return true;
		}
	}

	for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it)
	{
		if (_option_name == (*it)->GetOptionName() && (*it)->CheckAndSet(_value))
		{
			(*it)->SetStringValue(_value);
			SetOption(option_name.c_str(), "custom");
			return true;
		}
	}	

	ShowErrorMessage();
	return false;
}


void GraphicsQuality::Print() const
{
	Option::Print();

	for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it)
	{
		(*it)->Print();
	}
}

void GraphicsQuality::Save(std::ofstream& file, bool _is_binary) const
{
	Option::Save(file, _is_binary);

		for (auto it = dependent_options.begin(); it != dependent_options.end(); ++it)
	{
		(*it)->Save(file, _is_binary);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

Resolution::Resolution(const char * _value) : Option("resolution")
{
	SetOption(option_name.c_str(), _value);
}

bool Resolution::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "640*480" || value == "1024*768" || value == "1920*1080")
	{
		current_value = _value;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

ScreenMode::ScreenMode(const char * _value) : Option("full-screen_mode")
{
	SetOption(option_name.c_str(), _value);
}

bool ScreenMode::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "true")
	{
		current_value = true;
		return true;
	}

	if (value == "false")
	{
		current_value = false;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

Antialiasing::Antialiasing(const char * _value) : Option("antialiasing")
{
	SetOption(option_name.c_str(), _value);
}

bool Antialiasing::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "MULTISAMPLE_NONE")
	{
		current_value = EA_MULTISAMPLE_NONE;
		return true;
	}

	if (value == "MULTISAMPLE_2")
	{
		current_value = EA_MULTISAMPLE_2;
		return true;
	}

	if (value == "MULTISAMPLE_4")
	{
		current_value = EA_MULTISAMPLE_4;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

ViewDistance::ViewDistance(const char * _value) : Option("view_distance")
{
	SetOption(option_name.c_str(), _value);
}

bool ViewDistance::CheckAndSet(const char * _value)
{
	int value = atoi(_value);
	if (value >= 100 && value <= 1000)
	{
		current_value = value;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

TextureQuality::TextureQuality() : Option("texture_quality")
{
}

bool TextureQuality::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "low")
	{
		current_value = ETQ_LOW;
		return true;
	}

	if (value == "medium")
	{
		current_value = ETQ_MEDIUM;
		return true;
	}

	if (value == "heigh")
	{
		current_value = ETQ_HIGH;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

DynamicLightning::DynamicLightning() : Option("dynamic_lightning")
{
}

bool DynamicLightning::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "true")
	{
		current_value = true;
		return true;
	}

	if (value == "false")
	{
		current_value = false;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

ShadowsQuality::ShadowsQuality() : Option("shadows_quality")
{
}

bool ShadowsQuality::CheckAndSet(const char * _value)
{
	string value = _value;
	if (value == "low")
	{
		current_value = ESQ_LOW;
		return true;
	}

	if (value == "medium")
	{
		current_value = ESQ_MEDIUM;
		return true;
	}

	if (value == "heigh")
	{
		current_value = ESQ_HIGH;
		return true;
	}

	return false;
}
#endif