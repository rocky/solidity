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

#include <testp0/libsolidityp0/Common.h>
#include <testp0/libsolidityp0/SyntaxTest.h>
#include <testp0/Options.h>
#include <testp0/TestCase.h>
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

namespace
{

int parseUnsignedInteger(string::iterator& _it, string::iterator _end)
{
	if (_it == _end || !isdigit(*_it))
		throw runtime_error("Invalid test expectation. Source location expected.");
	int result = 0;
	while (_it != _end && isdigit(*_it))
	{
		result *= 10;
		result += *_it - '0';
		++_it;
	}
	return result;
}

// Copied from SolidityParser.cpp
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

}

// Duplicated from TestCase.cpp
string SyntaxTest::parseSourceAndSettings(istream& _stream)
{
	string source;
	string line;
	static string const settingsDelimiter("// ====");
	static string const delimiter("// ----");
	bool sourcePart = true;
	while (getline(_stream, line))
	{
		if (boost::algorithm::starts_with(line, delimiter))
			break;
		else if (boost::algorithm::starts_with(line, settingsDelimiter))
			sourcePart = false;
		else if (sourcePart)
			source += line + "\n";
		else if (boost::algorithm::starts_with(line, "// "))
		{
			size_t colon = line.find(':');
			if (colon == string::npos)
				throw runtime_error(string("Expected \":\" inside setting."));
			string key = line.substr(3, colon - 3);
			string value = line.substr(colon + 1);
			boost::algorithm::trim(key);
			boost::algorithm::trim(value);
			m_settings[key] = value;
		}
		else
			throw runtime_error(string("Expected \"//\" or \"// ---\" to terminate settings and source."));
	}
	return source;
}

SyntaxTest::SyntaxTest(string const& _filename)
{
	ifstream file(_filename);
	if (!file)
		BOOST_THROW_EXCEPTION(runtime_error("Cannot open test contract: \"" + _filename + "\"."));
	file.exceptions(ios::badbit);

	m_source = parseSourceAndSettings(file);
	m_expectations = parseExpectations(file);
}

bool SyntaxTest::run(ostream& _stream, string const& _linePrefix, bool _formatted)
{
	string const versionPragma = "pragma solidity >=0.0;\n";

	// rocky: requires "AnalysisFrameWork which we don't have
	// m_compiler.reset();
	// m_compiler.setSources({{"", versionPragma + m_source}});
	// m_compiler.setEVMVersion(m_evmVersion);

	// if (m_compiler.parse())
	//	m_compiler.analyze();

	// The above code is replaced below try/catch.
	ErrorList errors;
	try
	{
		parseText(m_source, errors);
	}
	catch (FatalError const& /*_exception*/)
	{
		// no-op
	}


	// for (auto const& currentError: filterErrors(m_compiler.errors(), true))
	for (auto const& currentError: errors)
	{
		int locationStart = -1, locationEnd = -1;
		if (auto location = boost::get_error_info<errinfo_sourceLocation>(*currentError))
		{
			// ignore the version pragma inserted by the testing tool when calculating locations.
			if (location->start >= static_cast<int>(versionPragma.size()))
				locationStart = location->start - versionPragma.size();
			if (location->end >= static_cast<int>(versionPragma.size()))
				locationEnd = location->end - versionPragma.size();
		}
		m_errorList.emplace_back(SyntaxTestError{
			currentError->typeName(),
			errorMessage(*currentError),
			locationStart,
			locationEnd
		});
	}

	return printExpectationAndError(_stream, _linePrefix, _formatted);
}

bool SyntaxTest::printExpectationAndError(ostream& _stream, string const& _linePrefix, bool _formatted)
{
	if (m_expectations != m_errorList)
	{
		string nextIndentLevel = _linePrefix + "  ";
		AnsiColorized(_stream, _formatted, {BOLD, CYAN}) << _linePrefix << "Expected result:" << endl;
		printErrorList(_stream, m_expectations, nextIndentLevel, _formatted);
		AnsiColorized(_stream, _formatted, {BOLD, CYAN}) << _linePrefix << "Obtained result:" << endl;
		printErrorList(_stream, m_errorList, nextIndentLevel, _formatted);
		return false;
	}
	return true;
}

