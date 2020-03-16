//
//  Game.cpp
//
//  Copyright � 2018 Alun Evans. All rights reserved.
//

#include "Game.h"
#include "Shader.h"
#include "extern.h"
#include "Parsers.h"

Game::Game() {

}

//Nothing here yet
void Game::init(int w, int h) {

	window_width_ = w; window_height_ = h;
	//******* INIT SYSTEMS *******

	//init systems except debug, which needs info about scene
	control_system_.init();
	graphics_system_.init(window_width_, window_height_, "data/assets/");
	debug_system_.init(&graphics_system_);
    script_system_.init(&control_system_);
	gui_system_.init(window_width_, window_height_);

	/******** SHADERS **********/

	Shader* cubemap_shader = graphics_system_.loadShader("data/shaders/cubemap.vert", "data/shaders/cubemap.frag");
	Shader* phong_shader = graphics_system_.loadShader("data/shaders/phong.vert", "data/shaders/phong.frag");
	Shader* reflection_shader = graphics_system_.loadShader("data/shaders/reflection.vert", "data/shaders/reflection.frag");
	
	/******** MATERIALS **********/
	//basic blue material
    int mat_blue_check_index = graphics_system_.createMaterial();
	Material& mat_blue_check = graphics_system_.getMaterial(mat_blue_check_index);
	mat_blue_check.shader_id = phong_shader->program;
	mat_blue_check.diffuse_map = Parsers::parseTexture("data/assets/block_blue.tga");
	mat_blue_check.specular = lm::vec3(0, 0, 0);
    
    //basic grey material
    int mat_grey_index = graphics_system_.createMaterial();
    Material& mat_grey = graphics_system_.getMaterial(mat_grey_index);
    mat_grey.shader_id = phong_shader->program;
    mat_grey.specular = lm::vec3(0, 0, 0);
	mat_grey.normal_map = Parsers::parseTexture("data/assets/bricks_normal.tga");
	mat_grey.uv_scale = lm::vec2(50.0f,50.0f);
	mat_grey.normal_factor = 0.2f;
    
    //whole bunch of materials!
    Parsers::parseMTL("data/assets/nanosuit/", "nanosuit.mtl", graphics_system_.getMaterials(), phong_shader->program);

    /******** GEOMETRIES **********/
    
    int suit_geom = graphics_system_.createMultiGeometryFromFile("data/assets/nanosuit/nanosuit.obj");
    int cube_geom = graphics_system_.createGeometryFromFile("data/assets/open_room_inv.obj");

    
	//******** ENTITIES  **********/

    //cube
    int cube_entity = ECS.createEntity("cube");
    Transform& cube_trans = ECS.getComponentFromEntity<Transform>(cube_entity);
    cube_trans.scale(20.0f, 20.0f, 20.0f);
    cube_trans.translate(0, 10, 0);
    Mesh& cube_mesh = ECS.createComponentForEntity<Mesh>(cube_entity);
    cube_mesh.geometry = cube_geom;
    cube_mesh.material = mat_grey_index;
    cube_mesh.render_mode = RenderModeForward;
    
    //suit 
    int suit_entity = ECS.createEntity("Suit");
    Transform& st = ECS.getComponentFromEntity<Transform>(suit_entity);
    st.scale(0.4f, 0.4f, 0.4f);
    st.translate(-3.0f, 0.0f, 2.0f);
    st.rotate(-45.0f*DEG2RAD, lm::vec3(0.0f,1.0f,0.0f));
    Mesh& suit_mesh = ECS.createComponentForEntity<Mesh>(suit_entity);
    suit_mesh.geometry = suit_geom;
    suit_mesh.material = mat_blue_check_index;
    suit_mesh.render_mode = RenderModeForward;
    
    
    // direction/spot light
    int ent_light_dir = ECS.createEntity("light_dir");
    ECS.getComponentFromEntity<Transform>(ent_light_dir).translate(5, 9, 10);
    Light& light_comp_dir = ECS.createComponentForEntity<Light>(ent_light_dir);
    light_comp_dir.color = lm::vec3(1.99f, 1.99f, 1.99f);
    light_comp_dir.direction = lm::vec3(-0.5f,-0.5f,-0.5f);
    light_comp_dir.type = 2; //change for direction or spot
    light_comp_dir.linear_att = 0.0007f;
    light_comp_dir.quadratic_att = 0.00008f;
    light_comp_dir.spot_inner = 50.0f;
    light_comp_dir.spot_inner = 60.0f;
    light_comp_dir.position = lm::vec3(5.0f, 9.0f, 10.0f);
    light_comp_dir.forward = light_comp_dir.direction.normalize();
    light_comp_dir.setPerspective(60.0f*DEG2RAD, 1.0f, 5.0f, 50.0f);
    light_comp_dir.update();
    light_comp_dir.cast_shadow = true;
    
	//light
    int ent_light = ECS.createEntity("light1");
    ECS.getComponentFromEntity<Transform>(ent_light).translate(-8, 17, -8);
    Light& light_comp = ECS.createComponentForEntity<Light>(ent_light);
    light_comp.color = lm::vec3(0.0f, 1.0f, 1.0f);
    light_comp.type = 1;
    light_comp.linear_att = 0.22f;
    light_comp.quadratic_att = 0.20f;
    light_comp.calculateRadius();

    int ent_light1 = ECS.createEntity("light2");
    ECS.getComponentFromEntity<Transform>(ent_light1).translate(8, 17, -8);
    Light& light_comp1 = ECS.createComponentForEntity<Light>(ent_light1);
    light_comp1.color = lm::vec3(1.0f, 1.0f, 0.0f);
    light_comp1.type = 1;
    light_comp1.linear_att = 0.22f;
    light_comp1.quadratic_att = 0.20f;
    light_comp1.calculateRadius();

    int ent_light2 = ECS.createEntity("light3");
    ECS.getComponentFromEntity<Transform>(ent_light2).translate(-8, 1, -8);
    Light& light_comp2 = ECS.createComponentForEntity<Light>(ent_light2);
    light_comp2.color = lm::vec3(0.0f, 1.0f, 0.0f);
    light_comp2.type = 1;
    light_comp2.linear_att = 0.22f;
    light_comp2.quadratic_att = 0.20f;
    light_comp2.calculateRadius();

    int ent_light3 = ECS.createEntity("light4");
    ECS.getComponentFromEntity<Transform>(ent_light3).translate(8, 1, -8);
    Light& light_comp3 = ECS.createComponentForEntity<Light>(ent_light3);
    light_comp3.color = lm::vec3(1.0f, 0.0f, 1.0f);
    light_comp3.type = 1;
    light_comp3.linear_att = 0.22f;
    light_comp3.quadratic_att = 0.20f;
    light_comp3.calculateRadius();

	//create camera
	createFreeCamera_();
    
    //******* LATE INIT AFTER LOADING RESOURCES *******//
    graphics_system_.lateInit();
    script_system_.lateInit();
    debug_system_.lateInit();

	debug_system_.setActive(false);

}

