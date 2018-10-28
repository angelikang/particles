find_package(Magnum REQUIRED GL Sdl2Application Shaders MeshTools Primitives)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(Particles Application.cpp)
target_link_libraries(Particles PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
    Magnum::MeshTools
    Magnum::Primitives)
