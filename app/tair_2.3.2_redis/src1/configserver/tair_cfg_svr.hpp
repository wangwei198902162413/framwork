/*
 * (C) 2007-2010 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: tair_cfg_svr.hpp 28 2010-09-19 05:18:09Z ruohai@taobao.com $
 *
 * Authors:
 *   Daoan <daoan@taobao.com>
 *
 */
#ifndef TAIR_CONFIG_SERVER_H
#define TAIR_CONFIG_SERVER_H
#include <string>
#include <ext/hash_map>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <tbsys.h>
#include <tbnet.h>
#include "server_conf_thread.hpp"
#include "packet_factory.hpp"

using namespace std;
using namespace __gnu_cxx;

namespace tair {
  namespace config_server {
  	//IServerAdapter 作为话单的处理父类
  	//IPacketQueueHandler 线程的queue队列处理父类
    class tair_config_server:public tbnet::IServerAdapter,
      public tbnet::IPacketQueueHandler {
    public:
      tair_config_server();
      ~tair_config_server();
      void start();
      void stop();
      // iserveradapter interface
        tbnet::IPacketHandler::HPRetCode handlePacket(tbnet::Connection *
                                                      connection,
                                                      tbnet::Packet * packet);
      // IPacketQueueHandler interface
      bool handlePacketQueue(tbnet::Packet * packet, void *args);

    private:
      int stop_flag;
	  //不同报文类型的加码解码处理方法工厂
      tair_packet_factory packet_factory;
	  //报文内容的加码解码识别
        tbnet::DefaultPacketStreamer packet_streamer;
	  //以线程运行的socket服务器
        tbnet::Transport packet_transport;
        tbnet::Transport heartbeat_transport;        
		//类似线程池的工作，创建若干线程对象，启动若干线程
		//packet处理，有一个packet queue，启动线程不断的从queue取数据交给_hadler处理
		//this线程不断的将packet放入queue
		//主要功能为缓存数据
		tbnet::PacketQueueThread task_queue_thread;
	  //run  读取group_conf表，insert 进入hashmap，通过心跳信息，不断的update hashmap
	  //提供查询hashmap表的api
      server_conf_thread my_server_conf_thread;

    private:
        inline int initialize();
      inline int destroy();
    };
  }
}
#endif
///////////////////////END
