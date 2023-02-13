project(Nuclear.Engine 
		VERSION 0.0
        DESCRIPTION "Nuclear Engine by Zone Org."
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE NUCLEAR_HEADER_FILES "include/*.h")
file(GLOB_RECURSE NUCLEAR_SOURCE_FILES "Source/*.cpp")

add_library(${PROJECT_NAME}  SHARED 
    ${NUCLEAR_SOURCE_FILES} ${NUCLEAR_HEADER_FILES}
)

add_library(Nuclear::Engine ALIAS ${PROJECT_NAME} )


target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} 
    DiligentCore
    Assimp
    OpenAL
)