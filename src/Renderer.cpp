#include "Renderer.h"

#include "C_Files/icon.c"

//need these for making the objects
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

#include "C_Files/game_ended1.c"
#include "C_Files/game_ended2.c"
#include "C_Files/game_ended3.c"
#include "C_Files/game_start.c"

/*
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
*/

void MouseButtonPressed(SDL_Renderer*, bool& hasRenderedPossMove, const SDL_Event&, bool&);
void ButtonSelection(SDL_Renderer*, const SDL_Event&, bool);
void FreeAllHeapMemory();

void applyBlurRow(SDL_Surface*, int);
void applyBlurColumn(SDL_Surface*, int);
const int KERNEL_SIZE = 13;
const double GAUSSIAN_KERNEL[KERNEL_SIZE] = { 0.0188, 0.0345, 0.0566, 0.0832, 0.1095, 0.1292, 0.1365, 0.1292, 0.1095, 0.0832, 0.0566, 0.0345, 0.0188 };

Piece** boardPosition = new Piece* [64];
int gameEnded = 0;
int promotion = 99;

//must declare here or else it doesnt remember ehhhh
Piece* pieceClicked = nullptr;

void Chess::MainRenderer()
{
	bool hasRenderedPossMoves = false;
	//true = white false = black
	bool currentTurn = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;

	SDL_RWops* iconRW = SDL_RWFromMem((void*)icon_ico, sizeof(icon_ico));
	SDL_Surface* iconSurface = IMG_Load_RW(iconRW, 1);
	if (!iconSurface)
	{
		MissingTexture(gameEnded, "icon.ico");
		gameEnded = 1;
		return;
	}

	SDL_Window* Window = SDL_CreateWindow("Chess The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowIcon(Window, iconSurface);
	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

	//ChessBoard
	SDL_RenderClear(Renderer);
	Chess::DrawChessBoard(Renderer);
	SDL_RenderPresent(Renderer);

	bool initialRun = true;
	SDL_Event gameEvent;
	while (1)
	{
		if (SDL_WaitEvent(&gameEvent))
		{
			if (gameEvent.type == SDL_QUIT)
				break;
			else if (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_ESCAPE)
				break;
			else if (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_d)
			{
				//to debug anything
			}
			
			if (initialRun)
			{
				SDL_RenderClear(Renderer);
				Chess::DrawChessBoard(Renderer);
				initialRun = false;
				Chess::Init(Renderer);
				Chess::AddGaussianBlur(Renderer);
				
				//Draw play button
				SDL_Rect pieceRect{};
				pieceRect.w = WIDTH / 4;
				pieceRect.h = HEIGHT / 4;
				pieceRect.y = (WIDTH / 8) * 5;
				pieceRect.x = (HEIGHT / 8) * 3;

				SDL_RWops* tempRwops1 = SDL_RWFromMem((void*)game_start_png, sizeof(game_start_png));
				SDL_Surface* tempSurface1 = IMG_Load_RW(tempRwops1, 1);
				SDL_Texture* tempTexture1 = SDL_CreateTextureFromSurface(Renderer, tempSurface1);
				SDL_RenderCopy(Renderer, tempTexture1, nullptr, &pieceRect);
				SDL_RenderPresent(Renderer);
				promotion = 100;
			}

			if (gameEvent.type == SDL_MOUSEBUTTONDOWN && !gameEnded && promotion == 99)
			{
				MouseButtonPressed(Renderer, hasRenderedPossMoves, gameEvent, currentTurn);
			}
			else if (gameEvent.type == SDL_MOUSEBUTTONDOWN && promotion != 99)
			{
				ButtonSelection(Renderer, gameEvent, currentTurn);
			}
		}
		SDL_Delay(10);
	}

	//Freeing resources
	FreeAllHeapMemory();
	SDL_FreeRW(iconRW);
	SDL_FreeSurface(iconSurface);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Chess::Init(SDL_Renderer* Renderer)
{
	//BLACK PAWN
	boardPosition[0 + (1*8)] = new Pawn(Renderer, false, 0, 1);
	boardPosition[1 + (1*8)] = new Pawn(Renderer, false, 1, 1);
	boardPosition[2 + (1*8)] = new Pawn(Renderer, false, 2, 1);
	boardPosition[3 + (1*8)] = new Pawn(Renderer, false, 3, 1);
	boardPosition[4 + (1*8)] = new Pawn(Renderer, false, 4, 1);
	boardPosition[5 + (1*8)] = new Pawn(Renderer, false, 5, 1);
	boardPosition[6 + (1*8)] = new Pawn(Renderer, false, 6, 1);
	boardPosition[7 + (1*8)] = new Pawn(Renderer, false, 7, 1);

	//WHITE PAWN
	boardPosition[0 + (6*8)] = new Pawn(Renderer, true, 0, 6);
	boardPosition[1 + (6*8)] = new Pawn(Renderer, true, 1, 6);
	boardPosition[2 + (6*8)] = new Pawn(Renderer, true, 2, 6);
	boardPosition[3 + (6*8)] = new Pawn(Renderer, true, 3, 6);
	boardPosition[4 + (6*8)] = new Pawn(Renderer, true, 4, 6);
	boardPosition[5 + (6*8)] = new Pawn(Renderer, true, 5, 6);
	boardPosition[6 + (6*8)] = new Pawn(Renderer, true, 6, 6);
	boardPosition[7 + (6*8)] = new Pawn(Renderer, true, 7, 6);

	//BLACK KNIGHT
	boardPosition[1 + (0 * 8)] = new Knight(Renderer, false, 1, 0);
	boardPosition[6 + (0 * 8)] = new Knight(Renderer, false, 6, 0);

	//WHITE KNIGHT		  
	boardPosition[1 + (7 * 8)] = new Knight(Renderer, true, 1, 7);
	boardPosition[6 + (7 * 8)] = new Knight(Renderer, true, 6, 7);

	//BLACK BISHOP
	boardPosition[2 + (0*8)] = new Bishop(Renderer, false, 2, 0);
	boardPosition[5 + (0*8)] = new Bishop(Renderer, false, 5, 0);

	//WHITE BISHOP
	boardPosition[2 + (7*8)] = new Bishop(Renderer, true, 2, 7);
	boardPosition[5 + (7*8)] = new Bishop(Renderer, true, 5, 7);

	//BLACK ROOK
	boardPosition[0 + (0*8)] = new Rook(Renderer, false, 0, 0);
	boardPosition[7 + (0*8)] = new Rook(Renderer, false, 7, 0);

	//WHITE ROOK
	boardPosition[0 + (7*8)] = new Rook(Renderer, true, 0, 7);
	boardPosition[7 + (7*8)] = new Rook(Renderer, true, 7, 7);

	//BLACK QUEEN
	boardPosition[3 + (0 * 8)] = new Queen(Renderer, false, 3, 0);

	//WHITE QUEEN
	boardPosition[3 + (7 * 8)] = new Queen(Renderer, true, 3, 7);

	//BLACK KING 
	boardPosition[4 + (0 * 8)] = new King(Renderer, false, 4,0);

	//WHITE KING
	boardPosition[4 + (7 * 8)] = new King(Renderer, true, 4,7);

	//set 16-47 to nullptr
	for (int i = 16; i < 48; i++)
	{
		boardPosition[i] = nullptr;
	}
}

// Linearly interpolates between two values a and b by the interpolant t.
float Chess::Lerp(const int& a, const int& b, const float& t)
{
	return a + ((b - a) * t);
}


void Chess::RenderAllPiece(SDL_Renderer* Renderer)
{
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (boardPosition[i + (j * 8)])
			{
				boardPosition[i + (j * 8)]->RenderThePiece(Renderer, boardPosition[i + (j * 8)]->GetPieceType(), boardPosition[i + (j * 8)]->GetPieceTeam(), boardPosition[i + (j * 8)]->GetPieceX(), boardPosition[i + (j * 8)]->GetPieceY());
			}
		}
	}
}

