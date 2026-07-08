#pragma once

/**Lectura del dataset auspol2019.csv
 * Extrae el user_id (de la columna 5) y el user_screen_name (de la columna 7) de cada fila
 * Descarta las filas mal formadas o con campos vacios.
 */

#include <string>
#include <vector>

/*Representacion minima de un tweet para el conteo por usuario*/
struct Tweet {
    long long user_id;
    std::string user_screen_name;
};

/**
 * Lee tweets desde un archivo CSV, en rutaArchivo,  que es la ruta al CSV (en este caso datasets/auspol2019.csv).
 * el limite de filas (-1 = leer todo el archivo)
 * retorna el vector de tweets validos
 */
std::vector<Tweet> leerTweets(const std::string& rutaArchivo, int maxFilas = -1);
