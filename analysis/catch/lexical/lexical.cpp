#include "char.hpp"
#include "location.hpp"
#include "token.hpp"

auto main() -> int
{
    using namespace cerb::test;

    lexicalCharTest();
    txtManagerTest();
    tokenTest();

    return 0;
}