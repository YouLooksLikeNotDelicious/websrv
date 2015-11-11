#include "SiteLanguage.hpp"

SiteLanguagePackage g_theSiteLanguage;

void SiteLanguagePackage::decode_from_json_object(const Json::Value &root)
{
    Json::Value tmp;
    
    if ( root.isObject() && root.isMember("LanguageMap") ) {
        const Json::Value map_LanguageMap = root["LanguageMap"];
        if ( !map_LanguageMap.isNull() ) {
            for( Json::ValueIterator it = map_LanguageMap.begin(); it != map_LanguageMap.end(); ++it ) {
            
                std::string key = it.key().asString();
                LanguageMap[key] = map_LanguageMap[key].asString();
            }
        }
    }   
}

bool SiteLanguagePackage::decode(const std::string &jsonbuf)
{
    Json::Reader reader;
    Json::Value root;
    if ( !reader.parse(jsonbuf, root) ) {
        return false;
    }
    decode_from_json_object(root);
    return true;
}

Json::Value SiteLanguagePackage::encode_to_json_object()
{
    Json::Value root(Json::objectValue);

    root["LanguageMap"] = Json::Value(Json::objectValue);
    for ( std::map< std::string, std::string >::iterator it = LanguageMap.begin(); it != LanguageMap.end(); ++it) {
        root["LanguageMap"][(*it).first] = (*it).second;
    }
    return root;
}

std::string SiteLanguagePackage::encode(bool readable)
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


bool SiteLanguagePackage::InitializeMap(const std::string& file_path)
{
    bool isDec = false;

    FILE* lpFile = fopen(file_path.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int len = ftell(lpFile);

        char* lpBuff = new char[len + 1];

        if ( lpBuff != NULL )
        {
            lpBuff[len] = '\0';

            fseek(lpFile, 0, SEEK_SET);

            fread(lpBuff, sizeof(char), len, lpFile);
            
            decode(lpBuff);

            isDec = true;
        }

        if ( lpBuff != NULL )
        {
            delete[] lpBuff;

            lpBuff = NULL;
        }
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }

    return isDec;
}

std::string SiteLanguagePackage::get_value_by_key(const std::string& key)
{
    std::string ret_val = key;

    auto it = LanguageMap.find(key);

    if ( it != LanguageMap.end() )
    {
        ret_val = (*it).second;
    }

    return ret_val;
}

#if 0
int main()
{
    std::map<std::string, std::string>& theMap = g_theSiteLanguage.LanguageMap;

    theMap["Index"] = "首页"; //
    theMap["Video"] = "视频";
    theMap["Music"] = "音乐";
    theMap["Pictures"] = "图片";
    theMap["Game" ] =  "游戏";//
    theMap["App"] = "应用";
    theMap["Search" ] =  "搜索";//
    theMap["ChangePassword"] = "修改密码";
    theMap["SignOut"] = "注销";
    theMap["Balance"] = "余额:";
    theMap["Information"] = "我的信息";
    theMap["NewPassword"] = "新密码";
    theMap["Confirm"] = "确认";
    theMap["changePassWdSuccess"] = "修改密码成功!";
    theMap["oldPwdAlert"] = "请数据旧密码.";
    theMap["newPwdAlert"] = "请输入新密码.";
    theMap["newPwdLength"] = "密码长度必须大于6.";
    theMap["confirmpwdNull"] = "请输入确认密码.";
    theMap["samePassWD"] = "请确认两次密码是否一致";
    theMap["ForgetPassword"] = "忘记密码";
    theMap["Next"] = "下一步";
    theMap["CheckingUserNameFailed"] = "检查用户名失败!";
    theMap["failed"] = "失败";
    theMap["retry"] = "请重试";
    theMap["reset"] = "重置";
    theMap["userName"] = "用户名";
    theMap["PhoneNumberNotBeNull"] = "手机号码不能为空.";
    theMap["inputAnswer"] = "请输入答案";
    theMap["successSignIn"] = "修改密码成功.";
    theMap["changePassWdFaild"] = "修改密码失败.";
    theMap["signIn"] = "登录";
    theMap["signUp"] = "注册";
    theMap["rememberMe"] = "记住我";
    theMap["forgetPassword"] = "忘记密码?";
    theMap["signInFailed"] = "登录失败, 请重试";
    theMap["phoneNumberEmpty"] = "手机号码不能为空";
    theMap["passwordEmpty"] = "密码不能为空";
    theMap["recharge"] = "充值";
    theMap["rechargeSuccess"] = "充值成功!";
    theMap["rechargeFailed"] = "充值失败!";
    theMap["inputCardNumber"] = "请输入充值卡号!";
    theMap["inputCardPasswd"] = "请输入充值卡密码!";
    theMap["forRetrivePasswordOnly"] = "为了找回密码";
    theMap["equipmentFiling"] = "Pengisian perangkat";
    theMap["inputUsername"] = "请输入用户名";
    theMap["inputPassword"] = "请输入密码";
    theMap["loginFailed"] = "用户名或密码错误";
    theMap["tryAgain"] = "请重试.";
    theMap["introduction"] = "介绍";
    theMap["screentshots"] = "截图";
    theMap["download"] = "下载";
    theMap["size"] = "大小";
    theMap["version"] = "版本";
    theMap["duration"] = "时间";
    theMap["year"] = "年份";
    theMap["actors"] = "演员";
    theMap["singer"] = "歌手";
    theMap["getHDPictures"] = "获取高清图片";
    theMap["noData"] = "没有相关数据";
    theMap["previous"] = "上一页";
    theMap["next page"] = "下一页";
    theMap["unnamePage"] = "未命名的页面";
    theMap["listenning"] = "正在试听:";
    theMap["report"] = "报告";
    theMap["wifiStation"] = "Stasiun WiFi";
    theMap["selectCountry"] = "Silahkan pilih negara";
    theMap["staff"] = "staff";
    theMap["selectStation"] = "Silahkan pilih lokasi";
    theMap["View" ] =  "查看";//
    theMap["View_ALL" ] =  "查看所有的"; //
    theMap["RECORDS" ] =  "条记录";//
    theMap["HISTORY" ] =  "历史";//
    theMap["PHONE" ] =  "手机号码"; //
    theMap["CONFIRM_PASSWORD" ] =  "确认密码";
    theMap["failedTryAgain"] = "失败, 请重试:";
    theMap["load"] = "加载";
    theMap["checkAnswerFailed"] = "密码答案错误.";

    std::string cs = g_theSiteLanguage.encode(false);

    printf("%s", cs.c_str());

    return 0;
}
#endif
