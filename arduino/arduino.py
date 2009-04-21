import serial
import time

# hello, world, v2

class arduino:
    def __init__(self):
        self.compass=0
        self.battery=0
        self.frontBumper=0
        self.backBumper=0
        self.poop=0
        self.frontRange=0
        self.backRange=0
        self.motorPower=0
        # find yer port!
        self.ser=serial.Serial( 'COM4' ,19200,timeout=0)
        print("serial port open")

    '''
    from the arduino program:
    these are the indexes of the data in the serial string
    #define compassHighByte 0
    #define compassLowByte 1
    #define batteryLevel 2
    #define frontBumper 3
    #define backBumper 4
    #define poopLevel 5
    #define frontRange 6
    #define backRange 7
    #define motorPower 8

    the checksum is at position 9
    '''



    def getData(self):
        self.ser.write('\n')
        time.sleep(.05)
        line=self.ser.read(12)
        print(line)
        self.compass=ord(line[0])*256+ord(line[1])
        self.battery=ord(line[2])
        self.frontBumper=ord(line[3])
        self.backBumper=ord(line[4])
        self.poop=ord(line[5])
        self.frontRange=ord(line[6])
        self.backRange=ord(line[7])
        self.motorPower=ord(line[8])
        

    def close(self):
        self.ser.close()

    def poop(self):
        print("poop!")
    
