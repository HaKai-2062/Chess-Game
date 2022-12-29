#include "Pawn.h"
#include <iostream>
Pawn::Pawn(SDL_Renderer* m_Renderer, bool m_pieceTeam, int m_XPos, int m_YPos)
	: Piece(m_Renderer, PAWN, m_pieceTeam, m_XPos, m_YPos)
{

}

Pawn::~Pawn()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Pawn::RenderPossibleMoves(SDL_Renderer* Renderer, const bool& isWhite, const int& x, const int& y)
{
	Piece** boardPosition = Chess::GetBoardPos();
	//Render Black Pawn Moves
	if (currentTurn != isWhite && this->GetPieceTeam() != isWhite && !(this->HasPieceMoved()))
	{
		//2 blocks below are checked if empty or not
		if (!boardPosition[x + ((y + 1) * 8)] && !boardPosition[x + ((y + 2) * 8)])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y + 1) * (HEIGHT / 8),  WIDTH / 8 , 2 * HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
		//only the next block seems empty
		else if (!boardPosition[x + ((y + 1) * 8)])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y + 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
	}
	
	//Render White Pawn Moves
	else if (currentTurn == isWhite && this->GetPieceTeam() == isWhite && !(this->HasPieceMoved()))
	{
		//2 blocks above are checked if empty or not
		if (!boardPosition[x + ((y -1) * 8)] && !boardPosition[x + ((y - 2) * 8)])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y - 2) * (HEIGHT / 8),  WIDTH / 8 , 2 * HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
		//only the next block seems empty
		else if (!boardPosition[x + ((y -1) * 8)])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y - 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
	}
	
	//Black's Normal Move and no piece below
	else if (currentTurn != isWhite && this->GetPieceTeam() != isWhite)
	{
		if (!boardPosition[x + (y + 1) * 8])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y + 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
		//found enemy piece on right
		if (boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam() == isWhite)
		{
			SDL_Rect temp2{ (x + 1) * (WIDTH / 8), (y + 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp2);
		}
		//found enemy piece on right
		if (boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam() == isWhite)
		{
			SDL_Rect temp3{ (x - 1) * (WIDTH / 8), (y + 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp3);
		}
	}
	//White's Normal Move and no piece above
	else if (currentTurn == isWhite && this->GetPieceTeam() == isWhite)
	{
		if (!boardPosition[x + (y + 1) * 8])
		{
			SDL_Rect temp1{ x * (WIDTH / 8), (y - 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp1);
		}
		//found enemy piece on right
		if (boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam() != isWhite)
		{
			SDL_Rect temp2{ (x + 1) * (WIDTH / 8), (y - 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp2);
		}
		//found enemy piece on left
		if (boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam() != isWhite)
		{
			SDL_Rect temp3{ (x - 1) * (WIDTH / 8), (y - 1) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
			SDL_RenderFillRect(Renderer, &temp3);
		}
	}
}