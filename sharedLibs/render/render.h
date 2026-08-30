#ifndef RENDER_H
#define RENDER_H
#include "../clientState.h"
#include "../worldGeneration/generation.h"

Vector3 newVector3(float x, float y, float z);
void startRender(ClientState *clientState);
void renderizarMundo(World world);
void carregarRecursos();

#endif