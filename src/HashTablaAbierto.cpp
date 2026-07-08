/**Implementacion de tabla hash con encadenamiento.
 *
 * busca la clave en la lista de celdas; si existe incrementa, si no inserta una nueva Entry(clave, 1).
 */

#include "HashTablaAbierto.hpp"

HashTablaAbierto<long long>::HashTablaAbierto(int tamanoTabla)
    : tamano_(tamanoTabla), numUsuarios_(0) {
    tabla_.resize(static_cast<std::size_t>(tamano_));
}

int HashTablaAbierto<long long>::indice(long long clave) const {
    return hashfn::indiceUserId(clave, tamano_);
}

void HashTablaAbierto<long long>::incrementar(long long clave) {
    const int idx = indice(clave);
    for (auto& entry : tabla_[static_cast<std::size_t>(idx)]) {
        if (entry.clave == clave) {
            entry.conteo++;
            return;
        }
    }
    tabla_[static_cast<std::size_t>(idx)].emplace_back(clave, 1);
    numUsuarios_++;
}

int HashTablaAbierto<long long>::obtener(long long clave) const {
    const int idx = indice(clave);
    for (const auto& entry : tabla_[static_cast<std::size_t>(idx)]) {
        if (entry.clave == clave) return entry.conteo;
    }
    return 0;
}

void HashTablaAbierto<long long>::limpiar() {
    for (auto& bucket : tabla_) bucket.clear();
    numUsuarios_ = 0;
}

std::size_t HashTablaAbierto<long long>::numUsuarios() const {
    return numUsuarios_;
}

std::size_t HashTablaAbierto<long long>::memoriaBytes() const {
    std::size_t bytes = sizeof(*this);
    bytes += tabla_.capacity() * sizeof(std::list<Entry>);

    for (const auto& bucket : tabla_) {
        bytes += bucket.size() * (sizeof(Entry) + sizeof(void*) * 2);
    }
    return bytes;
}

HashTablaAbierto<std::string>::HashTablaAbierto(int tamanoTabla)
    : tamano_(tamanoTabla), numUsuarios_(0) {
    tabla_.resize(static_cast<std::size_t>(tamano_));
}

int HashTablaAbierto<std::string>::indice(const std::string& clave) const {
    return hashfn::indiceScreenName(clave, tamano_);
}

void HashTablaAbierto<std::string>::incrementar(const std::string& clave) {
    const int idx = indice(clave);
    for (auto& entry : tabla_[static_cast<std::size_t>(idx)]) {
        if (entry.clave == clave) {
            entry.conteo++;
            return;
        }
    }
    tabla_[static_cast<std::size_t>(idx)].emplace_back(clave, 1);
    numUsuarios_++;
}

int HashTablaAbierto<std::string>::obtener(const std::string& clave) const {
    const int idx = indice(clave);
    for (const auto& entry : tabla_[static_cast<std::size_t>(idx)]) {
        if (entry.clave == clave) return entry.conteo;
    }
    return 0;
}

void HashTablaAbierto<std::string>::limpiar() {
    for (auto& bucket : tabla_) bucket.clear();
    numUsuarios_ = 0;
}

std::size_t HashTablaAbierto<std::string>::numUsuarios() const {
    return numUsuarios_;
}

std::size_t HashTablaAbierto<std::string>::memoriaBytes() const {
    std::size_t bytes = sizeof(*this);
    bytes += tabla_.capacity() * sizeof(std::list<Entry>);

    for (const auto& bucket : tabla_) {
        for (const auto& entry : bucket) {
            bytes += sizeof(Entry) + sizeof(void*) * 2 + entry.clave.capacity();
        }
    }
    return bytes;
}
