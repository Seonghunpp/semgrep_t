basic_json parse(IteratorType first,
                 IteratorType last,
                 const parser_callback_t cb = nullptr,
                 const bool allow_exceptions = true,
                 const bool ignore_comments = false)
{
    basic_json result;
    parser(detail::input_adapter(std::move(first), std::move(last)),
           cb,
           allow_exceptions,
           ignore_comments).parse(true, result);
    return result;
}
