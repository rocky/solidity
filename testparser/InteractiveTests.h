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

#include <testparser/TestCase.h>

#include <boost/filesystem.hpp>

namespace dev
{
namespace solidity
{
namespace test
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
	Title                  Path            Subpath                SMT    IPC    Creator function */
	{"Semantic",            "libsolidity", "semanticTests",       false, true,  &SemanticTest::create},
};

}
}
}
