#14-09-2015
#graphics with arduino an python for:
#Carapaus of racing! or in other words, "F1 carapaus".
#

#import libraies
import serial # import Serial Library
import numpy  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
from drawnow import *
from array import array
#global variables and objects
tempF= []
tempF2= []
humF1= []
humF2= []
arduinoData = serial.Serial('com5', 115200) #Creates a serial object named arduinoData
plt.ion() #Tell matplotlib that we want interactive mode to plot live data
cnt=0

#data from the arduino should be CSV , separated field.
#use twinx() to make as many graph's as we want, but be sure to
#use the same mesuring units
#

#function makeFig()
def makeFig(): #Create a function that makes the desired plot
    plt.subplot(2, 1, 1)
    plt.ylim([-30,50])                                      #Set y min and max values
    plt.title('Temperatura em Graus C')                     #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('Temp-1 c')                                  #Set ylabels
    plt.plot(tempF, 'ro-', label='temperatura em graus')    #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
    plt2=plt.twinx()                                        #Create a second y axis
    plt.ylim(-30,50)                                        #Set limits of second y axis- adjust to readings you are getting
    plt2.plot(tempF2, 'b^-', label='Temp-2 c')              #plot temperature array
    plt2.set_ylabel('Temp-2 c')                             #label second y axis
    plt2.ticklabel_format(useOffset=False)                  #Force matplotlib to NOT autoscale y axis
    plt2.legend(loc='upper right')                          #plot the legend
    #second plot same figure (same window)
    plt.subplot(2, 1, 2)
    plt.ylim([0,100])                                       #Set y min and max values
    plt.title('Humidade do Ar em Percentagem')              #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('Himidade-1 %')                              #Set ylabels
    plt.plot(humF1, 'ro-', label='Humidade')                #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
    plt2=plt.twinx()                                        #Create a second y axis
    plt.ylim(0,100)                                         #Set limits of second y axis- adjust to readings you are getting
    plt2.plot(humF2, 'b^-', label='Humidade-2 %')           #plot temperature array
    plt2.set_ylabel('Humidade-2 %')                         #label second y axis
    plt2.ticklabel_format(useOffset=False)                  #Force matplotlib to NOT autoscale y axis
    plt2.legend(loc='upper right')                          #plot the legend
    
#mainProgram
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data. otherwise fault would accur
        pass #do nothing
    arduinoString = arduinoData.readline()  #read the line of text from the serial port
    #dataArray = arduinoString.split(',')   #Split it into an array called dataArray
    fta, fkha, ftb, fkhb = arduinoString.split(',')  #the magic of string spliting into multiple vars! ;)
    #case you found the variable name strange, read my favore pice of C code at: https://dl.dropboxusercontent.com/u/21721067/forkabout.c
    #the fricking indian magic happens here
    fa = float(fta);                        #converts the string to a float otherwise it just fucks it up!
    fb = float(ftb);                        #converts the string to a float otherwise it just fucks it up!
    fha= float(fkha);                       #converts the string to a float otherwise it just fucks it up!
    fhb= float(fkhb);                       #converts the string to a float otherwise it just fucks it up!
    dataArrayA = array('f')                 #declares an array of floats
    dataArrayB = array('f')                 #declares an array of floats
    tempArrayA = array('f')                 #declares an array of floats
    tempArrayB = array('f')                 #declares an array of floats (yes! you need to store the data somewhere right ? dont play a *f iDiot with me! Ok ?)
    dataArrayA.append(fa)                   #appends a value to the float array (here the data is stored to each array! Sure I could have used arrays of objects! But I'm not in the mood!
    dataArrayB.append(fb)                   #appends a value to the float array (and since you noticed that, bring me some coffe!)
    tempArrayA.append(fha)                  #appends a value to the float array
    tempArrayB.append(fhb)                  #appends a value to the float array
    temp = dataArrayA[0]                    #Convert first element to floating number and put in temp
    temp2 = dataArrayB[0]                   #Convert first element to floating number and put in temp2
    hum1 = tempArrayA[0]                    #Convert first element to floating number and put in hum1 (the humidity array for sensor one)
    hum2 = tempArrayB[0]                    #Convert first element to floating number and put in hum2 (if you got this far and understood, this line does not need explanations!)
    tempF.append(temp)                      #Build the tempFf array by appending temp readings
    tempF2.append(temp2)                    #Build the tempF2 array by appending temp readings
    humF1.append(hum1)                      #Build the humF1 array by appending hum1 readings
    humF2.append(hum2)                      #dont play dumb again! And if you do increment the following counter in 1 ApplePhoneCounter = 0
    drawnow(makeFig)                        #Call drawnow to update our live graph
    plt.pause(.000001)                      #Pause Briefly. Important to keep drawnow from crashing
    cnt=cnt+1                                #increments counter
    if(cnt>50):                             #If you have 50 or more points, delete the first one from the array
        tempF.pop(0)                        #This allows us to just see the last 50 data points and of clears the *f mess that is done before
        tempF2.pop(0)
        humF1.pop(0)                        #This allows us to just see the last 50 data points and of clears the *f mess that is done before
        humF2.pop(0)
