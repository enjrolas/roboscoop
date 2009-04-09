import serial
import time

'''
To use this class, you'd create a robotDrive object:
robot=robotDrive()

To tell the robot to move to a position target (i.e. 'go 200 clicks'),
type robot.position(200)

to move at a certain velocity (4 clicks/sec), type robot.velocity(4)
'''

class robotDrive:
    def __init__(self):
        self.front=serial.Serial( 'COM5' ,9600,timeout=0)
        self.back=serial.Serial( 'COM6' ,9600,timeout=0)
        print("serial port open")

#initialized position feedback mode
    def positionMode(self):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(5)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(5)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        print("In position mode")

#initializes velocity feedback mode
    def velocityMode(self):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        print("In velocity mode")

    #return the 15-bit bitwise inverted representation of a number
    def invert(self, number):
        inverse=32767
        for exponent in range(15,-1,-1):
            if(number>=2**exponent):
                inverse-=2**exponent
                number-=2**exponent
        return inverse
    
    #so--the brainstem docs are wrong about how the number going to the stem
    #is represented--it's just a 2s complement negative number, not a weird bitshifted
    #number they describe
    def leftPosition(self,setpoint):
        if setpoint<0:
            setpoint*=-1
            setpoint-=1
            param1=128+(self.invert(setpoint)>>8)
            param2=(self.invert(setpoint)&255)
        else:
            param1=setpoint>>8
            param2=(setpoint&255)
        moveString=chr(4)+chr(4)+chr(62)+chr(1)+chr(param1)+chr(param2)
        self.front.write(moveString)
        self.back.write(moveString)
        print(param1, param2)

    def rightPosition(self,setpoint):
        if setpoint<0:
            setpoint*=-1
            setpoint-=1
            param1=128+(self.invert(setpoint)>>8)
            param2=(self.invert(setpoint)&255)
        else:
            param1=setpoint>>8
            param2=(setpoint&255)            
        moveString=chr(4)+chr(4)+chr(62)+chr(0)+chr(param1)+chr(param2)
        self.front.write(moveString)
        self.back.write(moveString)

    def position(self, setpoint):  #move both motors a <setpoint> number of clicks
        self.positionMode()
        self.leftPosition(-1*setpoint)
        self.rightPosition(setpoint)

    def velocity(self, velocity):
        self.velocityMode()


    def close(self):
        self.ser.close()

    def poop(self):
        print("poop!")
    
