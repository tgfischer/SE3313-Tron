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
        Socket theSocket("104.131.43.58", 2000);
        //Socket theSocket("192.168.0.12", 2000);
        theSocket.Open();
        std::string quitGame = "NO";
        std::string dir = "";
        int code;

		FlexWait waiter(2, &cinWatcher, &theSocket);

		std::string ready = "NO";

		while (ready == "NO") {
			ByteArray message;
			code = theSocket.Read(message);

			if (code == 0) {
				std::cout << "Server was closed. Terminating process" << std::endl;
				break;
			}

			if (message.ToString() == "LEFT" || message.ToString() == "RIGHT") {
				dir = message.ToString();
				ready = "YES";
			} else if (message.ToString() == "WAIT") {
				std::cout << "Waiting for another player to join. Type 'quit' to exit." << std::endl;

				Blockable* result = waiter.Wait();
				if (result == &theSocket) {
					theSocket.Read(message);
					theSocket.Write(ByteArray("STILL_HERE"));
				} else {
					std::string s;
					std::cin >> s;

					if (s=="quit")
					{
						// No need to call SocketServer::Shutdown.  It isn't active.
						code = 0;
						break;
					}
					else
						continue;
				}
			}
		}

		if (code != 0) {
			std::cout << "Ready!" << std::endl;

			Grid grid;

			clear();
			grid.draw();

			bt.off();

			while (quitGame == "NO")
			{
				Blockable* result = waiter.Wait();

				if (result == &theSocket) {
					quitGame = grid.recvSingleFrom(theSocket);
					theSocket.Write(ByteArray(dir));
					quitGame = grid.recvSingleFrom(theSocket);

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
						quitGame = "USER QUIT";
						break;
					}
				}
			}

			bt.on();
            if(quitGame.size() == 1)
                std::cout << "Other Player Quit - You Win!"<<std::endl;
            else
			    std::cout << quitGame << std::endl;
			theSocket.Close();
		}
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
