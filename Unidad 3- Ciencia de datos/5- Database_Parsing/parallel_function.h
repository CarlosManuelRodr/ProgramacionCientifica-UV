#pragma once
#include <vector>
#include "ThreadPool.h"
#include "progress_bar.h"

/**
* @brief Lanza threads con argumento en el rango especificado.
* @param f Función a lanzar.
* @param arg Vector de argumentos a evaluar.
*/
template<typename R, typename T>
std::vector<R> parallel_function(std::function<R(T)> f, std::vector<T> arg, bool showProgress = true)
{
    unsigned int hc = std::thread::hardware_concurrency();

    ThreadPool pool(hc);
    std::vector<std::future<R>> result;
    std::vector<R> result_values;

    for (unsigned i = 0; i < arg.size(); i++)
        result.push_back(pool.enqueue(f, arg[i]));

    for (auto& r : result)
    {
        if (showProgress)
            progress_bar((double)(&r - &result[0]), 0.0, (double) result.size(), 1.0);

        result_values.push_back(r.get());
    }

    if (showProgress)
        progress_bar(1.0);

    return result_values;
}
