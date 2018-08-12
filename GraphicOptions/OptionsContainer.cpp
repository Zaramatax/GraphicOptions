#ifndef OPTIONS_CONTAINER_CPP
#define OPTIONS_CONTAINER_CPP

#include "OptionsContainer.h"

using namespace std;

OptionsContainer::OptionsContainer()
{
	options.push_back(new Resolution("640*480"));
	options.push_back(new ScreenMode("true"));
	options.push_back(new Antialiasing("MULTISAMPLE_NONE"));
	options.push_back(new ViewDistance("500"));
	options.push_back(new GraphicsQuality("medium"));
}

OptionsContainer::OptionsContainer(const OptionsContainer & _right)
{
	Resolution* o1 = dynamic_cast<Resolution*>(_right.options[0]);
	options.push_back(new Resolution(*o1));

	ScreenMode* o2 = dynamic_cast<ScreenMode*>(_right.options[1]);
	options.push_back(new ScreenMode(*o2));

	Antialiasing* o3 = dynamic_cast<Antialiasing*>(_right.options[2]);
	options.push_back(new Antialiasing(*o3));

	ViewDistance* o4 = dynamic_cast<ViewDistance*>(_right.options[3]);
	options.push_back(new ViewDistance(*o4));

	GraphicsQuality* o5 = dynamic_cast<GraphicsQuality*>(_right.options[4]);
	options.push_back(new GraphicsQuality(*o5));
}

OptionsContainer::~OptionsContainer()
{
	for (auto it = options.begin(); it != options.end(); ++it)
	{
		delete *it;
	}

	options.clear();
}

bool OptionsContainer::CreateChangedOptions(const char * _option_name, const char * _value)
{
	Option * o = NULL;

	//Get option (if option is dependent return parent option)
	for (auto it = options.begin(); it != options.end(); ++it)
	{
		o = (*it)->GetOptionByName(_option_name);
		if (o)
		{
			break;
		}
	}

	if (!o)
	{
		std::cout << "Option " << _option_name << " not found" << std::endl;
	}
	else if (o->SetOption(_option_name, _value))
	{
		return true;
	}

	delete o;

	return false;
}

void OptionsContainer::PrintAll() const
{
	for (auto it = options.begin(); it != options.end(); ++it)
	{
		(*it)->Print();
	}
}

void OptionsContainer::Print(const char * _option_name) const
{
	for (auto it = options.begin(); it != options.end(); ++it)
	{
		if (_option_name == (*it)->GetOptionName())
		{
			(*it)->Print();
			return;
		}
	}

	cout << "Option " << _option_name << " not found" << endl;
}

void OptionsContainer::Save(const bool _is_binary)
{
	string file_name = "save";
	ofstream save_file;

	try
	{
		if (_is_binary)
		{
			file_name += ".dat";
			save_file.open(file_name, ios::out | ios::binary | ios::app);
		}
		else
		{
			file_name += ".xml";
			save_file.open(file_name);

			save_file << "<?xml version=\"1.1\" encoding=\"UTF-8\" ?>" << endl;
			save_file << "<options>" << endl;
		}

		for (auto it = options.begin(); it != options.end(); ++it)
		{
			(*it)->Save(save_file, _is_binary);
		}

		if (!_is_binary)
		{
			save_file << "</options>" << endl;
		}

		save_file.close();
	}
	catch(...) 
	{
		cout << "Unable to save options";
		throw;
	}
}
#endif