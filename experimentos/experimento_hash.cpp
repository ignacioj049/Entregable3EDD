 /**
 * Ejecuta las 5 estructuras (abierto, cerrado_linear, cerrado_quadratic,
 * cerrado_double, unordered_map) por las 2 claves (user_id, user_screen_name) por
 * los tamanos de muestra 10000, 20000, ..., hasta el total de tweets del
 * dataset. Cada combinacion se repite [repeticiones] veces (20 por defecto)
 * y se reporta el tiempo promedio, la desviacion estandar y el tamano en
 * memoria de la estructura.
 */

#include "ExperimentoRunner.hpp"
#include "LectorTweets.hpp"
#include "Medicion.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int REPETICIONES_DEFECTO = 20;

/* Corre un experimento (con la estructura+clave+muestra) las 20 veces y agrega los resultados */
struct ResultadoAgregado {
    double tiempoPromedioSegundos;
    double desviacionSegundos;
    std::size_t memoriaBytes;
};

ResultadoAgregado medirExperimento(experimento::Estructura estructura,
                                   experimento::ClaveTipo clave,
                                   const std::vector<Tweet>& muestra,
                                   int repeticiones) {
    std::vector<double> tiempos;
    tiempos.reserve(static_cast<std::size_t>(repeticiones));
    std::size_t memoria = 0;

    for (int i = 0; i < repeticiones; ++i) {
        const auto resultado = experimento::ejecutarUnaMedicion(estructura, clave, muestra);
        tiempos.push_back(resultado.tiempoSegundos);
        memoria = resultado.memoriaBytes;
    }

    return {medicion::promedio(tiempos), medicion::desviacionEstandar(tiempos), memoria};
}

void registrarFila(std::ofstream& out, int numeroExperimento, const std::string& nombreDataset,
                   experimento::Estructura estructura, experimento::ClaveTipo clave,
                   int cantidadTweets, const ResultadoAgregado& resultado) {
    out << numeroExperimento << ';' << nombreDataset << ';'
        << experimento::nombreEstructura(estructura) << ';'
        << experimento::nombreClave(clave) << ';' << cantidadTweets << ';'
        << resultado.tiempoPromedioSegundos << ';' << resultado.desviacionSegundos << ';'
        << resultado.memoriaBytes << '\n';
    out.flush();
}

std::string nombreArchivo(const std::string& ruta) {
    const auto pos = ruta.find_last_of("/\\");
    return (pos == std::string::npos) ? ruta : ruta.substr(pos + 1);
}

}  

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <dataset.csv> <resultados.csv> [repeticiones]\n";
        return 1;
    }

    const std::string rutaDataset = argv[1];
    const std::string rutaSalida = argv[2];
    const int repeticiones = (argc > 3) ? std::stoi(argv[3]) : REPETICIONES_DEFECTO;

    try {
        std::cout << "Entregable 3: Tablas Hash\n";
        std::cout << "Dataset: " << rutaDataset << "\n";
        std::cout << "Salida:  " << rutaSalida << "\n";
        std::cout << "Repeticiones por experimento: " << repeticiones << "\n";

        const auto tweets = leerTweets(rutaDataset);
        std::cout << "Tweets validos leidos: " << tweets.size() << "\n\n";

        if (tweets.empty()) {
            std::cerr << "[ERROR] El dataset no contiene tweets validos.\n";
            return 1;
        }

        std::ofstream out(rutaSalida, std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("No se pudo crear el archivo de salida: " + rutaSalida);
        }
        out << "experimento;dataset;estructura;clave;cantidad_tweets;"
               "tiempo_promedio_segundos;desviacion_segundos;memoria_bytes\n";

        const auto puntos = experimento::puntosMedicion(static_cast<int>(tweets.size()));
        const std::string nombreDataset = nombreArchivo(rutaDataset);

        const experimento::Estructura estructuras[] = {
            experimento::Estructura::ABIERTO,
            experimento::Estructura::CERRADO_LINEAR,
            experimento::Estructura::CERRADO_QUADRATIC,
            experimento::Estructura::CERRADO_DOUBLE,
            experimento::Estructura::UNORDERED_MAP,
        };

        int numeroExperimento = 1;

        for (const auto estructura : estructuras) {
            const std::string etiqueta = experimento::nombreEstructura(estructura);

            for (int n : puntos) {
                const std::vector<Tweet> muestra(tweets.begin(), tweets.begin() + n);

                const auto resultadoId = medirExperimento(
                    estructura, experimento::ClaveTipo::USER_ID, muestra, repeticiones);
                registrarFila(out, numeroExperimento++, nombreDataset, estructura,
                             experimento::ClaveTipo::USER_ID, n, resultadoId);

                const auto resultadoNombre = medirExperimento(
                    estructura, experimento::ClaveTipo::SCREEN_NAME, muestra, repeticiones);
                registrarFila(out, numeroExperimento++, nombreDataset, estructura,
                             experimento::ClaveTipo::SCREEN_NAME, n, resultadoNombre);

                std::cout << "[" << etiqueta << "] " << n << " tweets procesados\n";
            }
        }

        std::cout << "\nExperimentos finalizados. CSV en " << rutaSalida << "\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << '\n';
        return 1;
    }

    return 0;
}
