"""lab5 controller."""
from controller import Robot, Motor, Camera, RangeFinder, Lidar, Keyboard
import math
import numpy as np
from heapdict import heapdict
from matplotlib import pyplot as plt
from scipy.signal import convolve2d # Uncomment if you want to use something else for finding the configuration space

MAX_SPEED = 7.0  # [rad/s]
MAX_SPEED_MS = 0.633 # [m/s]
AXLE_LENGTH = 0.4044 # m
MOTOR_LEFT = 10
MOTOR_RIGHT = 11
N_PARTS = 12

LIDAR_ANGLE_BINS = 667
LIDAR_SENSOR_MAX_RANGE = 2.75 # Meters
LIDAR_ANGLE_RANGE = math.radians(240)


##### vvv [Begin] Do Not Modify vvv #####

# create the Robot instance.
robot = Robot()
# get the time step of the current world.
timestep = int(robot.getBasicTimeStep())

# The Tiago robot has multiple motors, each identified by their names below
part_names = ("head_2_joint", "head_1_joint", "torso_lift_joint", "arm_1_joint",
              "arm_2_joint",  "arm_3_joint",  "arm_4_joint",      "arm_5_joint",
              "arm_6_joint",  "arm_7_joint",  "wheel_left_joint", "wheel_right_joint")

# All motors except the wheels are controlled by position control. The wheels
# are controlled by a velocity controller. We therefore set their position to infinite.
target_pos = (0.0, 0.0, 0.09, 0.07, 1.02, -3.16, 1.27, 1.32, 0.0, 1.41, 'inf', 'inf')
robot_parts=[]

for i in range(N_PARTS):
    robot_parts.append(robot.getDevice(part_names[i]))
    robot_parts[i].setPosition(float(target_pos[i]))
    robot_parts[i].setVelocity(robot_parts[i].getMaxVelocity() / 2.0)

# The Tiago robot has a couple more sensors than the e-Puck
# Some of them are mentioned below. We will use its LiDAR for Lab 5

# range = robot.getDevice('range-finder')
# range.enable(timestep)
# camera = robot.getDevice('camera')
# camera.enable(timestep)
# camera.recognitionEnable(timestep)
lidar = robot.getDevice('Hokuyo URG-04LX-UG01')
lidar.enable(timestep)
lidar.enablePointCloud()

# We are using a GPS and compass to disentangle mapping and localization
gps = robot.getDevice("gps")
gps.enable(timestep)
compass = robot.getDevice("compass")
compass.enable(timestep)

# We are using a keyboard to remote control the robot
keyboard = robot.getKeyboard()
keyboard.enable(timestep)

# The display is used to display the map. We are using 360x360 pixels to
# map the 12x12m2 apartment
display = robot.getDevice("display")

# Odometry
pose_x     = 0
pose_y     = 0
pose_theta = 0

vL = 0
vR = 0

lidar_sensor_readings = [] # List to hold sensor readings
lidar_offsets = np.linspace(-LIDAR_ANGLE_RANGE/2., +LIDAR_ANGLE_RANGE/2., LIDAR_ANGLE_BINS)
lidar_offsets = lidar_offsets[83:len(lidar_offsets)-83] # Only keep lidar readings not blocked by robot chassis

map = None
##### ^^^ [End] Do Not Modify ^^^ #####





