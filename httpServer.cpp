
#include <thread>
#include "siteService/nlohmann/json.hpp"
#include "siteService/service_site_manager.h"
#include "log/Logging.h"

using namespace std;
using namespace servicesite;
using namespace httplib;
using json = nlohmann::json;


int main(int argc, char* argv[]) {
    if(argc != 2){
        fprintf(stderr, "./httpServer <logPaht>...\n");
        exit(-1);
    }

    string path(argv[1]);
    muduo::logInitLogger(path);     //设置log路径

    httplib::ThreadPool threadPool_(10);
    // 创建 serviceSiteManager 对象, 单例
    ServiceSiteManager* serviceSiteManager = ServiceSiteManager::getInstance();
    serviceSiteManager->setServerPort(9000);
    ServiceSiteManager::setSiteIdSummary("httpServer", "服务器测试站点");

    serviceSiteManager->registerServiceRequestHandler("testService",
                                                      [](const Request& request, Response& response) -> int{
        
        LOG_INFO << "received: " << request.body;
        return 0;
    });


    // 站点监听线程启动
    threadPool_.enqueue([&](){
        while(true){
            //自启动方式
            int code = serviceSiteManager->start();
            if(code != 0){
                LOG_INFO << "===>scribeSite startByRegister error, code = ";
                LOG_INFO << "===>scribeSite startByRegister in 3 seconds....";
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        }
    });


    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(60 *10));
    }

    return 0;
}
