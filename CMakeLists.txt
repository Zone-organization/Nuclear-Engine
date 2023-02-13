cmake_minimum_required(VERSION 3.5)

project(Nuclear.Project)
add_subdirectory(Nuclear.Engine)
add_subdirectory(Samples)

set(CMAKE_MODULE_PATH "${/cmake")


#third party
add_subdirectory(External/Assimp)
add_subdirectory(External/Diligent)
add_subdirectory(External/OpenAL)
add_subdirectory(External/msdf-atlas-gen)