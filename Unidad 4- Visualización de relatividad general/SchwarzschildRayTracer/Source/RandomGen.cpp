#include "RandomGen.h"
#include <chrono>

/****************************
*                           *
*  Generador de aleatorios  *
*                           *
****************************/

RandomAlgorithm RandomGen::m_ra = RandomAlgorithm::MT19937;
std::mt19937 RandomGen::mt;
std::ranlux24 RandomGen::rl24;
std::ranlux48 RandomGen::rl48;

void RandomGen::SetAlgorithm(RandomAlgorithm ra)
{
    m_ra = ra;
}
void RandomGen::Seed(int seed)
{
    if (seed == -1)
        seed = static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());

    switch (m_ra)
    {
    case RandomAlgorithm::MT19937:
        mt.seed(seed);
        break;
    case RandomAlgorithm::LCG:
        srand(seed);
        break;
    case RandomAlgorithm::RANLUX24:
        rl24.seed(seed);
        break;
    case RandomAlgorithm::RANLUX48:
        rl48.seed(seed);
        break;
    };
}
int RandomGen::GetInt(int i)
{
    switch (m_ra)
    {
    case RandomAlgorithm::MT19937:
        return mt() % i;
        break;
    case RandomAlgorithm::LCG:
        return rand() % i;
        break;
    case RandomAlgorithm::RANLUX24:
        return rl24() % i;
        break;
    case RandomAlgorithm::RANLUX48:
        return rl48() % i;
        break;
    };
    return 0;
}
float RandomGen::Getfloat()
{
    switch (m_ra)
    {
    case RandomAlgorithm::MT19937:
        return (float)mt() / (float)mt.max();
        break;
    case RandomAlgorithm::LCG:
        return (float)rand() / (float)RAND_MAX;
        break;
    case RandomAlgorithm::RANLUX24:
        return (float)rl24() / (float)rl24.max();
        break;
    case RandomAlgorithm::RANLUX48:
        return (float)rl48() / (float)rl48.max();
        break;
    };
    return 0.0;
}
