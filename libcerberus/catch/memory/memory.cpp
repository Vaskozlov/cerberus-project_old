namespace cerb::debug
{
    auto testFill() -> int;
    auto testEqual() -> int;
    auto testCopy() -> int;
    auto testPointerWrapper() -> int;
    auto testFind() -> int;

    auto memoryTest() -> int
    {
        testFill();
        testCopy();
        testEqual();
        testFind();
        testPointerWrapper();
        return 0;
    }
}// namespace cerb::debug
