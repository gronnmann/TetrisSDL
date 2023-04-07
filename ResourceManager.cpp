#include "ResourceManager.h"
#include <iostream>
#include "SDL_mixer.h"

ResourceManager::ResourceManager(SDL_Renderer* renderer) : renderer(renderer) {

    // Initialize all sounds
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0){
        std::cout << "Failed to init SDL_mixer. Error: " << Mix_GetError() << std::endl;
        return;
    }

    backgroundMusic = Mix_LoadMUS("res/music/Bit Bit Loop.mp3");
    if (backgroundMusic == nullptr){
        std::cout << "Failed to load background music." << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(backgroundMusic, 100);

    std::map<Sound, std::string>::iterator it;
    for (it = soundEffectLocations.begin(); it != soundEffectLocations.end(); it++){
        Mix_Chunk* sndEffect = Mix_LoadWAV(it->second.c_str());
        if (sndEffect == nullptr){
            std::cout << "Failed to load sound effect: " << it->second << " (" << Mix_GetError() << ")" << std::endl;
            return;
        }

        soundEffects.insert({it->first, sndEffect});
    }


    if (TTF_Init() != 0){
        std::cout << "Failed to initialize SDL_TTF: " << TTF_GetError() << std::endl;
    }


    // Fonts follow the pattern 16-32-64-xxx

    std::string font_loc = "res/fonts/Minimal5x7.ttf";

    for (int i = 0; i < static_cast<int>(FontSize::_LAST_INDEX); i++){
        int fontSize = pow(2, (4+i));

        TTF_Font* font = TTF_OpenFont(font_loc.c_str(), fontSize);

        if (!font){
            std::cout << "Failed to load fonts: " << TTF_GetError() << std::endl;
            return;
        }

        fonts.insert(std::pair<FontSize, TTF_Font*>(static_cast<FontSize>(i), font));
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        std::cout << "Failed to init IMG_PNG: " << IMG_GetError() << std::endl;
        return;
    }

    initSuccess = true;

}

ResourceManager::~ResourceManager() {
    Mix_FreeMusic(backgroundMusic);

    std::map<Sound, Mix_Chunk*>::iterator it;
    for (it = soundEffects.begin(); it != soundEffects.end(); it++){
        Mix_FreeChunk(it->second);
    }

    Mix_Quit();
}

void ResourceManager::playSound(Sound sound) {
    if (soundEffects.at(sound) == nullptr) return;

    Mix_PlayChannel(-1, soundEffects.at(sound), 0);
}

void ResourceManager::drawText(int x, int y, const std::string &text, FontSize size, SDL_Color color, bool aroundCenter) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(fonts.at(size), text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect dest;
    if (aroundCenter){
        dest = { x -textSurface->w/2, y - textSurface->h/2, textSurface->w, textSurface->h };
    }else{
        dest = {x, y, textSurface->w, textSurface->h};
    }


    SDL_RenderCopy(renderer, textTexture, NULL, &dest);
    SDL_FreeSurface(textSurface);
}

void ResourceManager::drawImage(int x, int y, Texture texture, bool aroundCenter) {
    SDL_Surface* imageSurface = IMG_Load(textureLocations.at(texture).c_str());
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    SDL_Rect dest;
    if (aroundCenter){
        dest = { x -imageSurface->w/2, y - imageSurface->h/2, imageSurface->w, imageSurface->h };
    }else{
        dest = {x, y, imageSurface->w, imageSurface->h};
    }

    SDL_RenderCopy(renderer, imageTexture, NULL, &dest);
    SDL_FreeSurface(imageSurface);
}

void ResourceManager::drawImage(int x, int y, int w, int h, Texture texture, bool aroundCenter) {
    SDL_Surface* imageSurface = IMG_Load(textureLocations.at(texture).c_str());
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    SDL_Rect dest;
    if (aroundCenter){
        dest = { x -w/2, y - h/2, w, h };
    }else{
        dest = {x, y, w, h};
    }

    SDL_RenderCopy(renderer, imageTexture, NULL, &dest);
    SDL_FreeSurface(imageSurface);
}
