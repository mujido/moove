project(moove)

bison_target(
	parser moove.y 
	${CMAKE_CURRENT_BINARY_DIR}/moove.tab.cpp 
	DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/moove.tab.h
)

FLEX_TARGET(lexer
	moove_lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/moove_lexer.l.cpp
	DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/moove_lexer.l.h
)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

set(INCLUDE_FILES
	ast_visitor.hpp
	basic_op_pkgs.hpp
	bc_program.hpp
	builtin_registry.hpp
	ci_string.hpp
	codevect.hpp
	compare.hpp
	compiler.hpp
	default_ops.hpp
	deref.hpp
	disassembler.hpp
	dynamic_array.hpp
	errorvar.hpp
	except.hpp
	exec_state.hpp
	expr_ast.hpp
	flow_ctrl.hpp
	gen-accepts.sh
	highlight.hpp
	immediate.hpp
	interpreter.hpp
	intvar.hpp
	label_linker.hpp
	listvar.hpp
	marray.hpp
	memory.hpp
	op_map.hpp
	op_package.hpp
	op_wrappers.hpp
	opcodes.hpp
	parser.hpp
	parser_msgs.hpp
	parser_state.hpp
	partial_ast_visitor.hpp
	program_ast.hpp
	realvar.hpp
	refcount.hpp
	reply.hpp
	stmt_ast.hpp
	string_utils.hpp
	strvar.hpp
	symbol_table.hpp
	task.hpp
	task_frame.hpp
	type_registry.hpp
	value_stack.hpp
	variant.hpp
	variant_factory.hpp
	variant_id.hpp
	vm.hpp
)

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_SOURCE_DIR}/include/)

set(SOURCE_FILES
    reply.cpp 
    type_registry.cpp 
    intvar.cpp 
    realvar.cpp 
    strvar.cpp 
    op_map.cpp 
    op_package.cpp 
    listvar.cpp 
    compiler.cpp 
    disassembler.cpp 
    codevect.cpp 
    opcodes.cpp 
    symbol_table.cpp 
    bc_program.cpp 
    string_utils.cpp 
    label_linker.cpp 
    immediate.cpp 
    flow_ctrl.cpp 
    interpreter.cpp 
    exec_state.cpp 
    task_frame.cpp 
    builtin_registry.cpp 
    parser_state.cpp 
    expr_ast.cpp 
    stmt_ast.cpp 
	parser.cpp   
	lexer_helpers.cpp
)

add_library(moove 
	${INCLUDE_FILES} 
	${SOURCE_FILES} 
	moove.y 
	moove_lexer.l 
	${BISON_parser_OUTPUTS} 
	${FLEX_lexer_OUTPUTS}
)

source_group("schema" FILES moove.y moove_lexer.l)

target_include_directories(moove 
	PUBLIC ${CMAKE_SOURCE_DIR}/include 
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(moove CONAN_PKG::boost)
