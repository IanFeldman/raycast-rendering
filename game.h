#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "math.h"

class Game {

public:
    Game();
    bool Initialize();
    void Shutdown();
    void Runloop();
    void ProcessUpdate();
    void UpdateGame();
    void GenerateOutput();

    class Map* GetMap() {return mMap;}

private:
    bool LoadData();

protected:
    bool mRunning;
    int mPreviousTime;
    
    class Renderer* mRenderer;
    class Player* mPlayer;
    class Map* mMap;
    float mFOV;
    float mDepth;
    float mRayIncrement;
    float mFloorMaxLight;
    int mMapScale;
    Vector3 mMapWallColor;
    Vector3 mMapFloorColor;
    Vector3 mCrosshairColor;
    Vector3 mMapPlayerColor;
    bool mMapShown;
    
    
};
