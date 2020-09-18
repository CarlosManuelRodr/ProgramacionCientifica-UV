#pragma once
#include <numeric>
#include <random>

/****************************
*                           *
*  Generador de aleatorios  *
*                           *
****************************/

/**
* @enum RandomAlgorithm
* @brief Tipos de generadores de números aleatorios.
*/
enum class RandomAlgorithm
{
    LCG, MT19937, RANLUX24, RANLUX48
};

/**
* @class RandomGen
* @brief Generador de números aleatorios.
*/
class RandomGen
{
    static RandomAlgorithm m_ra;
    static std::mt19937 mt;
    static std::ranlux24 rl24;
    static std::ranlux48 rl48;
public:
    static void SetAlgorithm(RandomAlgorithm ra);
    static void Seed(int seed = -1);
    static int GetInt(int i);
    static float Getfloat();
};