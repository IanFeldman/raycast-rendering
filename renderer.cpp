#include <SDL2/SDL.h>
#include "renderer.h"
#include "math.h"

Renderer::Renderer()
    :mWindowSize((SDL_Point){800, 400})
{
}

bool Renderer::Create() {
    // init video
    int initialized = SDL_Init(SDL_INIT_VIDEO);
    if (initialized != 0)
        return false;
    
    // create window
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowSize.x, mWindowSize.y, SDL_WINDOW_SHOWN);
    if (mWindow == NULL)
        return false;
    
    // create renderer
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == NULL)
        return false;
    
    // create texture
    mTexture = SDL_CreateTexture
        (
         mRenderer,
         SDL_PIXELFORMAT_ARGB8888,
         SDL_TEXTUREACCESS_STREAMING,
         mWindowSize.x, mWindowSize.y
        );
    
    return true;
}

void Renderer::Destroy() {
    // Destory renderer
    SDL_DestroyRenderer(mRenderer);
    // Destroy window
    SDL_DestroyWindow(mWindow);
}

void Renderer::ClearScreen() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mRenderer);
}

void Renderer::RenderTexture(std::vector<unsigned char> pixels) {
    // lock texture
    unsigned char* lockedPixels = nullptr;
    int pitch = 0;
    SDL_LockTexture
        (
            mTexture,
            NULL,
            reinterpret_cast<void**>(&lockedPixels),
            &pitch
        );
    // send pixel data to texture
    std::memcpy(lockedPixels, pixels.data(), pixels.size());
    // unlock texture
    SDL_UnlockTexture(mTexture);

    // render texture to screen
    SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
    SDL_RenderPresent(mRenderer);
}