void SyntaxTest::printSource(ostream& _stream, string const& _linePrefix, bool _formatted) const
{
	if (_formatted)
	{
		if (m_source.empty())
			return;

		vector<char const*> sourceFormatting(m_source.length(), formatting::RESET);
		for (auto const& error: m_errorList)
			if (error.locationStart >= 0 && error.locationEnd >= 0)
			{
				assert(static_cast<size_t>(error.locationStart) <= m_source.length());
				assert(static_cast<size_t>(error.locationEnd) <= m_source.length());
				bool isWarning = error.type == "Warning";
				for (int i = error.locationStart; i < error.locationEnd; i++)
					if (isWarning)
					{
						if (sourceFormatting[i] == formatting::RESET)
							sourceFormatting[i] = formatting::ORANGE_BACKGROUND_256;
					}
					else
						sourceFormatting[i] = formatting::RED_BACKGROUND;
			}

		_stream << _linePrefix << sourceFormatting.front() << m_source.front();
		for (size_t i = 1; i < m_source.length(); i++)
		{
			if (sourceFormatting[i] != sourceFormatting[i - 1])
				_stream << sourceFormatting[i];
			if (m_source[i] != '\n')
				_stream << m_source[i];
			else
			{
				_stream << formatting::RESET << endl;
				if (i + 1 < m_source.length())
					_stream << _linePrefix << sourceFormatting[i];
			}
		}
		_stream << formatting::RESET;
	}
	else
	{
		stringstream stream(m_source);
		string line;
		while (getline(stream, line))
			_stream << _linePrefix << line << endl;
	}
}

void SyntaxTest::printErrorList(
	ostream& _stream,
	vector<SyntaxTestError> const& _errorList,
	string const& _linePrefix,
	bool _formatted
)
{
	if (_errorList.empty())
		AnsiColorized(_stream, _formatted, {BOLD, GREEN}) << _linePrefix << "Success" << endl;
	else
		for (auto const& error: _errorList)
		{
			{
				AnsiColorized scope(_stream, _formatted, {BOLD, (error.type == "Warning") ? YELLOW : RED});
				_stream << _linePrefix;
				_stream << error.type << ": ";
			}
			if (error.locationStart >= 0 || error.locationEnd >= 0)
			{
				_stream << "(";
				if (error.locationStart >= 0)
					_stream << error.locationStart;
				_stream << "-";
				if (error.locationEnd >= 0)
					_stream << error.locationEnd;
				_stream << "): ";
			}
			_stream << error.message << endl;
		}
}

string SyntaxTest::errorMessage(Exception const& _e)
{
	if (_e.comment() && !_e.comment()->empty())
		return boost::replace_all_copy(*_e.comment(), "\n", "\\n");
	else
		return "NONE";
}

vector<SyntaxTestError> SyntaxTest::parseExpectations(istream& _stream)
{
	vector<SyntaxTestError> expectations;
	string line;
	while (getline(_stream, line))
	{
		auto it = line.begin();

		skipSlashes(it, line.end());
		skipWhitespace(it, line.end());

		if (it == line.end()) continue;

		auto typeBegin = it;
		while (it != line.end() && *it != ':')
			++it;
		string errorType(typeBegin, it);

		// skip colon
		if (it != line.end()) it++;

		skipWhitespace(it, line.end());

		int locationStart = -1;
		int locationEnd = -1;

		if (it != line.end() && *it == '(')
		{
			++it;
			locationStart = parseUnsignedInteger(it, line.end());
			expect(it, line.end(), '-');
			locationEnd = parseUnsignedInteger(it, line.end());
			expect(it, line.end(), ')');
			expect(it, line.end(), ':');
		}

		skipWhitespace(it, line.end());

		string errorMessage(it, line.end());
		expectations.emplace_back(SyntaxTestError{
			move(errorType),
			move(errorMessage),
			locationStart,
			locationEnd
		});
	}
	return expectations;
}

void SyntaxTest::expect(string::iterator& _it, string::iterator _end, string::value_type _c)
{
	if (_it == _end || *_it != _c)
		throw runtime_error(string("Invalid test expectation. Expected: \"") + _c + "\".");
	++_it;
}
