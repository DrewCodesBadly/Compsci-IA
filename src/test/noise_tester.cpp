#include "../vec2.cpp"
#include "../noise.cpp"
#include <iostream>

int main()
{
    Noise noise = Noise(7, 0.5);
    double max{-500};
    double min{500};
    for (double x{0}; x < 500; x += 0.1)
    {
        for (double y{0}; y < 500; y += 0.1)
        {
            double val = noise.get_value(x, y);
            if (val > max)
                max = val;
            if (val < min)
                min = val;
        }
    }

    std::cout << "min: " << min << "\nmax: " << max;

    return 0;
}