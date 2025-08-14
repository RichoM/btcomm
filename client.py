import serial
import time
import threading

class Comm:
    def __init__(self):
        self.port = None

    def is_connected(self):
        return self.port != None and self.port.is_open
    
    def connect(self, port_name):
        if self.is_connected(): return False

        port = serial.Serial(port_name, baudrate=9600)
        if port.is_open:
            self.port = port
            self.start_debug_thread()
            return True
        else:
            return False
        
    def disconnect(self):
        if self.is_connected():
            self.port.close()
            self.port = None

    def start_debug_thread(self):
        t = threading.Thread(target=lambda: self.debug_printout())
        t.start()

    def debug_printout(self):
        try:
            while self.is_connected():
                line = self.port.readline()
                print(line.decode("utf-8"))
        except Exception:
            print("ERROR reading! Bye...")
            self.disconnect()

    def set_motors(self, motor_1, motor_2, motor_3, motor_4):
        if not self.is_connected(): return

        directions = 0
        if motor_1 > 0: directions |= (1 << 0)
        if motor_2 > 0: directions |= (1 << 1)
        if motor_3 > 0: directions |= (1 << 2)
        if motor_4 > 0: directions |= (1 << 3)

        msg = bytes([1, directions, abs(motor_1), abs(motor_2), abs(motor_3), abs(motor_4)])
        self.port.write(msg)

    def turn(self, speed):
        if not self.is_connected(): return

        is_clockwise = speed > 0

        msg = bytes([2, is_clockwise, abs(speed)])
        self.port.write(msg)

    def turn_left(self, speed=255):
        self.turn(-speed)

    def turn_right(self, speed=255):
        self.turn(speed)

    def move_lr(self, speed):
        if not self.is_connected(): return

        is_left = speed < 0

        msg = bytes([3, is_left, abs(speed)])
        self.port.write(msg)

    def move_left(self, speed=255):
        self.move_lr(-speed)
    
    def move_right(self, speed=255):
        self.move_lr(speed)

    def stop(self):
        self.set_motors(0, 0, 0, 0)

    def readline(self):
        return self.port.readline() # TODO(Richo): Just for debugging...

comm = Comm()
comm.connect("COM21") # USB
comm.connect("COM23") # Bluetooth
print("Connected!")

comm.set_motors(255, 0, 0, 0)

comm.turn_left()
comm.stop()

comm.turn_right()
comm.stop()

comm.move_left()
comm.stop()

comm.move_right()
comm.stop()

comm.disconnect()

