#!/usr/bin/env python

import rospy
import cv2
from cv_bridge import CvBridge
import pydbow3 as DBoW3
import os
import glob
import numpy as np

class AmclMonitor:
    def __init__(self):
        rospy.init_node('amcl_monitor', anonymous=True)
        self.bridge = CvBridge()
        self.image_sub = None

        # Load vocabulary and create database
        rospy.loginfo("[Loading vocabulary] start")
        voc_path = os.path.join(os.path.dirname(__file__), '../config/sthereo_07_rgb_4_3.yaml')
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

        self.test_image_dir = os.path.join(os.path.dirname(__file__), '../dataset/207/test')
        self.test_image_paths = self.load_image_paths(self.test_image_dir)
        self.test_image_index = 0
        self.image_callback()

    def load_image_paths(self, directory):
        """Load image paths from the specified directory."""
        return glob.glob(os.path.join(directory, '*.png'))

    def image_callback(self):
        for test_image_path in self.test_image_paths:
            try:
                cv_image = cv2.imread(test_image_path, cv2.IMREAD_GRAYSCALE)
                # Detect and compute ORB features
                keypoints, descriptors = self.orb.detectAndCompute(cv_image, None)

                # Query the database
                results = self.query_database(descriptors)
                rospy.loginfo("----------------------------------------")
                rospy.loginfo("Query results %s:",os.path.basename(test_image_path))

                match_images = []
                match_info = []
                for i, result in enumerate(results[:3]):
                        match_path = self.db_to_image_path(result.Id)
                        match_image = cv2.imread(match_path, cv2.IMREAD_GRAYSCALE)
                        match_images.append(match_image)
                        match_info.append((os.path.basename(match_path), result.Score))
                        rospy.loginfo("Match %d: Score = %f, %s", i+1, result.Score, os.path.basename(match_path))

                self.display_images(cv_image, os.path.basename(test_image_path), match_images, match_info)
            except Exception as e:
                rospy.logerr("Can not load image: %s", e)

    def query_database(self, descriptors):
        if descriptors is not None:
            results = self.db.query(descriptors, 3)
            return results
        else:
            return None

    def db_to_image_path(self, db_id):
        """Convert database ID to image path."""
        image_paths = self.load_image_paths(self.image_dir)
        if 0 <= db_id < len(image_paths):
            return image_paths[db_id]
        return None

    def display_images(self, test_image, test_image_name, match_images, match_info):
        """Display the test image and top 3 match images in a 2x2 grid with text overlay."""
        h, w = test_image.shape
        # Create a blank composite image
        composite_image = np.zeros((2 * h, 2 * w), dtype=np.uint8)
        
        # Place the test image at the top-left and add text
        composite_image[:h, :w] = test_image
        cv2.putText(composite_image, f'{test_image_name}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
        
        # Place the match images in the remaining slots and add text
        for i, (match_image, (file_name, score)) in enumerate(zip(match_images, match_info)):
            row = (i + 1) // 2
            col = (i + 1) % 2
            composite_image[row * h:(row + 1) * h, col * w:(col + 1) * w] = match_image
            cv2.putText(composite_image, f'{file_name}', (col * w + 10, row * h + 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2, cv2.LINE_AA)
            cv2.putText(composite_image, f'Score: {score:.2f}', (col * w + 10, row * h + 60), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2, cv2.LINE_AA)
        
        # Display the composite image
        cv2.imshow('Test Image and Top 3 Matches', composite_image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
if __name__ == '__main__':
    monitor = AmclMonitor()
    rospy.spin()
