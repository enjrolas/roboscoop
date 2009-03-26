import sys, serial

def init():
    leftMotorSerial=Serial(0,19200)  #this connection talks to the motors on the 'bots
                                     #left-hand side
    rightMotorSerial=Serial(1,19200)#talks to the motors on the right-hand side
    arduinoSerial=Serial(2, 19200)   #talks to the arduino, which reports on the
                                     #compass, bump sensors, battery voltage and other
                                     #internal states of the 'bot
    
def move(speed):  #moves in a straight line forward or backward.  The command is
                  #open-loop, meaning it doesn't look at the encoder data as it moves
                  #to see if it's slipping.  A negative speed moves backwards, positive
                  #moves forwards
    

def turn(radius):  # a skid turn where the difference in the motor side speeds is
                   #inversely porpotional to the radius of the turn.  A negative
                   # radius turns left, positive turns right

def sharpTurn(speed):  #a turn in place where one side goes forward at <speed> while
                       #other side goes backwards.  Negative speed turns left,
                       #positive turns right

def quit():
    ser.close()
    

