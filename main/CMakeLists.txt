set(srcs
    main.c
    speech_actions.c)

set(requires
    ewth_hardware_driver)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS include
                       REQUIRES ${requires})

component_compile_options(-w)
