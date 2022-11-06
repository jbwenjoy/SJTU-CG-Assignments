#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, int &case_flag);

// 窗口设置
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// 五角星顶点着色器
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float scale;\n"
"uniform float angle;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(scale * (aPos.x * cos(angle) - aPos.y * sin(angle)), scale * (aPos.x * sin(angle) + aPos.y * cos(angle)), aPos.z, 1.0);\n"
"}\0";

// 五角星片元着色器
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

// 闪烁效果的顶点着色器
const char* vertexShaderSource_glowing = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // 位置变量的属性位置值为 0 
"layout (location = 1) in vec3 glowingVertexColor;\n"
"uniform float glowingColorAValue;\n"
"out vec4 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = vec4(glowingVertexColor, glowingColorAValue);\n"
"}\0";

// 闪烁效果的片元着色器
const char* fragmentShaderSource_glowing = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

int main()
{
	// glfw 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW1 Glowing Pentagram", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad 加载OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 打开深度测试
	glEnable(GL_DEPTH_TEST);

	// -----五角星着色器-----
	// 编译顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 编译片元着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 链接着色器
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// -----五角星属性-----
	// 设置顶点数据
	const float cos72 = cos(72.0 / 180.0 * M_PI);
	const float sin72 = sin(72.0 / 180.0 * M_PI);
	const float cos36 = cos(36.0 / 180.0 * M_PI);
	const float sin36 = sin(36.0 / 180.0 * M_PI);
	const float sin18 = cos72;
	const float cos18 = sin72;
	const float short_r = 0.5f * static_cast<float>(sin(18.0 / 180.0 * M_PI) / sin(126.0 / 180.0 * M_PI));
	const float vertices[] = {
		0.0f, 0.0f, 0.0f,										// 中心
		0.0f, 0.5f, 0.1f,											// 外-顶
		-1.0f * short_r * sin36, short_r * cos36, 0.1f,			// 内-左上
		-0.5f * cos18, 0.5f * sin18, 0.1f,							// 外-左上
		-1.0f * short_r * cos18, -1.0f * short_r * sin18, 0.1f,	// 内-左下
		-0.5f * sin36, -0.5f * cos36, 0.1f,							// 外-左下
		0.0f , -1.0f * short_r, 0.1f,							// 内-下
		0.5f * sin36, -0.5f * cos36, 0.1f,							// 外-右下
		1.0f * short_r * cos18, -1.0f * short_r * sin18, 0.1f,	// 内-右下
		0.5f * cos18, 0.5f * sin18, 0.1f,							// 外-右上
		1.0f * short_r * sin36, short_r* cos36, 0.1f			// 内-右上
	};

	const unsigned int indices_0[] = {
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8,
		0, 9, 10
	};

	const unsigned int indices_1[] = {
		0, 2, 3,
		0, 4, 5,
		0, 6, 7,
		0, 8, 9,
		0, 10, 1
	};

	// 设置顶点缓冲对象VBO与顶点数组对象VAO
	unsigned int VBO[2], VAO[2], EBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	// 浅色组
	// 顶点数组绑定到顶点数组对象VAO
	glBindVertexArray(VAO[0]);
	// 将VBO与GL_ARRAY_BUFFER绑定，并写入VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 将EBO与GL_ELEMENT_ARRAY_BUFFER绑定，并写入EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_0), indices_0, GL_STATIC_DRAW);
	// 告诉OpenGL该如何解析顶点数据，一般有几种顶点属性就调几次这个函数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	// 解绑前激活顶点属性才能绘制，参数为顶点属性位置值，shader中设置的location为0
	glEnableVertexAttribArray(0);
	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 深色组
	// 顶点数组绑定到顶点数组对象VAO
	glBindVertexArray(VAO[1]);
	// 将VBO与GL_ARRAY_BUFFER绑定，并写入VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 将EBO与GL_ELEMENT_ARRAY_BUFFER绑定，并写入EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);
	// 告诉OpenGL该如何解析顶点数据，一般有几种顶点属性就调几次这个函数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	// 解绑前激活顶点属性才能绘制，参数为顶点属性位置值，shader中设置的location为0
	glEnableVertexAttribArray(0);
	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// -----闪烁效果-----
	// 小三角形背景顶点
	float vertices_glowing[361 * 6] = {0.0f};
	vertices_glowing[2] = 0.2f; // 设置起点深度
	vertices_glowing[3] = 0.0f; // 设置起点颜色RGB
	vertices_glowing[4] = 0.0f;
	vertices_glowing[5] = 0.0f;
	for (int i = 1; i < 361; ++i) // 只初始化外圈点，中心点相当于i=0
	{
		// 终点坐标
		vertices_glowing[6 * i] = 1.0f * static_cast<float>(cos(i * M_PI / 180.0)); // x坐标
		vertices_glowing[6 * i + 1] = 1.0f * static_cast<float>(sin(i * M_PI / 180.0)); // y坐标
		vertices_glowing[6 * i + 2] = 0.2f; // 深度坐标
		// RGB
		vertices_glowing[6 * i + 3] = 0.0f;
		vertices_glowing[6 * i + 4] = 0.5f;
		vertices_glowing[6 * i + 5] = 1.0f;
	}
	//for (int i = 0; i < 360; ++i)
	//{
	//	std::cout << vertices_glowing[6 * i] << "\t";
	//	std::cout << vertices_glowing[6 * i + 1] << "\t";
	//	std::cout << vertices_glowing[6 * i + 2] << "\t";
	//	std::cout << vertices_glowing[6 * i + 3] << "\t";
	//	std::cout << vertices_glowing[6 * i + 4] << "\t";
	//	std::cout << vertices_glowing[6 * i + 5] << "\n";
	//}
	//std::cout << "\n\n+++++++++++++++++++++++++++++++++\n\n";

	unsigned int indices_glowing[360 * 3] = {0};
	for (int i = 0; i < 360; ++i)
	{
		indices_glowing[3 * i] = 0;
		indices_glowing[3 * i + 1] = i + 1;
		indices_glowing[3 * i + 2] = i + 2;
	}
	indices_glowing[360 * 3 - 1] = 1;
	//for (int i = 0; i < 360; ++i)
	//{
	//	std::cout << indices_glowing[3 * i] << "\t";
	//	std::cout << indices_glowing[3 * i + 1] << "\t";
	//	std::cout << indices_glowing[3 * i + 2] << "\n";
	//}

	// 背景三角形各顶点颜色的数组
	//float vertices_glowing_color[361 * 3] = {0};
	//for (int i = 1; i < 361; ++i)
	//{
	//	vertices_glowing_color[3 * i] = 0.0f;
	//	vertices_glowing_color[3 * i + 1] = 1.0f;
	//	vertices_glowing_color[3 * i + 2] = 0.0f;
	//}
	//for (int i = 0; i < 361; ++i)
	//{
	//	std::cout << vertices_glowing_color[3 * i] << "\t";
	//	std::cout << vertices_glowing_color[3 * i + 1] << "\t";
	//	std::cout << vertices_glowing_color[3 * i + 2] << "\n";
	//}

	// 编译闪烁顶点着色器
	unsigned int vertexShader_glowing = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_glowing, 1, &vertexShaderSource_glowing, nullptr);
	glCompileShader(vertexShader_glowing);
	glGetShaderiv(vertexShader_glowing, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader_glowing, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::GLOWING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 编译闪烁片元着色器
	unsigned int fragmentShader_glowing = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_glowing, 1, &fragmentShaderSource_glowing, nullptr);
	glCompileShader(fragmentShader_glowing);
	glGetShaderiv(fragmentShader_glowing, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_glowing, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::GLOWING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 链接闪烁着色器
	unsigned int shaderProgram_glowing = glCreateProgram();
	glAttachShader(shaderProgram_glowing, vertexShader_glowing);
	glAttachShader(shaderProgram_glowing, fragmentShader_glowing);
	glLinkProgram(shaderProgram_glowing);
	glGetProgramiv(shaderProgram_glowing, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_glowing, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::GLOWING::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader_glowing);
	glDeleteShader(fragmentShader_glowing);

	// 设置顶点缓冲对象VBO与顶点数组对象VAO
	unsigned int VBOs, VAOs, EBOs;
	glGenVertexArrays(1, &VAOs);
	glGenBuffers(1, &VBOs);
	glGenBuffers(1, &EBOs);

	// 顶点数组绑定到顶点数组对象VAO
	glBindVertexArray(VAOs);
	// 将VBO与GL_ARRAY_BUFFER绑定，并写入VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_glowing), vertices_glowing, GL_STREAM_DRAW);
	// 将EBO与GL_ELEMENT_ARRAY_BUFFER绑定，并写入EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_glowing), indices_glowing, GL_STATIC_DRAW);
	// 告诉OpenGL该如何解析顶点数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// 解绑前激活顶点属性才能绘制，参数为顶点属性位置值
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	

	// 解绑
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// -----渲染循环-----
	int case_flag = 1; // 按键变量
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window, case_flag);

		// 先激活shader程序
		glUseProgram(shaderProgram);
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
		int scaleLocation = glGetUniformLocation(shaderProgram, "scale");
		int angleLocation = glGetUniformLocation(shaderProgram, "angle");
		glUseProgram(shaderProgram_glowing);
		int glowingColorAValueLocation = glGetUniformLocation(shaderProgram_glowing, "glowingColorAValue");

		double timeValue = glfwGetTime();
		float redValue = 0;
		float greenValue = 0;
		float blueValue = 0;
		float glowingColorAValue = 0;
		float scale_times = 1;
		float angleValue = 0;

		// 响应按键
		switch (case_flag)
		{
		case 1:
			// 渲染
			glClearColor(0.6f, 0.6f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

			glBindVertexArray(VAO[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
			break;

		case 2:
			// 渲染
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

			glBindVertexArray(VAO[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 0.8f, 0.0f, 0.0f, 1.0f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
			break;

		case 3:
			// 渲染
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			timeValue = glfwGetTime();
			redValue = static_cast<float>(cos(3 * timeValue - 2 * M_PI / 3) / 2.0 + 0.5);
			greenValue = static_cast<float>(cos(3 * timeValue) / 2.0 + 0.5);
			blueValue = static_cast<float>(cos(3 * timeValue + 2 * M_PI / 3) / 2.0 + 0.5);
			glowingColorAValue = static_cast<float>(sin(3 * timeValue) / 2.0 + 0.5);
			scale_times = static_cast<float>(1 + 0.1 * sin(3 * timeValue) + 0.01 * cos(120 * timeValue));
			angleValue = static_cast<float>(0.015 * sin(timeValue * 180 * 30 / M_PI));

			// 五角星，亮
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glUniform1f(scaleLocation, scale_times);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 0.1f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
			// 五角星，暗
			glBindVertexArray(VAO[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
			glUniform1f(scaleLocation, scale_times);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 0.8f, 0.0f, 0.0f, 0.3f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

			// 闪烁背景
			for (int i = 1; i < 361; ++i) // 只更新外圈点，中心相当于i=0
			{
				// 不同时间的长度变化
				vertices_glowing[6 * i] = static_cast<float>(cos(6 * timeValue + i * M_PI / 3) * cos(i * M_PI / 180.0)); // x坐标
				vertices_glowing[6 * i + 1] = static_cast<float>(cos(6 * timeValue + i * M_PI / 3) * sin(i * M_PI / 180.0)); // y坐标
				// 不同角度的长度变化
				vertices_glowing[6 * i] *= 1.0f + 0.5f * static_cast<float>(cos(timeValue * i / 10));
				vertices_glowing[6 * i + 1] *= 1.0f + 0.5f * static_cast<float>(cos(timeValue * i / 10));
				// 颜色变化
				vertices_glowing[6 * i + 3] = redValue; // R
				vertices_glowing[6 * i + 4] = greenValue; // G
				vertices_glowing[6 * i + 5] = blueValue; // B
			}
			glUseProgram(shaderProgram_glowing);
			glBindVertexArray(VAOs);
			glBindBuffer(GL_ARRAY_BUFFER, VBOs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_glowing), vertices_glowing, GL_STREAM_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glUniform1f(glowingColorAValueLocation, glowingColorAValue);
			glDrawElements(GL_TRIANGLES, 360 * 3, GL_UNSIGNED_INT, 0);
			break;

		case 0:
		default:
			// 渲染
			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 33, GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(VAO[1]);
			glUniform1f(scaleLocation, 1.0f);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 33, GL_UNSIGNED_INT, nullptr);
		}

		// glfw 交换颜色缓冲并检测触发事件
		glfwSwapBuffers(window); // 双缓冲交换颜色缓冲，用于绘制输出
		glfwPollEvents(); // 检测触发事件（键盘输入、鼠标移动等），更新窗口状态，并调用对应的回调函数
	}

	// 回收资源
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VAOs);
	glDeleteBuffers(1, &VBOs);
	glDeleteBuffers(1, &EBOs);
	glDeleteProgram(shaderProgram_glowing);

	// glfw 结束
	glfwTerminate();
	return 0;
}

// 处理键盘输入
void processInput(GLFWwindow* window, int &case_flag)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 按ESC退出
	{
		case_flag = 0;
		glfwSetWindowShouldClose(window, true);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // 按C立体显示
	{
		case_flag = 2;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // 按S闪烁
	{
		case_flag = 3;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // 按Q复原
	{
		case_flag = 1;
		return;
	}

}

// glfw 随窗口大小改变
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
