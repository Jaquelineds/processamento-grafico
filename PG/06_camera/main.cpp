/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Virtual Camera - create and modify VIEW and PROJECTION matrices              |
| keyboard controls: W,S,A,D,left and right arrows                             |
\******************************************************************************/
#include "gl_utils.h"
#include "maths_funcs.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
// GlM Inclues
#include <glm\glm.hpp>
#include <glm\matrix.hpp>
#include <glm\trigonometric.hpp>
#include <glm\vec3.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\mat4x4.hpp>
#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define PI 3.14

int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

enum Camera_Mode {
	Normal,
	Orbital
};

int main() {
	restart_gl_log();
	/*------------------------------start GL
	 * context------------------------------*/
	start_gl();

#pragma region Cube Init

	GLfloat points[] = {
		+0.5f, +0.5f, +0.5f,
		+0.5f, -0.5f, +0.5f,
		-0.5f, -0.5f, +0.5f,
		-0.5f, +0.5f, +0.5f,
		+0.5f, +0.5f, -0.5f,
		+0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, +0.5f,-0.5f,
	};

	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,
	};


	GLfloat points2[] = {
		+2.5f, +0.5f, +0.5f,
		+2.5f, -0.5f, +0.5f,
		+1.5f, -0.5f, +0.5f,
		+1.5f, +0.5f, +0.5f,
		+2.5f, +0.5f, -0.5f,
		+2.5f, -0.5f, -0.5f,
		+1.5f, -0.5f, -0.5f,
		+1.5f, +0.5f, -0.5f,
	};

	GLfloat colors2[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,
	};

	GLfloat points3[] = {
		-1.5f, +0.5f, +0.5f,
		-1.5f, -0.5f, +0.5f,
		-2.5f, -0.5f, +0.5f,
		-2.5f, +0.5f, +0.5f,
		-1.5f, +0.5f, -0.5f,
		-1.5f, -0.5f, -0.5f,
		-2.5f, -0.5f, -0.5f,
		-2.5f, +0.5f, -0.5f,
	};

	GLfloat colors3[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,
	};

	unsigned int indices[] = {
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Left Face
		3, 2, 6,
		6, 7, 3,

		// Right Face
		4, 5, 1,
		1, 0, 4,

		// Top Face
		4, 0, 3,
		3, 7, 4,

		// Down Face
		1, 5, 6,
		6, 2, 1,

		// Back Face
		7, 6, 5,
		5, 4, 7,

	};

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint colours_vbo;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	GLuint points2_vbo, colors2_vbo;

	glGenBuffers(1, &points2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);


	glGenBuffers(1, &colors2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);

	GLuint vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, points2_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors2_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint EBO2;
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint points3_vbo, colors3_vbo;

	glGenBuffers(1, &points3_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points3_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points3), points3, GL_STATIC_DRAW);

	glGenBuffers(1, &colors3_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors3_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors3), colors3, GL_STATIC_DRAW);
	

	GLuint vao3;
	glGenVertexArrays(1, &vao3);
	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, points3_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors3_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint EBO3;
	glGenBuffers(1, &EBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma endregion

#pragma region Shader Init
	/*------------------------------create
	 * shaders--------------------------------*/
	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
	parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);


	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			shader_programme);
		print_programme_info_log(shader_programme);
		return false;
	}
