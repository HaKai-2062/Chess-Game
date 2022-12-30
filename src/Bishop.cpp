#include "Bishop.h"

Bishop::Bishop(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, BISHOP, m_pieceTeam, m_XPos, m_YPos)
{

}

Bishop::~Bishop()
{
	SDL_DestroyTexture(this->GetTexture());
}
