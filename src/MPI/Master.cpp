#include "Master.h"
#include "Fractal.h"

namespace MPI
{
    int Master::Run()
    {
        if (stations)
        {

        }
        else
        {
            Fractal::Mandelbrot(2000, { 0, 2000, 0, 2000 }, { -2.0, 1.0, -1.5, 1.5 }, 2.0, 550).Save("bin/mandelbrot");
        }
        return 0;
    }
}