#!/usr/bin/env python

import rospy
import tf2_ros
import numpy as np
from geometry_msgs.msg import TransformStamped
import os

def get_robot_pose():
    rospy.init_node('robot_pose_listener', anonymous=True)

    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)

    poses = []

    rate = rospy.Rate(10.0)
    try:
        while not rospy.is_shutdown():
            try:
                trans = tfBuffer.lookup_transform('map', 'base_link', rospy.Time())
                # Chuyển đổi Transform thành ma trận đồng nhất 4x4
                transform_matrix = np.identity(4)
                transform_matrix[0, 3] = trans.transform.translation.x
                transform_matrix[1, 3] = trans.transform.translation.y
                transform_matrix[2, 3] = trans.transform.translation.z

                # Chuyển đổi quaternion thành ma trận xoay
                q = trans.transform.rotation
                q_matrix = np.array([
                    [1 - 2*q.y**2 - 2*q.z**2, 2*q.x*q.y - 2*q.z*q.w, 2*q.x*q.z + 2*q.y*q.w],
                    [2*q.x*q.y + 2*q.z*q.w, 1 - 2*q.x**2 - 2*q.z**2, 2*q.y*q.z - 2*q.x*q.w],
                    [2*q.x*q.z - 2*q.y*q.w, 2*q.y*q.z + 2*q.x*q.w, 1 - 2*q.x**2 - 2*q.y**2]
                ])
                transform_matrix[:3, :3] = q_matrix

                # Lưu ma trận vào danh sách poses
                pose = transform_matrix[:3].flatten()
                poses.append(pose)
                rospy.loginfo("Robot Position in map frame: %s", pose)
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
                rospy.logwarn("Failed to get transform")
                continue

            rate.sleep()
    except rospy.ROSInterruptException:
        pass
    finally:
        # Ghi các pose vào file txt
        output_dir = '/home/focal/result'  # Đường dẫn tuyệt đối
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        output_file = os.path.join(output_dir, 'c9nopl.txt')
        with open(output_file, 'w') as f:
            for pose in poses:
                f.write(' '.join(map(str, pose)) + '\n')

        # Thông báo in ra màn hình nếu ghi file thành công
        rospy.loginfo("Successfully wrote poses to %s", output_file)

if __name__ == '__main__':
    try:
        get_robot_pose()
    except rospy.ROSInterruptException:
        pass
