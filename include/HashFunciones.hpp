#pragma once

/*
Provee las funciones primarias (h1) y secundarias (h2) usadas en el double hashing, y el tamano de la tabla m usado en los experimentos que es de 200003
*/

#include <cmath>
#include <cstddef>
#include <string>

namespace hashfn {

/*Constante A = (sqrt(5) - 1) / 2 del metodo de multiplicacion. */
inline constexpr double A = 0.6180339887498948482;

/* h1(k, m) = k mod m, utilizando el metodo dem la division */
inline int h1(long long k, int m) {
    long long r = k % m;
    if (r < 0) r += m;
    return static_cast<int>(r);
}

/* h2(k, m) utilizando el metodo de multiplicacion, el paso de salto para double hashing con user_id. Retorna al menos 1*/
inline int h2(long long k, int m) {
    double a = static_cast<double>(k) * A;
    a -= std::floor(a);
    int r = static_cast<int>(m * a);
    return r == 0 ? 1 : r;
}

/*Segunda funcion hash para strings, y paso de salto en double hashing */
inline int h2String(const std::string& s, int m) {
    std::size_t hash = 0;
    for (unsigned char c : s) {
        hash = hash * 31 + c;
    }
    int r = static_cast<int>(hash % static_cast<std::size_t>(m));
    return r == 0 ? 1 : r;
}

/* Hash con djb2 (base 33, semilla 5381) para user_screen_name */
inline std::size_t hashString(const std::string& s) {
    std::size_t hash = 5381;
    for (unsigned char c : s) {
        hash = hash * 33 + c;
    }
    return hash;
}

/*Indice de celda para las claves user_id */
inline int indiceUserId(long long k, int m) {
    return h1(k, m);
}

/*Indice de la celda para claves user_screen_name */
inline int indiceScreenName(const std::string& s, int m) {
    return static_cast<int>(hashString(s) % static_cast<std::size_t>(m));
}

} 
