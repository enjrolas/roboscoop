import serial
import time

'''
To use this class, you'd create a motor object:
myMotor=motor()

To tell the motor to move to a position target (i.e. 'go 200 clicks'), 
'''

class motor:
    def __init__(self,port):
        self.ser=serial.Serial( port ,9600,timeout=0)
        print("serial port open")

#initialized position feedback mode
    def positionMode(self):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(5)+chr(0)
        self.ser.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(5)+chr(0)
        self.ser.write(initString)
        print("In position mode")

#initializes velocity feedback mode
    def velocityMode(self):
        initString=chr(4)+chr(5)+chr(63)+chr(0)+chr(0)+chr(7)+chr(0)
        self.ser.write(initString)
        initString=chr(4)+chr(5)+chr(63)+chr(1)+chr(0)+chr(7)+chr(0)
        self.ser.write(initString)
        print("In velocity mode")
        
    def leftPosition(self,setpoint):
        self.positionMode()
        if setpoint<0:
            setpoint*=-1
            param1=128+int(setpoint/256)
            param2=setpoint%256
        else:
            param1=int(setpoint/256)
            param2=setpoint%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(1)+chr(param1)+chr(param2)
        self.ser.write(moveString)
        print(param1, param2)

    def rightPosition(self,setpoint):
        self.positionMode()
        if setpoint<0:
            setpoint*=-1
            param1=128+int(setpoint/256)
            param2=setpoint%256
        else:
            param1=int(setpoint/256)
            param2=setpoint%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(0)+chr(param1)+chr(param2)
        self.ser.write(moveString)
        print(param1, param2)

    def position(self, setpoint):  #move both motors a <setpoint> number of clicks
        self.positionMode()
        if setpoint<0:
            setpoint*=-1
            param1=128+int(setpoint/256)
            param2=setpoint%256
        else:
            param1=int(setpoint/256)
            param2=setpoint%256            
        moveString=chr(4)+chr(4)+chr(62)+chr(0)+chr(param1)+chr(param2)
        moveString=chr(4)+chr(4)+chr(62)+chr(1)+chr(param1)+chr(param2)
        self.ser.write(moveString)
        print(param1, param2)
        
    def velocity(self, velocity):
        self.velocityMode()


    def close(self):
        self.ser.close()

    def poop(self):
        print("poop!")
    
