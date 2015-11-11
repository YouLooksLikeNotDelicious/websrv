#ifndef SiteLanguage_INCLUDED
#define SiteLanguage_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include <jsoncpp/json/json.h>

#include "../libs/logger.hpp"

class SiteLanguagePackage {
public:
    std::map < std::string, std::string >  LanguageMap;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
public:
   bool InitializeMap(const std::string&);
    std::string get_value_by_key(const std::string& key);
};

extern SiteLanguagePackage g_theSiteLanguage;
#endif
