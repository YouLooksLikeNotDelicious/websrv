#ifndef __ACCOUNTACTION_H
#define __ACCOUNTACTION_H

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include "Business/AccountBusiness.h"
#include "Business/SessionBusiness.hpp"
#include "../libs/web.hpp"
#include "../libs/json_result.hpp"

void RegisterPage(zxy_request_context* context);

void RegisterPageJson(zxy_request_context* context);

void GetQuestion(zxy_request_context* context);

void RegisterAccount(zxy_request_context* context);

void RechargePage(zxy_request_context* context);

void Recharge(zxy_request_context*);

void LoginPage(zxy_request_context*);

void Login(zxy_request_context*);

void Logout(zxy_request_context*);

void Signout(zxy_request_context*, void*);

void AccountPage(zxy_request_context*);

void AccountPageJson(zxy_request_context*);

void GetUserInfo(zxy_request_context*);

void ForgetFirst(zxy_request_context*);

void CheckUserName(zxy_request_context*);

void ForgetSecond(zxy_request_context*);

void ForgetSecondJson(zxy_request_context*);

void CheckAnswer(zxy_request_context*);

void ForgetThird(zxy_request_context*);

void ResetPwd(zxy_request_context*);

void ChangePasswordPage(zxy_request_context*);

void ChangePassword(zxy_request_context*);

void DestropToken(zxy_request_context*);

void CheckUser(zxy_request_context*, const std::string&, const std::string&);

#endif
