#ifndef _KAFKACPP_PARTITIONER_H_
#define _KAFKACPP_PARTITIONER_H_


namespace dmsg {

//!	����ѡ�������
/*!
	�����߷�����Ϣ��kafkaʱ���Բ�ָ���������ò�������Ը���Ĭ�ϵ����ѡ����ԣ�Ӧ�ó�����̳и��࣬��ʵ��AssignPartition����
*/
class Partitioner {
public:
	virtual ~Partitioner() {
	}

	/*!
		����ѡ����Ժ�����Ӧ��ʵ�ָú����������ߵ��øú������ø÷�����֪���ͷ�����
		\return ������
	*/
	virtual int assignPartition() = 0;
};

}

#endif


