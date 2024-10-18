#!/usr/bin/env python

# Photocell input and parsing for Ras Pi
# Must be used with GPIO 0.3.1a or later - earlier versions aren't fast enough!
# Set for photocell input on pin 18 by default

import RPi.GPIO as GPIO, time, os, sys 

# Settings
IN_RC = 16     #Input pin
OUT_LOW = 21   #Low-light output
OUT_HIGH = 20  #High-light output
OUT_STATE = 26 #Program state output

DEBUG = 1
GPIO.setmode(GPIO.BCM)

GPIO.setup(OUT_LOW, GPIO.OUT)
GPIO.setup(OUT_HIGH, GPIO.OUT)
GPIO.setup(OUT_STATE, GPIO.OUT)

def RCtime (RCpin):
    reading = 0
    GPIO.setup(RCpin, GPIO.OUT)
    GPIO.output(RCpin, GPIO.LOW)
    time.sleep(0.1)

    GPIO.setup(RCpin, GPIO.IN)
    # This takes about 1 millisecond per loop cycle
    while (GPIO.input(RCpin) == GPIO.LOW):
        reading += 1
    return reading

def ledOut (state):
    if state == 0:
        GPIO.output(OUT_LOW, True)
        GPIO.output(OUT_HIGH, True)
        return 0
    if state == 1:
        GPIO.output(OUT_LOW, False)
        GPIO.output(OUT_HIGH, True)
        return 0
    if state == 2:
        GPIO.output(OUT_LOW, True)
        GPIO.output(OUT_HIGH, False)
        return 0
    if state == 3:
        GPIO.output(OUT_LOW, False)
        GPIO.output(OUT_HIGH, False)
        return 0
    return 1

def photocellParse(reading):
    out = ""
    if reading <= 65:
        out = "0"
        ledOut(0)
        return out
    if reading <= 150:
        out = "o"
        ledOut(1)
        return out
    if reading <= 350:
        out = "."
        ledOut(2)
        return out
    out = " "
    ledOut(3)
    return out

while True:
    GPIO.output(OUT_STATE, True)
    #sys.stdout.write(photocellParse(RCtime(IN_RC))) # Read RC timing using RC_IN pin, parse it, and spit it into stdout
    #print photocellParse(RCtime(IN_RC))
    print(RCtime(IN_RC))
    sys.stdout.flush()
    GPIO.output(OUT_STATE, False)