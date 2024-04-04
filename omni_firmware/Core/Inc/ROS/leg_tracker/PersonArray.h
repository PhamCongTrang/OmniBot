#ifndef _ROS_leg_tracker_PersonArray_h
#define _ROS_leg_tracker_PersonArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "leg_tracker/Person.h"

namespace leg_tracker
{

  class PersonArray : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t people_length;
      typedef leg_tracker::Person _people_type;
      _people_type st_people;
      _people_type * people;

    PersonArray():
      header(),
      people_length(0), st_people(), people(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->people_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->people_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->people_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->people_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->people_length);
      for( uint32_t i = 0; i < people_length; i++){
      offset += this->people[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t people_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      people_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      people_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      people_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->people_length);
      if(people_lengthT > people_length)
        this->people = (leg_tracker::Person*)realloc(this->people, people_lengthT * sizeof(leg_tracker::Person));
      people_length = people_lengthT;
      for( uint32_t i = 0; i < people_length; i++){
      offset += this->st_people.deserialize(inbuffer + offset);
        memcpy( &(this->people[i]), &(this->st_people), sizeof(leg_tracker::Person));
      }
     return offset;
    }

    virtual const char * getType() override { return "leg_tracker/PersonArray"; };
    virtual const char * getMD5() override { return "7b54bf6148ab21083b85a27eec643002"; };

  };

}
#endif
