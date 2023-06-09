# TetrisSDL
An implementation of the game Tetris using C++ and SDL2.

## Pictures
<img width="800" alt="obraz" src="https://user-images.githubusercontent.com/13346371/230686177-4d1b31b1-3b28-4912-993a-6bf7ab8732da.png">
<img width="806" alt="obraz" src="https://user-images.githubusercontent.com/13346371/230686238-1d5d5491-a787-4920-9ddf-0262f3376705.png">

## Controls
Use the LEFT and RIGHT arrow keys to move the Tetromino  
Use the UP and DOWN arrow keys to rotate the Tetromino  
Use SPACE to drop down  
Use ESC to pause the game
## Requirements
[SDL2](https://github.com/libsdl-org/SDL)  
[SDL_mixer](https://github.com/libsdl-org/SDL_mixer)  
[SDL_ttf](https://github.com/libsdl-org/SDL_ttf)  
[SDL_image](https://github.com/libsdl-org/SDL_image/)

Note that the CMakeLists.txt is set up using hard coded links to the libraries except for SDL2,
as the FindSDL_XXX does not work correctly. You may need to update them to compile everything.

## Credits
Background Music: '[Bit Bit Loop](https://freepd.com/electronic.php)' by Kevin MacLeod  
Sound Effects: '[8 bit sound effect pack](https://opengameart.org/content/8-bit-sound-effect-pack)' by OwlishMedia

Font: '[Minimalist Pixel Fonts](https://opengameart.org/content/minimalist-pixel-fonts)' by kheftel

The original game Tetris: Alexey Pajitnov  
All rights to the franchise Tetris goes to The Tetris Company. 
This is just a fan-remake and is in no way associated with the company.
