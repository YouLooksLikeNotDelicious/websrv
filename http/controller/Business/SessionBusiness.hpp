#ifndef __SESSION_H
#define __SESSION_H

#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "../../libs/session.hpp"

#include "../base.hpp"
#include "Request.hpp"
#include "SessionInfo.hpp"

#define SESSIONDATA_LEN 0x2000

typedef struct _SessionQuestion
{
    std::string name;
    std::string id;
}_SESSION_QUESTION, *_LPSESSION_QUESTION;

typedef struct _SessionBuyedList
{
    std::map<std::string, std::vector<std::string> > buyedCount;
}_SESSION_BUYEDLIST, *_LPSESSION_BUYEDLIST;

class SessionBusiness
{
public:
    std::map<std::string, std::string> m_theSessionIDMap;

    std::string m_csOnlineIDFile;

    std::string m_csSessionPath;

    std::map<std::string, SessionInfo> m_theAllSession;
public:
    static int SplitString(std::vector<std::string>&, const std::string&, const std::string&);
public:
    bool InitSession(const std::string&, const std::string&);
    bool GetOnlineSessionIDList(const std::string& csFilePath);
    bool InitSessionMemoryData();
    bool ResumeSession(const std::string&);
    bool ReloadSession(zxy_request_context*,const std::string&);

    bool SessionSet(const SessionInfo&);
    bool DecodeSession(zxy_request_context* ctx, SessionInfo& theOldSession);
    // std::string GetDeviceMac(zxy_request_context*ctx, std::string&);
public:
    const std::map<std::string, bool>* GetBuyedList(zxy_request_context*);
    const std::map<std::string, std::string>* GetCookie(zxy_request_context*);
    const char* GetUserName(zxy_request_context* context);
    _LPSESSION_QUESTION GetQuestion(zxy_request_context* context);
    const char* GetAnswer(zxy_request_context* context);
    int GetAfCoin(zxy_request_context*);
    const std::map<std::string, int>* GetBuyCount(zxy_request_context*);
    const char* GetUserID(zxy_request_context*);
    const char* GetPassword(zxy_request_context*);
    const char* GetDisplayName(zxy_request_context*);
    const char* GetBalance(zxy_request_context*);
    const char* GetUserPhoto(zxy_request_context*);
    const int* GetIsLogin(zxy_request_context*);
    const char* GetMac(zxy_request_context*);
    const char* GetMpAccount(zxy_request_context*);
    const char* GetMpAgentId(zxy_request_context*);
    const char* GetLocalTime(const std::string& mac);
    bool isTimeOutSession(zxy_request_context*);
    const char* GetIp(const std::string& mac);
    int GetAdInfo(zxy_request_context*, const std::string &);
public:
    std::string SerialFromMapSS(const std::map<std::string, std::string>&);
    std::string SerialFromMapSI(const std::map<std::string, int>&);
    std::string SerialFromMapSB(const std::map<std::string, bool>&);
    bool DeserialToMapSS(std::map<std::string, std::string>&, const std::string&);
    bool DeserialToMapSI(std::map<std::string, int>&, const std::string&);
    bool DeserialToMapSB(std::map<std::string, bool>&, const std::string&);
    bool UpdataSession(zxy_request_context*, const std::string&, void*, const char* lpMac=NULL); 
    bool UpdataOnline(const std::string&);
    bool SessionSerial(zxy_request_context* ctx, const std::string&, void*, const char* lpMac=NULL);
public:
    bool SetUserName(zxy_request_context*, const std::string&);
    bool SetBuyList(zxy_request_context*, const std::string& uuid);
    bool SetBuyList(zxy_request_context*, const std::map<std::string, std::string>&);
    bool SetBuyCountAdd(zxy_request_context*, const std::string&);
    bool SetBuyCount(zxy_request_context*, const std::map<std::string, int>&);
    bool SetPassword(zxy_request_context*, const std::string&);
    bool SetDisplayName(zxy_request_context*, const std::string&);
    bool SetUserId(zxy_request_context*, const std::string&);
    bool SetBalance(zxy_request_context*, const std::string&);
    bool SetUserPhoto(zxy_request_context*, const std::string&);
    bool SetCookie(zxy_request_context*, const std::map<std::string, std::string>&);
    bool SetQuestion(zxy_request_context*, const CheckUserNameRequestQuestion&);
    bool SetAnswer(zxy_request_context*, const std::string&);
    bool SetIsLogin(zxy_request_context*, int);
    bool SetIsActivate(zxy_request_context*);
    bool SetAfCoin(zxy_request_context*, int);
    bool SetMac(zxy_request_context*, const std::string&);
    bool SetIp(zxy_request_context*, const std::string&);
    bool SetAdInfo(zxy_request_context*,  int, const std::string &mac);
    bool SetMpAccount(zxy_request_context*, const std::string&, const char*);
    bool SetMpAgentId(zxy_request_context*, const std::string&, const char*);
    bool SetLocalTime(zxy_request_context*, const char*, const char* lpTime=NULL);
    bool UpdateCookie(zxy_request_context*, const std::string& csCookie);
public:
    static char* SafeCopyNewBuffer(const std::string&);
    static void SafeReleaseBuffer(void*);
    static std::map<std::string, bool>* SafeCopyNewSBMap(const std::map<std::string, bool>&);
    static std::map<std::string, bool>* SafeCopySBMapFromVec(const std::vector<std::string>&);
    static std::map<std::string, bool>* SafeAllocNewSBMap();
    static std::map<std::string, int>* SafeAllocNewSIMap();
    static void SafeReleaseSBMap(void*);
    static int* SafeCopyNewInteger(int);
    static void SafeReleaseInteger(void*);
    static _LPSESSION_QUESTION SafeCopyQuestion(const CheckUserNameRequestQuestion&);
    static void SafeReleaseQuestion(void*);
    static std::map<std::string, std::string>* SafeCopyNewSSMap(const std::map<std::string, std::string>&);
    static void SafeReleaseSSMap(void*);
    static std::map<std::string, int>* SafeCopyNewSIMap(const std::map<std::string, int>&);
    static void SafeReleaseSIMap(void*);
public:
    bool SessionDestory(zxy_request_context*);
    bool DestoryLogoutSession(const std::string& session_id);
public:
    SessionBusiness();
};

extern SessionBusiness g_theSessionBusiness;
extern int g_nDisTimeOut;
#endif

