/*
 * service_site_manager.h
 *
 *  Created on: 2022年4月22日
 *      Author: van
 */

#ifndef LIB_SERVICE_SITE_MANAGER_H_
#define LIB_SERVICE_SITE_MANAGER_H_

#include <iostream>
#include <functional>
#include <semaphore.h>
#include <queue>
#include "http/httplib.h"

#define SERV_LIB_LOG(...) printf(__VA_ARGS__)
// #define SERV_LIB_LOG(...) easylogging_log(__VA_ARGS__)
 
using namespace std;
using namespace httplib;

void http_exception_handler(const Request& request, Response& response, std::exception& e);

void easylogging_log(const char* format, ...);

namespace servicesite {

typedef struct MessageIdT {
    string messageId;
    string name;
    string summary;
} MessageId;

class SiteHandle;
class MessageSubscriberSiteHandle;
class MessageSubscriber;
class ServiceSiteManager;

/**
 * @brief 服务请求处理函数
 * 
 * @param 参数1 服务请求 同 http 库
 * @param 参数2 服务返回 同 http 库
 * @return int 错误码
 */
using ServiceRequestHandler = std::function<int(const Request&, Response&)>;

/**
 * @brief 消息处理函数
 * 
 * @param 参数1 消息 JSON 字符串
 * @return int 错误码
 */
using MessageHandler = std::function<void(const Request&)>;


using ServiceRequestHandlers = std::vector<std::pair<string, ServiceRequestHandler>>;
using MessageIds = std::vector<MessageId>;
using MessageHandlers = std::vector<std::pair<string, MessageHandler>>;

/**
 * @brief 服务站点管理器，服务站点提供相关操作支持
 * 
 */
class ServiceSiteManager {
    static const string SERVICE_ID_GET_SERVICE_LIST;
    static const string SERVICE_ID_GET_MESSAGE_LIST;
    static const string SERVICE_ID_SUBSCRIBE_MESSAGE;
    static const string SERVICE_ID_UNSUBSCRIBE_MESSAGE;
    static const string SERVICE_ID_DEBUG;

    static const int STR_BUF_MAX_SIZE = 1024;

    static ServiceRequestHandlers serviceRequestHandlers;
    static MessageIds messageIds;
    static MessageHandlers messageHandlers;

    static std::vector<SiteHandle> siteHandleList;
    static std::vector<MessageSubscriber> messageSubscriberList;
    static std::vector<MessageSubscriberSiteHandle*> messageSubscriberSiteHandlePList;

    static void rawHttpRequestHandler(const Request& request, Response& response);
    
    static int serviceRequestHandlerGetServiceList(const Request& request, Response& response);
    static int serviceRequestHandlerGetMessageList(const Request& request, Response& response);
    static int serviceRequestHandlerSubscribeMessage(const Request& request, Response& response);
    static int serviceRequestHandlerUnsubscribeMessage(const Request& request, Response& response);

    static int serviceRequestHandlerDebug(const Request& request, Response& response);

    static void messageHandlerRegisterAgain(const Request& request);

    static string siteId;
    static string summary;

    static Server server;
    static int serverPort;

    static const string MESSAGE_SUBSCRIBER_CONFIG_FILE;
    static string messageSubscriberConfigPath;

    static std::thread* pingThreadP;

    static void saveMessageSubscriber(void);
    static void loadMessageSubscriber(void);

    static int registerSite(void);
    static bool subscribeMessage(string message_id, string ip, int port);

    ServiceSiteManager();
    static ServiceSiteManager instance;
public:
    static const int RET_CODE_OK;
    static const int RET_CODE_ERROR_START_SERVER;
    static const int RET_CODE_ERROR_REQ_CONN;
    static const int RET_CODE_ERROR_REQ_STATUS_CODE;
    static const int RET_CODE_ERROR_REQ_NOT_JSON;
    static const int RET_CODE_ERROR_REQ_JSON_FORMAT;
    static const int RET_CODE_ERROR_REQ_CODE;

    static const string RET_OK;

    static const string QUERY_SITE_IP;

    /**
     * @brief 查询站点端口， 约定为 9000， 其他站点端口再次基础上增加
     * 
     */
    static const int QUERY_SITE_PORT = 9000;
    static const int PING_PER_SECONDS = 10;

    static const string QUERY_SITE_SERVICE_ID_SITE_REGISTER;
    static const string QUERY_SITE_SERVICE_ID_SITE_PING;

    static const string QUERY_SITE_MESSAGE_ID_REGISTER_AGAIN;


    static ServiceSiteManager* getInstance() {
        return &instance;                                                   
    }  

    /**
     * @brief 启动 ServiceSite 指定端口
     * 
     * @return int 错误码参照错误码定义
     */
    int start(void);

    /**
     * @brief 启动 ServiceSite 通过服务发布站点注册
     * 
     * @param pSiteId 站点 ID
     * @param pSummary 站点 简介
     * @return int 错误码参照错误码定义
     */
    static int startByRegister(void);

    /**
     * @brief 注册服务请求处理函数
     * 
     * @param serviceId 服务ID
     * @param handler 服务请求处理函数指针
     * @return int 错误码参照错误码定义
     */
    int registerServiceRequestHandler(string serviceId, ServiceRequestHandler handler);

