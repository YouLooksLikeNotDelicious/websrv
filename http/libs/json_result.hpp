#include <jsoncpp/json/json.h>

class json_result {
private:
    int status;
    std::string msg;
    Json::Value data;

public:
    json_result(int status, Json::Value data, std::string msg);

    std::string to_json_str(bool readable);
};
