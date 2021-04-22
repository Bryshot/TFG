cmake_minimum_required(VERSION 3.5)

project(AutoProducerContest)

if(BUILD_OUT_OF_TREE)
	# out of tree builds
	# credits to c3r1c3
	set(CMAKE_PREFIX_PATH "${QTDIR}")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external")

	include(external/FindLibObs.cmake)
	find_package(LibObs REQUIRED)
	find_package(Qt5Core REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	find_package(Libcurl REQUIRED)
	find_path(LIBOBS_FRONTEND_INCLUDE_DIR HINTS ${LIBOBS_INCLUDE_DIRS})
	find_file(LIBOBS_FRONTEND_API_LIB NAMES libobs-frontend-api.* HINTS ${LIBOBS_LIB} )

	if(APPLE)
		find_library(COCOA Cocoa)
		find_package(Qt5MacExtras REQUIRED)
		include_directories(${COCOA})
		add_link_options("-v")
	endif()

	if(UNIX AND NOT APPLE)
		find_package(X11 REQUIRED)
		link_libraries(${X11_LIBRARIES})
		include_directories(${X11_INCLUDE_DIR})
	endif()

	set(autoProducerContest_HEADERS
		${autoProducerContest_HEADERS}
		src/headers/autoProducerContest.hpp
		src/headers/switcher-data-structs.hpp
		src/headers/curl-helper.hpp
		src/headers/setup.h
		src/headers/importIPs.h
		src/headers/curlCore.h
		)

	set(autoProducerContest_SOURCES
		${autoProducerContest_SOURCES}
		src/autoProducerContest.cpp
		src/autoProducerContest-module.c
		src/hotkey.cpp
		src/general.cpp
		src/setup.cpp
		src/importIPs.cpp
		src/curl-helper.cpp
		src/curlCore.cpp
		)

	set(autoProducerContest_UI
		${autoProducerContest_UI}
		forms/autoProducerContest.ui
		)

	qt5_wrap_ui(autoProducerContest_UI_HEADERS
		${autoProducerContest_UI}
		${autoProducerContest_PLATFORM_UI})

	add_library(autoProducerContest MODULE
		${autoProducerContest_HEADERS}
		${autoProducerContest_SOURCES}
		${autoProducerContest_UI_HEADERS}
		)

	include_directories(
		"${LIBOBS_INCLUDE_DIRS}"
		"${LIBOBS_FRONTEND_INCLUDE_DIR}"
		${Qt5Core_INCLUDES}
		${Qt5Widgets_INCLUDES}
		${LIBCURL_INCLUDE_DIRS})

	target_link_libraries(autoProducerContest
		${LIBOBS_LIB}
		${LIBOBS_FRONTEND_API_LIB}
		Qt5::Core
		Qt5::Widgets)

	# Additional commands to install the module in the correct place.
	# Find all the translation files so we can copy them to the correct
	# place later on.
	file(GLOB ASS_TRANSLATION_FILES
			"data/locale/*.ini"
			)

	# Windows
	if(WIN32)
		string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		message("${CMAKE_CXX_FLAGS_RELEASE}")

		find_package(w32-pthreads REQUIRED)

		target_link_libraries(autoProducerContest
				w32-pthreads)
	endif()

	# OSX
	if(APPLE)
		set_target_properties(autoProducerContest PROPERTIES PREFIX "")

	endif()

	# Linux
	if(UNIX AND NOT APPLE)
		set(CMAKE_C_FLAGS "-Wall -Wextra -Wvla -Wno-unused-function -Werror-implicit-function-declaration -Wno-missing-braces -Wno-missing-field-initializers ${CMAKE_C_FLAGS} -std=c99 -fno-strict-aliasing")

		if(ARCH EQUAL 64)
			set(ARCH_NAME "x86_64")
		else()
			set(ARCH_NAME "i686")
		endif()

		set_target_properties(autoProducerContest PROPERTIES PREFIX "")

		install(TARGETS autoProducerContest
				LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/obs-plugins)
		install(FILES ${ASS_TRANSLATION_FILES}
				DESTINATION "${CMAKE_INSTALL_PREFIX}/share/obs/obs-plugins/autoProducerContest/locale")
	endif()
else ()
	# in-tree build
	if(APPLE)
		find_library(COCOA Cocoa)
		include_directories(${COCOA})
	endif()

	if(UNIX AND NOT APPLE)
		find_package(X11 REQUIRED)
		link_libraries(${X11_LIBRARIES})
		include_directories(${X11_INCLUDE_DIR})
	endif()

	set(autoProducerContest_HEADERS
		${autoProducerContest_HEADERS}
		src/headers/autoProducerContest.hpp
		src/headers/switcher-data-structs.hpp
		src/headers/curl-helper.hpp
		src/headers/setup.h
		src/headers/importIPs.h
		src/headers/curlCore.h
		)

	set(autoProducerContest_SOURCES
		${autoProducerContest_SOURCES}
		src/autoProducerContest.cpp
		src/autoProducerContest-module.c
		src/hotkey.cpp
		src/general.cpp
		src/setup.cpp
		src/importIPs.cpp
		src/curl-helper.cpp
		src/curlCore.cpp
		)

	set(autoProducerContest_UI
		${autoProducerContest_UI}
		forms/autoProducerContest.ui
		)


	qt5_wrap_ui(autoProducerContest_UI_HEADERS
		${autoProducerContest_UI}
		${autoProducerContest_PLATFORM_UI})

	add_library(autoProducerContest MODULE
		${autoProducerContest_HEADERS}
		${autoProducerContest_SOURCES}
		${autoProducerContest_UI_HEADERS}
		)

	target_link_libraries(autoProducerContest
		obs-frontend-api
		Qt5::Widgets
		libobs)

	install_obs_plugin(autoProducerContest data)
endif()
