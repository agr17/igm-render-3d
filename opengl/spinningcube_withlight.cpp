// Copyright (C) 2020 Emilio J. Padrón
// Released as Free Software under the X11 License
// https://spdx.org/licenses/X11.html

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// GLM library to deal with matrix operations
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_inverse.hpp> // glm::inverseTranspose
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "util/textfile_ALT.h"

#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

int gl_width = 640;
int gl_height = 480;

void glfw_window_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void render(double);

GLuint setupVertexArrayObject(const GLfloat vertex_positions[], size_t num_elements);

GLuint shader_program = 0; // shader program to set render pipeline
GLuint vao, tetrahedron_vao = 0; // Vertext Array Object to set input data
GLint model_location, view_location, proj_location, normal_location, view_pos_location, 
  mat_diff_location, mat_spec_location, mat_shine_location, 
  light_amb_location, light_diff_location, light_spec_location, light_pos_location,
  light_2_amb_location, light_2_diff_location, light_2_spec_location, light_2_pos_location; // Uniforms for transformation matrices

GLuint texture = 0; // Texture object

// Shader names
const char *vertexFileName = "shaders/spinningcube_withlight_vs.glsl";
const char *fragmentFileName = "shaders/spinningcube_withlight_fs.glsl";

// Camera
glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);

// Lighting
glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
glm::vec3 light_ambient(0.2f, 0.2f, 0.2f);
glm::vec3 light_diffuse(0.5f, 0.5f, 0.5f);
glm::vec3 light_specular(1.0f, 1.0f, 1.0f);

glm::vec3 light_2_pos(-1.2f, 1.0f, 2.0f);

// Material
glm::vec3 material_specular(0.5f, 0.5f, 0.5f);
const GLfloat material_shininess = 32.0f;

