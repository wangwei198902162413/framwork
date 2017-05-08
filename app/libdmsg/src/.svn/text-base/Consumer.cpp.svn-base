#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
//#include "rdkafka.h"
#include "KafkaHandle.h"
#include "Consumer.h"


namespace dmsg {

const int offsetBeginning = -2;
const int offsetEnd = -1;

/*
void consumeCallback(rd_kafka_message_t *rkmessage, void *opaque) {
    std::cerr << "MSG: " << rkmessage->err << std::endl;
    ConsumerActor* actor = static_cast<ConsumerActor*>(opaque);
    actor->processMessage(rkmessage);
}
*/


Consumer::Consumer(Config* conf) : DmsgWorker(RD_KAFKA_CONSUMER, conf) {
}

Consumer::~Consumer() {
    TopicPatitionMap::iterator itr=topicPartitions_.begin();
    for (; itr!=topicPartitions_.end(); ++itr ) {
        for (std::set<int>::iterator pItr=itr->second.begin(); pItr!=itr->second.end(); pItr++) {
            rd_kafka_consume_stop((rd_kafka_topic_t*)(itr->first), *pItr);
        }
    }

/*
    if (actor_ != NULL) {
        delete actor_;
        actor_ = NULL;
    }
*/
}

//This method can be called more than once but the offset will only be set in the first time.
int Consumer::ensureConsumeStart(std::string& topic, int partition, int64_t offset, pDmsgHandle& rkt) {
    rkt = getTopicHandle(topic, false);
    if (rkt == NULL) {
        rkt = getTopicHandle(topic);
        if (rkt == NULL) {
            //std::cerr << "Failed to get topic structure." << std::endl;
            return -1;
        }
        if (rd_kafka_consume_start((rd_kafka_topic_t*)rkt, partition, offset) != 0) {
            //std::cerr << "Failed to start consume: [" << topic << ":" << partition << "]" << std::endl;
            return -1;
        }
        sleep(2);
    } else {
        if (topicPartitions_[rkt].find(partition) == topicPartitions_[rkt].end()) {
            rd_kafka_consume_stop((rd_kafka_topic_t*)rkt, partition);
            if (rd_kafka_consume_start((rd_kafka_topic_t*)rkt, partition, offset) != 0) {
                //std::cerr << "Failed to start consume: [" << topic << ":" << partition << "]" << std::endl;
                return -1;
            }
            sleep(2);
        } else {
            return 0;
        }
    }

    topicPartitions_[rkt].insert(partition);

    return 0;
}

void Consumer::setConsumeStartOffset(std::string& topic, int partition, int64_t offset) {
    pDmsgHandle rkt = NULL;
    ensureConsumeStart(topic, partition, offset, rkt);
}

int Consumer::consumeOneMessage(std::string& topic, int partition, int64_t offset, int timeout, consumeMessageCallback cb) {
    rd_kafka_topic_t* rkt = NULL;
    pDmsgHandle h = NULL;
    if (ensureConsumeStart(topic, partition, offset, h) != 0) {
        return -1;
    } else {
        rkt = (rd_kafka_topic_t*)h;
    }

    rd_kafka_message_t* msg = rd_kafka_consume(rkt, partition, timeout);
    if (msg != NULL && msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
        cb(static_cast<char*>(msg->payload), msg->len, msg->offset);
        rd_kafka_message_destroy(msg);
    } else {
        //TODO, record error log.
        if (msg == NULL) {
            //std::cerr << rd_kafka_err2str(rd_kafka_errno2err(errno)) << std::endl;
        } else {
            rd_kafka_message_destroy(msg);
    }
        return -1;
    }

    return 0;    
}

int64_t Consumer::getPartitionOffset(std::string& topic,
    int partition, int timeout/* = 500 */) {
    rd_kafka_topic_t* rkt = NULL;    
    rkt = rd_kafka_topic_new(kafka_->getRdKafka(), topic.c_str(),
            kafka_->getConfig()->dupTopicConf());
    if (rkt == 0) {
        return -998;
    }

    if (rd_kafka_consume_start(rkt, partition, RD_KAFKA_OFFSET_END) != 0) {
        //std::cerr << "Failed to start consume: [" << topic << ":" << partition << "]" << std::endl;
        return -997;
    }

    int64_t offset = -1;
    rd_kafka_message_t* msg = rd_kafka_consume(rkt, partition, timeout);
    if (NULL == msg) {
        return -999;
    }
    else if (RD_KAFKA_RESP_ERR__PARTITION_EOF == msg->err) {   
        offset = msg->offset - 1;        
    } else {
        return msg->err < 0 ? msg->err : -msg->err;
    }

    rd_kafka_message_destroy(msg);

    rd_kafka_consume_stop(rkt, partition);

    return offset;
}

int Consumer::consumeOneMessage(std::string &topic, int partition, int64_t offset, int timeout,
        char*&buf, size_t& len, int64_t& mOffset) {
    rd_kafka_topic_t* rkt = NULL;
    pDmsgHandle h = NULL;
    if (ensureConsumeStart(topic, partition, offset, h) != 0) {
        return -1;
    } else {
        rkt = (rd_kafka_topic_t*)h;
    }

    rd_kafka_message_t* msg = rd_kafka_consume(rkt, partition, timeout);
    if (NULL == msg) {
        return -1;
    }
    else if (msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
        buf = static_cast<char*>(malloc(msg->len));
        memcpy(buf, msg->payload, msg->len);
        len = msg->len;
        mOffset = msg->offset;
        rd_kafka_message_destroy(msg);
    } else {
        //TODO, record error log.
        if (msg == NULL) {
            //std::cerr << rd_kafka_err2str(rd_kafka_errno2err(errno)) << std::endl;
        }
        rd_kafka_message_destroy(msg);
        return -2;
    }

    return 0;
}

/*
int Consumer::consumeOneMessage(ConsumerActor *actor) {
    actor_ = actor;
    std::string topic = actor_->topic();
    int partition = actor_->partition();
    int64_t offset = actor_->offset();

    rd_kafka_topic_t* rkt = NULL;
    pDmsgHandle h;
    if (ensureConsumeStart(topic, partition, offset, h) != 0) {
        return -1;
    } else {
        rkt = (rd_kafka_topic_t*)h;
    }

    rd_kafka_message_t* msg = rd_kafka_consume(rkt, partition, actor_->timeout());
    if (msg != NULL && msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
        actor->processMessage(msg);
        rd_kafka_message_destroy(msg);
    } else {
        //TODO, record error log.
        return -1;
    }

    return 0;
}
*/

void Consumer::resetConsumeStat(std::string& topic, int partition)
{
    pDmsgHandle h = getTopicHandle(topic, false);
    if (h == NULL) return;
    rd_kafka_topic_t* rkt = (rd_kafka_topic_t*)h;
    std::set<int>::iterator itr = topicPartitions_[rkt].find(partition);
    if (itr == topicPartitions_[rkt].end())
    {
        return;
    }
    else
    {
        rd_kafka_consume_stop(rkt, partition);
        topicPartitions_[rkt].erase(itr);
    }
    
}

/*int Consumer::startConsume(ConsumerActor *actor) {
    actor_ = actor;
    isRun_ = true;
    std::string topic;
    std::vector<int> partitions;
    actor_->getTopicAndPartition(topic, partitions);

    rd_kafka_topic_t* rkt = getTopicHandle(topic);
    if (rkt == NULL) {
        //std::cerr << "Failed to get topic structure." << std::endl;
        return -1;
    }

    std::vector<int>::iterator itr = partitions.begin();
    while (isRun_) {
        if (rd_kafka_consume_start(rkt, *itr, RD_KAFKA_OFFSET_STORED) != 0) {
            itr++;
            continue;
        }
        
        rd_kafka_message_t* msg = rd_kafka_consume(rkt, *itr, actor_->getPollInterval());
        if (msg != NULL && msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
            actor_->processMessage(msg);
            rd_kafka_message_destroy(msg);
        } else {
            //TODO, record error log.
            itr++;
            rd_kafka_consume_stop(rkt, *itr);
            continue;
        }
        //if (rd_kafka_consume_callback(rkt, *itr, actor_->getPollInterval(), consumeCallback, actor_) == -1) {
            //TODO, recrod error log;
        //}
        rd_kafka_consume_stop(rkt, *itr);
        //std::cerr << "consume once: [" << topic << ":" << *itr << "]" << std::endl;
        itr++;
        if (itr == partitions.end()) {
            itr = partitions.begin();
        }
    }
    
    return 0;
}

void Consumer::stopConsume() {
    isRun_ = false;
}*/


}
