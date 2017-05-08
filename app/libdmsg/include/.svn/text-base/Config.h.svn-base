#ifndef _KAFKACPP_CONFIG_H_
#define _KAFKACPP_CONFIG_H_

extern "C" {
#include "rdkafka.h"
}
#include <string>

namespace dmsg {

//!	kafka配置类
/*!
	存放kafka客户端需要配置项，提供给Producer或Consumer使用。
*/
class Config {
public:
	//! 构造函数
    Config();

	//! 析构函数
    ~Config();

	/*!
		设置单个配置项的值
		\param[in] key 配置项的键(名称)
		\param[in] value 配置项的值
		\return 0 - 表示成功， 其他 - 表示失败
	*/
    rd_kafka_conf_res_t setConf(const std::string &key, const std::string &value);

	/*!
		获取单个配置项的值
		\param[in] key 配置项的键(名称)
		\param[out] value 配置项的值
		\return true - 表示成功， false - 表示失败
	*/
    bool getConf(const std::string &key, std::string &value);

	/*! TODO, 暂未实现
		在标准输出打印当前的配置项和相应的值
	*/
    void dumpConf();

	/*!
		获取全局配置的指针
		\return NULL - 失败， 其他 - 全局配置指针
	*/
    rd_kafka_conf_t* getGlobalConf();

	/*!
		获取Topic配置的指针
		\return NULL - 失败， 其他 - Topic配置指针
	*/
    rd_kafka_topic_conf_t* getTopicConf();

		/*!
		获取全局配置的指针，该指针指向内部全局配置的一份拷贝
		\return NULL - 失败， 其他 - Topic配置结构指针
	*/
    rd_kafka_conf_t* dupGlobalConf();

	/*!
		获取Topic配置的指针，该指针指向内部全局配置的一份拷贝
		\return NULL - 失败， 其他 - Topic配置结构指针
	*/
    rd_kafka_topic_conf_t* dupTopicConf();

private:
    rd_kafka_conf_t* mConfGlobal;		//!< 全局配置，构造函数中分配
    rd_kafka_topic_conf_t* mConfTopic;	//!< Topic配置，构造函数中分配
};

}

#endif



