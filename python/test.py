import serial

# Create a serial connection
ser = serial.Serial('COM12', 9600)  # Replace 'COM1' with the appropriate port

# Send data to Arduino
ser.write(b'Hello Arduino!\n')

# Read data from Arduino
while True:
    ser.write(b'Hello Arduino!\n')
    data = ser.readline()
    print(data)

# Close the serial connection
ser.close()
