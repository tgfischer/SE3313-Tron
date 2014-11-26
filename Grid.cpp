/*
 * Grid.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: tomfischer
 */

#include "Grid.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>

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
bool Grid::check(std::string dir, Player& p) {
	if (dir == "DOWN" && (p.y + 1 >= this->rows || this->_grid[p.x][p.y + 1] != ' ')) {
		return false;
	} else if (dir == "RIGHT" && (p.x + 1 >= this->cols || this->_grid[p.x + 1][p.y] != ' ')) {
		return false;
	} else if (dir == "UP" && (p.y - 1 < 0 || this->_grid[p.x][p.y - 1] != ' ')) {
		return false;
	} else if (dir == "LEFT" && (p.x - 1 < 0 || this->_grid[p.x - 1][p.y] != ' ')) {
		return false;
	}

	return true;
}

/**
 * Checks for collisions. If there are none, put the player in that cell
 */
void Grid::update(std::string dir, Player& p) {
	if (dir == "DOWN") {
		this->_grid[p.x][++p.y] = p.value;
	} else if (dir == "RIGHT") {
		this->_grid[++p.x][p.y] = p.value;
	} else if (dir == "UP") {
		this->_grid[p.x][--p.y] = p.value;
	} else if (dir == "LEFT") {
		this->_grid[--p.x][p.y] = p.value;
	}
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

void Grid::sendSingleTo(Socket& sock, std::string gameOver) {
	ByteArray returned;
	char buf[33];

	sock.Write(ByteArray(this->to_string(this->p1.x)));
	sock.Read(returned);
	sock.Write(ByteArray(this->to_string(this->p1.y)));
	sock.Read(returned);
	sock.Write(ByteArray(this->to_string(this->p1.value)));
	sock.Read(returned);
	sock.Write(ByteArray(this->to_string(this->p2.x)));
	sock.Read(returned);
	sock.Write(ByteArray(this->to_string(this->p2.y)));
	sock.Read(returned);
	sock.Write(ByteArray(this->to_string(this->p2.value)));
	sock.Read(returned);
	sock.Write(ByteArray(gameOver));
	sock.Read(returned);
}

std::string Grid::recvFrom(Socket& sock) {
	ByteArray byte;

	for (int i = 0; i < this->cols; i++) {
		for (int j = 0; j < this->rows; j++) {
			sock.Read(byte);
			this->_grid[i][j] = byte.ToString()[0];
			sock.Write(ByteArray("Got it"));
		}
	}

	sock.Read(byte);
	return byte.ToString();
}

std::string Grid::recvSingleFrom(Socket& sock) {
	ByteArray x, y, value, gameOver;

	for (int i = 0; i < 2; i++) {
		sock.Read(x);
		sock.Write(ByteArray("RECEIVED"));
		sock.Read(y);
		sock.Write(ByteArray("RECEIVED"));
		sock.Read(value);
		sock.Write(ByteArray("RECEIVED"));

		this->_grid[atoi(x.ToString().c_str())][atoi(y.ToString().c_str())] = value.ToString()[0];
	}

	sock.Read(gameOver);
	sock.Write(ByteArray("RECEIVED"));

	return gameOver.ToString();
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

std::string Grid::to_string(int value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << value ;

  //convert the string stream into a string and return
  return os.str() ;
}
