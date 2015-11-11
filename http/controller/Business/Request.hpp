#ifndef Request_INCLUDED
#define Request_INCLUDED

#include <string>
#include <vector>
#include <map>

#include <jsoncpp/json/json.h>

class LoginRequestData {
public:
    std::string  userId;
    std::string  userName;
    std::string  afCoin;
    std::string  localPwd;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class LoginRequest {
public:
    LoginRequestData  data;
    std::string  errMsg;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class CheckAnswerRequest {
public:
    std::string  errMsg;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class CheckUserNameRequestQuestion {
public:
    std::string  id;
    std::string  name;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class CheckUserNameRequest {
public:
    CheckUserNameRequestQuestion  question;
    std::string  errMsg;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class QuestionRequest {
public:
    std::vector < CheckUserNameRequestQuestion >  data;
    std::string  msg;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class RechargeRequestData {
public:
    std::string  afCoin;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class RechargeRequest {
public:
    RechargeRequestData  data;
    std::string  errMsg;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyedListItem {
public:
    std::string  contentUuid;
    std::string  createTime;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class GetBuyedListRequestData {
public:
    std::map < std::string, std::string >  contentUuidMap;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyCountItem {
public:
    std::string  name;
    int  count;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyCountData {
public:
    std::map < std::string, std::string >  buyCountMap;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyedListRequestData {
public:
    std::string  total;
    std::map < std::string, std::string >  contentUuid;
    std::map < std::string, int >  buyCount;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyedListRequest {
public:
    int  status;
    BuyedListRequestData  data;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class GetBuyedListRequest {
public:
    GetBuyedListRequestData  data;
    int  status;
    int  total;
    std::string  errMsg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyRequestData {
public:
    std::map < std::string, std::string >  contentUuid;
    std::string  afCoin;
    std::string  countAdd;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BuyRequest {
public:
    BuyRequestData  data;
    int  status;
    std::string  errMsg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class DownContentCheckRequestData {
public:
    std::string  filePath;
    std::string  type;
    std::string  uuid;
    std::string  name;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class DownContentCheckRequest {
public:
    DownContentCheckRequestData  data;
    int  status;
    std::string  errMsg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class ResetPwdRequest {
public:
    int  status;
    std::string  errMsg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class AccountPageRequest {
public:
    std::string  userName;
    std::string  balance;
    int  buyedGameCount;
    int  buyedSoftCount;
    int  buyedPictureCount;
    int  buyedMusicCount;
    int  buyedVideoCount;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class GetUserInfoRequest {
public:
    AccountPageRequest  data;
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class RegisterAccountRequest {
public:
    int  status;
    std::string  msg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class ChangePwdRequest {
public:
    int  status;
    std::string  msg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class LogoutRequest {
public:
    int  status;
    std::string  errMsg;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};
class BaseResponseRequest {
public:
    int  status;
public:
    bool decode(const std::string &jsonbuf);
    void decode_from_json_object(const Json::Value &jsonobj);
    Json::Value encode_to_json_object();
    std::string encode(bool readable);
};

#endif