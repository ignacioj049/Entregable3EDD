#pragma once

/** Utilidades para la medicion de tiempos de ejecucion.
 * se usa std::chrono::high_resolution_clock, para reportar el tiempo promedio y la desviacion estandar en microsegundos.
 */

#include <chrono>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

namespace medicion {

inline double promedio(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

inline double desviacionEstandar(const std::vector<double>& v) {
    if (v.size() < 2) return 0.0;
    const double prom = promedio(v);
    double suma = 0.0;
    for (double x : v) {
        const double d = x - prom;
        suma += d * d;
    }
    return std::sqrt(suma / static_cast<double>(v.size() - 1));
}

/* Ejecuta f repeticiones veces y retorna {promedio_us, desviacion_us} */
template <typename Func>
std::pair<double, double> medirTiempo(Func f, int repeticiones = 20) {
    std::vector<double> tiempos;
    tiempos.reserve(static_cast<std::size_t>(repeticiones));

    for (int i = 0; i < repeticiones; ++i) {
        const auto inicio = std::chrono::high_resolution_clock::now();
        f();
        const auto fin = std::chrono::high_resolution_clock::now();
        const double us = std::chrono::duration<double, std::micro>(fin - inicio).count();
        tiempos.push_back(us);
    }

    return {promedio(tiempos), desviacionEstandar(tiempos)};
}

}  // namespace medicion
