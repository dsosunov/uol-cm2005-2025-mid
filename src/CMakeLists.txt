# Source files
set(SOURCES
    actions/print_hello_action.cpp
    actions/show_version_action.cpp
    actions/product_query_action.cpp
    core/ui/io/standard_input.cpp
    core/ui/io/standard_output.cpp
    core/ui/menu/menu_node.cpp
    core/ui/menu/menu_renderer.cpp
    core/ui/menu/menu_input.cpp
    core/ui/menu/menu_engine.cpp
    core/ui/menu/menu_builder.cpp
    core/ui/form/form.cpp
    core/ui/form/form_builder.cpp
    core/ui/form/menu_form_input_provider.cpp
    core/ui/form/form_context.cpp
    core/ui/form/data_source.cpp
    core/ui/form/fields/text_field.cpp
    core/ui/form/fields/selection_field.cpp
    forms/product_query/fields/currency_pair_field.cpp
    forms/product_query/fields/query_type_field.cpp
    forms/product_query/fields/timeframe_field.cpp
    forms/product_query/fields/start_date_field.cpp
    forms/product_query/fields/end_date_field.cpp
    forms/product_query/validators/currency_pair_validator.cpp
    forms/product_query/binders/field_binders.cpp
    forms/product_query/product_query_form.cpp
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
