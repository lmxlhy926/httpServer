//
// Created by 78472 on 2022/4/29.
//

#include "QData.h"
#include <sstream>
#include <fstream>

namespace qlibc{
    QData::QData() {
        _value.reset(new Json::Value(Json::nullValue));
        _mutex.reset(new std::recursive_mutex);
    }

    QData::QData(const string &source) {
        _value.reset(new Json::Value(Json::nullValue));
        _mutex.reset(new std::recursive_mutex);
        parseJson(source, *_value);
    }

    QData::QData(const char* str, int size){
        _value.reset(new Json::Value(Json::nullValue));
        _mutex.reset(new std::recursive_mutex);
        parseJson(str, size, *_value);
    }

    QData::QData(const Json::Value &val) {
        _value.reset(new Json::Value(val));
        _mutex.reset(new std::recursive_mutex);
    }

    QData::QData(const QData& data) {
        _value.reset(new Json::Value(data.asValue()));
        _mutex.reset(new std::recursive_mutex);
    }

    QData& QData::operator= (const QData& data){
        _value.reset(new Json::Value(data.asValue()));
        return *this;
    }

    Json::Value& QData::asValue() const{
        return *_value;
    }

    Json::ArrayIndex QData::size() const {
        return _value->size();
    }

    Json::ValueType QData::type() const {
        return _value->type();
    }

    bool QData::empty() const {
        return _value->empty();
    }

    void QData::clear() {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(_value->isNull() || _value->isObject() || _value->isArray()){
            _value->clear();
        }
    }

