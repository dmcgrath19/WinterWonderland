#pragma warning(disable : 5208)

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "maths.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define	AMBIENT	"sound/reg.wav"
#pragma comment(lib, "Winmm.lib")


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
//#define MESH_NAME "candyCane.obj"
#define HEAD "snowman/top.dae"
#define HAT "snowman/hat.dae"
#define BROWS "snowman/brows.dae"
#define BOTTOM "snowman/bottom.dae"
#define LARM "snowman/larm.dae"
#define RARM "snowman/rarm.dae"
#define MID "snowman/mid.dae"
#define HAIR "snowman/hair.dae"
#define NOSE "snowman/nose.dae"
#define EYES "snowman/eyes.dae"

#define CANDY_CANE "scene/candycane.dae"
#define MESH_NAME "woodenhouse.obj"
#define HOUSE "woodenhouse.obj"

#define BENCH "scene/bench.dae"
#define CABIN "scene/cabin.dae"
#define LOLIPOP "scene/lolipop.dae"
#define STARS "scene/stars.dae"
#define SLED "scene/sled.dae"
#define MOON "scene/moon.dae"

#define LWING "bird/lwing.dae"
#define RWING "bird/rwing.dae"
#define BASE "bird/base.dae"

//#define BENCH_TEX "scene/bench.PNG"
//#define CABIN_TEX "scene/cabin.jpg"
#define LOLIPOP_TEX "scene/lolipop.jpg"
#define MOON_TEX "scene/moon.jpeg"
#define STARS_TEX "scene/stars.jpeg"
#define SLED_TEX "scene/sled.png"
#define SKY "scene/skybox.dae"


#define SNOWmd "scene/snow_tex.jpg"
#define CANDY_TEX "scene/candycane.jpg"
#define SKY_TEX "scene/skyy.jpeg"

#define TOP_TEX "snowman/top.jpeg"
#define MID_TEX "snowman/mid.jpeg"
#define BOTTOM_TEX "snowman/bottom.jpeg"
#define LARM_TEX "snowman/larm.jpeg"
#define RARM_TEX "snowman/rarm.jpeg"
#define NOSE_TEX "snowman/nose.jpeg"
#define HAT_TEX "snowman/hat.jpeg"
#define HAIR_TEX "snowman/hair.jpeg"
#define BROWS_TEX "snowman/brows.jpeg"
#define EYES_TEX "snowman/eyes.jpeg"

#define CCANE "scene/ccane.dae"
#define CBLUE "scene/cblue.jpg"
#define CORANGE "scene/corange.jpg"
#define CGREEN "scene/cgreen.jpg"
#define CPURPLE "scene/cpurple.jpg"
#define CRED "scene/cred.jpg"

#define BG_NAME		"scene/snow.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/


unsigned int snow_tex, candycane_tex, sky_tex;
unsigned int top_tex, bottom_tex, mid_tex, larm_tex, rarm_tex, nose_tex, hat_tex, hair_tex, brows_tex, eyes_tex;
unsigned int bench_tex, cabin_tex, lolipop_tex, moon_tex, stars_tex, sled_tex;
unsigned int cblue, cgreen, cpurple, corange, cyellow, cred;
// Scaling
float scaling_x = 1.4f;
float scaling_y = 1.4f;
float scaling_z = 1.4f;
// Camera
float camera_x = 0.0f;
float camera_y = 5.0f;
float camera_z = 15.0f;
float target_x = 0.0f;
float target_y = 0.0f;
float target_z = 0.0f;
vec3 camera_pos = vec3(0.0f, 0.0f, 0.0f);		// initial position of eye
vec3 camera_target = vec3(0.0f, 0.0f, 0.0f);	// initial position of target
vec3 up = vec3(0.0f, 1.0f, 0.0f);				// up vector
// Mouse Movement
float mouse_x = 0.0f;
float mouse_y = 0.0f;
float old_x = 0.0f;
float old_y = 0.0f;
float forward_x = 0.0f;
float forward_y = 0.0f;
float forward_z = 0.0f;
float left_x = 0.0f;
float left_y = 0.0f;
float left_z = 0.0f;
float right_x = 0.0f;
float right_y = 0.0f;
float right_z = 0.0f;
float speed = 0.01f;

GLfloat rotate_y = 0.0f;
GLfloat rotate_x = 0.0f;
GLfloat rotate_z = 0.0f;


float translation_x = -2.0f;
float translation_y = 0.05f;
float translation_z = 2.0f;

float adjust_translation_y = .0f;
float adjust_translation_x = .0f;
bool forward = false;

#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes
// Buffers
unsigned int VP_VBOs[19]; // vertex positions
unsigned int VN_VBOs[19]; // vertex normals
unsigned int VT_VBOs[19]; // vertex textures
unsigned int VAOs[1];
int count = 10000;

