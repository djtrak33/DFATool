Git repo for UC's Drone Fault Assessment Tool

PX4 1.11)
1)	Prepare a Ubuntu 18.04 VM

2)	Follow the Installation Instructions on [2] for ROS/Gazebo.
In case of error “No module named future”, the module future may be needed to be installed through pip (also get pip with “sudo apt install python-pip”): 
pip install –U future 
And try step 2 again

3)	Download the PX4 Source Code at [3]

4)	When trying to start the firmware depicted in [3] at “First Build”, the following packages may be needed to be installed through pip3 (also get pip3 with “sudo apt install python3-pip”):
pip3 install –user empy |pip3 install –user toml | pip3 install –user pyros-genmsg | pip3 install –user packaging | pip3 install –user numpy | pip3 install –user jinja2

5)	If after installing these packages, running “make px4_sitl gazebo” from the Firmware directory still fails, additional packages may be needed to be installed. Install the following packages [4]:
sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
6)	After this, the firmware should boot up correctly by typing make px4_sitl gazebo in the terminal from the firmware folder.
7)	In [5] we can find instructions in order to run ROS in parallel with Gazebo.
8)	Follow the instructions for Ubuntu/Linux in [6] in order to install qgroundcontrol
9)	Check [1] at around 4:20 in order to verify that all the tools are able to run in parallel

[1] https://www.youtube.com/watch?v=-zEddRFbMvQ - Complete Guide - PX4 SITL - Gazebo - QGroundControl Installation
[2] https://dev.px4.io/master/en/setup/dev_env_linux_ubuntu.html - ROS with Gazebo Simulation + PX4
[3] https://dev.px4.io/master/en/setup/building_px4.html
[4] https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c
[5] https://dev.px4.io/master/en/simulation/ros_interface.html
[6] https://docs.qgroundcontrol.com/master/en/getting_started/download_and_install.html

Currently the tool requires for manual change in directory rights through for example:
sudo chmod -R 777 /(PX4Auto-Pilot Folder)/

From within the tool define the px4 folder as the "PX4-Autopilot" folder and the mission folder as the "dronekit-scripts" folder
