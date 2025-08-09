import serial
import time

class Comm:
    def __init__(self):
        self.port = None

    def is_connected(self):
        return self.port != None and self.port.is_open
    
    def connect(self, port_name):
        if self.is_connected(): return False

        port = serial.Serial(port_name)
        if port.is_open:
            self.port = port
            return True
        else:
            return False
        
    def disconnect(self):
        if self.is_connected():
            self.port.close()
            self.port = None

    def set_motors(self, motor_1, motor_2, motor_3, motor_4):
        if not self.is_connected(): return

        directions = 0
        if motor_1 > 0: directions |= (1 << 0)
        if motor_2 > 0: directions |= (1 << 1)
        if motor_3 > 0: directions |= (1 << 2)
        if motor_4 > 0: directions |= (1 << 3)

        msg = bytes([1, directions, abs(motor_1), abs(motor_2), abs(motor_3), abs(motor_4)])
        self.port.write(msg)

    def readline(self):
        return self.port.readline() # TODO(Richo): Just for debugging...

comm = Comm()
comm.connect("COM26")

comm.set_motors(32, 64, 128, 255)
print(comm.readline())
time.sleep(2)

comm.set_motors(0, 0, 0, 0)
print(comm.readline())
time.sleep(2)

comm.set_motors(-255, -128, -64, -32)
print(comm.readline())
time.sleep(2)

comm.disconnect()
