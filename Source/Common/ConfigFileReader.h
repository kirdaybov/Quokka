#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

namespace quokka
{
  struct SConfigValue
  {
    SConfigValue() {}
    SConfigValue(const char* value): _string(value) {}

    int GetInt()
    {
      return std::stoi(_string);
    }

    bool GetBool()
    {
      return std::stoi(_string) > 0;
    }

    std::string GetString()
    {
      return _string;
    }

    std::string _string;
  };

  class QConfigFileReader
  {
  public:
    bool Read(std::string filename)
    {
      std::ifstream file(filename);

      std::string line;
      std::string name;
      std::string value;
      std::string inSection;

      int posEqual;
      while (std::getline(file, line)) {

        if (!line.length()) continue;

        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        if (line[0] == '[') {
          inSection = Trim(line.substr(1, line.find(']') - 1));
          continue;
        }

        posEqual = line.find('=');
        name = Trim(line.substr(0, posEqual));
        value = Trim(line.substr(posEqual + 1));

        Values[inSection + '/' + name] = SConfigValue(value.c_str());
      }

      return true;
    }
    std::unordered_map<std::string, SConfigValue> Values;
  private:
    std::string Trim(std::string const& source, char const* delims = " \t\r\n") 
    {
      std::string result(source);
      std::string::size_type index = result.find_last_not_of(delims);
      if (index != std::string::npos)
        result.erase(++index);

      index = result.find_first_not_of(delims);
      if (index != std::string::npos)
        result.erase(0, index);
      else
        result.erase();
      return result;
    }
  };

}