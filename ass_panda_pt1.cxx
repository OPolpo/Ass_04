#include "pandaFramework.h"
#include "windowFramework.h"
#include "nodePath.h"
#include "clockObject.h"
#include "asyncTask.h"
#include "genericAsyncTask.h"
#include "bulletWorld.h"
#include "bulletPlaneShape.h"
#include "bulletSphereShape.h"
#include "bulletBoxShape.h"
#include "cardMaker.h"
#include "texture.h"
#include "texturePool.h"
#include "textureStage.h"

// Global stuff
PandaFramework framework;
WindowFramework *window;
AsyncTaskManager *task_mgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
NodePath camera;
NodePath ground, table, mantis, astronaut, sky;


BulletWorld *physics_world;

ClockObject *co = ClockObject::get_global_clock();
TexturePool *texture_pool = TexturePool::get_global_ptr();

 
AsyncTask::DoneStatus update_scene(GenericAsyncTask* task, void* data){
	ClockObject *co = ClockObject::get_global_clock();
	physics_world ->do_physics(co->get_dt(), 10, 1.0 / 180.0);
	return	AsyncTask::DS_cont;
}




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


void init_ball(){
  double radius = 0.1;
	LVecBase3f speed(0 , 30 , 0);

  BulletSphereShape* sphere_shape = new BulletSphereShape( radius ) ;
  BulletRigidBodyNode* sphere_rigid_node = new BulletRigidBodyNode("Sphere");

  sphere_rigid_node->set_mass(0.02);
  sphere_rigid_node->add_shape(sphere_shape);
 
  physics_world->attach_rigid_body(sphere_rigid_node);

  NodePath np_sphere = window->get_render().attach_new_node(sphere_rigid_node);
  np_sphere.set_pos(camera.get_pos()[0], camera.get_pos()[1]+5, camera.get_pos()[2]);
 
  NodePath np_sphere_model = window->load_model(framework.get_models(), "smiley");
  np_sphere_model.reparent_to(np_sphere);
	np_sphere_model.set_scale(radius);
	sphere_rigid_node->set_linear_velocity(speed);
}


void ballLaunch(const Event * theEvent, void * data){
	cout<<"baaaaaaaaaaaaaaaaaaaaaaaaaaall" <<endl;
	init_ball();
}

void modelSetup(WindowFramework *window){

	sky = window->load_model(framework.get_models(),"models/sky/blue_sky_sphere");
	sky.reparent_to(window->get_render());
	sky.set_scale(1);
	sky.set_pos(0, 0, -1);
	sky.set_hpr(0, 0, 0);

	//ground = window->load_model(framework.get_models(),"models/moonsurface/moonsurface");
	//ground.reparent_to(window->get_render());
	//ground.set_scale(0.02);
	//ground.set_pos(0, 0, 0);
	//ground.set_hpr(0, 0, 0);

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
	astronaut.set_scale(0.5);
	astronaut.set_pos(-1.5, 0, 2.7);
	astronaut.set_hpr(190, 0, 0);


	
}

void init_table(){
	LVecBase3f normal(0.5 , 0.5 , 0.5);

  BulletBoxShape* box_shape = new BulletBoxShape(normal);
  BulletRigidBodyNode* box_rigid_node = new BulletRigidBodyNode("Box");

  box_rigid_node->set_mass(1.0);
  box_rigid_node->add_shape(box_shape);

  physics_world->attach_rigid_body(box_rigid_node);
 
  NodePath np_box = window->get_render().attach_new_node(box_rigid_node);
  np_box.set_pos_hpr(0, 0, 1.5, 0, 0, 0);
    
  table.reparent_to(np_box);

}


void init_cube(){ 
  LVecBase3f normal(0.5 , 0.5 , 0.5);

  BulletBoxShape* box_shape = new BulletBoxShape(normal);
  BulletRigidBodyNode* box_rigid_node = new BulletRigidBodyNode("Box");

  box_rigid_node->set_mass(1.0);
  box_rigid_node->add_shape(box_shape);

  physics_world->attach_rigid_body(box_rigid_node);
 
  NodePath np_box = window->get_render().attach_new_node(box_rigid_node);
  np_box.set_pos_hpr(0, 0, 7, 45, 45, 45);
    
  NodePath np_box_model = window->load_model(framework.get_models(), "models/box");
  np_box_model.set_pos(-1.0, -1.0, -1.0);
  np_box_model.set_scale(2);
  np_box_model.reparent_to(np_box);
}

void init_floor(){  
  LVecBase3f normal (0 , 0 , 1) ;
  double d = 1;

  BulletPlaneShape * floor_shape = new BulletPlaneShape (normal, d);
  BulletRigidBodyNode * floor_rigid_node = new BulletRigidBodyNode("Ground");
  floor_rigid_node -> add_shape (floor_shape);
  NodePath np_ground = window -> get_render().attach_new_node(floor_rigid_node);
  np_ground.set_pos(0, 30, 0);
  physics_world -> attach_rigid_body(floor_rigid_node);
  

  CardMaker* cm = new CardMaker("ground");
  cm->set_frame(-60, 60, -60, 60);
    
  NodePath np_ground_tex = window->get_render().attach_new_node(cm->generate());
  np_ground_tex.reparent_to(np_ground);
  
  TextureStage* ts = new TextureStage("ts");
  ts->set_mode(TextureStage::M_modulate);

  Texture* tex;
  tex = texture_pool->load_texture("floor1.jpg");
  
  np_ground_tex.set_p(270);
  np_ground_tex.set_tex_scale(ts, 9, 9);
  np_ground.set_texture(ts, tex);
}


void inputBinding(){
	framework.define_key("escape", "exit", Exit, 0);
	framework.define_key("arrow_up", "camera forward", moveForward, 0);
	framework.define_key("arrow_down", "camera backward", moveBackward, 0);
	framework.define_key("arrow_left", "camera left", moveLeft, 0);
	framework.define_key("arrow_right", "camera right", moveRight, 0);
	framework.define_key("space", "ball launch", ballLaunch, 0);
}

 
int main(int argc, char *argv[]) {

    framework.open_framework(argc, argv);
    framework.set_window_title("Panda3D - Assignment");
 
    window = framework.open_window();
 
    camera = window->get_camera_group();
	camera.set_pos(0,-20, 6);
	camera.set_hpr(0, -5, 0);
 
    physics_world = new BulletWorld () ;
    physics_world->set_gravity(0 , 0 , -9.8) ;
		
	
	window->enable_keyboard();	
	inputBinding();
	modelSetup(window);

    init_floor();
    //init_ball();
    init_cube();
	init_table();

    PT(GenericAsyncTask) task;
    task = new GenericAsyncTask("Scene update" , &update_scene , (void *) NULL );
    task_mgr->add(task);
    
 
    framework.main_loop();
    framework.close_framework();
 
    return 0;
}

