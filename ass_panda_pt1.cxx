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
#include "bulletCylinderShape.h"
#include "bulletConvexHullShape.h"
#include "cardMaker.h"
#include "texture.h"
#include "texturePool.h"
#include "textureStage.h"


#include "ambientLight.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotlight.h"

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


void init_astronaut(){
	
	double height = 1;
	double pos_x = 0;
	double pos_y = 0;
	double pos_z = 3;//the foot of the astronaut

	
	BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(1.0,1.0,height));
	BulletRigidBodyNode* astronaut_rigid_node = new BulletRigidBodyNode("Box");

	astronaut_rigid_node->set_mass(0.3);

	astronaut_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,0.0)));

	physics_world->attach_rigid_body(astronaut_rigid_node);
 
	NodePath np_astronaut = window->get_render().attach_new_node(astronaut_rigid_node);
	np_astronaut.set_pos_hpr(pos_x, pos_y , height + pos_z, 180, 0, 0);

	astronaut = window->load_model(framework.get_models(),"models/astronaut/astronaut");
	astronaut.reparent_to(window->get_render());
	astronaut.set_scale(0.5*height);
	astronaut.set_pos(-0, 0, -height);
	astronaut.set_hpr(0, 0, 0);

	astronaut.reparent_to(np_astronaut);
	astronaut_rigid_node->set_friction(0.6);
	
}

void init_table(){
	LVecBase3f normal(5 , 3 , 2);

	double h = 1;

	//BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(4.0*h,2.5*h,0.1*h));
	BulletConvexHullShape *shape1 = new BulletConvexHullShape();
	shape1->add_point(LPoint3f(3.65, 0, 0));//
	shape1->add_point(LPoint3f(3.35, 0.9, 0));
	shape1->add_point(LPoint3f(2.45, 1.75, 0));
	shape1->add_point(LPoint3f(1.3, 2.3, 0));
	shape1->add_point(LPoint3f(0, 2.4, 0));//
	shape1->add_point(LPoint3f(-3.65, 0, 0));//
	shape1->add_point(LPoint3f(0, -2.4, 0));//

	
	BulletBoxShape *shape2 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape3 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape4 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape5 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletRigidBodyNode* table_rigid_node = new BulletRigidBodyNode("Box");

	table_rigid_node->set_mass(1.0);

	table_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,2*h+1)));
	table_rigid_node->add_shape(shape2, TransformState::make_pos(LPoint3f(-2.8*h,-0.9*h,h)));
	table_rigid_node->add_shape(shape3, TransformState::make_pos(LPoint3f(-2.8*h,0.9*h,h)));
	table_rigid_node->add_shape(shape4, TransformState::make_pos(LPoint3f(2.8*h,-0.9*h,h)));
	table_rigid_node->add_shape(shape5, TransformState::make_pos(LPoint3f(2.8*h,0.9*h,h)));

	physics_world->attach_rigid_body(table_rigid_node);
 
	NodePath np_table = window->get_render().attach_new_node(table_rigid_node);
	np_table.set_pos_hpr(0, 0, 0, 0, 0, 0);


	table = window->load_model(framework.get_models(),"models/dining_table_2/DiningTable2");
	table.reparent_to(window->get_render());
	table.set_scale(0.77);
	//table.set_scale(0.01);
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
	floor_rigid_node->set_friction(0.4);
}

void locate_models(){
	init_floor();
	init_astronaut();
	init_table();
}
void locate(const Event * theEvent, void * data){
	cout << "locate" <<endl;
	locate_models();
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
	LVecBase3f speed(00 , 0 , -20);
	LVecBase3f ang_speed(10 , 0 , 0);

  BulletSphereShape* sphere_shape = new BulletSphereShape( radius ) ;
  BulletRigidBodyNode* sphere_rigid_node = new BulletRigidBodyNode("Sphere");

  sphere_rigid_node->set_mass(0.02);
  sphere_rigid_node->add_shape(sphere_shape);
 
  physics_world->attach_rigid_body(sphere_rigid_node);

  NodePath np_sphere = window->get_render().attach_new_node(sphere_rigid_node);
  np_sphere.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]);
 
  //NodePath np_sphere_model = window->load_model(framework.get_models(), "smiley");
	NodePath np_sphere_model = window->load_model(framework.get_models(), "models/baseball/baseball");
  np_sphere_model.reparent_to(np_sphere);
	np_sphere_model.set_scale(radius*7);
	sphere_rigid_node->set_linear_velocity(speed);
	sphere_rigid_node->set_angular_velocity(ang_speed);
	//sphere_rigid_node->set_friction(1);
	
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

void debugModeHandler(const Event *eventPtr, void *dataPtr)
{
    BulletDebugNode *debugNode = (BulletDebugNode *)dataPtr;
    static bool show_bounding_boxes_value = false;
    static bool show_constraints_value = false;
    static bool show_normals_value = false;
    static bool show_wireframe_value = false;
    if(eventPtr->get_name() == "f1")
    {
        show_bounding_boxes_value = !show_bounding_boxes_value;
        debugNode->show_bounding_boxes(show_bounding_boxes_value);
    }
    if(eventPtr->get_name() == "f2")
    {
        show_constraints_value = !show_constraints_value;
        debugNode->show_constraints(show_constraints_value);
    }
    if(eventPtr->get_name() == "f3")
    {
        show_normals_value = !show_normals_value;
        debugNode->show_normals(show_normals_value);
    }
    if(eventPtr->get_name() == "f4")
    {
        show_wireframe_value = !show_wireframe_value;
        debugNode->show_wireframe(show_wireframe_value);
    }
}
 
int main(int argc, char *argv[]) {

    framework.open_framework(argc, argv);
    framework.set_window_title("Panda3D - Assignment");
 
    window = framework.open_window();
 
    camera = window->get_camera_group();
	camera.set_pos(-0,-0, 23);
	camera.set_hpr(-0, -90, 0);
 
    physics_world = new BulletWorld () ;
    physics_world->set_gravity(0 , 0 , -9.8) ;
		
	
	window->enable_keyboard();	
	inputBinding();
	modelSetup(window);

   	locate_models();

    PT(GenericAsyncTask) task;
    task = new GenericAsyncTask("Scene update" , &update_scene , (void *) NULL );
    task_mgr->add(task);
    

	PT(Spotlight) s_light;
	s_light = new Spotlight("my s_light");
	s_light->set_color(LVecBase4f(4.0, 4.0, 4.0, 1));
	NodePath slnp = window->get_render().attach_new_node(s_light);
	slnp.set_pos(-0, -0, 10);
	slnp.set_hpr(-00, -90, 0);
	window->get_render().set_light(slnp);

	PT(AmbientLight) a_light;
	a_light = new AmbientLight("my a_light");
	a_light->set_color(LVecBase4f(0.2, 0.2, 0.2, 1));
	NodePath alnp = window->get_render().attach_new_node(a_light);
	window->get_render().set_light(alnp);

	// debug node
    PT(BulletDebugNode) bullet_dbg_node;
    bullet_dbg_node = new BulletDebugNode("Debug");
    bullet_dbg_node->show_bounding_boxes(false);
    bullet_dbg_node->show_constraints(false);
    bullet_dbg_node->show_normals(false);
    bullet_dbg_node->show_wireframe(false);
    NodePath np_dbg_node = window->get_render().attach_new_node(bullet_dbg_node);
    np_dbg_node.show();
    physics_world->set_debug_node(bullet_dbg_node);
    // end debug node
	
	// debugger keys
    framework.define_key("f1", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f2", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f3", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f4", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);

    framework.main_loop();
    framework.close_framework();
 
    return 0;
}

