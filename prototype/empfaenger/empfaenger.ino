
#include <SPI.h>        

#include <Ethernet.h>

#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 6, 25);

IPAddress destinationIP(192, 168, 6, 20);  // Address of target machine
unsigned int destinationPort = 6555;      // Port to send to


unsigned int localPort = 8888;      // local port to listen on



// An EthernetUDP instance to let us send and receive packets over UDP

EthernetUDP Udp;


void setup(){
  Serial.begin(115200);

    // start the Ethernet and UDP:

  Ethernet.begin(mac,ip);

  Udp.begin(localPort);
}

void loop(){

  while(Serial.available() > 0 ){
    String str = Serial.readStringUntil('\n');
    //Serial.println(str);
    Udp.beginPacket(destinationIP, destinationPort);
    Udp.print(str);
    Udp.endPacket();

  }
}
