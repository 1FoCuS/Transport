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

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter)
{
    const auto [lhs, rhs_opt] = Parser::SplitTwoStrict(s, delimiter);
    return { lhs, rhs_opt.value_or("") };
}

std::string_view Parser::ReadToken(std::string_view& s, std::string_view delimiter)
{
    const auto [lhs, rhs] = Parser::SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}


// @todo optimisation read number
template <typename Number>
Number Parser::ReadNumber(std::istream& in_stream)
{
    assert(std::is_arithmetic<Number>::value);

    Number number;
    in_stream >> number;
    std::string value;
    std::getline(in_stream, value);

    return number;
}

RequestPtr Parser::ParseRequest(std::string_view str_request, Request::Mode mode)
{
    const auto type_request = CheckTypeRequest(str_request, mode);
    if (!type_request)
    {
        return nullptr;
    }
    auto request_ptr = Request::Create(type_request.value());
    if (request_ptr)
    {
        Parser::ReadToken(str_request);
        request_ptr->Parse(str_request);
    }

    return request_ptr;
}


