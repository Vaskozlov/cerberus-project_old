namespace cerb::debug
{
    auto testFill() -> int;
    auto testEqual() -> int;
    auto testCopy() -> int;
    auto testPointerWrapper() -> int;
    auto testStringOperations() -> int;

    auto memoryTest() -> int
    {
        testFill();
        testCopy();
        testEqual();
        testStringOperations();
        testPointerWrapper();
        return 0;
    }
}// namespace cerb::debug
