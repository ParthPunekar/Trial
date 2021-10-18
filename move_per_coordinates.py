#!/usr/bin/env python

import threading
from threading import *
from time import *

import rospy
from geometry_msgs.msg import Twist

def timer():
	global counter
	counter += 1
	if recall:
		threading.Timer(1.0, timer).start()

def read_data():

	global counter 
	counter = 0

	x = int(input('Enter X Co-ordinate'))
	y = int(input('Enter Y Co-ordinate'))

	y_rotations = abs(y) // distance_covered
	y_time_required = time_for_one_rotation * y_rotations

	x_rotations = abs(x) // distance_covered
	x_time_required = time_for_one_rotation * x_rotations

	x_direction = 0
	y_direction = 0

	if y > 0:
		y_direction = 1
	elif y < 0:
		y_direction = -1
	else:
		y_direction = 0

	if x > 0:
		x_direction = 1
	elif x < 0:
		x_direction = -1
	else:
		x_direction = 0

	return y_time_required, y_direction, x_time_required, x_direction


def move_cooper():
	
	rospy.init_node('move_cooper', anonymous=True)
	pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
	rate = rospy.Rate(10)

	vel = Twist()

	y_time_required, y_direction, x_time_required, x_direction = read_data()

	while not rospy.is_shutdown():

		if counter < y_time_required:
			vel.linear.x = y_direction
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = 0

		else:
			vel.linear.x = 0
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = 0

		if counter > (y_time_required) and counter < (y_time_required + 3):
			vel.linear.x = 0
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = x_direction
		else:
			vel.linear.x = 0
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = 0

		if counter > (y_time_required + 4) and counter < (x_time_required + y_time_required + 5):
			vel.linear.x = 1
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = 0
		else:
			vel.linear.x = 0
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = 0

		if counter == (x_time_required + y_time_required + 7):
			print('....................Task Completed....................')
			print('Provide new X and Y values')
			y_time_required, y_direction, x_time_required, x_direction = read_data()

		pub.publish(vel)
		rate.sleep()
	recall = False


if __name__ == '__main__':

	recall = True
	counter = 0

	time_for_one_rotation = 4 								# in seconds
	distance_covered = 30									# Approx Distance covered by wheel in one rotation
	rotation_to_cover_distance = 1							# 1 Revolution covers 30 cm

	timer()
	try:
		move_cooper()
	except rospy.ROSInterruptException:
		pass