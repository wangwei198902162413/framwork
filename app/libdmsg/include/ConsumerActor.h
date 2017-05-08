#ifndef _KAFKACPP_CONSUMER_ACTOR_H_
#define _KAFKACPP_CONSUMER_ACTOR_H_

namespace dmsg {

class ConsumerActor {
public:
    ConsumerActor() : timeout_(50), stop_flag_(false) {
   	}

    virtual ~ConsumerActor() {
    }

	virtual std::string topic() = 0;

	virtual int partition() = 0;

	virtual int64_t offset() = 0;

	virtual void processMessage(rd_kafka_message_t *msg) = 0;

	inline void set_stop_flag(bool flag) {
		stop_flag_ = flag;
	}

	inline bool stop_flag() {
		return stop_flag_;
	}

	inline void set_timeout(int timeout) {
		timeout_ = timeout;
	}

	inline int timeout() {
		return timeout_;
	}

protected:
	int timeout_;  			//!< Poll interval between partitions.
	bool stop_flag_;;		//!< Stop consuming flag.
};

}

#endif



