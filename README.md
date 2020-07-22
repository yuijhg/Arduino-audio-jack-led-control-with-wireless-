# Arduino-audio-jack-led-control-with-wireless-
This project requires at least two separate arduinos. We used a Mega for the transmitter and nanos for the recievers.
This project requires two "NRF24L01 Wireless Transceiver Module+2.4GHz Antenna for Arduino" or an arduinot with one of these integrated in like the
"Emakefun BLE Nano for Arduino with Wireless BLE and Micro-USB Interface,TICC2540 Chip,Compatible to Arduino Nano V3.0".
A female audio jack port that breaks out the signals is required for this project. 
A lm348 Audio amoplifier is recommended for this project. I recommend the "DZS Elec 2pcs LM386 Audio Amplifier Module 200 Times Multiplier Speaker Wire Holder 
5V-12V Input 10K Adjustable Resistance DIY Electronics for Arduino EK1236"
working2.5 is the transmitter code which handles finding the beats in the music and sending values to the secondary board. 
RecieverRipple is the reciever code that takes the information transfered from the promary arduino, and drives an led strip based on the data.
