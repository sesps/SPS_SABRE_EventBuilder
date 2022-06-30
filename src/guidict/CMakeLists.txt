add_library(GuiDict SHARED)

target_include_directories(GuiDict SYSTEM PUBLIC ../../vendor/spdlog/include/ ${ROOT_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(gui_dict EVBMainFrame.h FileViewFrame.h LINKDEF LinkDef_Gui.h MODULE GuiDict)

target_sources(GuiDict PRIVATE
    FileViewFrame.h
    FileViewFrame.cpp
    EVBMainFrame.h
    EVBMainFrame.cpp
)
target_link_libraries(GuiDict
    EventBuilderCore
    SPSDict
    ${ROOT_LIBRARIES}
)
set_target_properties(GuiDict PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EVB_LIBRARY_DIR})

add_custom_command(TARGET GuiDict POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/libGuiDict_rdict.pcm
    ${EVB_LIBRARY_DIR}/libGuiDict_rdict.pcm
)
