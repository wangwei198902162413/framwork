/*
 * (C) 2007-2010 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * common response packet which only includes return code
 *
 * Version: $Id: response_return_packet.hpp 28 2010-09-19 05:18:09Z ruohai@taobao.com $
 *
 * Authors:
 *   ruohai <ruohai@taobao.com>
 *     - initial release
 *
 */
#ifndef TAIR_PACKET_RESPONSE_SIMPLE_PACKET_H
#define TAIR_PACKET_RESPONSE_SIMPLE_PACKET_H
#include "base_packet.hpp"
namespace tair {
  class response_simple_packet:public base_packet
  {
  public:
    response_simple_packet ()
    {
      config_version = 0;
      setPCode (TAIR_RESP_SIMPLE_PACKET);
      code = 0;
      buff_len = 0;
      buff = NULL;
    }

    ~response_simple_packet ()
    {
        if (buff) {
            delete []buff;
        }
    }

    bool encode (tbnet::DataBuffer * output)
    {
      output->writeInt32 (config_version);
      output->writeInt32 (code);

      PUT_CSTR_TO_BUFFER(output, buff, buff_len);

      return true;
    }

    bool decode (tbnet::DataBuffer * input, tbnet::PacketHeader * header)
    {
      GETKEY_FROM_INT32(input, config_version);
      GETKEY_FROM_INT32(input, code);

      GET_CSTR_FROM_BUFFER(input, buff, buff_len);

      return true;
    }

    void set_meta (uint32_t config_version, uint32_t code)
    {
      this->config_version = config_version;
      this->code = code;
    }

    void set_code (int cde)
    {
      code = cde;
    }

    int get_code ()
    {
      return code;
    }

    void set_data(char* data, const int data_len)
    {
      buff = data;
      buff_len = data_len;
    }

    void set_version (uint16_t ver)
    {
      version = ver;
    }

  public:
    uint32_t config_version;

    char* buff;
    int buff_len;
  private:
    int32_t code;
    uint16_t version;
  private:
    response_simple_packet (const response_simple_packet &);
  };
}
#endif
