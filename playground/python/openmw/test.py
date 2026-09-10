# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import pyb # Import module for board related functions
import sensor # Import the module for sensor related functions
import image # Import module containing machine vision algorithms
import time # Import module for tracking elapsed time

sensor.reset()  # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)  # Set pixel format to RGB565 (or GRAYSCALE)
id = sensor.get_id()
print(id)
sensor.set_framesize(sensor.QVGA)  # Set frame size to QVGA (640x480)
sensor.set_vflip(True) # Flips the image vertically
sensor.set_hmirror(True) # Mirrors the image horizontally
sensor.skip_frames(time=2000)  # Wait for settings take effect.

# Define the min/max LAB values we're looking for
thresholdsApple = (24, 60, 32, 54, 0, 42)
thresholdsBanana = (45, 75, 5, -10, 40, 12)
ledRed = pyb.LED(1) # Initiates the red led
clock = time.clock() # Instantiates a clock object

while(True):

    clock.tick() # Advances the clock
    img = sensor.snapshot() # Takes a snapshot and saves it in memory
    # Find blobs with a minimal area of 50x50 = 2500 px
    # Overlapping blobs will be merged
    blobs = img.find_blobs([thresholdsApple, thresholdsBanana], area_threshold=2500, merge=True)
    # Draw blobs

    for blob in blobs:
        # Draw a rectangle where the blob was found
        img.draw_rectangle(blob.rect(), color=(0,255,0))
        # Draw a cross in the middle of the blob
        img.draw_cross(blob.cx(), blob.cy(), color=(0,255,0))

    # Turn on green LED if a blob was found
    if len(blobs) > 0:
        ledRed.off()
    else:
    # Turn the red LED on if no blob was found
        ledRed.on()
    pyb.delay(50) # Pauses the execution for 50ms
    print(clock.fps()) # Prints the framerate to the serial console
