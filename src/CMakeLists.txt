set(RUNTIME Timer)

add_executable(${RUNTIME}
    main.cpp
)

target_link_libraries(${RUNTIME} PUBLIC logging)