//Shader objectShader;
//Shader skyboxShader;

GLuint loc1[10 * 3];
GLuint loc2[6 * 3];
GLuint loc3[3 * 3];


//Shader Variables
GLuint shaderProgramID;
//GLuint loc1, loc2, loc3;

ModelData mesh_data;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

ModelData house, candycane;
ModelData parent, child;
ModelData background;

ModelData head, bottom, mid;
ModelData brows;
ModelData larm, rarm;
ModelData hat;
ModelData eyes, nose, hair;

ModelData lolipop, cabin, moon, stars, sled, bench, cane, sky, lwing, rwing, base;

//Perspective Transform Variables
bool keyStates[256];
bool persp = true;

//Model Transform Variables
glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
//GLfloat rotate_y = 0.0f;
//glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
//rotation
// Rotations
float tail_rotation = 0.0f;
float hat_rotation = 0.0f;
float tree_rotation = 0.1f;
float model_rotation = 0.0f;
//animations

boolean left_up = true;
boolean right_up = false;
float tail_z = 0.0f;
float tree_z = 0.0f;
float leg_l_y = 0.0f;
float leg_r_y = 0.0f;
float leg_translation = 0.0f;
// Scaling

mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);


#define MAX_PARTICLES 1000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2


float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData LoadMesh(const char* file_name)
{
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene)
	{
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++)
	{
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++)
		{
			if (mesh->HasPositions())
			{
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals())
			{
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0))
			{
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents())
			{
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}
#pragma endregion MESH LOADING

