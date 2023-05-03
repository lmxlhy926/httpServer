//
// Created by 78472 on 2022/6/15.
//

#ifndef STL_NONCOPYABLE_H
#define STL_NONCOPYABLE_H

/*
 * 继承此类的所有派生类，不支持拷贝构造和拷贝赋值
 */
namespace muduo{

    class noncopyable{
    public:
        //不支持拷贝构造
        noncopyable(const noncopyable&) = delete;
        //不支持拷贝赋值
        void operator=(const noncopyable&) = delete;
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}








#endif //STL_NONCOPYABLE_H
