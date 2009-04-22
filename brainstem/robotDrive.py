import serial
import time

'''
To use this class, you'd create a motor object:
myMotor=motor()

To tell the motor to move to a position target (i.e. 'go 200 clicks'), 
'''


class robotDrive:
    def __init__(self):
        self.positionP=1
        self.positionI=0
        self.positionD=0

        self.velocityP=1
        self.velocityI=0
        self.velocityD=0

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
        self.motorConfigure(self.formatVariable(self.positionP), self.formatVariable(self.positionI), self.formatVariable(self.positionD))        
        print("In position mode")

#initializes velocity feedback mode
    def velocityMode(self):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(7)+chr(0)
        self.front.write(initString)
        self.back.write(initString)
        self.motorConfigure(self.formatVariable(self.velocityP), self.formatVariable(self.velocityI), self.formatVariable(self.velocityD))        
        print("In velocity mode")
        


#formats PID variables in the silly-ish brainstem format
    def formatVariable(self, variable):
        formatted=0
        done=0
        fraction=variable-int(variable)
        for i in range(0,32):   #the bottom 5 bits of the formatted number are fractional bits representing 32ths
            if((i/32)>fraction) and (done==0):
                formatted+=i
                done=1
        formatted+=int(variable)*32
        return formatted

#writes PID values to the brainstem
    def motorConfigure(self, P, I, D):
        print P
        print I
        print D
        pString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(int(P/256))+chr(P%256)
        self.front.write(pString)
        self.back.write(pString)
        pString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(int(P/256))+chr(P%256)
        self.front.write(pString)
        self.back.write(pString)

        iString=chr(4)+chr(5)+chr(63)+chr(0)+chr(2)+chr(int(I/256))+chr(I%256)
        self.front.write(iString)
        self.back.write(iString)
        iString=chr(4)+chr(5)+chr(63)+chr(1)+chr(2)+chr(int(I/256))+chr(I%256)
        self.front.write(iString)
        self.back.write(iString)

        dString=chr(4)+chr(5)+chr(63)+chr(0)+chr(3)+chr(int(D/256))+chr(D%256)
        self.front.write(dString)
        self.back.write(dString)
        dString=chr(4)+chr(5)+chr(63)+chr(1)+chr(3)+chr(int(D/256))+chr(D%256)
        self.front.write(dString)
        self.back.write(dString)
        
        
    def invert(self, number):  #returns the 2s complement negative of a positive 16-bit number
        inverse=0;
        for i in range(15,-1, -1):
            if(number>=2**i):
                number-=2**i
            else:
                inverse+=2**i
        inverse+=1
        if(inverse >= 2**16):
                inverse-=2**16
        return inverse
        
                
        
    def leftValue(self,value):
        if value<0:
            value=self.invert(-1*value)
            param1=int(value/256)
            param2=value%256
        else:
            param1=int(value/256)
            param2=value%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(1)+chr(param1)+chr(param2)
        self.front.write(moveString)
        self.back.write(moveString)


    def rightValue(self,value):
        if value<0:
            value=self.invert(-1*value)
            param1=int(value/256)
            param2=value%256
        else:
            param1=int(value/256)
            param2=value%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(0)+chr(param1)+chr(param2)
        self.front.write(moveString)
        self.back.write(moveString)

    def position(self, setpoint):  #move both motors a <setpoint> number of clicks
        self.positionMode()
        self.leftValue(setpoint)
        self.rightValue(-1*setpoint)
        
    def velocity(self, velocity):
        self.velocityMode()
        self.leftValue(velocity)
        self.rightValue(-1*velocity)

    def rotatePosition(self, angle):
        self.positionMode()
        self.leftValue(angle)
        self.rightValue(angle)

    def rotateVelocity(self, velocity):
        self.velocityMode()
        self.leftValue(velocity)
        self.rightValue(velocity)

    def close(self):
        self.front.close()
        self.back.close()

    def poop(self):
        print("poop!")
    
