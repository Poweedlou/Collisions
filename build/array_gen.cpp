#include <random>
#include <ctime>
#include "Types.h"
#include <cmath>


static double sqr(double x){
return x * x;}


extern "C" void gen_array(position * molecules, dimension dims, unsigned long N, const position * shape, const double radius)
{
    std::mt19937_64 rng(static_cast<uint64_t>(time(0)));
    unsigned long done = 0;
    double sqr_r = 4 * radius * radius;
    std::uniform_real_distribution<position> distr[3];
    std::uniform_real_distribution<position> foo;
    for (dimension i = 0; i < 3; i++)
    {
        std::uniform_real_distribution<position> bar(radius, shape[i] - radius);
        distr[i] = bar;
    }
    double dist;
    dimension ok;
    while (done < N)
    {
        ok = 1;
        for (dimension dim = 0; dim < dims; ++dim)
            molecules[dim * N + done] = distr[dim](rng);
        for (unsigned long i = 0; i < done; i++)
        {
            dist = 0.0;
            for (dimension j = 0; j < dims; j++)
                dist += sqr(molecules[j * N + i] - molecules[j * N + done]);
            if (sqr_r > dist)
            {
                ok = 0;
                break;
            }
        }
        if (ok == 1)
            done++;
    }
}

extern "C" void gen_speed_2d(position * molecules, unsigned long N, const double max_speed)
{
    static double pi = 2 * asin(1.0);
    std::mt19937_64 rng(static_cast<uint64_t>(time(0)));
    std::uniform_real_distribution<position> angle_distr(0.0, 2 * pi);
    std::uniform_real_distribution<position> speed_distr(0.0, max_speed);
    double angle;
    double speed;
    for (unsigned long j = 0; j < N; ++j)
    {
        angle = angle_distr(rng);
        speed = speed_distr(rng);
        molecules[j] = sin(angle) * speed;
        molecules[N + j] = cos(angle) * speed;
    }
        
}

extern "C" void gen_speed_3d(position * molecules, unsigned long N, const double max_speed)
{
    static const double pi = 2 * asin(1.0);
    std::mt19937_64 rng(static_cast<uint64_t>(time(0)));
    std::uniform_real_distribution<position> angle_distr(0.0, 1);
    std::uniform_real_distribution<position> speed_distr(0.0, max_speed);
    double theta, phi, speed, foo;
    for (unsigned long j = 0; j < N; ++j)
    {
        theta = angle_distr(rng) * 2 * pi;
        phi = angle_distr(rng) * 2 - 1;
        foo = sqrt(1 - phi * phi) * speed;
        speed = speed_distr(rng);
        molecules[j] = cos(theta) * foo;
        molecules[N + j] = sin(theta) * foo;
        molecules[2 * N + j] = phi * speed;
    }
}