#include "face_common.h"
#include "face_messages.h"
#include "face_ios.h"
#include "config_parser.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void setDiscreteConvenient(int channel, _Bool value, FACE_CONFIG_DATA_TYPE *config, FACE_INTERFACE_HANDLE_TYPE *handle_arr,
			   FACE_RETURN_CODE_TYPE* retCode);
_Bool readDiscreteConvenient(int channel, FACE_CONFIG_DATA_TYPE *config, FACE_INTERFACE_HANDLE_TYPE *handle_arr,
			     FACE_RETURN_CODE_TYPE* retCode);

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      printf("Please specify a config file\n");
      return 1;
   }

   FACE_RETURN_CODE_TYPE retCode;

   // Call init

   // Parse the config file
   uint32_t numConnections[1];
   *numConnections = (argc > 2) ? (uint32_t)*argv[2]-'0' : 32;
   FACE_CONFIG_DATA_TYPE config[*numConnections];

   PasrseConfigFile( argv[1], config, numConnections);
   FACE_INTERFACE_HANDLE_TYPE handle_arr[*numConnections];

   // print out what got read from the config
   int i = 0;
   printf("Found the following devices:\n");
   for(i = 0; i < *numConnections; i++) {
     printf("Ch%d: %s\n", config[i].channel, config[i].name);
   }
   
   printf("The following devices are discretes:\n");
   for(i = 0; i < *numConnections; i++) {
     if(config[i].busType == FACE_DISCRETE) {
       printf("Ch%d: %s\n", config[i].channel, config[i].name);
     }
   }
   printf("\n");

   // Open channels from config

   for(i = 0; i < *numConnections; i++) {
     FACE_IO_Open(config[i].name, &handle_arr[i], &retCode);
   }

   // testing setting a discrete
   //   void setDiscrete(FACE_INTERFACE_HANDLE_TYPE handle, int channel, _Bool value, FACE_RETURN_CODE_TYPE* retCode)
   printf("Ch1: %d\n", readDiscreteConvenient(1, config, handle_arr, &retCode));
   if(retCode == FACE_TIMED_OUT) {
     printf("ERROR: ch1 read timed out\n");
   }
   printf("Setting ch1\n");
   setDiscreteConvenient(1, 1, config, handle_arr, &retCode);
   if(retCode == FACE_TIMED_OUT) {
     printf("ERROR: ch1 set timed out\n");
   }
   printf("Ch1: %d\n", readDiscreteConvenient(1, config, handle_arr, &retCode));
   if(retCode == FACE_TIMED_OUT) {
     printf("ERROR: ch1 read timed out\n");
   }
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

void setDiscreteConvenient(int channel, _Bool value, FACE_CONFIG_DATA_TYPE *config, FACE_INTERFACE_HANDLE_TYPE *handle_arr,
   FACE_RETURN_CODE_TYPE* retCode)
{
  setDiscrete(handle_arr[channel], channel, value, retCode);
}

_Bool readDiscreteConvenient(int channel, FACE_CONFIG_DATA_TYPE *config, FACE_INTERFACE_HANDLE_TYPE *handle_arr,
   FACE_RETURN_CODE_TYPE* retCode)
{
  return readDiscrete(handle_arr[channel], channel, retCode);
}
