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

#pragma once

#include <testp0/TestCase.h>
#include <testp0/libsolidityp0/SyntaxTest.h>
#include <testp0/libsolidityp0/ASTRecoveryTest.h>

#include <boost/filesystem.hpp>

namespace dev
{
namespace solidity
{
namespace testparser
{

/** Container for all information regarding a testsuite */
struct Testsuite
{
	char const* title;
	boost::filesystem::path const path;
	boost::filesystem::path const subpath;
	bool smt;
	bool ipc;
	TestCase::TestCaseCreator testCaseCreator;
};


/// Array of testsuits that can be run interactively as well as automatically
Testsuite const g_interactiveTestsuites[] = {
/*
	Title                  Path              Subpath                SMT    IPC    Creator function */
	{"Error Recovery",      "libsolidityp0", "errorRecoveryTests",  false, false, &SyntaxTest::createErrorRecovery},
	{"JSON AST Recovery",   "libsolidityp0", "ASTRecoveryTests",    false, false, &ASTRecoveryTest::create},
};

}
}
}
