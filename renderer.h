#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "math.h"

class Renderer {
    
public:
    Renderer();
    bool Create();
    void Destroy();
    void ClearScreen();
    void RenderTexture(std::vector<unsigned char> pixels);
    
    SDL_Renderer* GetRenderer() { return mRenderer; }
    SDL_Window* GetWindow() { return mWindow; }
    SDL_Point GetWindowSize() { return mWindowSize; }
    SDL_Texture* GetTexture() { return mTexture; }
    
protected:
    SDL_Point mWindowSize;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;
};
