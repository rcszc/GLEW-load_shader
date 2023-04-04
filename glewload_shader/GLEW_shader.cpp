// GLEW_shader.
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

#include "GLEW_shader.h"

using namespace std;

//******************************** load shader ********************************
// opengl glew shader 3.3

char* GLEWSHADER::_load_shader_file(const char* _file) {
	char* _retFileData = {}, _ch = {};
	string _dataTemp = {};
	fstream _load_ShaderData(_file);

	if (_load_ShaderData) {
		while (_load_ShaderData.get(_ch)) { // load source
			_dataTemp.push_back(_ch);
		}

		size_t _dataLen = _dataTemp.size();
		_retFileData = new char[_dataLen];
		for (int i = 0; i < _dataLen; i++)
			_retFileData[i] = _dataTemp[i];
		// Free string
		_dataTemp.clear();
		_dataTemp.shrink_to_fit();
		
		cout << "File load " << _file << endl;
	}
	else
		cout << "File error " << _file << endl;
	return _retFileData;
}

void GLEWSHADER::_shader_compile(GLuint _shaderhandle, const char* _shader) {
	int _compile_success = NULL;
	int _shaderloglen = NULL;
	char* _shader_infoLog = {};

	if (_shader != "PROGRAM") { // shader
		glGetShaderiv(_shaderhandle, GL_COMPILE_STATUS, &_compile_success);
		glGetShaderiv(_shaderhandle, GL_INFO_LOG_LENGTH, &_shaderloglen); // log length
		if (!_compile_success) {

			_shader_infoLog = new char[_shaderloglen];
			glGetShaderInfoLog(_shaderhandle, _shaderloglen, NULL, _shader_infoLog);

			// print error
			string error_info = GLEW_SHADER_ERROR;
			error_info += _shader;
			cout << error_info.c_str() << " " << _shader_infoLog << endl;

			delete[] _shader_infoLog;
		}
		else
			cout << _shader << " Compilation succeeded." << endl;
	}
	else { // shader program
		glGetProgramiv(_shaderhandle, GL_LINK_STATUS, &_compile_success);
		glGetProgramiv(_shaderhandle, GL_INFO_LOG_LENGTH, &_shaderloglen); // log length
		if (!_compile_success) {

			_shader_infoLog = new char[_shaderloglen];
			glGetProgramInfoLog(_shaderhandle, _shaderloglen, NULL, _shader_infoLog);

			// print error
			string error_info = GLEW_SHADER_ERROR;
			error_info += _shader;
			cout << error_info.c_str() << " " << _shader_infoLog << endl;

			delete[] _shader_infoLog;
		}
		else
			cout << _shader << " Compilation succeeded." << endl;
	}
}

GLenum SHADER_GLEWINIT() { return glewInit(); }
GLenum PROGRAM_CREATE() { return glCreateProgram(); };

void GLEWSHADER::load_shader_vs(const char* File_vs) {
	_shader_v = glCreateShader(GL_VERTEX_SHADER);

	source_vertexShader = _load_shader_file(File_vs);
	glShaderSource(_shader_v, GLEWSOURCE_COUNT, &source_vertexShader, NULL);
	glCompileShader(_shader_v);
	_shader_compile(_shader_v, "vertex"); // vertex shader info
	glAttachShader(SHADER_PROGRAM, _shader_v);
}

void GLEWSHADER::load_shader_fs(const char* File_fs) {
	_shader_f = glCreateShader(GL_FRAGMENT_SHADER);

	source_fragmentShader = _load_shader_file(File_fs);
	glShaderSource(_shader_f, GLEWSOURCE_COUNT, &source_fragmentShader, NULL);
	glCompileShader(_shader_f);
	_shader_compile(_shader_f, "fragment"); // fragment shader info
	glAttachShader(SHADER_PROGRAM, _shader_f);
}

GLuint GLEWSHADER::Link_shader_program() {
	glLinkProgram(SHADER_PROGRAM);
	_shader_compile(SHADER_PROGRAM, "PROGRAM");
	return SHADER_PROGRAM;
}

GLuint* GLEWSHADER::GET_SHADER_HANDLE() {
	static GLuint retshader[2] = {};
	retshader[0] = _shader_v;
	retshader[1] = _shader_f;
	return retshader;
}

