void apply_json_patch(const basic_json& json_patch, basic_json& result)
{
    for (const auto& val : json_patch)
    {
        // wrapper to get a value for an operation
        const auto get_value = [&val](const std::string& op,
                                      const std::string& member,
                                      bool string_type) -> basic_json&
        {
            auto it = val.m_value.object->find(member);

            const auto error_msg = (op == "op") ? "operation"
                                                : detail::concat("operation '", op, '\'');

            if (JSON_HEDLEY_UNLIKELY(it == val.m_value.object->end()))
            {
                JSON_THROW(parse_error::create(
                    105, 0,
                    detail::concat(error_msg, " must have member '", member, "'"),
                    &val));
            }

            if (JSON_HEDLEY_UNLIKELY(string_type && !it->second.is_string()))
            {
                JSON_THROW(parse_error::create(
                    105, 0,
                    detail::concat(error_msg, " must have string member '", member, "'"),
                    &val));
            }

            return it->second;
        };

        // check type
        if (JSON_HEDLEY_UNLIKELY(!val.is_object()))
        {
            JSON_THROW(parse_error::create(104, 0,
                                           "JSON patch must be an array of objects",
                                           &val));
        }

        const auto op = get_value("op", "op", true).template get<std::string>();
        const auto path = get_value(op, "path", true).template get<std::string>();
        json_pointer ptr(path);

        switch (get_op(op))
        {
            case patch_operations::add:
            {
                operation_add(ptr, get_value("add", "value", false));
                break;
            }

            case patch_operations::remove:
            {
                operation_remove(ptr);
                break;
            }

            case patch_operations::replace:
            {
                result.at(ptr) = get_value("replace", "value", false);
                break;
            }

            case patch_operations::move:
            {
                const auto from_path = get_value("move", "from", true).template get<std::string>();
                json_pointer from_ptr(from_path);
                basic_json v = result.at(from_ptr);
                operation_remove(from_ptr);
                operation_add(ptr, v);
                break;
            }

            case patch_operations::copy:
            {
                const auto from_path = get_value("copy", "from", true).template get<std::string>();
                json_pointer from_ptr(from_path);
                basic_json v = result.at(from_ptr);
                operation_add(ptr, v);
                break;
            }

            case patch_operations::test:
            {
                bool success = false;
                JSON_TRY
                {
                    success = (result.at(ptr) == get_value("test", "value", false));
                }
                JSON_INTERNAL_CATCH(out_of_range&)
                {
                    // do nothing, success remains false
                }

                if (JSON_HEDLEY_UNLIKELY(!success))
                {
                    JSON_THROW(other_error::create(
                        501, detail::concat("unsuccessful: ", val.dump()), &val));
                }

                break;
            }

            case patch_operations::invalid:
            default:
            {
                JSON_THROW(parse_error::create(
                    105, 0,
                    detail::concat("operation value '", op, "' is invalid"),
                    &val));
            }
        }
    }
}
