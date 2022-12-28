#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>


namespace Chess
{
	void MainRenderer();
	void Init(SDL_Renderer*);
	void MissingTexture(bool, std::string);
	int GetBlockSelected(Uint16, Uint16);
}

static bool gameQuit = false;
static const int WIDTH = 800;
static const int HEIGHT = 800;
static const std::string DIRECTORY = "Resoucre Files/";

//true = white false = enemy
static bool currentTurn = true;