#include <sys/time.h>
#include <syslog.h>
#include <iostream>
#include "KafkaHandle.h"
#include "Config.h"

namespace dmsg {

#define DESTROY_TIMEOUT 5000

KafkaHandle::KafkaHandle(int type, Config* conf)
    : mHandleKafka(NULL), mConf(conf) {
    if (NULL == mConf) {
        mConf = new Config();
    }
    char errStr[1024] = {0};
    rd_kafka_type_t handleType;
    if (type == 0) {
        handleType = RD_KAFKA_PRODUCER;
    } else {
        handleType = RD_KAFKA_CONSUMER;
    }
    mHandleKafka = rd_kafka_new(handleType, mConf->dupGlobalConf(),
            errStr, sizeof errStr);
	rd_kafka_set_logger(mHandleKafka, logger);
    rd_kafka_set_log_level(mHandleKafka, LOG_DEBUG);
}

KafkaHandle::~KafkaHandle() {
    if (mHandleKafka != NULL) {
        rd_kafka_destroy(mHandleKafka);
		mHandleKafka = NULL;
        int ret = rd_kafka_wait_destroyed(DESTROY_TIMEOUT);
        if (ret != 0) {
            //std::cerr << "Failed to wait rd_kafka_destroy" << std::endl;
        }
    }
    if (mConf != NULL) {
        delete mConf;
		mConf = NULL;
    }
}

void KafkaHandle::logger(const rd_kafka_t *rk, int level,
					const char *fac, const char *buf) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::cerr << tv.tv_sec << "." << tv.tv_usec / 1000 
		<< " KAFKACPP-" << level << "-" << fac << ": " 
		<< rd_kafka_name(rk) << ": " << buf << std::endl;
}

int KafkaHandle::addBroker(const std::string &brokerList) {
    return rd_kafka_brokers_add(mHandleKafka, brokerList.c_str());
}

void KafkaHandle::addZkServer(const std::string &zookeeperList) {
}

}

