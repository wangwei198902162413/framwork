#ifndef _KAFKACPP_PRODUCER_H_
#define _KAFKACPP_PRODUCER_H_

//#include <cstdint>////
#include <sys/types.h>
#include <string>
#include "DmsgWorker.h"
#include "Partitioner.h"

namespace dmsg {
class Config;

//!	生产者类
/*!
	实现生产者逻辑，提供给外部应用使用
*/
class Producer : public DmsgWorker {
public:
	/*!
		构造函数
		\param[in] conf 生产者配置
	*/
    Producer(Config* conf = NULL);

	/*!
		析构函数，释放partitioner
	*/
    virtual ~Producer();

	/*!
		生产单个消息(发送单个消息到kafka)
		\param[in] topic 指定主题名称
		\param[in] message 消息地址
		\param[in] len 消息长度
		\param[in] partition 指定分区(默认随机分配)
		\return 0 - 成功， -1 - 失败
	*/
    bool produceMessage(const std::string &topic, char *message, size_t len, int32_t partition = -1);

	/*!
		生产单个消息(发送单个消息到kafka)
		\param[in] topic 指定主题
		\param[in] message 消息地址
		\param[in] len 消息长度
		\param[in] partition 指定分区(默认随机分配)
		\return 0 - 成功， -1 - 失败
	*/
    bool produceMessage(pDmsgHandle topic, char *message, size_t len, int32_t partition = -1);

	/*!
		设置分区策略对象
		析构时分区策略对象也会被删除
		\param[in] partitioner 分区策略对象，需实现AssignPartition方法
	*/
	inline void setPartitioner(Partitioner* partitioner) {
		if (partitioner_) {
			delete partitioner_;
		}
		partitioner_ = partitioner;
	}
	
protected:
	Partitioner* partitioner_;	//!< 分区策略对象，用于在不指定分区时选择发送的分区

};

}

#endif

