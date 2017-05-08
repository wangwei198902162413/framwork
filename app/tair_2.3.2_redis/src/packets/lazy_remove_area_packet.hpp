/*
 * (C) 2007-2010 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * this packet is for remove one particular area
 *
 * Version: $Id: remove_area_packet.hpp 28 2010-09-19 05:18:09Z ruohai@taobao.com $
 *
 * Authors:
 *   ruohai <ruohai@taobao.com>
 *     - initial release
 *
 */
#ifndef TAIR_PACKET_LAZY_REMOVE_AREA_PACKET_H
#define TAIR_PACKET_LAZY_REMOVE_AREA_PACKET_H
#include "base_packet.hpp"
namespace tair {
   class request_lazy_remove_area : public base_packet {
   public:

      request_lazy_remove_area()
      {
         setPCode(TAIR_REQ_LAZY_REMOVE_AREA_PACKET);
         area = 0;
         pwd_len = 0;
         password = NULL;
      }


      request_lazy_remove_area(request_lazy_remove_area &packet)
      {
         setPCode(TAIR_REQ_LAZY_REMOVE_AREA_PACKET);
         area = packet.area;
         if (packet.pwd_len > 0) {
             password = new char[packet.pwd_len + 1];
             if (password == NULL) {
                 pwd_len = 0;
             } else {
                 memcpy(password, packet.password, packet.pwd_len);
                 password[packet.pwd_len] = '\0';
                 pwd_len = packet.pwd_len;
             }
         }
      }


      ~request_lazy_remove_area()
      {
          if (password) {
              delete []password;
          }
      }


      bool encode(tbnet::DataBuffer *output)
      {
         output->writeInt32(area);
         PUT_CSTR_TO_BUFFER(output,password,pwd_len);
         return true;
      }


      bool decode(tbnet::DataBuffer *input, tbnet::PacketHeader *header)
      {
         HEADER_VERIFY;
         GET_CSTR_FROM_BUFFER(input,password,pwd_len);
         return true;
      }

   public:
      int area;
      int pwd_len;
      char* password;
   };
}
#endif
