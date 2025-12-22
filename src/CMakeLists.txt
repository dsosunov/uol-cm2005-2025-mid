# Source files
set(SOURCES
    actions/print_hello_action.cpp
    actions/show_version_action.cpp
    io/standard_input.cpp
    io/standard_output.cpp
    menu/menu_node.cpp
    menu/menu_renderer.cpp
    menu/menu_input.cpp
    menu/menu_engine.cpp
    menu/menu_builder.cpp
    main.cpp
)

# Create executable
add_executable(MenuSystem ${SOURCES})

# Include directories
target_include_directories(MenuSystem PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Set folder for IDE
set_target_properties(MenuSystem PROPERTIES FOLDER "Application")
