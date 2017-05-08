#ifndef _KAFKACPP_PARTITIONER_H_
#define _KAFKACPP_PARTITIONER_H_


namespace dmsg {

//!	分区选择策略类
/*!
	生产者发送消息到kafka时可以不指定分区，该策略类可以覆盖默认的随机选择策略，应用程序需继承该类，并实现AssignPartition方法
*/
class Partitioner {
public:
	virtual ~Partitioner() {
	}

	/*!
		分区选择策略函数，应用实现该函数，生产者调用该函数利用该分区得知发送分区号
		\return 分区号
	*/
	virtual int assignPartition() = 0;
};

}

#endif


