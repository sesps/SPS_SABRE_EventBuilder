root_generate_dictionary(G__SPSDict DataStructs.h LINKDEF LinkDef_sps.h)

add_library(SPSDict SHARED DataStructs.h DataStructs.cpp G__SPSDict.cxx)
add_custom_command(TARGET SPSDict
	POST_BUILD
	COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/libSPSDict_rdict.pcm ${CMAKE_INSTALL_PREFIX}/lib/
	VERBATIM)
target_include_directories(SPSDict PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR})
target_precompile_headers(SPSDict PUBLIC ${PCH_DIR}/EventBuilder.h)
set_target_properties(SPSDict PROPERTIES PUBLIC_HEADER "DataStructs.h")
target_link_libraries(SPSDict
	PUBLIC
		ROOT::Gui
		ROOT::Core
		ROOT::Gpad
		ROOT::Graf3d
		ROOT::Graf
		ROOT::Hist
		ROOT::Imt
		ROOT::MathCore
		ROOT::Matrix
		ROOT::MultiProc
		ROOT::Net
		ROOT::Physics
		ROOT::Postscript
		ROOT::RIO
		ROOT::ROOTDataFrame
		ROOT::ROOTVecOps
		ROOT::Rint
		ROOT::Thread
		ROOT::TreePlayer
		ROOT::Tree
)

install(TARGETS SPSDict
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include)