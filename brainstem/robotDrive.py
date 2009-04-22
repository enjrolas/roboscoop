import serial
import time

'''
To use this class, you'd create a motor object:
myMotor=motor()

To tell the motor to move to a position target (i.e. 'go 200 clicks'), 
'''

positionP=1
positionI=0
positionD=0

velocityP=1
velocityI=0
velocityD=0

class robotDrive:
    def __init__(self):
        self.front=serial.Serial( '/dev/tty.usbserial-A60087za' ,9600,timeout=0)
        self.back=serial.Serial( '/dev/tty.usbserial-A60087PJ' ,9600,timeout=0)
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


#formats PID variables in the silly-ish brainstem format
    def formatVariable(self, variable):
        formatted=0
        fraction=variable-int(variable)
        for i in range(0,32):   #the bottom 5 bits of the formatted number are fractional bits representing 32ths
            if((i/32)>fraction):
                formatted+=i
                goto .exit
        label .exit
        formatted+=int(variable)*32
        return formatted

#writes PID values to the brainstem
    def motorConfigure(self, P, I, D):
        pString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(int(P/256))+chr(P%256)
        self.front.write(pString)
        self.back.write(pString)
        pString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(int(P/256))+chr(P%256)
        self.front.write(pString)
        self.back.write(pString)

        iString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(int(I/256))+chr(I%256)
        self.front.write(iString)
        self.back.write(iString)
        iString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(int(I/256))+chr(I%256)
        self.front.write(iString)
        self.back.write(iString)

        dString=chr(4)+chr(5)+chr(63)+chr(0)+chr(1)+chr(int(D/256))+chr(D%256)
        self.front.write(dString)
        self.back.write(dString)
        dString=chr(4)+chr(5)+chr(63)+chr(1)+chr(1)+chr(int(D/256))+chr(D%256)
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
            value=invert(self,-1*value)
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
            value=invert(self,-1*value)
            param1=int(value/256)
            param2=value%256
        else:
            param1=int(value/256)
            param2=value%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(0)+chr(param1)+chr(param2)
        self.front.write(moveString)
        self.back.write(moveString)

    def position(self, setpoint):  #move both motors a <setpoint> number of clicks
        self.positionMode(self)
        self.motorConfigure(self,formatVariables(self, self.positionP), formatVariables(self, self.positionI), formatVariables(self, self.positionD))
        leftPosition(self, setpoint)
        rightPosition(self, setpoint)
        
    def velocity(self, velocity):
        self.velocityMode(self)
        self.motorConfigure(self,formatVariables(self, self.velocityP), formatVariables(self, self.velocityI), formatVariables(self, self.velocityD))
        self.leftValue(self, velocity)
        self.rightValue(self, velocity)


    def close(self):
        self.ser.close()

    def poop(self):
        print("poop!")
    
