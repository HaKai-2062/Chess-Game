#include "Renderer.h"
#include <iostream>

int WIDTH = 800;
int HEIGHT = 800;
bool gameQuit = false;

int boardPosition[64];
std::string DIRECTORY = "Resouce Files/";
std::vector<SDL_Texture*> pieceTextures;
SDL_Window* Window = SDL_CreateWindow("Chess The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

void Chess::MainRenderer()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	
	SDL_Surface* Icon = IMG_Load((DIRECTORY + "icon.ico").c_str());
	SDL_SetWindowIcon(Window, Icon);

	//ChessBoard
	SDL_Texture* Image = IMG_LoadTexture(Renderer, (DIRECTORY+"chessboard.png").c_str());
	if (!Icon || !Image)
	{
		MissingTexture();
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
				//std::cout << blockNumber<<std::endl;
				if ()
			}
			else if (initialRun)
			{
				SDL_RenderClear(Renderer);
				SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
				Chess::Init();
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


void Chess::Init()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			//0 means no piece 1 means enemy piece 2 means friendly piece
			boardPosition[y + x * 8] = 0;

			//ROCK
			if (y == 0 && (x == 0 || x == 7))
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "rock_bl.png", 0);
			}
			else if (y == 7 && (x == 0 || x == 7))
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "rock.png", 1);
			}
			//KNIGHTS
			else if (y == 0 && (x == 1 || x == 6))
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "knight_bl.png", 0);
			}
			else if (y == 7 && (x == 1 || x == 6))
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "knight.png", 1);
			}
			//BISHOPS
			else if (y == 0 && (x == 2 || x == 5))
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "bishop_bl.png", 0);
			}
			else if (y == 7 && (x == 2 || x == 5))
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "bishop.png", 1);
			}
			//QUEEN
			else if (y == 0 && x == 3)
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "queen_bl.png", 0);
			}
			else if (y == 7 && x == 3)
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "queen.png", 1);
			}
			//QUEEN
			else if (y == 0 && x == 4)
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "king_bl.png", 0);
			}
			else if (y == 7 && x == 4)
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "king.png", 1);
			}
			//PAWN
			else if (y == 1)
			{
				boardPosition[y + x * 8] = 1;
				Chess::RenderPieces(x, y, "pawn_bl.png", 0);
			}
			else if (y == 6)
			{
				boardPosition[y + x * 8] = 2;
				Chess::RenderPieces(x, y, "pawn.png", 1);
			}
		}
	}
}

void Chess::RenderPieces(int x, int y, const char* pieceName, int pieceTeam)
{
	SDL_Texture* pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + pieceName).c_str());
	if (!pieceTexture)
	{
		Chess::MissingTexture();
		return;
	}
	SDL_Rect temp{};
	temp.w = WIDTH/8;
	temp.h = HEIGHT/8;
	temp.x = x * temp.w;
	temp.y = y * temp.h;
	SDL_RenderCopy(Renderer, pieceTexture, nullptr, &temp);
	pieceTextures.push_back(pieceTexture);
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

void Chess::MissingTexture()
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing Textures", "Make sure you have \"Resource Files\" folder\nand all the required textures", NULL);
	gameQuit = true;
	return;
}
