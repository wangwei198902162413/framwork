#ifndef _DMSG_DMSG_WORKER_H_
#define _DMSG_DMSG_WORKER_H_

#include <map>

namespace dmsg {

/*typedef enum {
	STOP,
	START
} PARTITION_CONSUMER_STAT;

typedef struct TopicMetaData_ {
	rd_kafka_topic_t* rkt;
	int64_t offset;
	PARTITION_CONSUMER_STAT stat;
} TopicMetaData;*/

class KafkaHandle;
class Config;
typedef void* pDmsgHandle;
//!	�����ߺ������߻���
/*!
	ʵ�������ߺ������߹�ͬ���߼����ṩ���ⲿӦ��ʹ��
*/
class DmsgWorker {
public:
	/*!
		���캯��
		\param[in] type kafka�ͻ��˵�����(�����߻�������)
		\param[in] conf ����������
	*/
	// ����kafka�������
    DmsgWorker(int type, Config* conf = NULL);

	/*!
		�����������ͷ�kafka��topic�Ľṹ
	*/
    virtual ~DmsgWorker();

	/*!
		��ʼ������
		\param[in] broker_list ָ��kafka����������Ͷ˿�
		\param[in] zookeeper_list ָ��zookeeper����������Ͷ˿�
		\return true - �ɹ��� false - ʧ��
	*/
	//ret = kafka_->addBroker(broker_list); add brokers
    bool init(const std::string &broker_list, const std::string &zookeeper_list);
	
	/*!
		��������(topic)���ƻ�ȡ��Ӧ�Ľṹָ��
		\param[in] topic ��������
		\param[in] create �Ƿ��Զ���������ṹ
		\return ��0 - kafka����ṹָ�룬 NULL - ʧ��
	*/
	// rd_kafka_topic_new ����topic
    pDmsgHandle getTopicHandle(const std::string &topic, bool create = true);

	/*!
		��������(topic)���ƻ�ȡ����(partition)������
		\param[in] topic ��������
		\return >0 - ���������� 0 - ʧ��
	*/
	int getPartitionCount(const std::string &topic);

protected:
    typedef std::map<std::string, pDmsgHandle> TopicList;// pDmsgHandle:rd_kafka_topic_t
    TopicList topicList_;	//!< �������ƺͽṹ��Ӧ��ϵ
    KafkaHandle *kafka_;	//!< kafka�������ָ��
};

}

#endif

