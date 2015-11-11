#include "json_result.hpp"

json_result::json_result(int status, Json::Value data, std::string msg)
{
    this->status = status;
    this->data = data;
    this->msg = msg;
}

std::string json_result::to_json_str(bool readable)
{
    Json::Value root(Json::objectValue);
    root["status"] = this->status;
    root["msg"] = this->msg;
    root["data"] = this->data;
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
