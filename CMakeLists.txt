cmake_minimum_required(VERSION 3.16)
project(OilyBob VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find system SDL3 (or fail clearly)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_executable(oily-bob
    main.cpp
    level-maker.cpp
    # add any other .cpp files here
)

target_link_libraries(oily-bob PRIVATE SDL3::SDL3)

# Optional: copy assets next to the executable after build
add_custom_command(TARGET oily-bob POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:oily-bob>/assets
    COMMENT "Copying assets..."
)