add_executable(example_ftxui main.cpp)

target_include_directories(example_ftxui
                           PUBLIC "${CMAKE_SOURCE_DIR}/deps/FTXUI/include/")
target_link_libraries(example_ftxui PRIVATE ftxui::screen ftxui::dom
                                             ftxui::component)

target_compile_features(example_ftxui PRIVATE cxx_std_20)

install(TARGETS example_ftxui RUNTIME DESTINATION bin)
