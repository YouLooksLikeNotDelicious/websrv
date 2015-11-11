#ifndef __ACCOUNTBUSINESS_H
#define __ACCOUNTBUSINESS_H

#include <string>
#include <vector>

#include "../base.hpp"
#include "../../libs/web.hpp"
#include "../../libs/session.hpp"

#include "Request.hpp"
#include "SessionBusiness.hpp"

class AccountBusiness
{
public:
    bool Recharge(zxy_request_context* content, const std::string&, const std::string&, RechargeRequest*, async_action_callback);
    static void RechargeCallBack(zxy_request_context*, evhtp_request_t*, const std::string&, const std::map<std::string, std::string>& cookie, zxy_upstream_request_status, void*);
};

extern AccountBusiness g_theAccountBusiness;
#endif
