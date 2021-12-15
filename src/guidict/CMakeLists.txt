root_generate_dictionary(G__GUIDict EVBMainFrame.h FileViewFrame.h LINKDEF LinkDef_Gui.h)

add_library(GUIDict SHARED EVBMainFrame.cpp FileViewFrame.cpp EVBMainFrame.h FileViewFrame.h G__GUIDict.cxx)
add_custom_command(TARGET GUIDict
	POST_BUILD
	COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/libGUIDict_rdict.pcm ${CMAKE_INSTALL_PREFIX}/lib/
	VERBATIM)
target_include_directories(GUIDict PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PCH_DIR} ${SPDLOG_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/../evb)
target_precompile_headers(GUIDict PUBLIC ${PCH_DIR}/EventBuilder.h)
set_target_properties(GUIDict PROPERTIES PUBLIC_HEADER "EVBMainFrame.h;FileViewFrame.h")
target_link_libraries(GUIDict
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
		spdlog::spdlog
)

install(TARGETS GUIDict 
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include)