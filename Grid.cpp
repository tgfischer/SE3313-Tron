/*
 * Grid.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: tomfischer
 */

#include "Grid.h"
#include <math.h>
#include <iostream>

Grid::Grid()
: p1((int)floor(this->cols / 4), (int)floor(this->rows / 2), '1'),
  p2((int)(3 * floor(this->cols / 4)), (int)floor(this->rows / 2), '2') {
	this->_grid = new char*[this->cols];

	this->initialize();
}

/**
 * Delete all of the pointers
 */
Grid::~Grid() {
}

/**
 * Initialize the grid
 */
void Grid::initialize() {
	for (int i = 0; i < this->cols; i++) {
		this->_grid[i] = new char[this->rows];

		for (int j = 0; j < this->rows; j++) {
			this->_grid[i][j] = ' ';
		}
	}

	this->_grid[this->p1.x][this->p1.y] = this->p1.value;
	this->_grid[this->p2.x][this->p2.y] = this->p2.value;
}

/**
 * Checks for collisions. If there are none, put the player in that cell
 */
bool Grid::update(std::string dir, Player& p) {
	if (dir == "DOWN") {
		if (p.y + 1 >= this->rows || this->_grid[p.x][p.y + 1] != ' ')
			return false;

		this->_grid[p.x][++p.y] = p.value;
	} else if (dir == "RIGHT") {
		if (p.x + 1 >= this->cols || this->_grid[p.x + 1][p.y] != ' ')
			return false;

		this->_grid[++p.x][p.y] = p.value;
	} else if (dir == "UP") {
		if (p.y - 1 < 0 || this->_grid[p.x][p.y - 1] != ' ')
			return false;

		this->_grid[p.x][--p.y] = p.value;
	} else if (dir == "LEFT") {
		if (p.x - 1 < 0 || this->_grid[p.x - 1][p.y] != ' ')
			return false;

		this->_grid[--p.x][p.y] = p.value;
	} else {
		return false;
	}

	return true;
}

void Grid::sendTo(Socket& sock, std::string gameOver) {
	ByteArray returned;

	for (int i = 0; i < this->cols; i++) {
		for (int j = 0; j < this->rows; j++) {
			sock.Write(ByteArray(std::string(1, this->_grid[i][j])));
			sock.Read(returned);
		}
	}


	sock.Write(ByteArray(gameOver));
}

bool Grid::recvFrom(Socket& sock) {
	ByteArray byte;

	for (int i = 0; i < this->cols; i++) {
		for (int j = 0; j < this->rows; j++) {
			sock.Read(byte);
			this->_grid[i][j] = byte.ToString()[0];
			sock.Write(ByteArray("Got it"));
		}
	}

	sock.Read(byte);
	std::string gameOver = byte.ToString();

	if (gameOver != "NO") {
		std::cout << gameOver << std::endl;
		return false;
	}

	return true;
}

/**
 * Draws the grid
 */
void Grid::draw() {
	std::cout << "+";

	for (int i = 0; i < this->cols; i++) {
		std::cout << "-";
	}

	std::cout << "+" << std::endl;

	for (int j = 0; j < this->rows; j++) {
		std::cout << "|";

		for (int i = 0; i < this->cols; i++) {
			std::cout << this->_grid[i][j];
		}

		std::cout << "|" << std::endl;
	}

	std::cout << "+";

		for (int i = 0; i < this->cols; i++) {
			std::cout << "-";
		}

	std::cout << "+" << std::endl;
}
