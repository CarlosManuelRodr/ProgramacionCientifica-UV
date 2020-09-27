/**
* @file Auxiliar.h
* @brief Funciones auxiliares.
* @author Carlos Manuel Rodríguez Martínez
* @date 8/06/2015
*/

#ifndef _AUX
#define _AUX

#include <vector>
#include <numeric>
#include <random>

/**
* @brief Informa si find_val está dentro de v.
* @param v Vector dónde buscar.
* @param find_val Valor a buscar.
*/
template <class N> bool aux_is_in(std::vector<N> &v, const N find_val)
{
    typename std::vector<N>::iterator it = find(v.begin(), v.end(), find_val);
    if (it != v.end())
        return true;
    else
        return false;
}

template <class N> bool aux_is_in_const(std::vector<N> v, const N find_val)
{
    typename std::vector<N>::iterator it = find(v.begin(), v.end(), find_val);
    if (it != v.end())
        return true;
    else
        return false;
}

/**
* @brief Informa si find_val está dentro de lista.
* @param list Lista dónde buscar.
* @param find_val Valor a buscar.
*/
template <class N> bool aux_is_in(std::initializer_list<N> list, const N find_val)
{
    std::vector<N> v = list;
    typename std::vector<N>::iterator it = find(v.begin(), v.end(), find_val);
    if (it != v.end())
        return true;
    else
        return false;
}

/**
* @brief Informa la posición dentro de v donde se encuentra find_val.
* @param v Vector dónde buscar.
* @param find_val Valor a buscar.
* @return La posición ó -1 en caso de que no se encuentre.
*/
template <class N> int aux_find_pos(std::vector<N> &v, const N find_val)
{
    typename std::vector<N>::iterator it = find(v.begin(), v.end(), find_val);
    if (it != v.end())
        return distance(v.begin(), it);
    else
        return -1;
}

template <class N> int aux_find_pos_const(std::vector<N> v, const N find_val)
{
    typename std::vector<N>::iterator it = find(v.begin(), v.end(), find_val);
    if (it != v.end())
        return distance(v.begin(), it);
    else
        return -1;
}

/**
* @brief Devuelve el promedio de los elementos de un vector.
*/
template <class N> double aux_mean(std::vector<N> v)
{
    return (double)std::accumulate(v.begin(), v.end(), 0.0)/(double)v.size();
}

/**
* @brief Convierte cadena de texto a número.
*/
template <class N> N aux_string_to_num(const std::string &s)
{
    std::istringstream i(s);
    N x;
    if (!(i >> x))
        return 0;
    return static_cast<N>(x);
}

/****************************
*                           *
*  Generador de aleatorios  *
*                           *
****************************/

/**
* @enum RandomAlgorithm
* @brief Tipos de generadores de números aleatorios.
*/
enum RandomAlgorithm
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
    static double GetDouble();
};

#endif