def path_planner(maze, start, end):
    """
    Returns a list of tuples as a path from the given start to the given end in the given maze
    :param maze:
    :param start:
    :param end:
    :return:
    """
    #referenced: https://github.com/Viktoo/shortest-path-2D/blob/master/w.py
    numR = len(maze)
    numC = len(maze[0])
    
    sR = start[0]
    sC = start[1]
    
    eR = end[0]
    eC = end[1]
    
    
    dist = np.array([[float('inf') for x in range(numC)] for y in range(numR)])
    visited = np.array([[False for x in range(numC)] for y in range(numR)])
    prev = {(x,y):0 for x in range(numC) for y in range(numR)}
    
    # Provide directions
    dr = [-1, +1, 0, 0, -1, +1, -1, +1]
    dc = [0, 0, +1, -1, -1, -1, +1, +1]
    
    # Add priority queue data structure, key-value pairs (node index, dist)
    hd = heapdict()
    
    # Add first node and mark it visisted with no distance weight
    dist[sR,sC] = 0
    hd[sR,sC] = 0
    visited[sR,sC] = True
    
    reached_end = False
    #count = 0;
    while reached_end == False:
    # Tells you which node to visit next based on which key-value pair has the lowest value
        #count = count+1
        #print(count)
        if(len(hd) != 0):
            a = hd.popitem()
        else:
           path = []
           return path 
        
        r, c = a[0]
        for i in range (0,8):
            rr = r + dr[i]
            cc = c + dc[i]
    
            if rr < 0 or cc < 0: continue 
            if rr >= numR-1 or cc >= numC-1: continue
            if visited[rr,cc]: continue
            if maze[rr,cc] != 0: continue
            
    
            # Save parent node for use in path reconstruction
            node = (r,c)
            prev[rr,cc] = node 
    
            # Keep exploring until the end is reached
            if (rr,cc) == (eR, eC):
                reached_end = True 
                break 
    
            # The core of Dijkstras Algorithm
            d = int(dist[r,c]) + int(maze[rr,cc])
            hd[rr,cc] = d
            dist[rr,cc] = d
    
            visited[rr,cc] = True
        
    path = []
    xy = (eR, eC) 
    path.append(xy)
    while xy != start:
        path.append(prev[xy]) 
        xy = prev[xy]
    path.reverse()
    return path


##################### IMPORTANT #####################
# Set the mode here. Please change to 'autonomous' before submission
#mode = 'manual' # Part 1.1: manual mode
#mode = 'planner'
mode = 'autonomous'

###################
#
# Planner
#
###################
if mode == 'planner':
    # Part 2.3: Provide start and end in world coordinate frame and convert it to map's frame
    #start_w = (4.5,8.1) # (Pose_X, Pose_Z) in meters
    #end_w = None # (Pose_X, Pose_Z) in meters

    # Convert the start_w and end_w from the webots coordinate frame into the map frame
    start = (135,117) # (x, y) in 360x360 map
    end = (247, 174) # (x, y) in 360x360 map
    #note: the coordinates given in the writeup (300,210) in the world, were in a wall for our map for some reason, leading to a path not being found,so 
    #we chose the entrance for the bottom left bedroom. 
    
    
    # Part 2.1: Load map (map.npy) from disk and visualize it
    map = np.load("map.npy")

    
   
    inc = 8
    # Part 2.2: Compute an approximation of the “configuration space”
    for x in range(0,359):
        for y in range(0,359):
            #if there is an obstacle to go around
            if(map[x][y] == 1):
                #x increment
                for z in range(-inc,inc):
                    #y increment
                    for a in range(-inc, inc):
                        #if out of bounds, skip
                        if((x+z) > 359 or (y+a) > 359):
                            continue
                        #if not already an obstacle, fill it in to a value other than one to indicate that it is buffer
                        if(map[x+z][y+a] != 1):
                            map[x+z][y+a] = 2
                 
       
    np.save("approxmap.npy", map)

    # Part 2.3 continuation: Call path_planner
    
    paths = path_planner(map, start, end)
    
    
    # Part 2.4: Turn paths into waypoints and save on disk as path.npy and visualize it
    #map[135][117] = 10
    #map[300][210] = 10
    for x in paths:
        map[x[0]][x[1]] = 4
        
    plt.imshow(map)
    plt.show()  
    np.save("pathcoord.npy",paths)
    waypoints = np.multiply(paths, (1/30))
    np.save("pathfloat.npy", waypoints)

######################
#
# Map Initialization
#
######################

