//
// Created by thomas on 04/09/23.
//

#include "SDLHandler.h"


SDLHandler::SDLHandler() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ||
        !(window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN)) ||
        !(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) {

        this->logError("SDL_Init Error", SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

SDLHandler::~SDLHandler() {
    this->cleanUp();
}

void SDLHandler::cleanUp() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}


std::queue<SDL_Event> SDLHandler::eventHandler() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        eventStream.push(event);
    }
    return eventStream;
}

void SDLHandler::clear() const {
    SDL_RenderClear(renderer);
}

void SDLHandler::render() const {
    SDL_RenderPresent(renderer);
}

Sprite *SDLHandler::getSprite(const std::string &spriteBaseName) {
    std::string ext = spriteBaseName.substr(spriteBaseName.size() - 4, spriteBaseName.size());
    std::string spriteName = spriteBaseName.substr(0, spriteBaseName.size() - 4);
    if (sprites.find(spriteName) == sprites.end()) {
        this->loadTexture(spriteName, ext);
    }
    return sprites[spriteName];
}

void SDLHandler::loadTexture(const std::string& textureName, const std::string& ext) {
    std::string texturePath = "resources/assets/" + textureName + ext;
    SDL_Texture *texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (!texture) {
        this->logError("IMG_LoadTexture Error", SDL_GetError());
    }
    auto sprite = new Sprite();
    sprite->texture = texture;
    SDL_QueryTexture(
            texture, nullptr, nullptr,
            reinterpret_cast<int *>(&sprite->width),
            reinterpret_cast<int *>(&sprite->height)
    );

    sprites[textureName] = sprite;
}

template<typename... Args>
void SDLHandler::logError(const char *fmt, Args... args) {
    SDL_Log(fmt, args...);
    this->cleanUp();
    exit(1);
}