#pragma once

#define cobalt_assert( cond ) do{ if(!(cond)) { cobalt::Core::Log::assert( #cond, nullptr, __FILE__, __LINE__, __PRETTY_FUNCTION__);  } } while(false)
#define cobalt_assert_msg( cond, msg ) do{ if(!(cond)) { cobalt::Core::Log::assert( #cond, #msg, __FILE__, __LINE__, __PRETTY_FUNCTION__);  } } while(false)

namespace cobalt
{
namespace core
{

    class Log
    {
    public:
        enum class Level { Debug, Info, Warn, Error, Fatal };
        static Level sMinLogLevel;

        static void log( Level level, const char* msg, ... );

        static void debug( const char* msg, ... );
        static void info( const char* msg, ... );
        static void warn( const char* msg, ... );
        static void error( const char* msg, ... );
        static void fatal( const char* msg, ... );
    private:
        static void assert( const char* condition, const char* message, const char* file, int line, const char* function );

    };
}
}

