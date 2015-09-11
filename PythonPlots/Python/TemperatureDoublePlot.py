#31-08-2015
#graphics with arduino an python for:
#Carapaus of racing! or in other words, "F1 carapaus".
#

#import libraies
import serial # import Serial Library
import numpy  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
from drawnow import *
#global variables and objects
tempF= []
tempF2= []
arduinoData = serial.Serial('com5', 115200) #Creates a serial object named arduinoData
plt.ion() #Tell matplotlib that we want interactive mode to plot live data
cnt=0

#data from the arduino should be CSV , separated field.
#use twinx() to make as many graph's as we want, but be sure to
#use the same mesuring units
#

#function makeFig()
def makeFig(): #Create a function that makes the desired plot
    plt.ylim(-30,50)                                  #Set y min and max values
    plt.title('Temperatura em Graus C')               #Plot the title
    plt.grid(True)                                    #Turn on the grid
    plt.ylabel('Temp-1 c')                            #Set ylabels
    plt.plot(tempF, 'ro-', label='Graus C')           #plot the temperature
    plt.legend(loc='upper left')                      #plot the legend
    plt2=plt.twinx()                                  #Create a second y axis
    plt.ylim(-30,50)                                  #Set limits of second y axis- adjust to readings you are getting
    plt2.plot(tempF2, 'b^-', label='Graus C')         #plot temperature array
    plt2.set_ylabel('Temp-2 c')                       #label second y axis
    plt2.ticklabel_format(useOffset=False)            #Force matplotlib to NOT autoscale y axis
    plt2.legend(loc='upper right')                   #plot the legend
    
#mainProgram
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data. otherwise fault would accur
        pass #do nothing
    arduinoString = arduinoData.readline() #read the line of text from the serial port
    dataArray = arduinoString.split(',')   #Split it into an array called dataArray
    #the fricking magic happens here
    temp = float( dataArray[0])            #Convert first element to floating number and put in temp
    temp2 = float( dataArray[1])           #Convert second element to floating number and put in temp2
    tempF.append(temp)                     #Build our tempF array by appending temp readings
    tempF2.append(temp2)                   #Building our pressure array by appending temp2 readings
    drawnow(makeFig)                       #Call drawnow to update our live graph
    plt.pause(.000001)                     #Pause Briefly. Important to keep drawnow from crashing
    cnt=cnt+1
    if(cnt>50):                            #If you have 50 or more points, delete the first one from the array
        tempF.pop(0)                       #This allows us to just see the last 50 data points
        tempF2.pop(0)
