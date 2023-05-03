//
// Created by 78472 on 2022/5/18.
//

#ifndef EXHIBITION_HTTPUTIL_H
#define EXHIBITION_HTTPUTIL_H

#include <string>
#include "http/httplib.h"
#include "qlibc/QData.h"
#include <vector>

using namespace httplib;
using namespace std;

class httpUtil {
public:
    static bool sitePostRequest(const string& ip, int port, qlibc::QData& request, qlibc::QData& response);

};


class SingleSite{
private:
    string siteIp;
    int    sitePort{};
public:
    SingleSite() = default;

    explicit SingleSite(string ip, int port);

    SingleSite& operator= (const SingleSite& other)= default;

    //向站点发送请求
    bool send(qlibc::QData& request, qlibc::QData& response);

    //释放客户端
    void deleteClient();

    string getSiteIp();

    int getSitePort();
};


using SingleSiteVec = std::map<string, SingleSite>;
class SiteRecord{
private:
    SingleSiteVec sites;
    std::recursive_mutex rMutex;
    static SiteRecord* instance;
public:
    static SiteRecord* getInstance();

    void addSite(string siteName, string siteIp, int sitePort);

    void removeSite(string siteName);

    //清除不存在的站点连接
    void removeSitesNonExist(std::map<string, Json::Value>& sitesMap);

    bool sendRequest2Site(string siteName, qlibc::QData& request, qlibc::QData& response);

    void printMap();

    std::set<string> getSiteName();

    bool getSiteInfo(const string& siteName, string& ip, int& port);
};

#endif //EXHIBITION_HTTPUTIL_H
