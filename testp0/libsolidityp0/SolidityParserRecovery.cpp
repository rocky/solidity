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

#include <fstream>
#include <testp0/libsolidityp0/Common.h>
#include <testp0/libsolidityp0/SyntaxTest.h>

using namespace std;
using namespace langutil;
using namespace dev::solidity::testparser;

namespace dev
{
namespace solidity
{
namespace testp0
{


BOOST_AUTO_TEST_SUITE(SolidityParserError)

BOOST_AUTO_TEST_CASE(solidity_parser_recovery)
{
	string const versionPragma = "pragma solidity >=0.0;\n";
	auto parser_error_tests = {
		"../../testp0/libsolidityp0/syntaxTests/parsing/errorRecovery/e1.sol",
		"../../testp0/libsolidityp0/syntaxTests/parsing/errorRecovery/e2_EOS.sol"
	};

	for (auto const& test_file: parser_error_tests)
	{
		SyntaxTest testCase = SyntaxTest(test_file);
		ifstream file(test_file);
		std::string m_source = testCase.parseSourceAndSettings(file);
		vector<SyntaxTestError> m_expectations = testCase.parseExpectations(file);
		bool status = testCase.run(cerr);
		BOOST_CHECK(status);
	}
}


BOOST_AUTO_TEST_SUITE_END()
}
}
} // end namespaces