#pragma endregion

	// Anthom Camera
	/*
	// --------------------------create camera
	// matrices----------------------------
	// create PROJECTION MATRIX
	// input variables
	float near = 0.1f;										// clipping plane
	float far = 100.0f;										// clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD;						// convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height;	// aspect ratio
	// matrix components
	float range = tan( fov * 0.5f ) * near;
	float Sx = ( 2.0f * near ) / ( range * aspect + range * aspect );
	float Sy = near / range;
	float Sz = -( far + near ) / ( far - near );
	float Pz = -( 2.0f * far * near ) / ( far - near );
	GLfloat proj_mat[] = { Sx,   0.0f, 0.0f,  0.0f,
						   0.0f,   Sy, 0.0f,  0.0f,
						   0.0f, 0.0f,  Sz, -1.0f,
						   0.0f, 0.0f,  Pz,  0.0f };

	// create VIEW MATRIX
	float cam_speed = 1.0f;			 // 1 unit per second
	float cam_yaw_speed = 100.0f; // 10 degrees per second
	float cam_pos[] = { 0.0f, 0.0f, 2.0f }; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f;				// y-rotation in degrees
	mat4 T =
		translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1], -cam_pos[2] ) );
	mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );
	mat4 view_mat = R * T;*/

	Camera_Mode mode = Normal;

	float cam_speed = 1.0f;
	float orbit_speed = 25.0f;
	float cam_yaw_speed = 2.5f;
	float cam_pitch_speed = 1.0f;

	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * ONE_DEG_IN_RAD;
	float aspect = (float)g_gl_width / (float)g_gl_height;

	glm::vec3 camPos(0.0f, 0.0f, 2.0f);
	glm::vec4 forward(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 targetPos(0.0f, 0.0f, 0.0f);
	glm::mat4 rotationM = glm::mat4(1.0f);

	glm::vec3 upVec(0.0, 1.0f, 0.0f);
	glm::vec3 rightVect(1.0f, 0.0f, 0.0f);

	float pitch = 0.0f;
	float yaw = 0.0f;

	float hAngle = 0.0f;
	float vAngle = 90.0f;
	float distance = 2.0f;

	glm::mat4x4 viewM = glm::lookAt(camPos, targetPos, upVec);
	glm::mat4x4 projM = glm::perspective(fov, aspect, near, far);


	/* get location numbers of matrices in shader programme */
	GLint view_mat_location = glGetUniformLocation(shader_programme, "view");
	GLint proj_mat_location = glGetUniformLocation(shader_programme, "proj");
	/* use program (make current in state machine) and set default matrix values*/
	glUseProgram(shader_programme);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &viewM[0][0]);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projM[0][0]);

	/* some rendering defaults */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// Enabling depth test to not draw objects on top of each other
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	while (!glfwWindowShouldClose(g_window)) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter(g_window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shader_programme);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(vao2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(vao3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwPollEvents();

		upVec = rotationM * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

		glm::vec4 forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec3 forward_rotated = rotationM * forward;

		glm::vec4 right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 right_rotated = rotationM * right;

		// control keys
		bool cam_moved = false;

		if (glfwGetKey(g_window, GLFW_KEY_N)) {
			mode = Camera_Mode::Normal;
			camPos = glm::vec3(0.0f, 0.0f, 2.0f);
			forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
			rotationM = glm::mat4(1.0f);
			viewM = glm::lookAt(camPos, targetPos, upVec);
			pitch = yaw = 0.0f;
			upVec = glm::vec3(0.0, 1.0f, 0.0f);
			rightVect = glm::vec3(1.0f, 0.0f, 0.0f);
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &viewM[0][0]);
		}
		if (glfwGetKey(g_window, GLFW_KEY_O)) {
			mode = Camera_Mode::Orbital;
			camPos = glm::vec3(0.0f, 0.0f, 2.0f);
			forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
			rotationM = glm::mat4(1.0f);
			distance = 2.0f;
			viewM = glm::lookAt(camPos, targetPos, upVec);
			upVec = glm::vec3(0.0, 1.0f, 0.0f);
			rightVect = glm::vec3(1.0f, 0.0f, 0.0f);
			hAngle = 0.0f;
			vAngle = 90.0f;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &viewM[0][0]);
		}

		if (mode == Camera_Mode::Normal) {
			if (glfwGetKey(g_window, GLFW_KEY_A)) {
				camPos -= right_rotated * cam_speed * (float)elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_D)) {
				camPos += right_rotated * cam_speed * (float)elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_R)) {
				camPos.y += cam_speed * elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_F)) {
				camPos.y -= cam_speed * elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_W)) {
				camPos += forward_rotated * cam_speed * (float)elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_S)) {
				camPos -= forward_rotated * cam_speed * (float)elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
				yaw += cam_yaw_speed * elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
				yaw -= cam_yaw_speed * elapsed_seconds;
				cam_moved = true;
			}

			//if (glfwGetKey(g_window, GLFW_KEY_Q)) {
			//	pitch += cam_pitch_speed * elapsed_seconds;
			//	cam_moved = true;
			//}
			//if (glfwGetKey(g_window, GLFW_KEY_E)) {
			//	pitch -= cam_pitch_speed * elapsed_seconds;
			//	cam_moved = true;
			//}
		}
		else if (mode == Camera_Mode::Orbital) {

			if (glfwGetKey(g_window, GLFW_KEY_A)) {
				hAngle -= cam_speed * elapsed_seconds * orbit_speed;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_D)) {
				hAngle += cam_speed * elapsed_seconds * orbit_speed;
				cam_moved = true;
			}

			if (glfwGetKey(g_window, GLFW_KEY_W)) {
				vAngle -= cam_speed * elapsed_seconds * orbit_speed;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_S)) {
				vAngle += cam_speed * elapsed_seconds * orbit_speed;
				cam_moved = true;
			}

			if (glfwGetKey(g_window, GLFW_KEY_R)) {
				orbit_speed += 1;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_F)) {
				orbit_speed -= 1;
				cam_moved = true;
			}

			if (glfwGetKey(g_window, GLFW_KEY_UP)) {
				distance -= (float)elapsed_seconds;
				cam_moved = true;
			}
			if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
				distance += (float)elapsed_seconds;
				cam_moved = true;
			}

		}

		if (cam_moved) {

			if (mode == Camera_Mode::Normal) {
				glm::mat4 matPitch = glm::mat4(1.0f);
				glm::mat4 matYaw = glm::mat4(1.0f);

				matPitch = glm::rotate(matPitch, pitch, rightVect);
				matYaw = glm::rotate(matYaw, yaw, upVec);

				rotationM = matYaw * matPitch;

				glm::vec4 forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
				glm::vec3 forward_rotated = rotationM * forward;

				targetPos = camPos + glm::vec3(forward_rotated.x, forward_rotated.y, forward_rotated.z);
			}
			else if (mode == Camera_Mode::Orbital) {

				float theta = hAngle * PI / 180;
				float phi = vAngle * PI / 180;

				//float disX = targetPos.x - camPos.x;
				//float disY = targetPos.x - camPos.x;
				//float disZ = targetPos.x - camPos.x;

				//camPos.x = targetPos.x + (distance * cos(phi) * sin(theta));
				//camPos.y = targetPos.y + (distance * sin(phi) * sin(theta));
				//camPos.z = targetPos.z + (distance * cos(theta));

				camPos.x = targetPos.x + (distance * sin(phi) * sin(theta));
				camPos.y = targetPos.y + (distance * cos(phi));
				camPos.z = targetPos.z + (distance * cos(theta) * sin(phi));

			}

			//right_rotated = glm::cross(targetPos - camPos, upVec);
			//upVec = glm::cross(right_rotated, targetPos - camPos);

			viewM = glm::lookAt(camPos, targetPos, upVec);
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &viewM[0][0]);
		}

		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}

		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
