#include <vector>
#include <cstring>
#include "game.h"
#include "renderer.h"
#include "player.h"
#include "map.h"
#include "math.h"

Game::Game()
    :mRunning(true)
    ,mFOV(Math::Pi/4.0f)
    ,mDepth(8.0f)
    ,mRayIncrement(0.05f)
    ,mFloorMaxLight(200.0f)
    ,mMapShown(false)
    ,mMapScale(5)
    ,mMapWallColor(150.0f, 150.0f, 150.0f)
    ,mMapFloorColor(0.0f, 0.0f, 0.0f)
    ,mCrosshairColor(255.0f, 0.0f, 0.0f)
    ,mMapPlayerColor(150.0f, 0.0f, 0.0f)
{
}

bool Game::Initialize() {
    if (!LoadData())
        return false;
    mPreviousTime = SDL_GetTicks();
    return true;
};

bool Game::LoadData() {
    // create renderer
    mRenderer = new Renderer();
    if (!mRenderer->Create())
        return false;

    // player
    mPlayer = new Player(this, Vector2(8,8));

    // map
    mMap = new Map();
    Vector2 size = Vector2(16, 16);
    mMap->SetSize(size);
    std::string layout;
    layout += "################";
    layout += "#......##......#";
    layout += "#......##......#";
    layout += "#..............#";
    layout += "##.##########.##";
    layout += "#..............#";
    layout += "#......##......#";
    layout += "#..............#";
    layout += "###..##..##..###";
    layout += "#.....#..#.....#";
    layout += "#.....#..#.....#";
    layout += "#.....#..#.....#";
    layout += "#######..#######";
    layout += "#..............#";
    layout += "#..............#";
    layout += "#######..#######";
    mMap->SetLayout(layout);
    
    return true;
}

void Game::Runloop() {
    while (mRunning) {
        ProcessUpdate();
        UpdateGame();
        GenerateOutput();
    }
};

void Game::ProcessUpdate() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:  mRunning = false;        break;
                    case SDLK_m:       mMapShown = !mMapShown;  break;
                    default:                                    break;
                }
                break;
            default:
                break;
        }
    }
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    mPlayer->ProcessInput(keyboardState);
}

void Game::UpdateGame() {
    while ((int)SDL_GetTicks() < mPreviousTime + 16) {}
    int currentTime = SDL_GetTicks();
    float deltaTime = (float)(currentTime - mPreviousTime) / 1000;
    mPreviousTime = currentTime;

    // set window title to fps
    int FPS = (int)(1.0f / deltaTime);
    std::string stringFPS = std::to_string(FPS) + " fps";
    const char* charFPS = stringFPS.c_str();
    SDL_SetWindowTitle(mRenderer->GetWindow(), charFPS);
}

