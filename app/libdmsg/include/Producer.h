#ifndef _KAFKACPP_PRODUCER_H_
#define _KAFKACPP_PRODUCER_H_

//#include <cstdint>////
#include <sys/types.h>
#include <string>
#include "DmsgWorker.h"
#include "Partitioner.h"

namespace dmsg {
class Config;

//!	��������
/*!
	ʵ���������߼����ṩ���ⲿӦ��ʹ��
*/
class Producer : public DmsgWorker {
public:
	/*!
		���캯��
		\param[in] conf ����������
	*/
    Producer(Config* conf = NULL);

	/*!
		�����������ͷ�partitioner
	*/
    virtual ~Producer();

	/*!
		����������Ϣ(���͵�����Ϣ��kafka)
		\param[in] topic ָ����������
		\param[in] message ��Ϣ��ַ
		\param[in] len ��Ϣ����
		\param[in] partition ָ������(Ĭ���������)
		\return 0 - �ɹ��� -1 - ʧ��
	*/
    bool produceMessage(const std::string &topic, char *message, size_t len, int32_t partition = -1);

	/*!
		����������Ϣ(���͵�����Ϣ��kafka)
		\param[in] topic ָ������
		\param[in] message ��Ϣ��ַ
		\param[in] len ��Ϣ����
		\param[in] partition ָ������(Ĭ���������)
		\return 0 - �ɹ��� -1 - ʧ��
	*/
    bool produceMessage(pDmsgHandle topic, char *message, size_t len, int32_t partition = -1);

	/*!
		���÷������Զ���
		����ʱ�������Զ���Ҳ�ᱻɾ��
		\param[in] partitioner �������Զ�����ʵ��AssignPartition����
	*/
	inline void setPartitioner(Partitioner* partitioner) {
		if (partitioner_) {
			delete partitioner_;
		}
		partitioner_ = partitioner;
	}
	
protected:
	Partitioner* partitioner_;	//!< �������Զ��������ڲ�ָ������ʱѡ���͵ķ���

};

}

#endif

