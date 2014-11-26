SE3313-Tron
===========
This is a multiplayer game called Tron. This game allows users to play others from different clients

How To Use
-----------
1. In the Server.cpp file, change the IP in the line `Socket theSocket("172.30.74.242", 2000);` to the IP that the Server.cpp file runs on. You can also change the port, but make sure you also change the port in the Client.cpp file
2. Open up the terminal that the Server.cpp file will run on, and type `make Server`, and then `./Server`. This will start your server
3. Open up the terminal that will run the first Client.cpp file (Player 1). Type `make Client`, and then `./Client`.
4. Open up the terminal that will run the second Client.cpp file (Player 2). Type `make Client`, and then `./Client`.
5. This should start the game. Enjoy!
