#!/usr/bin/env python

import rospy
import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image, PointCloud2
from geometry_msgs.msg import PoseWithCovarianceStamped, PoseArray
import pydbow3 as DBoW3
import os
import glob
import yaml

class AmclMonitor:
    def __init__(self):
        rospy.init_node('amcl_monitor', anonymous=True)
        self.bridge = CvBridge()
        self.image_sub = None
        self.pose_pub = rospy.Publisher('/initialpose', PoseWithCovarianceStamped, queue_size=10)

        # Load vocabulary and create database
        rospy.loginfo("[Loading vocabulary] start")
        # voc_path = rospy.get_param('~vocabulary_path', '../config/sthereo_07_rgb_4_3.yaml')
        # voc_path = "/home/focal/omni_ws/src/OmniBot/place_recognition/config/sthereo_07_rgb_4_3.yaml"
        voc_path = os.path.join(os.path.dirname(__file__), '/home/focal/omni_ws/src/OmniBot/place_recognition/config/sthereo_07_rgb_4_3.yaml')
        # print("Current path is", os.getcwd())
        self.voc = DBoW3.Vocabulary()
        self.voc.load(voc_path)

        # Initialize the database
        self.db = DBoW3.Database()
        self.db.setVocabulary(self.voc, False, 0)
        rospy.loginfo("[Loading vocabulary] end")

        # Initialize ORB detector

        self.orb = cv2.ORB_create()

        # Load images and add to database
        rospy.loginfo("[DBoW3::Database::add] start")
        self.image_dir = os.path.join(os.path.dirname(__file__), '../dataset/207/image')
        image_paths = self.load_image_paths(self.image_dir)
        for path in image_paths:
            image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
            keypoints, descriptors = self.orb.detectAndCompute(image, None)
            self.db.add(descriptors)
        rospy.loginfo(self.db)
        rospy.loginfo("[DBoW3::Database::add] end")

        # Subscribe to /particlecloud
        # rospy.Subscriber("/particlecloud", PointCloud2, self.particlecloud_callback)
        rospy.Subscriber("/particlecloud", PoseArray, self.particlecloud_callback)

    def load_image_paths(self, directory):
        """Load image paths from the specified directory."""
        return glob.glob(os.path.join(directory, '*.png'))

    def particlecloud_callback(self, data):
        # particle_count = len(list(self.point_cloud2_to_array(data)))
        particle_count = len(data.poses)
        rospy.loginfo("Number of particles: %d", particle_count)
        if particle_count > some_threshold:
            rospy.logwarn("Number of particles > threshold!")
            self.lost_localization = True
            if self.image_sub is None:
                self.image_sub = rospy.Subscriber("/camera/color/image_raw", Image, self.image_callback)
        else:
            self.lost_localization = False
            if self.image_sub is not None:
                self.image_sub.unregister()
                self.image_sub = None

    def point_cloud2_to_array(self, cloud_msg):
        # Converts PointCloud2 to an array (optional implementation)
        from sensor_msgs import point_cloud2
        return point_cloud2.read_points(cloud_msg, field_names=("x", "y", "z"), skip_nans=True)

    def image_callback(self, img_msg):
        if not self.lost_localization:
            return
        try:
            cv_image = self.bridge.imgmsg_to_cv2(img_msg, "mono8")
            rospy.loginfo("Received image from /camera/color/image_raw")

            # Detect and compute ORB features
            keypoints, descriptors = self.orb.detectAndCompute(cv_image, None)

            # Query the database
            results = self.query_database(descriptors)
            rospy.loginfo("Query results: %s", str(results))
            
            if results:
                # Get the image path with the highest score
                best_match_id = results[0].Id
                best_match_path = self.db_to_image_path(best_match_id)
                
                # Find corresponding .txt file in odom directory
                odom_path = best_match_path.replace('image', 'pose').replace('.png', '.txt')
                if os.path.exists(odom_path):
                    self.publish_pose(odom_path)
                    rospy.loginfo("ID: %s", str(odom_path))
                else:
                    rospy.logwarn("Corresponding odom file not found: %s", odom_path)
            
        except Exception as e:
            rospy.logerr("Can not convert image: %s", e)

    def query_database(self, descriptors):
        if descriptors is not None:
            results = self.db.query(descriptors, 1)
            return results
        else:
            return None

    def db_to_image_path(self, db_id):
        """Convert database ID to image path."""
        image_paths = self.load_image_paths(self.image_dir)
        if 0 <= db_id < len(image_paths):
            return image_paths[db_id]
        return None

    def publish_pose(self, odom_path):
        """Publish pose from the odom file."""
        try:
            with open(odom_path, 'r') as file:
                pose_data = yaml.safe_load(file)
                pose_msg = PoseWithCovarianceStamped()

                pose_msg.header.stamp = rospy.Time.now()
                pose_msg.header.frame_id = pose_data['header']['frame_id']
                
                pose_msg.pose.pose.position.x = pose_data['pose']['pose']['position']['x']
                pose_msg.pose.pose.position.y = pose_data['pose']['pose']['position']['y']
                pose_msg.pose.pose.position.z = pose_data['pose']['pose']['position']['z']
                pose_msg.pose.pose.orientation.x = pose_data['pose']['pose']['orientation']['x']
                pose_msg.pose.pose.orientation.y = pose_data['pose']['pose']['orientation']['y']
                pose_msg.pose.pose.orientation.z = pose_data['pose']['pose']['orientation']['z']
                pose_msg.pose.pose.orientation.w = pose_data['pose']['pose']['orientation']['w']
                
                pose_msg.pose.covariance = pose_data['pose']['covariance']

                self.pose_pub.publish(pose_msg)
                rospy.loginfo("Published initial pose from: %s", odom_path)
                
        except Exception as e:
            rospy.logerr("Can not read file odom: %s", e)

if __name__ == '__main__':
    some_threshold = 1500  # Set threshold for the number of particles
    monitor = AmclMonitor()
    rospy.spin()