# Part 1.2: Map Initialization

# Initialize your map data structure here as a 2D floating point array
map = np.empty(shape=(360,360), dtype = np.single) # Replace None by a numpy 2D floating point array

waypoints = []

if mode == 'autonomous':
    # Part 3.1: Load path from disk and visualize it
    map = np.load("approxmap.npy")
    waypointscoord = np.load("pathcoord.npy")
    
    waypointsfloat = np.load("pathfloat.npy")

        
   
    goalpath = []
    
    #goaldisplay = []
    #length = len(waypointsfloat)
    
    #goalpath.append(waypointsfloat[0])
    #for x in range(0,length):
        #if ((x % 150) == 0):
            #goalpath.append(waypointsfloat[x])
    #goalpath.append(waypointsfloat[len(waypointsfloat)-1])
    
    #goaldisplay.append(waypointscoord[0])
    #for x in range(0,len(waypointscoord)):
        #if ((x % 150) == 0):
            #goaldisplay.append(waypointscoord[x]) 
    #goaldisplay.append(waypointscoord[len(waypointscoord)-1])
    
    #to visualize portions of the path
    #for x in goaldisplay:
        #map[x[0]][x[1]] = 4
    
    #to visualize whole path
    for x in waypointscoord:
        map[x[0]][x[1]] = 4
        
    #plt.imshow(map)
    #plt.show()
    #for x in waypointsfloat:
        #print(x)
    
    state = 0
    #print(waypointsfloat[state][0] , waypointsfloat[state][1])
    #print("done")



