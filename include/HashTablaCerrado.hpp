#pragma once

/**
 *Tabla hash con direccionamiento abierto (hashing cerrado).
 * Los elementos se almacenan directamente en un arreglo de tamano fijo m, y ante colisiones, se aplica una de tres estrategias de salto:
 * LINEAR:   (h1 + i) mod m
 * QUADRATIC: (h1 + desplazamiento(i)) mod m, con desplazamiento alternante +1^2,-1^2,+2^2,-2^2,...
 * DOUBLE:   (h1 + i * h2) mod m
 * y cada entrada tiene un estado VACIO, OCUPADO o BORRADO para permitir así reutilizar los slots eliminados sin romper cadenas de salto
 */

#include "HashFunciones.hpp"

#include <cstddef>
#include <string>
#include <vector>

/*Estrategia de resolucion de colisiones por salto*/
enum class ProbingStrategy { LINEAR, QUADRATIC, DOUBLE };

// Tabla hash con direccionamiento abierto (hashing cerrado).
template <typename Clave>
class HashTablaCerrado;

/*Para las claves user_id (como long long). */
template <>
class HashTablaCerrado<long long> {
public:
    /*Crea la tabla con tamanoTabla celdas (m) y la estrategia de sondeo indicada*/
    HashTablaCerrado(int tamanoTabla, ProbingStrategy estrategia);
    /*Incrementa el conteo de tweets para la clave dada*/
    void incrementar(long long clave);
    /*Retorna el conteo asociado, o 0 si la clave no existe*/
    int obtener(long long clave) const;
    /*Vacia la tabla y reinicia el contador de usuarios*/
    void limpiar();
    /*Cantidad de claves distintas almacenadas*/
    std::size_t numUsuarios() const;
    std::size_t memoriaBytes() const;

private:
    enum class Estado { VACIO, OCUPADO, BORRADO };

    struct Entry {
        long long clave = 0;
        int conteo = 0;
        Estado estado = Estado::VACIO;
    };

    int tamano_;
    ProbingStrategy estrategia_;
    std::size_t numUsuarios_;
    std::vector<Entry> tabla_;

    int indiceInicial(long long clave) const;
    int pasoSondeo(long long clave) const;
    int sondeo(long long clave, int intento) const;
    int buscarSlot(long long clave);
};

/*Para las claves user_screen_name (como std::string) */
template <>
class HashTablaCerrado<std::string> {
public:
    /*Crea la tabla con tamanoTabla celdas (m) y la estrategia de sondeo indicada*/
    HashTablaCerrado(int tamanoTabla, ProbingStrategy estrategia);
    /*Incrementa el conteo de tweets para la clave dada*/
    void incrementar(const std::string& clave);
    /*Retorna el conteo asociado, o 0 si la clave no existe*/
    int obtener(const std::string& clave) const;
    /*Vacia la tabla y reinicia el contador de usuarios*/
    void limpiar();
    /*Cantidad de claves distintas almacenadas*/
    std::size_t numUsuarios() const;
    std::size_t memoriaBytes() const;

private:
    enum class Estado { VACIO, OCUPADO, BORRADO };

    struct Entry {
        std::string clave;
        int conteo = 0;
        Estado estado = Estado::VACIO;
    };

    int tamano_;
    ProbingStrategy estrategia_;
    std::size_t numUsuarios_;
    std::vector<Entry> tabla_;

    int indiceInicial(const std::string& clave) const;
    int pasoSondeo(const std::string& clave) const;
    int sondeo(const std::string& clave, int intento) const;
    int buscarSlot(const std::string& clave);
};
