#pragma once
#include <map>
#include <string>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

enum class Sound{
    CLEAR_ROW,
    DROP,
    GAME_OVER,
};

enum class FontSize{
    X_SMALL,
    SMALL,
    MEDIUM,
    LARGE,
    X_LARGE,
    _LAST_INDEX,
};

enum class Texture{
    BACKGROUND,
    LOGO,
    BLOCK_I,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_P,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z,
};


class ResourceManager{
public:
    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    void playSound(Sound sound);
    void drawText(int x, int y, const std::string& text, FontSize size, SDL_Color color, bool aroundCenter = false);
    void drawImage(int x, int y, Texture texture, bool aroundCenter = false);
    void drawImage(int x, int y, int w, int h, Texture texture, bool aroundCenter = false);


    bool isInitialized() const{ return initSuccess;}

private:
    bool initSuccess = false;

    std::map<Sound, std::string> soundEffectLocations = {
            {Sound::CLEAR_ROW, "res/snd/clear_row.wav"},
            {Sound::DROP, "res/snd/drop.wav"},
            {Sound::GAME_OVER, "res/snd/game_over.wav"},
    };

    std::map<Sound, Mix_Chunk*> soundEffects;

    std::map<Texture, std::string> textureLocations = {
            {Texture::BACKGROUND, "res/img/background.png"},
            {Texture::LOGO, "res/img/logo_outline.png"},
            {Texture::BLOCK_I, "res/img/block_I.png"},
            {Texture::BLOCK_J, "res/img/block_J.png"},
            {Texture::BLOCK_L, "res/img/block_L.png"},
            {Texture::BLOCK_O, "res/img/block_O.png"},
            {Texture::BLOCK_P, "res/img/block_P.png"},
            {Texture::BLOCK_S, "res/img/block_S.png"},
            {Texture::BLOCK_T, "res/img/block_T.png"},
            {Texture::BLOCK_Z, "res/img/block_Z.png"},
    };

    Mix_Music* backgroundMusic;

    std::map<FontSize, TTF_Font*> fonts;
    SDL_Renderer* renderer;
};