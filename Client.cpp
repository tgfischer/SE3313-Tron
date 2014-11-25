#include "SharedObject.h"
#include "Semaphore.h"
#include "socket.h"
#include "Grid.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include "BufferToggle.cpp"

#define clear() printf("\033[H\033[J");

int main(void)
{
    BufferToggle bt;

    try
    {
        Socket theSocket("192.168.0.16", 2000);
        //Socket theSocket("127.0.0.1", 2000);
        theSocket.Open();
        std::string quitGame = "NO";
        std::string dir = "";

		FlexWait waiter(2, &cinWatcher, &theSocket);

		std::string ready = "NO";

		while (ready == "NO") {
			ByteArray message;
			theSocket.Read(message);

			if (message.ToString() == "LEFT" || message.ToString() == "RIGHT") {
				dir = message.ToString();
				ready = "YES";
			} else if (message.ToString() == "WAIT") {
				std::cout << "Waiting for another player to join..." << std::endl;

				theSocket.Read(message);
				theSocket.Write(ByteArray("STILL_HERE"));
			} else {

			}
		}

		std::cout << "Ready!" << std::endl;

		Grid grid;

		bt.off();

        while (quitGame == "NO")
        {
            Blockable* result = waiter.Wait();

            if (result == &theSocket) {
            	grid.recvFrom(theSocket);
				theSocket.Write(ByteArray(dir));
				quitGame = grid.recvFrom(theSocket);

				clear();

				grid.draw();
            } else {
            	switch(getchar()) {
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
            	case 'q':
					quitGame = true;
					break;
            	}
            }
        }

        bt.on();

        std::cout << quitGame << std::endl;
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
