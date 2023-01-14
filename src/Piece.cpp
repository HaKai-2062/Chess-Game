#include <chrono>
#include <thread>

//need these files for promotion stuff
#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

#include <cmath>

Piece::Piece(SDL_Renderer* Renderer, PieceType pieceType, bool pieceTeam, float xPos, float yPos)
	:m_Renderer(Renderer), m_pieceType(pieceType), m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	RenderThePiece(Renderer, pieceType, pieceTeam, xPos, yPos);
}
Piece::~Piece()
{
}

bool Piece::isBlackInCheck = false;
bool Piece::isWhiteInCheck = false;
int Piece::blackKingPos = 4;
int Piece::whiteKingPos = 60;

int Piece::enPassant = 99;
int Piece::enPassantEnd = 99;

int Piece::castleBlockBlack[2] = { 99, 99 };
int Piece::castleBlockWhite[2] = { 99, 99 };


void Piece::RenderThePiece(SDL_Renderer* Renderer, const PieceType& pieceType, const bool& pieceTeam, const float& xPos, const float& yPos)
{
	std::string fileName = "";
	int textureNum = 0;
	//get piecetexture
	switch (m_pieceType)
	{
	case PAWN:
		fileName = "pawn";
		if (!m_pieceTeam)
			textureNum = 1;
		else
			textureNum = 2;
		break;
	case KNIGHT:
		fileName = "knight";
		if (!m_pieceTeam)
			textureNum = 3;
		else
			textureNum = 4;
		break;
	case BISHOP:
		fileName = "bishop";
		if (!m_pieceTeam)
			textureNum = 5;
		else
			textureNum = 6;
		break;
	case ROOK:
		fileName = "rook";
		if (!m_pieceTeam)
			textureNum = 7;
		else
			textureNum = 8;
		break;
	case QUEEN:
		fileName = "queen";
		if (!m_pieceTeam)
			textureNum = 9;
		else
			textureNum = 10;
		break;
	case KING:
		fileName = "king";
		if (!m_pieceTeam)
			textureNum = 11;
		else
			textureNum = 12;
		break;
	default:
		break;
	}

	SDL_Texture* pieceTexture = Chess::GetPieceTexture(textureNum);
	
	if (!pieceTexture)
	{
		std::cout << "texture error\n";
		if (!m_pieceTeam)
			Chess::MissingTexture(false, fileName + "_bl.png");
		else
			Chess::MissingTexture(false, fileName + ".png");
		return;
	}

	SDL_Rect pieceRect{};
	pieceRect.w = WIDTH / 8;
	pieceRect.h = HEIGHT / 8;
	pieceRect.x = (int)(m_XPos * pieceRect.w);
	pieceRect.y = (int)(m_YPos * pieceRect.h);

	SDL_RenderCopy(Renderer, pieceTexture, nullptr, &pieceRect);
}

