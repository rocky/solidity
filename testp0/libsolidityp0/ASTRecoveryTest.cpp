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

#include <testp0/libsolidityp0/ASTRecoveryTest.h>
#include <testp0/Options.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/throw_exception.hpp>
#include <fstream>
#include <memory>
#include <stdexcept>

using namespace langutil;
using namespace dev::solidity;
using namespace dev::solidity::testparser;
using namespace dev::formatting;
using namespace dev;
using namespace std;
namespace fs = boost::filesystem;
using namespace boost::unit_test;

ASTRecoveryTest::ASTRecoveryTest(string const& _filename, langutil::EVMVersion _evmVersion): m_evmVersion(_evmVersion)
{
	ifstream file(_filename);
	if (!file)
		BOOST_THROW_EXCEPTION(runtime_error("Cannot open test contract: \"" + _filename + "\"."));
	file.exceptions(ios::badbit);

	m_source = parseSourceAndSettings(file);
	m_parserErrorRecovery = true;
}

TestCase::TestResult ASTRecoveryTest::run(ostream& _stream, string const& _linePrefix, bool _formatted)
{
	string const versionPragma = "pragma solidity >=0.0;\n";
	compiler().reset();
	compiler().setSources({{"", versionPragma + m_source}});
	compiler().setEVMVersion(m_evmVersion);
	compiler().setParserErrorRecovery(m_parserErrorRecovery);
	compiler().parse();
	compiler().analyze();

	// For now we just want to make sure we don't SEGV
	(void) _stream;
	(void) _linePrefix;
	(void) _formatted;
	return TestResult::Success;
	// return printExpectationAndError(bool _success,  _stream, _linePrefix, _formatted) ? TestResult::Success : TestResult::Failure;
}

// bool ASTRecoveryTest::printExpectationAndError(bool _success, ostream& _stream, string const& _linePrefix, bool _formatted)
// {
// 	if (!_success)
// 	{
// 		string nextIndentLevel = _linePrefix + "  ";
// 		AnsiColorized(_stream, _formatted, {BOLD, CYAN}) << _linePrefix << "Something went wrong" << endl;
// 		return false;
// 	}
// 	return true;
// }


void ASTRecoveryTest::printSource(ostream& _stream, string const& _linePrefix, bool _formatted) const
{
	// To be determined...
	(void) _stream;
	(void) _linePrefix;
	(void) _formatted;
	return;
}
