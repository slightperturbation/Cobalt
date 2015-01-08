#include <cstdio>
#include <cstdarg>

#include <Core/Log.hpp>

namespace cobalt {
using namespace core;

Log::Level Log::sMinLogLevel = Level::Debug;

void Log::assert( const char *condition, const char *message, const char *file, int line, const char *function )
{
    std::printf( "Cobalt ASSERT: (%s)\\n\"%s\", %s:%d in %s\n", condition, message, file, line, function );
}

void Log::log( Level level, const char *msg, ... )
{
    if( sMinLogLevel <= level )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

void Log::debug( const char *msg, ... )
{
    if( sMinLogLevel <= Log::Level::Debug )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

void Log::info( const char *msg, ... )
{
    if( sMinLogLevel <= Log::Level::Info )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

void Log::warn( const char *msg, ... )
{
    if( sMinLogLevel <= Log::Level::Warn )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

void Log::error( const char *msg, ... )
{
    if( sMinLogLevel <= Log::Level::Error )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

void Log::fatal( const char *msg, ... )
{
    if( sMinLogLevel <= Log::Level::Fatal )
    {
        va_list args;
        va_start( args, msg );
        std::vprintf( msg, args );
        std::printf( "\n" );
        va_end( args );
    }
}

}
