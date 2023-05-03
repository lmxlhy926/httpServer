//
// Created by 78472 on 2022/4/29.
//

#ifndef EXHIBITION_QLIBC_H
#define EXHIBITION_QLIBC_H

#include <string>
#include <memory>
#include <mutex>
#include "jsoncpp/json.h"

using namespace std;

namespace qlibc{
    /*
 * 封装Json::Value的操作，增加判断条件，避免操作抛出异常从而终止程序
 */
    class QData {
    private:
        std::shared_ptr<Json::Value> _value;
        std::shared_ptr<std::recursive_mutex> _mutex;
    public:
        //构造函数，失败则_value被赋值为Json::Value(Json::nullValue)
        QData();
        explicit QData(const std::string& source);
        explicit QData(const char* str, int size);
        explicit QData(const Json::Value& val);
        //拷贝构造、赋值函数
        QData(const QData& data);
        QData& operator= (const QData& data);

    public:
        Json::Value& asValue() const;
        Json::ArrayIndex size() const;
        Json::ValueType type() const;
        bool empty() const;
        void clear();
        void removeMember(const std::string& key);
        Json::Value::Members getMemberNames() const;

        //将_value赋值为新值
        QData& setInitData(const QData& data);
        QData& setInitValue(const Json::Value& value);

        void toJsonString(std::string& str, bool expand = false) const;
        std::string toJsonString(bool expand = false) const;
        void loadFromFile(const std::string& filePathName);
        void saveToFile(const std::string& filePathName, bool expand = false);

        bool getBool(const std::string& key, bool defValue) const;
        bool getBool(const std::string& key) const;
        QData& setBool(const std::string& key, bool value);

        int getInt(const std::string& key, int defValue) const;
        int getInt(const std::string& key) const;
        QData& setInt(const std::string& key, int val);

        std::string getString(const std::string& key, const std::string& defValue) const;
        std::string getString(const std::string& key) const;
        QData& setString(const std::string& key, const std::string& value);

        void getData(const std::string& key, QData& data) const;
        QData getData(const std::string& key) const;
        QData& putData(const std::string& key, const QData& data);

        void getValue(const std::string& key, Json::Value& value) const;
        Json::Value getValue(const std::string& key) const;
        QData& setValue(const std::string& key, const Json::Value& value);

        void getArrayElement(Json::ArrayIndex index, QData& element);

        QData getArrayElement(Json::ArrayIndex index);

        QData& arrayInsert(Json::ArrayIndex, QData& element);

        QData& append(const QData& data);

        QData& append(const Json::Value &value);

        void deleteArrayItem(Json::ArrayIndex index);

    public:
        /**
        * 判断源字符串中是否含有目标字符串
        * 成功返回true; 失败返回false;
        */
        static bool contains(const std::string& src, const std::string& dest);

        /**
         * 将字符串转换为Json::Value对象
         * 成功返回true; 失败返回false,且destValue为Json::Value(Json::nullValue)
         */
        static bool parseJson(const std::string& srcStr, Json::Value& destValue);

        /**
         * 将字符串转换为Json::Value对象
         * 成功返回true; 失败返回false,且destValue为Json::Value(Json::nullValue)
         */
        static bool parseJson(const char* srcStr, int srcSize, Json::Value& destValue);


        /**
         * 将字符串转换为Json::Value对象,
         * 失败返回Json::Value(Json::nullValue)
         */
        static Json::Value parseJson(const std::string& srcStr);

        /**
         * 将Json::Value对象转换为字符串，失败则ret为空。
         * 成功返回true; 失败返回false,且ret为空;
         */
        static bool valueToJsonString(const Json::Value& obj, std::string& ret);

        /**
         * 将从文件读取的内容转换为Json::Value对象
         * 成功返回true; 失败返回fasle,且value为Json::Value(Json::nullValue)
         */
        static bool parseFromFile(const std::string& fileNamePath, Json::Value& value);

        /**
         * 将从文件读取的内容转换为Json::Value对象
         * 失败返回Json::Value(Json::nullValue)
         */
        static Json::Value parseFromFile(const std::string& filePathName);

        /**
         * 将value对象转换为字符串后写入到文件中（truncate方式）
         * 成功返回true;失败返回false;
         */
        static bool writeToFile(const std::string& filePathName, const Json::Value& value, bool expand = false);

    };

}


#endif //EXHIBITION_QLIBC_H