int main() {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(gl_width, gl_height, "My spinning cube", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwSetWindowSizeCallback(window, glfw_window_size_callback);
  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  // glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* vendor = glGetString(GL_VENDOR); // get vendor string
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* glversion = glGetString(GL_VERSION); // version as a string
  const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string
  printf("Vendor: %s\n", vendor);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", glversion);
  printf("GLSL version supported %s\n", glslversion);
  printf("Starting viewport: (width: %d, height: %d)\n", gl_width, gl_height);

  // Enable Depth test: only draw onto a pixel if fragment closer to viewer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // set a smaller value as "closer"

  // Vertex Shader
  char* vertex_shader = textFileRead(vertexFileName);

  // Fragment Shader
  char* fragment_shader = textFileRead(fragmentFileName);

  // Shaders compilation
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  free(vertex_shader);
  glCompileShader(vs);

  int  success;
  char infoLog[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    printf("ERROR: Vertex Shader compilation failed!\n%s\n", infoLog);

    return(1);
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  free(fragment_shader);
  glCompileShader(fs);

  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fs, 512, NULL, infoLog);
    printf("ERROR: Fragment Shader compilation failed!\n%s\n", infoLog);

    return(1);
  }

  // Create program, attach shaders to it and link it
  shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  glValidateProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    printf("ERROR: Shader Program linking failed!\n%s\n", infoLog);

    return(1);
  }

  // Release shader objects
  glDeleteShader(vs);
  glDeleteShader(fs);

  // Cube to be rendered
  //
  //          0        3
  //       7        4 <-- top-right-near
  // bottom
  // left
  // far ---> 1        2
  //       6        5
  //
  const GLfloat vertex_positions[] = {
    -0.25f, -0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 1
    -0.25f,  0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 0
     0.25f, -0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 2
     0.25f,  0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 3
     0.25f, -0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 2
    -0.25f,  0.25f, -0.25f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 0

     0.25f, -0.25f, -0.25f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 2
     0.25f,  0.25f, -0.25f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 3
     0.25f, -0.25f,  0.25f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 5
     0.25f,  0.25f,  0.25f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 4
     0.25f, -0.25f,  0.25f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 5
     0.25f,  0.25f, -0.25f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 3

     0.25f, -0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 5
     0.25f,  0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 4
    -0.25f, -0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 6
    -0.25f,  0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 7
    -0.25f, -0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 6
     0.25f,  0.25f,  0.25f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 4

    -0.25f, -0.25f,  0.25f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 6
    -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 7
    -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 1
    -0.25f,  0.25f, -0.25f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 0
    -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 1
    -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 7

     0.25f, -0.25f, -0.25f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 2
     0.25f, -0.25f,  0.25f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 5
    -0.25f, -0.25f, -0.25f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 1
    -0.25f, -0.25f,  0.25f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 6
    -0.25f, -0.25f, -0.25f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 1
     0.25f, -0.25f,  0.25f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 5

     0.25f,  0.25f,  0.25f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 4
     0.25f,  0.25f, -0.25f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 3
    -0.25f,  0.25f,  0.25f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 7
    -0.25f,  0.25f, -0.25f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 0
    -0.25f,  0.25f,  0.25f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 7
     0.25f,  0.25f, -0.25f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f  // 3
  };

  // Tetrahedron to be rendered (based on cube_vertex_positions)
  //
  //                  
  //            0 <-- top
  // bottom
  // left
  // far --->   1       
  //       2        3
  //
  const GLfloat tetrahedron_vertex_positions[] = {
    // ec. plano: 16x - 4y + 8z + 1 = 0
    // vec. normal: (16, -4, 8) -> invertir direccion
    // vec. normalizado: (0.873f, -0.218f, 0.436f) -> invertir direccion
    0.00f,   0.25f,  0.00f, -0.873f, 0.218f, -0.436f, 0.5f, 1.0f, // 0
    0.00f,  -0.25f, -0.25f, -0.873f, 0.218f, -0.436f, 0.0f, 0.0f, // 1
    -0.25f, -0.25f,  0.25f, -0.873f, 0.218f, -0.436f, 1.0f, 0.0f, // 2

    // ec. plano: 16x + 4y - 8z - 1 = 0
    // vec. normal: (16, 4, -8)
     0.00f,  0.25f,  0.00f, 0.873f, 0.218f, -0.436f, 0.5f, 1.0f, // 0
     0.00f, -0.25f, -0.25f, 0.873f, 0.218f, -0.436f, 1.0f, 0.0f, // 1
     0.25f, -0.25f,  0.25f, 0.873f, 0.218f, -0.436f, 0.0f, 0.0f, // 3

    // ec. plano: 4y + 8z - 1 = 0
    // vec. normal: (0, 4, 8)
     0.00f,  0.25f, 0.00f, 0.000f, 0.447f, 0.894f, 0.5f, 1.0f, // 0
    -0.25f, -0.25f, 0.25f, 0.000f, 0.447f, 0.894f, 0.0f, 0.0f, // 2
     0.25f, -0.25f, 0.25f, 0.000f, 0.447f, 0.894f, 1.0f, 0.0f, // 3

     0.00f, -0.25f, -0.25f, 0.0f, -1.0f,  0.0f, 0.5f, 1.0f, // 1
    -0.25f, -0.25f,  0.25f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 2
     0.25f, -0.25f,  0.25f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f  // 3
  };

  // Vertex Array Object
  size_t num_elements = sizeof(vertex_positions) / sizeof(GLfloat);
  vao = setupVertexArrayObject(vertex_positions, num_elements);

  num_elements = sizeof(tetrahedron_vertex_positions) / sizeof(GLfloat);
  tetrahedron_vao = setupVertexArrayObject(tetrahedron_vertex_positions, num_elements);

  // Uniforms
  model_location = glGetUniformLocation(shader_program, "model");
  view_location = glGetUniformLocation(shader_program, "view");
  proj_location = glGetUniformLocation(shader_program, "projection");

  normal_location = glGetUniformLocation(shader_program, "normal_to_world");
  view_pos_location = glGetUniformLocation(shader_program, "view_pos");

  light_amb_location = glGetUniformLocation(shader_program, "light_1.ambient");
  light_diff_location = glGetUniformLocation(shader_program, "light_1.diffuse");
  light_spec_location = glGetUniformLocation(shader_program, "light_1.specular");
  light_pos_location = glGetUniformLocation(shader_program, "light_1.position");

  light_2_amb_location = glGetUniformLocation(shader_program, "light_2.ambient");
  light_2_diff_location = glGetUniformLocation(shader_program, "light_2.diffuse");
  light_2_spec_location = glGetUniformLocation(shader_program, "light_2.specular");
  light_2_pos_location = glGetUniformLocation(shader_program, "light_2.position");

  mat_diff_location = glGetUniformLocation(shader_program, "material.diffuse");
  mat_spec_location = glGetUniformLocation(shader_program, "material.specular");
  mat_shine_location = glGetUniformLocation(shader_program, "material.shininess");

  // Create texture object
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image for texture
  int width, height, nrChannels;
  // Before loading the image, we flip it vertically because
  // Images: 0.0 top of y-axis  OpenGL: 0.0 bottom of y-axis
  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load("maps/container2.png", &width, &height, &nrChannels, 0);
  // maps/container2.png
  // Image from http://www.flickr.com/photos/seier/4364156221
  // CC-BY-SA 2.0
  if (data) {
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    // Generate texture from image
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture\n");
  }

  // Free image once texture is generated
  stbi_image_free(data);

