#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

/*
struct PieceInfo
{
public:
	int pieceX = 0, pieceY = 0, pieceTeam = 0, pieceType = 0;
	PieceInfo(int x, int y, int team, int type)
		: pieceX(x), pieceY(y), pieceTeam(team), pieceType(type) {};
};
*/
class Piece;
namespace Chess
{
	void MainRenderer();
	void Init(SDL_Renderer*);
	void MissingTexture(bool, std::string);
	int GetBlockSelected(Uint16, Uint16);
	Piece** GetBoardPos();
	void RenderAllPiece(SDL_Renderer*);
}

static const int WIDTH = 800;
static const int HEIGHT = 800;
static const std::string DIRECTORY = "Resoucre Files/";

//true = white false = enemy
static bool currentTurn = false;
