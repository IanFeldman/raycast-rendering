#pragma once
#include "math.h"
#include <SDL2/SDL_stdinc.h>
#include "game.h"

class Player
{
public:
    Player(class Game* game, Vector2 initPosition);
    Vector2 GetPosition() {return mPosition;}
    void SetPosition(Vector2 position) {mPosition = position;}
    float GetAngle() {return mAngle;}
    void SetAngle(float angle) {mAngle = angle;}
    void ProcessInput(const Uint8* keyboardState);

protected:
    class Game* mGame;
    Vector2 mPosition;
    float mAngle;
    float mSpeed;
    float mRotateSpeed;
};
