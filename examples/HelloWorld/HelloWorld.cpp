#include <Core/Log.hpp>

using namespace cobalt::core;

int main( int argc, char* argv[] )
{
    Log::debug( "Hello %s!", "World" );
    return 0;
}
