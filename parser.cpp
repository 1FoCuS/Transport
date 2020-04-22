#include "parser.h"

pars_str_t Parser::SplitTwoStrict(std::string_view str, std::string_view delimiter)
{
    const std::size_t pos = str.find(delimiter);

    if (pos == str.npos)
    {
        return std::make_pair(str, std::nullopt);
    }
    else
    {
        return std::make_pair(str.substr(0, pos), str.substr(pos + delimiter.length()));
    }
}

std::pair<std::string_view, std::string_view> Parser::SplitTwo(std::string_view s, std::string_view delimiter)
{
    const auto [lhs, rhs_opt] = Parser::SplitTwoStrict(s, delimiter);
    return std::make_pair(lhs, rhs_opt.value_or(""));
}

std::string_view Parser::ReadToken(std::string_view& s, std::string_view delimiter)
{
    const auto [lhs, rhs] = Parser::SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}

double Parser::ConvertToInt(std::string_view str)
{
    size_t pos;
    const int result = stoi(std::string(str), &pos);
//    if (pos != str.length())
//    {
//        std::stringstream error;
//        error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
//        throw std::invalid_argument(error.str());
//    }

    return result;
}

double Parser::ConvertToDouble(std::string_view str)
{
    size_t pos;
    const double result = stod(std::string(str), &pos);
//    if (pos != str.length())
//    {
//        std::stringstream error;
//        error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
//        throw std::invalid_argument(error.str());
//    }

    return result;
}
