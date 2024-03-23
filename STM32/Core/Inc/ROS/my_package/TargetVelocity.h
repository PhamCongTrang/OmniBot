#ifndef _ROS_my_package_TargetVelocity_h
#define _ROS_my_package_TargetVelocity_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace my_package
{

  class TargetVelocity : public ros::Msg
  {
    public:
      typedef double _target_velocity_type;
      _target_velocity_type target_velocity;

    TargetVelocity():
      target_velocity(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_target_velocity;
      u_target_velocity.real = this->target_velocity;
      *(outbuffer + offset + 0) = (u_target_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_target_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_target_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_target_velocity.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_target_velocity.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_target_velocity.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_target_velocity.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_target_velocity.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->target_velocity);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_target_velocity;
      u_target_velocity.base = 0;
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_target_velocity.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->target_velocity = u_target_velocity.real;
      offset += sizeof(this->target_velocity);
     return offset;
    }

    virtual const char * getType() override { return "my_package/TargetVelocity"; };
    virtual const char * getMD5() override { return "e64ef444ba1c1e91736f1d60c5947ad5"; };

  };

}
#endif
