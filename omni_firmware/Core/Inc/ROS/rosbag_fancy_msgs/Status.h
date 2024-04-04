#ifndef _ROS_rosbag_fancy_msgs_Status_h
#define _ROS_rosbag_fancy_msgs_Status_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "rosbag_fancy_msgs/TopicStatus.h"

namespace rosbag_fancy_msgs
{

  class Status : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _status_type;
      _status_type status;
      typedef const char* _bagfile_type;
      _bagfile_type bagfile;
      typedef uint64_t _bytes_type;
      _bytes_type bytes;
      typedef uint64_t _free_bytes_type;
      _free_bytes_type free_bytes;
      typedef float _bandwidth_type;
      _bandwidth_type bandwidth;
      uint32_t topics_length;
      typedef rosbag_fancy_msgs::TopicStatus _topics_type;
      _topics_type st_topics;
      _topics_type * topics;
      enum { STATUS_PAUSED =  0 };
      enum { STATUS_RUNNING =  1 };

    Status():
      header(),
      status(0),
      bagfile(""),
      bytes(0),
      free_bytes(0),
      bandwidth(0),
      topics_length(0), st_topics(), topics(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      uint32_t length_bagfile = strlen(this->bagfile);
      varToArr(outbuffer + offset, length_bagfile);
      offset += 4;
      memcpy(outbuffer + offset, this->bagfile, length_bagfile);
      offset += length_bagfile;
      *(outbuffer + offset + 0) = (this->bytes >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->bytes >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->bytes >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->bytes >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->bytes >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->bytes >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->bytes >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->bytes >> (8 * 7)) & 0xFF;
      offset += sizeof(this->bytes);
      *(outbuffer + offset + 0) = (this->free_bytes >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->free_bytes >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->free_bytes >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->free_bytes >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->free_bytes >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->free_bytes >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->free_bytes >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->free_bytes >> (8 * 7)) & 0xFF;
      offset += sizeof(this->free_bytes);
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
      *(outbuffer + offset + 0) = (this->topics_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->topics_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->topics_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->topics_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->topics_length);
      for( uint32_t i = 0; i < topics_length; i++){
      offset += this->topics[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->status);
      uint32_t length_bagfile;
      arrToVar(length_bagfile, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_bagfile; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_bagfile-1]=0;
      this->bagfile = (char *)(inbuffer + offset-1);
      offset += length_bagfile;
      this->bytes =  ((uint64_t) (*(inbuffer + offset)));
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->bytes |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->bytes);
      this->free_bytes =  ((uint64_t) (*(inbuffer + offset)));
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->free_bytes |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->free_bytes);
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
      uint32_t topics_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      topics_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      topics_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      topics_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->topics_length);
      if(topics_lengthT > topics_length)
        this->topics = (rosbag_fancy_msgs::TopicStatus*)realloc(this->topics, topics_lengthT * sizeof(rosbag_fancy_msgs::TopicStatus));
      topics_length = topics_lengthT;
      for( uint32_t i = 0; i < topics_length; i++){
      offset += this->st_topics.deserialize(inbuffer + offset);
        memcpy( &(this->topics[i]), &(this->st_topics), sizeof(rosbag_fancy_msgs::TopicStatus));
      }
     return offset;
    }

    virtual const char * getType() override { return "rosbag_fancy_msgs/Status"; };
    virtual const char * getMD5() override { return "947e6f4818d85a50b2f28a48a2711b80"; };

  };

}
#endif
