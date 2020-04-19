#ifndef PARSER_H
#define PARSER_H

#include <string_view>
#include <optional>

using pars_str_t = std::pair<std::string_view, std::optional<std::string_view>> ;
class Parser
{
public:
    static pars_str_t ReadToken(std::string_view str, std::string_view delimiter);
};

#endif // PARSER_H
