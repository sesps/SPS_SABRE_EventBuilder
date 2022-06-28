add_library(SPSDict SHARED)

target_include_directories(SPSDict SYSTEM PUBLIC ${ROOT_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(sps_dict DataStructs.h LINKDEF LinkDef_sps.h MODULE SPSDict)

target_sources(SPSDict PRIVATE
    DataStructs.h
    DataStructs.cpp
)
target_link_libraries(SPSDict ${ROOT_LIBRARIES})
set_target_properties(SPSDict PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EVB_LIBRARY_DIR})

add_custom_command(TARGET SPSDict POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/libSPSDict_rdict.pcm
    ${EVB_LIBRARY_DIR}/libSPSDict_rdict.pcm
)
