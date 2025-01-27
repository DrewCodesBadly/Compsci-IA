#include "../vec2.h"
#include "../vec2.cpp"
#include "../terrain_rng.h"
#include "../terrain_rng.cpp"
#include <vector>
#include <cmath>
#include <iostream>

#define SIZE_X 16
#define SIZE_Y 16
#define PI 3.14159265

using namespace std;
void insert_object(vector<vector<Vec2>> &grid, Vec2 p, double cellsize, double obj_size)
{
    const int pos_x{(int)(p.x / cellsize)};
    const int pos_y{(int)(p.y / cellsize)};
    grid[pos_x][pos_y] = p;
}

void object_scatter(double r, TerrainRNG main_rng, int k)
{
    const double cell_size{r / sqrt(2)};
    const int cells_x{(int)ceil(SIZE_X / cell_size)};
    const int cells_y{(int)ceil(SIZE_Y / cell_size)};

    // might be unneeded, we'll see how output works best
    // initialize to -1, 0 vectors
    vector<vector<Vec2>> grid;
    for (int x{0}; x < cells_x; ++x)
    {
        vector<Vec2> v;
        for (int y{0}; y < cells_y; ++y)
        {
            v.push_back(Vec2{-1.0, 0.0});
        }
        grid.push_back(v);
    }

    // Initial point
    vector<Vec2> points;
    vector<Vec2> active;
    Vec2 p0{(double)(main_rng.next() % SIZE_X), (double)(main_rng.next() % SIZE_Y)};
    insert_object(grid, p0, cell_size, r);
    points.push_back(p0);
    active.push_back(p0);

    bool success;
    while (active.size() > 0)
    {
        // Pick a random active point
        unsigned long long idx{main_rng.next() % active.size()};
        Vec2 point = active[idx];

        // Try up to k times to find a new point
        success = false;
        for (int i{0}; i < k; ++i)
        {
            // Pick point random angle away between r and 2r distance
            // Uses a random angle + distance and basic trig
            double angle{(double)main_rng.next() / UINT_MAX * 2 * PI};
            double dist{(double)main_rng.next() / UINT_MAX * r + r};
            Vec2 new_point{point.x + (dist * cos(angle)), point.y + (dist * sin(angle))};

            // Check neighboring cells to determine if this point is valid
            // Code will continue as soon as it determines the point is invalid, otherwise set success to true
            // Check if point is inbounds
            if (new_point.x < 0 || new_point.y < 0 || new_point.x >= SIZE_X || new_point.y >= SIZE_Y)
                continue;

            // Check 8 neighbor cells
            bool valid{true};
            int x{(int)(new_point.x / cell_size)}, y{(int)(new_point.y / cell_size)};
            int x_min{std::max(x - 1, 0)}, x_max{std::min(x + 1, cells_x - 1)};
            int y_min{std::max(y - 1, 0)}, y_max{std::min(y + 1, cells_y - 1)};
            for (int x{x_min}; x < x_max; ++x)
            {
                for (int y{y_min}; y < y_max; ++y)
                {
                    if (grid[x][y].x != -1.0 && new_point.dist(grid[x][y]) < r)
                        valid = false;
                }
            }
            if (!valid)
                continue;

            // Nothing went wrong so we add the point and break out of the for loop
            points.push_back(new_point);
            active.push_back(new_point);
            insert_object(grid, new_point, cell_size, r);
            success = true;
            break;
        }

        // Add new point or remove p from active
        if (!success)
        {
            active.erase(active.begin() + idx);
        }
    }

    // debug
    cout << "cellsize: " << cell_size << '\n';
    for (vector<Vec2> v : grid)
    {
        for (Vec2 v2 : v)
        {
            cout << '(' << v2.x << ", " << v2.y << "), ";
        }
        cout << '\n';
    }
}

int main()
{
    object_scatter(5.0, TerrainRNG(3U), 30);
    cout << "done!";
    return 0;
}