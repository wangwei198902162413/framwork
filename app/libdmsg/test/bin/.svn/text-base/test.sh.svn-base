#!/bin/bash

ZOOKEEPER_SERVER=127.0.0.1:2181
KAFKA_DIR=/root/code/kafka_2.9.2-0.8.1.1
PRODUCER_TEST_TOPIC=test_producer
CONSUMER_TEST_TOPIC=test_consumer
CONSUMER_TEST_TOPIC2=test_consumer2
PERFOMANCE_TEST_TOPIC_SMALL=performance_small
PERFOMANCE_TEST_TOPIC_BIG=performance_big

prepare_topic() {
	topic = $1
	partitions = $2
	${KAFKA_DIR}/bin/kafka-run-class.sh kafka.admin.DeleteTopicCommand --topic ${topic} --zookeeper ${ZOOKEEPER_SERVER}
	${KAFKA_DIR}/bin/kafka-topics.sh --create --topic ${topic} --replication-factor 1 --partitions ${partitions} --zookeeper ${ZOOKEEPER_SERVER}
}

prepare_topic ${PRODUCER_TEST_TOPIC} 1
prepare_topic ${CONSUMER_TEST_TOPIC} 1
prepare_topic ${CONSUMER_TEST_TOPIC2} 1
prepare_topic ${PERFOMANCE_TEST_TOPIC_SMALL} 10
prepare_topic ${PERFOMANCE_TEST_TOPIC_BIG} 10
