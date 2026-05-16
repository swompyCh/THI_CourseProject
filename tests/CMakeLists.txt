cmake_minimum_required(VERSION 3.14)

find_package(Qt6 REQUIRED COMPONENTS Test Widgets Charts Network Positioning)

# Пути к исходникам основного приложения
set(APP_SOURCES
    ../mainwindow.cpp
    ../weatherapi.cpp
    ../location.cpp
    ../forecastchartwindow.cpp
)

# UI
set(APP_UIS
    ../mainwindow.ui
)

qt6_wrap_ui(APP_UI_HEADERS ${APP_UIS})

# Общие заголовки
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Включаем автоматическую генерацию moc/uic/rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# =========================================================
# NEW TEST: Weather Emoji
# =========================================================
qt_add_executable(TestWeatherEmoji
    test_weatheremoji.cpp
    ${APP_SOURCES}
    ${APP_UI_HEADERS}
)

target_link_libraries(TestWeatherEmoji PRIVATE
    Qt6::Test
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
    Qt6::Positioning
)

add_test(NAME TestWeatherEmoji COMMAND TestWeatherEmoji)

# =========================================================
# NEW TEST: Parse Forecast
# =========================================================
qt_add_executable(TestParseForecast
    test_parseforecast.cpp
    ${APP_SOURCES}
    ${APP_UI_HEADERS}
)

target_link_libraries(TestParseForecast PRIVATE
    Qt6::Test
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
    Qt6::Positioning
)

add_test(NAME TestParseForecast COMMAND TestParseForecast)

# =========================================================
# NEW TEST: Invalid JSON
# =========================================================
qt_add_executable(TestInvalidJson
    test_invalidjson.cpp
    ${APP_SOURCES}
    ${APP_UI_HEADERS}
)

target_link_libraries(TestInvalidJson PRIVATE
    Qt6::Test
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
    Qt6::Positioning
)

add_test(NAME TestInvalidJson COMMAND TestInvalidJson)