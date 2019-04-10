set(sources
	ast/AST_accept.h
	ast/ASTAnnotations.cpp
	ast/ASTAnnotations.h
	ast/AST.cpp
	ast/ASTEnums.h
	ast/ASTForward.h
	ast/AST.h
	ast/ASTJsonConverter.cpp
	ast/ASTJsonConverter.h
	ast/ASTVisitor.h
	ast/ExperimentalFeatures.h
	ast/Types.cpp
	ast/Types.h
	parsing/DocStringParser.cpp
	parsing/DocStringParser.h
	parsing/Parser.cpp
	parsing/Parser.h
	parsing/SemVerHandler.cpp
	parsing/SemVerHandler.h
	parsing/Token.h
	parsing/Version.cpp
	parsing/Version.h
)

add_library(solidityp0 ${sources})
target_link_libraries(solidityp0 PUBLIC langutil devcore ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