int Chess::GetBlockX(const int& blockNumber)
{
	return blockNumber % 8;
}

int Chess::GetBlockY(const int& blockNumber)
{
	return blockNumber / 8;
}

void MouseButtonPressed(SDL_Renderer* Renderer, bool& hasRenderedPossMoves, const SDL_Event& gameEvent, bool& currentTurn)
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int xBlockStart = (WIDTH / 8) * x;
			//start of next block is end of first
			int xBlockEnd = (WIDTH / 8) * (x + 1);

			int yBlockStart = (HEIGHT / 8) * y;
			int yBlockEnd = (HEIGHT / 8) * (y + 1);

			//if Moves havent been rendered yet then render them
			if (gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
			{
				//clicked where some piece is present and piece belong to currentTurn's team
				if (boardPosition[x + (y * 8)] && boardPosition[x + (y * 8)]->GetPieceTeam() == currentTurn)
				{
					//Clear and Rerender pieces if they had any renderedPossMoves before
					if (hasRenderedPossMoves)
					{
						SDL_RenderClear(Renderer);
						Chess::DestroyAllPieceTextures();
						Chess::DrawChessBoard(Renderer);
						Chess::RenderAllPiece(Renderer);

						//clear any previous rendered position in the vector
						if (pieceClicked)
						{
							pieceClicked->PossibleMovesVector().clear();
							pieceClicked = nullptr;
						}
					}
					
					hasRenderedPossMoves = true;
					pieceClicked = boardPosition[x + (y * 8)];
					pieceClicked->RenderPossibleMoves(Renderer);
					SDL_RenderPresent(Renderer);

					//break out of both loops
					y = 8;
					break;
				}
				//clicked where no piece is present
				else if (hasRenderedPossMoves)
				{
					if (pieceClicked)
					{
						for (int z = 0; z < pieceClicked->PossibleMovesVector().size(); z++)
						{
							//if block cliked is same as rendered block
							if (pieceClicked->PossibleMovesVector()[z] == (x + (y * 8)))
							{
								pieceClicked->MoveThePiece(Renderer, (x + (y * 8)), currentTurn);
								//no need to loop anymore
								break;
							}
						}
						pieceClicked->PossibleMovesVector().clear();
						pieceClicked = nullptr;
					}
					//avoid screen refreshed when promoting/reaching endgame
					if (!gameEnded && promotion == 99)
					{
						//Clear and Rerender pieces if they had any renderedPossMoves before
						SDL_RenderClear(Renderer);
						Chess::DestroyAllPieceTextures();
						Chess::DrawChessBoard(Renderer);
						Chess::RenderAllPiece(Renderer);
						SDL_RenderPresent(Renderer);
					}
					//end screen set here and not promotion
					else if (promotion == 99)
					{
						Chess::AddGaussianBlur(Renderer);

						SDL_Rect pieceRect{};
						pieceRect.w = WIDTH / 2;
						pieceRect.h = HEIGHT / 2;
						pieceRect.y = pieceRect.h/4;
						pieceRect.x = pieceRect.w/2;

						if (gameEnded == 1)
						{
							SDL_RWops* tempRwops1 = SDL_RWFromMem((void*)game_ended1_png, sizeof(game_ended1_png));
							SDL_Surface* tempSurface1 = IMG_Load_RW(tempRwops1, 1);
							SDL_Texture* tempTexture1 = SDL_CreateTextureFromSurface(Renderer, tempSurface1);
							SDL_RenderCopy(Renderer, tempTexture1, nullptr, &pieceRect);
						}
						else if (gameEnded == 2)
						{
							SDL_RWops* tempRwops2 = SDL_RWFromMem((void*)game_ended2_png, sizeof(game_ended2_png));
							SDL_Surface* tempSurface2 = IMG_Load_RW(tempRwops2, 1);
							SDL_Texture* tempTexture2 = SDL_CreateTextureFromSurface(Renderer, tempSurface2);
							SDL_RenderCopy(Renderer, tempTexture2, nullptr, &pieceRect);
						}
						else if (gameEnded == 3 || gameEnded == 4)
						{
							SDL_RWops* tempRwops3 = SDL_RWFromMem((void*)game_ended3_png, sizeof(game_ended3_png));
							SDL_Surface* tempSurface3 = IMG_Load_RW(tempRwops3, 1);
							SDL_Texture* tempTexture3 = SDL_CreateTextureFromSurface(Renderer, tempSurface3);
							SDL_RenderCopy(Renderer, tempTexture3, nullptr, &pieceRect);
						}
						//button added for play button at ending here
						if (gameEnded == 1 || gameEnded == 2 || gameEnded == 3 || gameEnded == 4)
						{
							SDL_Rect pieceRect{};
							pieceRect.w = WIDTH / 4;
							pieceRect.h = HEIGHT / 4;
							pieceRect.y = (WIDTH / 8) * 5;
							pieceRect.x = (HEIGHT / 8) * 3;

							SDL_RWops* tempRwops4 = SDL_RWFromMem((void*)game_start_png, sizeof(game_start_png));
							SDL_Surface* tempSurface4 = IMG_Load_RW(tempRwops4, 1);
							SDL_Texture* tempTexture4 = SDL_CreateTextureFromSurface(Renderer, tempSurface4);
							SDL_RenderCopy(Renderer, tempTexture4, nullptr, &pieceRect);
							//promotion 101 is set for resetting the game
							promotion = 101;
						}
						SDL_RenderPresent(Renderer);
					}

					hasRenderedPossMoves = false;
					//break out of both loops
					y = 8;
					break;
				}
			}
		}
	}
}

