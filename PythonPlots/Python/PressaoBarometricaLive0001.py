# -*- coding: cp1252 -*-
#16-09-2015
#graphics with arduino an python for:
#Carapaus of racing! or in other words, "F1 carapaus".
#Optimized version 1, for low resources.

#import libraies
import serial # import Serial Library
import numpy  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
import matplotlib.gridspec as gridspec #import gridspec
from drawnow import *
from array import array

#global variables and objects
#all data grafics arrays declared explicit as arrays of floats
tempF = array('f')
pressure= array('f')
#line data array
arduinoData = serial.Serial('com6', 115200) #Creates a serial object named arduinoData
plt.ion() #Tell matplotlib that we want interactive mode to plot live data
cnt=0

#data from the arduino should be CSV , separated field.
#use twinx() to make as many graph's as we want, but be sure to
#use the same mesuring units
#

#function makeFig()
def makeFig(): #Create a function that makes the desired plot
    gs = gridspec.GridSpec(3, 3) #gridspec is created 3x3
    #First plot fig 1
    #Plot 1
    plt.subplot(gs[0, :])                                   #subplot position atributes
    plt.ylim([-30,50])                                      #Set y min and max values
    plt.title('Temperatura em Graus C')                     #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('Temp-1 c')                                  #Set ylabels
    plt.plot(tempF, 'ro-', label='temperatura em graus')    #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
    #second plot same figure (same window)
    #Plot 2
    plt.subplot(gs[1, :])                                   #subplot position atributes
    plt.ylim([600,1200])                                       #Set y min and max values
    plt.title('Pressao Atmosferica')              #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('mbar')                              #Set ylabels
    plt.plot(pressure, 'ro-', label='mbar')                #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
#mainProgram
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data. otherwise fault would accur
        pass #do nothing
    arduinoString = arduinoData.readline()  #read the line of text from the serial port
    #case you found the variable name strange, read my favore pice of C code at: https://dl.dropboxusercontent.com/u/21721067/forkabout.c
    #the fricking indian magic happens here
    splitedArray = [float(s) for s in arduinoString.split(',')] #splits the string into an array of floats
    #assign each array value to the correspondent variable, I dont *f like in-line opreations cause they tend to make messy code
    temp = splitedArray[0]  #assign each array value to the correspondent variable
    press = splitedArray[1] #I dont *f like in-line opreations cause they tend to make messy code
    #appends a value to the float array (here the data is stored to each array! Sure I could have used arrays of objects! But I'm not in the mood!
    #if you do, cool for you! if you dont, that makes us 2! if you just think this could be more optimized... you are right!
    #but I'm not in the *f mood to do it! So shut the *f up and screw you
    tempF.append(temp)                      #Build the tempFf array by appending temp readings
    pressure.append(press)                    #Build the tempF2 array by appending temp readings
    drawnow(makeFig)                        #Call drawnow to update our live graph
    plt.pause(.000005)                      #Pause Briefly. Important to keep drawnow from crashing (just a bit, cause the augment of the code preformance made it necessary
    cnt=cnt+1                               #increments counter
    if(cnt>50):                             #If you have 50 or more points, delete the first one from the array
        tempF.pop(0)                        #This allows us to just see the last 50 data points and of clears the *f mess that is done before
        pressure.pop(0)                     #...
        #dont read my *f code! you fricking iDiot!
