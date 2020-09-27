#pragma once

/**
* @brief Muestra barra de progreso.
* @param progress Valor fraccional del progreso.
*/
void progress_bar(double progress);

/**
* @brief Borra barra de progreso.
*/
void progress_bar_clear();

/**
* @brief Muestra barra de progreso.
* @param val Valor en intervalo [0, max].
* @param max Máximo valor.
* @param dt Intervalo de valores.
*/
void progress_bar(double val, double min, double max, double dt);