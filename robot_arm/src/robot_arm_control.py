#!/usr/bin/env python3

import rospy
import sys
import moveit_commander
import moveit_msgs.msg
import geometry_msgs.msg
from math import pi, radians
from std_msgs.msg import String
from moveit_commander.conversions import pose_to_list

class RobotArm():
    def __init__(self):
        moveit_commander.roscpp_initialize(sys.argv)
        rospy.init_node('robot_arm_control', anonymous=True)

        self.robot_arm = moveit_commander.RobotCommander()
        self.scene = moveit_commander.PlanningSceneInterface()

        self.group_name = "arm_group"
        self.group = moveit_commander.MoveGroupCommander(self.group_name)

        self.display_trajectory_publisher = rospy.Publisher('/move_group/display_planned_path', moveit_msgs.msg.DisplayTrajectory, queue_size=20)
        # self.goalPub = rospy.Publisher("/move_group/goal", moveit_msgs.msg.MoveGroupActionGoal, queue_size=10)

    def getDetails(self):
        # We can get the name of the reference frame for this robot:
        planning_frame = self.group.get_planning_frame()
        print("============ Reference frame: %s" % planning_frame)

        # We can also print the name of the end-effector link for this group:
        eef_link = self.group.get_end_effector_link()
        print("============ End effector: %s" % eef_link)

        # We can get a list of all the groups in the robot:
        group_names = self.robot_arm.get_group_names()
        print("============ Robot Groups:", self.robot_arm.get_group_names())

        # Sometimes for debugging it is useful to print the entire state of the
        # robot:
        print("============ Printing robot state")
        print(self.robot_arm.get_current_state())
        print("")
    
    def jointGoal(self, joint_goal):
        for angle in range(len(joint_goal)):
            joint_goal[angle] = radians(joint_goal[angle])
        self.group.set_joint_value_target(joint_goal)
        
        self.group.go(wait=True)

    def poseGoal(self):
        pose_goal = geometry_msgs.msg.Pose()
        pose_goal.orientation.x = 0
        pose_goal.orientation.y = 0
        pose_goal.orientation.z = 0
        pose_goal.orientation.w = -1.57
        pose_goal.position.x = 0
        pose_goal.position.y = 0
        pose_goal.position.z = 0
        self.group.set_pose_target(pose_goal)

        self.group.plan()
        plan = self.group.go(wait=True)
        # Calling `stop()` ensures that there is no residual movement
        self.group.stop()
        # It is always good to clear your targets after planning with poses.
        # Note: there is no equivalent function for clear_joint_value_targets()
        self.group.clear_pose_targets()

if __name__ == "__main__":

    robot_arm = RobotArm()
    # robot_arm.getDetails()

    while not rospy.is_shutdown():
        joint_angles = list(map(float, input("Enter joint angles: ").split()))
        robot_arm.jointGoal(joint_angles)