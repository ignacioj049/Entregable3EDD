/** Parser de CSV para el dataset auspol2019.csv
 * Soporta campos entre comillas, comillas escapadas ("") y comas internas
 * Columnas usadas: indice 5 (con user_id) y el indice 7 (con user_screen_name)
 */

#include "LectorTweets.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

bool leerCaracter(std::ifstream& archivo, char& c) {
    return static_cast<bool>(archivo.get(c));
}

bool leerCampo(std::ifstream& archivo, std::string& campo) {
    campo.clear();

    const int siguiente = archivo.peek();
    if (siguiente == ',' || siguiente == '\n' || siguiente == '\r' ||
        siguiente == EOF) {
        return true;
    }

    char c = '\0';
    if (!leerCaracter(archivo, c)) return false;

    if (c == '"') {
        while (leerCaracter(archivo, c)) {
            if (c == '"') {
                if (archivo.peek() == '"') {
                    archivo.get();
                    campo.push_back('"');
                } else {
                    break;
                }
            } else {
                campo.push_back(c);
            }
        }
        return true;
    }

    campo.push_back(c);
    while (archivo.peek() != ',' && archivo.peek() != '\n' &&
           archivo.peek() != '\r' && archivo.peek() != EOF) {
        leerCaracter(archivo, c);
        campo.push_back(c);
    }
    return true;
}

bool leerFila(std::ifstream& archivo, std::vector<std::string>& campos) {
    campos.clear();

    while (archivo.peek() == '\r' || archivo.peek() == '\n') {
        archivo.get();
    }
    if (archivo.peek() == EOF) return false;

    while (true) {
        std::string campo;
        if (!leerCampo(archivo, campo)) {
            return !campos.empty();
        }
        campos.push_back(std::move(campo));

        const int siguiente = archivo.peek();
        if (siguiente == '\n' || siguiente == '\r' || siguiente == EOF) {
            break;
        }
        if (siguiente == ',') {
            archivo.get();
        }
    }

    if (archivo.peek() == '\r') archivo.get();
    if (archivo.peek() == '\n') archivo.get();
    return true;
}

long long parseUserId(const std::string& texto) {
    try {
        return std::stoll(texto);
    } catch (...) {
        return 0;
    }
}

} 

std::vector<Tweet> leerTweets(const std::string& rutaArchivo, int maxFilas) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir: " + rutaArchivo);
    }

    std::vector<Tweet> tweets;
    std::vector<std::string> campos;

    if (!leerFila(archivo, campos)) {
        throw std::runtime_error("CSV vacio o sin encabezado: " + rutaArchivo);
    }

    while (leerFila(archivo, campos)) {
        if (campos.size() < 8) continue;

        Tweet tweet;
        tweet.user_id = parseUserId(campos[5]);
        tweet.user_screen_name = campos[7];

        if (tweet.user_id == 0 || tweet.user_screen_name.empty()) continue;

        tweets.push_back(std::move(tweet));
        if (maxFilas > 0 && static_cast<int>(tweets.size()) >= maxFilas) break;
    }

    return tweets;
}
