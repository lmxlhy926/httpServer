//
// Created by 78472 on 2022/6/12.
//

#ifndef EXHIBITION_TIMESTAMP_H
#define EXHIBITION_TIMESTAMP_H

#include <cstdint>
#include <ctime>
#include <string>

using namespace std;

namespace muduo{

    /**
     * 时间戳：
     *      1. 精确到秒
     *      2. 时间戳的字符串形式表示
     */
    class TimeStamp {
    private:
        time_t secondsSinceEpoch_;
    public:
        TimeStamp() :secondsSinceEpoch_(0){}

        explicit TimeStamp(time_t secondsSinceEpochArg)
                : secondsSinceEpoch_(secondsSinceEpochArg){}

        static TimeStamp now();

        static TimeStamp fromUnixTime(time_t t);

        static string toFormattedString(bool printOption = true);
    };
}



#endif //EXHIBITION_TIMESTAMP_H
