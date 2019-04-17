/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Christian <c@ethdev.com>
 * @date 2014
 * Unit tests for the solidity parser.
 */

#include <testp0/libsolidityp0/Common.h>

using namespace std;
using namespace langutil;

namespace dev
{
namespace solidity
{
namespace testp0
{

namespace
{
ASTPointer<ContractDefinition> parseText(std::string const& _source, ErrorList& _errors)
{
	ErrorReporter errorReporter(_errors);
	ASTPointer<SourceUnit> sourceUnit = Parser(errorReporter).parse(std::make_shared<Scanner>(CharStream(_source, "")));
	if (!sourceUnit)
		return ASTPointer<ContractDefinition>();
	for (ASTPointer<ASTNode> const& node: sourceUnit->nodes())
		if (ASTPointer<ContractDefinition> contract = dynamic_pointer_cast<ContractDefinition>(node))
			return contract;
	BOOST_FAIL("No contract found in source.");
	return ASTPointer<ContractDefinition>();
}

Error getError(std::string const& _source)
{
	ErrorList errors;
	try
	{
		parseText(_source, errors);
	}
	catch (FatalError const& /*_exception*/)
	{
		// no-op
	}
	Error const* error = Error::containsErrorOfType(errors, Error::Type::ParserError);
	BOOST_REQUIRE(error);
	return *error;
}

}

BOOST_AUTO_TEST_SUITE(SolidityParserError)

BOOST_AUTO_TEST_CASE(semicolon_error)
{
	char const* text = R"(
contract Five {
  function five() returns(int) {
    uint256 a;
    a = 1
      a = 3;
      return 5;
    }
}
	)";
	CHECK_PARSE_ERROR(text, "Expected ';' but got identifier.");
}


BOOST_AUTO_TEST_CASE(semicolon_error_to_EOS)
{
	char const* text = R"(
contract Five {
  function five() returns(int) {
    uint256 a;
    a = 1
	)";
	CHECK_PARSE_ERROR(text, "Expected ';' but got end of source.");
}


BOOST_AUTO_TEST_SUITE_END()
}
}
} // end namespaces
