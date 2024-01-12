//
// Created by thomas on 04/09/23.
//

#pragma once

#include <SDL_image.h>
#include <SDL2/SDL.h>

#include "sprite.h"

/**
 * @brief This class acts as a wrapper for SDL functions.\n
 * They allow us to handle the complete SDL lifecycle, from initialization to cleanup.
 */
class SDLHandler {
    public:
        std::queue<SDL_Event> eventStream;
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::unordered_map<std::string, Sprite*> sprites;

        SDLHandler();
        ~SDLHandler();

        Sprite *getSprite(const std::string& spriteName);

        /**
         * @brief Clean up SDL
         */
        void cleanUp();

        /**
         * @brief Get SDL events from the event stream
         * @return A queue of SDL events
         */
        std::queue<SDL_Event> eventHandler();

        /**
         * @brief Render the current frame
         */
        void render() const;

        /**
         * @brief Clear the screen
         */
        void clear() const;

    private:

        /**
         * @brief Load a texture from a file (png, jpg, bmp, etc.) and save it to the texture map
         * @param textureName the name of the texture
         * @param ext the extension of the texture file
         */
        void loadTexture(const std::string& textureName, const std::string& ext);

        /**
         * @brief generic error logger
         * @tparam Args variadic template for the arguments
         * @param fmt the format string
         * @param args the arguments
         */
        template<typename... Args>
        void logError(const char *fmt, Args... args);
};