void ButtonSelection(SDL_Renderer* Renderer, const SDL_Event& gameEvent, bool currentTurn)
{
	int xBlockStart;
	int xBlockEnd;
	int yBlockStart;
	int yBlockEnd;
	if (promotion != 100 && promotion != 101)
	{
		for (int mouseX = 2; mouseX < 6; mouseX++)
		{
			xBlockStart = (WIDTH / 8) * mouseX;
			xBlockEnd = (WIDTH / 8) * (mouseX + 1);
			yBlockStart = (HEIGHT / 8) * 3;
			yBlockEnd = (HEIGHT / 8) * 4;

			//if Moves havent been rendered yet then render them, promotion to 100 means start screen
			if (gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
			{
				//piece must exist or something fishy
				if (boardPosition[promotion])
					delete boardPosition[promotion];

				if (mouseX == 2)
				{
					boardPosition[promotion] = new Queen(Renderer, !currentTurn, (float)Chess::GetBlockX(promotion), (float)Chess::GetBlockY(promotion));
				}
				else if (mouseX == 3)
				{
					boardPosition[promotion] = new Bishop(Renderer, !currentTurn, (float)Chess::GetBlockX(promotion), (float)Chess::GetBlockY(promotion));
				}
				else if (mouseX == 4)
				{
					boardPosition[promotion] = new Knight(Renderer, !currentTurn, (float)Chess::GetBlockX(promotion), (float)Chess::GetBlockY(promotion));
				}
				else if (mouseX == 5)
				{
					boardPosition[promotion] = new Rook(Renderer, !currentTurn, (float)Chess::GetBlockX(promotion), (float)Chess::GetBlockY(promotion));

				}

				//Clear and Rerender stuff
				SDL_RenderClear(Renderer);
				Chess::DestroyAllPieceTextures();
				Chess::DrawChessBoard(Renderer);
				Chess::RenderAllPiece(Renderer);
				SDL_RenderPresent(Renderer);
				promotion = 99;
				break;
			}
		}
	}

	//this is the start screen
	if (promotion == 100 || promotion == 101)
	{
		xBlockStart = (WIDTH / 8) * 3;
		xBlockEnd = (WIDTH / 8) * 5;
		yBlockStart = (HEIGHT / 8) * 5;
		yBlockEnd = (HEIGHT / 8) * 7;


		if (promotion == 100 && gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
		{
			//Clear and Rerender stuff
			SDL_RenderClear(Renderer);
			Chess::DestroyAllPieceTextures();
			Chess::DrawChessBoard(Renderer);
			Chess::RenderAllPiece(Renderer);
			SDL_RenderPresent(Renderer);
			promotion = 99;
		}
		else if (promotion == 101 && gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
		{
			//Clear and Rerender stuff
			SDL_RenderClear(Renderer);
			FreeAllHeapMemory();
			Chess::DrawChessBoard(Renderer);
			Chess::Init(Renderer);
			SDL_RenderPresent(Renderer);
			//restart the game
			gameEnded = 0;
			promotion = 99;
		}
	}

}

void FreeAllHeapMemory()
{
	for (int i = 0; i < 64; i++)
	{
		//piece object deleted
		if (boardPosition[i])
		{
			delete boardPosition[i];
			boardPosition[i] = nullptr;
		}
	}
	//dont delete this incase i want to restart the game
	//delete[] boardPosition;
}

void Chess::MissingTexture(bool gameQuit, std::string filename)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing Textures", ("Could not find " + filename + "\nMake sure you have \"Resource Files\" folder\nand all the required textures").c_str(), NULL);
	gameQuit = true;
	return;
}

