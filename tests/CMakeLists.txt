# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(menu_tests
    test_full_name_validator.cpp
    test_menu_auth_visibility.cpp
    test_timeframe_date_binders.cpp
    test_candlestick_service_e2e.cpp
    test_form_read_retries.cpp
    test_transactions_activity_summary_e2e.cpp
    test_trading_simulation_initial_balances_e2e.cpp
)

# Link against gtest and the MenuSystemLib library
target_link_libraries(menu_tests
    gtest_main
    gtest
    MenuSystemLib
)

# Include directories
target_include_directories(menu_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(menu_tests)

# Set folder for IDE
# set_target_properties(menu_tests PROPERTIES FOLDER "Tests")
set_target_properties(gtest PROPERTIES FOLDER "GoogleTest")
set_target_properties(gtest_main PROPERTIES FOLDER "GoogleTest")
set_target_properties(gmock PROPERTIES FOLDER "GoogleTest")
set_target_properties(gmock_main PROPERTIES FOLDER "GoogleTest")
