#pragma once
#include "Core\Parsers\INIParserClass.h"
#include "Core\Parsers\INIFile.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>

using std::string;


namespace NuclearEngine {
	namespace Core {
		namespace Parsers {
			//INIFile INIReader("RulesBE.ini");
			INIFile::INIFile(string filename)
			{
				_error = INIParser.ini_parse(filename.c_str(), ValueHandler, this);
			}

			int INIFile::ParseError() const
			{
				return _error;
			}

			string INIFile::GetString(string section, string name, string default_value) const
			{
				string key = MakeKey(section, name);
				return _values.count(key) ? _values.at(key) : default_value;
			}

			long INIFile::GetInteger(string section, string name, long default_value) const
			{
				string valstr = GetString(section, name, "");
				const char* value = valstr.c_str();
				char* end;
				// This parses "1234" (decimal) and also "0x4D2" (hex)
				long n = strtol(value, &end, 0);
				return end > value ? n : default_value;
			}

			double INIFile::GetReal(string section, string name, double default_value) const
			{
				string valstr = GetString(section, name, "");
				const char* value = valstr.c_str();
				char* end;
				double n = strtod(value, &end);
				return end > value ? n : default_value;
			}

			bool INIFile::GetBoolean(string section, string name, bool default_value) const
			{
				string valstr = GetString(section, name, "");
				// Convert to lower case to make string comparisons case-insensitive
				std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
				if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
					return true;
				else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
					return false;
				else
					return default_value;
			}

			string INIFile::MakeKey(string section, string name)
			{
				string key = section + "=" + name;
				// Convert to lower case to make section/name lookups case-insensitive
				std::transform(key.begin(), key.end(), key.begin(), ::tolower);
				return key;
			}

			int INIFile::ValueHandler(void* user, const char* section, const char* name,
				const char* value)
			{
				INIFile* reader = (INIFile*)user;
				string key = MakeKey(section, name);
				if (reader->_values[key].size() > 0)
					reader->_values[key] += "\n";
				reader->_values[key] += value;
				return 1;
			}
		}
	}
}