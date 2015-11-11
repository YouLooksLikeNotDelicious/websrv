#include "SessionInfo.hpp"

void SessionInfo::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("localTime") ) {
        tmp = root["localTime"];
        if ( !tmp.isNull() )
        {
            localTime = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("sessionId") ) {
        tmp = root["sessionId"];
        if ( !tmp.isNull() )
        {
            sessionId = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("mac") ) {
        tmp = root["mac"];
        if ( !tmp.isNull() )
        {
            mac = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("ip") ) {
        tmp = root["ip"];
        if ( !tmp.isNull() )
        {
            ip = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("mpAccount") ) {
        tmp = root["mpAccount"];
        if ( !tmp.isNull() )
        {
            mpAccount = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("mpAgentId") ) {
        tmp = root["mpAgentId"];
        if ( !tmp.isNull() )
        {
            mpAgentId = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("isActive") ) {
        tmp = root["isActive"];
        if ( !tmp.isNull() )
        {
            isActive = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("userId") ) {
        tmp = root["userId"];
        if ( !tmp.isNull() )
        {
            userId = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("userName") ) {
        tmp = root["userName"];
        if ( !tmp.isNull() )
        {
            userName = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("password") ) {
        tmp = root["password"];
        if ( !tmp.isNull() )
        {
            password = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("displayName") ) {
        tmp = root["displayName"];
        if ( !tmp.isNull() )
        {
            displayName = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("balance") ) {
        tmp = root["balance"];
        if ( !tmp.isNull() )
        {
            balance = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("userPhoto") ) {
        tmp = root["userPhoto"];
        if ( !tmp.isNull() )
        {
            userPhoto = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("questionId") ) {
        tmp = root["questionId"];
        if ( !tmp.isNull() )
        {
            questionId = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("questionName") ) {
        tmp = root["questionName"];
        if ( !tmp.isNull() )
        {
            questionName = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("answer") ) {
        tmp = root["answer"];
        if ( !tmp.isNull() )
        {
            answer = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("afCoin") ) {
        tmp = root["afCoin"];
        if ( !tmp.isNull() )
        {
            afCoin = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("isPlayAd") ) {
        tmp = root["isPlayAd"];
        if ( !tmp.isNull() )
        {
            isPlayAd = tmp.asString();
        }
    }

    

    

    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    if ( root.isObject() && root.isMember("buyedList") ) {
        const Json::Value &array_buyedList = root["buyedList"];
        if ( !array_buyedList.isNull() )
        {
            int size = array_buyedList.size();
            for ( int i = 0; i < size; i++ ) {
                buyedList.push_back(array_buyedList[i].asString());
            }
        }
    }
    
    if ( root.isObject() && root.isMember("buyedCount") ) {
        const Json::Value map_buyedCount = root["buyedCount"];
        if ( !map_buyedCount.isNull() ) {
            for( Json::ValueIterator it = map_buyedCount.begin(); it != map_buyedCount.end(); ++it ) {
            
                std::string key = it.key().asString();
                buyedCount[key] = map_buyedCount[key].asInt();
            }
        }
    }   
    
    if ( root.isObject() && root.isMember("cookie") ) {
        const Json::Value map_cookie = root["cookie"];
        if ( !map_cookie.isNull() ) {
            for( Json::ValueIterator it = map_cookie.begin(); it != map_cookie.end(); ++it ) {
            
                std::string key = it.key().asString();
                cookie[key] = map_cookie[key].asString();
            }
        }
    }   
}

bool SessionInfo::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value SessionInfo::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["localTime"] = localTime;

    
    root["sessionId"] = sessionId;

    
    root["mac"] = mac;

    
    root["ip"] = ip;

    
    root["mpAccount"] = mpAccount;

    
    root["mpAgentId"] = mpAgentId;

    
    root["isActive"] = isActive;

    
    root["userId"] = userId;

    
    root["userName"] = userName;

    
    root["password"] = password;

    
    root["displayName"] = displayName;

    
    root["balance"] = balance;

    
    root["userPhoto"] = userPhoto;

    
    root["questionId"] = questionId;

    
    root["questionName"] = questionName;

    
    root["answer"] = answer;

    
    root["afCoin"] = afCoin;

    
    root["isPlayAd"] = isPlayAd;

    

    

    


    int size = 0;
    int i = 0;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    size = buyedList.size();
    root["buyedList"] = Json::Value(Json::arrayValue);
    
    for ( i = 0; i < size; i++ ) {
        root["buyedList"].append(buyedList[i]);
    }
    // map in map, how to do ?
    
    
    root["buyedCount"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, int >::iterator it = buyedCount.begin(); it != buyedCount.end(); ++it) {
        root["buyedCount"][(*it).first] = (*it).second;
    }
    
    
    root["cookie"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = cookie.begin(); it != cookie.end(); ++it) {
        root["cookie"][(*it).first] = (*it).second;
    }
    return root;
}

std::string SessionInfo::encode(bool readable)
{
    Json::Value root = encode_to_json_object();
    if ( readable )
    {
        Json::StyledWriter writer;
        return writer.write(root);
    }
    else
    {
        Json::FastWriter writer;
        return writer.write(root);
    }
}

