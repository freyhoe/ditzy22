#pragma once
#include <vector>
#include <iostream>
#include <array>
#include <ctime>
#include <ratio>
#include <chrono>

const int HEIGHT = 21;
const int WIDTH = 10;
const int MAX_COMBO = 13;
const int COMBO_TABLE[MAX_COMBO] = {0,0,1,1,1,2,2,3,3,4,4,4,5};
const int PIECE_SPAWN[2] = {4,19};
enum PieceType : int8_t {
	PIECE_I,
	PIECE_O,
	PIECE_T,
	PIECE_L,
	PIECE_J,
	PIECE_S,
	PIECE_Z,
	PIECE_NULL,
};

enum RotationType : int8_t{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

const int PIECE_SHAPES[7][4][4][2] = {//7 piece types, 4 rotation states, 4 minos, xy coords
	{
		{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },
		{ { 0, 1 }, { 0, 0 }, { 0, -1 }, { 0, -2 } },
		{ { 1, 0 }, { 0, 0 }, { -1, 0 }, { -2, 0 } },
		{ { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },
	},
	{
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } } ,
		{ { 0, 0 }, { 0, -1 }, { 1, 0 }, { 1, -1 } } ,
		{ { 0, 0 }, { -1, 0 }, { 0, -1 }, { -1, -1 } } ,
		{ { 0, 0 }, { 0, 1 }, { -1, 0 }, { -1, 1 } } ,
	},
	{
		{ { 0, 1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } } ,
		{ { 1, 0 }, { 0, 1 }, { 0, 0 }, { 0, -1 } } ,
		{ { 0, -1 }, { 1, 0 }, { 0, 0 }, { -1, 0 } } ,
		{ { -1, 0 }, { 0, -1 }, { 0, 0 }, { 0, 1 } } ,
	},
	{
		{ { 1, 1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } } ,
		{ { 1, -1 }, { 0, 1 }, { 0, 0 }, { 0, -1 } } ,
		{ { -1, -1 }, { 1, 0 }, { 0, 0 }, { -1, 0 } } ,
		{ { -1, 1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } } ,
	},
	{
		{ { -1, 1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } } ,
		{ { 1, 1 }, { 0, 1 }, { 0, 0 }, { 0, -1 } } ,
		{ { 1, -1 }, { 1, 0 }, { 0, 0 }, { -1, 0 } } ,
		{ { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } } ,
	},
	{
		{ { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } } ,
		{ { 0, 1 }, { 0, 0 }, { 1, 0 }, { 1, -1 } } ,
		{ { 1, 0 }, { 0, 0 }, { 0, -1 }, { -1, -1 } } ,
		{ { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } } ,
	},
	{
		{ { 1, 0 }, { 0, 0 }, { 0, 1 }, { -1, 1 } } ,
		{ { 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } } ,
		{ { -1, 0 }, { 0, 0 }, { 0, -1 }, { 1, -1 } } ,
		{ { 0, 1 }, { 0, 0 }, { -1, 0 }, { -1, -1 } } ,
	},
};

enum ActionType: int8_t{
	CW,
	FLIP,
	CCW,
	LEFT,
	RIGHT,
	SD,
};

const int KICK_TABLE[2][4][3][5][2]=//i piece kicks then others (o not needed), 4 rotation states, 3 actions (cw,ccw,180), 5 offset sets
{
	{
			{
				{ { 1, 0 }, { -1, 0 }, { 2, 0 }, { -1, -1 }, { 2, 2 } },
				{ { 1, -1 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
				{ { 0, -1 }, { -1, -1 }, { 2, -1 }, { -1, 1 }, { 2, -2 } },
			},
			{
				{ { 0, -1 }, { -1, -1 }, { 2, -1 }, { -1, 1 }, { 2, -2 } },
				{ { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
				{ { -1, 0 }, { 1, 0 }, { -2, 0 }, { 1, 1 }, { -2, -2 } },
			},
			{
				{ { -1, 0 }, { 1, 0 }, { -2, 0 }, { 1, 1 }, { -2, -2 } },
				{ { -1, 1 }, { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
				{ { 0, 1 }, { 1, 1 }, { -2, 1 }, { 1, -1 }, { -2, 2 } },
			},
			{
				{ { 0, 1 }, { 1, 1 }, { -2, 1 }, { 1, -1 }, { -2, 2 } },
				{ { 1, 1 }, { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
				{ { 1, 0 }, { -1, 0 }, { 2, 0 }, { -1, -1 }, { 2, 2 } },
			}
	},
	{
		{
			{ { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
			{ { 0, 0 }, { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
			{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
		},
		{
			{ { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } },
			{ { 0, 0 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
			{ { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } },
		},
		{
			{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
			{ { 0, 0 }, { 0, -1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
			{ { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
		},
		{
			{ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } },
			{ { 0, 0 }, { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
			{ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } },
		},
	}
};

enum ClearType: int8_t {
	CLEAR0,
	CLEAR1,
	CLEAR2,
	CLEAR3,
	CLEAR4,
	TSMS,
	TSS,
	TSD,
	TST,
	PC,
	B2B,
};

const int ATTACK_TABLE[11] = {0,0,1,2,4,0,2,4,6,10,1};