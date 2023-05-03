//
// Created by 78472 on 2022/5/30.
//

#include "LogStream.h"
#include <algorithm>

namespace muduo{
    const char digits[] = "9876543210123456789";
    const char* zero = digits + 9;
    const char digitsHex[] = "0123456789ABCDEF";

    //Efficient Integer to String Conversions;
    template<typename T>
    size_t convert(char buf[], T value){
        T i = value;    //待转换整数值
        char* p = buf;  //指向buf空间首地址

        //逆序转换
        do{
            int lsd = i % 10;
            *p++ = zero[lsd];
            i /= 10;
        }while(i != 0);

        //增加正负号以及字符串结尾'\0'
        if(value < 0)
            *p++ = '-';
        *p = '\0';

        //转换为正序，返回所占用的空间大小
        std::reverse(buf, p);
        return p - buf;
    }

    //将指针地址转换为16进制表示的字符串:
    //uintptr_t：一个无符号整数类型，它的属性是，任何指向void的有效指针都可以转换为此类型
    size_t convertHex(char buf[], uintptr_t value){
        uintptr_t i = value;
        char* p = buf;

        do{
            int lsd = static_cast<int>(i % 16);
            *p++ = digitsHex[lsd];
            i /= 16;
        }while(i != 0);

        *p = '\0';
        std::reverse(buf, p);
        return p -buf;
    }

    //将整数转换为字符串，并写入到buffer_中
    template<typename T>
    void LogStream::formatInteger(T v) {
        if(buffer_.avail() >= KMaxNumericSize){
            size_t len = convert(buffer_.current(), v);
            buffer_.add(len);
        }
    }

    LogStream& LogStream::operator<<(bool v) {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& LogStream::operator<<(char v) {
       buffer_.append(&v, 1);
       return *this;
    }

    LogStream& LogStream::operator<<(short v) {
       *this << static_cast<int>(v);
       return *this;
    }

    LogStream& LogStream::operator<<(unsigned short v) {
        *this << static_cast<int>(v);
        return *this;
    }

    LogStream& LogStream::operator<<(int v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned int v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(long v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned long v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(long long int v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned long long int v) {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(float v) {
        *this << static_cast<double>(v);
        return *this;
    }

    //最传统的做法就是用snprintf函数，将各种类型的数据转换为字符串
    LogStream& LogStream::operator<<(double v) {
        if(buffer_.avail() >= KMaxNumericSize){
            int len = snprintf(buffer_.current(), KMaxNumericSize, "%.12g", v);
            buffer_.add(len);
        }
        return *this;
    }

    //打印地址
    LogStream& LogStream::operator<<(const void * p) {
        auto v = reinterpret_cast<uintptr_t>(p);
        if(buffer_.avail() >= KMaxNumericSize){
            char* buf = buffer_.current();
            buf[0] = '0';
            buf[1] = 'x';
            size_t len = convertHex(buf + 2, v);
            buffer_.add(len + 2);
        }
        return *this;
    }

    LogStream& LogStream::operator<<(const char* str) {
        if(str)
            buffer_.append(str, strlen(str));
        else
            buffer_.append("(null)", 6);
        return *this;
    }

    LogStream& LogStream::operator<<(const unsigned char* str) {
        *this << reinterpret_cast<const char*>(str);
        return *this;
    }

    LogStream& LogStream::operator<<(const string& v) {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void LogStream::append(const char *data, size_t len) {
        buffer_.append(data, len);
    }

    const LogStream::Buffer& LogStream::buffer() const {
        return buffer_;
    }

    void LogStream::resetBuffer() {
        buffer_.reset();
    }

}
