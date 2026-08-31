#ifndef PACKET_H
#define PACKET_H
#include "../player/player.h"
#include "../worldGeneration/generation.h"

typedef struct {
    Player player;
    char packet[1048];
} PlayerPacket;

typedef struct {
    Block block;
    char packet[1048];
} BlockPacket;

typedef struct {
    Chunk chunk;
    char packet[1048];
} ChunkPacket;

#endif