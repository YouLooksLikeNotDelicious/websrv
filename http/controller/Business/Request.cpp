#include "Request.hpp"

void LoginRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
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

    
    if ( root.isObject() && root.isMember("afCoin") ) {
        tmp = root["afCoin"];
        if ( !tmp.isNull() )
        {
            afCoin = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("localPwd") ) {
        tmp = root["localPwd"];
        if ( !tmp.isNull() )
        {
            localPwd = tmp.asString();
        }
    }

    
    
    
    
}

bool LoginRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value LoginRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["userId"] = userId;

    
    root["userName"] = userName;

    
    root["afCoin"] = afCoin;

    
    root["localPwd"] = localPwd;


    int size = 0;
    int i = 0;
    
    
    
    
    return root;
}

std::string LoginRequestData::encode(bool readable)
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


void LoginRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
    
}

bool LoginRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value LoginRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["errMsg"] = errMsg;

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    
    
    return root;
}

std::string LoginRequest::encode(bool readable)
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


void CheckAnswerRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
}

bool CheckAnswerRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value CheckAnswerRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["errMsg"] = errMsg;

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string CheckAnswerRequest::encode(bool readable)
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


void CheckUserNameRequestQuestion::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("id") ) {
        tmp = root["id"];
        if ( !tmp.isNull() )
        {
            id = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("name") ) {
        tmp = root["name"];
        if ( !tmp.isNull() )
        {
            name = tmp.asString();
        }
    }

    
    
}

bool CheckUserNameRequestQuestion::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value CheckUserNameRequestQuestion::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["id"] = id;

    
    root["name"] = name;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string CheckUserNameRequestQuestion::encode(bool readable)
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


void CheckUserNameRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("question") ) {
        tmp = root["question"];
        if ( !tmp.isNull() )
        {
            question.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
    
}

bool CheckUserNameRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value CheckUserNameRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["question"] = question.encode_to_json_object();

    
    root["errMsg"] = errMsg;

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    
    
    return root;
}

std::string CheckUserNameRequest::encode(bool readable)
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


void QuestionRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    

    
    if ( root.isObject() && root.isMember("msg") ) {
        tmp = root["msg"];
        if ( !tmp.isNull() )
        {
            msg = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
    if ( root.isObject() && root.isMember("data") ) {
        const Json::Value &array_data = root["data"];
        if ( !array_data.isNull() )
        {
            int size = array_data.size();
            for ( int i = 0; i < size; i++ ) {
                CheckUserNameRequestQuestion item;
                item.decode_from_json_object(array_data[i]);
                data.push_back(item);
            }
        }
    }
    
    
}

bool QuestionRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value QuestionRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    

    
    root["msg"] = msg;

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    size = data.size();
    root["data"] = Json::Value(Json::arrayValue);
    
    for ( i = 0; i < size; i++ ) {
        root["data"].append(data[i].encode_to_json_object());
    }
    // map in map, how to do ?
    
    
    return root;
}

std::string QuestionRequest::encode(bool readable)
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


void RechargeRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("afCoin") ) {
        tmp = root["afCoin"];
        if ( !tmp.isNull() )
        {
            afCoin = tmp.asString();
        }
    }

    
}

bool RechargeRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value RechargeRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["afCoin"] = afCoin;


    int size = 0;
    int i = 0;
    
    return root;
}

std::string RechargeRequestData::encode(bool readable)
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


void RechargeRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
    
}

bool RechargeRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value RechargeRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["errMsg"] = errMsg;

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    
    
    return root;
}

std::string RechargeRequest::encode(bool readable)
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


void BuyedListItem::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("contentUuid") ) {
        tmp = root["contentUuid"];
        if ( !tmp.isNull() )
        {
            contentUuid = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("createTime") ) {
        tmp = root["createTime"];
        if ( !tmp.isNull() )
        {
            createTime = tmp.asString();
        }
    }

    
    
}

bool BuyedListItem::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyedListItem::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["contentUuid"] = contentUuid;

    
    root["createTime"] = createTime;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string BuyedListItem::encode(bool readable)
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


void GetBuyedListRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    

    
    if ( root.isObject() && root.isMember("contentUuidMap") ) {
        const Json::Value map_contentUuidMap = root["contentUuidMap"];
        if ( !map_contentUuidMap.isNull() ) {
            for( Json::ValueIterator it = map_contentUuidMap.begin(); it != map_contentUuidMap.end(); ++it ) {
            
                std::string key = it.key().asString();
                contentUuidMap[key] = map_contentUuidMap[key].asString();
            }
        }
    }   
}

bool GetBuyedListRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value GetBuyedListRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    


    int size = 0;
    int i = 0;
    
    
    root["contentUuidMap"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = contentUuidMap.begin(); it != contentUuidMap.end(); ++it) {
        root["contentUuidMap"][(*it).first] = (*it).second;
    }
    return root;
}

std::string GetBuyedListRequestData::encode(bool readable)
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


