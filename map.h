#pragma once
#include "math.h"

class Map {

public:
    Map() {}
    void SetSize(Vector2 size) {mSize = size;}
    void SetLayout(std::string layout) {mLayout = layout;}
    Vector2 GetSize() {return mSize; }
    std::string GetLayout() {return mLayout;}

protected:
    Vector2 mSize;
    std::string mLayout;
};
