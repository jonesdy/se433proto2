#include "face_common.h"
#include "face_messages.h"
#include "face_ios.h"
#include "config_parser.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      printf("Please specify a config file\n");
      return 1;
   }

   // Call init

   // Parse the config file
   FACE_CONFIG_DATA_TYPE config[32];
   uint32_t numConnections[1];
   numConnections[0] = 32;
   PasrseConfigFile( argv[1], config, numConnections);
   printf("First name: %s\n", config[0].name);

   // Open channels from config

   // Get user input and set or read

   // Close channels

   return 0;
}

#define MAX_BUFF_SIZE 1024

void setDiscrete(FACE_INTERFACE_HANDLE_TYPE handle, int channel, _Bool value,
   FACE_RETURN_CODE_TYPE* retCode)
{
   // The messages and stuff we will need
   char txBuff[MAX_BUFF_SIZE];
   FACE_IO_MESSAGE_TYPE *txFaceMsg = (FACE_IO_MESSAGE_TYPE*)txBuff;

   // Zero it out
   memset(txBuff, 0, MAX_BUFF_SIZE);

   // Set the fixed fields
   // TODO: What should we set these GUIDs to??
   txFaceMsg->guid = htonl(100);
   // TODO: This might need to be more generic? or add ARINC functions?
   txFaceMsg->busType = FACE_DISCRETE;
   // TODO: What is this FACE_DATA?
   txFaceMsg->message_type = htons(FACE_DATA);
   // TODO: Why 4?
   FaceSetPayLoadLength(txFaceMsg, 4);

   FaceSetDiscreteChannelNumber(txFaceMsg, channel);

   if(value == true)
   {
      FaceSetDiscreteState(txFaceMsg, 1);
   }
   else
   {
      FaceSetDiscreteState(txFaceMsg, 0);
   }

   FACE_IO_Write(handle, 0, FACE_MSG_HEADER_SIZE + 4, txFaceMsg, retCode);
}

_Bool readDiscrete(FACE_INTERFACE_HANDLE_TYPE handle, int channel,
   FACE_RETURN_CODE_TYPE* retCode)
{
   char rxBuff[MAX_BUFF_SIZE];
   FACE_MESSAGE_LENGTH_TYPE msgLen;
   FACE_IO_MESSAGE_TYPE *rxFaceMsg = (FACE_IO_MESSAGE_TYPE*)rxBuff;

   memset(rxBuff, 0, MAX_BUFF_SIZE);

   rxFaceMsg->guid = htonl(200);

   rxFaceMsg->busType = FACE_DISCRETE;

   rxFaceMsg->message_type = htons(FACE_DATA);

   FaceSetPayLoadLength(rxFaceMsg, 4);

   FACE_IO_Read(handle, 0, &msgLen, rxFaceMsg, retCode);

   // TODO: Make sure this works
   return (_Bool)FaceDiscreteState(rxFaceMsg);
}
