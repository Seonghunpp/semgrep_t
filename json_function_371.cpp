iterator insert(const_iterator pos, const basic_json& val)
{
    // insert only works for arrays
    if (JSON_HEDLEY_LIKELY(is_array()))
    {
        // check if iterator pos fits to this JSON value
        if (JSON_HEDLEY_UNLIKELY(pos.m_object != this))
        {
            JSON_THROW(invalid_iterator::create(202, "iterator does not fit current value", this));
        }

        // insert to array and return iterator
        return insert_iterator(pos, val);
    }

    JSON_THROW(type_error::create(309, detail::concat("cannot use insert() with ", type_name()), this));
}
