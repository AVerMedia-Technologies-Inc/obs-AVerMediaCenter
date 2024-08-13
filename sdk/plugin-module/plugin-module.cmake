set(AVT_OSB_PLUGIN_MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(AVT_OSB_PLUGIN_MODULE_INCLUDE_DIRS
    ${AVT_OSB_PLUGIN_MODULE_DIR}/include
)

#set(AVT_OSB_PLUGIN_MODULE_SOURCES
#    ${AVT_OSB_PLUGIN_MODULE_DIR}/
#)

set(AVT_OSB_PLUGIN_MODULE_HEADERS
    ${AVT_OSB_PLUGIN_MODULE_DIR}/include/Logger.h
	${AVT_OSB_PLUGIN_MODULE_DIR}/include/PluginModule.h
	${AVT_OSB_PLUGIN_MODULE_DIR}/include/PluginModule_global.h
)

function(configure_avt_obs_plugin_module target)
	target_include_directories(${target} PRIVATE
        ${AVT_OSB_PLUGIN_MODULE_INCLUDE_DIRS}
    )
    target_sources(${target} PRIVATE
#        ${AVT_OSB_PLUGIN_MODULE_SOURCES}
        ${AVT_OSB_PLUGIN_MODULE_HEADERS}
    )
	
	if(WIN32)
		target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/AvtPluginModule.lib
		)
		add_target_additional_deps(${CMAKE_PROJECT_NAME} MORE_FILES
			"${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/bin/AvtPluginModule.dll"
		)
	elseif(APPLE)
		#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
		#    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/libAvtPluginModule.dylib
		#)
		target_sources(${CMAKE_PROJECT_NAME} PRIVATE
			src/PluginModuleForMac.cpp
		)
		target_add_resource(${CMAKE_PROJECT_NAME}
			${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/libAvtPluginModule.dylib
		)
	elseif(UNIX)
		message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
	endif()
endfunction()