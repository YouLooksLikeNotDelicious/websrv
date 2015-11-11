#include "SessionBusiness.hpp"
#include "../../libs/logger.hpp"

SessionBusiness g_theSessionBusiness;
int g_nDisTimeOut;
extern std::string g_csGateway;

char* SessionBusiness::SafeCopyNewBuffer(const std::string& cs)
{
    int nLen = cs.length() + 1;

    char* lpBuff = new char[nLen];

    memcpy(lpBuff, cs.c_str(), nLen - 1);

    lpBuff[nLen - 1] = '\0';

    return lpBuff;
}

void SessionBusiness::SafeReleaseBuffer(void* lp)
{
    if ( lp != NULL )    
    {
        char* lpDel = (char*)lp;

        delete[] lpDel;

        lpDel = NULL;
    }
}

std::map<std::string, bool>* SessionBusiness::SafeCopySBMapFromVec(const std::vector<std::string>& theVec)
{
    std::map<std::string, bool>* lpMap = new std::map<std::string, bool>;

    if ( lpMap != NULL )
    {
        int nCount = theVec.size();

        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            (*lpMap)[theVec[nIndex]] = true;
        }
    }

    return lpMap;
}

std::map<std::string, bool>* SessionBusiness::SafeCopyNewSBMap(const std::map<std::string, bool>& theSrcMap)
{
    std::map<std::string, bool>* lpMap = NULL;
    lpMap = new std::map<std::string, bool>;

    auto it = theSrcMap.begin();

    for (; it != theSrcMap.end(); ++it)
    {
        (*lpMap)[(*it).first] = (*it).second;
        
        // (*lpMap)[(*it).first] = true; ?????????????????? 
    }

    return lpMap;
}

std::map<std::string, bool>* SessionBusiness::SafeAllocNewSBMap()
{
    std::map<std::string, bool>* lpMap = NULL;

    lpMap = new std::map<std::string, bool>;

    return lpMap;
}

void SessionBusiness::SafeReleaseSBMap(void* lp)
{
    std::map<std::string, bool>* lpMap;

    if ( lp != NULL )
    {
        lpMap = (std::map<std::string, bool>*)lp;

        delete lpMap;

        lpMap = NULL;
    }
}

int* SessionBusiness::SafeCopyNewInteger(int nSrc)
{
    int* lpRet = NULL;

    lpRet = new int;

    *lpRet = nSrc;

    return lpRet;
}

void SessionBusiness::SafeReleaseInteger(void* lp)
{
    if ( lp != NULL )
    {
        int* lpDel = (int*)lp;

        delete lpDel;

        lpDel = NULL;
    }
}

