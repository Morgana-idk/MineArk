#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "generation.h"
#include "../render/render.h"

Block buildBlock(Vector3 position, Vector3 size, int type, int chunkMom) {
    return (Block){position, size, type, chunkMom};
}

Chunk generateChunk(Vector3 position, int id) {
    Chunk chunk = {0};
    chunk.blocks = calloc(1048, sizeof(Block));
    chunk.blocks_size = 0;
    chunk.id = id;

    for (int x = -8; x < 9; x++) {
        for (int z = -8; z < 9; z++) {
            Vector3 target = Vector3Add(position, newVector3(x, 0, z));
            Block block = buildBlock(target, newVector3(1, 1, 1), 0, id);

            chunk.blocks[chunk.blocks_size] = block;
            chunk.blocks_size++;
        }
    }

    return chunk;
}