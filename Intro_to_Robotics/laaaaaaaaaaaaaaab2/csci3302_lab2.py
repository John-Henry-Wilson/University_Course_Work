"""csci3302_lab2 controller."""


import math
from controller import Robot, Motor, DistanceSensor
import os


# Ground Sensor Measurements under this threshold are black
# measurements above this threshold can be considered white.
# TODO: Fill this in with a reasonable threshold that separates "line detected" from "no line detected"
GROUND_SENSOR_THRESHOLD = 310

# These are your pose values that you will update by solving the odometry equations
pose_x = 0
pose_y = 0
pose_theta = 0

counter = 0
flag = False


# Index into ground_sensors and ground_sensor_readings for each of the 3 onboard sensors.
LEFT_IDX = 2
CENTER_IDX = 1
RIGHT_IDX = 0

# create the Robot instance.
robot = Robot()

# ePuck Constants
EPUCK_AXLE_DIAMETER = 0.053 # ePuck's wheels are 53mm apart.
EPUCK_MAX_WHEEL_SPEED = .125 # TODO: To be filled in with ePuck wheel speed in m/s
MAX_SPEED = 6.28

# get the time step of the current world.
SIM_TIMESTEP = int(robot.getBasicTimeStep())

# Initialize Motors
leftMotor = robot.getDevice('left wheel motor')
rightMotor = robot.getDevice('right wheel motor')
leftMotor.setPosition(float('inf'))
rightMotor.setPosition(float('inf'))
leftMotor.setVelocity(0.0)
rightMotor.setVelocity(0.0)

counter = 0
# Initialize and Enable the Ground Sensors
gsr = [0, 0, 0]
ground_sensors = [robot.getDevice('gs0'), robot.getDevice('gs1'), robot.getDevice('gs2')]
for gs in ground_sensors:
    gs.enable(SIM_TIMESTEP)

# Allow sensors to properly initialize
for i in range(10): robot.step(SIM_TIMESTEP)  

vL = 0 # TODO: Initialize variable for left speed, radians/sec
vR = 0 # TODO: Initialize variable for right speed, radians/sec

def update_odometry(vL, vR):
    
    global pose_x
    global pose_y
    global pose_theta
    
    
    vL = vL / MAX_SPEED
    vR = vR / MAX_SPEED
    
    
    vL= vL * EPUCK_MAX_WHEEL_SPEED
    vR = vR * EPUCK_MAX_WHEEL_SPEED
    
    
    
    correct_timestep = SIM_TIMESTEP / 1000.0
    
    Ldist = vL * correct_timestep
    Rdist = vR * correct_timestep
    #print(Ldist, Rdist)
    
    xr = (Ldist + Rdist) / 2
    theta_r = (Rdist - Ldist) / EPUCK_AXLE_DIAMETER
    
   
    
    xi = math.cos(pose_theta) * xr
    yi = math.sin(pose_theta) * xr
    theta_i = theta_r
    #print(xi, yi, theta_i)
    
    
    pose_x += xi
    pose_y += yi
    pose_theta += theta_i
    


     
# Main Control Loop:
while robot.step(SIM_TIMESTEP) != -1:

    # Read ground sensor values
    for i, gs in enumerate(ground_sensors):
        gsr[i] = gs.getValue()

    #print(gsr) 
            
    
    if(gsr[1] < GROUND_SENSOR_THRESHOLD):
        vL = MAX_SPEED / 3
        vR = MAX_SPEED / 3
    if(gsr[0] < GROUND_SENSOR_THRESHOLD):
        vR = MAX_SPEED / 3
        vL = (MAX_SPEED / 3) * 0.007
    if(gsr[2] < GROUND_SENSOR_THRESHOLD):
        vL = MAX_SPEED / 3
        vR = (MAX_SPEED / 3) * 0.007
    if(gsr[0] > 840 and gsr[1] > 840 and gsr[2] > 840):
        vL = 0
        vR = MAX_SPEED / 3
    if(gsr[0] < GROUND_SENSOR_THRESHOLD and gsr[1] < GROUND_SENSOR_THRESHOLD and gsr[2] < GROUND_SENSOR_THRESHOLD ):
        vL = MAX_SPEED / 3
        vR = MAX_SPEED / 3
        
    
  
    
    
    update_odometry(vL, vR)

    #
    # 3) Do simple checks to make sure things are working. In the beginning, 
    #    only one value changes. Once you do a right turn, this value should be constant.
    #
    # 4) Focus on getting things generally right first, then worry
    #    about calculating odometry in the world coordinate system of the
    #    Webots simulator first (x points down, y points right)


    # TODO: Insert Loop Closure Code Here
    
    # Hints:
    #
    # 1) Set a flag whenever you encounter the line
    #
    # 2) Use the pose when you encounter the line last 
    #    for best results
    if(gsr[0] < GROUND_SENSOR_THRESHOLD and gsr[1] < GROUND_SENSOR_THRESHOLD and gsr[2] < GROUND_SENSOR_THRESHOLD ):
        counter = counter + 1
        

    if(counter > 10):
        pose_x = 0
        pose_y = 0
        pose_theta = 0  
        counter = 0
          
    print("Current pose: [%5f, %5f, %5f]" % (pose_x, pose_y, pose_theta))
    leftMotor.setVelocity(vL)
    rightMotor.setVelocity(vR)
    

    