void BuyCountItem::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("name") ) {
        tmp = root["name"];
        if ( !tmp.isNull() )
        {
            name = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("count") ) {
        tmp = root["count"];
        if ( !tmp.isNull() )
        {
            count = tmp.asInt();
        }
    }

    
    
}

bool BuyCountItem::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyCountItem::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["name"] = name;

    
    root["count"] = count;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string BuyCountItem::encode(bool readable)
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


void BuyCountData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    

    
    if ( root.isObject() && root.isMember("buyCountMap") ) {
        const Json::Value map_buyCountMap = root["buyCountMap"];
        if ( !map_buyCountMap.isNull() ) {
            for( Json::ValueIterator it = map_buyCountMap.begin(); it != map_buyCountMap.end(); ++it ) {
            
                std::string key = it.key().asString();
                buyCountMap[key] = map_buyCountMap[key].asString();
            }
        }
    }   
}

bool BuyCountData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyCountData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    


    int size = 0;
    int i = 0;
    
    
    root["buyCountMap"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = buyCountMap.begin(); it != buyCountMap.end(); ++it) {
        root["buyCountMap"][(*it).first] = (*it).second;
    }
    return root;
}

std::string BuyCountData::encode(bool readable)
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


void BuyedListRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("total") ) {
        tmp = root["total"];
        if ( !tmp.isNull() )
        {
            total = tmp.asString();
        }
    }

    

    

    
    
    if ( root.isObject() && root.isMember("contentUuid") ) {
        const Json::Value map_contentUuid = root["contentUuid"];
        if ( !map_contentUuid.isNull() ) {
            for( Json::ValueIterator it = map_contentUuid.begin(); it != map_contentUuid.end(); ++it ) {
            
                std::string key = it.key().asString();
                contentUuid[key] = map_contentUuid[key].asString();
            }
        }
    }   
    
    if ( root.isObject() && root.isMember("buyCount") ) {
        const Json::Value map_buyCount = root["buyCount"];
        if ( !map_buyCount.isNull() ) {
            for( Json::ValueIterator it = map_buyCount.begin(); it != map_buyCount.end(); ++it ) {
            
                std::string key = it.key().asString();
                buyCount[key] = map_buyCount[key].asInt();
            }
        }
    }   
}

bool BuyedListRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyedListRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["total"] = total;

    

    


    int size = 0;
    int i = 0;
    
    
    
    root["contentUuid"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = contentUuid.begin(); it != contentUuid.end(); ++it) {
        root["contentUuid"][(*it).first] = (*it).second;
    }
    
    
    root["buyCount"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, int >::iterator it = buyCount.begin(); it != buyCount.end(); ++it) {
        root["buyCount"][(*it).first] = (*it).second;
    }
    return root;
}

std::string BuyedListRequestData::encode(bool readable)
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


void BuyedListRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    
}

bool BuyedListRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyedListRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;

    
    root["data"] = data.encode_to_json_object();


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string BuyedListRequest::encode(bool readable)
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


void GetBuyedListRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("total") ) {
        tmp = root["total"];
        if ( !tmp.isNull() )
        {
            total = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    
    
    
}

bool GetBuyedListRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value GetBuyedListRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["status"] = status;

    
    root["total"] = total;

    
    root["errMsg"] = errMsg;


    int size = 0;
    int i = 0;
    
    
    
    
    return root;
}

std::string GetBuyedListRequest::encode(bool readable)
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


void BuyRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    

    
    if ( root.isObject() && root.isMember("afCoin") ) {
        tmp = root["afCoin"];
        if ( !tmp.isNull() )
        {
            afCoin = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("countAdd") ) {
        tmp = root["countAdd"];
        if ( !tmp.isNull() )
        {
            countAdd = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("contentUuid") ) {
        const Json::Value map_contentUuid = root["contentUuid"];
        if ( !map_contentUuid.isNull() ) {
            for( Json::ValueIterator it = map_contentUuid.begin(); it != map_contentUuid.end(); ++it ) {
            
                std::string key = it.key().asString();
                contentUuid[key] = map_contentUuid[key].asString();
            }
        }
    }   
    
    
}

bool BuyRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    

    
    root["afCoin"] = afCoin;

    
    root["countAdd"] = countAdd;


    int size = 0;
    int i = 0;
    
    
    root["contentUuid"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = contentUuid.begin(); it != contentUuid.end(); ++it) {
        root["contentUuid"][(*it).first] = (*it).second;
    }
    
    
    return root;
}

std::string BuyRequestData::encode(bool readable)
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


void BuyRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    
    
}

bool BuyRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BuyRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["status"] = status;

    
    root["errMsg"] = errMsg;


    int size = 0;
    int i = 0;
    
    
    
    return root;
}

std::string BuyRequest::encode(bool readable)
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


