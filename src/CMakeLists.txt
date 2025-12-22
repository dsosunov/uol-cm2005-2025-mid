# Source files
set(SOURCES
    actions.cpp
    io.cpp
    menu_node.cpp
    menu_renderer.cpp
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
