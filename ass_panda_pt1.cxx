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

void locate(const Event * theEvent, void * data){
	cout << "locate" <<endl;
	//camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]+0.1);
}
void moveUp(const Event * theEvent, void * data){
	cout << "Up" <<endl;
	camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]+0.1);
}
void moveDown(const Event * theEvent, void * data){
	cout<<"Down" <<endl;
	camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]-0.1);
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
	LVecBase3f speed(00 , 40 , 2);

  BulletSphereShape* sphere_shape = new BulletSphereShape( radius ) ;
  BulletRigidBodyNode* sphere_rigid_node = new BulletRigidBodyNode("Sphere");

  sphere_rigid_node->set_mass(0.02);
  sphere_rigid_node->add_shape(sphere_shape);
 
  physics_world->attach_rigid_body(sphere_rigid_node);

  NodePath np_sphere = window->get_render().attach_new_node(sphere_rigid_node);
  np_sphere.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]);
 
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


void init_astronaut(){

	BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(1.0,1.0,4.0));
	BulletRigidBodyNode* astronaut_rigid_node = new BulletRigidBodyNode("Box");

	astronaut_rigid_node->set_mass(0.1);

	astronaut_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,0.0)));

	physics_world->attach_rigid_body(astronaut_rigid_node);
 
	NodePath np_astronaut = window->get_render().attach_new_node(astronaut_rigid_node);
	np_astronaut.set_pos_hpr(0, -5, 6, 180, 0, 0);

	astronaut = window->load_model(framework.get_models(),"models/astronaut/astronaut");
	//astronaut = window->load_model(framework.get_models(),"models/misc/rgbCube");	
	astronaut.reparent_to(window->get_render());
	astronaut.set_scale(2.0);
	astronaut.set_pos(-0, 0, -4);
	astronaut.set_hpr(0, 0, 0);

	astronaut.reparent_to(np_astronaut);
	
}

void init_table(){
	LVecBase3f normal(5 , 3 , 2);

	double h = 1.5;

	BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(5.0,2.5,0.2));
	BulletBoxShape *shape2 = new BulletBoxShape(LVecBase3f(0.2,0.2,h));
	BulletBoxShape *shape3 = new BulletBoxShape(LVecBase3f(0.2,0.2,h));
	BulletBoxShape *shape4 = new BulletBoxShape(LVecBase3f(0.2,0.2,h));
	BulletBoxShape *shape5 = new BulletBoxShape(LVecBase3f(0.2,0.2,h));
	BulletRigidBodyNode* table_rigid_node = new BulletRigidBodyNode("Box");

	table_rigid_node->set_mass(1.0);

	table_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,2*(h-0.2))));
	table_rigid_node->add_shape(shape2, TransformState::make_pos(LPoint3f(-3.8,-1.2,h)));
	table_rigid_node->add_shape(shape3, TransformState::make_pos(LPoint3f(-3.8,1.2,h)));
	table_rigid_node->add_shape(shape4, TransformState::make_pos(LPoint3f(3.8,-1.2,h)));
	table_rigid_node->add_shape(shape5, TransformState::make_pos(LPoint3f(3.8,1.2,h)));

	physics_world->attach_rigid_body(table_rigid_node);
 
	NodePath np_table = window->get_render().attach_new_node(table_rigid_node);
	np_table.set_pos_hpr(0, 0, 0, 0, 0, 0);


	table = window->load_model(framework.get_models(),"models/dining_table_2/DiningTable2");
	table.reparent_to(window->get_render());
	table.set_scale(1);
	table.set_pos(0, 0, 0);
	table.set_hpr(0, 0, 0);
    
  table.reparent_to(np_table);
	
}


void init_floor(){  
  LVecBase3f normal (0 , 0 , 1) ;
  double d = 1;

  BulletPlaneShape * floor_shape = new BulletPlaneShape (normal, d);
  BulletRigidBodyNode * floor_rigid_node = new BulletRigidBodyNode("Ground");
  floor_rigid_node -> add_shape (floor_shape,TransformState::make_pos(LPoint3f(0.0,0.0,-1.0)));
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
	framework.define_key("c", "camera up", moveUp, 0);
	framework.define_key("v", "camera down", moveDown, 0);
	framework.define_key("l", "home locator", locate, 0);
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
	camera.set_pos(-0,-10, 0);
	camera.set_hpr(-0, -0, 0);
 
    physics_world = new BulletWorld () ;
    physics_world->set_gravity(0 , 0 , -9.8) ;
		
	
	window->enable_keyboard();	
	inputBinding();
	modelSetup(window);

    init_floor();
    //init_ball();
	init_astronaut();
	init_table();

    PT(GenericAsyncTask) task;
    task = new GenericAsyncTask("Scene update" , &update_scene , (void *) NULL );
    task_mgr->add(task);
    
 
    framework.main_loop();
    framework.close_framework();
 
    return 0;
}

