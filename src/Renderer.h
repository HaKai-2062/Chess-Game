#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

namespace Chess
{
	void MainRenderer();
	void MissingTexture();
	void Init();
	void RenderPieces(int, int, const char*, int);
	int GetBlockSelected(Uint16, Uint16);
}