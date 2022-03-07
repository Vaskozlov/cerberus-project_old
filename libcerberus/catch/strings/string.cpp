namespace cerb::debug
{
    auto testStringView() -> int;
    auto testStringPool() -> int;

    auto testStringModule() -> int
    {
        testStringView();
        testStringPool();
        return 0;
    }
}// namespace cerb::debug