void Chess::DrawChessBoard(SDL_Renderer* Renderer)
{
	//Render the possible Moves Box here
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			SDL_Rect temp1{ x * (WIDTH / 8), y * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
			if ((y % 2) == (x+(y*8)) % 2)
				SDL_SetRenderDrawColor(Renderer, 235, 236, 208, 255);
			else
				SDL_SetRenderDrawColor(Renderer, 119, 149, 86, 255);

			SDL_RenderFillRect(Renderer, &temp1);
		}
	}
}

void Chess::DestroyAllPieceTextures()
{
	//Destroy All piece texture to free all the memory leaks
	for (int i = 0; i < 64; i++)
	{
		if (boardPosition[i])
		{
			SDL_DestroyTexture(boardPosition[i]->GetPieceTexture());
			SDL_FreeRW(boardPosition[i]->GetPieceRW());
			SDL_FreeSurface(boardPosition[i]->GetPieceSurface());
			boardPosition[i]->PossibleMovesVector().clear();
		}
	}
}

void Chess::AddGaussianBlur(SDL_Renderer* Renderer)
{
	// Create the surface to hold the blurred image
	SDL_Surface* blurSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);

	// Lock the texture and copy its pixel data to the surface
	SDL_Texture* texture = NULL; // Assume this is the texture we want to blur
	SDL_LockTexture(texture, NULL, &blurSurface->pixels, &blurSurface->pitch);
	SDL_RenderReadPixels(Renderer, NULL, blurSurface->format->format, blurSurface->pixels, blurSurface->pitch);
	SDL_UnlockTexture(texture);

	// Apply a blur to the surface
	for (int i = 0; i < blurSurface->h; i++) {
		// Apply one-dimensional blur to rows
		applyBlurRow(blurSurface, i);
	}

	for (int i = 0; i < blurSurface->w; i++) {
		// Apply one-dimensional blur to columns
		applyBlurColumn(blurSurface, i);
	}

	// Create a new texture from the blurred surface
	SDL_Texture* blurTexture = SDL_CreateTextureFromSurface(Renderer, blurSurface);

	// Use the new texture to render the blur effect on screen
	SDL_RenderCopy(Renderer, blurTexture, NULL, NULL);

	// Free the surface
	SDL_FreeSurface(blurSurface);
	SDL_DestroyTexture(blurTexture);
}

