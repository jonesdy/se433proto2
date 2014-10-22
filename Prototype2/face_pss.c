#include "face_pss.h"
#include "face_common.h"
#include "face_messages.h"
#include "config_parser.h"

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
   FACE_RETURN_CODE_TYPE retCode;
   // We are on the PSS side, so we add 0 to the
   // beginning of the config file name
   FACE_CONGIGURATION_FILE_NAME configName = "0";
   strcat(configName, argv[1]);
   FACE_IO_Initialize(configName, &retCode);
   if(retCode != FACE_NO_ERROR)
   {
      printf("Error occurred during initialization: %d\n", retCode);
      return 1;
   }

   // Parse the config file
   FACE_CONFIG_DATA_TYPE config[32];
   uint32_t numConnections[1];
   numConnections[0] = 32;
   PasrseConfigFile( argv[1], config, numConnections);

   // Open channels from config and store handles
   FACE_INTERFACE_HANDLE_TYPE handles[32];
   int i;
   for(i = 0; i < 32; i++)
   {
      if(config[i].channel != 0)
      {
         FACE_IO_Open(config[i].name, &handles[i], &retCode);
         if(retCode != FACE_NO_ERROR)
         {
            printf("Error occurred while opening %s: %d\n", config[i].name,
               retCode);
         }
         else
         {
            printf("Successfully opened %s\n", config[i].name);
         }
      }
   }

   // Get user input and set or read
   int channel = 9000;  // TODO: Replace this with real input
   uint8_t discreteValue = 0;

   // Check that the user input is valid
   if(channel >= 1 && channel <= 16)
   {
      // RX, must want to read
   }
   else if(channel >= 17 && channel <= 32)
   {
      // TX, must want to set
   }

   // Do actual read or write (see below functions)
   // Find the handle for this channel
   FACE_INTERFACE_HANDLE_TYPE handle = NULL;
   for(i = 0; i < 32; i++)
   {
      if(config[i].channel == channel)
      {
         handle = handles[i];
         break;
      }
   }

   if(handle == NULL)
   {
      printf("Channel %d is not configured properly\n", channel);
   }
   else
   {
      // Setting
      setDiscrete(handle, channel, discreteValue, &retCode);
      if(retCode != FACE_NO_ERROR)
      {
         printf("Failed to set %d to %u: %d", channel, discreteValue,
            retCode);
      }
      else
      {
         printf("Set channel %d to %u successfully", channel, discreteValue);
      }

      // Reading
      discreteValue = readDiscrete(handle, channel, &retCode);
      if(retCode != FACE_NO_ERROR)
      {
         printf("Failed to read %d: %d", channel, retCode);
      }
      else
      {
         printf("Channel %d: %u", channel, discreteValue);
      }
   }

   // Close channels
   for(i = 0; i < 32; i++)
   {
      if(config[i].channel != 0)
      {
         FACE_IO_Close(handles[i], &retCode);
         if(retCode != FACE_NO_ERROR)
         {
            printf("Error occurred while closing %s: %d\n", config[i].name,
               retCode);
         }
         else
         {
            printf("Successfully closed %s\n", config[i].name);
         }
      }
   }

   return 0;
}

#define MAX_BUFF_SIZE 1024
#define PAYLOAD_LENGTH 4

void setDiscrete(FACE_INTERFACE_HANDLE_TYPE handle, int channel,
   uint8_t value, FACE_RETURN_CODE_TYPE *retCode)
{
   // The message and stuff we will need
   char txBuff[MAX_BUFF_SIZE];
   FACE_IO_MESSAGE_TYPE *txFaceMsg = (FACE_IO_MESSAGE_TYPE*)txBuff;

   // Zero it out
   memset(txBuff, 0, MAX_BUFF_SIZE);

   // Set the fixed fields
   // TODO: What should we set these GUIDs to??
   txFaceMsg->guid = htonl(100);
   txFaceMsg->busType = FACE_DISCRETE;
   txFaceMsg->message_type = htons(FACE_DATA);
   FaceSetPayLoadLength(txFaceMsg, PAYLOAD_LENGTH);

   FaceSetDiscreteChannelNumber(txFaceMsg, channel);

   FaceSetDiscreteState(txFaceMsg, value);

   FACE_IO_Write(handle, 0, FACE_MSG_HEADER_SIZE + PAYLOAD_LENGTH,
      txFaceMsg, retCode);
}

uint8_t readDiscrete(FACE_INTERFACE_HANDLE_TYPE handle, int channel,
   FACE_RETURN_CODE_TYPE *retCode)
{
   // The message and stuff we will need
   char rxBuff[MAX_BUFF_SIZE];
   FACE_MESSAGE_LENGTH_TYPE msgLen;
   FACE_IO_MESSAGE_TYPE *rxFaceMsg = (FACE_IO_MESSAGE_TYPE*)rxBuff;

   // Zero it out
   memset(rxBuff, 0, MAX_BUFF_SIZE);

   // Set the fixed fields
   // TODO: What should we set these GUIDs to??
   rxFaceMsg->guid = htonl(200);
   rxFaceMsg->busType = FACE_DISCRETE;
   rxFaceMsg->message_type = htons(FACE_DATA);
   FaceSetPayLoadLength(rxFaceMsg, PAYLOAD_LENGTH);

   FaceSetDiscreteChannelNumber(rxFaceMsg, channel);

   FACE_IO_Read(handle, 0, &msgLen, rxFaceMsg, retCode);

   return FaceDiscreteState(rxFaceMsg);
}
