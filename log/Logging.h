//
// Created by 78472 on 2022/5/30.
//

#ifndef EXHIBITION_LOGGING_H
#define EXHIBITION_LOGGING_H

#include "LogStream.h"
#include "TimeStamp.h"
#include <functional>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

/*
带格式控制的一般格式为：
    "\033[控制码1； 控制码2；控制码3...m<字符串内容>\033[控制码m"
控制码：
    \033[0m    关闭所有属性   ：一般放在后面，这样只会影响你输入的字符串的格式
    \033[1m    设置高亮度
    \033[4m    下划线
    \033[5m    闪烁
    \033[7m    反显
    \033[8m    消隐
    \033[30m    --    \033[37m    设置字体颜色
    \033[40m    --    \033[47m    设置背景色

字背景颜色范围:
    40  41  42  43  44   45   46   47
    黑  红  绿   黄   蓝   紫   深绿  白
字体颜色范围：
    30  31  32  33  34   35   36   37
    黑  红  绿   黄   蓝   紫   深绿  白
 */

#define COLOR_NONE  "\033[0m"        //表示清除前面设置的格式
#define BLACK       "\033[1;30m"     //黑色
#define RED         "\033[1;31m"     //红色
#define GREEN       "\033[1;32m"     //绿色
#define YELLOW      "\033[1;33m"     //黄色
#define BLUE        "\033[1;34m"     //蓝色
#define PURPLE      "\033[1;35m"     //紫色
#define DEEP_GREEN   "\033[1;36m"     //深绿
#define WHITE       "\033[1;37m"     //白色


namespace muduo{
    extern void logInitLogger(string& path);    //初始化log文件路径

    /*
     * 打印过程：创建一个Logger对象(构造函数)，输出内容，析构（提取内容，真正打印输出）
     *      1. 向LogStream中写入初始数据：打印行所在文件的文件名，打印行所在的行号，时间戳等
     *      2. 向LogStream中写入真正需要打印的数据
     *      3. 向LogStream写入换行符
     *      4. 从LogStream中提取出buffer中的内容，处理buffer的内容(真正的输出).
     */
    class Logger {
    public:
        enum LogLevel{
            H_WHITE,
            H_DEEP_GREEN,
            H_RED,
            H_GREEN,
            H_YELLOW,
            H_BLUE,
            H_PURPLE,
        };

        explicit Logger(const char* file, int line,  const char* func, LogLevel level);

        //析构时调用真正的输出函数，将日志内容输出
        ~Logger();

        LogStream& stream() { return impl_.stream_; }

        using OutputFunc = std::function<void(const char* msg, size_t len, Logger::LogLevel level)>;
        static void setOutput(OutputFunc);

    private:
        //__FILE__是预编译器提供的字符串表示的绝对路径，从中提取出文件名
        class SourceFile{
        private:
            const char* data_;
            size_t size_;
        public:
            explicit SourceFile(const char* fileName) : data_(fileName){
                const char* slash1 = strrchr(data_, '/');
                const char* slash2 = strrchr(data_, '\\');
                if(slash1){      //提取出文件名
                    data_ = slash1 + 1;
                }else if(slash2){
                    data_ = slash2 + 1;
                }
                size_ = strlen(data_);
            }

            //返回文件名（只是文件的名字，不包含路径）
            string data(){
                return string(data_, size_);
            }
        };

        class Impl{
        public:
            SourceFile fileName_;       //打印语句所在的文件名
            int line_;                  //打印语句所在的行
            Logger::LogLevel level_;    //打印方式
            LogStream stream_;          //打印流，用于输出打印

        public:
            explicit Impl(const char* fileName, int line, const char* func, Logger::LogLevel level)
                : fileName_(fileName), line_(line), level_(level){
                stream_ << "<" << TimeStamp::toFormattedString() << "> ";
            }

            void finish(){ stream_ << "\n"; }
        };

        Impl impl_;
    };


    #define LOG_INFO    muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_WHITE).stream()
    #define LOG_HLIGHT  muduo::Logger(__FILE__, __LINE__, __FUNCTION__,  muduo::Logger::H_DEEP_GREEN).stream()

    #define LOG_RED  muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_RED).stream()
    #define LOG_GREEN  muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_GREEN).stream()
    #define LOG_YELLOW  muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_YELLOW).stream()
    #define LOG_BLUE  muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_BLUE).stream()
    #define LOG_PURPLE  muduo::Logger(__FILE__, __LINE__, __FUNCTION__, muduo::Logger::H_PURPLE).stream()
}




#endif //EXHIBITION_LOGGING_H
