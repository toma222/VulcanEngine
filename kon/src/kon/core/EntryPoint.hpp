
#include "Context.hpp"
#include "kon/engine/Application.hpp"
#include "kon/core/Context.hpp"
#include "kon/types/Path.hpp"
#include <kon/debug/Debug.hpp>

extern kon::Application *GetApplication(kon::Context *context);

int main(int argc, char const *argv[])
{
    KN_TRACE("entering main");

    KN_INSTRUMENT_NEW_FILE("log/Startup.json");
	kon::Path prjPath("kon/templates/Sandbox3D/");
    kon::Context *context = new kon::Context(prjPath);
    kon::Application *app = new kon::Application(context); // GetApplication(context);
    KN_INSTRUMENT_CLOSE_FILE();

    KN_INSTRUMENT_NEW_FILE("log/Runtime.json");
    app->Run();
    KN_INSTRUMENT_CLOSE_FILE();

    KN_INSTRUMENT_NEW_FILE("log/Close.json");
    delete app;
    delete context;
    KN_INSTRUMENT_CLOSE_FILE();
    
    return 0;
}

