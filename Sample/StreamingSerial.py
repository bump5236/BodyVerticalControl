import serial
import signal
import sys
import numpy as np
# import quaternion
from NatNetClient import NatNetClient


def setupserial(serialPort, baudRate):

    global ser, pos1, pos2

    ser = serial.Serial(serialPort, baudRate)
    print("Open port.")

    pos1 = None
    pos2 = None

# This is a callback function that gets connected to the NatNet client and called once per mocap frame.
def receiveNewFrame( frameNumber, markerSetCount, unlabeledMarkersCount, rigidBodyCount, skeletonCount,
                    labeledMarkerCount, latency, timecode, timecodeSub, timestamp, isRecording, trackedModelsChanged ):
    # print("frameNumber", frameNumber)
    # print( "timestamp", timestamp )
    pass

# This is a callback function that gets connected to the NatNet client. It is called once per rigid body per frame
def receiveRigidBodyFrame( id, position, rotation ):
    print("id", id)
    print("position", position)
    # print("rotation", rotation)
    # quat = np.quaternion(rotation[3], rotation[0], rotation[1], rotation[2]*(-1))
    # euler = quaternion.as_euler_angles(quat)
    # print("euler", euler)

    global ser, pos1, pos2

    if id == 1: # Rigid Body 1
        if pos1 == None:
            pos1 = position[1]

        if (position[1] <= 0.09) & (position[1] <= pos1):
            ser.write(b"1")

        else:
            ser.write(b"0")

        pos1 = position[1]

    elif id == 2: # Rigid Body 2
        if pos2 == None:
            pos2 = position[1]

        if (position[1] <= 0.09) & (position[1] <= pos2):
            ser.write(b"2")

        else:
            ser.write(b"0")

        pos2 = position[1]


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