void Game::GenerateOutput() {
    mRenderer->ClearScreen();
    
    float width = mRenderer->GetWindowSize().x;
    float height = mRenderer->GetWindowSize().y;
    
    float mapWidth = mMap->GetSize().x;
    float mapHeight = mMap->GetSize().y;

    // pixel data
    std::vector<unsigned char> pixels(width * height * 4, 0); // rgba for every pixel

    // ray casting
    for (int x = 0; x < width; x++)
    {
        // get every angle in fov
        // player angle - half fov + some% of total fov
        // sweeping from left to right
        float rayAngle = (mPlayer->GetAngle() - mFOV / 2.0f) + ((float)x / (float)width) * mFOV;

        float distanceToWall = 0;
        bool hitWall = false;
        while (!hitWall && distanceToWall < mDepth)
        {
            // step ray forward
            distanceToWall += mRayIncrement;

            // position of test point
            int testX = (int)(distanceToWall * Math::Cos(rayAngle) + mPlayer->GetPosition().x);
            int testY = (int)(distanceToWall * Math::Sin(rayAngle) + mPlayer->GetPosition().y);

            // test if ray is out of bounds
            if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight)
            {
                hitWall = true;
                distanceToWall = mDepth; // so it will be dark
            }
            else
            {
                // ray is in bounds so check if ray cell is wall block
                if (mMap->GetLayout()[testY * mapWidth + testX] == '#')
                    hitWall = true;
            }
        }

        // calculate distance to ceiling and floor
        // the greater the dist to wall, the smaller number we subtract from midpoint of screen:
        // the smaller the wall appears
        float ceiling = (float)height / 2.0f - (float)height / distanceToWall;
        float floor = (float)height - ceiling;

        for (int y = 0; y < height; y++)
        {
            // walls
            if (y > ceiling && y < floor) // 0,0 is top left
            {
                float lightLevel = (1 - distanceToWall / mDepth) * 255.0f;
                if (lightLevel > 255.0f)
                    lightLevel = 255.0f;
                else if (lightLevel < 0.0f )
                    lightLevel = 0.0f;

                int pixelPos = (y * width * 4) + x * 4;
                pixels[pixelPos + 0] = lightLevel;          // b
                pixels[pixelPos + 1] = lightLevel;          // g
                pixels[pixelPos + 2] = lightLevel;          // r
                pixels[pixelPos + 3] = SDL_ALPHA_OPAQUE;    // a
            }
            // floor
            else if (y > ceiling && y > floor)
            {
                // dist to midscreen / mid screen
                // closer point is to midscreen, the darker it is
                float lightLevel = ((float)y - (float)height / 2.0f) / ((float)height / 2.0f) * mFloorMaxLight;
                if (lightLevel > mFloorMaxLight)
                    lightLevel = mFloorMaxLight;
                else if (lightLevel < 0.0f)
                    lightLevel = 0.0f;

                int pixelPos = (y * width * 4) + x * 4;
                pixels[pixelPos + 0] = lightLevel;          // b
                pixels[pixelPos + 1] = lightLevel;          // g
                pixels[pixelPos + 2] = lightLevel;          // r
                pixels[pixelPos + 3] = SDL_ALPHA_OPAQUE;    // a
            }
        }
    }

    // render crosshair on top
    for (int chWidth = -2; chWidth < 2; chWidth++)
    {
        for (int chHeight = -2; chHeight < 2; chHeight++)
        {
            int chPixel = ((height / 2 + chHeight) * width * 4) + (width / 2 + chWidth) * 4;
            pixels[chPixel + 0] = mCrosshairColor.z;        // b
            pixels[chPixel + 1] = mCrosshairColor.y;        // g
            pixels[chPixel + 2] = mCrosshairColor.x;        // r
            pixels[chPixel + 3] = SDL_ALPHA_OPAQUE;         // a
        }
    }

    // render minimap
    if (mMapShown)
    {
        for (int mapX = 0; mapX < mapWidth; mapX++)
        {
            for (int mapY = 0; mapY < mapHeight; mapY++)
            {
                // walls
                if (mMap->GetLayout()[mapY * mapWidth + mapX] == '#')
                {
                    // mMapScale x mMapScale units
                    for (int mapPixelX = 0; mapPixelX < mMapScale; mapPixelX++)
                    {
                        for (int mapPixelY = 0; mapPixelY < mMapScale; mapPixelY++)
                        {
                            int pixelPosX = mapX * mMapScale + mapPixelX;
                            int pixelPosY = mapY * mMapScale + mapPixelY;
                            int pixelIndex = (pixelPosY * width * 4) + pixelPosX * 4;
                            pixels[pixelIndex + 0] = mMapWallColor.z;      // b
                            pixels[pixelIndex + 1] = mMapWallColor.y;      // g
                            pixels[pixelIndex + 2] = mMapWallColor.x;      // r
                            pixels[pixelIndex + 3] = SDL_ALPHA_OPAQUE;     // a
                        }
                    }
                }
                // floor
                else
                {
                    for (int mapPixelX = 0; mapPixelX < mMapScale; mapPixelX++)
                    {
                        for (int mapPixelY = 0; mapPixelY < mMapScale; mapPixelY++)
                        {
                            int pixelPosX = mapX * mMapScale + mapPixelX;
                            int pixelPosY = mapY * mMapScale + mapPixelY;
                            int pixelIndex = (pixelPosY * width * 4) + pixelPosX * 4;
                            pixels[pixelIndex + 0] = mMapFloorColor.z;   // b
                            pixels[pixelIndex + 1] = mMapFloorColor.y;   // g
                            pixels[pixelIndex + 2] = mMapFloorColor.x;   // r
                            pixels[pixelIndex + 3] = SDL_ALPHA_OPAQUE;   // a
                        }
                    }
                }
            }
        }
        // player on map
        int mapPlayerX = (int)mPlayer->GetPosition().x;
        int mapPlayerY = (int)mPlayer->GetPosition().y;
        for (int mapPixelX = 0; mapPixelX < mMapScale; mapPixelX++)
        {
            for (int mapPixelY = 0; mapPixelY < mMapScale; mapPixelY++)
            {
                int pixelPosX = mapPlayerX * mMapScale + mapPixelX;
                int pixelPosY = mapPlayerY * mMapScale + mapPixelY;
                int pixelIndex = (pixelPosY * width * 4) + pixelPosX * 4;
                pixels[pixelIndex + 0] = mMapPlayerColor.z;    // b
                pixels[pixelIndex + 1] = mMapPlayerColor.y;    // g
                pixels[pixelIndex + 2] = mMapPlayerColor.x;    // r
                pixels[pixelIndex + 3] = SDL_ALPHA_OPAQUE;     // a
            }
        }
    }
    
    // render to screen
    mRenderer->RenderTexture(pixels);
}

void Game::Shutdown() {
    mRenderer->Destroy();
    SDL_Quit();
};
