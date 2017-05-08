#include "redis/redislib.h"
#include "redis_server_cron.h"
#include "redis_db_context.h"
#include "scope_lock.h"

USE_NS

using namespace tbsys;

redis_server_cron::redis_server_cron(const redis_db_context &context)
    : context(context)
{
}

redis_server_cron::~redis_server_cron()
{
    stop();
    wait();
}


void redis_server_cron::run(CThread *thread, void *arg)
{
    int sleep_time_ms = 100;
    while (_stop == false)
    {
        updateLRUClock();
        int db_num = context.server->db_num;
        for(int index = 0; index < db_num; index++)
        {
            {
                scope_lock lock(&(context.server->db_mutexs[index]));
				
                serverCronByDb(context.server,index);
            }
        }
        context.server->cronloops++;
        cond.lock();
        if (!_stop)
            cond.wait(sleep_time_ms);
        cond.unlock();
    }
}

void redis_server_cron::stop()
{
    if (_stop)
        return ;
    cond.lock();
    _stop = true;
    cond.signal();
    cond.unlock();
}

