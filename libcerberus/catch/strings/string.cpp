namespace cerb::debug
{
    auto testStringView() -> int;
    auto testStringPool() -> int;
    auto testHashString() -> int;

    auto testStringModule() -> int
    {
        testStringView();
        testStringPool();
        testHashString();
        return 0;
    }
}// namespace cerb::debug
