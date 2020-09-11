import serial
import signal
import sys
import math
import numpy as np
# import quaternion
from NatNetClient import NatNetClient
from scipy.spatial.transform import Rotation as R


def quaternion_to_euler_zyx(q):
    r = R.from_quat([q[0], q[1], q[2], q[3]])
    return r.as_euler('zyx', degrees=True)

def setupserial(serialPort, baudRate):

    global ser, euler_x, euler_y, euler_z

    ser = serial.Serial(serialPort, baudRate)
    print("Open port.")

    euler_x = None
    euler_y = None
    euler_z = None

# This is a callback function that gets connected to the NatNet client and called once per mocap frame.
def receiveNewFrame( frameNumber, markerSetCount, unlabeledMarkersCount, rigidBodyCount, skeletonCount,
                    labeledMarkerCount, latency, timecode, timecodeSub, timestamp, isRecording, trackedModelsChanged ):
    # print("frameNumber", frameNumber)
    # print( "timestamp", timestamp )
    pass

Kp = 0.05
base_torq = 0


# This is a callback function that gets connected to the NatNet client. It is called once per rigid body per frame
def receiveRigidBodyFrame( id, position, rotation ):

    global ser, euler_x, euler_y, euler_z

    # print("id", id)
    # print("position", position)
    # print("rotation", rotation)
    euler = quaternion_to_euler_zyx(rotation)

    # euler = quaternion.as_euler_angles(quat)
    print("euler", euler)

    if id == 1: # Rigid Body 1
        if euler_z == None:
            euler_z = euler[0]

        if euler[0] == 0:
            euler[0] = euler_z
        elif euler[0] == None:
            euler[0] = euler_z
        
        # Serial
        if euler[0] < 0:
            add_torq = - Kp*(euler[0])
        
        else :
            add_torq = 0
        
        # トルク
        tgt_torq = base_torq + add_torq

        # 電流値指令(int16_t)
        tgt_cur = math.floor((tgt_torq)/3.3 * 2000/12.5)

        s = str(euler_z) + '\1'
        ser.write(s.encode())

        s = str(tgt_cur) + '\2'
        ser.write(s.encode())

        euler_z = euler[0]
        euler_y = euler[1]
        euler_x = euler[2]

signal.signal(signal.SIGINT, signal.SIG_DFL)

# Change this value to the IP address of the NatNet server.
serverIPAddress = "127.0.0.1"   #ローカル通信
# serverIPAddress = "133.68.35.190" #UDP通信

# Change this value to the IP address of your local network interface
localIPAddress = "127.0.0.1"
# localIPAddress = "133.68.35.190"

# This should match the multicast address listed in Motive's streaming settings.
multicastAddress = "239.255.42.99"

# NatNet Command channel
commandPort = 1510

# NatNet Data channel
dataPort = 1511

serialPort = "COM11"

baudRate = 57600

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
