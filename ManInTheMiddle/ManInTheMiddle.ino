
#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication

//The following pins are default aruino pins for doing SPI communication
// Pin 11 : SI
// Pin 12 : SO
// Pin 13 : CLK

struct can_frame canMsg;
struct can_frame canMsg1;
MCP2515 mcp2515(10);                 // SPI CS Pin 10
MCP2515 mcp2515_out(9);                 // SPI CS Pin 9
 
void setup() {
 
  Serial.begin(115200);                //Begins Serial Communication at 9600 baudrate 
  Serial.println("Setting Up");
  mcp2515.reset();                          
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode

  mcp2515_out.reset();                          
  mcp2515_out.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515_out.setNormalMode();                  //Sets CAN at normal mode
}

void loop() 
{
  int msgStatus = mcp2515.readMessage(&canMsg);
  if (msgStatus == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    Serial.print("Message received: ");
    Serial.println(canMsg.can_id, HEX);
    /*If the can message comes from the outlander CMU (address 0x036)
    then reassign its ID and send out the message on bus 2 */
    if(canMsg.can_id == 0x036){
      Serial.println(canMsg.can_id, HEX);
      Serial.println(canMsg.data[0], HEX);
      canMsg1 = canMsg;
      canMsg1.can_id = 0x037; // changed from 0x036
      int status = mcp2515_out.sendMessage(&canMsg1);     //Sends the CAN message
      Serial.print("Modified Message Sent with status: ");
      Serial.println(status);
    }
  }
}
