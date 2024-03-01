CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

SET(world_sources
	client.cpp
	cliententry.cpp
	clientlist.cpp
	console.cpp
	eql_config.cpp
	launcher_link.cpp
	launcher_list.cpp
	login_server.cpp
	login_server_list.cpp
	net.cpp
	queryserv.cpp
	ucs.cpp
	wguild_mgr.cpp
	world_event_scheduler.cpp
	world_config.cpp
	world_server_command_handler.cpp
	worlddb.cpp
	zonelist.cpp
	zoneserver.cpp
)

SET(world_headers
	client.h
	cliententry.h
	clientlist.h
	console.h
	eql_config.h
	launcher_link.h
	launcher_list.h
	login_server.h
	login_server_list.h
	net.h
	queryserv.h
	char_create_data.h
	ucs.h
	wguild_mgr.h
	world_config.h
	world_tcp_connection.h
	world_server_command_handler.h
	worlddb.h
	world_event_scheduler.h
	zonelist.h
	zoneserver.h
)

ADD_EXECUTABLE(world ${world_sources} ${world_headers})

INSTALL(TARGETS world RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

IF (WIN32 AND EQEMU_BUILD_PCH)
    TARGET_PRECOMPILE_HEADERS(world PRIVATE ../common/pch/pch.h)
ENDIF ()

ADD_DEFINITIONS(-DWORLD)

TARGET_LINK_LIBRARIES(world ${SERVER_LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
