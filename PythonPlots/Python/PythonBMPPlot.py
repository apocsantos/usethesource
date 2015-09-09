import serial # import Serial Library
import numpy as np  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
from drawnow import *
import time

tempF= []   #Array for our Temperature dat
pressure=[] #Array for our Pressure Data
ht=[]       #Array for our calculated Heights
arduinoData = serial.Serial('com11', 115200) #Creating our serial object named arduinoData
plt.ion() #Tell matplotlib you want interactive mode to plot live data
cnt=0

def makeFig(): #Create a function that makes our desired plot
    plt.ylim(0,10)                                 #Set y min and max values
    plt.title('My SuperCool Height-O-Meter')      #Plot the title
    plt.grid(True)                                  #Turn the grid on
    plt.ylabel('Height in Feet')                            #Set ylabels
    plt.plot(ht, 'ro-', label='Height')       #plot the height array
    plt.legend(loc='upper left')                    #plot the legend
    plt2=plt.twinx()                                #Create a second y axis
    plt.ylim(75,85)                           #Set limits of second y axis- adjust to readings you are getting
    plt2.plot(tempF, 'b^-', label='tempF') #plot temperature array
    plt2.set_ylabel('temp F')                    #label second y axis
    plt2.ticklabel_format(useOffset=False)           #Force matplotlib to NOT autoscale y axis
    plt2.legend(loc='upper right')                  #plot the legend

tempBucket = 0  #Create bucket to hold sum of temperature readings
PBucket=0       #Create bucket to hold sum of pressure readings

print "PLease put Sensor Circuit on Ground for Calibration"  #Calibrate sensor for readings on floor, P0
print "5"           #Give user time to put sensor on floor
time.sleep(1)
print "4"
time.sleep(1)
print "3"
time.sleep(1)
print "2"
time.sleep(1)
print "1"
time.sleep(1)
print "Calibrating Sensor . . ."

for i in np.arange(1,11,1):  #Loop ten times to take ten measurements
    while (arduinoData.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    arduinoString = arduinoData.readline() #read the line of text from the serial port
    dataArray = arduinoString.split(',')   #Split it into an array called dataArray
    temp = float( dataArray[0])            #Convert first element to floating number and put in temp
    P =    float( dataArray[1])            #Convert second element to floating number and put in P
    print "P = ",P, " , Temp= ", temp
    tempBucket=tempBucket + temp
    PBucket = PBucket + P
P0 = PBucket/10  #Calcualte average pressure on floor
tempK = ((tempBucket/10)-32)/1.8 +273.15  #Calculate average temperature on floor in K

print "Baseline Temp in K is: ", tempK #Print resulrs
print "Baseline Pressure inPa. : ", P0
    


while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    arduinoString = arduinoData.readline() #read the line of text from the serial port
    dataArray = arduinoString.split(',')   #Split it into an array called dataArray
    temp = float( dataArray[0])            #Convert first element to floating number and put in temp
    P =    float( dataArray[1])            #Convert second element to floating number and put in P
    h=98.57*tempK*np.log(P0/P)
    ht.append(h)
    tempF.append(temp)                     #Build our tempF array by appending temp readings
    pressure.append(P)                     #Building our pressure array by appending P readings
    drawnow(makeFig)                       #Call drawnow to update our live graph
    plt.pause(.000001)                     #Pause Briefly. Important to keep drawnow from crashing
    cnt=cnt+1
    if(cnt>50):                            #If you have 50 or more points, delete the first one from the array
        tempF.pop(0)                       #This allows us to just see the last 50 data points
        pressure.pop(0)
        ht.pop(0)