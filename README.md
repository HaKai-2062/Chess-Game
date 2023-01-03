# Chess The Game

![Chess The Game](https://imgur.com/a/cfOHRsO)

# Introduction

This repository contains a beta version of a chess game but at the moment it lacks the special chess rules like en passant or castling. It uses [SDL2](https://github.com/libsdl-org/SDL) and [SDL2_image](https://github.com/libsdl-org/SDL_image) for drawing and rendering the textures.

# Softwares used
## Visual Studio
[Visual Studio](https://visualstudio.microsoft.com/vs/) is an integrated development environment (IDE) and was used to build the project into exe to run on windows.

## bin2c
[bin2c](https://sourceforge.net/projects/bin2c/) was used to convert all the assets present inside the Resources Files folder into a C format so that they can be included inside the project without having the need of finding them at runtime.
```
You can add the bin2c.exe and C_File_Generator.bat inside the Resource Folder and generate the C files yourself using the batfile if you like.
 ```

# Buidling the Game
## Windows
- Make sure you have [Git](https://git-scm.com/downloads) installed on your system.
- Go to the directory where you want to store all the raw files and build it.
- Right click inside the directory anmd open Git Bash.
- Paste the following command in gut bash:
```
    git clone https://github.com/HaKaiiii/Chess-The-Game.git
```
- After that open **Chess The Game.sln** in Visual Studio to build the project from solution.
- All the required libraries are already present (statically linked) and all the assets were taken from [PNGEGG](https://www.pngegg.com)

# Future Plans
It currently lacks basic functionalities but after implementing basic functionalities, the plan is to add an AI and probably extend it to be multiplayer(p2p) with basic chat functionalities as well.

# License
[GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)