//
// Created by 78472 on 2022/6/12.
//

#include "TimeStamp.h"
#include "sys/time.h"
#include <cstring>

muduo::TimeStamp muduo::TimeStamp::now() {
    time_t now = time(nullptr);
    return TimeStamp(now);
}

muduo::TimeStamp muduo::TimeStamp::fromUnixTime(time_t t) {
    return TimeStamp(t);
}

/**
 * 格式化打印时间
 * 将时间戳拆分为秒、微秒，单独打印
 * @param showMicrosecods
 * @return
 */
 string muduo::TimeStamp::toFormattedString(bool printOption){
    char buf[64]{};
    //时间戳转换为：年月日时分秒时间结构
    struct timeval tv{};
    struct timezone tz{};
    struct tm *tm_time = nullptr;

    gettimeofday(&tv, &tz);
    tm_time = localtime(&tv.tv_sec);

    if(printOption){
        snprintf(buf, sizeof buf, "%4d-%02d-%02d %02d:%02d:%02d:%06ld",
                 tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday,
                 tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec, tv.tv_usec);
    }else{
        strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", tm_time);
    }

    return string(buf, strlen(buf));
}

