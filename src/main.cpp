#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include "uihelper.h"


using namespace std;

bool endProcess = false;

UIHelper* uiHelper;

void booter()
{
	uiHelper = UIHelper::getInstance();
}

void destructor()
{
	if (uiHelper)
		delete uiHelper;
}

void killProcess(int /*param*/)
{
    endProcess = true;
}


int main(int argc, char* argv[])
{
    signal(SIGTERM, killProcess);

	if (argc < 2) {
		booter();
	}
	else
	{
		printf("Invalid argument\n");
		return -1;
	}

    //char input;
    while (!endProcess)
    {
        sleep(2);
    }

	destructor();

	printf("uihelper exitted\n");

	return 0;
}
