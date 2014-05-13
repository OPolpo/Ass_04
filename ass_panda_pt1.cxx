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
#include "material.h"
#include "audioManager.h"
#include "audio.h"
#include "audioSound.h"

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
NodePath ground, table, rolling_pin, lunch_box, astronaut, sky;



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
	double pos_z = 2.7;//the foot of the astronaut

	
	BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(height*0.25,height*0.20,height*0.5));//legs
	BulletSphereShape *shape2 = new BulletSphereShape(height*0.2);//head
	BulletBoxShape *shape3 = new BulletBoxShape(LVecBase3f(height*0.5,height*0.3,height*0.4));//body
	BulletRigidBodyNode* astronaut_rigid_node = new BulletRigidBodyNode("Box");

	astronaut_rigid_node->set_mass(0.3);

	astronaut_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.10*height,0.0)));//legs
	astronaut_rigid_node->add_shape(shape2, TransformState::make_pos(LPoint3f(0.0,0.0,height*1.4)));//head
	astronaut_rigid_node->add_shape(shape3, TransformState::make_pos(LPoint3f(0.0,-0.05*height,height*0.8)));//body

	physics_world->attach_rigid_body(astronaut_rigid_node);
 
	NodePath np_astronaut = window->get_render().attach_new_node(astronaut_rigid_node);
	np_astronaut.set_pos_hpr(pos_x, pos_y , height + pos_z, 180, 0, 0);

	astronaut = window->load_model(framework.get_models(),"models/astronaut/astronaut");
	astronaut.reparent_to(window->get_render());
	astronaut.set_scale(0.5*height);
	astronaut.set_pos(-0, 0, -height*0.50);
	astronaut.set_hpr(0, 0, 0);

	astronaut.reparent_to(np_astronaut);
	astronaut_rigid_node->set_friction(0.6);
	
}

void init_rolling_pin(){
	
	double length = 2;
	double pos_x = 3;
	double pos_y = 0;
	double pos_z = 2.5;

	
	BulletCylinderShape *shape1 = new BulletCylinderShape(0.1*length,length);
	BulletRigidBodyNode* rolling_pin_rigid_node = new BulletRigidBodyNode("Box");

	rolling_pin_rigid_node->set_mass(0.3);

	rolling_pin_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,0.0)));


	physics_world->attach_rigid_body(rolling_pin_rigid_node);
 
	NodePath np_rolling_pin = window->get_render().attach_new_node(rolling_pin_rigid_node);
	np_rolling_pin.set_pos_hpr(pos_x, pos_y , length + pos_z, 45, 90, 0);

	rolling_pin = window->load_model(framework.get_models(),"models/rolling/RollingPin");
	rolling_pin.reparent_to(window->get_render());
	rolling_pin.set_scale(length);
	rolling_pin.set_pos(0, 0, 0);
	rolling_pin.set_hpr(0, 0, 90);

	rolling_pin.reparent_to(np_rolling_pin);
	rolling_pin_rigid_node->set_friction(0.6);
	rolling_pin_rigid_node->set_anisotropic_friction(0.8);
	
}

void init_lunch_box(){
	
	double length = 0.8;
	double pos_x = -3;
	double pos_y = 0;
	double pos_z = 3.5;

	
	BulletBoxShape *shape1 = new BulletBoxShape(LVecBase3f(length*1.1,length*0.5,length*1.2));
	BulletRigidBodyNode* lunch_box_rigid_node = new BulletRigidBodyNode("Box");

	lunch_box_rigid_node->set_mass(0.3);

	lunch_box_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,0.0)));


	physics_world->attach_rigid_body(lunch_box_rigid_node);
 
	NodePath np_lunch_box = window->get_render().attach_new_node(lunch_box_rigid_node);
	np_lunch_box.set_pos_hpr(pos_x, pos_y , length + pos_z, 45, 0, 0);

	lunch_box = window->load_model(framework.get_models(),"models/lunchBox/LunchBox");
	lunch_box.reparent_to(window->get_render());
	lunch_box.set_scale(length*3);
	lunch_box.set_pos(0, 0, 0);
	lunch_box.set_hpr(0, 0, 90);

	lunch_box.reparent_to(np_lunch_box);
	lunch_box_rigid_node->set_friction(0.6);
	lunch_box_rigid_node->set_anisotropic_friction(0.8);
	
}

