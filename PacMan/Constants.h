#pragma once

/* OpenGL window constants */
#define WINDOW_X 400
#define WINDOW_Y 20		// window's position

#define WINDOW_W 560		// window's width
#define WINDOW_H 750		// window's height

#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

#define MAP_W 560			
#define MAP_H 620

/* GAME constants */
#define MAP_WIDTH 28
#define MAP_HEIGHT 31
#define NUM_ROW 28
#define NUM_COL 31			// same

#define BLOCK_SIZE 20
#define TILE_SIZE 20		// block size == tile size

#define MOVE_SPEED 10

// Map boundary
#define LEFT_BOUNDARY (-(NUM_ROW / 2) * BLOCK_SIZE)
#define RIGHT_BOUNDARY ((NUM_ROW / 2) * BLOCK_SIZE)
#define TOP_BOUNDARY ((NUM_COL) * (BLOCK_SIZE / 2))
#define BOTTOM_BOUNDARY (-(NUM_COL) * (BLOCK_SIZE / 2))

typedef enum { u, w, h, p, o, O } tileType;
/*
    u : undefined
    w : wall
    h : half-wall
    p : path
    o : small coin
    O : big coin
*/
