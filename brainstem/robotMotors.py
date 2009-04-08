class robotMotors:
    def __init__(self):
        front=motor('/dev/tty.usbserial-A60087za')
        back=motor('/dev/tty.usbserial-A60087PJ')

    def position(self, setpoint):
        front.position(setpoint)
        back.position(setpoint)
        
    def velocity(self, velocity):
        front.velocity(velocity)
        back.velocity(velocity)

    
