#include "face_common.h"
#include "face_messages.h"
#include "face_ios.h"
#include "config_parser.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
   if(argc < 2)
   {
      printf("Please specify a config file\n");
      return 1;
   }

   return 0;
}
