#include <stdio.h>

#ifdef COLOR
#define FAILMESSAGE "\x1b[31mFAILED\x1b[39m\n"
#define GOODMESSAGE "\x1b[32mSUCCESS\x1b[39m\n"
#else
#define FAILMESSAGE "FAILED\n"
#define GOODMESSAGE "SUCCESS\n"
#endif


void repl()
{
	printf("h for help\n");
	while(1)
	{
		
		char command;
		int channel = -1;
		printf(">");
		scanf("%c", &command);
		if(command == 'h')
		{
			printf("s 5 -- set discrete 5\n");
			printf("c 5 -- clear discrete 5\n");
			printf("r 5 -- read discrete 5\n");
			printf("q   -- quit\n");
			printf("h   -- help\n");
			scanf("%c", &command);//discard the character so we don't get >> (I don't like this)
			continue;
		}
		if(command == 'q')
			return;
		if(command == '\n')
			continue;
		if(command != 's' && command != 'c' && command != 'r')
		{
			while(command != '\n')
				scanf("%c", &command);
			//ungetc(command, stdin);//this is pretty bad
			continue;
		}
		scanf("%d", &channel);
		//test if channel is available here
		if(command == 's')
		{
			int result = 1;
			printf("set channel %d:", channel);
			//setDiscrete(channel, 1);
			printf(result?FAILMESSAGE:GOODMESSAGE);
		}
		if(command == 'c')
		{
			int result = 1;
			printf("clear channel %d:", channel);
			//setDiscrete(channel, 0);
			printf(result?FAILMESSAGE:GOODMESSAGE);
		}
		if(command == 'r')
		{
			int result = 1;
			int value = -1;
			printf("read channel %d:", channel);
			//value = readDiscrete(channel)
			printf(result?FAILMESSAGE:GOODMESSAGE);
			printf("value of channel %d:%d\n", channel, value); 
		}
		scanf("%c", &command);//discard the character so we don't get >> (I don't like this)
	}

}

#ifdef TESTREPL
int main(){repl();}
#endif
