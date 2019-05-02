set(sources
	ast/AST.cpp
	ast/AST.h
	ast/AST_accept.h
	ast/ASTAnnotations.cpp
	ast/ASTAnnotations.h
	ast/ASTEnums.h
	ast/ASTForward.h
	ast/ASTJsonConverter.cpp
	ast/ASTJsonConverter.h
	ast/ASTVisitor.h
	ast/ExperimentalFeatures.h
	ast/Types.cpp
	ast/Types.h
	ast/TypeProvider.cpp
	ast/TypeProvider.h
	interface/CompilerStack.cpp
	interface/CompilerStack.h
	interface/ReadFile.h
	interface/StandardCompiler.h
	interface/Version.cpp
	interface/Version.h
	parsing/DocStringParser.cpp
	parsing/DocStringParser.h
	parsing/Parser.cpp
	parsing/Parser.h
	parsing/Token.h
)

add_library(solidityp0 ${sources})
target_link_libraries(solidityp0 PUBLIC langutil devcore ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
