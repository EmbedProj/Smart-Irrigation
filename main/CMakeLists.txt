# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS "main.c" "../src/WiFi.c" "../src/Mqtt.c" "../src/typeconv.c" "../src/htdriver.c" "../src/moisturedriver.c" "../src/pump.c" "../src/smart_controller.c" "../src/user_button.c"        # list the source files of this component
    INCLUDE_DIRS "../inc"       # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