while robot.step(timestep) != -1 and mode != 'planner':

    ###################
    #
    # Mapping
    #
    ###################

    ################ v [Begin] Do not modify v ##################
    # Ground truth pose
    pose_y = gps.getValues()[2]
    pose_x = gps.getValues()[0]

    n = compass.getValues()
    rad = -((math.atan2(n[0], n[2]))-1.5708)
    pose_theta = rad

    lidar_sensor_readings = lidar.getRangeImage()
    lidar_sensor_readings = lidar_sensor_readings[83:len(lidar_sensor_readings)-83]

    for i, rho in enumerate(lidar_sensor_readings):
        alpha = lidar_offsets[i]

        if rho > LIDAR_SENSOR_MAX_RANGE:
            continue

        # The Webots coordinate system doesn't match the robot-centric axes we're used to
        rx = math.cos(alpha)*rho
        ry = -math.sin(alpha)*rho

        # Convert detection from robot coordinates into world coordinates
        wx =  math.cos(pose_theta)*rx - math.sin(pose_theta)*ry + pose_x
        wy =  -(math.sin(pose_theta)*rx + math.cos(pose_theta)*ry) + pose_y
    
        ################ ^ [End] Do not modify ^ ##################

        #print("Rho: %f Alpha: %f rx: %f ry: %f wx: %f wy: %f" % (rho,alpha,rx,ry,wx,wy))

        if rho < LIDAR_SENSOR_MAX_RANGE:
            # Part 1.3: visualize map gray values.
            
            y = 360-int(wy*30)
            x = int(wx*30)
            if x <= 0 or x >= 360 or y <= 0 or y >= 360:
                x = 1
                y = 1
                continue
            
            if map[x][y] <= .9999:
                map[x][y] = map[x][y] + .005
                    #print(value)
            
           
            g = int(np.nan_to_num(map[x][y]) * 255) # converting [0,1] to grayscale intensity [0,255]
        
            if(g > 255):
                g = 255
            
            try:
                color = int(g*256**2+g*256+g)
                display.setColor(color)
                display.drawPixel(y ,x)
            except:
                pass
            
            #print(color)
            # You will eventually REPLACE the following 2 lines with a more robust version of the map
            # with a grayscale drawing containing more levels than just 0 and 1.
            #if(color > 256):
                #color = 0
            

    # Draw the robot's current pose on the 360x360 display
        display.setColor(int(0xFF0000))
        display.drawPixel(360-int(pose_y*30),int(pose_x*30))



    ###################
    #
    # Controller
    #
    ###################
    if mode == 'manual':
        key = keyboard.getKey()
        while(keyboard.getKey() != -1): pass
        if key == keyboard.LEFT :
            vL = -MAX_SPEED
            vR = MAX_SPEED
        elif key == keyboard.RIGHT:
            vL = MAX_SPEED
            vR = -MAX_SPEED
        elif key == keyboard.UP:
            vL = MAX_SPEED
            vR = MAX_SPEED
        elif key == keyboard.DOWN:
            vL = -MAX_SPEED
            vR = -MAX_SPEED
        elif key == ord(' '):
            vL = 0
            vR = 0
        elif key == ord('S'):
            # Part 1.4: Filter map and save to filesystem
            newmap = map > .5
            newmap = np.multiply(newmap, 1)
            np.save("map.npy", newmap)
            print("Map file saved")
        elif key == ord('L'):
            # You will not use this portion in Part 1 but here's an example for loading saved a numpy array
            map = np.load("map.npy")
            print("Map loaded")
        else: # slow down
            vL *= 0.75
            vR *= 0.75
    else: # not manual mode
        # Part 3.2: Feedback controller
        #STEP 1: Calculate the error
        #print(waypointsfloat[state][0] , waypointsfloat[state][1])
        p = math.sqrt(pow((pose_x - waypointsfloat[state][0]),2) + pow((pose_y - waypointsfloat[state][1]),2)) #pos
        a = -math.atan2((waypointsfloat[state][1] - pose_y),(waypointsfloat[state][0] - pose_x)) + pose_theta
        
        #we narrowed down the problem to something with our rho, it almost always returns a huge value regardless of how
        #close it was to the target. even after checking over our waypoints being passed in and our rho equation however,
        #we were still unable to find the source of the problem. 
        #print(p)
        if(p < .3): #increase goal array position
           # print("incrementing")
            state += 1 
            if(state == len(waypointsfloat)):
                vL = 0
                vR = 0
                break

        
        #STEP 2: Controller    
        
        if(a > .25):
            xr = 0
            theta_r = 10 * a
        else:
            xr = 10
        
        theta_r = 5 * a    
            
        
        vL = ((xr) - (theta_r * AXLE_LENGTH)/2)
        vR = ((xr) + (theta_r * AXLE_LENGTH)/2) 
        pass    
        
        
    if(vR > MAX_SPEED/2):
        vR = MAX_SPEED/2
    if(vR < -MAX_SPEED/2): #pos and negative clamp for vR
        vR = -MAX_SPEED/2
    if(vL > MAX_SPEED/2):
        vL = MAX_SPEED/2
    if(vL < -MAX_SPEED/2): #pos and negative clamp for vL
        vL = -MAX_SPEED/2
        

    


    # Odometry code. Don't change vL or vR speeds after this line.
    # We are using GPS and compass for this lab to get a better pose but this is how you'll do the odometry
    pose_x += (vL+vR)/2/MAX_SPEED*MAX_SPEED_MS*timestep/1000.0*math.cos(pose_theta)
    pose_y -= (vL+vR)/2/MAX_SPEED*MAX_SPEED_MS*timestep/1000.0*math.sin(pose_theta)
    pose_theta += (vR-vL)/AXLE_LENGTH/MAX_SPEED*MAX_SPEED_MS*timestep/1000.0
    
    #print("X: %f Z: %f Theta: %f" % (pose_x, pose_y, pose_theta))
    
    if pose_theta > 6.28+3.14/2: pose_theta -= 6.28
    if pose_theta < -3.14: pose_theta += 6.28
    
    # Actuator commands
    robot_parts[MOTOR_LEFT].setVelocity(vL)
    robot_parts[MOTOR_RIGHT].setVelocity(vR)
