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

#include "Common.h"

#include <libdevcore/Assertions.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace dev
{
namespace testp0
{

/// If non-empty returns the value of the env. variable ETH_TEST_PATH, otherwise
/// it tries to find a path that contains the directories "libsolidityparse/syntaxTests"
/// and returns it if found.
/// The routine searches in the current directory, and inside the "test" directory
/// starting from the current directory and up to three levels up.
/// @returns the path of the first match or an empty path if not found.
boost::filesystem::path testPath()
{
	if (auto path = getenv("ETH_TEST_PATH"))
		return path;

	auto const searchPath =
	{
		fs::current_path() / ".." / ".." / ".." / "test",
		fs::current_path() / ".." / ".." / "test",
		fs::current_path() / ".." / "test",
		fs::current_path() / "test",
		fs::current_path()
	};
	for (auto const& basePath: searchPath)
	{
		fs::path syntaxTestPath = basePath / "libsolidityparser" / "syntaxTests";
		if (fs::exists(syntaxTestPath) && fs::is_directory(syntaxTestPath))
			return basePath;
	}
	return {};
}

CommonOptions::CommonOptions(std::string _caption):
	options(_caption,
		po::options_description::m_default_line_length,
		po::options_description::m_default_line_length - 23
	)
{
}

void CommonOptions::validate() const
{
	assertThrow(
		!testPath.empty(),
		ConfigException,
		"No test path specified. The --testpath argument must not be empty when given."
	);
	assertThrow(
		fs::exists(testPath),
		ConfigException,
		"Invalid test path specified."
	);
}

bool CommonOptions::parse(int argc, char const* const* argv)
{
	po::variables_map arguments;

	po::command_line_parser cmdLineParser(argc, argv);
	cmdLineParser.options(options);
	po::notify(arguments);
	return true;
}

}

}