#pragma region TEXTURE LOADING
unsigned int load_tex(const char* file_name) {
	// load and create a texture 
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
#pragma endregion TEXTURE LOADING
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");
	if (fp == NULL) { return NULL; }
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';
	fclose(fp);
	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();

	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "Shaders/combinedSV.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "Shaders/combinedFS.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

#pragma region VBO_FUNCTIONS
void GenerateObjectBufferMesh()
{
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/
	parent = LoadMesh(MESH_NAME);
	//cabin = LoadMesh(CABIN);
	bench = LoadMesh(STARS);
	lolipop = LoadMesh(LOLIPOP);
	sled = LoadMesh(SLED);
	cane = LoadMesh(CCANE);
	sky = LoadMesh(SKY);
	base = LoadMesh(BASE);
	lwing = LoadMesh(LWING);
	rwing = LoadMesh(RWING);

	sky_tex = load_tex(SKY_TEX);
	snow_tex = load_tex(SNOWmd);
	candycane_tex = load_tex(CANDY_TEX);
	top_tex = load_tex(TOP_TEX);
	bottom_tex = load_tex(BOTTOM_TEX);
	mid_tex = load_tex(MID_TEX);
	nose_tex = load_tex(NOSE_TEX);
	brows_tex = load_tex(BROWS_TEX);
	larm_tex = load_tex(LARM_TEX);
	rarm_tex = load_tex(RARM_TEX);
	hair_tex = load_tex(HAIR_TEX);
	hat_tex = load_tex(HAT_TEX);
	eyes_tex = load_tex(EYES_TEX);
	//cabin_tex = load_tex(CABIN_TEX);
	lolipop_tex = load_tex(LOLIPOP_TEX);
	moon_tex = load_tex(MOON_TEX);
	//stars_tex = load_tex(STARS_TEX);
	bench_tex = load_tex(STARS_TEX);
	sled_tex = load_tex(SLED_TEX);
	cblue = load_tex(CBLUE);
	cgreen = load_tex(CGREEN);
	cpurple = load_tex(CPURPLE);
	corange = load_tex(CORANGE);
	cred = load_tex(CRED);
	
	background = LoadMesh(BG_NAME);
	candycane = LoadMesh(CANDY_CANE);

	head = LoadMesh(HEAD);
	bottom = LoadMesh(BOTTOM);
	mid = LoadMesh(MID);
	brows = LoadMesh(BROWS);
	larm = LoadMesh(LARM);
	rarm = LoadMesh(RARM);
	hat = LoadMesh(HAT);
	hair = LoadMesh(HAIR);
	eyes = LoadMesh(EYES);
	nose = LoadMesh(NOSE);
	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	glGenBuffers(15, VP_VBOs); // position buffer
	glGenBuffers(15, VN_VBOs); // normal buffer
	glGenBuffers(15, VT_VBOs); // texture buffer

	//bottom of snowman
	loc1[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, bottom.mPointCount * sizeof(vec3), &bottom.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, bottom.mPointCount * sizeof(vec3), &bottom.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, bottom.mPointCount * sizeof(vec2), &bottom.mTextureCoords[0], GL_STATIC_DRAW);

	//mid of snowman
	loc1[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, mid.mPointCount * sizeof(vec3), &mid.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, mid.mPointCount * sizeof(vec3), &mid.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, mid.mPointCount * sizeof(vec2), &mid.mTextureCoords[0], GL_STATIC_DRAW);

	//head of snowman
	loc1[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, head.mPointCount * sizeof(vec3), &head.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, head.mPointCount * sizeof(vec3), &head.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, head.mPointCount * sizeof(vec2), &head.mTextureCoords[0], GL_STATIC_DRAW);

	//larm of snowman
	loc1[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, larm.mPointCount * sizeof(vec3), &larm.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, larm.mPointCount * sizeof(vec3), &larm.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, larm.mPointCount * sizeof(vec2), &larm.mTextureCoords[0], GL_STATIC_DRAW);

	//rarm of snowman
	loc1[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, rarm.mPointCount * sizeof(vec3), &rarm.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, rarm.mPointCount * sizeof(vec3), &rarm.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, rarm.mPointCount * sizeof(vec2), &rarm.mTextureCoords[0], GL_STATIC_DRAW);

	//brows of snowman
	loc1[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, brows.mPointCount * sizeof(vec3), &brows.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, brows.mPointCount * sizeof(vec3), &brows.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, brows.mPointCount * sizeof(vec2), &brows.mTextureCoords[0], GL_STATIC_DRAW);

	//hat of snowman
	loc1[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, hat.mPointCount * sizeof(vec3), &hat.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, hat.mPointCount * sizeof(vec3), &hat.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, hat.mPointCount * sizeof(vec2), &hat.mTextureCoords[0], GL_STATIC_DRAW);

	//hair of snowman
	loc1[21] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[22] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[23] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[7]);
	glBufferData(GL_ARRAY_BUFFER, hair.mPointCount * sizeof(vec3), &hair.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[7]);
	glBufferData(GL_ARRAY_BUFFER, hair.mPointCount * sizeof(vec3), &hair.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[7]);
	glBufferData(GL_ARRAY_BUFFER, hair.mPointCount * sizeof(vec2), &hair.mTextureCoords[0], GL_STATIC_DRAW);

	//nose of snowman
	loc1[24] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[25] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[26] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[11]);
	glBufferData(GL_ARRAY_BUFFER, nose.mPointCount * sizeof(vec3), &nose.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[11]);
	glBufferData(GL_ARRAY_BUFFER, nose.mPointCount * sizeof(vec3), &nose.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[11]);
	glBufferData(GL_ARRAY_BUFFER, nose.mPointCount * sizeof(vec2), &nose.mTextureCoords[0], GL_STATIC_DRAW);

	//eyes of snowman
	loc1[27] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[28] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[29] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[12]);
	glBufferData(GL_ARRAY_BUFFER, eyes.mPointCount * sizeof(vec3), &eyes.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[12]);
	glBufferData(GL_ARRAY_BUFFER, eyes.mPointCount * sizeof(vec3), &eyes.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[12]);
	glBufferData(GL_ARRAY_BUFFER, eyes.mPointCount * sizeof(vec2), &eyes.mTextureCoords[0], GL_STATIC_DRAW);

	
	// Scene - Ground
	loc2[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[8]);
	glBufferData(GL_ARRAY_BUFFER, background.mPointCount * sizeof(vec3), &background.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[8]);
	glBufferData(GL_ARRAY_BUFFER, background.mPointCount * sizeof(vec3), &background.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[8]);
	glBufferData(GL_ARRAY_BUFFER, background.mPointCount * sizeof(vec2), &background.mTextureCoords[0], GL_STATIC_DRAW);


	//bench
	loc2[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[9]);
	glBufferData(GL_ARRAY_BUFFER, bench.mPointCount * sizeof(vec3), &bench.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[9]);
	glBufferData(GL_ARRAY_BUFFER, bench.mPointCount * sizeof(vec3), &bench.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[9]);
	glBufferData(GL_ARRAY_BUFFER, bench.mPointCount * sizeof(vec2), &bench.mTextureCoords[0], GL_STATIC_DRAW);

	//candycane
	loc2[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[10]);
	glBufferData(GL_ARRAY_BUFFER, candycane.mPointCount * sizeof(vec3), &candycane.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[10]);
	glBufferData(GL_ARRAY_BUFFER, candycane.mPointCount * sizeof(vec3), &candycane.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[10]);
	glBufferData(GL_ARRAY_BUFFER, candycane.mPointCount * sizeof(vec2), &candycane.mTextureCoords[0], GL_STATIC_DRAW);

	//lolipop
	loc2[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[13]);
	glBufferData(GL_ARRAY_BUFFER, lolipop.mPointCount * sizeof(vec3), &lolipop.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[13]);
	glBufferData(GL_ARRAY_BUFFER, lolipop.mPointCount * sizeof(vec3), &lolipop.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[13]);
	glBufferData(GL_ARRAY_BUFFER, lolipop.mPointCount * sizeof(vec2), &lolipop.mTextureCoords[0], GL_STATIC_DRAW);

	//colouredcane
	loc2[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glBufferData(GL_ARRAY_BUFFER, cane.mPointCount * sizeof(vec3), &cane.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glBufferData(GL_ARRAY_BUFFER, cane.mPointCount * sizeof(vec3), &cane.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glBufferData(GL_ARRAY_BUFFER, cane.mPointCount * sizeof(vec2), &cane.mTextureCoords[0], GL_STATIC_DRAW);

	//box
	loc2[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[15]);
	glBufferData(GL_ARRAY_BUFFER, sky.mPointCount * sizeof(vec3), &sky.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[15]);
	glBufferData(GL_ARRAY_BUFFER, sky.mPointCount * sizeof(vec3), &sky.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[15]);
	glBufferData(GL_ARRAY_BUFFER, sky.mPointCount * sizeof(vec2), &sky.mTextureCoords[0], GL_STATIC_DRAW);

}
#pragma endregion VBO_FUNCTIONS

void renderSnowman(int proj_mat_location, int view_mat_location, mat4 view, int matrix_location, int alterx, int altery) {
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	// Root - Snowman - bottom
	mat4 model = identity_mat4();
	model = rotate_y_deg(model, rotate_y);
	model = rotate_x_deg(model, rotate_x);
	model = rotate_z_deg(model, rotate_z);
	model = translate(model, vec3(translation_x-alterx+adjust_translation_x-5, translation_y-.7, translation_z + altery+adjust_translation_y-2));
	//glTranslatef(-alterx, -10, 0);
	model = scale(model, vec3(scaling_x, scaling_y, scaling_z));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.6f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glEnableVertexAttribArray(loc1[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[0]);
	glVertexAttribPointer(loc1[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[0]);
	glVertexAttribPointer(loc1[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[0]);
	glVertexAttribPointer(loc1[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bottom_tex);
	glDrawArrays(GL_TRIANGLES, 0, bottom.mPointCount);

	// snowman - mid
	mat4 modelChild = identity_mat4();
	modelChild = rotate_y_deg(modelChild, 0.0f); // only rotates on z axis-s
	modelChild = rotate_x_deg(modelChild, 0.0f);
	modelChild = rotate_z_deg(modelChild, tail_z);
	modelChild = translate(modelChild, vec3(0.0f, 0.0f, 0.0f));
	modelChild = model * modelChild;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.2f);
	glEnableVertexAttribArray(loc1[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[1]);
	glVertexAttribPointer(loc1[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[1]);
	glVertexAttribPointer(loc1[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[1]);
	glVertexAttribPointer(loc1[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, mid_tex);
	glDrawArrays(GL_TRIANGLES, 0, mid.mPointCount);

	// snowman - top
	mat4 modelLids = identity_mat4();
	modelLids = rotate_y_deg(modelLids, 0.0f); // only rotates on z axis-s
	modelLids = rotate_x_deg(modelLids, 0.0f);
	modelLids = rotate_z_deg(modelLids, -tail_z / 2);
	modelLids = translate(modelLids, vec3(0.0f, 0.0f, 0.0f));
	modelLids = modelChild * modelLids;
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.4f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelLids.m);
	glEnableVertexAttribArray(loc1[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[2]);
	glVertexAttribPointer(loc1[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[2]);
	glVertexAttribPointer(loc1[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[2]);
	glVertexAttribPointer(loc1[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, top_tex);
	glDrawArrays(GL_TRIANGLES, 0, head.mPointCount);

	//snowman left arm
	mat4 le = identity_mat4();
	le = rotate_y_deg(le, 0.0f); // only rotates on z axis-s
	le = rotate_x_deg(le, 0.0f);
	le = rotate_z_deg(le, tail_z);
	le = translate(le, vec3(0.0f, 0.0f, 0.0f));
	le = modelChild * le;
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.01f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, le.m);
	glEnableVertexAttribArray(loc1[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[3]);
	glVertexAttribPointer(loc1[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[3]);
	glVertexAttribPointer(loc1[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[3]);
	glVertexAttribPointer(loc1[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, larm_tex);
	glDrawArrays(GL_TRIANGLES, 0, larm.mPointCount);

	//snowman right arm
	mat4 rightarmmodel = identity_mat4();
	//rightarmmodel = translate(rightarmmodel, vec3(0.0f, leg_r_y, 0.0f));
	rightarmmodel = modelChild * rightarmmodel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, rightarmmodel.m);
	glEnableVertexAttribArray(loc1[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[4]);
	glVertexAttribPointer(loc1[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[4]);
	glVertexAttribPointer(loc1[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[4]);
	glVertexAttribPointer(loc1[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, rarm_tex);
	glDrawArrays(GL_TRIANGLES, 0, rarm.mPointCount);

	//snowman brows & lips
	mat4 wmodel = identity_mat4();
	wmodel = translate(wmodel, vec3(0.0f, leg_r_y, 0.0f));
	wmodel = modelLids * wmodel;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wmodel.m);
	glEnableVertexAttribArray(loc1[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[5]);
	glVertexAttribPointer(loc1[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[5]);
	glVertexAttribPointer(loc1[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[5]);
	glVertexAttribPointer(loc1[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, brows_tex);
	glDrawArrays(GL_TRIANGLES, 0, brows.mPointCount);

	//snowman hat
	mat4 hatmodel = identity_mat4();
	hatmodel = rotate_y_deg(hatmodel, -model_rotation); // only rotates on z axis-s
	hatmodel = rotate_x_deg(hatmodel, 0.0f);
	hatmodel = rotate_z_deg(hatmodel, 0.0f);
	hatmodel = translate(hatmodel, vec3(0.0f, 0.0f, 0.0f));
	hatmodel = modelLids * hatmodel;
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.91f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, hatmodel.m);
	glEnableVertexAttribArray(loc1[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[6]);
	glVertexAttribPointer(loc1[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[6]);
	glVertexAttribPointer(loc1[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[6]);
	glVertexAttribPointer(loc1[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, hat_tex);
	glDrawArrays(GL_TRIANGLES, 0, hat.mPointCount);

	//snowman hair
	mat4 hairmodel = identity_mat4();
	hairmodel = rotate_y_deg(hairmodel, -model_rotation); // only rotates on z axis-s
	hairmodel = rotate_x_deg(hairmodel, 0.0f);
	hairmodel = rotate_z_deg(hairmodel, 0.0f);
	hairmodel = translate(hairmodel, vec3(0.0f, 0.0f, 0.0f));
	hairmodel = modelLids * hairmodel;
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.01f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, hairmodel.m);
	glEnableVertexAttribArray(loc1[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[7]);
	glVertexAttribPointer(loc1[21], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[7]);
	glVertexAttribPointer(loc1[22], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[7]);
	glVertexAttribPointer(loc1[23], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, hair_tex);
	glDrawArrays(GL_TRIANGLES, 0, hair.mPointCount);

	//snowman nose 11
	mat4 nosemodel = identity_mat4();
	nosemodel = rotate_y_deg(nosemodel, -model_rotation); // only rotates on z axis-s
	nosemodel = rotate_x_deg(nosemodel, 0.0f);
	nosemodel = rotate_z_deg(nosemodel, 0.0f);
	nosemodel = translate(nosemodel, vec3(0.0f, 0.0f, 0.0f));
	nosemodel = modelLids * nosemodel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, nosemodel.m);
	glEnableVertexAttribArray(loc1[24]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[11]);
	glVertexAttribPointer(loc1[24], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[25]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[11]);
	glVertexAttribPointer(loc1[25], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[26]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[11]);
	glVertexAttribPointer(loc1[26], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, nose_tex);
	glDrawArrays(GL_TRIANGLES, 0, nose.mPointCount);

	//snowman eyes 12
	mat4 eyesmodel = identity_mat4();
	eyesmodel = rotate_y_deg(eyesmodel, -model_rotation); // only rotates on z axis-s
	eyesmodel = rotate_x_deg(eyesmodel, 0.0f);
	eyesmodel = rotate_z_deg(eyesmodel, 0.0f);
	eyesmodel = translate(eyesmodel, vec3(0.0f, 0.0f, 0.0f));
	eyesmodel = modelLids * eyesmodel;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, eyesmodel.m);
	glEnableVertexAttribArray(loc1[27]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[12]);
	glVertexAttribPointer(loc1[27], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[28]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[12]);
	glVertexAttribPointer(loc1[28], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc1[29]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[12]);
	glVertexAttribPointer(loc1[29], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, eyes_tex);
	glDrawArrays(GL_TRIANGLES, 0, eyes.mPointCount);

}

void renderCandyCane(int proj_mat_location, int view_mat_location, mat4 view, int matrix_location, int alterx, int altery, int scalen) {
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	float r = rand()%100;
	mat4 cane = identity_mat4();
	cane = rotate_y_deg(cane, 0.0f);
	cane = rotate_x_deg(cane, 0.0f);
	cane = rotate_z_deg(cane, 0.0f);
	cane = translate(cane, vec3(-14.0f - alterx, 8.0f, -6.0f - altery));
	cane = scale(cane, vec3(1.0f, 1.0f,  1.0f));
	glColor3f(1, 0, 0);
	//glBindTexture(GL_ARRAY_BUFFER, snow_tex);
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.8f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cane.m);
	glEnableVertexAttribArray(loc2[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[10]);
	glVertexAttribPointer(loc2[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[10]);
	glVertexAttribPointer(loc2[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[10]);
	glVertexAttribPointer(loc2[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, candycane_tex);
	glDrawArrays(GL_TRIANGLES, 0, candycane.mPointCount);
}

void Display()
{
	// Tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthMask(GL_TRUE); //update the depth buffer
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // sky colour - same colour as fog
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	// Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	// Camera/View
	mat4 view = identity_mat4();
	camera_target = vec3(target_x, target_y, target_z);
	camera_pos = vec3(camera_x, camera_y, camera_z);
	view = look_at(camera_pos, camera_target, up);

	glUniform3f(glGetUniformLocation(shaderProgramID, "ViewPos"), target_x, target_y, target_z);
	
	// Update Uniforms & Draw

	//snowman 1:
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 0, 0);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 0, -5);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 3, -3);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 2, -7);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 6, -8);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 7, -4);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 4, -10);
	renderSnowman(proj_mat_location, view_mat_location, view, matrix_location, 4, 0);



	mat4 bg = identity_mat4();
	
	bg = rotate_y_deg(bg, 300.0f);
	bg = rotate_x_deg(bg, 0.0f);
	bg = rotate_z_deg(bg, 0.0f);
	
	//bg = scale(bg, vec3(114.5f, 114.5f, 114.5f));
	bg = scale(bg, vec3(40, 0, 10));
	glColor3f(1, 0, 0);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, bg.m);
	glEnableVertexAttribArray(loc2[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[8]);
	glVertexAttribPointer(loc2[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[8]);
	glVertexAttribPointer(loc2[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[8]);
	glVertexAttribPointer(loc2[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, snow_tex);
	glDrawArrays(GL_TRIANGLES, 0, background.mPointCount);






	mat4 loghouse = identity_mat4();
	glColor3f(1, 0, 0);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, loghouse.m);
	glEnableVertexAttribArray(loc2[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[9]);
	glVertexAttribPointer(loc2[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[9]);
	glVertexAttribPointer(loc2[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[9]);
	glVertexAttribPointer(loc2[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, bench_tex);
	glDrawArrays(GL_TRIANGLES, 0, bench.mPointCount);

	mat4 lol = identity_mat4();
	glColor3f(1, 0, 0);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lol.m);
	glEnableVertexAttribArray(loc2[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[13]);
	glVertexAttribPointer(loc2[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[13]);
	glVertexAttribPointer(loc2[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[13]);
	glVertexAttribPointer(loc2[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, lolipop_tex);
	glDrawArrays(GL_TRIANGLES, 0, lolipop.mPointCount);

	mat4 c = identity_mat4();
	glColor3f(1, 0, 0);
	c = translate(c, vec3(10.0f, 0, -10.0f));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.0f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, c.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, cblue);
	glDrawArrays(GL_TRIANGLES, 0, cane.mPointCount);

	mat4 c2 = identity_mat4();
	glColor3f(1, 0, 0);
	c2 = translate(c, vec3(17.0f, 0, -10.0f));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 1.0f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, c2.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, cblue);
	glDrawArrays(GL_TRIANGLES, 0, cane.mPointCount);

	mat4 c3 = identity_mat4();
	glColor3f(1, 0, 0);
	c3 = translate(c, vec3(10.0f, 0, -7.0f));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.2f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, c3.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, cblue);
	glDrawArrays(GL_TRIANGLES, 0, cane.mPointCount);

	mat4 c4 = identity_mat4();
	glColor3f(1, 0, 0);
	c4 = translate(c, vec3(10.0f, 0, -5.0f));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), 0.4f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, c4.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, cblue);
	glDrawArrays(GL_TRIANGLES, 0, cane.mPointCount);

	mat4 c5 = identity_mat4();
	glColor3f(1, 0, 0);
	c5 = translate(c, vec3(7.0f , 0, -5.0f));
	glUniform1f(glGetUniformLocation(shaderProgramID, "material.shininess"), .8f);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, c5.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[14]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[14]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[14]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, cblue);
	glDrawArrays(GL_TRIANGLES, 0, cane.mPointCount);



	mat4 ccane = identity_mat4();
	ccane = rotate_y_deg(ccane, rotate_y);
	ccane = rotate_x_deg(ccane, rotate_x);
	ccane = rotate_z_deg(ccane, rotate_z);
	ccane = translate(ccane, vec3(0.0f, 10+ leg_r_y, 20.0f));
	glColor3f(1, 0, 0);
	//glBindTexture(GL_ARRAY_BUFFER, snow_tex);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ccane.m);
	glEnableVertexAttribArray(loc2[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBOs[10]);
	glVertexAttribPointer(loc2[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBOs[10]);
	glVertexAttribPointer(loc2[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(loc2[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBOs[10]);
	glVertexAttribPointer(loc2[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glDrawArrays(GL_TRIANGLES, 0, candycane.mPointCount);
	for (int i = 0; i < 50; i++) {
		if (i % 2 == 0) {
			renderCandyCane(proj_mat_location, view_mat_location, view, matrix_location, 10 - i * 2, 0 + i * 2, (i / 50) * 1.0f);
			renderCandyCane(proj_mat_location, view_mat_location, view, matrix_location, 10 - i * .5, 0 + i * 1.5, (i * 2 / 50) * 1.0f);
		}
		else {
			renderCandyCane(proj_mat_location, view_mat_location, view, matrix_location, 10 - i * 1., 0 + i * 1.5, (i / 50) * 1.0f);
			renderCandyCane(proj_mat_location, view_mat_location, view, matrix_location, 10 + i * 2, 0 + i * 1.5, (i * 2 / 50) * 1.0f);
		}
	}
	int i, j;
	float x, y, z;



	
		//drawSnow();
		
	

	glutSwapBuffers();
}

void SwapProjection()
{
	if (keyStates['p'])
	{
		persp = true;
	}
	else if (keyStates['o'])
	{
		persp = false;
	}
}

void RotateModel(float delta)
{
	float speed = 20.0f;
	if (keyStates['-'])
	{
		speed = -speed;
	}

	if (keyStates['r'])
	{
		if (keyStates['x'])
		{
			rotate.x += speed * delta;
			rotate.x = fmodf(rotate.x, 360.0f);
		}
		if (keyStates['y'])
		{
			rotate.y += speed * delta;
			rotate.y = fmodf(rotate.y, 360.0f);
		}
		if (keyStates['z'])
		{
			rotate.z += speed * delta;
			rotate.z = fmodf(rotate.z, 360.0f);
		}
	}
	
	rotate_y += speed * delta;
	rotate_y = fmodf(rotate_y, 360.0f);
	
}
void translateModel() {
	if (!forward) {
		adjust_translation_x += .0003f;
		adjust_translation_y += .0003f;
		if (adjust_translation_y >= 3.0f)
			forward = true;
	}
	else  if(forward) {
		adjust_translation_x -= .0003f;
		adjust_translation_y -= .0003f;
		count++;
		if (adjust_translation_y <= .0f)
			forward = false;
	}
}


void UpdateScene()
{
	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Proximity Calculation
	vec3 dist = vec3(translation_x, translation_y, translation_z) - vec3(camera_x, camera_y, camera_z);
	float x_distance = dist.v[0];
	float z_distance = dist.v[2];

	//Projection Transformation
	SwapProjection();
	translateModel();
	//Model Transformation
	RotateModel(delta);

//	if (x_distance > -5 && x_distance < 5 && z_distance > -5 && z_distance < 5) {
		// Rotate hat
		tail_z += tail_rotation * delta;
		if (fmodf(tail_z, 15.0f) > 0) tail_rotation = -30.0f;
		if (tail_z / 15.0f <= -1) tail_rotation = 30.0f;
		// Wiggle Legs
		if (left_up) {
			leg_l_y += leg_translation;
			if (leg_r_y > 0.0f) leg_r_y -= leg_translation;
			if (leg_l_y >= .15f) {
				left_up = false;
				right_up = true;
			}
		}
		if (right_up) {
			leg_r_y += leg_translation;
			if (leg_l_y > 0.0f) leg_l_y -= leg_translation;
			if (leg_r_y >= .15f) {
				left_up = true;
				right_up = false;
			}
		//}
	}
	// Rotate Model
	//rotate_x += model_rotation * delta;
	rotate_x = fmodf(rotate_x, 360.0f);
	// Rotate Trees (very slight)
	//tree_z += tree_rotation * delta;
	// Draw the next frame
	glutPostRedisplay();

	//Escape Window
	if (keyStates[27]) {
		glutWarpPointer(width + 10, -10);
	}

	// Draw the next frame
	glutPostRedisplay();
}

void KeyDown(unsigned char key, int x, int y)
{
	keyStates[key] = true;
}

void KeyUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 21) - 10;
	par_sys[i].ypos = 10.0;
	par_sys[i].zpos = (float)(rand() % 21) - 10;

	par_sys[i].red = 0.5;
	par_sys[i].green = 0.5;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.8;//-0.8;

}
void drawSnow() {
	float x, y, z;
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
			glTranslatef(x, y, z);
			glutSolidSphere(0.2, 16, 16);
			glPopMatrix();

			// Update values
			//Move
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				int zi = z - zoom + 10;
				int xi = x + 10;
				ground_colors[zi][xi][0] = 1.0;
				ground_colors[zi][xi][2] = 1.0;
				ground_colors[zi][xi][3] += 1.0;
				if (ground_colors[zi][xi][3] > 1.0) {
					ground_points[xi][zi][1] += 0.1;
				}
				par_sys[loop].life = -1.0;
			}

			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}
void Init()
{
	int x, z;
	GLuint shaderProgramID = CompileShaders();
	//texture
	glEnable(GL_TEXTURE_2D);

	//Setup:
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	
	GenerateObjectBufferMesh();
	for (z = 0; z < 21; z++) {
		for (x = 0; x < 21; x++) {
			ground_points[x][z][0] = x - 10.0;
			ground_points[x][z][1] = accum;
			ground_points[x][z][2] = z - 10.0;

			ground_colors[z][x][0] = r; // red value
			ground_colors[z][x][1] = g; // green value
			ground_colors[z][x][2] = b; // blue value
			ground_colors[z][x][3] = 0.0; // acummulation factor
		}
	}

	// Initialize particles
	for (loop = 0; loop < MAX_PARTICLES; loop++) {
		initParticles(loop);
	}
}


void arrows(int key, int x, int y) {

	if (key == GLUT_KEY_DOWN) {
		translation_y -= 0.1;
	}
	if (key == GLUT_KEY_UP) {
		translation_y += 0.1;
	}
	if (key == GLUT_KEY_LEFT) {
		translation_x -= 0.1;
	}
	if (key == GLUT_KEY_RIGHT) {
		translation_x += 0.1;
	}
	glutPostRedisplay();
}

void keypress(unsigned char key, int x, int y) {

	// camera motions
	if (key == 'w') {	// Move camera forwards
		camera_x += forward_x * speed;
		camera_z += forward_z * speed;
		target_x += forward_x * speed;
		target_z += forward_z * speed;
	}
	if (key == 'a') {	// Move camera left
		vec3 left = cross(vec3(forward_x, forward_y, forward_z), up);
		camera_x -= left.v[0] * speed;
		camera_z -= left.v[2] * speed;
		target_x -= left.v[0] * speed;
		target_z -= left.v[2] * speed;
	}
	if (key == 's') {	// Move camera backwards
		camera_x -= forward_x * speed;
		camera_z -= forward_z * speed;
		target_x -= forward_x * speed;
		target_z -= forward_z * speed;
	}
	if (key == 'd') {	// Move camera left
		vec3 right = cross(vec3(forward_x, forward_y, forward_z), up);
		camera_x += right.v[0] * speed;
		camera_z += right.v[2] * speed;
		target_x += right.v[0] * speed;
		target_z += right.v[2] * speed;
	}
	if (key == 'e') { // Snow
		fall = SNOW;
		glutPostRedisplay();
	}
	if (key == '1') { // really '<' slow down
		if (slowdown > 4.0) slowdown += 0.01;
	}
	if (key == '2') { // really '>' speed up
		if (slowdown > 1.0) slowdown -= 0.01;
	}
	// model scaling - uniform
	if (key == 'c') {	// Larger
		scaling_x += 0.1f;
		scaling_y += 0.1f;
		scaling_z += 0.1f;
	}
	if (key == 'v') {	// Smaller
		scaling_x -= 0.1f;
		scaling_y -= 0.1f;
		scaling_z -= 0.1f;
	}
	if (key == 'x') { // start tail wag
		tail_rotation = 30.0f;
		leg_translation = 0.00001f;
	}
	// model rotation
	if (key == ',') {	// rotate left (x-axis)
		// calc forward vector for camera movement
		PlaySound(TEXT(AMBIENT), NULL, SND_ASYNC);
	}
	if (key == '.') {	// rotate right (x-axis)
		model_rotation = -20.0f;
	}
	glutPostRedisplay();
}

void mouse(int x, int y) {
	// calc forward vector for camera movement
	forward_x = target_x - camera_x;
	forward_y = target_y - camera_y;
	forward_z = target_z - camera_z;
	// save old mouse vals
	old_x = mouse_x;
	old_y = mouse_y;
	mouse_x = x;
	mouse_y = y;
	// range check
	if (mouse_y < 100) {
		mouse_y = 100;
		glutWarpPointer(mouse_x, height / 2);
	}
	if (mouse_y > height - 100) {
		mouse_y = height - 100;
		glutWarpPointer(mouse_x, height / 2);
	}
	// movements
	if ((mouse_x - old_x) > 0)	// moved right
		target_x += 0.15f;
	else if ((mouse_x - old_x) < 0)	// moved left
		target_x -= 0.15f;
	if ((mouse_y - old_y) > 0)	// moved up
		target_y += 0.15f;
	else if ((mouse_y - old_y) < 0)	// moved down
		target_y -= 0.15f;
}

int main(int argc, char** argv) {
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("WinterWonderland");
	// Tell glut where the display function is
	glutIdleFunc(UpdateScene);
	glutDisplayFunc(Display);
	
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(KeyUp);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(arrows);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	Init();
	// Begin infinite event loop
	
	glutMainLoop();
	return 0;
}