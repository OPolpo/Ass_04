#include "pandaFramework.h"
#include "pandaSystem.h"

#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

// Global stuff
PandaFramework framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
NodePath camera;
NodePath myCube;

// Task to move the Cube
AsyncTask::DoneStatus rotatingCube(GenericAsyncTask* task, void* data) {
    double time = globalClock->get_real_time();
    double angledegrees = time * 40.0;
	myCube.set_hpr(angledegrees, angledegrees/40*20, 0);
    
    return AsyncTask::DS_cont;
}

int main(int argc, char *argv[]) {
    // Open a new window framework and set the title
    framework.open_framework(argc, argv);
    framework.set_window_title("My Panda3D Window");
    
    // Open the window
    WindowFramework *window = framework.open_window();
    camera = window->get_camera_group(); // Get the camera and store it
    camera.set_pos(0,0,0);
    camera.set_hpr(0, 0, 0);
    
    // Load cube
    myCube = window->load_model(framework.get_models(),"models/misc/rgbCube");
    myCube.reparent_to(window->get_render());
    myCube.set_scale(1);
    myCube.set_pos(0, 5, 0);
    myCube.set_hpr(45,45,0);
    
    // Add our task.
    taskMgr->add(new GenericAsyncTask("Spins the camera", &rotatingCube, (void*) NULL));
    
    // Run the engine.
	framework.main_loop();
	// Shut down the engine when done.
	framework.close_framework();
    return (0);
}
