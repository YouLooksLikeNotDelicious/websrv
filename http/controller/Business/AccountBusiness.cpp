#include "AccountBusiness.h"
#include "../../libs/url_encode.hpp"
#include "../../libs/json_result.hpp"

AccountBusiness g_theAccountBusiness;

typedef struct _Recharge_CallbackParam
{
    async_action_callback callback;
    RechargeRequest* lpRequest;
}_RECHARGE_CALLBACKPARAM, *_LPRECHARGE_CALLBACKPARAM;

bool AccountBusiness::Recharge(zxy_request_context* context, const std::string& cardId, const std::string& cardPwd, RechargeRequest* lpRequest, async_action_callback callback)
{
    char szUri[0x200] = {0};

    char *host = context->http_server->site_config->user_host;
    char *apid = context->http_server->site_config->apid;

    std::string encode_host = host;
    std::string encode_apid = url_encode(apid);
    std::string encode_cardId = url_encode(cardId);
    std::string encode_cardPwd = url_encode(cardPwd);

    snprintf(szUri, sizeof(szUri), 
            "%s/Account/recharge?apId=%s&cardId=%s&cardPwd=%s", 
            encode_host.c_str(), encode_apid.c_str(), encode_cardId.c_str(), encode_cardPwd.c_str());

    const std::map<std::string, std::string> *lpCookie = (std::map<std::string, std::string>*)session_get(context, "cookie");

    std::map<std::string, std::string> theEmptyCookieMap;

    if (lpCookie == NULL) {
        lpCookie = &theEmptyCookieMap;
    }

    _LPRECHARGE_CALLBACKPARAM lpParam = new _RECHARGE_CALLBACKPARAM;
    if (lpParam != NULL) {
        lpParam->callback = callback;
        lpParam->lpRequest = lpRequest;

        zxy_upstream_request_uri(context, szUri, *lpCookie, RechargeCallBack, (void*)lpParam);
    }
    else {
        abort();
    }

    return true;
}

void AccountBusiness::RechargeCallBack(zxy_request_context* context, evhtp_request_t* , const std::string& content, const std::map<std::string, std::string>& , zxy_upstream_request_status status, void* arg)
{
    _LPRECHARGE_CALLBACKPARAM lpParam = (_LPRECHARGE_CALLBACKPARAM)arg;

    if (lpParam != NULL) {
        if (status == REQ_OK) {
            RechargeRequest* lpRequest = lpParam->lpRequest;
            lpRequest->decode(content);
            async_action_callback callback = lpParam->callback;
            if (lpRequest->status == 1000) {
                int afCoin = strtol(lpRequest->data.afCoin.c_str(), NULL, 10);
                g_theSessionBusiness.SetAfCoin(context, afCoin);
            }
            
            callback(context, lpRequest);
        }
        else if (status == REQ_TIMEOUT) {
            json_result result = json_result(1001,
                Json::Value(Json::nullValue), "Recharge timed out, please retry.");
            return_json(context, result.to_json_str(false));
        }
        else {
            json_result result = json_result(1001,
                Json::Value(Json::nullValue), "Network error, please retry.");
            return_json(context, result.to_json_str(false));
        }
    }

    if (lpParam != NULL) {
        delete lpParam;
        lpParam = NULL;
    }
}


