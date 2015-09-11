import serial #Import Serial Library
 
arduinoSerialData = serial.Serial('com5',9600) #Create Serial port object called arduinoSerialData
 
 
while (1==1):
    if (arduinoSerialData.inWaiting()>0): #if there is no data on the line, just leave the **f line, and frok off!
        myData = arduinoSerialData.readline()
        print myData
