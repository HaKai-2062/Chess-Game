#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rock.h"

static const int WIDTH = 800;
static const int HEIGHT = 800;
static const std::string DIRECTORY = "Resouce Files/";

namespace Chess
{
	void MainRenderer();
	void Init(SDL_Renderer*, int* const boardPosition);
	void MissingTexture(bool, std::string);
	int GetBlockSelected(Uint16, Uint16);
}