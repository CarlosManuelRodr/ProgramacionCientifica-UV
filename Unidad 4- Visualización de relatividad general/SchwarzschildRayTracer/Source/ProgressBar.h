#pragma once
#include <iostream>

/**
* @brief Muestra barra de progreso.
* @param progress Valor fraccional del progreso.
*/
void progress_bar(float progress)
{
    const int barWidth = 35;
    std::cout << "[";
    int pos = (int)(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
    if (progress == 1.0)
        std::cout << std::endl;
}

/**
* @brief Muestra barra de progreso.
* @param val Valor en intervalo [0, max].
* @param max Máximo valor.
* @param dt Intervalo de valores.
*/
void progress_bar(float val, float min, float max, float dt)
{
    double progress;
    if (val + dt > max)
        progress = 1.0;
    else
        progress = (val - min) / (max - min);

    const int barWidth = 35;
    std::cout << "[";
    int pos = (int)(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
    if (progress == 1.0)
        std::cout << std::endl;
}