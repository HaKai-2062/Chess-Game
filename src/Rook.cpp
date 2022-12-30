#include "Rook.h"

Rook::Rook(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, ROOK, m_pieceTeam, m_XPos, m_YPos)
{

}

Rook::~Rook()
{
	SDL_DestroyTexture(this->GetTexture());
}
