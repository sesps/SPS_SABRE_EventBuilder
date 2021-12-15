add_subdirectory(spsdict)
add_subdirectory(guidict)

add_executable(EventBuilder)
target_compile_definitions(EventBuilder PUBLIC ETC_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../etc/")
target_sources(EventBuilder
	PRIVATE
		main.cpp
		evb/ChannelMap.cpp
		evb/ChannelMap.h
		evb/CompassFile.cpp
		evb/CompassFile.h
		evb/CompassRun.cpp
		evb/CompassRun.h
		evb/CutHandler.cpp
		evb/CutHandler.h
		evb/EVBApp.cpp
		evb/EVBApp.h
		evb/FastSort.cpp
		evb/FastSort.h
		evb/FlagHandler.cpp
		evb/FlagHandler.h
		evb/FP_kinematics.cpp
		evb/FP_kinematics.h
		evb/MassLookup.cpp
		evb/MassLookup.h
		evb/OrderChecker.cpp
		evb/OrderChecker.h
		evb/RunCollector.cpp
		evb/RunCollector.h
		evb/SFPAnalyzer.cpp
		evb/SFPAnalyzer.h
		evb/SFPPlotter.cpp
		evb/SFPPlotter.h
		evb/SlowSort.cpp
		evb/SlowSort.h
		evb/Stopwatch.cpp
		evb/Stopwatch.h
		evb/ShiftMap.cpp
		evb/ShiftMap.h
		spsdict/DataStructs.h
)
target_include_directories(EventBuilder PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(EventBuilder PUBLIC ${CMAKE_CURRENT_LIST_DIR}/evb)
target_include_directories(EventBuilder PUBLIC ${CMAKE_CURRENT_LIST_DIR}/spsdict)
target_link_libraries(EventBuilder
	PUBLIC
		SPSDict
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


add_executable(EventBuilderGui)
target_compile_definitions(EventBuilderGui PUBLIC ETC_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../etc/")
target_sources(EventBuilderGui
	PRIVATE
		gui_main.cpp
		evb/ChannelMap.cpp
		evb/ChannelMap.h
		evb/CompassFile.cpp
		evb/CompassFile.h
		evb/CompassRun.cpp
		evb/CompassRun.h
		evb/CutHandler.cpp
		evb/CutHandler.h
		evb/EVBApp.cpp
		evb/EVBApp.h
		evb/FastSort.cpp
		evb/FastSort.h
		evb/FlagHandler.cpp
		evb/FlagHandler.h
		evb/FP_kinematics.cpp
		evb/FP_kinematics.h
		evb/MassLookup.cpp
		evb/MassLookup.h
		evb/OrderChecker.cpp
		evb/OrderChecker.h
		evb/RunCollector.cpp
		evb/RunCollector.h
		evb/SFPAnalyzer.cpp
		evb/SFPAnalyzer.h
		evb/SFPPlotter.cpp
		evb/SFPPlotter.h
		evb/SlowSort.cpp
		evb/SlowSort.h
		evb/Stopwatch.cpp
		evb/Stopwatch.h
		evb/ShiftMap.cpp
		evb/ShiftMap.h
		spsdict/DataStructs.h
		guidict/EVBMainFrame.h
		guidict/FileViewFrame.h
)
target_include_directories(EventBuilderGui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(EventBuilderGui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/evb)
target_include_directories(EventBuilderGui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/spsdict)
target_include_directories(EventBuilderGui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/guidict)
target_link_libraries(EventBuilderGui
	PUBLIC
		SPSDict
		GUIDict
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

install(TARGETS EventBuilder EventBuilderGui RUNTIME DESTINATION bin)