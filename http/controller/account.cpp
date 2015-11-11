#include "account.h"

void RechargePage(zxy_request_context* context)
{
    const bool _isIos = is_ios(context) ? 1 : 0;

#include "../tpl/Account/rechargePage.hpp"

    return_content(context, tpl);
}

void RechargeCallback(zxy_request_context* context, void* arg)
{
    RechargeRequest* lpRequest = (RechargeRequest*)arg;

    if (lpRequest != NULL) {
        return_json(context, lpRequest->encode(false));
    }
    else {
        abort();
    }

    if (lpRequest != NULL) {
        delete lpRequest;
        lpRequest = NULL;
    }
}

void Recharge(zxy_request_context* context)
{
    std::string cardId = get_params_from_query(context, "cardId");
    std::string cardPwd = get_params_from_query(context, "cardPwd");
    RechargeRequest* lpRequest = new RechargeRequest;

    if (lpRequest != NULL) {
        g_theAccountBusiness.Recharge(context, cardId, cardPwd, lpRequest, RechargeCallback);
    }
    else {
        abort();
    }
}

void LoginPage(zxy_request_context* context)
{
    const bool _isIos = is_ios(context) ? 1 : 0;

#include "../tpl/Account/login.hpp" 

    return_content(context, tpl);
}