void applyBlurRow(SDL_Surface* surface, int row)
{
	Uint32* pixels = (Uint32*)surface->pixels;
	int width = surface->w;
	int height = surface->h;

	for (int x = 0; x < width; x++) {
		double r = 0, g = 0, b = 0, a = 0;

		for (int k = -KERNEL_SIZE / 2; k <= KERNEL_SIZE / 2; k++) {
			int idx = x + k;
			if (idx >= 0 && idx < width) {
				Uint32 pixel = pixels[idx + row * width];
				double weight = GAUSSIAN_KERNEL[k + KERNEL_SIZE / 2];

				r += weight * ((pixel & surface->format->Rmask) >> surface->format->Rshift);
				g += weight * ((pixel & surface->format->Gmask) >> surface->format->Gshift);
				b += weight * ((pixel & surface->format->Bmask) >> surface->format->Bshift);
				a += weight * ((pixel & surface->format->Amask) >> surface->format->Ashift);
			}
		}

		Uint32 newPixel = ((Uint32)(a + 0.5f) << surface->format->Ashift) |
			((Uint32)(r + 0.5f) << surface->format->Rshift) |
			((Uint32)(g + 0.5f) << surface->format->Gshift) |
			((Uint32)(b + 0.5f) << surface->format->Bshift);
		pixels[x + row * width] = newPixel;
	}
}
void applyBlurColumn(SDL_Surface* surface, int column)
{
	Uint32* pixels = (Uint32*)surface->pixels;
	int width = surface->w;
	int height = surface->h;

	for (int y = 0; y < height; y++) {
		double r = 0, g = 0, b = 0, a = 0;

		for (int k = -KERNEL_SIZE / 2; k <= KERNEL_SIZE / 2; k++) {
			int idx = y + k;
			if (idx >= 0 && idx < height) {
				Uint32 pixel = pixels[column + idx * width];
				double weight = GAUSSIAN_KERNEL[k + KERNEL_SIZE / 2];

				r += weight * ((pixel & surface->format->Rmask) >> surface->format->Rshift);
				g += weight * ((pixel & surface->format->Gmask) >> surface->format->Gshift);
				b += weight * ((pixel & surface->format->Bmask) >> surface->format->Bshift);
				a += weight * ((pixel & surface->format->Amask) >> surface->format->Ashift);
			}
		}

		Uint32 newPixel = ((Uint32)(a + 0.5f) << surface->format->Ashift) |
			((Uint32)(r + 0.5f) << surface->format->Rshift) |
			((Uint32)(g + 0.5f) << surface->format->Gshift) |
			((Uint32)(b + 0.5f) << surface->format->Bshift);
		pixels[column + y * width] = newPixel;
	}
}