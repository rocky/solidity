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


#pragma once

#include <string>
#include <memory>
#include <liblangutil/Scanner.h>
#include <libsolidityp0/parsing/Parser.h>
#include <liblangutil/ErrorReporter.h>
#include <testp0/Options.h>
#include <testp0/libsolidityp0/ErrorCheck.h>

#define CHECK_PARSE_ERROR(source, substring) \
do \
{\
	Error err = getError((source)); \
	BOOST_CHECK(searchErrorMessage(err, (substring))); \
}\
while(0)
