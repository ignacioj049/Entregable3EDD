#pragma once

/**
 * Se usa como referencia de la biblioteca estandar, donde la STL gestiona las celdad y hace rehash internamente
 */

#include <cstddef>
#include <string>
#include <unordered_map>

template <typename Clave>
class HashTablaUnorderedMap;

template <>
class HashTablaUnorderedMap<long long>
{
public:
    explicit HashTablaUnorderedMap(int) {}

    void incrementar(long long clave) { map_[clave]++; }
    int obtener(long long clave) const
    {
        const auto it = map_.find(clave);
        return it == map_.end() ? 0 : it->second;
    }
    void limpiar() { map_.clear(); }
    std::size_t numUsuarios() const { return map_.size(); }
    std::size_t memoriaBytes() const
    {
        std::size_t bytes = sizeof(*this) + map_.bucket_count() * sizeof(void *);
        for (const auto &[clave, conteo] : map_)
        {
        /* Overhead por nodo en libstdc++ son de 8 bytes correspondientes al puntero de encadenamiento interno, por eso se suman a la aproximacion */
            bytes += sizeof(clave) + sizeof(conteo) + 8;
        }
        return bytes;
    }

private:
    std::unordered_map<long long, int> map_;
};

template <>
class HashTablaUnorderedMap<std::string>
{
public:
    explicit HashTablaUnorderedMap(int) {}

    void incrementar(const std::string &clave) { map_[clave]++; }
    int obtener(const std::string &clave) const
    {
        const auto it = map_.find(clave);
        return it == map_.end() ? 0 : it->second;
    }
    void limpiar() { map_.clear(); }
    std::size_t numUsuarios() const { return map_.size(); }
    std::size_t memoriaBytes() const
    {
        std::size_t bytes = sizeof(*this) + map_.bucket_count() * sizeof(void *);
        for (const auto &[clave, conteo] : map_)
        {
    /* el overhead por nodo en libstdc++ son 16 bytes = 8 del puntero de encadenamiento + 8 del
       valor hash */
            bytes += sizeof(conteo) + clave.capacity() + 16;
        }
        return bytes;
    }

private:
    std::unordered_map<std::string, int> map_;
};
