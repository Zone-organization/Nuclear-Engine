cmake_minimum_required(VERSION 3.5)

project(Nuclear.Project)
add_subdirectory(Nuclear.Engine)
add_subdirectory(Samples)


#third party
add_subdirectory(External/Assimp)
add_subdirectory(External/Diligent)
add_subdirectory(External/OpenAL)