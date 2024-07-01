#!/usr/bin/env python

import rospy
from nav_msgs.msg import Odometry
import numpy as np
import os

def odom_callback(odom_msg):
    # Lấy thông tin vị trí và hướng từ message Odometry
    x = odom_msg.pose.pose.position.x
    y = odom_msg.pose.pose.position.y
    z = odom_msg.pose.pose.position.z

    qx = odom_msg.pose.pose.orientation.x
    qy = odom_msg.pose.pose.orientation.y
    qz = odom_msg.pose.pose.orientation.z
    qw = odom_msg.pose.pose.orientation.w

    # Chuyển đổi quaternion thành ma trận xoay
    q_matrix = np.array([
        [1 - 2*qy**2 - 2*qz**2, 2*qx*qy - 2*qz*qw, 2*qx*qz + 2*qy*qw],
        [2*qx*qy + 2*qz*qw, 1 - 2*qx**2 - 2*qz**2, 2*qy*qz - 2*qx*qw],
        [2*qx*qz - 2*qy*qw, 2*qy*qz + 2*qx*qw, 1 - 2*qx**2 - 2*qy**2]
    ])

    # Tạo ma trận đồng nhất 4x4 từ vị trí và ma trận xoay
    transform_matrix = np.identity(4)
    transform_matrix[0, 3] = x
    transform_matrix[1, 3] = y
    transform_matrix[2, 3] = z
    transform_matrix[:3, :3] = q_matrix

    # Lưu ma trận vào danh sách poses
    pose = transform_matrix[:3].flatten()
    poses.append(pose)
    rospy.loginfo("Robot Position from /odom: %s", pose)

if __name__ == '__main__':
    rospy.init_node('robot_pose_listener', anonymous=True)

    poses = []

    # Đăng ký subscriber nhận dữ liệu từ topic /odom
    rospy.Subscriber("/odom", Odometry, odom_callback)

    rate = rospy.Rate(10.0)
    try:
        while not rospy.is_shutdown():
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
    finally:
        # Ghi các pose vào file txt khi thoát chương trình
        output_dir = '/home/focal/result'  # Đường dẫn tuyệt đối
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        output_file = os.path.join(output_dir, 'groundtruth.txt')
        with open(output_file, 'w') as f:
            for pose in poses:
                f.write(' '.join(map(str, pose)) + '\n')

        # Thông báo in ra màn hình nếu ghi file thành công
        rospy.loginfo("Successfully wrote poses to %s", output_file)
