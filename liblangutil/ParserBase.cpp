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
 * @date 2016
 * Solidity parser shared functionality.
 */

#include <liblangutil/ParserBase.h>
#include <liblangutil/Scanner.h>
#include <liblangutil/ErrorReporter.h>

// Should this be moved somewhere more global
namespace fmt {

template< class ...Args >
std::string sprintf( const char * f, Args && ...args ) {
	int size = snprintf( nullptr, 0, f, args... );
	std::string res;
	res.resize( size );
	snprintf( & res[ 0 ], size + 1, f, args... );
	return res;
}

}

using namespace std;
using namespace langutil;

int ParserBase::position() const
{
	return m_scanner->currentLocation().start;
}

int ParserBase::endPosition() const
{
	return m_scanner->currentLocation().end;
}

Token ParserBase::currentToken() const
{
	return m_scanner->currentToken();
}

Token ParserBase::peekNextToken() const
{
	return m_scanner->peekNextToken();
}

std::string ParserBase::currentLiteral() const
{
	return m_scanner->currentLiteral();
}

Token ParserBase::advance()
{
	return m_scanner->next();
}

std::string ParserBase::tokenName(Token _token)
{
	if (_token == Token::Identifier)
		return string("identifier");
	else if (_token == Token::EOS)
		return string("end of source");
	else if (TokenTraits::isReservedKeyword(_token))
		return string("reserved keyword '") + TokenTraits::friendlyName(_token) + "'";
	else if (TokenTraits::isElementaryTypeName(_token)) //for the sake of accuracy in reporting
	{
		ElementaryTypeNameToken elemTypeName = m_scanner->currentElementaryTypeNameToken();
		return string("'") + elemTypeName.toString() + "'";
	}
	else
		return string("'") + TokenTraits::friendlyName(_token) + "'";
};

void ParserBase::expectToken(Token _value, bool _advance)
{
	Token tok = m_scanner->currentToken();
	if (tok != _value)
	{
		parserError(string("Expected ") + ParserBase::tokenName(_value) + string(" but got ") + ParserBase::tokenName(tok));
	}
	if (_advance)
		m_scanner->next();
}

void ParserBase::expectTokenOrConsumeUntil(Token _value, char const *_lhs, bool _advance)
{
	Token tok = m_scanner->currentToken();
	if (tok != _value)
	{
		Token token = m_scanner->currentToken();
		int startPosition = position();
		SourceLocation errorLoc = SourceLocation{startPosition, endPosition(), source()};
		while (token != _value && token != Token::EOS)
			token = m_scanner->next();
		std::string expectToken = ParserBase::tokenName(_value).c_str();
		std::string mess = fmt::sprintf("In <%s> expected %s but got %s.", _lhs, expectToken.c_str(), ParserBase::tokenName(tok).c_str());
		if (token == Token::EOS)
		{
			// rollback to where the token started, and raise exception to be caught at a higher level.
			m_scanner->seek(startPosition);
			fatalParserError(errorLoc, mess);
		}
		else
			parserError(errorLoc, mess + " Skipped to next " + expectToken + ".");
	}
	if (_advance)
		m_scanner->next();
}

void ParserBase::increaseRecursionDepth()
{
	m_recursionDepth++;
	if (m_recursionDepth >= 2560)
		fatalParserError("Maximum recursion depth reached during parsing.");
}

void ParserBase::decreaseRecursionDepth()
{
	solAssert(m_recursionDepth > 0, "");
	m_recursionDepth--;
}

void ParserBase::parserError(SourceLocation const& _location, string const& _description, bool _throw_error)
{
	m_errorReporter.parserError(_location, _description, _throw_error);
}

void ParserBase::parserError(string const& _description, bool _throw_error)
{
	m_errorReporter.parserError(SourceLocation{position(), endPosition(), source()}, _description, _throw_error);
}

void ParserBase::fatalParserError(string const& _description)
{
	m_errorReporter.fatalParserError(SourceLocation{position(), endPosition(), source()}, _description);
}

void ParserBase::fatalParserError(SourceLocation const& _location, string const& _description)
{
	m_errorReporter.fatalParserError(_location, _description);
}
