#include "Queen.h"

Queen::Queen(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, QUEEN, m_pieceTeam, m_XPos, m_YPos)
{

}

Queen::~Queen()
{
	SDL_DestroyTexture(this->GetTexture());
}
