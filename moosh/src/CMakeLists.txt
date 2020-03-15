project(moosh)

set(MOOSH_INCLUDES "${CMAKE_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_executable(moosh 
	msg_handler.cpp 
	msg_handler.hpp
	moosh_builtins.cpp 
	moosh_builtins.hpp
	moosh.cpp
	script_visitor.hpp
)

target_include_directories(moosh PUBLIC ${MOOSH_INCLUDES})
target_link_libraries(moosh moove CONAN_PKG::boost)

add_executable(disasm
	msg_handler.cpp
	disasm.cpp
)

target_include_directories(disasm PUBLIC ${MOOSH_INCLUDES})
target_link_libraries(disasm moove CONAN_PKG::boost)
