#include "progress_bar.h"
#include <iostream>
using namespace std;

void progress_bar(double progress)
{
    const int barWidth = 35;
    cout << "[";
    int pos = (int)(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
    if (progress == 1.0)
        progress_bar_clear();
}

void progress_bar_clear()
{
    const int barWidth = 50;
    cout << "\r" << flush;
    for (int i = 0; i < barWidth; ++i)
        cout << " ";
    cout << "\r" << flush;
}

void progress_bar(double val, double min, double max, double dt)
{
    double progress;
    if (val + dt > max)
        progress = 1.0;
    else
        progress = (val - min) / (max - min);

    const int barWidth = 35;
    cout << "[";
    int pos = (int)(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
    if (progress == 1.0)
        progress_bar_clear();
}