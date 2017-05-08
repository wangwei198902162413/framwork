#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Config.h"
#include "KafkaHandle.h"
#include "DmsgWorker.h"

namespace dmsg {

#define UPDATE_METADATA_TIMEOUT 5000

/*static void dr_cb(rd_kafka_t *rk, void *payload, size_t len,
        rd_kafka_resp_err_t err, void *opaque, void *msg_opaque) {
}

static void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t * rkmessage,
        void *opaque) {
    if (rkmessage->err) {
        std::cerr << "Failed to send message: "
            << rd_kafka_message_errstr(rkmessage) << std::endl;
    } else {
        std::string payload(static_cast<char*>(rkmessage->payload), rkmessage->len);
        std::cerr << "Succeeded to send message [" << payload << "]\n";
    }
}*/

DmsgWorker::DmsgWorker(int type, Config* conf) : kafka_(NULL) {
    if (conf == NULL) {
        conf = new Config();
    }
    kafka_ = new KafkaHandle(type, conf);
}

DmsgWorker::~DmsgWorker() {
    //Wait for all messages are sent out.
    while (rd_kafka_outq_len(kafka_->getRdKafka()) > 0) {
        rd_kafka_poll(kafka_->getRdKafka(), 50);
    }
    //Destroy topics.
    TopicList::iterator itr=topicList_.begin();
    for (; itr!=topicList_.end(); ++itr ) {
        rd_kafka_topic_destroy((rd_kafka_topic_t*)(itr->second));
    }
    //Destroy KafkaHandle.
    if (kafka_ != NULL) {
        delete kafka_;
    }
}

void* DmsgWorker::getTopicHandle(const std::string &topic, bool create) 
{
    TopicList::iterator itr = topicList_.find(topic);
    rd_kafka_topic_t *rkt = NULL;
    if (itr == topicList_.end()) {
        if (create) {
            rkt = rd_kafka_topic_new(kafka_->getRdKafka(), topic.c_str(),
                    kafka_->getConfig()->dupTopicConf());
            topicList_[topic] = rkt;
        } else {
            return NULL;
        }
    } else {
        rkt = (rd_kafka_topic_t*)(itr->second);
    }

    return rkt;
}


bool DmsgWorker::init(const std::string &broker_list,
        const std::string &zookeeper_list) {
    int ret = -1;
    if (broker_list.empty()) {
        return false;
    } else {
        ret = kafka_->addBroker(broker_list);
        if (ret <= 0) {
            return false;
        }
    }
    return true;
}

int DmsgWorker::getPartitionCount(const std::string &topic) {
    const struct rd_kafka_metadata *metadata;
    rd_kafka_topic_t* rkt = (rd_kafka_topic_t*)getTopicHandle(topic, false);	
    rd_kafka_resp_err_t ret = rd_kafka_metadata(kafka_->getRdKafka(), 0,
        rkt, &metadata, UPDATE_METADATA_TIMEOUT);
    if (ret != RD_KAFKA_RESP_ERR_NO_ERROR) {
        return 0;
    }

    int partitionCnt = 0;
    for (int i=0; i<metadata->topic_cnt; i++) {
        if (strcmp(topic.c_str(), metadata->topics[i].topic) == 0) {
            partitionCnt = metadata->topics[i].partition_cnt;
        }
    }

    rd_kafka_metadata_destroy(metadata);

    return partitionCnt;
}

}


