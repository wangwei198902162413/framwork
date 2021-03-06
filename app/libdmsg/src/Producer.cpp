#include <iostream>
#include <unistd.h>
#include <errno.h>
#include "Config.h"
#include "KafkaHandle.h"
#include "Producer.h"

namespace dmsg {

Producer::Producer(Config* conf) : DmsgWorker(RD_KAFKA_PRODUCER, conf), partitioner_(NULL) {
}

Producer::~Producer() {
    if (partitioner_) {
        delete partitioner_;
        partitioner_ = NULL;
    }
}

bool Producer::produceMessage(const std::string &topic, char *message, size_t len, int32_t partition) 
	{
    rd_kafka_topic_t* rkt = (rd_kafka_topic_t*)getTopicHandle(topic);

    if (partition == RD_KAFKA_PARTITION_UA && partitioner_ != NULL) {
        partition = partitioner_->assignPartition();
    }

    return produceMessage(rkt, message, len, partition);
}
//kakfa client proce
bool Producer::produceMessage(pDmsgHandle topic, char *message, size_t len, int32_t partition) 
	{
    while (rd_kafka_produce((rd_kafka_topic_t*)topic, partition,
            RD_KAFKA_MSG_F_COPY, message, len, NULL, 0, NULL) != 0) {
        if (RD_KAFKA_RESP_ERR__QUEUE_FULL != rd_kafka_errno2err(errno)) {
            std::cerr << rd_kafka_errno2err(errno) << std::endl;
            return false;
        }
        rd_kafka_poll(kafka_->getRdKafka(), 5);
    }

    return true;
}

}