void Piece::MoveThePiece(SDL_Renderer* Renderer, int boardPositionToMove, bool& currentTurn)
{
	if (currentTurn != GetPieceTeam())
		return;
	
	int xStart = static_cast<int>(m_XPos);
	int yStart = static_cast<int>(m_YPos);

	int xEnd = Chess::GetBlockX(boardPositionToMove);
	int yEnd = Chess::GetBlockY(boardPositionToMove);
	
	// Set the number of steps for the animation
	//better if it is power of 2
	float steps = 64;

	// Calculate the x and y step sizes for the animation
	float xStep = (xEnd - xStart) / steps;
	float yStep = (yEnd - yStart) / steps;

	//std::cout << xStep << "   " << yStep << std::endl;

	for (int i = 0; i < steps; i++)
	{
		m_XPos += xStep;
		m_YPos += yStep;

		//The piece is rerendered here
		SDL_RenderClear(Renderer);
		Chess::DrawChessBoard(Renderer);
		Chess::RenderAllPiece(Renderer);
		SDL_RenderPresent(Renderer);

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	int startPosition = xStart + (yStart * 8);
	int endPosition = xEnd + (yEnd * 8);

	//if piece exists then destroy it
	if (boardPosition[endPosition])
		delete boardPosition[endPosition];
	//assign new position to the piece
	boardPosition[endPosition] = boardPosition[startPosition];
	//set prev piece position to null
	boardPosition[startPosition] = nullptr;

	//if king then set its position and set the kingInCheck flag
	Piece::SetKingVariables();

	//Set EnPassant Move here
	Piece::SetEnPassant(xStart, yStart, xEnd, yEnd);

	//Set Castling Move here
	Piece::SetCastling(Renderer, xEnd, yEnd, currentTurn);

	//Set Promotion here
	Piece::SetPawnPromotion(Renderer, xEnd, yEnd);

	//highlight the blocks moved
	blocksMoved[0] = xStart + yStart * 8;
	blocksMoved[1] = xEnd + yEnd * 8;
	
	//piece moved set to true
	m_hasMoved = true;

	//change the turn
	currentTurn = !currentTurn;

	Piece::EndGameReached();
}

void Piece::RenderPossMovesBlock(SDL_Renderer* Renderer)
{
	//Render the possible Moves Box here
	for (int i = 0; i < PossibleMovesVector().size(); i++)
	{
		SDL_Rect temp1{ Chess::GetBlockX(PossibleMovesVector()[i]) * (WIDTH / 8), Chess::GetBlockY(PossibleMovesVector()[i]) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
		//enemy piece found
		if (boardPosition[PossibleMovesVector()[i]] && boardPosition[PossibleMovesVector()[i]]->GetPieceTeam() != GetPieceTeam())
		{
			SDL_SetRenderDrawColor(Renderer, 255, 20, 25, 128);
		}
		//coloring if we are caching the piece by enPassant
		else if (Piece::enPassantEnd != 99 && !boardPosition[PossibleMovesVector()[i]] && Piece::enPassantEnd == PossibleMovesVector()[i])
		{
			SDL_SetRenderDrawColor(Renderer, 115, 0, 247, 150);
		}
		//set castling piece color here
		else if (GetPieceType() == KING && !boardPosition[PossibleMovesVector()[i]] && (PossibleMovesVector()[i] == Piece::castleBlockWhite[0] || PossibleMovesVector()[i] == Piece::castleBlockWhite[1] || PossibleMovesVector()[i] == Piece::castleBlockBlack[0] || PossibleMovesVector()[i] == Piece::castleBlockBlack[1]))
		{
			SDL_SetRenderDrawColor(Renderer, 115, 0, 247, 150);
		}
		//empty block found
		else if (!boardPosition[PossibleMovesVector()[i]])
		{
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
		}
		SDL_RenderFillRect(Renderer, &temp1);
	}
}

void Piece::SetKingVariables()
{
	std::vector<int> enemyPiece;

	//if "this" is king then save position
	if (GetPieceTeam() && GetPieceType() == KING)
		Piece::whiteKingPos = static_cast<int>(m_XPos) + static_cast<int>(m_YPos * 8);
	else if (!GetPieceTeam() && GetPieceType() == KING)
		Piece::blackKingPos = static_cast<int>(m_XPos) + static_cast<int>(m_YPos * 8);

	//save the vars as false after a move has been performed
	if (GetPieceTeam())
		Piece::isWhiteInCheck = false;
	else
		Piece::isBlackInCheck = false;

	//set check here by calculating enemy piece's next moves
	enemyPiece.clear();
	enemyPiece = CalculatePossibleMoves();
	for (int i = 0; i < enemyPiece.size(); i++)
	{
		//white piece
		if (GetPieceTeam() && enemyPiece[i] == blackKingPos)
		{
			std::cout << "Black King Checked at: " << Piece::blackKingPos << std::endl;
			Piece::isBlackInCheck = true;
		}
		//black piece
		else if (!GetPieceTeam() && enemyPiece[i] == whiteKingPos)
		{
			std::cout << "White King Checked at: " << Piece::whiteKingPos << std::endl;
			Piece::isWhiteInCheck = true;
		}
	}
}

bool Piece::EndGameReached()
{
	//Stalemate and Checkmates are set to true initially and moves are calculated later on
	bool staleMateWhite = true;
	bool staleMateBlack = true;

	int tempVar1 = 0;
	int tempVar2 = 0;

	//enum value is stored and we evaluate the pieces left
	std::vector<int> friendlyPieces;
	std::vector<int> enemyPieces;
	friendlyPieces.clear();
	enemyPieces.clear();

	for (int i = 0; i < 64; i++)
	{
		if (boardPosition[i] && GetPieceTeam() != boardPosition[i]->GetPieceTeam())
		{
			//6 is a light bishop
			if (boardPosition[i]->GetPieceType() == BISHOP)
			{
				int x = static_cast<int>(boardPosition[i]->m_XPos);
				int y = static_cast<int>(boardPosition[i]->m_YPos);

				//light bishops (even)
				if ((y % 2) == (x + (y * 8)) % 2)
					enemyPieces.push_back(boardPosition[i]->GetPieceType());
				//dark bishops
				else
					enemyPieces.push_back(6);
			}
			//All Pieces except bishops are stored
			if (boardPosition[i]->GetPieceType() != BISHOP)
				enemyPieces.push_back(boardPosition[i]->GetPieceType());
			
			boardPosition[i]->CalculateLegitMoves();
			//White Piece so Black King is under danger
			if (GetPieceTeam())
			{
				//this variable just counts our pieces
				tempVar1++;
				if (Piece::isBlackInCheck && boardPosition[i]->PossibleMovesVector().size() == 0)
					//this variables is used to store the pieces that can no longer make a move
					tempVar2++;

				if (boardPosition[i]->PossibleMovesVector().size() != 0)
					staleMateWhite = false;
			}

			//Black Piece so White King is under danger
			else if (!GetPieceTeam())
			{
				//this variable just counts our pieces
				tempVar1++;
				if (Piece::isWhiteInCheck && boardPosition[i]->PossibleMovesVector().size() == 0)
					//this variables is used to store the pieces that can no longer make a move
					tempVar2++;
				
				if (boardPosition[i]->PossibleMovesVector().size() != 0)
					staleMateBlack = false;
			}

			boardPosition[i]->PossibleMovesVector().clear();
		}
		else if (boardPosition[i] && GetPieceTeam() == boardPosition[i]->GetPieceTeam())
		{
			//6 is a light bishop
			if (boardPosition[i]->GetPieceType() == BISHOP)
			{
				int x = static_cast<int>(boardPosition[i]->m_XPos);
				int y = static_cast<int>(boardPosition[i]->m_YPos);

				//light bishops (even)
				if ((y % 2) == (x + (y * 8)) % 2)
					friendlyPieces.push_back(boardPosition[i]->GetPieceType());
				//dark bishops
				else
					friendlyPieces.push_back(6);
			}
			//All Pieces except bishops are stored
			if (boardPosition[i]->GetPieceType() != BISHOP)
				friendlyPieces.push_back(boardPosition[i]->GetPieceType());
		}
	}
	
	if (GetPieceTeam() && tempVar1 == tempVar2)
	{
		std::cout << "White Checkmated Black" << std::endl;
		gameEnded = 1;
		return true;
	}
	else if (!GetPieceTeam() && tempVar1 == tempVar2)
	{
		std::cout << "Black Checkmated White" << std::endl;
		gameEnded = 2;
		return true;
	}
	else if (GetPieceTeam() && staleMateWhite)
	{
		std::cout << "Stalemate reached for Black" << std::endl;
		gameEnded = 3;
		return true;
	}
	else if (!GetPieceTeam() && staleMateBlack)
	{
		std::cout << "Stalemate reached for White" << std::endl;
		gameEnded = 3;
		return true;
	}

	//only checking this condition because after the change of turn this condition will automatically be set to true
	if (friendlyPieces.size() >= enemyPieces.size())
	{
		if (friendlyPieces.size() == 1 && friendlyPieces[0] == KING && enemyPieces[0] == KING)
		{
			std::cout << "DeadPosition: King vs King" << std::endl;
			gameEnded = 4;
			return true;
		}
		else if (friendlyPieces.size() == 2 && enemyPieces.size() == 1 && ((friendlyPieces[0] == KING && friendlyPieces[1] == BISHOP) || (friendlyPieces[1] == KING && friendlyPieces[0] == BISHOP)) && enemyPieces[0] == KING)
		{
			std::cout << "DeadPosition: King and Bishop vs King" << std::endl;
			gameEnded = 4;
			return true;
		}
		else if (friendlyPieces.size() == 2 && enemyPieces.size() == 1 && ((friendlyPieces[0] == KING && friendlyPieces[1] == KNIGHT) || (friendlyPieces[1] == KING && friendlyPieces[0] == KNIGHT)) && enemyPieces[0] == KING)
		{
			std::cout << "DeadPosition: King and Knight vs King" << std::endl;
			gameEnded = 4;
			return true;
		}
		else if (friendlyPieces.size() == 2 && enemyPieces.size() == 2)
		{
			if ((friendlyPieces[0] == KING && friendlyPieces[1] == BISHOP || (friendlyPieces[1] == KING && friendlyPieces[0] == BISHOP)) && (enemyPieces[0] == KING && enemyPieces[1] == BISHOP || (enemyPieces[1] == KING && enemyPieces[0] == BISHOP)))
			{
				std::cout << "DeadPosition: King and Bishop(Light Squared) vs King and Bishop(Light Squared)" << std::endl;
				gameEnded = 4;
				return true;
			}
			else if ((friendlyPieces[0] == KING && friendlyPieces[1] == 6 || (friendlyPieces[1] == KING && friendlyPieces[0] == 6)) && (enemyPieces[0] == KING && enemyPieces[1] == 6 || (enemyPieces[1] == KING && enemyPieces[0] == 6)))
			{
				std::cout << "DeadPosition: King and Bishop(Dark Squared) vs King and Bishop(Dark Squared)" << std::endl;
				gameEnded = 4;
				return true;
			}
		}
	}
	
	return false;
}

void Piece::CalculateLegitMoves()
{
	PossibleMovesVector().clear();
	std::vector<int> friendlyPieceMoves;
	friendlyPieceMoves.clear();
	friendlyPieceMoves = CalculatePossibleMoves();

	for (int i = 0; i < friendlyPieceMoves.size(); i++)
	{
		//checks if our move puts our king in danger and also sacrifices to save the king
		if (IsLegitMove(friendlyPieceMoves[i]))
			PossibleMovesVector().push_back(friendlyPieceMoves[i]);
	}
}

bool Piece::IsLegitMove(const int& pieceMove)
{
	std::vector<int> enemyPieceMoves;
	Piece* tempPiece = nullptr;

	int x = static_cast<int>(m_XPos);
	int y = static_cast<int>(m_YPos);
	bool legalMove = true;

	//if enemy piece is present then store it temporarily
	if (boardPosition[pieceMove])
		tempPiece = boardPosition[pieceMove];

	//temporarily save the position of our piece
	boardPosition[pieceMove] = boardPosition[x + (y * 8)];
	boardPosition[x + (y * 8)] = nullptr;

	enemyPieceMoves.clear();
	for (int i = 0; i < 64; i++)
	{
		//can this enemy piece attack my king?
		if (boardPosition[i] && GetPieceTeam() != boardPosition[i]->GetPieceTeam())
		{
			enemyPieceMoves = boardPosition[i]->CalculatePossibleMoves();
			for (int j = 0; j < enemyPieceMoves.size(); j++)
			{
				if (GetPieceType() != KING && ((GetPieceTeam() && enemyPieceMoves[j] == Piece::whiteKingPos) || (!GetPieceTeam() && enemyPieceMoves[j] == Piece::blackKingPos)))
				{
					legalMove = false;
					i = 64;
					break;
				}
				else if (GetPieceType() == KING && enemyPieceMoves[j] == pieceMove)
				{
					legalMove = false;
					i = 64;
					break;
				}
			}
		}
	}

	//restored the temporary position stored
	boardPosition[x + (y * 8)] = boardPosition[pieceMove];

	if (tempPiece)
		boardPosition[pieceMove] = tempPiece;
	else
		boardPosition[pieceMove] = nullptr;

	tempPiece = nullptr;

	return legalMove;
}

void Piece::SetEnPassant(const int& xStart, const int& yStart, const int& x, const int& y)
{
	//take out enemy by enPassant
	if (GetPieceType() == PAWN && (Piece::enPassant == xStart + (yStart * 8) || Piece::enPassant == (xStart + (yStart * 8)) * 100))
	{
		//White Pawn took Black by enPassant
		if (GetPieceTeam())
		{
			delete boardPosition[x + ((y + 1) * 8)];
			boardPosition[x + ((y + 1) * 8)] = nullptr;
		}

		//Black Pawn took White by enPassant
		else
		{
			delete boardPosition[x + ((y - 1) * 8)];
			boardPosition[x + ((y - 1) * 8)] = nullptr;
		}

		//enPassant resetted
		Piece::enPassant = 99;
		Piece::enPassantEnd = 99;
	}

	//Set enPassant to false if some other piece has moved
	else if (Piece::enPassant != 99 && Piece::enPassant != xStart + (yStart * 8) && Piece::enPassant != (xStart + (yStart * 8)) * 100)
	{
		Piece::enPassant = 99;
		Piece::enPassantEnd = 99;
	}

	//if "this" can be captured by enPassant then save it
	else if (!HasPieceMoved() && GetPieceType() == PAWN && (GetPieceTeam() && y == 4 || !GetPieceTeam() && y == 3))
	{
		//piece on right of board
		if (boardPosition[(x + 1) + (y * 8)] && boardPosition[(x + 1) + (y * 8)]->GetPieceTeam() != GetPieceTeam() && boardPosition[(x + 1) + (y * 8)]->GetPieceType() == PAWN)
		{
			Piece::enPassant = ((x + 1) + (y * 8));
			
			if (GetPieceTeam())
				Piece::enPassantEnd = x + (y + 1) * 8;
			else
				Piece::enPassantEnd = x + (y - 1) * 8;
		}
		//piece on left of board
		else if (boardPosition[(x - 1) + (y * 8)] && boardPosition[(x - 1) + (y * 8)]->GetPieceTeam() != GetPieceTeam() && boardPosition[(x - 1) + (y * 8)]->GetPieceType() == PAWN)
		{
			Piece::enPassant = ((x - 1) + (y * 8)) * 100;
			
			if (GetPieceTeam())
				Piece::enPassantEnd = x + (y + 1) * 8;
			else
				Piece::enPassantEnd = x + (y - 1) * 8;
		}
	}
}

void Piece::SetCastling(SDL_Renderer* Renderer, const int& x, const int& y, bool& currentTurn)
{
	// White Team Castle
	if (GetPieceType() == KING && GetPieceTeam() && (Piece::castleBlockWhite[0] != 99 || Piece::castleBlockWhite[1] != 99))
	{
		//on left hand side
		if (x == Chess::GetBlockX(Piece::castleBlockWhite[0]))
		{
			// added 1 because rook goes one block to right of king
			int temp = Piece::castleBlockWhite[0] + 1;
			Piece::castleBlockWhite[0] = 99;
			if (boardPosition[0 + y * 8])
				boardPosition[0 + y * 8]->MoveThePiece(Renderer, temp, currentTurn);
			currentTurn = !currentTurn;
		}
		//on right hand side
		else if (x == Chess::GetBlockX(Piece::castleBlockWhite[1]))
		{
			// subtracted 1 because rook goes one block to left of king
			int temp = Piece::castleBlockWhite[1] - 1;
			Piece::castleBlockWhite[1] = 99;
			if (boardPosition[7 + y * 8])
				boardPosition[7 + y * 8]->MoveThePiece(Renderer, temp, currentTurn);
			currentTurn = !currentTurn;
		}
	}
	// Black Team Castle
	else if (GetPieceType() == KING && !GetPieceTeam() && (Piece::castleBlockBlack[0] != 99 || Piece::castleBlockBlack[1] != 99))
	{
		//on left hand side
		if (x == Chess::GetBlockX(Piece::castleBlockBlack[0]))
		{
			// added 1 because rook goes one block to right of king
			int temp = Piece::castleBlockBlack[0] + 1;
			Piece::castleBlockBlack[0] = 99;
			if (boardPosition[0 + y * 8])
				boardPosition[0 + y * 8]->MoveThePiece(Renderer, temp, currentTurn);
			currentTurn = !currentTurn;
		}
		//on right hand side
		else if (x == Chess::GetBlockX(Piece::castleBlockBlack[1]))
		{
			// subtracted 1 because rook goes one block to left of king
			int temp = Piece::castleBlockBlack[1] - 1;
			Piece::castleBlockBlack[1] = 99;
			if (boardPosition[7 + y * 8])
				boardPosition[7 + y * 8]->MoveThePiece(Renderer, temp, currentTurn);
			currentTurn = !currentTurn;
		}
	}
}
void Piece::SetPawnPromotion(SDL_Renderer* Renderer, const int& x, const int& y)
{
	//since pawns dont move back so only checking boundary conditions
	if (GetPieceType() == PAWN &&  (y == 0 || y == 7))
	{
		Chess::AddGaussianBlur(Renderer);

		SDL_Texture* tempTexture1 = nullptr;
		SDL_Texture* tempTexture2 = nullptr;
		SDL_Texture* tempTexture3 = nullptr;
		SDL_Texture* tempTexture4 = nullptr;

		SDL_Rect pieceRect{};
		pieceRect.w = WIDTH / 8;
		pieceRect.h = HEIGHT / 8;
		pieceRect.y = 3 * pieceRect.h;

		//Queen image
		pieceRect.x = 2 * pieceRect.w;
		if (GetPieceTeam())
			tempTexture1 = Chess::GetPieceTexture(10);
		else
			tempTexture1 = Chess::GetPieceTexture(9);
		SDL_RenderCopy(Renderer, tempTexture1, nullptr, &pieceRect);

		//Bishop
		pieceRect.x = 3 * pieceRect.w;
		if (GetPieceTeam())
			tempTexture2 = Chess::GetPieceTexture(6);
		else
			tempTexture2 = Chess::GetPieceTexture(5);
		SDL_RenderCopy(Renderer, tempTexture2, nullptr, &pieceRect);

		//Knight
		pieceRect.x = 4 * pieceRect.w;
		if (GetPieceTeam())
			tempTexture3 = Chess::GetPieceTexture(4);
		else
			tempTexture3 = Chess::GetPieceTexture(3);
		SDL_RenderCopy(Renderer, tempTexture3, nullptr, &pieceRect);

		//Rook
		pieceRect.x = 5 * pieceRect.w;
		if (GetPieceTeam())
			tempTexture4 = Chess::GetPieceTexture(8);
		else
			tempTexture4 = Chess::GetPieceTexture(7);
		SDL_RenderCopy(Renderer, tempTexture4, nullptr, &pieceRect);

		SDL_RenderPresent(Renderer);

		//set game state to promotion
		promotion = x + y * 8;
	}
}