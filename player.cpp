#include <SDL2/SDL.h>
#include "player.h"
#include "map.h"
#include "math.h"

Player::Player(Game* game, Vector2 initPosition)
    :mGame(game)
    ,mPosition(initPosition)
    ,mAngle(0.0f)
    ,mSpeed(0.05f)
    ,mRotateSpeed(0.03f)
{
}

void Player::ProcessInput(const Uint8* keyboardState)
{
    bool _left = keyboardState[SDL_SCANCODE_A];
    bool _right = keyboardState[SDL_SCANCODE_D];
    bool _forward = keyboardState[SDL_SCANCODE_W];
    bool _back = keyboardState[SDL_SCANCODE_S];
    bool _leftRight = !(_left && _right) && !(!_left && !_right);
    bool _forwardBack = !(_forward && _back) && !(!_forward && !_back);
    if (_leftRight || _forwardBack) {
        if (_left) {
           mAngle -= mRotateSpeed;
        }
        if (_right) {
            mAngle += mRotateSpeed;
        }
        if (_forward) {
            float newX = mPosition.x + Math::Cos(mAngle) * mSpeed;
            float newY = mPosition.y + Math::Sin(mAngle) * mSpeed;

            if (mGame->GetMap()->GetLayout()[(int)newY * mGame->GetMap()->GetSize().x + (int)newX] != '#')
                mPosition = Vector2(newX, newY);
        }
        if (_back) {
            float newX = mPosition.x - Math::Cos(mAngle) * mSpeed;
            float newY = mPosition.y - Math::Sin(mAngle) * mSpeed;

            if (mGame->GetMap()->GetLayout()[(int)newY * mGame->GetMap()->GetSize().x + (int)newX] != '#')
                mPosition = Vector2(newX, newY);
        }
    }
}
