cmake_minimum_required(VERSION 3.14.0)

set(vst3sdk_SOURCE_DIR C:/Users/Public/Documents/Steinberg/VST_SDK/VST3_SDK)
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(WaveFoldSynth)
  
set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(WaveFoldSynth     
    source/version.h
    source/WaveFoldSynthCIDs.h
    source/WaveFoldSynthProcessor.h
    source/WaveFoldSynthProcessor.cpp
    source/WaveFoldSynthController.h
    source/WaveFoldSynthController.cpp
    source/WaveFoldSynth.cpp
    source/Voice.h
    source/Voice.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(
        WaveFoldSynth
        PRIVATE
            resource/editor.uidesc
    )
    target_link_libraries(
        WaveFoldSynth
        PRIVATE
            vstgui_support
    )
    smtg_add_vst3_resource(WaveFoldSynth "resource/editor.uidesc")
endif()
# -------------------

smtg_add_vst3_snapshot(WaveFoldSynth resource/F92508FE540557E2B860061BC3E97A16_snapshot.png)
smtg_add_vst3_snapshot(WaveFoldSynth resource/F92508FE540557E2B860061BC3E97A16_snapshot_2.0x.png)

target_link_libraries(WaveFoldSynth
    PRIVATE
        sdk
)

if(SMTG_MAC)
    smtg_set_bundle(WaveFoldSynth INFOPLIST 
        "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" 
        PREPROCESS
    )
elseif(SMTG_WIN)
    target_sources(WaveFoldSynth PRIVATE 
        resource/win32resource.rc
    )
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WaveFoldSynth)
	
	set_target_properties(WaveFoldSynth PROPERTIES
		VS_DEBUGGER_COMMAND
			"$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
		VS_DEBUGGER_COMMAND_ARGUMENTS
			" --pluginfolder \"$(OutDir)\" "
	)
endif()


