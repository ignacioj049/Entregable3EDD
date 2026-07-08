#pragma once

/*Tabla hash con encadenamiento (hashing abierto).
 * Cada bucket es una std::list de pares (clave, conteo), donde las colisiones
 * se resuelven insertando en la lista del bucket correspondiente
 * se utilizó long long (para user_id) y std::string (para user_screen_name).
*/

#include "HashFunciones.hpp"

#include <cstddef>
#include <list>
#include <vector>

// Tabla hash con encadenamiento (hashing abierto).
template <typename Clave>
class HashTablaAbierto;

/*Para uar claves user_id (como long long) */
template <>
class HashTablaAbierto<long long> {
public:
    struct Entry {
        long long clave;
        int conteo;
        Entry(long long k, int c) : clave(k), conteo(c) {}
    };

    /* Numero de buckets celdas m, el cual en el experimento se usara de 200003. */
    explicit HashTablaAbierto(int tamanoTabla);

    /*Incrementa el conteo de tweets para la clave dada*/
    void incrementar(long long clave);

    /*Retorna el conteo asociado, o 0 si la clave no existe*/
    int obtener(long long clave) const;

    /*Vacia la tabla y reinicia el contador de usuarios*/
    void limpiar();

    /*Cantidad de claves distintas almacenadas*/
    std::size_t numUsuarios() const;

    /*Estimacion de memoria ocupada en bytes*/
    std::size_t memoriaBytes() const;

private:
    int tamano_;
    std::size_t numUsuarios_;
    std::vector<std::list<Entry>> tabla_;

    int indice(long long clave) const;
};

/*Para claves user_screen_name (como std::string) */
template <>
class HashTablaAbierto<std::string> {
public:
    struct Entry {
        std::string clave;
        int conteo;
        Entry(std::string k, int c) : clave(std::move(k)), conteo(c) {}
    };

    explicit HashTablaAbierto(int tamanoTabla);

    void incrementar(const std::string& clave);
    int obtener(const std::string& clave) const;
    void limpiar();
    std::size_t numUsuarios() const;
    std::size_t memoriaBytes() const;

private:
    int tamano_;
    std::size_t numUsuarios_;
    std::vector<std::list<Entry>> tabla_;

    int indice(const std::string& clave) const;
};
