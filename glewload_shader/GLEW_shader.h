// GLEW_shader.
// log: crimson_logframe
// Libray: GL GLEW.
// rcsz. version 1.0.2 ahpla

#ifndef _glew_LoadShader_H
#define _glew_LoadShader_H
#include <GL/gl.h>

#define GLEW_INFOHEAD      "[shader]: "
#define GLEW_SHADER_ERROR  "ERROR::SHADER: "
#define GLEWSOURCE_COUNT 1
#define GLEW_SHADER_LOG  512

//******************************** load shader ********************************
// GLEW #version 330 core
#define GLEW_LOADSHADER_PATHNULL "/null"
#define GLEW_LOADSHADER_VERTEX   0xF001 // vertex shader
#define GLEW_LOADSHADER_FRAGMENT 0xF002 // fragment shader

GLenum SHADER_GLEWINIT();
GLenum PROGRAM_CREATE();

class GLEWSHADER {
protected:
	// shader str data 
	const char* source_vertexShader = {};
	const char* source_fragmentShader = {};
	GLuint _shader_v = NULL, _shader_f = NULL;
	GLuint SHADER_PROGRAM = NULL;

	char* _load_shader_file(const char* _file); // load file
	void _shader_compile(GLuint _shaderhandle, const char* _shader); // print errorlog
public:
	GLEWSHADER() {
		_shader_v = SHADER_GLEWINIT();
		_shader_f = SHADER_GLEWINIT();
		SHADER_PROGRAM = PROGRAM_CREATE();
	}
	~GLEWSHADER() { //free
		delete[] source_vertexShader;
		delete[] source_fragmentShader;
	}

	// Load_shader File_vs.顶点着色器 
	void load_shader_vs(const char* File_vs);
	// Load_shader File_fs.片段着色器
	void load_shader_fs(const char* File_fs);
	// 链接shader return program_handle
	GLuint Link_shader_program();

	// shader handle 0:vertex 1:fragment
	GLuint* GET_SHADER_HANDLE();
	// delete program shader
	void delete_program(GLuint _shaderhandle);
	// delete vertex fragment shader
	void delete_shader();
};

//******************************** load data ********************************

#define GLEWLOAD_DATA_BIASZERO  (void*)0
#define GLEWLOAD_DATA_FLOATSIZE sizeof(float)
#define GLEWLOAD_DATA_DATBEGIN  (GLuint)0
#define GLEWLOAD_DATA_VER2D     (GLuint)2
#define GLEWLOAD_DATA_VER3D     (GLuint)3

// 获取顶点属性上限 "GL_MAX_VERTEX_ATTRIBS"
GLint GLEW_LOADDATA_GETVMAX();

// VBO (Vertex Buffer Objects) 
void GLEW_LOADDAT_VBO(GLuint vbo_handle, GLuint dataByte_size, GLfloat* vertex_data);
// VAO (Vertex Array Object) 
// s_Location: shader:" layout (location = s_Location) "
// vertex_size: xy[2] xyz[3] ...
void GLEW_LOADDAT_VAO(GLuint s_Location, GLuint vertex_size, GLvoid* bias);

// 使用着色器程序 (改变渲染管线)
void GLEW_USESHADER(GLuint program_handle);

// shader全局变量 data[3] shader:" uniform vec3 uniform_name "
void GLEW_LOADDATA_uniform3f(GLuint program_handle, const char* uniform_name, GLfloat data[3]);
// shader全局变量 data[4] shader:" uniform vec4 uniform_name "
void GLEW_LOADDATA_uniform4f(GLuint program_handle, const char* uniform_name, GLfloat data[4]);
// shader全局变量 Texture 1i
void GLEW_LOADDATA_uniformTEXTURE(GLuint program_handle, const char* uniform_name, GLint TEXTURE);

// VBO -> VAO -> DRAW
void GLEW_LOADDATA_LINELOOP(GLuint program_handle, GLuint point_num);
void GLEW_LOADDATA_TRIANGLES(GLuint program_handle);
void GLEW_LOADDATA_QUADS(GLuint program_handle);

void GLEW_LOADDATA_unbind();

//******************************** frame.buffer.object ********************************

// 创建FBO fbo.handle texture.handle
bool GLEW_FBO_CREATE(GLuint fbo_handle, GLuint texture_handle);

//******************************** GPU info ********************************

// 系统GPU信息. 
// CreateWindow 上下文调用.
class GLEWGPU_INFO {
public:
	// 获取显存容量 [max]
	GLint GLFW_GETGPU_MEMcapacity();
	// 获取显存余量
	GLint GLFW_GETGPU_MEMsize();
	// 获取GPU信息
	void GLEW_SYSTEMGPU_INFO();
};

#endif