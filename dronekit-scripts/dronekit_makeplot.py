# Import DroneKit-Python
from dronekit import connect, Command, LocationGlobal
from pymavlink import mavutil
import time, sys, argparse, math, matplotlib, threading, requests
import matplotlib.pyplot as plt

start_time = time.time()
times = []
distances_east = []
distances_north = []
distances_down = []
has_finished = False

def update_values():
	t = threading.Timer(2.0, update_values)
	t.start()
	if(has_finished == True):
		t.cancel()
	if(vehicle.armed == True):
		times.append(time.time() - start_time)
		distances_east.append(vehicle.location.local_frame.east)
		distances_north.append(vehicle.location.local_frame.north)
		distances_down.append(vehicle.location.local_frame.down)
		print "Updating values"

def upload_file():
	headers = {'Content-Type' : 'text/xml',
				'User-agent' : 'Mozilla/5.0'} #or 'application/xml'
	sample_file = open("13_12_28.ulg", "rb")
	upload_file = {"Uploaded file": sample_file}
	r = requests.post("https://logs.px4.io/upload", data={'upload-button' : 'Upload'}, files={'filearg' : sample_file}, headers=headers)
	print(r.text)

upload_file()

# Connect to the Vehicle
print "Connecting"
connection_string = '127.0.0.1:14540'
vehicle = connect(connection_string, wait_ready=True)

update_values()

while(vehicle.armed == False):
	#catch drone
	continue

while(True):
	if(vehicle.armed == False):
		break

has_finished = True

print "Distance north:"
print distances_north
print "Distance east:" 
print distances_east
print "Distance down:"
print distances_down
figure, axis = plt.subplots(2,2)

axis[0,0].plot(times, distances_north)
axis[0,0].set_title("Distance North/time")
axis[0,1].plot(times, distances_east)
axis[0,1].set_title("Distance East/time")
axis[1,0].plot(times, distances_down)
axis[1,0].set_title("Distance Down/time")
#plt.plot(times, distances_east)
#plt.xlabel('Time')
#plt.ylabel('Distance East')
#plt.savefig('distance_east.png')

#plt.plot(times, distances_north)
#plt.xlabel('Time')
#plt.ylabel('Distance North')
#plt.savefig('distance_north.png')

#plt.plot(times, distances_down)
#plt.xlabel('Time')
#plt.ylabel('Distance Down')
plt.savefig('local_distances.png')

# Load commands
cmds = vehicle.commands
cmds.clear()

home = vehicle.location.global_relative_frame

# takeoff to 10 meters
wp = get_location_offset_meters(home, 0, 0, 10);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_TAKEOFF, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# move 10 meters north
wp = get_location_offset_meters(wp, 10, 0, 0);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_WAYPOINT, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# move 10 meters east
wp = get_location_offset_meters(wp, 0, 10, 0);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_WAYPOINT, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# move 10 meters south
wp = get_location_offset_meters(wp, -10, 0, 0);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_WAYPOINT, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# move 10 meters west
wp = get_location_offset_meters(wp, 0, -10, 0);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_WAYPOINT, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# land
wp = get_location_offset_meters(home, 0, 0, 10);
cmd = Command(0,0,0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_LAND, 0, 1, 0, 0, 0, 0, wp.lat, wp.lon, wp.alt)
cmds.add(cmd)

# Upload mission
cmds.upload()