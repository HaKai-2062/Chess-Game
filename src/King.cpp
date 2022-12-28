#include "King.h"
#include <iostream>
King::King(SDL_Renderer* m_Renderer, int m_pieceTeam, int m_XPos, int m_YPos)
	: Piece(m_Renderer, KING, m_pieceTeam, m_XPos, m_YPos)
{

}

King::~King()
{
	SDL_DestroyTexture(m_pieceTexture);
}
