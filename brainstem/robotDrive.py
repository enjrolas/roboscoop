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

    velocityP=32
    velocityI=0
    velocityD=5

    positionP=32
    positionI=0
    positionD=5

    rotateP=32
    rotateI=0
    rotateD=5
    
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
    def velocityMode(self, p, i, d):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)

        highP=int(p/256)
        lowP=p%256

        highI=int(i/256)
        lowI=i%256

        highD=int(d/256)
        lowD=d%256

        #set P value
        pString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(highP)+chr(lowP) #left side
        self.front.write(pString)
        self.back.write(pString)
        pString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(highP)+chr(lowP) #right side
        self.front.write(pString)
        self.back.write(pString)

        #set I value
        iString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(highI)+chr(lowI) #left side
        self.front.write(iString)
        self.back.write(iString)
        iString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(highI)+chr(lowI) #right side
        self.front.write(iString)
        self.back.write(iString)


        #set D value
        dString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(highD)+chr(lowD) #left side
        self.front.write(dString)
        self.back.write(dString)
        dString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(highD)+chr(lowD) #right side
        self.front.write(dString)
        self.back.write(dString)

        
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
    def leftValue(self,setpoint):
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

    def rightValue(self,setpoint):
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

    def position(self, setpoint, positionP, positionI, positionD):  #move both motors a <setpoint> number of clicks
        self.positionMode()
        self.leftValue(-1*setpoint)
        self.rightValue(setpoint)

    def velocity(self, velocity):
        self.velocityMode(self.velocityP, self.velocityI, self.velocityD)
        self.leftValue(-1*velocity)
        self.rightValue(velocity)

    def rotate(self, speed, rotateP, rotateI, rotateD):
        self.velocityMode()
        self.leftValue(speed)
        self.rightValue(speed)
    

    def rampConfig(self):
        self.velocityMode()

    def eepromReset(self):
        resetString=chr(4)+chr(16)+chr(0)+chr(0)+chr(255)
        self.front.write(resetString)
        resetString=chr(4)+chr(24)
        self.front.write(resetString)
        
    def close(self):
        self.front.close()
        self.back.close()
    def poop(self):
        print("poop!")
    
