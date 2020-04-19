#include "parser.h"

pars_str_t Parser::ReadToken(std::string_view str, std::string_view delimiter)
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
