#pragma once

/* Orquestacion de mediciones sobre las 5 estructuras de tablas hash.
 * Expone: los enums Estructura y ClaveTipo, los puntos de muestreo
 * (puntosMedicion) y la funcion ejecutarUnaMedicion, que crea la
 * estructura correspondiente, inserta la muestra de tweets, y mide
 * tiempo de creacion y memoria ocupada.
 */
#include "HashTablaAbierto.hpp"
#include "HashTablaCerrado.hpp"
#include "HashTablaUnorderedMap.hpp"
#include "LectorTweets.hpp"

#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace experimento {

constexpr int TAMANO_TABLA = 200003;

enum class Estructura {
    ABIERTO,
    CERRADO_LINEAR,
    CERRADO_QUADRATIC,
    CERRADO_DOUBLE,
    UNORDERED_MAP,
};

enum class ClaveTipo { USER_ID, SCREEN_NAME };

struct ResultadoMedicion {
    double tiempoSegundos;
    std::size_t memoriaBytes;
};

inline const char* nombreEstructura(Estructura estructura) {
    switch (estructura) {
        case Estructura::ABIERTO:
            return "abierto";
        case Estructura::CERRADO_LINEAR:
            return "cerrado_linear";
        case Estructura::CERRADO_QUADRATIC:
            return "cerrado_quadratic";
        case Estructura::CERRADO_DOUBLE:
            return "cerrado_double";
        case Estructura::UNORDERED_MAP:
            return "unordered_map";
    }
    return "desconocido";
}

inline const char* nombreClave(ClaveTipo clave) {
    switch (clave) {
        case ClaveTipo::USER_ID:
            return "user_id";
        case ClaveTipo::SCREEN_NAME:
            return "user_screen_name";
    }
    return "desconocido";
}

/* Se generan los tamanos de muestra a medir: 10000, 20000, ..., hasta totalTweets */
inline std::vector<int> puntosMedicion(int totalTweets) {
    std::vector<int> puntos;
    for (int n = 10000; n <= totalTweets; n += 10000) {
        puntos.push_back(n);
    }
    if (puntos.empty() || puntos.back() != totalTweets) {
        puntos.push_back(totalTweets);
    }
    return puntos;
}

namespace detail {

template <typename Tabla>
ResultadoMedicion contarUserId(const std::vector<Tweet>& muestra) {
    Tabla tabla(TAMANO_TABLA);

    const auto inicio = std::chrono::high_resolution_clock::now();
    for (const auto& tweet : muestra) {
        tabla.incrementar(tweet.user_id);
    }
    const auto fin = std::chrono::high_resolution_clock::now();

    return {std::chrono::duration<double>(fin - inicio).count(), tabla.memoriaBytes()};
}

template <typename Tabla>
ResultadoMedicion contarScreenName(const std::vector<Tweet>& muestra) {
    Tabla tabla(TAMANO_TABLA);

    const auto inicio = std::chrono::high_resolution_clock::now();
    for (const auto& tweet : muestra) {
        tabla.incrementar(tweet.user_screen_name);
    }
    const auto fin = std::chrono::high_resolution_clock::now();

    return {std::chrono::duration<double>(fin - inicio).count(), tabla.memoriaBytes()};
}

template <typename Tabla>
ResultadoMedicion contarUserIdCerrada(const std::vector<Tweet>& muestra,
                                      ProbingStrategy estrategia) {
    Tabla tabla(TAMANO_TABLA, estrategia);

    const auto inicio = std::chrono::high_resolution_clock::now();
    for (const auto& tweet : muestra) {
        tabla.incrementar(tweet.user_id);
    }
    const auto fin = std::chrono::high_resolution_clock::now();

    return {std::chrono::duration<double>(fin - inicio).count(), tabla.memoriaBytes()};
}

template <typename Tabla>
ResultadoMedicion contarScreenNameCerrada(const std::vector<Tweet>& muestra,
                                          ProbingStrategy estrategia) {
    Tabla tabla(TAMANO_TABLA, estrategia);

    const auto inicio = std::chrono::high_resolution_clock::now();
    for (const auto& tweet : muestra) {
        tabla.incrementar(tweet.user_screen_name);
    }
    const auto fin = std::chrono::high_resolution_clock::now();

    return {std::chrono::duration<double>(fin - inicio).count(), tabla.memoriaBytes()};
}

}  

inline ResultadoMedicion ejecutarUnaMedicion(Estructura estructura,
                                            ClaveTipo clave,
                                            const std::vector<Tweet>& muestra) {
    switch (estructura) {
        case Estructura::ABIERTO:
            if (clave == ClaveTipo::USER_ID) {
                return detail::contarUserId<HashTablaAbierto<long long>>(muestra);
            }
            return detail::contarScreenName<HashTablaAbierto<std::string>>(muestra);

        case Estructura::CERRADO_LINEAR:
            if (clave == ClaveTipo::USER_ID) {
                return detail::contarUserIdCerrada<HashTablaCerrado<long long>>(
                    muestra, ProbingStrategy::LINEAR);
            }
            return detail::contarScreenNameCerrada<HashTablaCerrado<std::string>>(
                muestra, ProbingStrategy::LINEAR);

        case Estructura::CERRADO_QUADRATIC:
            if (clave == ClaveTipo::USER_ID) {
                return detail::contarUserIdCerrada<HashTablaCerrado<long long>>(
                    muestra, ProbingStrategy::QUADRATIC);
            }
            return detail::contarScreenNameCerrada<HashTablaCerrado<std::string>>(
                muestra, ProbingStrategy::QUADRATIC);

        case Estructura::CERRADO_DOUBLE:
            if (clave == ClaveTipo::USER_ID) {
                return detail::contarUserIdCerrada<HashTablaCerrado<long long>>(
                    muestra, ProbingStrategy::DOUBLE);
            }
            return detail::contarScreenNameCerrada<HashTablaCerrado<std::string>>(
                muestra, ProbingStrategy::DOUBLE);

        case Estructura::UNORDERED_MAP:
            if (clave == ClaveTipo::USER_ID) {
                return detail::contarUserId<HashTablaUnorderedMap<long long>>(muestra);
            }
            return detail::contarScreenName<HashTablaUnorderedMap<std::string>>(muestra);
    }

    throw std::runtime_error("Estructura no soportada");
}

}  