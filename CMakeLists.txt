set(LLVM_LINK_COMPONENTS support)

add_clang_executable(func-name FuncNames.cpp)

target_link_libraries(func-name
	PRIVATE
	clangTooling
	clangBasic
	clangASTMatchers)
