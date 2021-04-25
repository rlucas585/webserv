#include "WebservConfig.hpp"

namespace Webserv {

ConfigStore::ConfigStore(void) { }
ConfigStore::~ConfigStore(void) { }

ConfigStore::ParseResult ConfigStore::parse_configfile(const std::string &fname)
{
	return (ParseResult::Ok(OK));
	(void)fname;
}

} // End of namespace Webserv
