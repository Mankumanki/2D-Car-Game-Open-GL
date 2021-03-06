#include<glew.h>
#include<glfw3.h>
#include<iostream>
#include<glm.hpp>
#include<mat4x4.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<gtc/matrix_transform.hpp>
#include<time.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
using namespace std;
using namespace glm;
int compileshader(int type, string& source)
{
	unsigned int id = glCreateShader(type);
	if (id == 0)
	{
		cout << "Fail to generate shader";
	}
	char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_COMPILE_STATUS, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(id, length, &length, message);
		cout << "\nFaied to compile shader";
		cout << "\n" << message;
		delete[] message;
	}
	return id;
}
int createshader(string& vertex, string& frag)
{
	unsigned int program = glCreateProgram();
	if (program == 0)
	{
		cout << "Fail to generate program";
	}
	unsigned int vs = compileshader(GL_VERTEX_SHADER, vertex);
	unsigned int fs = compileshader(GL_FRAGMENT_SHADER, frag);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_LINK_STATUS, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(program, length, &length, message);
		cout << "\nFaied to compile shader";
		cout << "\n" << message;
		delete[] message;
	}
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
int main()
{
	if (!glfwInit())
	{
		cout << "fail to initialize glfw";
		return-1;
	}

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "2DCarGAME", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return-1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		cout << "Error";
	}

	float road[] = {
					  0.7f,0.0f,5.5f,
					 -0.7f,0.0f,5.5f,
					  0.7f,0.0f,-7.5f,
					 -0.7f,0.0f,-7.5f,
	};


	float carup[] = {
						 0.0f,0.0f,0.45f,
						-0.07f,0.0f,0.55f,
						 0.07f,0.0f,0.55f
	};

	float carbod[] = {
						   -0.07f,0.0f,0.55f,
							0.07f,0.0f,0.55f,
							-0.07f,0.0f,0.7f,
							0.07f,0.0f,0.7f,
	};

	float roadline[] = {
							 -0.04f,0.0f,0.5f,
							  0.04f,0.0f,0.5f,
							 -0.04f,0.0f,0.7f,
							  0.04f,0.0f,0.7f,
	};

	vec3 translation[] = {
							   vec3(0.0f,0.0f,0.0f),
							   vec3(0.0f,0.0f,-0.8f),
							   vec3(0.0f,0.0f,-1.4f),
							   vec3(0.0f,0.0f,-2.2f),
							   vec3(0.0f,0.0f,-3.0f),
							   vec3(0.0f,0.0f,-4.0f),
							   vec3(0.0f,0.0f,-5.0f),
							   vec3(0.0,0.0,-6.0f)
	};


	float tyre1[] = {
		                 0.07f,0.0f,0.69f,
						 0.085f,0.0f,0.69f,
						 0.07f,0.0f,0.71f,
						 0.085f,0.0f,0.71f
	};

	float tyre2[] = {
						 0.07f,0.0f,0.54f,
						 0.085f,0.0f,0.54f,
						 0.07f,0.0f,0.56f,
						 0.085f,0.0f,0.56f
	};

	float tyre3[] = {
						 -0.07f,0.0f,0.54f,
						 -0.085f,0.0f,0.54f,
						 -0.07f,0.0f,0.56f,
						 -0.085f,0.0f,0.56f
	};

	float tyre4[] = {
						-0.07f,0.0f,0.69f,
						 -0.085f,0.0f,0.69f,
						 -0.07f,0.0f,0.71f,
						 -0.085f,0.0f,0.71f
	};

	float roadstripe[] = {
					  0.64f,0.0f,8.0f,
					  0.66f,0.0f,8.0f,
					  0.64f,0.0f,-4.0f,
					  0.66f,0.0f,-4.0f
	};

	float roadstripe1[] = {
				  -0.64f,0.0f,8.0f,
				  -0.66f,0.0f,8.0f,
				  -0.64f,0.0f,-4.0f,
				  -0.66f,0.0f,-4.0f
	};


	float object[] = {
						    0.03f,0.0f,-2.6f,
						    0.15f,0.0f,-2.6f,
							0.03f,0.0f,-2.8f,
							0.15f,0.0f,-2.8f
	};
	vec4 carclone = vec4(0.0f, 0.0f, 0.45f, 1.0f);

	vec4 objectclone = vec4(0.03f, 0.0f, -2.6f, 1.0f);

	vec4 objectclone1 = vec4(0.15f, 0.0f, -2.6f, 1.0f);


	unsigned int indices[] = {
				   0,1,2,
				   2,3,1
	};
	unsigned int buff[11], ind, vert[11], tex;
	/*int w, h, nr;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data=stbi_load("C:\\Users\\mehta\\Desktop\\road3.jpg", &w, &h, &nr,0);

	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,GL_UNSIGNED_BYTE,0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Fail to generate Texture";
	}
	stbi_image_free(data);*/

	glGenVertexArrays(11, vert);
	glGenBuffers(11, buff);
	glGenBuffers(1, &ind);

	glBindVertexArray(vert[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(road), road, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) *5 , (void*)(sizeof(float)*3));

	glBindVertexArray(vert[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(carup), carup, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(carbod), carbod, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[3]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roadline), roadline, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[4]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tyre1), tyre1, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[5]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[5]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tyre2), tyre2, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[6]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[6]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tyre3), tyre3, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[7]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[7]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tyre4), tyre4, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[8]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[8]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roadstripe), roadstripe, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[9]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[9]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roadstripe1), roadstripe1, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(vert[10]);
	glBindBuffer(GL_ARRAY_BUFFER, buff[10]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object), object, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	string vertexshader[] = {
		{"#version 330 core\n"
		"\n"
		"layout(location=0)in vec4 pos;\n"
		//"layout(location=1)in vec2 tex2;\n"
		//"out vec2 texcord;\n"
		"uniform mat4 vie;\n"
		"uniform mat4 project;\n"
		"uniform mat4 rotx;\n"
		"uniform mat4 roa;\n"
		"void main()\n"
		"{\n"
		"gl_Position=project*vie*rotx*roa*pos;\n"
		//"texcord=tex2;\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"layout(location=0)in vec4 pos1;\n"
		"uniform mat4 vie1;\n"
		"uniform mat4 project1;\n"
		"uniform mat4 rotx1;\n"
		"uniform mat4 tran;\n"
		"void main()\n"
		"{\n"
		"gl_Position=project1*vie1*rotx1*tran*pos1;\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"layout(location=0)in vec4 pos2;\n"
		"uniform mat4 vie2;\n"
		"uniform mat4 project2;\n"
		"uniform mat4 rotx2;\n"
		"uniform mat4 roa2;\n"
		"uniform mat4 tr;\n"
		"void main()\n"
		"{\n"
		"gl_Position=project2*vie2*rotx2*roa2*tr*pos2;\n"
		"}\n"},
		
	{"#version 330 core\n"
		"\n"
		"layout(location=0)in vec4 pos3;\n"
		"uniform mat4 vie3;\n"
		"uniform mat4 project3;\n"
		"uniform mat4 rotx3;\n"
		"uniform mat4 tran3;\n"
		"void main()\n"
		"{\n"
		"gl_Position=project3*vie3*rotx3*tran3*pos3;\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"layout(location=0)in vec4 pos4;\n"
		"uniform mat4 vie4;\n"
		"uniform mat4 project4;\n"
		"uniform mat4 rotx4;\n"
		"uniform mat4 roa4;\n"
		"uniform mat4 tr4;\n"
		"void main()\n"
		"{\n"
		"gl_Position=project4*vie4*rotx4*roa4*tr4*pos4;\n"                                                                      
		"}\n"},

	};

	string fragmentshader[] = {
		{"#version 330 core\n"
		"\n"
		"out vec4 color;\n"
		//"in vec2 texcord;\n"
		//"uniform sampler2D tex1;\n"
		"void main()\n"
		"{\n"
		"color=vec4(0.3f,0.3f,0.3f,1.0f);\n"
		"}\n"},

	{"#version 330 core\n"
		"\n"
		"out vec4 color1;\n"
		"void main()\n"
		"{\n"
		"color1=vec4(1.0f,0.0,0.0f,1.0f);\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"out vec4 color2;\n"
		"void main()\n"
		"{\n"
		"color2=vec4(1.0f,1.0,1.0f,1.0f);\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"out vec4 color3;\n"
		"void main()\n"
		"{\n"
		"color3=vec4(0.0f,0.0,0.0f,1.0f);\n"
		"}\n"},

		{"#version 330 core\n"
		"\n"
		"out vec4 color3;\n"
		"void main()\n"
		"{\n"
		"color3=vec4(0.0f,0.0,1.0f,1.0f);\n"
		"}\n"}

	};
	unsigned int shader[5];
	for (int i = 0;i < 5;i++)
	{
		shader[i] = createshader(vertexshader[i], fragmentshader[i]);
	}

	mat4 view = mat4(1.0f);
	mat4 view1 = mat4(1.0f);
	mat4 projection = mat4(1.0f);
	mat4 projection1 = mat4(1.0f);
	mat4 rotate, rotate1;
	float w1 = 60.0f;
	vec4 v1 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 v2 = vec4(0.0f, cos(radians(w1)), -sin(radians(w1)), 0.0f);
	vec4 v3 = vec4(0.0f, sin(radians(w1)), cos(radians(w1)), 0.0f);
	vec4 v4 = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec4 v5 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 v6 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 v7 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	vec4 v8 = vec4(0.0f, 0.0f, -2.0f, 1.0f);

	float t = 0.0f, t1 = 0.0f,t3=0.0f,t8=0.0f;

	bool gameover = false;
	int score = 0;
	while (!glfwWindowShouldClose(window))
	{
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shader[0]);
			rotate = transpose(mat4(v1, v2, v3, v4));
			view = mat4(v5, v6, v7, v8);
			projection = perspective(radians(45.0f), float(800) / float(600), 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader[0], "rotx"), 1, GL_FALSE, &rotate[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[0], "vie"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[0], "project"), 1, GL_FALSE, &projection[0][0]);
			vec4 v13 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			vec4 v14 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			vec4 v15 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
			vec4 v16 = vec4(0.0f, 0.0f, t3, 1.0f);
			mat4 roadmove = mat4(v13, v14, v15, v16);
			glUniformMatrix4fv(glGetUniformLocation(shader[0], "roa"), 1, GL_FALSE,&roadmove[0][0]);
			
			glBindVertexArray(vert[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glUseProgram(shader[2]);

			glUniformMatrix4fv(glGetUniformLocation(shader[2], "rotx2"), 1, GL_FALSE, &rotate[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[2], "vie2"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[2], "project2"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[2], "roa2"), 1, GL_FALSE, &roadmove[0][0]);
			for (int i = 0;i < 8;i++)
			{

				mat4 loc = mat4(1.0f);
				loc = translate(loc, translation[i]);
				glUniformMatrix4fv(glGetUniformLocation(shader[2], "tr"), 1, GL_FALSE, &loc[0][0]);
				glBindVertexArray(vert[3]);
				glBindBuffer(GL_ARRAY_BUFFER, buff[3]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			glBindVertexArray(vert[8]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[8]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vert[9]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[9]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glUseProgram(shader[1]);
			rotate1 = transpose(mat4(v1, v2, v3, v4));
			view1 = mat4(v5, v6, v7, v8);
			projection1 = perspective(radians(45.0f), float(800) / float(600), 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader[1], "rotx1"), 1, GL_FALSE, &rotate1[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[1], "vie1"), 1, GL_FALSE, &view1[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[1], "project1"), 1, GL_FALSE, &projection1[0][0]);

			vec4 v9 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			vec4 v10 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			vec4 v11 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
			vec4 v12 = vec4(t1, 0.0f, t, 1.0f);
		
			mat4 move = mat4(v9, v10, v11, v12);
			glUniformMatrix4fv(glGetUniformLocation(shader[1], "tran"), 1, GL_FALSE, &move[0][0]);

			glBindVertexArray(vert[1]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindVertexArray(vert[2]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[2]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glUseProgram(shader[3]);

			glUniformMatrix4fv(glGetUniformLocation(shader[3], "rotx3"), 1, GL_FALSE, &rotate1[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[3], "vie3"), 1, GL_FALSE, &view1[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[3], "project3"), 1, GL_FALSE, &projection1[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[3], "tran3"), 1, GL_FALSE, &move[0][0]);

			glBindVertexArray(vert[4]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[4]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vert[5]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[5]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vert[6]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[6]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
			glBindVertexArray(vert[7]);
			glBindBuffer(GL_ARRAY_BUFFER, buff[7]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glUseProgram(shader[4]);
			glUniformMatrix4fv(glGetUniformLocation(shader[4], "rotx4"), 1, GL_FALSE, &rotate[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[4], "vie4"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[4], "project4"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader[4], "roa4"), 1, GL_FALSE, &roadmove[0][0]);

			vec3 translation1[] = {
									   vec3(0.0f,0.0f,0.0f),
									   vec3(-0.05 + (cos(t8) / 4.5f),0.0f,-0.4f - (sin(t8) / 4.0f)),
									   vec3(0.2f + (cos(t8) / 4.5f),0.0f,-0.6f + (sin(t8) / 4.0f)),
									   vec3(-0.4f + (cos(t8) / 4.5f),0.0f,-0.8f - (sin(t8) / 4.0f))
			};

			mat4 loc2 = mat4(1.0f);
			mat4 loc3 = mat4(1.0f);
			mat4 loc4 = mat4(1.0f);
			mat4 loc5 = mat4(1.0f);

			loc2 = translate(loc2, translation1[0]);
			loc3 = translate(loc3, translation1[1]);
			loc4 = translate(loc4, translation1[2]);
			loc5 = translate(loc5, translation1[3]);

			for (int i = 0;i < 4;i++)
			{
				mat4 loc1 = mat4(1.0f);
				loc1 = translate(loc1, translation1[i]);
				glUniformMatrix4fv(glGetUniformLocation(shader[4], "tr4"), 1, GL_FALSE, &loc1[0][0]);
				glBindVertexArray(vert[10]);
				glBindBuffer(GL_ARRAY_BUFFER, buff[10]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
			}
			mat4 colla = roadmove * loc2;
			mat4 colla1 = roadmove * loc3;
			mat4 colla2 = roadmove * loc4;
			mat4 colla3 = roadmove * loc5;
			mat4 collb = move;

			vec4 cal = colla * objectclone;
			vec4 cal1 = colla1 * objectclone;
			vec4 cal2 = colla2 * objectclone;
			vec4 cal3 = colla3 * objectclone;

			vec4 cal4 = colla * objectclone1;
			vec4 cal5 = colla1 * objectclone1;
			vec4 cal6 = colla2 * objectclone1;
			vec4 cal7 = colla3 * objectclone1;

			vec4 calcar = collb * carclone;
			 
				if (cal.z > calcar.z && (calcar.x < fabsf(cal4.x + cal.x) && cal4.x > calcar.x && calcar.x > cal.x))
				{
					gameover = true;
				}

				else  if (cal1.z > calcar.z && (calcar.x < fabsf(cal5.x + cal1.x) && cal5.x > calcar.x && calcar.x>cal1.x))

				{

					gameover = true;

				}


				else if (cal2.z > calcar.z && (calcar.x < fabsf(cal6.x + cal2.x) && cal6.x > calcar.x && calcar.x > cal2.x))
				{
					gameover = true;
				}

				else if (cal3.z > calcar.z && (calcar.x < fabsf(cal7.x + cal3.x) && cal7.x > calcar.x && calcar.x > cal3.x))
				{
					gameover = true;
				}

			if (gameover)
			{
				glfwTerminate();
				cout << " #####     #    #     # ####### ####### #     # ####### ######\n";
				cout << "#     #   # #   ##   ## #       #     # #     # #       #     #\n";
				cout << "#        #   #  # # # # #       #     # #     # #       #     #\n";
				cout << "#  #### #     # #  #  # #####   #     # #     # #####   ######\n";
				cout << "#     # ####### #     # #       #     #  #   #  #       #   #\n";
				cout << "#     # #     # #     # #       #     #   # #   #       #    #\n";
				cout << " #####  #     # #     # ####### #######    #    ####### #     #\n";

				cout << "\n\nscore=" << score;
				cout << "\n";
				return -1;
			}

			srand(time(NULL));
			t3 += 0.004;
			if (t3 > 5.5f)
			{
				t3 = 0.0f;
				t8 = rand() % 101;
			}

			if (glfwGetKey(window, GLFW_KEY_UP))
			{
				t3 += 0.006;
			}


			if (glfwGetKey(window, GLFW_KEY_LEFT))
			{
				t1 -= 0.001;

				if (t1 < -0.55f)
				{
					t1 = -0.55f;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_RIGHT))
			{
				t1 += 0.001;
				if (t1 > 0.55f)
				{
					t1 = 0.55f;
				}
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
			score = score + 10;
		
}
	glfwTerminate();
	return 0;
}