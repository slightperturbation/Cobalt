#include <Core/Log.hpp>

using namespace cobalt::core;

// Forward declaration to avoid including GLFW
struct GLFWwindow;

namespace cobalt { namespace platform {

struct WindowConfiguration
{
    int width = 640;
    int height = 480;
    const char* name = "Hello World";
    bool isFullscreen = false;
    bool isMaximized = false; // overrides width and height
//    ColorFormat
};

    
/// Colbalt applications should subclass the Application
///
/// Example Usage:
/// class TrivialApplication : public Application
/// {
/// public:
///     TrivialApplication() {}
/// protected:
///     virtual void startup() override;
///     virtual void update( double dt ) override;
///     virtual void shutdown() override;
/// };
///
/// /sa launchCobaltApplication(Application*)
class Application
{
public:
    Application();
    virtual ~Application() {}

    virtual void onStartup();
    virtual void onUpdate( double dt );
    virtual void onShutdown();

    bool shouldUpdate() const { return mShouldUpdate; }
    
protected:
    // Cobalt applications should override these methods
    virtual void configure( WindowConfiguration& config ) = 0;
    virtual void startup() = 0;
    virtual void update( double dt ) = 0;
    virtual void shutdown() = 0;
    
protected:
    // services for Application subclases
    void getFrameBufferSize( int& outWidth, int& outHeight );
private:
    bool mShouldUpdate = true;
    GLFWwindow* mWindow;
    
    // GLFW Callbacks
    static void framebufferResizeCallback( GLFWwindow* window, int width, int height );

//    std::unique_ptr< WindowManager > windowManager;
//    std::unique_ptr< DisplayManager > displayManager;
//    std::unique_ptr< Renderer > renderer;
//    std::unique_ptr< ResourceManager > resourceManager;
//    std::unique_ptr< ResourceLocator > resourceLocator;
};
    
/// Start cobalt
/// Launches the application.  There can be only one.
///
/// Typical usage:
/// int main( int argc, char* argv[] )
/// {
///     Log::info( "Application initializing." );
///     TrivialApplication app;
///     launchCobaltApplication( &app );
///     return 0;
/// }
///
void launchCobaltApplication( Application* app );
    
}}


