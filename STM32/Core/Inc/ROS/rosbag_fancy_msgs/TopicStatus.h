#ifndef _ROS_rosbag_fancy_msgs_TopicStatus_h
#define _ROS_rosbag_fancy_msgs_TopicStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rosbag_fancy_msgs
{

  class TopicStatus : public ros::Msg
  {
    public:
      typedef const char* _name_type;
      _name_type name;
      typedef uint32_t _publishers_type;
      _publishers_type publishers;
      typedef uint64_t _messages_type;
      _messages_type messages;
      typedef uint64_t _messages_in_current_bag_type;
      _messages_in_current_bag_type messages_in_current_bag;
      typedef float _rate_type;
      _rate_type rate;
      typedef uint64_t _bytes_type;
      _bytes_type bytes;
      typedef float _bandwidth_type;
      _bandwidth_type bandwidth;

    TopicStatus():
      name(""),
      publishers(0),
      messages(0),
      messages_in_current_bag(0),
      rate(0),
      bytes(0),
      bandwidth(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      varToArr(outbuffer + offset, length_name);
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      *(outbuffer + offset + 0) = (this->publishers >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->publishers >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->publishers >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->publishers >> (8 * 3)) & 0xFF;
      offset += sizeof(this->publishers);
      *(outbuffer + offset + 0) = (this->messages >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->messages >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->messages >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->messages >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->messages >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->messages >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->messages >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->messages >> (8 * 7)) & 0xFF;
      offset += sizeof(this->messages);
      *(outbuffer + offset + 0) = (this->messages_in_current_bag >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->messages_in_current_bag >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->messages_in_current_bag >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->messages_in_current_bag >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->messages_in_current_bag >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->messages_in_current_bag >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->messages_in_current_bag >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->messages_in_current_bag >> (8 * 7)) & 0xFF;
      offset += sizeof(this->messages_in_current_bag);
      union {
        float real;
        uint32_t base;
      } u_rate;
      u_rate.real = this->rate;
      *(outbuffer + offset + 0) = (u_rate.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rate.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rate.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rate.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rate);
      *(outbuffer + offset + 0) = (this->bytes >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->bytes >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->bytes >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->bytes >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->bytes >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->bytes >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->bytes >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->bytes >> (8 * 7)) & 0xFF;
      offset += sizeof(this->bytes);
      union {
        float real;
        uint32_t base;
      } u_bandwidth;
      u_bandwidth.real = this->bandwidth;
      *(outbuffer + offset + 0) = (u_bandwidth.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_bandwidth.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_bandwidth.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_bandwidth.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->bandwidth);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_name;
      arrToVar(length_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      this->publishers =  ((uint32_t) (*(inbuffer + offset)));
      this->publishers |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->publishers |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->publishers |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->publishers);
      this->messages =  ((uint64_t) (*(inbuffer + offset)));
      this->messages |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->messages |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->messages);
      this->messages_in_current_bag =  ((uint64_t) (*(inbuffer + offset)));
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->messages_in_current_bag |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->messages_in_current_bag);
      union {
        float real;
        uint32_t base;
      } u_rate;
      u_rate.base = 0;
      u_rate.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rate.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rate.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rate.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rate = u_rate.real;
      offset += sizeof(this->rate);
      this->bytes =  ((uint64_t) (*(inbuffer + offset)));
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->bytes);
      union {
        float real;
        uint32_t base;
      } u_bandwidth;
      u_bandwidth.base = 0;
      u_bandwidth.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_bandwidth.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_bandwidth.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_bandwidth.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->bandwidth = u_bandwidth.real;
      offset += sizeof(this->bandwidth);
     return offset;
    }

    virtual const char * getType() override { return "rosbag_fancy_msgs/TopicStatus"; };
    virtual const char * getMD5() override { return "ab1d1b0e9b845daddfffb9265d5bb87c"; };

  };

}
#endif
