#20-09-2015
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
tempF2 = array('f')
humF1 = array('f')
humF2 = array('f')
lum = array('f')
moist = array('f')
#line data array
arduinoData = serial.Serial('com3', 115200) #Creates a serial object named arduinoData
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
    plt2=plt.twinx()                                        #Create a second y axis
    plt.ylim(-30,50)                                        #Set limits of second y axis- adjust to readings you are getting
    plt2.plot(tempF2, 'b^-', label='Temp-2 c')              #plot temperature array
    plt2.set_ylabel('Temp-2 c')                             #label second y axis
    plt2.ticklabel_format(useOffset=False)                  #Force matplotlib to NOT autoscale y axis
    plt2.legend(loc='upper right')                          #plot the legend
    #second plot same figure (same window)
    #Plot 2
    plt.subplot(gs[1, :])                                   #subplot position atributes
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
    #Third plot same figure (same window)
    #Plot 3
    plt.subplot(gs[-1,0])                                   #subplot position atributes
    plt.ylim([0,100])                                       #Set y min and max values
    plt.title('Humidade do solo')                           #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('Himidade %')                                #Set ylabels
    plt.plot(moist, 'ro-', label='Humidade')                #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
    #Fourth plot same figure (same window)
    #Plot 4
    plt.subplot(gs[-1,-1])                                  #subplot position atributes
    plt.ylim([0,2000])                                      #Set y min and max values
    plt.title('Luminosidade')                               #Plot the title
    plt.grid(True)                                          #Turn on the grid
    plt.ylabel('Luminosidade (lux)')                        #Set ylabels
    plt.plot(lum, 'ro-', label='Luminosidade')              #plot the temperature
    plt.legend(loc='upper left')                            #plot the legend
#mainProgram
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data. otherwise fault would accur
        pass #do nothing
    #try ... except implemented for when data on the line is not correct!
    try:
        arduinoString = arduinoData.readline()  #read the line of text from the serial port
        #case you found the variable name strange, read my favore pice of C code at: https://dl.dropboxusercontent.com/u/21721067/forkabout.c
        #the fricking indian magic happens here
        splitedArray = [float(s) for s in arduinoString.split(',')] #splits the string into an array of floats
        if (len(splitedArray) < 6 ):
            raise Exception("no data on the line!")
        else:
            pass
    except:
        time.sleep(1)
        pass
    #assign each array value to the correspondent variable, I dont *f like in-line opreations cause they tend to make messy code
    temp = splitedArray[0]     #assign each array value to the correspondent variable
    hum1 = splitedArray[1]     #I dont *f like in-line opreations cause they tend to make messy code
    temp2 = splitedArray[2]    #if you do, cool for you!
    hum2 = splitedArray[3]     #if you dont, that makes us 2
    moisture = splitedArray[4] #if you just think this could be more optimized...
    lumen = splitedArray[5]    #you are right, but I'm not in the *f mood to do it! So shut the *f up
    #appends a value to the float array (here the data is stored to each array! Sure I could have used arrays of objects! But I'm not in the mood!
    #if you do, cool for you! if you dont, that makes us 2! if you just think this could be more optimized... you are right!
    #but I'm not in the *f mood to do it! So shut the *f up and screw you
    tempF.append(temp)                      #Build the tempFf array by appending temp readings
    tempF2.append(temp2)                    #Build the tempF2 array by appending temp readings
    humF1.append(hum1)                      #Build the humF1 array by appending hum1 readings
    humF2.append(hum2)                      #Build the humF2 array by appending hum1 readings
    moist.append(moisture)                  #Build the moistureArray array by appending  readings
    lum.append(lumen)                       #dont play dumb again! And if you do increment the following counter in 1 ApplePhoneCounter = 0
    drawnow(makeFig)                        #Call drawnow to update our live graph
    plt.pause(.000005)                      #Pause Briefly. Important to keep drawnow from crashing (just a bit, cause the augment of the code preformance made it necessary
    cnt=cnt+1                               #increments counter
    if(cnt>50):                             #If you have 50 or more points, delete the first one from the array
        tempF.pop(0)                        #This allows us to just see the last 50 data points and of clears the *f mess that is done before
        tempF2.pop(0)                       #...
        humF1.pop(0)                        #...
        humF2.pop(0)                        #...
        moist.pop(0)                        #...
        lum.pop(0)                          #...
        #dont read my *f code! you fricking iDiot!
