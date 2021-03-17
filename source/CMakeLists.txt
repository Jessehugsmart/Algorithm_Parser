add_executable(algoTest main.c algoparse.c hug_windoor_state.c)
find_package(jansson CONFIG REQUIRED)
target_link_libraries(algoTest PUBLIC jansson::jansson)