#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <unistd.h>
//#include "gtest/gtest.h"
#include "qconf_msg.h"

using namespace std;
#define STATIC_MSG_QUEUE_KEY 0xaa1122ee
int main(int argc, char const *argv[])
{	
	int msgid;
	//发送一条消息
	int retCode = 0;
    key_t key = STATIC_MSG_QUEUE_KEY;
    string message;

    create_msg_queue(key, msgid);
    retCode = receive_msg(msgid, message);

    printf("%s\n", message.c_str());
	 
	return 0;
}
