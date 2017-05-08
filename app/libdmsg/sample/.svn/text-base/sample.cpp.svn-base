#include <iostream>
#include <string>

#include "Producer.h"
#include "Consumer.h"

const std::string BROKER_LIST("10.20.16.83:9093");
const std::string SAMPLE_TOPIC("test_sync");

void produce()
{
    dmsg::Producer p;

    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    if (!p.init(brokerList, zookeeperList))
    {
        std::cerr << "Initialize producer failed." << std::endl;
    }

    std::string message("normal message");
    std::string topic(SAMPLE_TOPIC);
    bool ret = p.produceMessage(topic, const_cast<char *>(message.c_str()),
            message.size(), 0);
    if (!ret)
    {
        std::cerr << "Produce message failed." << std::endl;
    }
    else
    {
        std::cout << "Send message: " << message << std::endl;
    }
}

void consume()
{
    dmsg::Consumer c;
    
    std::string brokerList = BROKER_LIST;
    std::string zookeeperList;
    if (!c.init(brokerList, zookeeperList))
    {
        std::cerr << "Initialize consumer failed." << std::endl;
    }

    std::string topic(SAMPLE_TOPIC);
    char *buf = NULL;
    size_t len = 0;
    int64_t offset = -1;
    int ret = c.consumeOneMessage(topic, 0, dmsg::offsetBeginning, 500, buf, len, offset);
    if (ret != 0)
    {
        std::cerr << "Receive message failed: " << std::endl;
    }
    else
    {
        std::string message(buf, len);
        std::cout << "Received message: " << message << std::endl;
    }
}

int main()
{
    produce();
    consume();
}

