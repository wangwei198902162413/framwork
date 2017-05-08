#include "gtest/gtest.h"
#include "KafkaHandle.h"

class KafkaHandleTest: public ::testing::Test {
public:
    KafkaHandleTest() {
    }
    virtual void SetUp() {
        
    }
    virtual void TearDown() {
    }

};

TEST_F(KafkaHandleTest,testGetConf) {
	dmsg::KafkaHandle h(RD_KAFKA_PRODUCER);
	EXPECT_TRUE(NULL != h.getConfig());
}

TEST_F(KafkaHandleTest,testGetRdKafka) {
	dmsg::KafkaHandle h(RD_KAFKA_PRODUCER);
	EXPECT_TRUE(NULL != h.getRdKafka());
}

TEST_F(KafkaHandleTest,testAddBroker) {
	dmsg::KafkaHandle h(RD_KAFKA_PRODUCER);
	
	std::string brokers = "127.0.0.1:9092,127.0.0.1:9093";
	int result = h.addBroker(brokers);
	EXPECT_EQ(2, result);
}

/*TEST_F(KafkaHandleTest,testAddZkServer) {

}*/
