if(ESP_PLATFORM)


file(GLOB_RECURSE SOURCES generated/*.c custom/*.c)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS  ../ ../lvgl/src/lv_font ./generated ./generated/guider_fonts ./custom
                       REQUIRES lvgl)

else()
    message(FATAL_ERROR "LVGL LV: ESP_PLATFORM is not defined. Try reinstalling ESP-IDF.")
endif()
