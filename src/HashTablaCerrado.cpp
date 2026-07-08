/**Implementacion de tabla hash con salto abierto
 * buscarSlot recorre la secuencia de salto hasta encontrar la clave, un slot vacio o agotar la tabla, y reutiliza el primer slot BORRADO
 * encontrado para nuevas inserciones
 */

#include "HashTablaCerrado.hpp"

#include <stdexcept>

namespace {

int normalizarIndice(long long indice, int tamano) {
    long long r = indice % tamano;
    if (r < 0) r += tamano;
    return static_cast<int>(r);
}

/**
 * Realiza un desplazamiento de salto cuadratico alternante asi como +1,-1,+4,-4,+9,-9,...
 * Con m primo y m = 3 (mod 4) (200003 lo es), esta secuencia visita todas las celdas de la tabla
 */
long long desplazamientoCuadratico(int intento) {
    if (intento == 0) return 0;
    const long long i = (intento + 1) / 2;
    const long long signo = (intento % 2 == 1) ? 1 : -1;
    return signo * i * i;
}

}  

HashTablaCerrado<long long>::HashTablaCerrado(int tamanoTabla, ProbingStrategy estrategia)
    : tamano_(tamanoTabla), estrategia_(estrategia), numUsuarios_(0) {
    tabla_.resize(static_cast<std::size_t>(tamano_));
}

int HashTablaCerrado<long long>::indiceInicial(long long clave) const {
    return hashfn::indiceUserId(clave, tamano_);
}

int HashTablaCerrado<long long>::pasoSondeo(long long clave) const {
    return hashfn::h2(clave, tamano_);
}

int HashTablaCerrado<long long>::sondeo(long long clave, int intento) const {
    const int h = indiceInicial(clave);
    switch (estrategia_) {
        case ProbingStrategy::LINEAR:
            return normalizarIndice(static_cast<long long>(h) + intento, tamano_);
        case ProbingStrategy::QUADRATIC:
            return normalizarIndice(static_cast<long long>(h) + desplazamientoCuadratico(intento),
                                   tamano_);
        case ProbingStrategy::DOUBLE:
            return normalizarIndice(static_cast<long long>(h) +
                                       static_cast<long long>(intento) * pasoSondeo(clave),
                                   tamano_);
    }
    return h;
}

int HashTablaCerrado<long long>::buscarSlot(long long clave) {
    for (int intento = 0; intento < tamano_; ++intento) {
        const int idx = sondeo(clave, intento);
        Entry& entry = tabla_[static_cast<std::size_t>(idx)];

        if (entry.estado == Estado::VACIO) return idx;
        if (entry.clave == clave) return idx;
    }

    return -1;
}

void HashTablaCerrado<long long>::incrementar(long long clave) {
    const int idx = buscarSlot(clave);
    if (idx < 0) {
        throw std::runtime_error("Tabla hash cerrada llena (user_id)");
    }

    Entry& entry = tabla_[static_cast<std::size_t>(idx)];
    if (entry.estado == Estado::OCUPADO) {
        entry.conteo++;
        return;
    }

    entry.clave = clave;
    entry.conteo = 1;
    entry.estado = Estado::OCUPADO;
    numUsuarios_++;
}

int HashTablaCerrado<long long>::obtener(long long clave) const {
    for (int intento = 0; intento < tamano_; ++intento) {
        const int idx = sondeo(clave, intento);
        const Entry& entry = tabla_[static_cast<std::size_t>(idx)];

        if (entry.estado == Estado::VACIO) return 0;
        if (entry.estado == Estado::OCUPADO && entry.clave == clave) return entry.conteo;
    }
    return 0;
}

void HashTablaCerrado<long long>::limpiar() {
    for (auto& entry : tabla_) {
        entry.estado = Estado::VACIO;
        entry.conteo = 0;
        entry.clave = 0;
    }
    numUsuarios_ = 0;
}

std::size_t HashTablaCerrado<long long>::numUsuarios() const {
    return numUsuarios_;
}

std::size_t HashTablaCerrado<long long>::memoriaBytes() const {
    return sizeof(*this) + tabla_.capacity() * sizeof(Entry);
}

HashTablaCerrado<std::string>::HashTablaCerrado(int tamanoTabla, ProbingStrategy estrategia)
    : tamano_(tamanoTabla), estrategia_(estrategia), numUsuarios_(0) {
    tabla_.resize(static_cast<std::size_t>(tamano_));
}

int HashTablaCerrado<std::string>::indiceInicial(const std::string& clave) const {
    return hashfn::indiceScreenName(clave, tamano_);
}

int HashTablaCerrado<std::string>::pasoSondeo(const std::string& clave) const {
    return hashfn::h2String(clave, tamano_);
}

int HashTablaCerrado<std::string>::sondeo(const std::string& clave, int intento) const {
    const int h = indiceInicial(clave);
    switch (estrategia_) {
        case ProbingStrategy::LINEAR:
            return normalizarIndice(static_cast<long long>(h) + intento, tamano_);
        case ProbingStrategy::QUADRATIC:
            return normalizarIndice(static_cast<long long>(h) + desplazamientoCuadratico(intento),
                                   tamano_);
        case ProbingStrategy::DOUBLE:
            return normalizarIndice(static_cast<long long>(h) +
                                       static_cast<long long>(intento) * pasoSondeo(clave),
                                   tamano_);
    }
    return h;
}

int HashTablaCerrado<std::string>::buscarSlot(const std::string& clave) {
    for (int intento = 0; intento < tamano_; ++intento) {
        const int idx = sondeo(clave, intento);
        Entry& entry = tabla_[static_cast<std::size_t>(idx)];

        if (entry.estado == Estado::VACIO) return idx;
        if (entry.clave == clave) return idx;
    }

    return -1;
}

void HashTablaCerrado<std::string>::incrementar(const std::string& clave) {
    const int idx = buscarSlot(clave);
    if (idx < 0) {
        throw std::runtime_error("Tabla hash cerrada llena (user_screen_name)");
    }

    Entry& entry = tabla_[static_cast<std::size_t>(idx)];
    if (entry.estado == Estado::OCUPADO) {
        entry.conteo++;
        return;
    }

    entry.clave = clave;
    entry.conteo = 1;
    entry.estado = Estado::OCUPADO;
    numUsuarios_++;
}

int HashTablaCerrado<std::string>::obtener(const std::string& clave) const {
    for (int intento = 0; intento < tamano_; ++intento) {
        const int idx = sondeo(clave, intento);
        const Entry& entry = tabla_[static_cast<std::size_t>(idx)];

        if (entry.estado == Estado::VACIO) return 0;
        if (entry.estado == Estado::OCUPADO && entry.clave == clave) return entry.conteo;
    }
    return 0;
}

void HashTablaCerrado<std::string>::limpiar() {
    for (auto& entry : tabla_) {
        entry.estado = Estado::VACIO;
        entry.conteo = 0;
        entry.clave.clear();
    }
    numUsuarios_ = 0;
}

std::size_t HashTablaCerrado<std::string>::numUsuarios() const {
    return numUsuarios_;
}

std::size_t HashTablaCerrado<std::string>::memoriaBytes() const {
    std::size_t bytes = sizeof(*this) + tabla_.capacity() * sizeof(Entry);
    for (const auto& entry : tabla_) {
        if (entry.estado == Estado::OCUPADO) {
            bytes += entry.clave.capacity();
        }
    }
    return bytes;
}