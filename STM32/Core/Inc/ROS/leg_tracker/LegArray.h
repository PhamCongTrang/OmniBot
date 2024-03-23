#ifndef _ROS_leg_tracker_LegArray_h
#define _ROS_leg_tracker_LegArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "leg_tracker/Leg.h"

namespace leg_tracker
{

  class LegArray : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t legs_length;
      typedef leg_tracker::Leg _legs_type;
      _legs_type st_legs;
      _legs_type * legs;

    LegArray():
      header(),
      legs_length(0), st_legs(), legs(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->legs_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->legs_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->legs_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->legs_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->legs_length);
      for( uint32_t i = 0; i < legs_length; i++){
      offset += this->legs[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t legs_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      legs_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      legs_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      legs_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->legs_length);
      if(legs_lengthT > legs_length)
        this->legs = (leg_tracker::Leg*)realloc(this->legs, legs_lengthT * sizeof(leg_tracker::Leg));
      legs_length = legs_lengthT;
      for( uint32_t i = 0; i < legs_length; i++){
      offset += this->st_legs.deserialize(inbuffer + offset);
        memcpy( &(this->legs[i]), &(this->st_legs), sizeof(leg_tracker::Leg));
      }
     return offset;
    }

    virtual const char * getType() override { return "leg_tracker/LegArray"; };
    virtual const char * getMD5() override { return "9e932f47090c69cbf30f535c2ab02e32"; };

  };

}
#endif
