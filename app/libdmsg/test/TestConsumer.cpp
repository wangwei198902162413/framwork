#include <sstream>
#include "rdkafka.h"
#include "gtest/gtest.h"
#include "Define.h"
#include "Config.h"
#include "Consumer.h"
#include "Producer.h"

void consumeMessage(char *buf, size_t len, int64_t offset) {
    std::string strMsg(buf, len);
    std::cerr << "Received message: " << strMsg << ", " << offset << std::endl;
}

static int count = 0;
void consumeMessagePerformance(char *buf, size_t len, int64_t offset) {
    count++;
}


class ConsumerTest: public ::testing::Test {
public:
    ConsumerTest() {
    }
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }

};

/*
class TestConsumerActor : public dmsg::ConsumerActor {
public:
    TestConsumerActor() {
    }

    ~TestConsumerActor() {
    }

    virtual void getTopicAndPartition(std::string& topic, int& partitions) {
        topic = CONSUMER_TOPIC;
        partitions = 0;
    }

    virtual void processMessage(rd_kafka_message_t *msg) {
        std::string strMsg(static_cast<const char *>(msg->payload), (msg->len));
        std::cerr << "Received message from partition: " << msg->partition << ", " << strMsg << std::endl;
    }
};
*/

TEST_F(ConsumerTest,testInitWithCorrectBroker) {
    dmsg::Consumer c;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));
}

TEST_F(ConsumerTest, testConsumeSinglePartition) {
    dmsg::Config* config = new dmsg::Config();
    std::string key("offset.store.path");
    std::string value("./tmp");
    config->setConf(key, value);
    key = "offset.store.sync.interval.ms";
    value = "0";
    config->setConf(key, value);

    
    dmsg::Consumer c(config);

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));

    std::string topic(CONSUMER_TOPIC);
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage));
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, 0, 500, consumeMessage));
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, 100, 500, consumeMessage));
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, -200, 500, consumeMessage));
    EXPECT_EQ(-1, c.consumeOneMessage(topic, 0, 800, 500, consumeMessage));
}

TEST_F(ConsumerTest, testConsumeSingleMessage) {
    dmsg::Consumer c;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));

    std::string topic(CONSUMER_TOPIC);
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage));

    char *buf = NULL;
    size_t len = 0;
    int64_t offset = -1;
    ASSERT_EQ(0, c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 500, buf, len, offset));
    std::string message(buf, len);
    EXPECT_STREQ("msg1", message.c_str());
    EXPECT_EQ(1, offset);
}

TEST_F(ConsumerTest, testGetPartitionOffset) {
    dmsg::Consumer c;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));

    std::string topic(CONSUMER_TOPIC);
    EXPECT_EQ(3, c.getPartitionOffset(topic, 0));

}

TEST_F(ConsumerTest, testConsumeMultipleTopicPartition) {
    dmsg::Consumer c;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));

    std::string topic(CONSUMER_TOPIC);
    EXPECT_EQ(0, c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage));
    EXPECT_EQ(0, c.consumeOneMessage(topic, 1, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage));
    while (c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage) == 0);

    std::string topic2(CONSUMER_TOPIC2);
    EXPECT_EQ(0, c.consumeOneMessage(topic2, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage));
    while (c.consumeOneMessage(topic2, 0, RD_KAFKA_OFFSET_BEGINNING, 500, consumeMessage) == 0);
}

TEST_F(ConsumerTest, testConsumePerformance) {
    dmsg::Consumer c;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(c.init(brokerList, zookeeperList));

    std::string topic(PERFORMANCE_TOPIC_SMALL);
    while (c.consumeOneMessage(topic, 0, RD_KAFKA_OFFSET_BEGINNING, 2000, consumeMessagePerformance) == 0);
    std::cerr << count << std::endl;
}