    void QData::removeMember(const string &key) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(_value->isNull() || _value->isObject()){
            _value->removeMember(key.c_str());
        }
    }

    Json::Value::Members QData::getMemberNames() const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(_value->isNull() || _value->isObject()){
            return _value->getMemberNames();
        }
        return Json::Value::Members();
    }

    QData& QData::setInitData(const QData& data){
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        setInitValue(data.asValue());
        return *this;
    }

    QData& QData::setInitValue(const Json::Value& value){
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        *_value = value;
        return *this;
    }

    void QData::toJsonString(string &str, bool expand) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(expand)
            str = _value->toStyledString();
        else
            valueToJsonString(*_value, str);
    }

    std::string QData::toJsonString(bool expand) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        string str;
        toJsonString(str, expand);
        return str;
    }

    void QData::loadFromFile(const string &filePathName) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        _value.reset(new Json::Value(parseFromFile(filePathName)));
    }

    void QData::saveToFile(const string &filePathName, bool expand) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        writeToFile(filePathName, *_value, expand);
    }

    bool QData::getBool(const string &key, bool defValue) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(!_value->isObject() || key.empty()) return defValue;
        Json::Value v = _value->get(key, Json::Value());
        if(v.isBool())  return v.asBool();
        return defValue;
    }

    bool QData::getBool(const std::string& key) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        return getBool(key, false);
    }

    QData &QData::setBool(const string &key, bool value) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if((!_value->isNull() && !_value->isObject()) || key.empty())   return *this;
        (*_value)[key] = value;
        return *this;
    }

    int QData::getInt(const string &key, int defValue) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(!_value->isObject() || key.empty())  return defValue;
        Json::Value v = (*_value).get(key, Json::Value());
        if(v.isInt())   return v.asInt();
        return defValue;
    }

    int QData::getInt(const string &key) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        return getInt(key, -1);
    }

    QData &QData::setInt(const string &key, int val) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if((!_value->isNull() && !_value->isObject()) || key.empty())
            return *this;
        (*_value)[key] = val;
        return *this;
    }

    std::string QData::getString(const string &key, const string &defValue) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(!_value->isObject() || key.empty())  return defValue;
        Json::Value v = _value->get(key, Json::Value());
        if(v.type() == Json::objectValue || v.type() == Json::arrayValue){
            return defValue;
        }
        return v.asString();
    }

    std::string QData::getString(const string &key) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        return getString(key, "");
    }

    QData &QData::setString(const string &key, const string &value) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if((!_value->isNull() && !_value->isObject()) || value.empty())
            return *this;
        (*_value)[key] = value;
        return *this;
    }

    void QData::getData(const string &key, QData &data) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(!_value->isObject() || key.empty()){
            data.setInitValue(Json::Value());
            return;
        }
        Json::Value v = _value->get(key, Json::Value());
        data.setInitValue(v);
    }

    QData QData::getData(const string &key) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        QData data;
        getData(key, data);
        return data;
    }

    QData& QData::putData(const string &key, const QData &data) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if((!_value->isNull() && !_value->isObject()) || key.empty())
            return *this;
        (*_value)[key] = data.asValue();
        return *this;
    }

    void QData::getValue(const string &key, Json::Value &value) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if(!_value->isObject() || key.empty()){
            value = Json::Value();
            return;
        }
        value = _value->get(key, Json::Value());
    }

    Json::Value QData::getValue(const string &key) const{
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        Json::Value v;
        getValue(key, v);
        return v;
    }

    QData& QData::setValue(const string &key, const Json::Value &value) {
        std::lock_guard<std::recursive_mutex> lg(*_mutex);
        if((!_value->isNull() && !_value->isObject()) || key.empty())  return *this;
        (*_value)[key] = value;
        return *this;
    }

    bool QData::contains(const string &src, const string &dest) {
        if(src.find(dest) != std::string::npos) return true;
        return false;
    }

    bool QData::parseJson(const string &srcStr, Json::Value &destValue) {
        Json::CharReaderBuilder b;
        std::unique_ptr<Json::CharReader> reader(b.newCharReader());
        const char* str = srcStr.c_str();
        JSONCPP_STRING errs;
        bool ok = reader->parse(str, str + srcStr.size(), &destValue, &errs);
        if (!ok){
            destValue = Json::nullValue;
            return false;
        }
        return true;
    }

    bool QData::parseJson(const char* srcStr, int srcSize, Json::Value& destValue){
        Json::CharReaderBuilder b;
        std::unique_ptr<Json::CharReader> reader(b.newCharReader());
        const char* str = srcStr;
        JSONCPP_STRING errs;
        bool ok = reader->parse(str, str + srcSize, &destValue, &errs);
        if (!ok){
            destValue = Json::nullValue;
            return false;
        }
        return true;
    }

    Json::Value QData::parseJson(const string &srcStr) {
        Json::Value destValue(Json::nullValue);
        parseJson(srcStr, destValue);
        return destValue;
    }

    bool QData::valueToJsonString(const Json::Value &obj, string &ret) {
        std::ostringstream os;
        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(obj, &os);
        if(!os.good()){
            ret = "";
            return false;
        }
        ret = os.str();
        return true;
    }

    bool QData::parseFromFile(const string &fileNamePath, Json::Value &value) {
        ifstream infile(fileNamePath,std::ios::in);
        Json::CharReaderBuilder b;
        JSONCPP_STRING errs;
        if (!parseFromStream(b, infile, &value, &errs)) {
            value = Json::nullValue;
            return false;
        }
        return true;
    }

    Json::Value QData::parseFromFile(const string &filePathName) {
        Json::Value value;
        parseFromFile(filePathName, value);
        return value;
    }

    bool QData::writeToFile(const string &filePathName, const Json::Value &value, bool expand) {
        string content;
        if(expand)
            content = value.toStyledString();
        else
            valueToJsonString(value, content);

        std::ofstream out(filePathName, std::ios::out);
        if(out.is_open()){
            out << content;
            return true;
        }

        return false;
    }

    void QData::getArrayElement(Json::ArrayIndex index, QData &element) {
        if(_value->isNull() || _value->isArray()){
            Json::Value v = _value->get(index, Json::Value());
            element.setInitValue(v);
        }
    }

    QData QData::getArrayElement(Json::ArrayIndex index) {
        QData data;
        getArrayElement(index, data);
        return data;
    }

    QData& QData::arrayInsert(Json::ArrayIndex index, QData &element) {
        if(_value->isNull() || _value->isArray()){
            (*_value)[index] = element.asValue();
        }
        return *this;
    }

    QData& QData::append(const QData &data) {
        if(_value->isNull() || _value->isArray()){
            _value->append(data.asValue());
            return *this;
        }
        return *this;
    }

    QData& QData::append(const Json::Value &value) {
        append( QData(value));
        return *this;
    }

    void QData::deleteArrayItem(Json::ArrayIndex index){
        Json::Value value;
        _value->removeIndex(index, &value);
    }

}