void GLEWSHADER::delete_program(GLuint _shaderhandle) {
	glDeleteProgram(_shaderhandle);
}
void GLEWSHADER::delete_shader() {
	glDeleteShader(_shader_v);
	glDeleteShader(_shader_f);
}

//******************************** load data[shader] ********************************
// opengl glew vao vbo

GLint GLEW_LOADDATA_GETVMAX() {
	GLint nrAttributes = NULL;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	return nrAttributes;
}

void GLEW_LOADDAT_VBO(GLuint vbo_handle, GLuint dataByte_size, GLfloat* vertex_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, dataByte_size, vertex_data, GL_STATIC_DRAW);
}
void GLEW_LOADDAT_VAO(GLuint s_Location, GLuint vertex_size, GLvoid* bias) {
	glVertexAttribPointer(s_Location, vertex_size, GL_FLOAT, GL_FALSE, vertex_size * GLEWLOAD_DATA_FLOATSIZE, bias);
	glEnableVertexAttribArray(s_Location);
}

void GLEW_USESHADER(GLuint program_handle) {
	glUseProgram(program_handle);
}

void GLEW_LOADDATA_uniform3f(GLuint program_handle, const char* uniform_name, GLfloat data[3]) {
	int uniformLocation = glGetUniformLocation(program_handle, uniform_name);
	glUniform3f(uniformLocation, data[0], data[1], data[2]);
}
void GLEW_LOADDATA_uniform4f(GLuint program_handle, const char* uniform_name, GLfloat data[4]) {
	int uniformLocation = glGetUniformLocation(program_handle, uniform_name);
	glUniform4f(uniformLocation, data[0], data[1], data[2], data[3]);
}
void GLEW_LOADDATA_uniformTEXTURE(GLuint program_handle, const char* uniform_name, GLint TEXTURE) {
	int uniformLocation = glGetUniformLocation(program_handle, uniform_name);
	glUniform1i(uniformLocation, TEXTURE);
}

void GLEW_LOADDATA_unbind() {
	glBindVertexArray(NULL);             //unbind vao
	glBindBuffer(GL_ARRAY_BUFFER, NULL); //unbind vbo
}

void GLEW_LOADDATA_LINELOOP(GLuint program_handle, GLuint point_num) {
	glDrawArrays(GL_LINE_LOOP, GLEWLOAD_DATA_DATBEGIN, point_num);
}
void GLEW_LOADDATA_TRIANGLES(GLuint program_handle) {
	glDrawArrays(GL_TRIANGLES, GLEWLOAD_DATA_DATBEGIN, 3);
}
void GLEW_LOADDATA_QUADS(GLuint program_handle) {
	glDrawArrays(GL_QUADS, GLEWLOAD_DATA_DATBEGIN, 4);
}

// glEnableVertexAttribArray(number)
// vertex shader: layout (location = number) in vec3 xxx
// glVertexAttribPointer(index, size, type, unified, step, bias)

//******************************** frame.buffer.object ********************************

bool GLEW_FBO_CREATE(GLuint fbo_handle, GLuint texture_handle) {
	glGenFramebuffers(1, &fbo_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

	// 将纹理添加为 FBO 的附件 连接在颜色附着点
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_handle, NULL);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) { // test FBO
		cout << "Frame buffer incomplete!" << endl;
		return TRUE;
	}
	else {
		cout << "Frame buffer complete!" << endl;
		return FALSE;
	}
}

//******************************** GPU info ********************************

GLint GLEWGPU_INFO::GLFW_GETGPU_MEMcapacity() {
	GLint MemoryKb = NULL;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &MemoryKb);
	return MemoryKb;
}
GLint GLEWGPU_INFO::GLFW_GETGPU_MEMsize() {
	GLint Curmemory = NULL;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &Curmemory);
	return Curmemory;
}

void GLEWGPU_INFO::GLEW_SYSTEMGPU_INFO() {
	cout << "OpenGL_GLEW" << endl;
	cout << "gpu_supplier: " << glGetString(GL_VENDOR) << endl;
	cout << "gpu_model: " << glGetString(GL_RENDERER) << endl;
	cout << "opengl_version: " << glGetString(GL_VERSION) << endl;
	cout << "glsl_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}