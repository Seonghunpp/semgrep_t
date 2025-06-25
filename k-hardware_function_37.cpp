void strlcpy_from_user(char* buf, vmiter it, size_t maxlen) {
    size_t i = 0;
    while (i + 1 < maxlen && it.user()) {
        buf[i] = *it.kptr<const char*>();
        ++i, ++it;
    }
    if (i < maxlen) {
        buf[i] = '\0';
    }
}