// Render loop
  while(!glfwWindowShouldClose(window)) {

    processInput(window);

    render(glfwGetTime());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void render(double currentTime) {
  float f = (float)currentTime * 0.3f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, gl_width, gl_height);

  glUseProgram(shader_program);
  glBindVertexArray(vao);

  glm::mat4 model_matrix, view_matrix, proj_matrix, model_tetrahedron;
  glm::mat3 normal_matrix;

  // Uniforms
  glUniform3f(view_pos_location, camera_pos.x, camera_pos.y, camera_pos.z);

  glUniform1i(mat_diff_location, 0);
  glUniform3f(mat_spec_location, material_specular.x, material_specular.y, material_specular.z);
  glUniform1f(mat_shine_location, material_shininess);

  glUniform3f(light_amb_location, light_ambient.x, light_ambient.y, light_ambient.z);
  glUniform3f(light_diff_location, light_diffuse.x, light_diffuse.y, light_diffuse.z);
  glUniform3f(light_spec_location, light_specular.x, light_specular.y, light_specular.z);
  glUniform3f(light_pos_location, light_pos.x, light_pos.y, light_pos.z);

  glUniform3f(light_2_amb_location, light_ambient.x, light_ambient.y, light_ambient.z);
  glUniform3f(light_2_diff_location, light_diffuse.x, light_diffuse.y, light_diffuse.z);
  glUniform3f(light_2_spec_location, light_specular.x, light_specular.y, light_specular.z);
  glUniform3f(light_2_pos_location, light_2_pos.x, light_2_pos.y, light_2_pos.z);

  // Cube
  model_matrix = glm::mat4(1.f);

  // Moving cube
  model_matrix = glm::rotate(model_matrix,
                          glm::radians(f * 45.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));
  model_matrix = glm::rotate(model_matrix,
                          glm::radians(f * 81.0f),
                          glm::vec3(1.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

  view_matrix = glm::lookAt(                 camera_pos,  // pos
                            glm::vec3(0.0f, 0.0f, 0.0f),  // target
                            glm::vec3(0.0f, 1.0f, 0.0f)); // up

  glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

  // Projection
  proj_matrix = glm::perspective(glm::radians(50.0f),
                                 (float)gl_width / (float)gl_height,
                                 0.1f, 1000.0f);
  glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));
  
  // Normal matrix: normal vectors to world coordinates
  normal_matrix = glm::inverseTranspose(glm::mat3(model_matrix));
  glUniformMatrix3fv(normal_location, 1, GL_FALSE, glm::value_ptr(normal_matrix));

  // Texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Bind tetrahedron vao
  glBindVertexArray(tetrahedron_vao);

  // Model tetrahedron
  model_tetrahedron = glm::mat4(1.f);

  model_tetrahedron = glm::translate(model_tetrahedron,
                                     glm::vec3(-1.0f, 0.0f, 0.0f));

  // Moving tetrahedron
  model_tetrahedron = glm::rotate(model_tetrahedron,
                                  glm::radians(f * 45.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

  model_tetrahedron = glm::rotate(model_tetrahedron,
                                  glm::radians(f * 81.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_tetrahedron));  

  // Normal matrix: normal vectors to world coordinates
  normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_tetrahedron)));
  glUniformMatrix3fv(normal_location, 1, GL_FALSE, glm::value_ptr(normal_matrix)); 

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

// Callback function to track window size and update viewport
void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
  gl_width = width;
  gl_height = height;
  printf("New viewport: (width: %d, height: %d)\n", width, height);
}

GLuint setupVertexArrayObject(const GLfloat vertex_positions[], size_t num_elements) {
  GLuint vao, vbo = 0;

  // Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Vertex Buffer Object (for vertex coordinates)
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, num_elements * sizeof(GLfloat), vertex_positions, GL_STATIC_DRAW);

  // Vertex attributes
  // 0: vertex position (x, y, z)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // 1: vertex normals (x, y, z)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 2: vertex texture coordinates (u, v)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind vbo (it was conveniently registered by VertexAttribPointer)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind vao
  glBindVertexArray(0);

  return vao;
}