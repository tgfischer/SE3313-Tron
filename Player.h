/*
 * Player.h
 *
 *  Created on: Nov 24, 2014
 *      Author: tomfischer
 */

#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
public:
	Player(int x, int y, char value);
	virtual ~Player();

	int x;
	int y;
	char value;
};

#endif /* PLAYER_H_ */
