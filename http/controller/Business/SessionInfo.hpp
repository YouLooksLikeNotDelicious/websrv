#ifndef SessionInfo_INCLUDED
#define SessionInfo_INCLUDED

#include <string>
#include <vector>
#include <map>

#include <jsoncpp/json/json.h>

class SessionInfo {
public:
    std::string  localTime;
    std::string  sessionId;
    std::string  mac;
    std::string  ip;
    std::string  mpAccount;
    std::string  mpAgentId;
    std::string  isActive;
    std::string  userId;
    std::string  userName;
    std::string  password;
    std::string  displayName;
    std::string  balance;
    std::string  userPhoto;
    std::string  questionId;
    std::string  questionName;
    std::string  answer;
    std::string  afCoin;
    std::string  isPlayAd;
    std::vector < std::string >  buyedList;
    std::map < std::string, int >  buyedCount;
    std::map < std::string, std::string >  cookie;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};

#endif