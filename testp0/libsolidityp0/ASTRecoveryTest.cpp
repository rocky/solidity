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
#include <libdevcore/AnsiColorized.h>
#include <liblangutil/SourceReferenceFormatterHuman.h>
#include <libsolidityp0/ast/ASTJsonConverter.h>
#include <libsolidityp0/interface/CompilerStack.h>
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

ASTRecoveryTest::ASTRecoveryTest(string const& _filename)
{
	if (!boost::algorithm::ends_with(_filename, ".sol"))
		BOOST_THROW_EXCEPTION(runtime_error("Invalid test contract file name: \"" + _filename + "\"."));

	m_astFilename = _filename.substr(0, _filename.size() - 4) + ".json";

	ifstream file(_filename);
	if (!file)
		BOOST_THROW_EXCEPTION(runtime_error("Cannot open test contract: \"" + _filename + "\"."));
	file.exceptions(ios::badbit);

	string sourceName;
	string source;
	string line;
	string const sourceDelimiter("// ---- SOURCE: ");
	string const delimiter("// ----");
	while (getline(file, line))
	{
		if (boost::algorithm::starts_with(line, sourceDelimiter))
		{
			if (!sourceName.empty())
				m_sources.emplace_back(sourceName, source);

			sourceName = line.substr(sourceDelimiter.size(), string::npos);
			source = string();
		}
		else if (!line.empty() && !boost::algorithm::starts_with(line, delimiter))
			source += line + "\n";
	}

	m_sources.emplace_back(sourceName.empty() ? "a" : sourceName, source);

	file.close();
	file.open(m_astFilename);
	if (file)
	{
		string line;
		while (getline(file, line))
			m_expectation += line + "\n";
	}

	file.close();
}

TestCase::TestResult ASTRecoveryTest::run(ostream& _stream, string const& _linePrefix, bool _formatted)
{
	CompilerStack c;
	StringMap sources;
	map<string, unsigned> sourceIndices;
	for (size_t i = 0; i < m_sources.size(); i++)
	{
		sources[m_sources[i].first] = m_sources[i].second;
		sourceIndices[m_sources[i].first] = i + 1;
	}

	c.setSources(sources);
	c.setEVMVersion(dev::testp0::Options::get().evmVersion());
	c.setParserErrorRecovery(true);
	c.parse();
	c.analyze();

	for (size_t i = 0; i < m_sources.size(); i++)
	{
		ostringstream result;
		ASTJsonConverter(false, sourceIndices).print(result, c.ast(m_sources[i].first));
		m_result += result.str();
		if (i != m_sources.size() - 1)
			m_result += ",";
		m_result += "\n";
	}

	bool resultsMatch = true;

	if (m_expectation != m_result)
	{
		string nextIndentLevel = _linePrefix + "  ";
		AnsiColorized(_stream, _formatted, {BOLD, CYAN}) << _linePrefix << "Expected result:" << endl;
		{
			istringstream stream(m_expectation);
			string line;
			while (getline(stream, line))
				_stream << nextIndentLevel << line << endl;
		}
		_stream << endl;
		AnsiColorized(_stream, _formatted, {BOLD, CYAN}) << _linePrefix << "Obtained result:" << endl;
		{
			istringstream stream(m_result);
			string line;
			while (getline(stream, line))
				_stream << nextIndentLevel << line << endl;
		}
		_stream << endl;
		resultsMatch = false;
	}

	return resultsMatch ? TestResult::Success : TestResult::Failure;
}

void ASTRecoveryTest::printSource(ostream& _stream, string const& _linePrefix, bool _formatted) const
{
	// To be determined...
	(void) _stream;
	(void) _linePrefix;
	(void) _formatted;
	return;
}
