#ifndef WEBSERVCONFIG_HPP
#define WEBSERVCONFIG_HPP

#include <list>

#include "../../Sys/src/File.hpp"
#include "../../Sys/src/BufReader.hpp"
#include "../../Str/src/Str.hpp"

namespace Webserv {

class ConfigStore
{
	public:
		typedef enum { FAILED, OK, INVALID_FILE } ParseStatus;
		typedef Utils::result<ParseStatus, std::string> ParseResult;

		typedef std::list<std::string> ValueList;
		typedef Utils::result<ValueList, std::string> KeyResult;

	public:
		ConfigStore(void);
		~ConfigStore(void);

		ParseResult parse_configfile(const std::string &fname);

		KeyResult get(const std::string &key);
		KeyResult get(const Str &key);

	private:
};

class ConfigParser
{
	public:
	private:
		//BufReader<File> bufreader;
};

} // End of namespace Webserv

#endif
