#include <iostream>
#include "noise.cpp"
#include "vec2.cpp"

int main()
{
    int x, y;
    std::cin >> x >> y;

    Noise noise = Noise{123987, 0.2};

    std::cout << noise.get_value(x, y);

    return 0;
}