    /**
     * @brief 注册消息ID
     * 
     * @param messageId 消息ID
     * @return int int 错误码参照错误码定义
     */
    int registerMessageId(string messageId);

    /**
     * @brief 注册消息ID
     * 
     * @param messageId 消息ID
     * @param name 消息名称
     * @param summary 消息描述
     * @return int int 错误码参照错误码定义
     */
    int registerMessageId(string messageId, string name, string summary);

    /**
     * @brief 注册消息处理函数
     * 
     * @param messageId 消息ID
     * @param handler 消息处理函数指针
     * @return int 错误码参照错误码定义
     */
    static int registerMessageHandler(string messageId, MessageHandler handler);

    /**
     * @brief 发布消息
     * 
     * @param meeageId 消息ID
     * @param message 消息 JSON 字符串
     * @return int 错误码参照错误码定义
     */
    void publishMessage(string messageId, string message);

    /**
     * @brief 更新局域网内所有站点信息
     * 
     * @return int 错误码参照错误码定义 
     */
    int updateSiteHandleList(void);

    /**
     * @brief 查询站点列表
     * 
     * @param siteHandleList 站点handle列表
     * @return int 错误码参照错误码定义 
     */
    int querySiteList(std::vector<SiteHandle>& pSiteHandleList);

    /**
     * @brief 通过站点ID列表查询站点列表
     * 
     * @param pSiteId 站点ID
     * @param pSiteHandleList 站点handle列表
     * @return int 错误码参照错误码定义 
     */
    int querySiteListBySiteId(string pSiteId, std::vector<SiteHandle>& pSiteHandleList);

    /**
     * @brief 获取站点服务列表
     * 
     * @param ip IP
     * @param port 端口号
     * @param serviceIdList  获取的服务ID列表
     * @return int 错误码参照错误码定义  
     */
    int getServiceList(string ip, int port, std::vector<string>& serviceIdList);

    /**
     * @brief 获取站点服务列表
     * 
     * @param ip IP
     * @param port 端口号
     * @param messageIdList  获取的消息ID列表
     * @return int 错误码参照错误码定义  
     */
    int getMessageList(string ip, int port, std::vector<string>& messageIdList);

    /**
     * @brief 订阅消息
     * 
     * @param ip IP
     * @param port 端口
     * @param messageIdList 消息ID
     * @return int 错误码参照错误码定义   
     */
    static int subscribeMessage(string ip, int port, std::vector<string> messageIdList);

    /**
     * @brief 取消订阅消息
     * 
     * @param ip IP
     * @param port 端口
     * @param messageIdList 消息ID
     * @return int 错误码参照错误码定义   
     */
    int unsubscribeMessage(string ip, int port, std::vector<string> messageIdList);

	int getServerPort() const {
		return serverPort;
	}

	void setServerPort(int serverPort) {
		this->serverPort = serverPort;
	}

    static int createDir(string sPathName);

	static void setMessageSubscriberConfigPath(string pMessageSubscriberConfigPath) {
		messageSubscriberConfigPath = pMessageSubscriberConfigPath;
	}

	static void setSiteIdSummary(string pSiteId, string pSummary) {
		siteId = pSiteId;
		summary = pSummary;
	}
};

class SiteHandle {
    string siteId;
    string summary;
    string ip;
    int port;
public:
    SiteHandle(string pSiteId, string pSummary, string pIp, int pPort);

	string getIp() const {
		return ip;
	}

	void setIp(string ip) {
		this->ip = ip;
	}

	int getPort() const {
		return port;
	}

	void setPort(int port) {
		this->port = port;
	}

	string getSiteId() const {
		return siteId;
	}

	void setSiteId(string siteId) {
		this->siteId = siteId;
	}

	string getSummary() const {
		return summary;
	}

	void setSummary(string summary) {
		this->summary = summary;
	}
};

class MessageSubscriber {
    string messageId;

    std::vector<MessageSubscriberSiteHandle*> siteMessageSubscriberSiteHandlePlist;
public:
    MessageSubscriber(string pMessageId);
    string getMessageId(void);
    bool addSiteMessageSubscriberSiteHandleP(MessageSubscriberSiteHandle* pMessageSubscriberSiteHandle);
    void delSiteMessageSubscriberSiteHandleP(MessageSubscriberSiteHandle* pMessageSubscriberSiteHandle);
    std::vector<MessageSubscriberSiteHandle*> getSiteMessageSubscriberSiteHandlePlist(void);
};

class MessageSubscriberSiteHandle {
    static const int MAX_QUEUE_SIZE = 20;
    static const int MAX_SEND_RETRY = 3;

    string ip;
    int port;

    Client* cli = nullptr;

    sem_t sem;
    std::queue<string> queue;
    std::mutex queue_mutex; // 保护 队列
    std::thread* sendMessageThreadP;

    int sendRetryCount;
    bool isStop;
    
public:
    MessageSubscriberSiteHandle(string pIp, int pPort);
    string getIp(void);
    int getPort(void);
    void sendFromThread(void);
    void sendMessage(string message);
    void setIsStop(bool pIsStop);
    int getSendRetryCount(void);
    bool getIsStop(void);
};

}

#endif /* LIB_SERVICE_SITE_MANAGER_H_ */
