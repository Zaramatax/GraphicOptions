#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <fstream>
#include <vector>
#include <map>

enum EAntialiasing
{
	EA_MULTISAMPLE_NONE, EA_MULTISAMPLE_2, EA_MULTISAMPLE_4,
};

enum ETextureQuality
{
	ETQ_LOW, ETQ_MEDIUM, ETQ_HIGH,
};

enum EShadowsQuality
{
	ESQ_LOW, ESQ_MEDIUM, ESQ_HIGH,
};

enum EGraphicsQuality
{
	EGQ_LOW, EGQ_MEDIUM, EGQ_HIGH, EGQ_CUSTOM,
};

class Option
{
protected:
	std::string option_name;
	std::string string_value;
public:
	Option(const char* _option_name);
	virtual ~Option();

	virtual bool SetOption(const char * _option_name, const char * _value);
	virtual void ShowErrorMessage() const;
	virtual Option * GetOptionByName(const char * _option_name);
	virtual void Print() const;
	virtual void Save(std::ofstream& file, bool _is_binary) const;
	std::string GetOptionName();
	virtual bool CheckAndSet(const char * _value) = 0;
	virtual void SetStringValue(const char * _value);
};

//////////////////////////////////////////////////////////////////////////////////////

class GraphicsQuality : public Option
{
public:
	GraphicsQuality(const char * _value);
	GraphicsQuality(const GraphicsQuality & _value);
	~GraphicsQuality();

	bool SetOption(const char * _option_name, const char * _value);
	bool CheckAndSet(const char * _value);
	void UpdateOption();
	Option * GetOptionByName(const char * _option_name);
	void Print() const;
	void Save(std::ofstream& file, bool _is_binary) const;
private:
	EGraphicsQuality current_value;
	std::vector<Option*> dependent_options;
	std::map<EGraphicsQuality, std::vector<std::string>> dependent_values;
};

//////////////////////////////////////////////////////////////////////////////////////

class Resolution : public Option
{
public:
	Resolution(const char * _value);

	bool CheckAndSet(const char * _value);
private:
	std::string current_value;
};

//////////////////////////////////////////////////////////////////////////////////////

class ScreenMode : public Option
{
public:
	ScreenMode(const char * _value);

	bool CheckAndSet(const char * _value);
private:
	bool current_value;
};

//////////////////////////////////////////////////////////////////////////////////////

class Antialiasing : public Option
{
public:
	Antialiasing(const char * _value);

	bool CheckAndSet(const char * _value);
private:
	EAntialiasing current_value;
};

//////////////////////////////////////////////////////////////////////////////////////

class ViewDistance : public Option
{
public:
	ViewDistance(const char * _value);

	bool CheckAndSet(const char * _value);
private:
	int current_value;
};

//////////////////////////////////////////////////////////////////////////////////////

class TextureQuality : public Option
{
public:
	TextureQuality();

	bool CheckAndSet(const char * _value);
private:
	ETextureQuality current_value;
};

//////////////////////////////////////////////////////////////////////////////////////

class DynamicLightning : public Option
{
public:
	DynamicLightning();

	bool CheckAndSet(const char * _value);
private:
	bool current_value;
};

class ShadowsQuality : public Option
{
public:
	ShadowsQuality();

	bool CheckAndSet(const char * _value);
private:
	EShadowsQuality current_value;
};
#endif