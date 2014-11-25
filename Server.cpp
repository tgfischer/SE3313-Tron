#include "SharedObject.h"
#include "Semaphore.h"
#include "thread.h"
#include "Grid.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <pthread.h>
#include <vector>

class CommThread : public Thread
{
private:
    Socket socketA, socketB;
public:
    CommThread(Socket const& A, Socket const& B) : Thread(true), socketA(A), socketB(B)
    {
        ;
    }

    long ThreadMain(void)
    {
        ByteArray dirA, dirB;
        std::string message = "NO";
        Grid grid;

        std::cout << "Created a socket thread!" << std::endl;

        while(true)
        {
        	grid.sendTo(socketA);
        	grid.sendTo(socketB);

        	int checkA = socketA.Read(dirA);
        	int checkB = socketB.Read(dirB);

            if (checkA == -1 || checkB == -1)
            {
                std::cout << "Error in socket detected" << std::endl;
                break;
            }
            else if (checkA == 0 || checkB == 0)
            {
                std::cout << "Socket closed at remote end" << std::endl;
                break;
            }
            else
            {
            	std::cout << dirA.ToString() << " " << dirB.ToString() << std::endl;

            	bool p1 = grid.update(dirA.ToString(), grid.p1);
            	bool p2 = grid.update(dirB.ToString(), grid.p2);

            	if (!p1 || !p2) {
					if (!p1 && !p2) {
						message = "DRAW!";
					} else if (!p1) {
						message = "P2 WINS!";
					} else if (!p2) {
						message = "P1 WINS!";
					}
            	}

            	grid.sendTo(socketA, message);
            	grid.sendTo(socketB, message);
            }

            if (message != "NO") {
            	std::cout << message << std::endl;
            	break;
            }
        }

        std::cout << "Game Over\nThread is gracefully ending" << std::endl;
    }

    ~CommThread(void)
    {
        socketA.Write(ByteArray("done"));
        socketB.Write(ByteArray("done"));
        terminationEvent.Wait();
        socketA.Close();
        socketB.Close();
    }
};

int main(void)
{
    std::cout << "I am a socket server.  Type 'quit' to exit" << std::endl;
    SocketServer theServer(2000);
    std::vector<CommThread *> threads;
    std::vector<Socket> sockets;

    while(true)
    {
    	ByteArray msg_recv;

        try
        {
            FlexWait waiter(2, &theServer, &cinWatcher);
            Blockable * result = waiter.Wait();
            if (result == &cinWatcher)
            {
                std::string s;
                std::cin >> s;
                if (s=="quit")
                {
                    // No need to call SocketServer::Shutdown.  It isn't active.
                    break;
                }
                else
                    continue;
            }

            std::cout << "\nWaiting for 2 new clients..." << std::endl;

            while (true) {
				// Accept should not now block.
				Socket sock = theServer.Accept();
				std::cout << "Received a socket connection from a client" << std::endl;

				if (sockets.size() > 0) {
					sockets.front().Write(ByteArray("CHECK"));
					int check = sockets.front().Read(msg_recv);

					if (check == 0) {
						std::cout << "First client disconnected. Trying to find another client" << std::endl;

						sockets.clear();
						sock.Write(ByteArray("WAIT"));
						sockets.push_back(sock);
					} else {
						sockets.front().Write(ByteArray("RIGHT"));
						sock.Write(ByteArray("LEFT"));
						threads.push_back(new CommThread(sockets.at(0), sock));
						sockets.clear();
					}
				} else {
					sock.Write(ByteArray("WAIT"));
					sockets.push_back(sock);
				}
            }
        }
        catch(TerminationException e)
        {
            std::cout << "The socket server is no longer listening. Exiting now." << std::endl;
            break;
        }
        catch(std::string s)
        {
            std::cout << "thrown " << s << std::endl;
            break;
        }
        catch(...)
        {
            std::cout << "caught  unknown exception" << std::endl;
            break;
        }
    }
    for (int i=0;i<threads.size();i++)
        delete threads[i];

    std::cout << "Sleep now" << std::endl;
    sleep(1);
    std::cout << "End of main" << std::endl;

}
