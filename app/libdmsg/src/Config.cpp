#include "Config.h"
#include <iostream>

namespace dmsg {

Config::Config() {
	//kafka的配置对象
    mConfGlobal = rd_kafka_conf_new();
	char errstr[128] = {0};
	//设置kafka的全局配置
	rd_kafka_conf_set(mConfGlobal, "fetch.message.max.bytes", "10485760", errstr, sizeof(errstr));
	//创建kafka的topic配置对象
	mConfTopic = rd_kafka_topic_conf_new();
}

Config::~Config() {
    if (mConfGlobal != NULL) {
        rd_kafka_conf_destroy(mConfGlobal);
		mConfGlobal = NULL;
    }
    if (mConfTopic != NULL) {
        rd_kafka_topic_conf_destroy(mConfTopic);
		mConfTopic = NULL;
    }
}

rd_kafka_conf_res_t Config::setConf(const std::string &key, const std::string &value)
{
    char errStr[1024] = {0};
    rd_kafka_conf_res_t result = rd_kafka_conf_set(mConfGlobal, key.c_str(),value.c_str(), errStr, sizeof errStr);
    if (result == RD_KAFKA_CONF_UNKNOWN) 
	{
        result = rd_kafka_topic_conf_set(mConfTopic, key.c_str(), value.c_str(),errStr, sizeof errStr);
    }
    return result;
}

bool Config::getConf(const std::string &key, std::string &value) 
{
    const char **confStr = NULL;
    size_t size = 0;

    //Search global configuration.
    confStr = rd_kafka_conf_dump(mConfGlobal, &size);
    size_t i = 0;
    while (i < size) {
        if (key == confStr[i]) {
            value = confStr[i+1];
            return true;
        }
        i += 2;
    }

    //Search topic configuration.
    confStr = rd_kafka_topic_conf_dump(mConfTopic, &size);
    i = 0;
    while (i < size) {
        if (key == confStr[i]) {
            value = confStr[i+1];
            return true;
        }
        i += 2;
    }
    return false;
}

void Config::dumpConf() {
}

rd_kafka_conf_t* Config::getGlobalConf() {
    return mConfGlobal;
}

rd_kafka_topic_conf_t* Config::getTopicConf() {
    return mConfTopic;
}

rd_kafka_conf_t* Config::dupGlobalConf() {
    return rd_kafka_conf_dup(mConfGlobal);
}

rd_kafka_topic_conf_t* Config::dupTopicConf() {
    return rd_kafka_topic_conf_dup(mConfTopic);
}

}

