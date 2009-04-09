class robotMotors:
    def __init__(self):
        front=motor('COM5')
        back=motor('COM6')

    def position(self, setpoint):
        front.position(setpoint)
        back.position(setpoint)
        
    def velocity(self, velocity):
        front.velocity(velocity)
        back.velocity(velocity)

    
