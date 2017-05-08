#ifndef TAIR_PACKET_SET_NS_ATTR_PACKET_H
#define TAIR_PACKET_SET_NS_ATTR_PACKET_H
#include "base_packet.hpp"

namespace tair
{
  class request_set_ns_attr:public base_packet
  {
  public:
    request_set_ns_attr()
    {
      setPCode(TAIR_REQ_SET_NS_ATTR_PACKET);
      server_flag = 0;
      attr = NULL;
      attr_len = 0;
      value = NULL;
      value_len = 0;
    }

    request_set_ns_attr(request_set_ns_attr & packet)
    {
      setPCode(packet.getPCode ());
      if (packet.attr != NULL && packet.attr_len != 0) {
          attr = new char[attr_len];
          if (attr == NULL) {
              return;
          }
          memcpy(attr, packet.attr, packet.attr_len);
          attr_len = packet.attr_len;
      }
      if (packet.value != NULL && packet.value_len != 0) {
          value = new char[value_len];
          if (value == NULL) {
              return;
          }
          memcpy(value, packet.value, packet.value_len);
          value_len = packet.value_len;
      }
    }

    ~request_set_ns_attr()
    {
        if (attr) {
            delete []attr;
        }
        if (value) {
            delete []value;
        }
    }

    bool encode(tbnet::DataBuffer * output)
    {
      CREATE_HEADER;

      PUT_CSTR_TO_BUFFER(output, attr, attr_len);
      PUT_CSTR_TO_BUFFER(output, value, value_len);

      return true;
    }

    bool decode (tbnet::DataBuffer * input, tbnet::PacketHeader * header)
    {

      HEADER_VERIFY;

      GET_CSTR_FROM_BUFFER(input, attr, attr_len);
      GET_CSTR_FROM_BUFFER(input, value, value_len);

      return true;
    }

  public:
    uint16_t area;
    char* attr;
    int attr_len;
    char* value;
    int value_len;
    /* not used, but for template function */
    data_entry key;
  };
}

#endif
