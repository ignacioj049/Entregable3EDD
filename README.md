# Entregable 3 — Tablas Hash

**Integrantes:** Ignacio Jara, Vicente Ramírez y Danitza Ávila


## Estructura

```
Entregable 3/
├── include/
│   ├── ExperimentoRunner.hpp
│   ├── HashFunciones.hpp
│   ├── HashTablaAbierto.hpp
│   ├── HashTablaCerrado.hpp
│   ├── HashTablaUnorderedMap.hpp
│   ├── LectorTweets.hpp
│   └── Medicion.hpp
├── src/
│   ├── HashTablaAbierto.cpp
│   ├── HashTablaCerrado.cpp
│   └── LectorTweets.cpp
├── experimentos/
│   └── experimento_hash.cpp    
├── datasets/
│   └── auspol2019.csv
└── resultados/
    └── experimentos.csv
```

## Comando para compilar 

```powershell
g++ -std=c++17 -Wall -O2 -Iinclude `
  src/HashTablaAbierto.cpp `
  src/HashTablaCerrado.cpp `
  src/LectorTweets.cpp `
  experimentos/experimento_hash.cpp `
  -o experimentos/experimento_hash.exe
```
Para ejecutar el experimento:
```powershell
.\experimentos\experimento_hash.exe datasets\auspol2019.csv resultados\experimentos.csv 
```                                                   
