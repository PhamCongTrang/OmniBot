#!/usr/bin/env python

import rospy
import math
from std_msgs.msg import Float32MultiArray
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose, Twist, PoseWithCovariance, TwistWithCovariance, Quaternion, Vector3
from tf.transformations import euler_from_quaternion, quaternion_from_euler

r = 0.05
d = 0.175*math.sqrt(2)
l=2*math.pi*r
class OdomPublisher:

    def __init__(self):
        rospy.init_node('odom_publisher', anonymous=True)
        self.odom_pub = rospy.Publisher('raw_odom', Odometry, queue_size=10)
        self.vel_enc_sub = rospy.Subscriber('vel_enc', Float32MultiArray, self.vel_enc_callback)
        self.rate = rospy.Rate(10) # 10 Hz

    def vel_enc_callback(self, data):
        if len(data.data) != 4:
            rospy.logwarn("Invalid data length received from 'vel_enc' topic.")
            return

        # Get vel_enc_data from message Float32MultiArray
        vel_enc_data = data.data

        # [v_x, v_y, v_theta]
        v_x = (1/math.sqrt(8)) * (-vel_enc_data[0] + vel_enc_data[1] + vel_enc_data[2] - vel_enc_data[3]) * (l/ 60)
        v_y = (1/math.sqrt(8)) * ( vel_enc_data[0] + vel_enc_data[1] - vel_enc_data[2] - vel_enc_data[3]) * (l/ 60)
        v_theta = (1/(4*d)) * (vel_enc_data[0] + vel_enc_data[1] + vel_enc_data[2] + vel_enc_data[3]) * (l/ 60)

        # Set message Odometry
        odom_msg = Odometry()

        # Set header
        odom_msg.header.stamp = rospy.Time.now()
        odom_msg.header.frame_id = 'odom'
        odom_msg.child_frame_id = 'base_link'

        # Set pose
        pose_covariance = [0] * 36  # 6x6 matrix
        pose_covariance[0] = 0.1  # x variance
        pose_covariance[7] = 0.1  # y variance
        pose_covariance[35] = 0.2  # yaw variance
        pose_with_covariance = PoseWithCovariance(
            pose=Pose(
                position=Vector3(0, 0, 0),
                orientation=Quaternion(*quaternion_from_euler(0, 0, v_theta))
            ),
            covariance=pose_covariance
        )
        odom_msg.pose = pose_with_covariance

        # Set twist
        twist_covariance = [0] * 36  # 6x6 matrix
        twist_covariance[0] = 0.1  # linear x variance
        twist_covariance[7] = 0.1  # linear y variance
        twist_covariance[35] = 0.2  # angular z variance
        twist_with_covariance = TwistWithCovariance(
            twist=Twist(
                linear=Vector3(v_x, v_y, 0),  # Assume no y and z motion
                angular=Vector3(0, 0, v_theta)
            ),
            covariance=twist_covariance
        )
        odom_msg.twist = twist_with_covariance

        # Publish message Odometry
        self.odom_pub.publish(odom_msg)
        # self.rate.sleep()

def main():
    odom_pub = OdomPublisher()
    rospy.spin()

if __name__ == '__main__':
    main()
