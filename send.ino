/*Minion_s
 * SimpleSend
 * RFM12B wireless demo - transmitter - no ack
 * Sends values of analog inputs 0 through 6
 *
 */

#include <RF12.h>  //from jeelabs.org
#include <Ports.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6, 5, 4); // for minion ....dont use D3...tied to RFM12

// RF12B constants:
const byte network  = 100;   // network group (can be in the range 1-255).
const byte myNodeID = 1;     // unique node ID of receiver (1 through 30) 1-send 2-receive

//Frequency of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_433MHZ; // Match freq to module

const byte RF12_NORMAL_SENDWAIT = 0;

void setup()
{
  rf12_initialize(myNodeID, freq, network);   // Initialize RFM12   
 
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(45);
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
 
  
}

const int payloadCount = 6; // the number of integers in the payload message
int payload[payloadCount];



void loop()
{
  for( int i= 0; i < payloadCount; i++)
  {
    payload[i] = analogRead(i);   
  }
  while (!rf12_canSend())  // is the driver ready to send?
    rf12_recvDone();       // no, so service the driver

  rf12_sendStart(rf12_hdr, payload, payloadCount*sizeof(int)); 
  rf12_sendWait(RF12_NORMAL_SENDWAIT); // wait for send completion

  delay(1000);  // send every second
}

=============================================
=============================================

/*Minion_r
 * SimpleSend
 * RFM12B wireless demo - transmitter - no ack
 * Sends values of analog inputs 0 through 6
 *
 */

#include <RF12.h>  //from jeelabs.org
#include <Ports.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6, 5, 4); // for minion ....dont use D3...tied to RFM12

// RF12B constants:
const byte network  = 100;   // network group (can be in the range 1-255).
const byte myNodeID = 2;     // unique node ID of receiver (1 through 30) 1-send 2-receive

//Frequency of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_433MHZ; // Match freq to module

void setup() {
  rf12_initialize(myNodeID, freq, network);   // Initialize RFM12  
  Serial.begin(9600); 
  Serial.println("RFM12B Receiver ready");
  Serial.println(network,DEC);   // print the network 
  Serial.println(myNodeID,DEC);  // and node ID 
  
  display.begin();
  display.setContrast(45);
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  
  /*display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  //display.display();
  delay(2000);
  */
  
}

const int payloadCount = 6; // the number of integers in the payload message


 

void loop()
{
  if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) 
  {
    int *payload = (int*)rf12_data;  // access rf12 data buffer as an arrya of ints
    
    display.clearDisplay();
    display.setCursor(0,0);
    
    for( int i= 0; i < payloadCount; i++)
    {
      Serial.print(payload[i]);
      Serial.print(" ");  
      display.print("Temp ");
      display.print(i+1);
      display.print(" = ");
      display.print(payload[i]);
      display.println("F");
    }	
    
    Serial.println();  
    display.display();
  }
}


/*

// text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
  
  */
