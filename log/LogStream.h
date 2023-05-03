//
// Created by 78472 on 2022/5/30.
//

#ifndef EXHIBITION_LOGSTREAM_H
#define EXHIBITION_LOGSTREAM_H

#include <cstdarg>
#include <cstring>
#include <string>

using namespace std;

/*
 * 将所有的数据类型转换为字符串
 */

namespace muduo{

    /*
     * 固定数组：存储字符数据，移动指示位置
     * 参数：
     *      位置：开始位置，当前位置，末尾位置，移动位置，复位位置
     *      容量：已存储长度，后续可存储长度
     *      添加：添加字符、移动位置
     *      转换：返回已存储的字符串
     */
    const int SmallBufferSize = 1024 * 64;

    template<int SIZE>
    class FixedBuffer{
    private:
        char data_[SIZE]{};                 //存储数据的buffer
        char* cur_;                         //记录当前位置
        const char* end() const{            //返回data_末尾位置，[)
            return data_ + sizeof data_;
        }
    public:
        FixedBuffer(): cur_(data_){}        //将当前位置定位到data_的起始位置

        ~FixedBuffer()= default;

        //返回buffer首地址
        const char* data() const{
            return data_;
        }

        //返回buffer当前指示位置
        char* current(){
            return cur_;
        }

        //返回buffer已存储的数据长度
        int length() const{
            return cur_ - data_;
        }

        //数组可用空间，保证最后一位一定为'\0'
        int avail() const{
            return end() - cur_ - 1;
        }

        //如果空间足够，写入数据，移动当前位置
        void append(const char* buf, size_t len){
            if(avail() > len){
                memcpy(cur_, buf, len);
                cur_ += len;
            }
        }

        //移动当前位置
        void add(size_t len){
            cur_ += len;
        }

        //数组清零，复位当前位置到buffer首地址
        void reset(){
            memset(data_, 0, SIZE);
            cur_ = data_;
        }

        //***以字符串格式返回data_内容
        string toString() const{
            return string(data_, length());
        }
    };


    /*
     * 将所有的数据类型全部以字符串的形式写入到Buffer中
     * 如果本次写入操作写入的字符数量大于buffer的剩余数量，则此次写入操作无效。
     */
    class LogStream {
    public:
        typedef LogStream self;
        typedef FixedBuffer<SmallBufferSize> Buffer;
    private:
        Buffer buffer_;                         //存储字符串的buffer
        static const int KMaxNumericSize = 32;  //数字转换后最多占用的字符数
    private:
        //整型-->字符串转换，并将结果写入buffer中
        template<typename T>
        void formatInteger(T);

    public:
        //布尔型
        self& operator<< (bool v);
        //字符型
        self& operator<<(char);
        //整型
        self& operator<<(short);
        self& operator<<(unsigned short);
        self& operator<<(int);
        self& operator<<(unsigned int);
        self& operator<<(long);
        self& operator<<(unsigned long);
        self& operator<<(long long);
        self& operator<<(unsigned long long);
        //浮点型
        self& operator<<(float);
        self& operator<<(double);

        //地址、字符串、string
        self& operator<<(const void*);              //地址
        self& operator<<(const char*);              //字符串地址
        self& operator<<(const unsigned char*);
        self& operator<<(const string&);

        //通用添加
        void append(const char* data, size_t len);

        //返回buffer，重置buffer
        const Buffer& buffer() const;
        void resetBuffer();
    };

}




#endif //EXHIBITION_LOGSTREAM_H
