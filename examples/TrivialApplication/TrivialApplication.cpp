#include <Platform/Application.hpp>
#include <Core/Log.hpp>

#include <GLFW/glfw3.h>

using namespace cobalt::core;
using namespace cobalt::platform;


class TrivialApplication : public Application
{
public:
    TrivialApplication() {}
protected:
    virtual void configure( WindowConfiguration& config ) override;
    virtual void startup() override;
    virtual void update( double dt ) override;
    virtual void shutdown() override;
};

void TrivialApplication::configure( WindowConfiguration& config )
{
    config.name = "Trivial Application";
    config.width = 1024;
    config.height = 768;
}

void TrivialApplication::startup()
{
    glClearColor( 0.15, 0.15, 0.4, 0.0 );
}

void TrivialApplication::update( double dt )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    Log::info( "%2.4g seconds since last onUpdate()", dt );
}

void TrivialApplication::shutdown()
{

}

/////////////////////////////////////////////////
// Cobalt platform


///////////////////////////////

int main( int argc, char* argv[] )
{
    Log::info( "Application initializing." );
    TrivialApplication app;
    launchCobaltApplication( &app );
    return 0;
}