void init_table(){
	LVecBase3f normal(5 , 3 , 2);

	double h = 1.5;
	double p1x =  3.65*h;
	double p1y =  0.00;

	double p2x =  3.35*h;
	double p2y =  0.90*h;

	double p3x =  2.45*h;
	double p3y =  1.75*h;

	double p4x =  1.30*h;
	double p4y =  2.30*h;
	
	double p5x =  0.00*h;
	double p5y =  2.40*h;
	

	BulletConvexHullShape *shape1 = new BulletConvexHullShape();

	double thickness = 0.1*h;
	shape1->add_point(LPoint3f(p1x, p1y, thickness));//
	shape1->add_point(LPoint3f(p2x, p2y, thickness));
	shape1->add_point(LPoint3f(p3x, p3y, thickness));
	shape1->add_point(LPoint3f(p4x, p4y, thickness));
	shape1->add_point(LPoint3f(p5x, p5y, thickness));//
	shape1->add_point(LPoint3f(-p4x, p4y, thickness));
	shape1->add_point(LPoint3f(-p3x, p3y, thickness));
	shape1->add_point(LPoint3f(-p2x, p2y, thickness));
	shape1->add_point(LPoint3f(-p1x, p1y, thickness));//
	shape1->add_point(LPoint3f(-p2x, -p2y, thickness));
	shape1->add_point(LPoint3f(-p3x, -p3y, thickness));
	shape1->add_point(LPoint3f(-p4x, -p4y, thickness));
	shape1->add_point(LPoint3f(-p5x, -p5y, thickness));//
	shape1->add_point(LPoint3f(p4x, -p4y, thickness));
	shape1->add_point(LPoint3f(p3x, -p3y, thickness));
	shape1->add_point(LPoint3f(p2x, -p2y, thickness));
	
	shape1->add_point(LPoint3f(p1x, p1y, 0));//
	shape1->add_point(LPoint3f(p2x, p2y, 0));
	shape1->add_point(LPoint3f(p3x, p3y, 0));
	shape1->add_point(LPoint3f(p4x, p4y, 0));
	shape1->add_point(LPoint3f(p5x, p5y, 0));//
	shape1->add_point(LPoint3f(-p4x, p4y, 0));
	shape1->add_point(LPoint3f(-p3x, p3y, 0));
	shape1->add_point(LPoint3f(-p2x, p2y, 0));
	shape1->add_point(LPoint3f(-p1x, p1y, 0));//
	shape1->add_point(LPoint3f(-p2x, -p2y, 0));
	shape1->add_point(LPoint3f(-p3x, -p3y, 0));
	shape1->add_point(LPoint3f(-p4x, -p4y, 0));
	shape1->add_point(LPoint3f(-p5x, -p5y, 0));//
	shape1->add_point(LPoint3f(p4x, -p4y, 0));
	shape1->add_point(LPoint3f(p3x, -p3y, 0));
	shape1->add_point(LPoint3f(p2x, -p2y, 0));

	BulletBoxShape *shape2 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape3 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape4 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletBoxShape *shape5 = new BulletBoxShape(LVecBase3f(0.2*h,0.2*h,h));
	BulletRigidBodyNode* table_rigid_node = new BulletRigidBodyNode("Box");

	table_rigid_node->set_mass(1.0);

	table_rigid_node->add_shape(shape1, TransformState::make_pos(LPoint3f(0.0,0.0,2*h)));
	//table_rigid_node->add_shape(shape11, TransformState::make_pos(LPoint3f(0.0,0.0,2*h)));
	table_rigid_node->add_shape(shape2, TransformState::make_pos(LPoint3f(-2.8*h,-0.9*h,h)));
	table_rigid_node->add_shape(shape3, TransformState::make_pos(LPoint3f(-2.8*h,0.9*h,h)));
	table_rigid_node->add_shape(shape4, TransformState::make_pos(LPoint3f(2.8*h,-0.9*h,h)));
	table_rigid_node->add_shape(shape5, TransformState::make_pos(LPoint3f(2.8*h,0.9*h,h)));

	physics_world->attach_rigid_body(table_rigid_node);
 
	NodePath np_table = window->get_render().attach_new_node(table_rigid_node);
	np_table.set_pos_hpr(0, 0, 0, 0, 0, 0);


	table = window->load_model(framework.get_models(),"models/dining_table_2/DiningTable2");
	table.reparent_to(window->get_render());
	table.set_scale(0.77*h);
	//table.set_scale(0.01);
	table.set_pos(0, 0, 0);
	table.set_hpr(0, 0, 0);
    
  table.reparent_to(np_table);
	PT(Material) table_material = new Material("table");
	table_material->set_diffuse(LVecBase4(0.5,0.5,0.5,0.5));
	table.set_material(table_material);
	
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

	PT(Material) floor_material = new Material();
	floor_material->set_diffuse(LVecBase4(1,1,1,1));
	np_ground.set_material(floor_material);
	
}

void locate_models(){
	init_floor();
	init_astronaut();
	init_table();
	init_rolling_pin();
	init_lunch_box();
}
void locate(const Event * theEvent, void * data){
	cout << "locate" <<endl;
	locate_models();
	//camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]+0.1);
}

void Exit(const Event * theEvent, void * data){
	exit(0);
}


void init_ball(){
	double radius = 0.1;
	LVecBase3f speed(00 , 60 , 0);
	LVecBase3f ang_speed(10 , 0 , 0);

	BulletSphereShape* sphere_shape = new BulletSphereShape( radius ) ;
	BulletRigidBodyNode* sphere_rigid_node = new BulletRigidBodyNode("Sphere");

	sphere_rigid_node->set_mass(0.02);
	sphere_rigid_node->add_shape(sphere_shape);

	physics_world->attach_rigid_body(sphere_rigid_node);
		
	NodePath np_sphere = window->get_render().attach_new_node(sphere_rigid_node);
	np_sphere.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]);


	NodePath np_sphere_model = window->load_model(framework.get_models(), "models/baseball/baseball");
	np_sphere_model.reparent_to(np_sphere);
	np_sphere_model.set_scale(radius*7);
	sphere_rigid_node->set_linear_velocity(speed);
	sphere_rigid_node->set_angular_velocity(ang_speed);
}