bool SessionBusiness::SetUserName(zxy_request_context* context, const std::string& userName)
{
    bool isSet = false;

    char* lpUserName = SafeCopyNewBuffer(userName);
    
    if ( lpUserName != NULL )
    {
        session_set(context, "userName", (void*)lpUserName, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "userName", (void*)userName.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetBuyList(zxy_request_context* context, const std::string& uuid)
{
    bool isSet = false;

    std::map<std::string, bool>* lpBuyedMap = (std::map<std::string, bool>*)session_get(context, "buyedList");

    if ( lpBuyedMap != NULL )
    {
        auto itFind = (*lpBuyedMap).find(uuid);

        if ( itFind == (*lpBuyedMap).end() )
        {
            (*lpBuyedMap)[uuid] = true;

            isSet = true;

            SessionSerial(context, "buyedListAdd", (void*)uuid.c_str());
        }
    }
    else
    {
        std::map<std::string, bool>* lpMap = SafeAllocNewSBMap();

        (*lpMap)[uuid] = true;

        session_set(context, "buyedList", lpMap, SessionBusiness::SafeReleaseSBMap);

        SessionSerial(context, "buyedListAdd", (char*)uuid.c_str());

        isSet = true;
    }

    return isSet;
}


bool SessionBusiness::SetBuyList(zxy_request_context* context, const std::map<std::string, std::string>& theMap)
{
    bool isSet = false;

    if ( theMap.size() != 0 )
    {
        std::map<std::string, bool>* lpBuyMap = (std::map<std::string, bool>*)session_get(context, "buyedList");

        if ( lpBuyMap != NULL )
        {
            auto it = theMap.begin();

            for (; it != theMap.end(); ++it)
            {
                const std::string& contentUuid = (*it).first;

                std::map<std::string, bool>::iterator itFind = (*lpBuyMap).find(contentUuid);

                if ( itFind == (*lpBuyMap).end() )
                {
                    (*lpBuyMap)[contentUuid] = true;

                    isSet = true;
                }
            }

            SessionSerial(context, "buyedList", (void*)lpBuyMap);
        }
        else
        {
            std::map<std::string, bool>* lpMap = SafeAllocNewSBMap();

            if ( lpMap != NULL )
            {
                auto it = theMap.begin();

                for (; it != theMap.end(); ++it)
                {
                    const std::string& contentUuid = (*it).first;

                    (*lpMap)[contentUuid] = true;
                }

                session_set(context, "buyedList", lpMap, SessionBusiness::SafeReleaseSBMap);

                SessionSerial(context, "buyedList", (void*)lpMap);

                isSet = true;
            }
        }
    }
   
    return isSet;
}

std::map<std::string, int>* SessionBusiness::SafeAllocNewSIMap()
{
    std::map<std::string, int>* lpMap = NULL;

    lpMap = new std::map<std::string, int>;

    return lpMap;
}

bool SessionBusiness::SetBuyCountAdd(zxy_request_context* context, const std::string& type)
{
    bool isSet = false;

    std::map<std::string, int>* lpMap = (std::map<std::string, int>*)session_get(context, "buyedCount");

    if ( lpMap != NULL )
    {
        auto itFind = (*lpMap).find(type);

        if ( itFind != (*lpMap).end() )
        {
            (*lpMap)[type] = (*itFind).second + 1;
        }
        else
        {
            (*lpMap)[type] = 1;
        }

        isSet = true;

        SessionSerial(context, "buyedCountAdd", (void*)type.c_str());
    }
    else
    {
        std::map<std::string, int>* lpMap = SafeAllocNewSIMap();

        (*lpMap)[type] = 1;

        session_set(context, "buyedCount", lpMap, SessionBusiness::SafeReleaseSIMap);

        std::string csSerialData = SerialFromMapSI(*lpMap);

        SessionSerial(context, "buyedCountAdd", (void*)type.c_str());
    }

    return isSet;
}

std::map<std::string, int>* SessionBusiness::SafeCopyNewSIMap(const std::map<std::string, int>& theMap)
{
    std::map<std::string, int>* lpNewMap = NULL;

    lpNewMap = new std::map<std::string, int>;

    if ( lpNewMap != NULL )
    {
        if ( theMap.size() != 0 )
        {
            auto it = theMap.begin();

            for(; it != theMap.end(); ++it)
            {
                const std::string type = (*it).first;

                int count = (*it).second;

                (*lpNewMap)[type] = count;
            }
        }
    }

    return lpNewMap;
}

void SessionBusiness::SafeReleaseSIMap(void* lpMap)
{
    if ( lpMap != NULL )
    {
        std::map<std::string, int>* lpDel = (std::map<std::string, int>*)lpMap;

        delete lpDel;

        lpDel = NULL;
    }
}

bool SessionBusiness::SetBuyCount(zxy_request_context* context, const std::map<std::string, int>& theMap)
{
    bool isSet = false;

    std::map<std::string, int>* lpBuyCountMap = (std::map<std::string, int>*)session_get(context, "buyedCount");

    if ( lpBuyCountMap == NULL )
    {
        std::map<std::string, int>* lpNewMap = SafeCopyNewSIMap(theMap);

        session_set(context, "buyedCount", lpNewMap, SessionBusiness::SafeReleaseSIMap);

        SessionSerial(context, "buyedCount", (void*)lpNewMap);
    }
    else
    {
        auto it = theMap.begin();

        for (; it != theMap.end(); ++it)
        {
            const std::string type = (*it).first;

            int count = (*it).second;

            (*lpBuyCountMap)[type] = count;

        }

        SessionSerial(context, "buyedCount", (void*)lpBuyCountMap);
    }

    return isSet;
}

bool SessionBusiness::SetPassword(zxy_request_context* context, const std::string& localPwd)
{
    bool isSet = false;

    char* lpPassword = SafeCopyNewBuffer(localPwd);

    if ( lpPassword != NULL )
    {
        session_set(context, "password", lpPassword, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "password", (void*)localPwd.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetDisplayName(zxy_request_context* context, const std::string& displayName)
{
    bool isSet = false;

    char* lpDisplayName = SafeCopyNewBuffer(displayName);

    if ( lpDisplayName != NULL )
    {
        session_set(context, "displayName", lpDisplayName, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "displayName", (void*)displayName.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetUserId(zxy_request_context* context, const std::string& userId)
{
    bool isSet = false;

    char* lpUserId = SafeCopyNewBuffer(userId);

    if ( lpUserId != NULL )
    {
        session_set(context, "userID", lpUserId, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "userID", (void*)userId.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetBalance(zxy_request_context* context, const std::string& balance)
{
    bool isSet = false;

    char* lpBalance = SafeCopyNewBuffer(balance);

    if ( lpBalance != NULL )
    {
        session_set(context, "balance", lpBalance, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "balance", (void*)balance.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetUserPhoto(zxy_request_context* context, const std::string& userPhoto)
{
    bool isSet = false;

    char* lpUserPhoto = SafeCopyNewBuffer(userPhoto);

    if ( lpUserPhoto != NULL )
    {
        session_set(context, "userPhoto", lpUserPhoto, SessionBusiness::SafeReleaseBuffer);

        SessionSerial(context, "userPhoto", (void*)userPhoto.c_str());

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetCookie(zxy_request_context* context, const std::map<std::string, std::string>& cookieMap)
{
    bool isSet = false;

    std::map<std::string, std::string>* lpCookie = SafeCopyNewSSMap(cookieMap);

    if ( lpCookie != NULL )
    {
        session_set(context, "cookie", lpCookie, SessionBusiness::SafeReleaseSSMap);

        std::string csSerialData = SerialFromMapSS(cookieMap);

        SessionSerial(context, "cookie", (void*)lpCookie);

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::UpdateCookie(zxy_request_context* , const std::string& )
{
    // return SetCookie(context, csCookie);
    return true;
}


void SessionBusiness::SafeReleaseQuestion(void* lp)
{
    if ( lp != NULL )
    {
        _LPSESSION_QUESTION lpDel = (_LPSESSION_QUESTION)lp;

        delete lpDel;

        lpDel = NULL;
    }
}

_LPSESSION_QUESTION SessionBusiness::SafeCopyQuestion(const CheckUserNameRequestQuestion& question)
{
    _LPSESSION_QUESTION lpQuestion = new _SESSION_QUESTION;

    if ( lpQuestion != NULL )
    {
        lpQuestion->name = question.name;

        lpQuestion->id = question.id;
    }

    return lpQuestion;
}

bool SessionBusiness::SetAnswer(zxy_request_context* context, const std::string& answer)
{
    bool isSet = false;

    if ( answer.length() != 0 )
    {
        char* lpAnswer = SafeCopyNewBuffer(answer);

        if ( lpAnswer != NULL )
        {
            session_set(context, "answer", lpAnswer, SessionBusiness::SafeReleaseBuffer);

            SessionSerial(context, "answer", (void*)answer.c_str());

            isSet = true;
        }
    }
    return isSet;
}

bool SessionBusiness::SetIsLogin(zxy_request_context* context, int isLogin)
{
    bool isSet = false;

    int* lpIsLogin = SafeCopyNewInteger(isLogin);

    if ( lpIsLogin != NULL )
    {
        session_set(context, "isLogin", lpIsLogin, SessionBusiness::SafeReleaseInteger);

        // session_data中没有"isLogin"字段, 不需要SessionSerial(context, "isLogin", "1");
        // 2表示准备就绪, 购买列表拉取完之后设置为1
        if (isLogin == 2)
        {
            SessionInfo theNewSession;

            theNewSession.sessionId = context->session_id;

            m_theAllSession[theNewSession.sessionId] = theNewSession;

            UpdataOnline(theNewSession.sessionId);
        }
    }

    return isSet;
}


bool SessionBusiness::SetIsActivate(zxy_request_context* ctx)
{
    bool isSet = false;

    const char* lpMac = GetMac(ctx);

    if ( lpMac )
    {
        std::string mac = lpMac;

        UpdataOnline(mac);

        isSet = true;
    }

    return isSet;
}

bool SessionBusiness::SetQuestion(zxy_request_context* context, const CheckUserNameRequestQuestion& question)
{
    bool isSet = false;

    _LPSESSION_QUESTION lpQuestion = SafeCopyQuestion(question);

    if ( lpQuestion != NULL )
    {
        session_set(context, "question", lpQuestion, SessionBusiness::SafeReleaseQuestion);

        SessionSerial(context, "question", (void*)lpQuestion);

        isSet = true;
    }

    return isSet;
}

std::map<std::string, std::string>* SessionBusiness::SafeCopyNewSSMap(const std::map<std::string, std::string>& theSrcMap)
{
    std::map<std::string, std::string>* lpNewMap = new std::map<std::string, std::string>;

    if ( lpNewMap != NULL )
    {
        auto it = theSrcMap.begin();

        for (; it != theSrcMap.end(); ++it)
        {
            (*lpNewMap)[(*it).first] = (*it).second;
        }
    }

    return lpNewMap;
}

bool SessionBusiness::SetAfCoin(zxy_request_context* context, int nAfCoin)
{
    bool isSet = false;

    int* lpAfCoin = SafeCopyNewInteger(nAfCoin);

    if ( lpAfCoin != NULL )
    {
        session_set(context, "afCoin", lpAfCoin, SessionBusiness::SafeReleaseInteger);

        char szBuff[0x10]  = {0};

        snprintf(szBuff, sizeof(szBuff), "%d", nAfCoin);

        SessionSerial(context, "afCoin", (void*)szBuff);

        isSet = true;
    }

    return isSet;
}

void SessionBusiness::SafeReleaseSSMap(void* lp)
{
    if ( lp != NULL )
    {
        std::map<std::string, std::string>* lpDel = (std::map<std::string, std::string>*)lp;

        delete lpDel;

        lpDel = NULL;
    }
}

bool SessionBusiness::SessionDestory(zxy_request_context* context)
{
    if ( context != NULL )
    {
        std::string csSessionId = get_session_id(context);

        DestoryLogoutSession(context->session_id);
            
        session_destroy(csSessionId);
    }

    return true;
}


bool SessionBusiness::DestoryLogoutSession(const std::string& session_id)
{
    bool isDestory = false;

    auto itFind = m_theSessionIDMap.find(session_id);

    if ( itFind != m_theSessionIDMap.end() )
    {
        itFind = m_theSessionIDMap.erase(itFind);
    }

    std::string csPosData;

    char szSingleData[0x100];

    auto it = m_theSessionIDMap.begin();

    for (; it != m_theSessionIDMap.end(); ++it)
    {
        memset(szSingleData, 0, 0x100);

        snprintf(szSingleData, 0x100, "%s;", (*it).first.c_str());

        csPosData += szSingleData;
    }
    // 重写文件, 原内容清空, 使用 "w+"方式
    FILE* lpFile = fopen(m_csOnlineIDFile.c_str(), "wb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_SET);

        int nWriteLen = csPosData.length();

        if ( nWriteLen != 0 )
        {
            char* lpWriteBuff = new char[nWriteLen + 1];

            if ( lpWriteBuff != NULL )
            {
                lpWriteBuff[nWriteLen] = '\0';

                memcpy(lpWriteBuff, csPosData.c_str(), nWriteLen);
                // 这里写SessionPos
                fwrite(lpWriteBuff, 1, nWriteLen, lpFile);

                fflush(lpFile);

                isDestory = true;
            }

            if ( lpWriteBuff != NULL )
            {
                delete[] lpWriteBuff;

                lpWriteBuff = NULL;
            }
        }
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);
    }

    return isDestory;
}

const std::map<std::string, bool>* SessionBusiness::GetBuyedList(zxy_request_context* context)
{
    std::map<std::string, bool>* lpBuyedList = (std::map<std::string, bool>*)session_get(context, "buyedList");

    return lpBuyedList;
}

const std::map<std::string, std::string>* SessionBusiness::GetCookie(zxy_request_context* context)
{
    std::map<std::string, std::string>* lpCookie = (std::map<std::string, std::string>*)session_get(context, "cookie");

    return lpCookie;
}

const char* SessionBusiness::GetUserName(zxy_request_context* context)
{
    char* lpUserName = (char*)session_get(context, "userName");

    return lpUserName;
}

_LPSESSION_QUESTION SessionBusiness::GetQuestion(zxy_request_context* context)
{
    const _LPSESSION_QUESTION lpQuestion = (_LPSESSION_QUESTION)session_get(context, "question");

    return lpQuestion;
}

const char* SessionBusiness::GetAnswer(zxy_request_context* context)
{
    char* lpAnswer = (char*)session_get(context, "answer");

    return lpAnswer;
}

int SessionBusiness::GetAfCoin(zxy_request_context* context)
{
    int nAfCoin = *(int*)session_get(context, "afCoin");

    return nAfCoin;
}

const std::map<std::string, int>* SessionBusiness::GetBuyCount(zxy_request_context* context)
{
    std::map<std::string, int>* lpBuyCount = (std::map<std::string, int>*)session_get(context, "buyedCount");

    return lpBuyCount;
}

bool SessionBusiness::UpdataOnline(const std::string& csSessionID)
{
    m_theSessionIDMap[csSessionID] = csSessionID;

    auto it = m_theSessionIDMap.begin();

    std::string csData;

    for (; it != m_theSessionIDMap.end(); ++it)
    {
        std::string& csID = (*it).second;

        csData += csID + ";";
    }

    FILE* lpFile = fopen(m_csOnlineIDFile.c_str(), "wb");

    if ( lpFile != NULL )
    {
        int nLen = csData.length();

        char* lpBuff = new char[nLen + 1];

        if ( lpBuff != NULL )
        {         
            memcpy(lpBuff, csData.c_str(), nLen);

            lpBuff[nLen] = '\0';

            fwrite(lpBuff, 1, nLen + 1, lpFile);
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

    return true;
}

bool SessionBusiness::UpdataSession(zxy_request_context* ctx, const std::string& key, void* val, const char* lpMac)
{
    std::string deviceMac = ::GetMac(ctx);
    if ( deviceMac == "ff:ff:ff:ff:ff:ff" && lpMac != NULL)
    {
        deviceMac = lpMac;
    }

    auto it = m_theAllSession.find(deviceMac);

    if ( it == m_theAllSession.end() )
    {
        SessionInfo theSession;

        theSession.sessionId = ctx->session_id;

        m_theAllSession[deviceMac] = theSession;
    }

    it = m_theAllSession.find(deviceMac);

    SessionInfo& theSession = (*it).second;

    if ( key == "userName" )
    {
        theSession.userName = (char*)val;
    }
    else if ( key == "password" )
    {
        theSession.password = (char*)val;
    }
    else if ( key == "displayName" )
    {
        theSession.displayName = (char*)val;
    }
    else if ( key == "userID" )
    {
        theSession.userId = (char*)val;
    }
    else if ( key == "balance" )
    {
        theSession.balance = (char*)val;
    }
    else if ( key == "userPhoto" )
    {
        theSession.userPhoto = (char*)val;
    }
    else if ( key == "question" )
    {
        _LPSESSION_QUESTION lpQuestion = (_LPSESSION_QUESTION)val;

        theSession.questionId = lpQuestion->id;

        theSession.questionName = lpQuestion->name;
    }
    else if ( key == "answer" )
    {
        theSession.answer = (char*)val;
    }
    else if ( key == "mac" )
    {
        theSession.mac = (char*)val;
    }
    else if ( key == "ip" )
    {
        theSession.ip = (char*)val;
    }
    else if ( key == "mpAccount" )
    {
        theSession.mpAccount = (char*)val;
    }
    else if ( key == "mpAgentId" )
    {
        theSession.mpAgentId = (char*)val;
    }
    else if ( key == "localTime" )
    {
        theSession.localTime = (char*)val;
    }
    else if ( key == "isLogin" )
    {
    }
    else if ( key == "afCoin" )
    {
        theSession.afCoin = (char*)val;
    }else if (key == "isPlayAd")
    {
        theSession.isPlayAd = (char *)val;
    }
    else if ( key == "cookie" )
    {
        std::map<std::string, std::string>& Cookie = *(std::map<std::string, std::string>*)val;

        auto itCookie = Cookie.begin();

        for (; itCookie != Cookie.end(); ++itCookie)
        {
            theSession.cookie[(*itCookie).first] = (*itCookie).second;
        }
    }
    else if ( key == "buyedListAdd" )
    {
        std::string uuid = (char*)val;

        auto itVec = theSession.buyedList.begin();

        bool isNew = true;

        while ( itVec != theSession.buyedList.end() )
        {
            if ( (*itVec) == uuid )
            {
                isNew = false;
                break;
            }
            ++itVec;
        }

        if ( isNew == true )
        {
            theSession.buyedList.push_back(uuid);
        }
    }
    else if ( key == "buyedList" )
    {
        std::map<std::string, bool>& theMap = *(std::map<std::string, bool>*)val;

        auto itBuyedList = theMap.begin();

        for (; itBuyedList != theMap.end(); ++itBuyedList)
        {
            theSession.buyedList.push_back((*itBuyedList).first);
        }
    }
    else if ( key == "buyedCountAdd" )
    {
        std::string type = (char*)val;

        auto itBuyedCount = theSession.buyedCount.find(type);

        if ( itBuyedCount != theSession.buyedCount.end() )
        {
            theSession.buyedCount[type] = (*itBuyedCount).second + 1;
        }
        else
        {
            theSession.buyedCount[type] = 1;
        }
    }
    else if ( key == "buyedCount" )
    {
        std::map<std::string, int>& theMap = *(std::map<std::string, int>*)val;

        auto itBuyCount = theMap.begin();

        for (; itBuyCount != theMap.end(); ++itBuyCount) 
        {
            theSession.buyedCount[(*itBuyCount).first] = (*itBuyCount).second;
        }
    }

    return true;
}

bool SessionBusiness::SessionSerial(zxy_request_context* ctx, const std::string& key, void* value, const char* lpMac)
{
    bool isWrite = false;

    UpdataSession(ctx, key, value, lpMac);

    std::string csMac = ::GetMac(ctx);
    if ( (csMac == "ff:ff:ff:ff:ff:ff" || csMac == "") && lpMac != NULL )
    {
        csMac = lpMac;
    }

    auto it = m_theAllSession.find(csMac);

    if ( it != m_theAllSession.end() )
    {
        SessionInfo& theSession = (*it).second;

        std::string csSessionData = theSession.encode(true);

        int nLen = csSessionData.length();

        char* lpBuff = new char[nLen + 1];

        if ( lpBuff != NULL )
        {
            lpBuff[nLen] = '\0';

            memcpy(lpBuff, csSessionData.c_str(), nLen);

            std::string filepath = m_csSessionPath + csMac + ".session";

            FILE* lpFile = fopen(filepath.c_str(), "wb");

            if ( lpFile == NULL )
            {
                log("=============================%d", errno);
            }

            if ( lpFile != NULL )
            {
                fwrite(lpBuff, 1, nLen + 1, lpFile);
            }

            if ( lpFile != NULL )
            {
                fclose(lpFile);

                lpFile = NULL;
            }
        }

        if ( lpBuff != NULL )
        {
            delete[] lpBuff;

            lpBuff = NULL;
        }
    }

    return isWrite;
}

const char* SessionBusiness::GetUserID(zxy_request_context* ctx)
{
    char* lpUserId = (char*)session_get(ctx, "userID");

    return lpUserId;
}

const char* SessionBusiness::GetPassword(zxy_request_context* ctx)
{
    char* lpPassword = (char*)session_get(ctx, "password");
    
    return lpPassword;
}

const char* SessionBusiness::GetDisplayName(zxy_request_context* ctx)
{
    char* lpDisplayName = (char*)session_get(ctx, "displayName");

    return lpDisplayName;
}

const char* SessionBusiness::GetBalance(zxy_request_context* ctx)
{
    char* lpBalance = (char*)session_get(ctx, "balance");

    return lpBalance;
}

const char* SessionBusiness::GetUserPhoto(zxy_request_context* ctx)
{
    char* lpUserPhoto = (char*)session_get(ctx, "userPhoto");

    return lpUserPhoto;
}

const int* SessionBusiness::GetIsLogin(zxy_request_context* ctx)
{
    int* lpIsLogin = (int*)session_get(ctx, "isLogin");

    return lpIsLogin;
}

SessionBusiness::SessionBusiness()
{
    /*
       */
}

std::string SessionBusiness::SerialFromMapSS(const std::map<std::string, std::string>& theMap)
{
    std::string csSerial;

    std::string csKey;

    std::string csVal;

    auto it = theMap.begin();

    for (; it != theMap.end(); ++it)
    {
        csKey = (*it).first;

        csVal = (*it).second;

        csSerial += csKey + "," + csVal + ";";
    }

    return csSerial;
}

std::string SessionBusiness::SerialFromMapSI(const std::map<std::string, int>& theMap)
{    
    std::string csSerial;

    std::string csKey;

    std::string csVal;

    char szBuff[0x10] = {0};

    auto it = theMap.begin();

    for (; it != theMap.end(); ++it)
    {
        csKey = (*it).first;

        int nVal = (*it).second;

        snprintf(szBuff, sizeof(szBuff), "%d", nVal);

        csVal = szBuff;

        csSerial += csKey + "," + csVal + ";";
    }

    return csSerial;
}

std::string SessionBusiness::SerialFromMapSB(const std::map<std::string, bool>& theMap)
{
    std::string csSerial;

    std::string csKey;

    std::string csVal;

    auto it = theMap.begin();

    for (; it != theMap.end(); ++it)
    {
        csKey = (*it).first;

        bool bVal = (*it).second;

        csVal = bVal == true ? "true" : "false";

        csSerial += csKey + "," + csVal + ";";
    }

    return csSerial;
}

bool SessionBusiness::DeserialToMapSS(std::map<std::string, std::string>& theMap, const std::string& csSerial)
{
    std::string csSubData, csKey, csVal;

    std::size_t nBeginPos = 0;

    std::size_t nDataLen = csSerial.length();

    std::size_t nNextPos = 0; // csSerial.find(";", nBeginPos);

    do
    {
        nNextPos = csSerial.find(";", nBeginPos);

        if ( nNextPos + 1 >= nDataLen )
        {
            break;
        }

        csSubData = csSerial.substr(nBeginPos, nNextPos - nBeginPos);

        std::size_t KVPos = csSubData.find(",");

        csKey = csSubData.substr(0, KVPos);

        csVal = csSubData.substr(KVPos + 1, csSubData.length() - KVPos);

        theMap[csKey] = csVal;

        nBeginPos = nNextPos + 1;
    }
    while (1);
    
    return true;
}

bool SessionBusiness::DeserialToMapSI(std::map<std::string, int>& theMap, const std::string& csSerial)
{
    std::string csSubData, csKey, csVal;

    std::size_t nBeginPos = 0;

    std::size_t nDataLen = csSerial.length();

    std::size_t nNextPos = 0; // csSerial.find(";", nBeginPos);

    do
    {
        nNextPos = csSerial.find(";", nBeginPos);

        if ( nNextPos + 1 >= nDataLen )
        {
            break;
        }

        csSubData = csSerial.substr(nBeginPos, nNextPos - nBeginPos);

        std::size_t KVPos = csSubData.find(",");

        csKey = csSubData.substr(0, KVPos);

        csVal = csSubData.substr(KVPos + 1, csSubData.length() - KVPos);

        theMap[csKey] = strtol(csVal.c_str(), NULL, 10);

        nBeginPos = nNextPos + 1;
    }
    while (1);

    return true;
}

bool SessionBusiness::DeserialToMapSB(std::map<std::string, bool>& theMap, const std::string& csSerial)
{
    std::string csSubData, csKey, csVal;

    std::size_t nBeginPos = 0;

    std::size_t nDataLen = csSerial.length();

    std::size_t nNextPos = 0; // csSerial.find(";", nBeginPos);

    do
    {
        nNextPos = csSerial.find(";", nBeginPos);

        if ( nNextPos + 1 >= nDataLen )
        {
            break;
        }

        csSubData = csSerial.substr(nBeginPos, nNextPos - nBeginPos);

        std::size_t KVPos = csSubData.find(",");

        csKey = csSubData.substr(0, KVPos);

        csVal = csSubData.substr(KVPos + 1, csSubData.length() - KVPos);

        theMap[csKey] = (csVal == "true" ? true : false);

        nBeginPos = nNextPos + 1;
    }
    while (1);

    return true;
}

bool SessionBusiness::InitSession(const std::string& csPosFilePath, const std::string& csSessionFilePath)
{
    m_csOnlineIDFile = csPosFilePath;
    
    m_csSessionPath = csSessionFilePath;

    if ( access(m_csSessionPath.c_str(), F_OK) != 0 )
    {
        mkdir(m_csSessionPath.c_str(), 0755);
    }

    if ( GetOnlineSessionIDList(csPosFilePath) == true )
    {
        InitSessionMemoryData();
    }

    return true;
}

int SessionBusiness::SplitString(std::vector<std::string>& theVec, const std::string& csSrc, const std::string& csSplit)
{
    std::size_t nBeginPos = 0;

    std::size_t nNextPos = 0; 

    std::string csSub;

    std::size_t nSize = csSrc.length();

    do
    {
        nNextPos = csSrc.find(csSplit, nBeginPos);

        csSub = csSrc.substr(nBeginPos, nNextPos - nBeginPos);

        theVec.push_back(csSub);

        if ( nNextPos + 1 >= nSize )
        {
            break;
        }

        nBeginPos = nNextPos + 1;
    }
    while (1);

    return theVec.size();
}

bool SessionBusiness::GetOnlineSessionIDList(const std::string& csFilePath)
{
    bool isInit = false;
    // 只读文件, 当文件不存在时创建, 使用 "r"
    FILE* lpFile = fopen(csFilePath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize > 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                lpBuff[nSize] = '\0';

                fread(lpBuff, 1, nSize, lpFile);

                std::vector<std::string> theVec;
                
                int nCount = SplitString(theVec, lpBuff, ";");

                for (int nIndex = 0; nIndex < nCount; ++nIndex)
                {
                    m_theSessionIDMap[theVec[nIndex]] = theVec[nIndex];
                }
                               
                isInit = true;
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }
    }
    else
    {
        // 文件不存在, 创建文件
        lpFile = fopen(csFilePath.c_str(), "wb");
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }

    return isInit;
}

bool SessionBusiness::InitSessionMemoryData()
{
    bool isInit = false;

    int nOnlineCount = m_theSessionIDMap.size();

    if ( nOnlineCount != 0 )
    {
        auto it = m_theSessionIDMap.begin();

        for (; it != m_theSessionIDMap.end(); ++it)
        {
            ResumeSession((*it).first);
        }
    }
   
    return isInit;
}

bool SessionBusiness::SessionSet(const SessionInfo& theInfo)
{
    int nLen = theInfo.userId.length() ;

    if ( theInfo.userId.length() != 0 )
    {
        char* lpUserID = SafeCopyNewBuffer(theInfo.userId);
        
        if ( lpUserID != NULL )
        {
            memcpy(lpUserID, theInfo.userId.c_str(), nLen);

            lpUserID[nLen] = '\0';
            
            session_set_ex(theInfo.sessionId, "userID", lpUserID, SessionBusiness::SafeReleaseBuffer);
        }
    }

    nLen = theInfo.userName.length();

    if ( nLen != 0 )
    {
        char* lpUserName = SafeCopyNewBuffer(theInfo.userName);

        if ( lpUserName != NULL )
        {
            memcpy(lpUserName, theInfo.userName.c_str(), nLen);

            lpUserName[nLen] = '\0';

            session_set_ex(theInfo.sessionId, "userName", lpUserName, SessionBusiness::SafeReleaseBuffer);
        }
    }

    nLen = theInfo.displayName.length();

    if ( nLen != 0 )
    {
        char* lpDisplayName = SafeCopyNewBuffer(theInfo.displayName);

        if ( lpDisplayName != NULL )
        {
            lpDisplayName[nLen] = '\0';

            memcpy(lpDisplayName, theInfo.displayName.c_str(), nLen);

            session_set_ex(theInfo.sessionId, "displayName", lpDisplayName, SessionBusiness::SafeReleaseBuffer);
        }
    }

    nLen = theInfo.balance.length();

    if ( nLen != 0 )
    {
        char* lpBalance = SafeCopyNewBuffer(theInfo.balance);
        if ( lpBalance != NULL )
        {
            memcpy(lpBalance, theInfo.balance.c_str(), nLen);

            lpBalance[nLen] = '\0';

            session_set_ex(theInfo.sessionId, "banlance", lpBalance, SessionBusiness::SafeReleaseBuffer);
        }
    }

    if ( theInfo.afCoin.length() != 0 )
    {
        int nAfCoin = strtol(theInfo.afCoin.c_str(), NULL, 10);

        int* lpAfCoin = SafeCopyNewInteger(nAfCoin);

        if ( lpAfCoin != NULL )
        {
            session_set_ex(theInfo.sessionId, "afCoin", lpAfCoin, SessionBusiness::SafeReleaseInteger);
        }
    }

    if ( theInfo.questionId.length() != 0 
            && theInfo.questionName.length() != 0 )
    {
        char* lpQuestionID = SafeCopyNewBuffer(theInfo.questionId);

        if ( lpQuestionID != NULL )
        {
            session_set_ex(theInfo.sessionId, "questionId", lpQuestionID, SessionBusiness::SafeReleaseBuffer);
        }

        char* lpQuestionName = SafeCopyNewBuffer(theInfo.questionName);

        if ( lpQuestionName != NULL )
        {
            session_set_ex(theInfo.sessionId, "questionName", lpQuestionName, SessionBusiness::SafeReleaseBuffer);
        }
    }

    if ( theInfo.answer.length() != 0 )
    {
        char* lpAnswer = SafeCopyNewBuffer(theInfo.answer);

        if ( lpAnswer != NULL )
        {
            session_set_ex(theInfo.sessionId, "answer", lpAnswer, SessionBusiness::SafeReleaseBuffer);
        }
    }

    if ( theInfo.buyedCount.size() != 0 )
    {
        std::map<std::string, int>* lpMap = SafeCopyNewSIMap(theInfo.buyedCount);
        
        if ( lpMap != NULL )
        {
            session_set_ex(theInfo.sessionId, "buyedCount", lpMap, SessionBusiness::SafeReleaseSIMap);
        }
    }

    if ( theInfo.buyedList.size() != 0 )
    {
        std::map<std::string, bool>* lpMap = SafeCopySBMapFromVec(theInfo.buyedList);

        if ( lpMap != NULL )
        {
            session_set_ex(theInfo.sessionId, "buyedList", lpMap, SessionBusiness::SafeReleaseSBMap);
        }
    }

    if ( theInfo.cookie.size() != 0 )
    {
        std::map<std::string, std::string>* lpMap = SafeCopyNewSSMap(theInfo.cookie);

        if ( lpMap != NULL )
        {
            session_set_ex(theInfo.sessionId, "cookie", lpMap, SessionBusiness::SafeReleaseSSMap);
        }
    }

    if ( 1 ) // IsLogin
    {
        int* lpIsLogin = SafeCopyNewInteger(1);

        session_set_ex(theInfo.sessionId, "isLogin", lpIsLogin, SessionBusiness::SafeReleaseBuffer);
    }

    return true;
}
bool SessionBusiness::ReloadSession(zxy_request_context* context, const std::string& mac)
{
    bool isResume = false;

    std::string filepath = m_csSessionPath + mac + ".session";
    
    FILE* lpFile = fopen(filepath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                SessionInfo theOldSession;

                theOldSession.decode(lpBuff);

                theOldSession.sessionId = context->session_id;

                m_theAllSession[theOldSession.sessionId] = theOldSession;

                SessionSet(theOldSession);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }

    return isResume;
}


bool SessionBusiness::ResumeSession( const std::string& session_id)
{
    bool isResume = false;

    std::string filepath = m_csSessionPath + session_id + ".session";
    
    FILE* lpFile = fopen(filepath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                SessionInfo theOldSession;

                theOldSession.decode(lpBuff);

                theOldSession.sessionId = session_id;

                m_theAllSession[theOldSession.sessionId] = theOldSession;

                SessionSet(theOldSession);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }

    return isResume;
}


bool SessionBusiness::SetMac(zxy_request_context* ctx, const std::string& mac)
{
    bool isSet = false;

    if ( mac.length() != 0 )
    {
        char* lpMac = SafeCopyNewBuffer(mac);

        if ( lpMac != NULL )
        {
            session_set(ctx, "mac", lpMac, SessionBusiness::SafeReleaseBuffer);

            SessionSerial(ctx, "mac", (void*)mac.c_str());

            isSet = true;
        }
    }

    return isSet;
}


const char* SessionBusiness::GetMac(zxy_request_context* ctx)
{
    SessionInfo theSess;
    if ( DecodeSession(ctx, theSess) == true )
        return theSess.mac.c_str();
    return NULL;
    //char* lpMac = (char*)session_get(ctx, "mac");
    //return lpMac;
}


bool SessionBusiness::SetMpAccount(zxy_request_context* ctx, const std::string& account, const char* mac)
{
    bool isSet = false;

    if ( account.length() != 0 )
    {
        char* lpAccount = SafeCopyNewBuffer(account);

        if ( lpAccount != NULL )
        {
            session_set(ctx, "mpAccount", lpAccount, SessionBusiness::SafeReleaseBuffer);

            SessionSerial(ctx, "mpAccount", (void*)account.c_str(), mac);

            isSet = true;
        }
    }

    return isSet;
}

bool SessionBusiness::SetMpAgentId(zxy_request_context* ctx, const std::string& agentId, const char* mac)
{
    bool isSet = false;

    if ( agentId.length() != 0 )
    {
        char* lpAgentId = SafeCopyNewBuffer(agentId);

        if ( lpAgentId != NULL )
        {
            session_set(ctx, "mpAgentId", lpAgentId, SessionBusiness::SafeReleaseBuffer);

            SessionSerial(ctx, "mpAgentId", (void*)agentId.c_str(), mac);

            isSet = true;
        }
    }

    return isSet;
}

bool SessionBusiness::DecodeSession(zxy_request_context* ctx, SessionInfo& theOldSession)
{
    bool isDecode = false;

    std::string mac = ::GetMac(ctx);

    std::string filepath = m_csSessionPath + mac + ".session";
    
    FILE* lpFile = fopen(filepath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                theOldSession.decode(lpBuff);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }

        isDecode = true;
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }

    return isDecode;
}

const char* SessionBusiness::GetMpAccount(zxy_request_context* ctx)
{
    // char* lpMpAccount = (char*)session_get(ctx, "mpAccount");
    SessionInfo theSess;
    if ( DecodeSession(ctx, theSess) == true )
        return theSess.mpAccount.c_str();
    return NULL;
}

const char* SessionBusiness::GetMpAgentId(zxy_request_context* ctx)
{
    //char* lpMpAgentId = (char*)session_get(ctx, "mpAgentId");
    SessionInfo theSess;
    if ( DecodeSession(ctx, theSess) == true )
        return theSess.mpAgentId.c_str();
    return NULL;
}

/*
std::string SessionBusiness::GetDeviceMac(zxy_request_context*ctx, std::string& OutMac)
{
    std::string RealIp = zxy_get_http_header(ctx, "X-Real-Ip");
    char mac[40] = {0};
    char ip[40] = {0};
    evhtp_request_t* req = ctx->request;
    int handle = init_arp_table_handle();
    inet_ntop(AF_INET, &((struct sockaddr_in*)req->conn->saddr)->sin_addr, ip, sizeof(ip));
    get_mac(handle, RealIp.c_str(), g_csGateway.c_str(), mac, sizeof(mac));
    release_arp_table_handle(handle);

    OutMac = mac;

    return mac;
}
*/

bool SessionBusiness::SetLocalTime(zxy_request_context* ctx, const char* lpMac, const char* lpTime)
{
    char szLocalTime[60] = {0};
    if ( lpTime == NULL )
    {
        time_t t = time(NULL);
        struct tm* lpTM = localtime(&t);
        strftime(szLocalTime, sizeof(szLocalTime), "%Y-%m-%d %H:%M:%S", lpTM);
    }
    else
    {
        snprintf(szLocalTime, 60, "%s", lpTime);
    }

    bool isSet = false;

    if ( strlen(szLocalTime) != 0 )
    {
        char* lpLocalTime = SafeCopyNewBuffer(szLocalTime);

        if ( lpLocalTime != NULL )
        {
            session_set(ctx, "localTime", lpLocalTime, SessionBusiness::SafeReleaseBuffer);
            SessionSerial(ctx, "localTime", (void*)lpLocalTime, lpMac);

            isSet = true;
        }
    }

    return isSet;
}


bool SessionBusiness::isTimeOutSession(zxy_request_context* ctx)
{
    SessionInfo theSess;
    if ( DecodeSession(ctx, theSess) == true )
    {
        std::string initTime = "1970-01-01 00:00:00";
        if ( theSess.localTime.length() == 0 || theSess.localTime == initTime )
        {
            return true;
        }
/*
        char szLocalTime[60] = {0};
        time_t t = time(NULL);
        struct tm* lpTM = localtime(&t);
        strftime(szLocalTime, sizeof(szLocalTime), "%Y-%m-%d %H:%M:%S", lpTM);

        int Y1, M1, D1, h1, m1, s1;
        sscanf(szLocalTime, "%d-%d-%d %d:%d:%d", &Y1, &M1, &D1, &h1, &m1, &s1);

        int Y2, M2, D2, h2, m2, s2;
        sscanf(theSess.localTime.c_str(), "%d-%d-%d %d:%d:%d",&Y2, &M2, &D2, &h2, &m2, &s2);
        
        if (Y1 != Y2 || M1 != M2 || D1 != D2)
        {
            return true;
        }
        
        int sec1 = h1 * 3600 + m1 * 60 + s1;
        int sec2 = h2 * 3600 + m2 * 60 + s2 + g_nDisTimeOut; // after disconnect 10 min
        if ( sec1 > sec2 )
        {
            return true;
        }
        else
        {
            log("+++++++++++++++++++disconnect time less then 10 min++++++++++++++++++");
        }
*/
    }

    return false;
}

bool SessionBusiness::SetIp(zxy_request_context* ctx, const std::string& ip)
{
    bool isSet = false;

    if ( ip.length() != 0 )
    {
        char* lpIP = SafeCopyNewBuffer(ip);

        if ( lpIP != NULL )
        {
            session_set(ctx, "ip", lpIP, SessionBusiness::SafeReleaseBuffer);

            SessionSerial(ctx, "ip", (void*)ip.c_str());

            isSet = true;
        }
    }

    return isSet;

}

bool SessionBusiness::SetAdInfo(zxy_request_context *context, int val, const std::string &mac)
{
   /* bool isSet = false;

    int* lpAfCoin = SafeCopyNewInteger(val);

    if ( lpAfCoin != NULL )
    {
        session_set(context, "isPlayAd", lpAfCoin, SessionBusiness::SafeReleaseInteger);

        char szBuff[0x10]  = {0};

        snprintf(szBuff, sizeof(szBuff), "%d", val);

        SessionSerial(context, "isPlayAd", (void*)szBuff);

        isSet = true;
    }

    return isSet;
*/
    bool isSet = false;

            //int* lpAfCoin = SafeCopyNewInteger(val);
    char szBuff[0x10]  = {0};

   snprintf(szBuff, sizeof(szBuff), "%d", val);
   std::string sval(szBuff) ;
   char *lpVal = SessionBusiness::SafeCopyNewBuffer(sval);

   if ( lpVal != NULL )
   {
    session_set(context, "isPlayAd", lpVal, SessionBusiness::SafeReleaseBuffer);

    SessionSerial(context, "isPlayAd", (void*)szBuff, mac.c_str());
    isSet = true;
   }

  return isSet;

}

const char* SessionBusiness::GetIp(const std::string& mac)
{
    SessionInfo theSess;
    bool isDecode = false;

    std::string filepath = m_csSessionPath + mac + ".session";
    FILE* lpFile = fopen(filepath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                theSess.decode(lpBuff);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }

        isDecode = true;
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }
    
    if ( isDecode == true )
    {
        return theSess.ip.c_str();
    }

    return "";
}

int SessionBusiness::GetAdInfo(zxy_request_context *context, const std::string &mac)
{


    SessionInfo theSess;
    bool isDecode = false;

    std::string filepath = m_csSessionPath + mac + ".session";
    FILE* lpFile = fopen(filepath.c_str(), "rb");
    log("%s", filepath.c_str());
    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                theSess.decode(lpBuff);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }

        isDecode = true;
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }
    
    if ( isDecode == true )
    {
        int val = atoi(theSess.isPlayAd.c_str());
        return val;
    }
    log("cant decode session");
    return 0;


 /*   char *buf = (char*)session_get(context, "isPlayAd");
    if(buf){
        int val = atoi(buf);
        return val;
    }


    return 0;
   */ 
}

const char* SessionBusiness::GetLocalTime(const std::string& mac)
{
    SessionInfo theSess;
    bool isDecode = false;

    std::string filepath = m_csSessionPath + mac + ".session";
    FILE* lpFile = fopen(filepath.c_str(), "rb");

    if ( lpFile != NULL )
    {
        fseek(lpFile, 0, SEEK_END);

        int nSize = ftell(lpFile);

        if ( nSize != 0 )
        {
            fseek(lpFile, 0, SEEK_SET);

            char* lpBuff = new char[nSize + 1];

            if ( lpBuff != NULL )
            {
                fread(lpBuff, 1, nSize, lpFile);

                lpBuff[nSize] = '\0';

                theSess.decode(lpBuff);
            }

            if ( lpBuff != NULL )
            {
                delete[] lpBuff;

                lpBuff = NULL;
            }
        }

        isDecode = true;
    }

    if ( lpFile != NULL )
    {
        fclose(lpFile);

        lpFile = NULL;
    }
    
    if ( isDecode == true )
    {
        return theSess.localTime.c_str();
    }

    return "1970-01-01 00:00:00";

}
