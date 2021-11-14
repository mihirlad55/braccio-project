#!/usr/bin/env python3

# Path to Arduino serial port
SERIAL_PORT_PATH = '/dev/ttyACM0'

# ArUco Marker Ids
ARUCO_IDS = [200, 300, 400]

DEBUG = True

import numpy as np
import cv2
import time
import serial

from cv2 import aruco
from picamera.array import PiRGBArray
from picamera import PiCamera


def main():
    # Connect to Arduino over serial
    print("Connecting to serial...")
    serial_handle = serial.Serial(SERIAL_PORT_PATH, 9600, timeout=1)
    print("Connected to serial!")

    # Initialize and set up Pi camera
    print("Initializing camera...")
    camera = PiCamera()
    camera.resolution = (1920, 1080)
    camera.framerate = 10
    raw_capture = PiRGBArray(camera, size=(1920, 1080))

    # Wait for camera to warm up
    time.sleep(0.1)
    print("Camera initialized!")

    # Continuously capture frames from the camera
    for frame in camera.capture_continuous(raw_capture, format="bgr"):
        # Get array for cv2
        image = frame.array

        if DEBUG:
            cv2.imwrite('/home/mihirlad55/frame.png', image)

        # Convert to gray image
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        if DEBUG:
            cv2.imwrite('/home/mihirlad55/frame-gray.png', image)

        # Get aruco code IDs and coordinates of corners
        aruco_dict = aruco.Dictionary_get(aruco.DICT_ARUCO_ORIGINAL)
        parameters = aruco.DetectorParameters_create()
        corners, detected_ids, _ = aruco.detectMarkers(gray_image,
                                                       aruco_dict,
                                                       parameters=parameters)

        # Create list of detected_ids
        detected_ids = list(np.array(detected_ids).reshape((-1,)))

        if DEBUG:
            print("Markers detected: ", detected_ids)

        #if DEBUG:
        #   cv2.imshow('gray', gray_frame)
        #   cv2.waitKey(1)

        if DEBUG:
            print(serial_handle.read_all())

        is_object_detected = False
        for id_ in ARUCO_IDS:
            if id_ not in detected_ids:
                # Send a byte with value 1 to Arduino to indicate object detected
                serial_handle.write(b'\x01')
                is_object_detected = True
                print("Object detected")
                break

        if not is_object_detected:
            # Send a byte with value 0 to Arduino to indicate no object detected
            serial_handle.write(b'\x00')
            print("No object detected!")

        # Prepare for next frame
        raw_capture.truncate(0)

        # 10 FPS
        time.sleep(0.1)

if __name__ == "__main__":
    main()
