/*
 * Grid.h
 *
 *  Created on: Nov 24, 2014
 *      Author: tomfischer
 */

#include "Player.h"
#include "socket.h"
#include <string>

#ifndef GRID_H_
#define GRID_H_

class Grid {
public:
	Grid();
	virtual ~Grid();

	Player p1, p2;

	bool check(std::string dir, Player& p);
	void update(std::string dir, Player& p);
	void sendTo(Socket& sock, std::string gameOver = "NO");
	void sendSingleTo(Socket& sock, std::string gameOver ="NO");
	std::string recvFrom(Socket& sock);
	std::string recvSingleFrom(Socket& sock);
	void draw();

	const static int cols = 24;
	const static int rows = 15;

private:
	char** _grid;

	std::string to_string(int value);

	void initialize();
};

#endif /* GRID_H_ */
