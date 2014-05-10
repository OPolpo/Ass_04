#include "pandaFramework.h"
#include "pandaSystem.h"

#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

// Global stuff
PandaFramework framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
NodePath camera;
NodePath ground, table, mantis, astronaut, sky;

void moveForward(const Event * theEvent, void * data){
	cout << "forward" <<endl;
	camera.set_pos(camera.get_pos()[0], camera.get_pos()[1]+0.1, camera.get_pos()[2]);
}
void moveBackward(const Event * theEvent, void * data){
	cout<<"backward" <<endl;
	camera.set_pos(camera.get_pos()[0], camera.get_pos()[1]-0.1, camera.get_pos()[2]);
}
void moveLeft(const Event * theEvent, void * data){
	cout<<"left" <<endl;
	camera.set_pos(camera.get_pos()[0]-0.1, camera.get_pos()[1], camera.get_pos()[2]);
}
void moveRight(const Event * theEvent, void * data){
	cout<<"right" <<endl;
	camera.set_pos(camera.get_pos()[0]+0.1, camera.get_pos()[1], camera.get_pos()[2]);
}
void Exit(const Event * theEvent, void * data){
	exit(0);
}

void modelSetup(WindowFramework *window){

	sky = window->load_model(framework.get_models(),"models/stars/stars");
	sky.reparent_to(window->get_render());
	sky.set_scale(1);
	sky.set_pos(0, 0, -1);
	sky.set_hpr(0, 0, 0);

	ground = window->load_model(framework.get_models(),"models/moonsurface/moonsurface");
	ground.reparent_to(window->get_render());
	ground.set_scale(0.02);
	ground.set_pos(0, 0, 0);
	ground.set_hpr(0, 0, 0);

	table = window->load_model(framework.get_models(),"models/dining_table_2/DiningTable2");
	table.reparent_to(window->get_render());
	table.set_scale(1);
	table.set_pos(0, 0, 0);
	table.set_hpr(0, 0, 0);

	mantis = window->load_model(framework.get_models(),"models/mantis/mantis");
	mantis.reparent_to(table);
	mantis.set_scale(2);
	mantis.set_pos(1.5, 0, 2);
	mantis.set_hpr(170, 0, 0);

	astronaut = window->load_model(framework.get_models(),"models/astronaut/astronaut");
	astronaut.reparent_to(table);
	astronaut.set_scale(2);
	astronaut.set_pos(1.5, 0, -2);
	astronaut.set_hpr(170, 0, 0);


	
}

void inputBinding(){
	framework.define_key("escape", "exit", Exit, 0);
	framework.define_key("arrow_up", "camera forward", moveForward, 0);
	framework.define_key("arrow_down", "camera backward", moveBackward, 0);
	framework.define_key("arrow_left", "camera left", moveLeft, 0);
	framework.define_key("arrow_right", "camera right", moveRight, 0);
}



int main(int argc, char *argv[]) {
	// Open a new window framework and set the title
	framework.open_framework(argc, argv);
	framework.set_window_title("Panda3D - Assignment");
	// Open the window
	WindowFramework *window = framework.open_window();

	//keyboard Management
	window->enable_keyboard();
	
	inputBinding();
	modelSetup(window);

	camera = window->get_camera_group(); // Get the camera and store it
	camera.set_pos(0,-20,5);
	camera.set_hpr(0, -10, 0);
	
	// Run the engine.
	framework.main_loop();
	// Shut down the engine when done.
	framework.close_framework();
	return (0);
}
