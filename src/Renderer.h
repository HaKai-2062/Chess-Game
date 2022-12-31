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
	Piece** GetBoardPos();
	void RenderAllPiece(SDL_Renderer*);

	int GetBlockX(const int&);
	int GetBlockY(const int&);
	float Lerp(const int&, const int&, const float&);
}

static const int WIDTH = 800;
static const int HEIGHT = 800;
static const std::string DIRECTORY = "Resoucre Files/";