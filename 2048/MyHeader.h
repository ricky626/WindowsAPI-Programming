#pragma once

extern HWND g_hWnd;

#define WINDOW_WIDTH 550
#define WINDOW_HEIGHT 600

#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4

#define TILE_SIZE 50

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

enum KEYBOARD_DIR
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
};

enum BLOCK_STATE
{
	BLOCK_STOP,
	BLOCK_MOVE,
};

typedef struct tagPoint
{
	int x, y;

	tagPoint() :x(0), y(0) {}
	tagPoint(int x, int y) : x(x), y(y) {}

}Point2D;

typedef struct tagTile
{
	Point2D pos;
	int option;

	tagTile() : option(0) {}
	tagTile(const Point2D& pt) : pos(pt), option(0) {}

}Tile;

typedef struct tagBlockInfo
{
	Point2D pos;
	int value;

	int index;

	int addCheck;

	tagBlockInfo() {}
	tagBlockInfo(const Point2D& pt, int val, int idx)
		: pos(pt), value(val), index(idx), addCheck(0) {}

}BlockInfo;

typedef struct tagRecordData
{
	double time;
	int key;

	Point2D newBlockPos;

	tagRecordData(double t, int k, const Point2D& pt) 
		: time(t), key(k), newBlockPos(pt) {}

}RecordData;