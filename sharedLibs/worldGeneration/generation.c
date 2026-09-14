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
    chunk.chunkPos = position;

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

bool checkIfChunked(World *world, Vector3 pos) {
    for (size_t i = 0; i < world->size; i++) {
        if (Vector3Equals(world->chunks[i].chunkPos, pos) > 1) {
            return true;
        }
    }

    return false;
}

void generateChunkGroups(World *world, Vector3 center, int size) {
    int distancia = 0;

    while (world->size < size) {
        for (int x = -distancia; x <= distancia; x++) {
            for (int z = -distancia; z <= distancia; z++) {
                Vector3 target = newVector3(center.x + x * 17, center.y, center.z + z * 17);

                if (!checkIfChunked(world, target)) {
                    world->chunks[world->size] = generateChunk(target, world->size);
                }

                if (world->size >= size) {
                    return;
                }
            }
        }

        distancia++;
    }
}