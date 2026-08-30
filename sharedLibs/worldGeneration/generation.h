#ifndef GENERATION_H
#define GENERATION_H
#include <raylib.h>

typedef struct {
    Vector3 position;
    Vector3 size;
    int type;
} Block;

typedef struct {
    Block *blocks;
    int blocks_size;
    int id;
} Chunk;

typedef struct {
    Chunk *chunks;
    int size;
} World;

Block buildBlock(Vector3 position, Vector3 size, int type);
Chunk generateChunk(Vector3 position, int id);

#endif