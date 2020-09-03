import serial
import signal
import sys
import numpy as np
from NatNetClient import NatNetClient
from scipy.spatial.transform import Rotation as R

def quaternion_to_euler_zyx(q):
    r = R.from_quat([q[0], q[1], q[2], q[3]])
    return r.as_euler('zyx', degrees=True)

def setupserial(serialPort, baudRate):

    global ser, pos1, pos2

    ser = serial.Serial(serialPort, baudRate)
    print("Open port.")

    angle_z = None

# This is a callback function that gets connected to the NatNet client and called once per mocap frame.
def receiveNewFrame( frameNumber, markerSetCount, unlabeledMarkersCount, rigidBodyCount, skeletonCount,
                    labeledMarkerCount, latency, timecode, timecodeSub, timestamp, isRecording, trackedModelsChanged ):
    # print("frameNumber", frameNumber)
    # print( "timestamp", timestamp )
    pass

# This is a callback function that gets connected to the NatNet client. It is called once per rigid body per frame
def receiveRigidBodyFrame( id, position, rotation ):
    # print("id", id)
    # print("position", position)
    # print("rotation", rotation)
    euler_z, euler_y, euler_x = quaternion_to_euler_zyx(rotation)
    print("euler_z:", euler_z)

    global ser, angle_z

    if id == 1: # Rigid Body 1
        if angle_z == None:
            angle_z = euler_z

        if (position[1] <= 0.09) & (position[1] <= pos1):
            ser.write(b"1")

        else:
            ser.write(b"0")

        pos1 = position[1]


signal.signal(signal.SIGINT, signal.SIG_DFL)

# Change this value to the IP address of the NatNet server.
# serverIPAddress = "127.0.0.1"   #ローカル通信
serverIPAddress = "133.68.35.190" #UDP通信

# Change this value to the IP address of your local network interface
# localIPAddress = "127.0.0.1"
localIPAddress = "133.68.35.190"

# This should match the multicast address listed in Motive's streaming settings.
multicastAddress = "239.255.42.99"

# NatNet Command channel
commandPort = 1510

# NatNet Data channel
dataPort = 1511

serialPort = "COM8"

baudRate = 115200

setupserial(serialPort, baudRate)

# This will create a new NatNet client
streamingClient = NatNetClient(
    serverIPAddress, localIPAddress, multicastAddress, commandPort, dataPort)

# Configure the streaming client to call our rigid body handler on the emulator to send data out.
streamingClient.newFrameListener = receiveNewFrame
streamingClient.rigidBodyListener = receiveRigidBodyFrame

# Start up the streaming client now that the callbacks are set up.
# This will run perpetually, and operate on a separate thread.
streamingClient.run()
