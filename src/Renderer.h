#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

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
	void RenderAllPiece(SDL_Renderer*);
	void DrawChessBoard(SDL_Renderer*);
	void DestroyAllPieceTextures();

	int GetBlockX(const int&);
	int GetBlockY(const int&);
	float Lerp(const int&, const int&, const float&);
	void AddGaussianBlur(SDL_Renderer*);
}

static const int WIDTH = 800;
static const int HEIGHT = 800;
//0 = game running, 1 = white checkmated black, 2 = black checkmated white, 3 = stalemate, 4 = deaposition, 5 = game start
extern int gameEnded;
extern int promotion;

extern Piece** boardPosition;