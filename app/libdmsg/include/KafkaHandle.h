#ifndef _KAFKACPP_KAFKAHANDLE_H_
#define _KAFKACPP_KAFKAHANDLE_H_

#include <string>
#include "Config.h"

extern "C" {
#include "rdkafka.h"
}

namespace dmsg {

//! Provide high level APIs of kafka to application.
class KafkaHandle {
public:
    KafkaHandle(int type, Config* conf = NULL);

    ~KafkaHandle();

    int addBroker(const std::string &brokerList);

    void addZkServer(const std::string &zookeeperList);

    inline Config* getConfig() {
        return mConf;
    }

    inline rd_kafka_t* getRdKafka() {
        return mHandleKafka;
    }

protected:
	static void logger(const rd_kafka_t *rk, int level,
					const char *fac, const char *buf);

private:
    rd_kafka_t* mHandleKafka;
    //zhandle_t mZkHandle;
    Config* mConf;
};

}

#endif

