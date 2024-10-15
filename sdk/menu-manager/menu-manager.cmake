set(AVT_OBS_MENU_MANAGER_DIR ${CMAKE_CURRENT_LIST_DIR})

function(configure_avt_obs_menu_manager target)
	target_include_directories(${target} PRIVATE
        ${AVT_OBS_MENU_MANAGER_DIR}/include
    )
    target_sources(${target} PRIVATE
        ${AVT_OBS_MENU_MANAGER_DIR}/include/AvtObsMenuManager.h
        ${AVT_OBS_MENU_MANAGER_DIR}/include/AvtObsMenuManager_global.h
    )
	
	if(WIN32)
		#message("https://cmake.org/cmake/help/latest/variable/WIN32.html?highlight=win32")
		target_link_libraries(${target} PRIVATE
			"${AVT_OBS_MENU_MANAGER_DIR}/lib/avt-obs-menu-manager.lib"
		)
	elseif(APPLE)
		#message("https://cmake.org/cmake/help/latest/variable/APPLE.html?highlight=apple")
		target_link_libraries(${target} PRIVATE
			"${AVT_OBS_MENU_MANAGER_DIR}/lib/libavt-obs-menu-manager_static.a"
		)
	elseif(UNIX)
		message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
	endif()
	
endfunction()
