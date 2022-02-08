"""lab3 controller."""
# Copyright Prof. Bradley Hayes <bradley.hayes@colorado.edu> 2021
# University of Colorado Boulder CSCI 3302 "Introduction to Robotics" Lab 3 Base Code.

from controller import Robot, Motor
import math

# Fill out with correct values from Robot Spec Sheet (or inspect PROTO definition for the robot)
MAX_SPEED = 2.84 # [rad/s]
MAX_SPEED_MS = 0.22 # [m/s]
AXLE_LENGTH = 0.16 # [m]



MOTOR_LEFT = 0 # Left wheel index
MOTOR_RIGHT = 1 # Right wheel index

# create the Robot instance.
robot = Robot()

# get the time step of the current world.
timestep = int(robot.getBasicTimeStep())

# The Turtlebot robot has two motors
part_names = ("left wheel motor", "right wheel motor")

goal = (1.5,1,0)
# Set wheels to velocity control by setting target position to 'inf'
target_pos = ('inf', 'inf')
robot_parts = []

for i in range(len(part_names)):
        robot_parts.append(robot.getDevice(part_names[i]))
        robot_parts[i].setPosition(float(target_pos[i]))

# Odometry
pose_x     = 0
pose_y     = 0
pose_theta = 0

# Rotational Motor Velocity [rad/s]
vL = 0
vR = 0

xr = 0
theta_r = 0


while robot.step(timestep) != -1:

    #STEP 2: Calculate sources of error
    p = math.sqrt(pow((pose_x - goal[0]),2) + pow((pose_y - goal[1]),2))
    a = math.atan2((goal[1] - pose_y),(goal[0] - pose_x)) - pose_theta
    n = goal[2] - pose_theta
    pass   
    
    #STEP 2.4: Feedback Controller    
    if(p > .001):
        theta_r = 10 * a
    else:
        theta_r = n
        
    if(a < .0000001 and a > -.0000001):
        xr = p
    else:
        xr = 0
  
    pass
    
    #STEP 1.2: Inverse Kinematics Equations 
    vL = ((2 * xr) - (theta_r * AXLE_LENGTH)) / 2
    vR = ((2 * xr) + (theta_r * AXLE_LENGTH)) / 2
    pass
    
    # STEP 2.5: Compute wheel velocities (vL, vR)
    #we compute these at the same time as the inverse kinematics equations above
    pass

    #STEP 2.7: Proportional velocities
    #if(vL < 0):
        #vL = -vL
    #if(vR < 0):
        #vR = -vR
    vL = robot_parts[MOTOR_LEFT].getMaxVelocity() * vL
    vR = robot_parts[MOTOR_RIGHT].getMaxVelocity() * vR
    pass

    #STEP 2.6: Clamp wheel speeds
    if(vR > robot_parts[MOTOR_RIGHT].getMaxVelocity()):
        vR = robot_parts[MOTOR_RIGHT].getMaxVelocity()
    if(vR < -robot_parts[MOTOR_RIGHT].getMaxVelocity()):
        vR = -robot_parts[MOTOR_RIGHT].getMaxVelocity()
    if(vL > robot_parts[MOTOR_LEFT].getMaxVelocity()):
        vL = robot_parts[MOTOR_LEFT].getMaxVelocity()
    if(vL < -robot_parts[MOTOR_LEFT].getMaxVelocity()):
        vL = -robot_parts[MOTOR_LEFT].getMaxVelocity()
    pass


        
    # Odometry code. Don't change speeds (vL and vR) after this line
    distL = (vL / robot_parts[MOTOR_LEFT].getMaxVelocity()) * MAX_SPEED_MS * timestep/1000.0
    distR = (vR / robot_parts[MOTOR_RIGHT].getMaxVelocity()) * MAX_SPEED_MS * timestep/1000.0
    pose_x += (distL+distR) / 2.0 * math.cos(pose_theta)
    pose_y += (distL+distR) / 2.0 * math.sin(pose_theta)
    pose_theta += (distR-distL)/AXLE_LENGTH
    # Bound pose_theta between [-pi, 2pi+pi/2]
    # Important to not allow big fluctuations between timesteps (e.g., going from -pi to pi)
    if pose_theta > 6.28+3.14/2: pose_theta -= 6.28
    if pose_theta < -3.14: pose_theta += 6.28
    


    # TODO: Set robot motors to the desired velocities
    robot_parts[MOTOR_LEFT].setVelocity(vL)
    robot_parts[MOTOR_RIGHT].setVelocity(vR)
    print(pose_x, pose_y, pose_theta)
    #print(p)
    #print(n)

    