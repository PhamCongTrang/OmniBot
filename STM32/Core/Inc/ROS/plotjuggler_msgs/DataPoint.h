#ifndef _ROS_plotjuggler_msgs_DataPoint_h
#define _ROS_plotjuggler_msgs_DataPoint_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace plotjuggler_msgs
{

  class DataPoint : public ros::Msg
  {
    public:
      typedef uint16_t _name_index_type;
      _name_index_type name_index;
      typedef double _stamp_type;
      _stamp_type stamp;
      typedef double _value_type;
      _value_type value;

    DataPoint():
      name_index(0),
      stamp(0),
      value(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->name_index >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->name_index >> (8 * 1)) & 0xFF;
      offset += sizeof(this->name_index);
      union {
        double real;
        uint64_t base;
      } u_stamp;
      u_stamp.real = this->stamp;
      *(outbuffer + offset + 0) = (u_stamp.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_stamp.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_stamp.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_stamp.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_stamp.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_stamp.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_stamp.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_stamp.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->stamp);
      union {
        double real;
        uint64_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_value.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_value.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_value.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_value.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->value);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->name_index =  ((uint16_t) (*(inbuffer + offset)));
      this->name_index |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->name_index);
      union {
        double real;
        uint64_t base;
      } u_stamp;
      u_stamp.base = 0;
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_stamp.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->stamp = u_stamp.real;
      offset += sizeof(this->stamp);
      union {
        double real;
        uint64_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_value.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    virtual const char * getType() override { return "plotjuggler_msgs/DataPoint"; };
    virtual const char * getMD5() override { return "580ca7c40f92b9a6ab4b921c02ebcd28"; };

  };

}
#endif
