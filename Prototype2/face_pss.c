#include "face_common.h"
#include "face_messages.h"
#include "face_ios.h"
#include "config_parser.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      printf("Please specify a config file\n");
      return 1;
   }

   return 0;
}

#define MAX_BUFF_SIZE 1024

void setDiscrete(int channel, _Bool value)
{
   FACE_RETURN_CODE_TYPE retCode;
   FACE_MESSAGE_LENGTH_TYPE msgLen;
   char txBuff[MAX_BUFF_SIZE];      // MAX_BUFF_SIZE
   char rxBuff[MAX_BUFF_SIZE];
   FACE_IO_MESSAGE_TYPE *txFaceMsg = (FACE_IO_MESSAGE_TYPE*)txBuff;
   FACE_IO_MESSAGE_TYPE *rxFaceMsg = (FACE_IO_MESSAGE_TYPE*)rxBuff;

   memset(txBuff, 0, MAX_BUFF_SIZE);
   memset(rxBuff, 0, MAX_BUFF_SIZE);
}

_Bool readDiscrete(int channel)
{
}
