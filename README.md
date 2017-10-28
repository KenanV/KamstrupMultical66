# KamstrupMultical66

## Introduction
Arduino project to read out data from a Kamstrup MULTICAL 66 Energy Meter using Optical
Eye. Some information from Kamstrup can be found on (http://products.kamstrup.com/  Heat->Previous Products->Meters)

In order to be able to read data from the energy meter, I bought an optical eye kit from:
https://wiki.hal9k.dk/projects/kamstrup


Unfortunately,
the software provided does not cover this specific energy meter. This project makes it possible to retrieve all information from the energy meter and send it over COM port to a PC.


The project is made for Arduino Uno and runs with Optical eye connected to GND, 5V, Rx and Tx pins on the board using the hardware UART because of the 7E2 configuration. 

## Functionality
Currently the software supports all messages and just retrieves them and transmits them over COM port:


a) Notice that the protocol for some reason is using two different configurations: Most of requests are sent using 300baud, 7E2.
and response is transmitted using 1200vaud, 7E2
  The request "/!?" uses  300baud, 7E2 for both sending request and recieving the response.

b) Do not send requests too fast. If you repeat requests too often you might get some interference and get some garbage as response.


## Testing
When testing use a terminal such as Termite (https://www.compuphase.com/software_termite.htm) and configure it for 300baud, 7, E, 2

Connect the hardware and wait for some time. The terminal will show both the requests and response.