//update each system in turn
void Game::update(float dt) {

	if (ECS.getAllComponents<Camera>().size() == 0) {print("There is no camera set!"); return;}

	//update input
	control_system_.update(dt);

	//collision
	collision_system_.update(dt);

	//scripts
	script_system_.update(dt);

	//render
	graphics_system_.update(dt);
    
	//gui
	gui_system_.update(dt);

	//debug
	debug_system_.update(dt);
   
}
//update game viewports
void Game::update_viewports(int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	auto& cameras = ECS.getAllComponents<Camera>();
	for (auto& cam : cameras) {
		cam.setPerspective(60.0f*DEG2RAD, (float)window_width_ / (float) window_height_, 0.01f, 10000.0f);
	}

	graphics_system_.updateMainViewport(window_width_, window_height_);
}


int Game::createFreeCamera_() {
	int ent_player = ECS.createEntity("PlayerFree");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	lm::vec3 the_position(0.0f, 5.0f, 15.0f);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(0, -0.3f, -1.0f);
	player_cam.setPerspective(60.0f*DEG2RAD, (float)window_width_/(float)window_height_, 0.1f, 10000.0f);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	control_system_.control_type = ControlTypeFree;

	return ent_player;
}

int Game::createPlayer_(float aspect, ControlSystem& sys) {
	int ent_player = ECS.createEntity("PlayerFPS");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	lm::vec3 the_position(0.0f, 3.0f, 5.0f);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(0.0f, 0.0f, -1.0f);
	player_cam.setPerspective(60.0f*DEG2RAD, aspect, 0.01f, 10000.0f);

	//FPS colliders 
	//each collider ray entity is parented to the playerFPS entity
	int ent_down_ray = ECS.createEntity("Down Ray");
	Transform& down_ray_trans = ECS.getComponentFromEntity<Transform>(ent_down_ray);
	down_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& down_ray_collider = ECS.createComponentForEntity<Collider>(ent_down_ray);
	down_ray_collider.collider_type = ColliderTypeRay;
	down_ray_collider.direction = lm::vec3(0.0, -1.0, 0.0);
	down_ray_collider.max_distance = 100.0f;

	int ent_left_ray = ECS.createEntity("Left Ray");
	Transform& left_ray_trans = ECS.getComponentFromEntity<Transform>(ent_left_ray);
	left_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& left_ray_collider = ECS.createComponentForEntity<Collider>(ent_left_ray);
	left_ray_collider.collider_type = ColliderTypeRay;
	left_ray_collider.direction = lm::vec3(-1.0, 0.0, 0.0);
	left_ray_collider.max_distance = 1.0f;

	int ent_right_ray = ECS.createEntity("Right Ray");
	Transform& right_ray_trans = ECS.getComponentFromEntity<Transform>(ent_right_ray);
	right_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& right_ray_collider = ECS.createComponentForEntity<Collider>(ent_right_ray);
	right_ray_collider.collider_type = ColliderTypeRay;
	right_ray_collider.direction = lm::vec3(1.0, 0.0, 0.0);
	right_ray_collider.max_distance = 1.0f;

	int ent_forward_ray = ECS.createEntity("Forward Ray");
	Transform& forward_ray_trans = ECS.getComponentFromEntity<Transform>(ent_forward_ray);
	forward_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& forward_ray_collider = ECS.createComponentForEntity<Collider>(ent_forward_ray);
	forward_ray_collider.collider_type = ColliderTypeRay;
	forward_ray_collider.direction = lm::vec3(0.0, 0.0, -1.0);
	forward_ray_collider.max_distance = 1.0f;

	int ent_back_ray = ECS.createEntity("Back Ray");
	Transform& back_ray_trans = ECS.getComponentFromEntity<Transform>(ent_back_ray);
	back_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& back_ray_collider = ECS.createComponentForEntity<Collider>(ent_back_ray);
	back_ray_collider.collider_type = ColliderTypeRay;
	back_ray_collider.direction = lm::vec3(0.0, 0.0, 1.0);
	back_ray_collider.max_distance = 1.0f;

	//the control system stores the FPS colliders 
	sys.FPS_collider_down = ECS.getComponentID<Collider>(ent_down_ray);
	sys.FPS_collider_left = ECS.getComponentID<Collider>(ent_left_ray);
	sys.FPS_collider_right = ECS.getComponentID<Collider>(ent_right_ray);
	sys.FPS_collider_forward = ECS.getComponentID<Collider>(ent_forward_ray);
	sys.FPS_collider_back = ECS.getComponentID<Collider>(ent_back_ray);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	sys.control_type = ControlTypeFPS;

	return ent_player;
}

