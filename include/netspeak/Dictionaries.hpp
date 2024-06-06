#ifndef NETSPEAK_DICTIONARIES_HPP
#define NETSPEAK_DICTIONARIES_HPP

#include <filesystem>
#include <istream>
#include <string>
#include <unordered_map>

/** \todo Rename Dictionaries namespace to something lowercase to fulfill naming conventions **/
namespace netspeak::Dictionaries {

/**
 * A class to represent a dictionary. A dictionary is a mapping from keys to
 * a list of values. Both keys and values have the type std::string. The text
 * file format of a dictionary is defined as follows:
 *
 * line = key TAB value0 TAB value1 TAB value2 ...
 *
 * Whereas whitespaces within keys and/or values are allowed.
 */
using Map = std::unordered_multimap<std::string, std::string>;

const Map read_from_file(const std::filesystem::path& csv);
const Map parse_csv(std::istream& stream);
} // namespace netspeak::Dictionaries

#endif // NETSPEAK_DICTIONARIES_HPP
