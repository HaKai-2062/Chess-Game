#include "Renderer.h"
#include <iostream>

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

void Chess::MainRenderer()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	
	SDL_Surface* Icon = IMG_Load((DIRECTORY + "icon.ico").c_str());
	bool gameQuit = false;

	int boardPosition[64];

	std::vector<SDL_Texture*> pieceTextures;
	SDL_Window* Window = SDL_CreateWindow("Chess The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowIcon(Window, Icon);

	//ChessBoard
	SDL_Texture* Image = IMG_LoadTexture(Renderer, (DIRECTORY+"chessboard.png").c_str());
	if (!Image)
	{
		MissingTexture(gameQuit, "chessboard.png");
		gameQuit = true;
		return;
	}
	if (!Icon)
	{
		MissingTexture(gameQuit, "icon.ico");
		gameQuit = true;
		return;
	}

	//Centering Image
	SDL_Rect rectangle{};
	rectangle.w = 0;
	rectangle.h = 0;
	SDL_QueryTexture(Image, nullptr, nullptr, &rectangle.w, &rectangle.h);
	rectangle.x = (WIDTH - rectangle.w) / 2;
	rectangle.y = (HEIGHT - rectangle.h) / 2;

	bool initialRun = true;

	SDL_Event gameEvent;
	while (1)
	{
		if (SDL_WaitEvent(&gameEvent))
		{
			if (gameEvent.type == SDL_QUIT || gameQuit)
				break;
			else if (gameEvent.key.keysym.sym == SDLK_ESCAPE)
				break;
			else if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				int blockNumber = Chess::GetBlockSelected(gameEvent.button.x, gameEvent.button.y);
				std::cout << blockNumber<<std::endl;
			}
			else if (initialRun)
			{
				SDL_RenderClear(Renderer);
				SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
				Chess::Init(Renderer, boardPosition);
				initialRun = false;
				SDL_RenderPresent(Renderer);
			}
		}
		SDL_Delay(10);
	}

	//Freeing resources
	for (int i = 0; i < pieceTextures.size(); i++)
		SDL_DestroyTexture(pieceTextures[i]);
	SDL_DestroyTexture(Image);	
	SDL_FreeSurface(Icon);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);

	return;
}

void Chess::Init(SDL_Renderer* Renderer, int* const boardPosition)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			//0 means no piece 1 means enemy piece 2 means friendly piece
			boardPosition[y + x * 8] = 0;

			//ROCK
			if (y == 0 && x == 0)
			{
				boardPosition[y + x * 8] = 1;
				Rock rock1(Renderer, 1, x, y);
			}
			else if (y == 0 && x == 7)
			{
				boardPosition[y + x * 8] = 1;
				Rock rock2(Renderer, 1, x, y);
			}
			else if (y == 7 && x == 0 )
			{
				boardPosition[y + x * 8] = 2;
				Rock rock3(Renderer, 2, x, y);
			}
			else if (y == 7 && x == 7)
			{
				boardPosition[y + x * 8] = 2;
				Rock rock4(Renderer, 2, x, y);
			}

			//KNIGHTS
			else if (y == 0 && x == 1)
			{
				boardPosition[y + x * 8] = 1;
				Knight knight1(Renderer, 1, x, y);
			}
			else if (y == 0 && x == 6)
			{
				boardPosition[y + x * 8] = 1;
				Knight knight2(Renderer, 1, x, y);
			}
			else if (y == 7 && x == 1)
			{
				boardPosition[y + x * 8] = 2;
				Knight knight3(Renderer, 2, x, y);
			}
			else if (y == 7 && x == 6)
			{
				boardPosition[y + x * 8] = 2;
				Knight knight4(Renderer, 2, x, y);
			}
			//BISHOPS
			else if (y == 0 && x == 2)
			{
				boardPosition[y + x * 8] = 1;
				Bishop bishop1(Renderer, 1, x, y);
			}
			else if (y == 0 && x == 5)
			{
				boardPosition[y + x * 8] = 1;
				Bishop bishop2(Renderer, 1, x, y);
			}
			else if (y == 7 && x == 2)
			{
				boardPosition[y + x * 8] = 2;
				Bishop bishop3(Renderer, 2, x, y);
			}
			else if (y == 7 && x == 5)
			{
				boardPosition[y + x * 8] = 2;
				Bishop bishop4(Renderer, 2, x, y);
			}
			//QUEEN
			else if (y == 0 && x == 3)
			{
				boardPosition[y + x * 8] = 1;
				Queen queen1(Renderer, 1, x, y);
			}
			else if (y == 7 && x == 3)
			{
				boardPosition[y + x * 8] = 2;
				Queen queen2(Renderer, 2, x, y);
			}
			//KING
			else if (y == 0 && x == 4)
			{
				boardPosition[y + x * 8] = 1;
				King king1(Renderer, 1, x, y);
			}
			else if (y == 7 && x == 4)
			{
				boardPosition[y + x * 8] = 2;
				King king2(Renderer, 2, x, y);
			}
			//PAWN
			else if (y == 1)
			{
				boardPosition[y + x * 8] = 1;
				Pawn pawn1(Renderer, 1, x, y);
				Pawn pawn2(Renderer, 1, x, y);
				Pawn pawn3(Renderer, 1, x, y);
				Pawn pawn4(Renderer, 1, x, y);
				Pawn pawn5(Renderer, 1, x, y);
				Pawn pawn6(Renderer, 1, x, y);
				Pawn pawn7(Renderer, 1, x, y);
				Pawn pawn8(Renderer, 1, x, y);
			}
			else if (y == 6)
			{
				boardPosition[y + x * 8] = 2;
				Pawn pawn9(Renderer, 1, x, y);
				Pawn pawn10(Renderer, 1, x, y);
				Pawn pawn11(Renderer, 1, x, y);
				Pawn pawn12(Renderer, 1, x, y);
				Pawn pawn13(Renderer, 1, x, y);
				Pawn pawn14(Renderer, 1, x, y);
				Pawn pawn15(Renderer, 1, x, y);
				Pawn pawn16(Renderer, 1, x, y);
			}
		}
	}
}

int Chess::GetBlockSelected(Uint16 mouseX, Uint16 mouseY)
{
	//returns block number from 1-64
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int xBlockStart = (WIDTH / 8) * x;
			//start of next block is end of first
			int xBlockEnd = (WIDTH / 8) * (x + 1);

			int yBlockStart = (HEIGHT / 8) * y;
			int yBlockEnd = (HEIGHT / 8) * (y + 1);


			if (mouseX > xBlockStart && mouseX <= xBlockEnd && mouseY > yBlockStart && mouseY <= yBlockEnd)
				return (x+y*8)+1;
		}
	}
	//invalid selection
	return 69;
}

void Chess::MissingTexture(bool gameQuit, std::string filename)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing Textures", ("Could not find "+filename+"\nMake sure you have \"Resource Files\" folder\nand all the required textures").c_str(), NULL);
	gameQuit = true;
	return;
}
