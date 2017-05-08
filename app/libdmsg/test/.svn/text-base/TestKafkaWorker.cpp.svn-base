#include "gtest/gtest.h"
#include "DmsgWorker.h"

#define BROKER_LIST  "127.0.0.1:9092"

class KafkaWorkerTest: public ::testing::Test {
public:
    KafkaWorkerTest() {
    }
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

TEST_F(KafkaWorkerTest,testGetPartitionCount) {
    dmsg::DmsgWorker w(1);

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    EXPECT_TRUE(w.init(brokerList, zookeeperList));

    std::string topic("unknown_topic");
    int cnt = w.getPartitionCount(topic);
    EXPECT_EQ(0, cnt);

    topic = "test_consumer";
    cnt = w.getPartitionCount(topic);
    EXPECT_EQ(10, cnt);
}


