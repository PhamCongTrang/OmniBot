#!/usr/bin/env python

import rospy
from sensor_msgs.msg import Image
from geometry_msgs.msg import PoseWithCovarianceStamped
from tf2_ros import TransformListener, Buffer
from geometry_msgs.msg import TransformStamped
import cv2
from cv_bridge import CvBridge
import os
import time

class ImageLocationSaver:
    def __init__(self):
        self.bridge = CvBridge()
        self.image = None
        self.position = None
        self.rotation = None

        self.image_sub = rospy.Subscriber("/camera/color/image_raw", Image, self.image_callback)

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer)

        self.save_interval = 2  # seconds
        self.last_save_time = time.time()

        # Define the paths
        package_dir = os.path.dirname(os.path.abspath(__file__))
        self.image_dir = os.path.join(package_dir, '../dataset/207/image')
        self.pose_dir = os.path.join(package_dir, '../dataset/207/pose')

        # Create directories if they don't exist
        if not os.path.exists(self.image_dir):
            os.makedirs(self.image_dir)
        if not os.path.exists(self.pose_dir):
            os.makedirs(self.pose_dir)

    def image_callback(self, msg):
        try:
            self.image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except Exception as e:
            rospy.logerr("Error converting image: %s", e)

    def get_robot_position(self):
        try:
            # Lookup transform from map to base_link (or base_footprint)
            transform: TransformStamped = self.tf_buffer.lookup_transform('map', 'base_link', rospy.Time(0))
            self.position = transform.transform.translation
            self.rotation = transform.transform.rotation

        except Exception as e:
            rospy.logerr("Error getting transform: %s", e)

    def save_data(self):
        self.get_robot_position()
        if self.position is None or self.rotation is None:
            rospy.logerr("Position is None")
        if self.image is None:
            rospy.logerr("Image is None")
        if self.image is not None and self.position is not None and self.rotation is not None:
            current_time = rospy.Time.now()
            timestamp = int(current_time.to_sec())  # Use integer timestamp for filenames

            # Save the image
            image_filename = os.path.join(self.image_dir, f"{timestamp}.png")
            cv2.imwrite(image_filename, self.image)
            rospy.loginfo(f"Saved image: {image_filename}")

            # Create PoseWithCovarianceStamped message
            pose_msg = PoseWithCovarianceStamped()
            pose_msg.header.stamp = current_time
            pose_msg.header.frame_id = "map"

            # Set the pose
            pose_msg.pose.pose.position = self.position
            pose_msg.pose.pose.orientation = self.rotation
            
            # Define the covariance
            covariance = [0.25] + [0.0] * 34 + [0.06853892326654787]

            # Define file name based on current time
            pose_file_name = os.path.join(self.pose_dir, f"{timestamp}.txt")

            # Write pose data to file with the required format
            with open(pose_file_name, 'w') as f:
                f.write(f"header:\n")
                f.write(f"  seq: 0\n")
                f.write(f"  stamp:\n")
                f.write(f"    secs: {pose_msg.header.stamp.secs}\n")
                f.write(f"    nsecs: {pose_msg.header.stamp.nsecs}\n")
                f.write(f"  frame_id: \"{pose_msg.header.frame_id}\"\n")
                f.write(f"pose:\n")
                f.write(f"  pose:\n")
                f.write(f"    position:\n")
                f.write(f"      x: {self.position.x}\n")
                f.write(f"      y: {self.position.y}\n")
                f.write(f"      z: {self.position.z}\n")
                f.write(f"    orientation:\n")
                f.write(f"      x: {self.rotation.x}\n")
                f.write(f"      y: {self.rotation.y}\n")
                f.write(f"      z: {self.rotation.z}\n")
                f.write(f"      w: {self.rotation.w}\n")
                f.write(f"  covariance: {covariance}\n")

            rospy.loginfo(f"Saved pose: {pose_file_name}")

            self.last_save_time = current_time.to_sec()

    def run(self):
        rate = rospy.Rate(1)  # 1 Hz
        while not rospy.is_shutdown():
            current_time = time.time()
            if current_time - self.last_save_time >= self.save_interval:
                self.save_data()
            rate.sleep()

if __name__ == '__main__':
    rospy.init_node('collect_dataset', anonymous=True)
    node = ImageLocationSaver()
    node.run()
