#include "Auxiliar.h"

#include <chrono>
#include <random>
#include <algorithm>
using namespace std;


/****************************
*                           *
*  Generador de aleatorios  *
*                           *
****************************/

RandomAlgorithm RandomGen::m_ra = MT19937;
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
    case MT19937:
        mt.seed(seed);
        break;
    case LCG:
        srand(seed);
        break;
    case RANLUX24:
        rl24.seed(seed);
        break;
    case RANLUX48:
        rl48.seed(seed);
        break;
    };
}
int RandomGen::GetInt(int i)
{
    switch (m_ra)
    {
    case MT19937:
        return mt() % i;
        break;
    case LCG:
        return rand() % i;
        break;
    case RANLUX24:
        return rl24() % i;
        break;
    case RANLUX48:
        return rl48() % i;
        break;
    };
    return 0;
}
double RandomGen::GetDouble()
{
    switch (m_ra)
    {
    case MT19937:
        return (double)mt()/(double)mt.max();
        break;
    case LCG:
        return (double)rand()/(double)RAND_MAX;
        break;
    case RANLUX24:
        return (double)rl24()/(double)rl24.max();
        break;
    case RANLUX48:
        return (double)rl48()/(double)rl48.max();
        break;
    };
    return 0.0;
}
