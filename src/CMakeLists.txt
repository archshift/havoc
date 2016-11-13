include_directories(".")

set(SOURCE_FILES
    main.cpp
    hardware-interface.cpp
    profile-interface.cpp
    raii-libusb.cpp
    )
set(HEADER_FILES
    hardware-interface.h
    havoc.h
    profile-interface.h
    raii-libusb.h
    )

add_executable(havoc ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(havoc ${LIBUSB_LIBRARIES})
add_dependencies(havoc libusb)

if (MSVC)
	add_custom_command(TARGET havoc POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${LIBUSB_OUTPUT_PREFIX}/libusb-1.0.dll" $<TARGET_FILE_DIR:havoc>)
endif()