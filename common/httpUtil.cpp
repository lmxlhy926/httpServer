//
// Created by 78472 on 2022/5/18.
//

#include "httpUtil.h"
#include "log/Logging.h"

bool httpUtil::sitePostRequest(const string& ip, int port, qlibc::QData& request, qlibc::QData& response){
    httplib::Client client(ip, port);
    client.set_connection_timeout(1, 0);
    client.set_read_timeout(2, 0);
    httplib::Result result =  client.Post("/", request.toJsonString(), "text/json");
    if(result != nullptr){
        response.setInitData(qlibc::QData(result.value().body));
        return true;
    }
    LOG_RED << "-->http Error: " << to_string(result.error());
    return false;
}


SingleSite::SingleSite(string ip, int port) {
    siteIp = std::move(ip);
    sitePort = port;
}

bool SingleSite::send(qlibc::QData &request, qlibc::QData &response) {
    httplib::Client cli(siteIp, sitePort);
    cli.set_connection_timeout(1, 0);
    cli.set_read_timeout(2, 0);
    httplib::Result result =  cli.Post("/", request.toJsonString(), "text/json");
    if(result != nullptr){
       response.setInitData(qlibc::QData(result.value().body));
       return true;
    }
    LOG_RED << "-->http Error: " << to_string(result.error());
    return false;
}

void SingleSite::deleteClient(){
}

string SingleSite::getSiteIp(){
    return siteIp;
}

int SingleSite::getSitePort(){
    return sitePort;
}

SiteRecord* SiteRecord::instance = nullptr;

 SiteRecord *SiteRecord::getInstance() {
    if(instance == nullptr){
        instance = new SiteRecord();
    }
    return instance;
}

void SiteRecord::addSite(string siteName, string siteIp, int sitePort) {
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    auto pos = sites.find(siteName);
    if(pos != sites.end()){
        if(pos->second.getSiteIp() == siteIp && pos->second.getSitePort() == sitePort){
            return;
        }else{
            pos->second.deleteClient();
            sites.erase(siteName);
            sites.emplace(siteName, SingleSite(siteIp, sitePort));
        }
    }else{
        sites.emplace(siteName, SingleSite(siteIp, sitePort));
    }
}

void SiteRecord::removeSite(string siteName) {
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    auto pos = sites.find(siteName);
    if(pos != sites.end()){
        pos->second.deleteClient();
        sites.erase(pos);
    }
}

void SiteRecord::removeSitesNonExist(std::map<string, Json::Value>& sitesMap){
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    for(auto pos = sites.begin(); pos != sites.end();){
        if(sitesMap.find(pos->first) == sitesMap.end()){
            pos->second.deleteClient();
            pos = sites.erase(pos);
        }else{
            pos++;
        }
    }
 }


bool SiteRecord::sendRequest2Site(string siteName, qlibc::QData &request, qlibc::QData &response) {
    bool canSend{false};
    SingleSite site;
    {
        std::lock_guard<std::recursive_mutex> lg(rMutex);
        auto pos = sites.find(siteName);
        if(pos != sites.end()){
            site = pos->second;
            canSend = true;
        }
    }
    if(canSend){
        return site.send(request, response);
    }
    return false;
}

void SiteRecord::printMap() {
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    LOG_INFO << "sites:";
    for(auto& elem :sites){
        LOG_INFO << elem.first << ": <" << elem.second.getSiteIp() << ", " << elem.second.getSitePort() << ">";
    }
}

std::set<string> SiteRecord::getSiteName() {
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    std::set<string> siteNameSet;
    for(auto& elem : sites){
       siteNameSet.insert(elem.first);
    }
    return siteNameSet;
}

bool SiteRecord::getSiteInfo(const string& siteName, string& ip, int& port){
    std::lock_guard<std::recursive_mutex> lg(rMutex);
    auto pos = sites.find(siteName);
    if(pos != sites.end()){
        ip = pos->second.getSiteIp();
        port = pos->second.getSitePort();
        return true;
    }
    return false;
 }

