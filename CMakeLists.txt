cmake_minimum_required(VERSION 3.14)

project(WeatherApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt модули
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Positioning Charts UiTools Test)

# Главная сборка приложения
qt_add_executable(WeatherApp
    main.cpp
    mainwindow.cpp
    weatherapi.cpp
    location.cpp
    forecastchartwindow.cpp

    mainwindow.h
    weatherapi.h
    location.h
    forecastchartwindow.h

    mainwindow.ui
)

# Настройки автоматической генерации moc/uic/rcc
set_target_properties(WeatherApp PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_include_directories(WeatherApp PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(WeatherApp PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Positioning
    Qt6::Charts
)

enable_testing()
add_subdirectory(tests)
