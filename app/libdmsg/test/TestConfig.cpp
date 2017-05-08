#include "gtest/gtest.h"
#include "Config.h"

class ConfigTest: public ::testing::Test {
public:
    ConfigTest() : mConf(NULL) {
    }
    virtual void SetUp() {
        mConf = new dmsg::Config();
    }
    virtual void TearDown() {
        delete mConf;
        mConf = NULL;
    }

protected:
    dmsg::Config *mConf;
};

TEST_F(ConfigTest,testDefaultConf) {
    ASSERT_TRUE(mConf != NULL);

    std::string key = "client.id";
    std::string value;
    EXPECT_TRUE(mConf->getConf(key, value));
    EXPECT_STREQ(value.c_str(), "rdkafka");

    key = "topic.metadata.refresh.fast.interval.ms";
    EXPECT_TRUE(mConf->getConf(key, value));
    EXPECT_STREQ(value.c_str(), "250");


    key = "offset.store.method";
    EXPECT_TRUE(mConf->getConf(key, value));
    EXPECT_STREQ(value.c_str(), "file");
}

TEST_F(ConfigTest,testSetGlobalConf) {
    ASSERT_TRUE(mConf != NULL);

    std::string key = "client.id";
    std::string value = "client_test";
    EXPECT_EQ(RD_KAFKA_CONF_OK, mConf->setConf(key, value));
    std::string valueNew;
    EXPECT_TRUE(mConf->getConf(key, valueNew));
    EXPECT_STREQ(value.c_str(), valueNew.c_str());

}

TEST_F(ConfigTest,testSetTopicConf) {
    ASSERT_TRUE(mConf != NULL);

    std::string key = "offset.store.method";
    std::string value = "broker";
    EXPECT_EQ(RD_KAFKA_CONF_OK, mConf->setConf(key, value));
    std::string valueNew;
    EXPECT_TRUE(mConf->getConf(key, valueNew));
    EXPECT_STREQ(value.c_str(), valueNew.c_str());
}

TEST_F(ConfigTest,testSetUnknownConf) {
    ASSERT_TRUE(mConf != NULL);

    std::string key = "conf.unknown";
    std::string value = "unknown";
    EXPECT_EQ(RD_KAFKA_CONF_UNKNOWN, mConf->setConf(key, value));
}

TEST_F(ConfigTest,testSetInvalidConfValue) {
    ASSERT_TRUE(mConf != NULL);

    std::string key = "offset.store.method";
    std::string value = "invalidValue";
    EXPECT_EQ(RD_KAFKA_CONF_INVALID, mConf->setConf(key, value));
}

TEST_F(ConfigTest,testgetConfPointer) {
    ASSERT_TRUE(mConf != NULL);

    rd_kafka_conf_t *conf  = mConf->getGlobalConf();
    EXPECT_TRUE(NULL != conf);
    rd_kafka_topic_conf_t *conft  = mConf->getTopicConf();
    EXPECT_TRUE(NULL != conft);

    //rd_kafka_conf_destroy(conf);
    //rd_kafka_topic_conf_destroy(conft);
}

TEST_F(ConfigTest,testDupConfPointer) {
    ASSERT_TRUE(mConf != NULL);

    rd_kafka_conf_t *conf  = mConf->dupGlobalConf();
    EXPECT_TRUE(NULL != conf);
    EXPECT_TRUE(conf != mConf->getGlobalConf());
    rd_kafka_topic_conf_t *conft  = mConf->dupTopicConf();
    EXPECT_TRUE(NULL != conft);
    EXPECT_TRUE(conft != mConf->getTopicConf());

    rd_kafka_conf_destroy(conf);
    rd_kafka_topic_conf_destroy(conft);
}

