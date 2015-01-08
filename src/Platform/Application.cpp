#include <Platform/Application.hpp>
#include <Core/Log.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef COBALT_EMSCRIPTEN
    #include <emscripten/emscripten.h>
#endif

namespace cobalt { namespace platform {

using namespace core;
    
    Application::Application()
    {
    }
    
    void Application::onStartup()
    {
        // Allow subclasses to configure settings
        WindowConfiguration config;
        configure( config );
        
        if( !glfwInit() )
        {
            return;
        }
        if( config.isMaximized )
        {
            const GLFWvidmode* videoMode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
            config.width = videoMode->width;
            config.height = videoMode->height;
        }
        
        GLFWmonitor* targetMonitor = nullptr;
        if( config.isFullscreen )
        {
            targetMonitor = glfwGetPrimaryMonitor();
        }
        mWindow = glfwCreateWindow( config.width, config.height, config.name, targetMonitor, nullptr );
        if( !mWindow )
        {
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent( mWindow );
        glewExperimental = GL_TRUE;
        glewInit();
        
        int width, height;
        getFrameBufferSize( width, height );
        glViewport( 0, 0, width, height );
        
        glfwSetFramebufferSizeCallback( mWindow, Application::framebufferResizeCallback );
        
        // Allow subclasses to startup
        startup();
    }

    void Application::onUpdate( double dt )
    {
        if( glfwWindowShouldClose( mWindow ) )
        {
            mShouldUpdate = false;
            return;
        }
        update( dt );

        glfwSwapBuffers( mWindow );
        glfwPollEvents();
    }

    void Application::onShutdown()
    {
        shutdown();
        glfwTerminate();
    }
    
    //// Services
    
    void Application::getFrameBufferSize( int& outWidth, int& outHeight )
    {
        glfwGetFramebufferSize( mWindow, &outWidth, &outHeight );
    }


    void Application::framebufferResizeCallback( GLFWwindow* window, int width, int height )
    {
        glViewport(0, 0, width, height);
    }

namespace impl {
    // Pointer to the single app used by Cobalt
    Application* gblApp;
    
    void gblUpdate()
    {
        static double lastFrameUpdateTime = glfwGetTime();
        double currentFrameUpdateTime = glfwGetTime();
        double dt = currentFrameUpdateTime - lastFrameUpdateTime;
        lastFrameUpdateTime = currentFrameUpdateTime;
        if( gblApp )
        {
            gblApp->onUpdate( dt );
        }
        double elapsedFrameUpdateTime = glfwGetTime() - currentFrameUpdateTime;
        if( elapsedFrameUpdateTime > 0.16 )
        {
            Log::error( "Application::OnUpdate took too long at %1.4g ms", elapsedFrameUpdateTime * 0.001 );
        }
        // emscripten doesn't need to swap buffers, does desktop version?
        //glfwSwapBuffers();
    }
}
    
void launchCobaltApplication( Application* app )
{
    using namespace impl;
    gblApp = app;
    
    Log::info( "Application starting." );
    gblApp->onStartup();
    Log::info( "Application startup complete." );
    
#ifdef COBALT_EMSCRIPTEN
    Log::info( "Passing update main loop to emscripten" );
    emscripten_set_main_loop( gblUpdate, 60 /*fps*/, 1 /*infinite loop*/ );
    // Never reach here?
#else
    Log::info( "Starting update main loop" );
    while( gblApp->shouldUpdate() ) { gblUpdate(); };
#endif
    
    Log::info( "Application shutting down." );
    gblApp->onShutdown();
    Log::info( "Application shut down." );
}
    
} }
