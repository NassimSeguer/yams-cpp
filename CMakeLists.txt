cmake_minimum_required(VERSION 3.20)
project(Yams LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Avertissements du compilateur, pour repérer les erreurs au plus tôt.
# Avec Visual Studio, /utf-8 indique que les fichiers sources sont en UTF-8 (accents, ┏, ●...).
if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra)
endif ()

# Fichiers du jeu, partagés entre l'exécutable et les tests
set(SOURCES_JEU
    src/unicode.cpp
    src/des.cpp
    src/score.cpp
    src/saisie.cpp
    src/affichage.cpp
    src/partie.cpp
)

# Le jeu
add_executable(yams src/main.cpp ${SOURCES_JEU})
target_include_directories(yams PRIVATE include)

# Les tests : lancer "ctest" dans le dossier de compilation
enable_testing()
add_executable(yams_tests tests/tests.cpp ${SOURCES_JEU})
target_include_directories(yams_tests PRIVATE include)
add_test(NAME tests_yams COMMAND yams_tests)
