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
/** @file boostTest.cpp
 * @author Marko Simovic <markobarko@gmail.com>
 * @date 2014
 * Stub for generating main boost.test module.
 * Original code taken from boost sources.
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4535) // calling _set_se_translator requires /EHa
#endif
#include <boost/test/unit_test.hpp>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#pragma GCC diagnostic pop

#include <testparser/TestCase.h>
#include <testparser/Options.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <string>

using namespace boost::unit_test;
namespace fs = boost::filesystem;
using namespace std;

namespace
{
}

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
	master_test_suite_t& master = framework::master_test_suite();
	master.p_name.value = "SolidityParseTests";
	return 0;
}

// BOOST_TEST_DYN_LINK should be defined if user want to link against shared boost test library
#ifdef BOOST_TEST_DYN_LINK

// Because we want to have customized initialization function and support shared boost libraries at the same time,
// we are forced to customize the entry point.
// see: https://www.boost.org/doc/libs/1_67_0/libs/test/doc/html/boost_test/adv_scenarios/shared_lib_customizations/init_func.html

int main(int argc, char* argv[])
{
	auto init_unit_test = []() -> bool { init_unit_test_suite(0, nullptr); return true; };
	return boost::unit_test::unit_test_main(init_unit_test, argc, argv);
}

#endif
