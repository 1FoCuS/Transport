#ifndef PARSER_H
#define PARSER_H

#include <string_view>
#include <optional>
#include "request.h"

using pars_str_t = std::pair<std::string_view, std::optional<std::string_view>>;

class Parser
{

public:
    static pars_str_t SplitTwoStrict(std::string_view str, std::string_view delimiter = " ");
    static std::string_view ReadToken(std::string_view&, std::string_view = " ");
    static std::pair<std::string_view, std::string_view> SplitTwo(std::string_view, std::string_view = " ");

    template <typename Number>
    static Number ReadNumber(std::istream&);

    static RequestPtr ParseRequest(std::string_view, Request::Mode);
};

#endif // PARSER_H
