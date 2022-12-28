#include "Bishop.h"

Bishop::Bishop(SDL_Renderer* m_Renderer, int m_pieceTeam, int m_XPos, int m_YPos)
	: Piece(m_Renderer, BISHOP, m_pieceTeam, m_XPos, m_YPos)
{

}

Bishop::~Bishop()
{
	SDL_DestroyTexture(m_pieceTexture);
}
