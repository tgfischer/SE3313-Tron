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
        std::cout << "Created a socket thread!" << std::endl;

        Grid grid;

        while(true)
        {
        	grid.sendTo(socketA);
        	grid.sendTo(socketB);

        	sleep(1);

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

            	if (!grid.update(dirA.ToString(), grid.p1) || !grid.update(dirB.ToString(), grid.p2))
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

    while(true)
    {
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

            // Accept should not now block.
            Socket socketA = theServer.Accept();
            std::cout << "Received a socket connection from client 1" << std::endl;
            socketA.Write(ByteArray("WAIT"));

            Socket socketB = theServer.Accept();
            std::cout << "Received a socket connection from client 2" << std::endl;

            socketA.Write(ByteArray("GO"));
            socketB.Write(ByteArray("GO"));
            threads.push_back(new CommThread(socketA, socketB));
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
