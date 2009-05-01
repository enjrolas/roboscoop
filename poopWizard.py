# poopWizard controls the robot reads (arduino.py) and writes to Brainstems (robotDrive.py)
# poop

# importing arduino & robot drives [check paths of cwd!]
import time
import sys
sys.path.append('C:\\Documents and Settings\\nub\\My Documents\\roboscoop\\arduino')
#sys.path.append('C:\\Documents and Settings\\wbosworth\\My Documents\\newPoop\\arduino')
from arduino import arduino
global a
a = arduino()

sys.path.append('C:\\Documents and Settings\\nub\\My Documents\\roboscoop\\brainstem')
#sys.path.append('C:\\Documents and Settings\\wbosworth\\My Documents\\newPoop\\brainstem')
from robotDrive import robotDrive
global bob
bob = robotDrive()


time.sleep(5) # give everythging a few seconds to settle down!!!!



def straightUntil():
    '''
    Set the robot to go straight until hitting an obstacle or until sensing poop.
    Exit modes designate whether it hit an obstacle or hit poop
    '''
    desiredVel = 10 # encoder clicks/sec
    bob.velocity(desiredVel)
    while(1):
        a.getData()
        #if(a.poopSensed == 255):
        #    return 'poop'
        if a.frontBumper == 0:
            return 'bumper'

def backAndTurn():
    '''
move backwards a small amount and then turn an arbitrary amount (random value??)
return 'completed action', 'poopSensed', or 'timeOut'
'''
    bob.velocity(0)
    bob.position(-500)
    time.sleep(3)  #get rid of me!
    bob.rotatePosition(300)
    time.sleep(3)
    # right now this function does nothing!

def stateMachine():
    while(1):
        reasonForBreak = straightUntil()
        if reasonForBreak == 'bumper':
            backAndTurn()
        elif resonForBreak == 'poop':
            bob.velocity(0)
            time.sleep(5)
        else:
            placeHolder = None




# the main loop
if __name__ == "__main__":
    stateMachine()
