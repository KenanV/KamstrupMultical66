// Example of arduino sketch that communicates with Kamstrup Multical 66C 
// using optical eye and writes the response to the COM port.

// Pin definitions
#define PIN_LED        13  // Standard Arduino LED

// Kamstrup optical IR serial
#define KAMTIMEOUT   5000  // Kamstrup timeout after transmit
#define BUFFER_SIZE  1500

byte rxdata[BUFFER_SIZE];  // buffer to hold received data

byte numRequest = 13; //There are 13 defined requests, se below
byte requests[]={'1','2','3','4','5','6','7','8','9','J','B','C','D'};

void setup () {

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 0);
  
  //Wait 30seconds before transmitting anything, since the TX wire has 
  //to be reconnected after programming
  delay(30000);
}

void loop () {
  // poll the data for each request
  for(int i =0; i<numRequest; i++){
    kamReadReq(requests[i]);

    //wait 30 seconds to poll next request
    delay(30000);
  }
}

// kamReadReq - send a Kamstrup request and return response through COM port
float kamReadReq(byte req) {
  
// prepare message to send and send it
  byte sendmsg[] = { '/', '#', req, 0x0D, 0x0A};

  int rxnum=0;
  byte single = 0;

  //initialize with random value for debugging
  for (int i = 0; i < BUFFER_SIZE; i++) {
    rxdata[i]= 8;
  }

  //Switch too 300baud for sending request
  Serial.begin(300, SERIAL_7E2); 
  for (int i = 0; i < 5; i++) {
    Serial.write(sendmsg[i]); //Send request
  }
  Serial.end();
  Serial.flush();

  //Switch to 1200 baud ro read response
  Serial.begin(1200, SERIAL_7E2);
  // listen if we get an answer
  rxnum = kamReceive();
  Serial.end();
  Serial.flush();

  //Switch to 300 baud to transmit to COM port.
  Serial.begin(300, SERIAL_7E2);
  // check if number of received bytes > 0 
  if(rxnum != 0){
    Serial.print("Received: ");
    Serial.print(rxnum);
    Serial.println(" characters");
    for(int i = 0; i < rxnum; i++){
      single = rxdata[i];
      
      //For debugging purposes the special character are spelled out
      switch(single)
      {
        case 0x02:
          Serial.print(" STX ");
          break;
        case 0x03:
          Serial.print(" ETX ");
          break;
        case 0x0A:
          Serial.println(" LF ");
          break;
        case 0x0D:
          Serial.println(" CR ");
          break;
        default:
          Serial.write(rxdata[i]);
         break;          
      }
    }
    Serial.println("Done");
    Serial.flush();
  }
  Serial.end();
  return 0;
}
// kamReceive - receive bytes from Kamstrup meter
unsigned short kamReceive() {

  unsigned long rxindex = 0;
  unsigned long starttime = millis();
  
  byte r=0;
 
  // loop until EOL received or timeout or out of space in buffer
  while(r != 0x03){
    // handle rx timeout
    if(millis()-starttime > KAMTIMEOUT) {
      return rxindex;
    }
    if(rxindex>=BUFFER_SIZE){
      return rxindex;
    }

    // handle incoming data
    while(Serial.available()>0) {
      // receive byte
      r = Serial.read();    
      rxdata[rxindex] = r;
      rxindex++; 
      starttime = millis(); //Refresh timeout after a char has been recieved
    }
  }
  return rxindex; 
}
