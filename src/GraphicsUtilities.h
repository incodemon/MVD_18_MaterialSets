#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
struct AABB {
	lm::vec3 center;
	lm::vec3 half_width;
};

struct Geometry {
	GLuint vao;
	GLuint num_tris;
	AABB aabb;
    
    
    std::vector<int> material_sets; //each entry is upper limit of set, in TRIANGLES
    std::vector<int> material_set_ids; //each entry is id of material for material set
    
    void render(int set); //new render function - render only certain amount of faces
	
	//constrctors
	Geometry() { vao = 0; num_tris = 0; }
	Geometry(int a_vao, int a_tris) : vao(a_vao), num_tris(a_tris) {}
	Geometry(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned int>& indices);
	
	//creation functions
	void createVertexArrays(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned int>& indices);
    void createTangentArrays(std::vector<float>& vertices,
                             std::vector<float>& uvs,
                             std::vector<float>& normals,
                             std::vector<unsigned int>& indices,
                             std::vector<float>& out_tangent
                             );
    
	void setAABB(std::vector<GLfloat>& vertices);
	int createPlaneGeometry();

	//rendering functions
    void createMaterialSet(int tri_count, int material_id);
    void render();

};

struct Material {
	std::string name;
	int index = -1;
	int shader_id;
	lm::vec3 ambient;
	lm::vec3 diffuse;
	lm::vec3 specular;
	float specular_gloss;

	int diffuse_map;
	int cube_map;
    int normal_map; //NEW!!
    int specular_map; //NEW!!
    float normal_factor; //how strong are out normals
    
    lm::vec2 uv_scale;;

	Material() {
		name = "";
		ambient = lm::vec3(0.1f, 0.1f, 0.1f);
		diffuse = lm::vec3(1.0f, 1.0f, 1.0f);
		specular = lm::vec3(1.0f, 1.0f, 1.0f);
		diffuse_map = -1;
		cube_map = -1;
        normal_map = -1;
        specular_map = -1;
        normal_factor = 1.0f;
		specular_gloss = 80.0f;
        uv_scale = lm::vec2(1.0f, 1.0f);
	}
};

struct Framebuffer {
	GLuint width, height;
	GLuint framebuffer = -1;
	GLuint num_color_attachments = 0;
	GLuint color_textures[10] = { 0,0,0,0,0,0,0,0,0,0 };
	void bindAndClear();
    void bindAndClear(lm::vec4 clear_color);
	void initColor(GLsizei width, GLsizei height);
	void initDepth(GLsizei width, GLsizei height);
    void initGbuffer(GLsizei width, GLsizei height);
};