void DownContentCheckRequestData::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("filePath") ) {
        tmp = root["filePath"];
        if ( !tmp.isNull() )
        {
            filePath = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("type") ) {
        tmp = root["type"];
        if ( !tmp.isNull() )
        {
            type = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("uuid") ) {
        tmp = root["uuid"];
        if ( !tmp.isNull() )
        {
            uuid = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("name") ) {
        tmp = root["name"];
        if ( !tmp.isNull() )
        {
            name = tmp.asString();
        }
    }

    
    
    
    
}

bool DownContentCheckRequestData::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value DownContentCheckRequestData::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["filePath"] = filePath;

    
    root["type"] = type;

    
    root["uuid"] = uuid;

    
    root["name"] = name;


    int size = 0;
    int i = 0;
    
    
    
    
    return root;
}

std::string DownContentCheckRequestData::encode(bool readable)
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


void DownContentCheckRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    
    
}

bool DownContentCheckRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value DownContentCheckRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["status"] = status;

    
    root["errMsg"] = errMsg;


    int size = 0;
    int i = 0;
    
    
    
    return root;
}

std::string DownContentCheckRequest::encode(bool readable)
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


void ResetPwdRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    
}

bool ResetPwdRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value ResetPwdRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;

    
    root["errMsg"] = errMsg;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string ResetPwdRequest::encode(bool readable)
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


void AccountPageRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("userName") ) {
        tmp = root["userName"];
        if ( !tmp.isNull() )
        {
            userName = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("balance") ) {
        tmp = root["balance"];
        if ( !tmp.isNull() )
        {
            balance = tmp.asString();
        }
    }

    
    if ( root.isObject() && root.isMember("buyedGameCount") ) {
        tmp = root["buyedGameCount"];
        if ( !tmp.isNull() )
        {
            buyedGameCount = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("buyedSoftCount") ) {
        tmp = root["buyedSoftCount"];
        if ( !tmp.isNull() )
        {
            buyedSoftCount = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("buyedPictureCount") ) {
        tmp = root["buyedPictureCount"];
        if ( !tmp.isNull() )
        {
            buyedPictureCount = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("buyedMusicCount") ) {
        tmp = root["buyedMusicCount"];
        if ( !tmp.isNull() )
        {
            buyedMusicCount = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("buyedVideoCount") ) {
        tmp = root["buyedVideoCount"];
        if ( !tmp.isNull() )
        {
            buyedVideoCount = tmp.asInt();
        }
    }

    
    
    
    
    
    
    
}

bool AccountPageRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value AccountPageRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["userName"] = userName;

    
    root["balance"] = balance;

    
    root["buyedGameCount"] = buyedGameCount;

    
    root["buyedSoftCount"] = buyedSoftCount;

    
    root["buyedPictureCount"] = buyedPictureCount;

    
    root["buyedMusicCount"] = buyedMusicCount;

    
    root["buyedVideoCount"] = buyedVideoCount;


    int size = 0;
    int i = 0;
    
    
    
    
    
    
    
    return root;
}

std::string AccountPageRequest::encode(bool readable)
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


void GetUserInfoRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("data") ) {
        tmp = root["data"];
        if ( !tmp.isNull() )
        {
            data.decode_from_json_object(tmp);
        }
    }

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    
}

bool GetUserInfoRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value GetUserInfoRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["data"] = data.encode_to_json_object();

    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string GetUserInfoRequest::encode(bool readable)
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


void RegisterAccountRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("msg") ) {
        tmp = root["msg"];
        if ( !tmp.isNull() )
        {
            msg = tmp.asString();
        }
    }

    
    
}

bool RegisterAccountRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value RegisterAccountRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;

    
    root["msg"] = msg;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string RegisterAccountRequest::encode(bool readable)
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


void ChangePwdRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("msg") ) {
        tmp = root["msg"];
        if ( !tmp.isNull() )
        {
            msg = tmp.asString();
        }
    }

    
    
}

bool ChangePwdRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value ChangePwdRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;

    
    root["msg"] = msg;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string ChangePwdRequest::encode(bool readable)
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


void LogoutRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
    if ( root.isObject() && root.isMember("errMsg") ) {
        tmp = root["errMsg"];
        if ( !tmp.isNull() )
        {
            errMsg = tmp.asString();
        }
    }

    
    
}

bool LogoutRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value LogoutRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;

    
    root["errMsg"] = errMsg;


    int size = 0;
    int i = 0;
    
    
    return root;
}

std::string LogoutRequest::encode(bool readable)
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


void BaseResponseRequest::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;

    
    if ( root.isObject() && root.isMember("status") ) {
        tmp = root["status"];
        if ( !tmp.isNull() )
        {
            status = tmp.asInt();
        }
    }

    
}

bool BaseResponseRequest::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value BaseResponseRequest::encode_to_json_object()
{
    Json::Value root(Json::objectValue);


    
    root["status"] = status;


    int size = 0;
    int i = 0;
    
    return root;
}

std::string BaseResponseRequest::encode(bool readable)
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

