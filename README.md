SE3313-Tron
===========
This is a multiplayer game called Tron. This game allows users to play others from different clients

How To Use
-----------
1. In the Client.cpp file, change the IP in the line `Socket theSocket("104.131.43.58", 2000);` to the IP of the server that you will upload this project to. You can also change the port, but you have to make sure it matches the IP in the Server.cpp file
2. Upload this project to a server, and two clients
2. Open up the terminal for the server, and type `make Server`, and then `./Server`. This will start your server
3. Open up the terminal for the first client (Player 1). Type `make Client`, and then `./Client`.
4. Open up the terminal for the second client (Player 2). Type `make Client`, and then `./Client`.
5. This should start the game. Enjoy!

Notes
-----------
This application was meant to run in the command line. It was built and tested using the Terminal in Ubuntu 14.04
