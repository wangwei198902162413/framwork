#ifndef _KAFKACPP_CONSUMER_H_
#define _KAFKACPP_CONSUMER_H_

#include <sys/types.h>
#include <set>
#include <string>
#include "DmsgWorker.h"
//#include "ConsumerActor.h"


namespace dmsg {

extern const int offsetBeginning;
extern const int offsetEnd;

typedef void (*consumeMessageCallback)(char *buf, size_t len, int64_t offset);

//!	消费者类
/*!
	实现消费者逻辑，提供给外部应用使用
*/
class Consumer : public DmsgWorker {
public:
	/*!
		构造函数
		\param[in] conf 消费者配置
	*/
    Consumer(Config* conf = NULL);

	/*!
		析构函数，调用rd_kafka_consume_stop方法
	*/
    virtual ~Consumer();

	/*!
		获取指定分区内消息当前的最大偏移量
		\param[in] topic 指定消费的主题
		\param[in] partition 指定消费的分区
		\return >0 - 偏移量， -1 - 失败
	*/
	int64_t getPartitionOffset(std::string& topic, int partition, int timeout = 500);

	/*!
		获取指定分区内消息当前的最大偏移量
		\param[in] topic 指定消费的主题
		\param[in] partition 指定消费的分区
		\param[in] offset 指定消费开始的偏移量
	*/
	void setConsumeStartOffset(std::string& topic, int partition, int64_t offset);

	/*!
		消费单个消息(从kafka获取单个消息)
		\param[in] topic 指定消费的主题
		\param[in] partition 指定消费的分区
		\param[in] offset 指定消费开始的偏移量
		\param[in] timeout 指定超时时间，超过这个时间未得到消息，函数将返回失败
		\param[in] consumeMessageCallback 获取消息后会调用该回调函数
		\return 0 - 成功， -1 - 失败
	*/
	int consumeOneMessage(std::string& topic, int partition, int64_t offset, int timeout, consumeMessageCallback cb);

	/*!
		消费单个消息(从kafka获取单个消息)
		\param[in] topic 指定消费的主题
		\param[in] partition 指定消费的分区
		\param[in] offset 指定消费开始的偏移量
		\param[in] timeout 指定超时时间，超过这个时间未得到消息，函数将返回失败
		\param[out] buf 获取的消息地址(如果获取消息成功，该地址的内存需要外部应用通过free释放)
		\param[out] len 获取的消息长度
		\param[out] offset 当前消息的偏移量
		\return 0 - 成功， -1 - 失败
	*/
	int consumeOneMessage(std::string& topic, int partition, int64_t offset, int timeout, char*& buf, size_t& len, int64_t& mOffset);

	//! TODO, 未测， 暂不可用
    //int consumeOneMessage(ConsumerActor *actor);

	/*!
		重置消费状态
		消费消息都是连续的，当已经开始消费消息时，要从另外的位置重新开始消费，要调用此接口
		\param[in] topic 指定消费的主题
		\param[in] partition 指定消费的分区
		\return 0 - 成功， -1 - 失败
	*/
	void resetConsumeStat(std::string& topic, int partition);
	
private:
	/*!
		确保主题结构已经分配，并已经调用rd_kafka_consume_start(仅供内部使用)
	*/
	int ensureConsumeStart(std::string& topic, int partition, int64_t offset, pDmsgHandle& rkt);

protected:
	typedef std::map<pDmsgHandle, std::set<int> > TopicPatitionMap;
	//ConsumerActor *actor_;				//!< 未测，暂不可用
	TopicPatitionMap topicPartitions_;	//!< 主题和分区对应关系
	
};

}

#endif


