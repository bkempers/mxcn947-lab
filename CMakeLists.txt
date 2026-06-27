# Module-level CMakeLists.txt
# This is NOT the application entry point — it's the Zephyr module entry
# referenced by zephyr/module.yml. It wires out-of-tree drivers and libs
# into the Zephyr build.

zephyr_include_directories(include)

add_subdirectory(drivers)
add_subdirectory(lib)
