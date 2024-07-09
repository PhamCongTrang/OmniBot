# OmniBot
An autonomous mobile robot project developed by a group of students at Hanoi University of Science and Technology, under the guidance of Prof. Assoc. Nguyễn Phạm Thục Anh
## Project Start: 01/03 - Still Updatting
![Ảnh1](https://github.com/PhamCongTrang/OmniBot/assets/88698811/e2daff82-4260-48b0-b4f7-3e36c6ec1a62)

<div align = "center"> 
  <img src = "https://github.com/PhamCongTrang/OmniBot/assets/88698811/68dff2fb-db69-4e3a-815b-30f0fa90dd1c">
</div>

## Introduction
This project is an Autonomous Mobile Robot (AMR) capable of autonomously navigating a predefined environment, avoiding obstacles, and performing tasks such as transporting goods.

## Table of Contents
- [Hardware](#hardware)
- [Installation](#installation)
  - [System Requirements](#system-requirements)
  - [Prerequisites](#prerequisites)
  - [Installation Steps](#installation-steps)
- [Usage](#usage)
  - [Omni SLAM](#omni-slam)
  - [Omni Navigation](#omni-navigation)
  - [Place Recognition](#place-recognition)

## Hardware
OmniBot is equipped with the following hardware components:
- **Camera**: RealSense Depth Camera D435
- **Lidar**: Hokuyo URG-04LX-UG01
- **Microcontroller**: STM32F407VGT6
- **IMU**: MPU9650
- **Motor Driver**: BTS7960
- **DC Motors**

## Installation
### System Requirements
- Python 3.8+
- ROS Noetic / Melodic
- OpenCV 4.2

### Prerequisites
#### Install LIDAR Hokuyo (urg_node)
1. Install urg_node:
    ```bash
    sudo apt-get install ros-<ros_distro>-urg-node
    ```

#### Install RealSense D435
1. Install the RealSense ROS package:
    ```bash
    sudo apt-get install ros-<ros_distro>-realsense2-camera
    ```
#### Install Hector-SLAM
1. Install Hector-SLAM:
    ```bash
    sudo apt-get install ros-<ros_distro>-hector-slam
    ```

#### Install GMapping
1. Install GMapping:
    ```bash
    sudo apt-get install ros-<ros_distro>-gmapping
    ```

#### Install Cartographer
1. Install dependencies:
    ```bash
    sudo apt-get update
    sudo apt-get install -y python-wstool python-rosdep ninja-build
    ```
2. Create a workspace and clone Cartographer:
    ```bash
    mkdir -p ~/cartographer_ws/src
    cd ~/cartographer_ws
    wstool init src
    wstool merge -t src https://raw.githubusercontent.com/cartographer-project/cartographer_ros/master/cartographer_ros.rosinstall
    wstool update -t src
    ```
3. Install and build Cartographer:
    ```bash
    src/cartographer/scripts/install_abseil.sh
    rosdep update
    rosdep install --from-paths src --ignore-src --rosdistro=${ROS_DISTRO} -y
    catkin_make_isolated --install --use-ninja
    ```

#### Install Pangolin
1. Clone and build Pangolin:
    ```bash
    git clone https://github.com/stevenlovegrove/Pangolin.git
    cd Pangolin
    mkdir build
    cd build
    cmake ..
    cmake --build .
    sudo make install
    ```

#### Install ORB-SLAM3
1. Clone and build ORB-SLAM3:
    ```bash
    git clone https://github.com/UZ-SLAMLab/ORB_SLAM3.git
    cd ORB_SLAM3
    chmod +x build.sh
    ./build.sh
    ```

#### Install DBoW3
1. Clone and build DBoW3:
    ```bash
    git clone https://github.com/rmsalinas/DBow3.git
    cd DBoW3
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
    ```
#### Install RTAB-Map
1. Install RTAB-Map:
    ```bash
    sudo apt-get install ros-<ros_distro>-rtabmap ros-<ros_distro>-rtabmap-ros
    ```
### Installation Steps
1. Create a catkin workspace:
    ```bash
    mkdir -p ~/catkin_ws/src
    cd ~/catkin_ws/src
    ```
2. Clone the repo:
    ```bash
    git clone https://github.com/PhamCongTrang/OmniBot.git
    cd ~/catkin_ws
    ```
3. Build the workspace:
    ```bash
    catkin build
    ```
4. Source the workspace:
    ```bash
    source devel/setup.bash
    ```

## Usage
### Omni SLAM
1. To start the SLAM system with a specific method, use the following command:
    ```bash
    roslaunch omni_slam omni_slam.launch slam_methods:=<method>
    ```
    Replace `<method>` with the desired SLAM method. For example:
    - Cartographer: `roslaunch omni_slam omni_slam.launch slam_methods:=cartographer`
    - Hector-SLAM: `roslaunch omni_slam omni_slam.launch slam_methods:=hector`
    - GMapping: `roslaunch omni_slam omni_slam.launch slam_methods:=gmapping`
    - ORB-SLAM3: `roslaunch omni_slam omni_slam.launch slam_methods:=orb_slam3`
    - RTAB-Map: `roslaunch omni_slam omni_slam.launch slam_methods:=rtabmap`

### Omni Navigation
1. To start the navigation system, use the following command:
    ```bash
    roslaunch omni_navigation omni_navigation.launch
    ```
### Place Recognition
#### Data Collection
1. To collect data for place recognition, run:
    ```bash
    rosrun place_recognition collect_data.py
    ```
    
#### Querying Database
1. To query the place recognition database, run:
    ```bash
    rosrun place_recognition query_database.py
    ```


