//
// Created by 78472 on 2022/5/8.
//

#include "StringUtils.h"
#include <cstdarg>

std::string StringUtils::formatString(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char buff[MAX_STR_BUFF_SIZE];
    vsnprintf(buff, MAX_STR_BUFF_SIZE , fmt, ap);
    va_end(ap);
    return buff;
}
