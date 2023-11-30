#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
import sys
import moveit_commander
import moveit_msgs.msg
import geometry_msgs.msg
from math import pi, radians
from moveit_commander.conversions import pose_to_list

class RobotManipulator():
    def __init__(self):
        self.joint_angles = []

        moveit_commander.roscpp_initialize(sys.argv)
        rospy.init_node('robot_arm_control', anonymous=True)

        self.robot_arm = moveit_commander.RobotCommander()
        self.scene = moveit_commander.PlanningSceneInterface()

        self.group_name = "arm_group"
        self.group = moveit_commander.MoveGroupCommander(self.group_name)

        self.display_trajectory_publisher = rospy.Publisher('/move_group/display_planned_path', moveit_msgs.msg.DisplayTrajectory, queue_size=20)
        rospy.Subscriber("/motor_cmd", String, self.motor_cmd_callback)
        

    def motor_cmd_callback(self, msg):
        print(msg.data)
        motor_commands = list(map(int, msg.data.strip('()').split(',')))
        print("motor command received:", motor_commands)

        self.convert_to_angles(motor_commands)
    
    def convert_to_angles(self, motor_commands):
        j1 = radians(motor_commands[0] - 90)
        j2 = radians(-motor_commands[1] + 90)
        j3 = radians(motor_commands[2] - 90)
        j4 = radians(motor_commands[3] - 90)

        print(j1, j2, j3, j4)
        self.joint_angles.append([j1, j2, j3, j4])

    def jointGoal(self):
        self.group.set_joint_value_target(self.joint_angles[0])        
        self.group.go(wait=True)
        rospy.sleep(5)

if __name__ == "__main__":
    robot_arm = RobotManipulator()

    while not rospy.is_shutdown():
        if len(robot_arm.joint_angles) != 0:
            robot_arm.jointGoal()
        rospy.sleep(1)