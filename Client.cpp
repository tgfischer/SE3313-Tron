#include "SharedObject.h"
#include "Semaphore.h"
#include "socket.h"
#include "Grid.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses>

#define clear() printf("\033[H\033[J");

int main(void)
{
    try
    {
        //Socket theSocket("10.0.2.15", 2000);
        Socket theSocket("127.0.0.1", 2000);
        theSocket.Open();
        std::string entry = "";
        std::string dir = "";

		FlexWait waiter(2, &cinWatcher, &theSocket);


		ByteArray message;
		theSocket.Read(message);

		if (message.ToString() == "WAIT") {
			std::cout << "Waiting for another player to join..." << std::endl;
			dir = "RIGHT";

			theSocket.Read(message);
		}

		if (dir != "RIGHT")
			dir = "LEFT";

		std::cout << "Ready!" << std::endl;

		Grid grid;

        while (entry != "done")
        {
            Blockable* result = waiter.Wait();

            if (result == &theSocket) {
            	grid.recvFrom(theSocket);

				clear();

				grid.draw();

				theSocket.Write(ByteArray(dir));
            } else {
            	int entry;

            	switch(getc(stdin)) {
            	case 'w':
            		dir = "UP";
            		break;
            	case 'a':
            		dir = "LEFT";
            		break;
            	case 's':
            		dir = "DOWN";
            		break;
            	case 'd':
            		dir = "RIGHT";
            		break;
            	}
            }
        }

        std::cout << "Sleep now" << std::endl;
        theSocket.Close();
        sleep(1);
    }
    catch(std::string s)
    {
        std::cout << s << std::endl;
    }
    catch(...)
    {
        std::cout << "Caught unexpected exception" << std::endl;
    }
}
