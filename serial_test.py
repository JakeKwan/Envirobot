import serial 

COM_PORT = "COM5"

port = serial.Serial(COM_PORT, 115200, timeout = 1)

def read_ser(num_char = 1): 
    string = port.read(num_char)
    return string.decode()

while(1):
    string = read_ser(10)
    print(string)   