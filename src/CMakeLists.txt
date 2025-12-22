# Source files
set(SOURCES
    actions/candlestick_summary_action.cpp
    actions/user_register_action.cpp
    actions/user_login_action.cpp
    actions/user_reset_action.cpp
    actions/wallet_view_balance_action.cpp
    actions/wallet_deposit_action.cpp
    actions/wallet_withdraw_action.cpp
    actions/transaction_show_last5_action.cpp
    actions/transaction_show_by_pair_action.cpp
    actions/transaction_activity_summary_action.cpp
    actions/auto_generate_trades_action.cpp
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
    forms/shared/validators/currency_pair_validator.cpp
    forms/candlestick/fields/currency_pair_field.cpp
    forms/candlestick/fields/asks_bids_field.cpp
    forms/candlestick/fields/timeframe_field.cpp
    forms/candlestick/fields/start_date_field.cpp
    forms/candlestick/fields/end_date_field.cpp
    forms/candlestick/candlestick_form.cpp
    forms/user/fields/full_name_field.cpp
    forms/user/fields/email_field.cpp
    forms/user/fields/password_field.cpp
    forms/user/fields/username_field.cpp
    forms/user/fields/login_password_field.cpp
    forms/user/fields/email_or_username_field.cpp
    forms/user/fields/new_password_field.cpp
    forms/user/registration_form.cpp
    forms/user/login_form.cpp
    forms/user/reset_form.cpp
    forms/wallet/fields/currency_field.cpp
    forms/wallet/fields/amount_field.cpp
    forms/wallet/validators/currency_validator.cpp
    forms/wallet/wallet_operation_form.cpp
    forms/transaction/fields/product_pair_field.cpp
    forms/transaction/fields/timeframe_field.cpp
    forms/transaction/fields/start_date_field.cpp
    forms/transaction/fields/end_date_field.cpp
    forms/transaction/product_pair_form.cpp
    forms/transaction/activity_summary_form.cpp
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