void modelSetup(WindowFramework *window){

	sky = window->load_model(framework.get_models(),"models/sky/blue_sky_sphere");
	sky.reparent_to(window->get_render());
	sky.set_scale(1);
	sky.set_pos(0, 0, -1);
	sky.set_hpr(0, 0, 0);
	
}

void KeyboardHandler(const Event *eventPtr, void *dataPtr)
{
    if(eventPtr->get_name() == "arrow_up"){
		camera.set_pos(camera.get_pos()[0], camera.get_pos()[1]+0.15, camera.get_pos()[2]);
    }
    if(eventPtr->get_name() == "arrow_down"){
		camera.set_pos(camera.get_pos()[0], camera.get_pos()[1]-0.15, camera.get_pos()[2]);
    }
    if(eventPtr->get_name() == "arrow_left"){
		camera.set_pos(camera.get_pos()[0]-0.15, camera.get_pos()[1], camera.get_pos()[2]);
    }
	if(eventPtr->get_name() == "arrow_right"){
		camera.set_pos(camera.get_pos()[0]+0.15, camera.get_pos()[1], camera.get_pos()[2]);
    }
	if(eventPtr->get_name() == "space"){
		cout<<"baaaaaaaaaaaaaaaaaaaaaaaaaaall" <<endl;
		//mySound->play();
		init_ball();
    }
	if(eventPtr->get_name() == "c"){
		camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]+0.15);
    }
	if(eventPtr->get_name() == "v"){
		camera.set_pos(camera.get_pos()[0], camera.get_pos()[1], camera.get_pos()[2]-0.15);
    }
	if(eventPtr->get_name() == "escape"){
		exit(0);
    }

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
void inputBinding(){
	framework.define_key("escape", "exit", KeyboardHandler, 0);
	framework.define_key("c", "camera up", KeyboardHandler, 0);
	framework.define_key("v", "camera down", KeyboardHandler, 0);
	framework.define_key("l", "home locator", KeyboardHandler, 0);
	framework.define_key("arrow_up", "camera forward", KeyboardHandler, 0);
	framework.define_key("arrow_down", "camera backward", KeyboardHandler, 0);
	framework.define_key("arrow_left", "camera left", KeyboardHandler, 0);
	framework.define_key("arrow_right", "camera right", KeyboardHandler, 0);
	framework.define_key("space", "ball launch", KeyboardHandler, 0);

}
void init_light(){

	PT(PointLight) s_light;
	s_light = new PointLight("my s_light");
	s_light->set_color(LVecBase4f(1.0, 1.0, 1.0, 1));
	NodePath slnp = window->get_render().attach_new_node(s_light);
	slnp.set_pos(5, -5, 10);
	window->get_render().set_light(slnp);

	PT(PointLight) s_light_2;
	s_light_2 = new PointLight("my s_light");
	s_light_2->set_color(LVecBase4f(1.0, 1.0, 1.0, 1));
	NodePath slnp_2 = window->get_render().attach_new_node(s_light_2);
	slnp_2.set_pos(-5, -5, 10);
	window->get_render().set_light(slnp_2);

	PT(AmbientLight) a_light;
	a_light = new AmbientLight("my a_light");
	a_light->set_color(LVecBase4f(0.2, 0.2, 0.2, 1));
	NodePath alnp = window->get_render().attach_new_node(a_light);
	window->get_render().set_light(alnp);
}

int main(int argc, char *argv[]) {

    framework.open_framework(argc, argv);
    framework.set_window_title("Panda3D - Assignment");
 
    window = framework.open_window();
	PT(AudioManager) AM = AudioManager::create_AudioManager();
	PT(AudioSound) mySound = AM->get_sound("aaa.ogg");
	AM->set_active(true);

	//mySound->set_volume(1);
	mySound->set_loop(true);
	cout << mySound->status() << endl;

	cout << mySound->status() << endl;
 
    camera = window->get_camera_group();
	camera.set_pos(-0,-20, 2);
	camera.set_hpr(-0, -0, 0);
 
    physics_world = new BulletWorld () ;
    physics_world->set_gravity(0 , 0 , -9.8) ;
		
	
	window->enable_keyboard();	
	inputBinding();
	modelSetup(window);

   	locate_models();

    PT(GenericAsyncTask) task;
    task = new GenericAsyncTask("Scene update" , &update_scene , (void *) NULL );
    task_mgr->add(task);
    

	init_light();

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
	framework.define_key("f1", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f2", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f3", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
    framework.define_key("f4", "callChangeDebugMode", &debugModeHandler, (void *) bullet_dbg_node);
	

	mySound->play();

    framework.main_loop();
    framework.close_framework();
 
    return 0;
}

