#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "qconf_log.h"
//qconf log 不能分文件写日志，只能写在一个文件中
using namespace std;
int main(int argc, char const *argv[])
{	
	const string QCONF_LOG_FMT("/1.log");
	string agent_dir(".");
	qconf_set_log_level(QCONF_LOG_ERR);
	string log_fmt = agent_dir + QCONF_LOG_FMT;
	qconf_log_init(log_fmt, 2);
	while(1)
	{
		LOG_INFO("test\n");
		sleep(0.01);
	}

	return 0;
}