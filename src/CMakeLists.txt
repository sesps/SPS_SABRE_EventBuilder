add_subdirectory(spsdict)
add_subdirectory(evb)
add_subdirectory(guidict)


add_executable(EventBuilder)
target_include_directories(EventBuilder SYSTEM PUBLIC ../vendor/spdlog/include ${ROOT_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(EventBuilder PRIVATE main.cpp)
target_link_libraries(EventBuilder
    SPSDict
    EventBuilderCore
    ${ROOT_LIBRARIES}
)
set_target_properties(EventBuilder PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EVB_BINARY_DIR})

add_executable(EventBuilderGui)
target_include_directories(EventBuilderGui SYSTEM PUBLIC ../vendor/spdlog/include ${ROOT_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(EventBuilderGui PRIVATE gui_main.cpp)
target_link_libraries(EventBuilderGui
    SPSDict
    GuiDict
    EventBuilderCore
    ${ROOT_LIBRARIES}
)
set_target_properties(EventBuilderGui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EVB_BINARY_DIR})