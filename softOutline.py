# this is a psuedocode document for the RoboScoop code
# outlines primary states, classes, and hardware code

'''
revisions:
# - action. user. date.

1 - created document. wbosworth. 04/07/09.
2 - 

'''


def RoboScoop():
    '''
    is a switch board for the states
        + WanderYard
        + ManipulatePoop (which might be 2 or 3 states)
        + Error
        + Initialize

    has access to the following classes (organized by I/O pipelines)
        Arduino (all sensor i/o ; there may be more than one formal Arduino class)
        Motor (brainstem front ; brainstem back)
        Log (onboard error/success documentation)
        Remote (remote access stuff. this may not be formal python)

        <can create additional semantic wrappers for the above classes
        where helpful/applicble (like motorLeft and motorRight)
    '''

    def WanderYard():
        - robot drives straight until flagged by an OBSTACLE or POOP or ERROR
            - if OBSTACLE
                robot moves back a bit and turns some and
                continues in WanderYard
            - if POOP
                exit WanderYard with flag into ManipulatePoop
            - if ERROR
                exit WanderYard with flag to Error

        OBSTACLES:
            - from Arduino
                + electric fence
                + bump front
                + bump back (unlikely?)
                + range sensors
                + motorPower
            - from Motors (?)
                + encoder movement
        POOP:
            - from Arduino
                + poop a,b,c
        ERROR:
            - from Arduino
                + battery
                + motorPower
            - from alot of stuff
                + error flags

    def ManipulatePoop():
        ...

    def Error():
        ...

    def Log():
        ...
____________________________________________________________

# signal IO classes organized by COM port / data pipelines

class Arduino():
        # the arduino reads the following sensors
        bumpFront
        bumpBack
        compass
        poopA
        poopB
        poopC
        battery
        frontRange
        backRange
        motorPower
        manipulatorVarious (maybe its own arduino class)
        electricFence
        # the arduino commands the following actuators
        manipulatorVarious
        Xbee (?)

class robotDrive():
        # motor class commands BrainStem modules the following
        setVelocity(direction, speed)
        setPosition(direction, speed)
        # additional wrappers for the MotorIO class might be
        turn(angle)
            -> sets v/p of both Left and Right sides, and talks to arduino.compass
        straightV(direction, velocity)
        straightP(direction, distance, (velocity))
        # motor class recieves the following
        encoderMovement
            used to find out if the robot is stuck when it ought to be moving
            (is this possible?)
class Log():
        controls access to various form of logging.
        txt files...
        
class Remote():
        this is probably not a formal piece software piece
        
________________________________________________________________
# machine code 
Arduino microcontroller code:
    arduino code (arduino is a language and a piece of hardware!) preps the arduino
    for the specific I/O we are implementing. covers same I/O as arduino class above

Brainstem code:
    brainstem (motor drivers) come programmed from the factory. their machine interface
    is online. There are APIs, but there is also a serial library


        
