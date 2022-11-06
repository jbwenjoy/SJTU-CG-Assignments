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

// ��������
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// ����Ƕ�����ɫ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float scale;\n"
"uniform float angle;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(scale * (aPos.x * cos(angle) - aPos.y * sin(angle)), scale * (aPos.x * sin(angle) + aPos.y * cos(angle)), aPos.z, 1.0);\n"
"}\0";

// �����ƬԪ��ɫ��
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

// ��˸Ч���Ķ�����ɫ��
const char* vertexShaderSource_glowing = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ 0 
"layout (location = 1) in vec3 glowingVertexColor;\n"
"uniform float glowingColorAValue;\n"
"out vec4 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = vec4(glowingVertexColor, glowingColorAValue);\n"
"}\0";

// ��˸Ч����ƬԪ��ɫ��
const char* fragmentShaderSource_glowing = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

int main()
{
	// glfw ��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw ��������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW1 Glowing Pentagram", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad ����OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����Ȳ���
	glEnable(GL_DEPTH_TEST);

	// -----�������ɫ��-----
	// ���붥����ɫ��
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

	// ����ƬԪ��ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������ɫ��
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

	// -----���������-----
	// ���ö�������
	const float cos72 = cos(72.0 / 180.0 * M_PI);
	const float sin72 = sin(72.0 / 180.0 * M_PI);
	const float cos36 = cos(36.0 / 180.0 * M_PI);
	const float sin36 = sin(36.0 / 180.0 * M_PI);
	const float sin18 = cos72;
	const float cos18 = sin72;
	const float short_r = 0.5f * static_cast<float>(sin(18.0 / 180.0 * M_PI) / sin(126.0 / 180.0 * M_PI));
	const float vertices[] = {
		0.0f, 0.0f, 0.0f,										// ����
		0.0f, 0.5f, 0.1f,											// ��-��
		-1.0f * short_r * sin36, short_r * cos36, 0.1f,			// ��-����
		-0.5f * cos18, 0.5f * sin18, 0.1f,							// ��-����
		-1.0f * short_r * cos18, -1.0f * short_r * sin18, 0.1f,	// ��-����
		-0.5f * sin36, -0.5f * cos36, 0.1f,							// ��-����
		0.0f , -1.0f * short_r, 0.1f,							// ��-��
		0.5f * sin36, -0.5f * cos36, 0.1f,							// ��-����
		1.0f * short_r * cos18, -1.0f * short_r * sin18, 0.1f,	// ��-����
		0.5f * cos18, 0.5f * sin18, 0.1f,							// ��-����
		1.0f * short_r * sin36, short_r* cos36, 0.1f			// ��-����
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

	// ���ö��㻺�����VBO�붥���������VAO
	unsigned int VBO[2], VAO[2], EBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	// ǳɫ��
	// ��������󶨵������������VAO
	glBindVertexArray(VAO[0]);
	// ��VBO��GL_ARRAY_BUFFER�󶨣���д��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ��EBO��GL_ELEMENT_ARRAY_BUFFER�󶨣���д��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_0), indices_0, GL_STATIC_DRAW);
	// ����OpenGL����ν����������ݣ�һ���м��ֶ������Ծ͵������������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	// ���ǰ��������Բ��ܻ��ƣ�����Ϊ��������λ��ֵ��shader�����õ�locationΪ0
	glEnableVertexAttribArray(0);
	// ���
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ��ɫ��
	// ��������󶨵������������VAO
	glBindVertexArray(VAO[1]);
	// ��VBO��GL_ARRAY_BUFFER�󶨣���д��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ��EBO��GL_ELEMENT_ARRAY_BUFFER�󶨣���д��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);
	// ����OpenGL����ν����������ݣ�һ���м��ֶ������Ծ͵������������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	// ���ǰ��������Բ��ܻ��ƣ�����Ϊ��������λ��ֵ��shader�����õ�locationΪ0
	glEnableVertexAttribArray(0);
	// ���
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// -----��˸Ч��-----
	// С�����α�������
	float vertices_glowing[361 * 6] = {0.0f};
	vertices_glowing[2] = 0.2f; // ����������
	vertices_glowing[3] = 0.0f; // ���������ɫRGB
	vertices_glowing[4] = 0.0f;
	vertices_glowing[5] = 0.0f;
	for (int i = 1; i < 361; ++i) // ֻ��ʼ����Ȧ�㣬���ĵ��൱��i=0
	{
		// �յ�����
		vertices_glowing[6 * i] = 1.0f * static_cast<float>(cos(i * M_PI / 180.0)); // x����
		vertices_glowing[6 * i + 1] = 1.0f * static_cast<float>(sin(i * M_PI / 180.0)); // y����
		vertices_glowing[6 * i + 2] = 0.2f; // �������
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

	// ���������θ�������ɫ������
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

	// ������˸������ɫ��
	unsigned int vertexShader_glowing = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_glowing, 1, &vertexShaderSource_glowing, nullptr);
	glCompileShader(vertexShader_glowing);
	glGetShaderiv(vertexShader_glowing, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader_glowing, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::GLOWING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������˸ƬԪ��ɫ��
	unsigned int fragmentShader_glowing = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_glowing, 1, &fragmentShaderSource_glowing, nullptr);
	glCompileShader(fragmentShader_glowing);
	glGetShaderiv(fragmentShader_glowing, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_glowing, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::GLOWING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������˸��ɫ��
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

	// ���ö��㻺�����VBO�붥���������VAO
	unsigned int VBOs, VAOs, EBOs;
	glGenVertexArrays(1, &VAOs);
	glGenBuffers(1, &VBOs);
	glGenBuffers(1, &EBOs);

	// ��������󶨵������������VAO
	glBindVertexArray(VAOs);
	// ��VBO��GL_ARRAY_BUFFER�󶨣���д��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_glowing), vertices_glowing, GL_STREAM_DRAW);
	// ��EBO��GL_ELEMENT_ARRAY_BUFFER�󶨣���д��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_glowing), indices_glowing, GL_STATIC_DRAW);
	// ����OpenGL����ν�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// ���ǰ��������Բ��ܻ��ƣ�����Ϊ��������λ��ֵ
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	

	// ���
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// -----��Ⱦѭ��-----
	int case_flag = 1; // ��������
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window, case_flag);

		// �ȼ���shader����
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

		// ��Ӧ����
		switch (case_flag)
		{
		case 1:
			// ��Ⱦ
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
			// ��Ⱦ
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
			// ��Ⱦ
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

			// ����ǣ���
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glUniform1f(scaleLocation, scale_times);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 0.1f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
			// ����ǣ���
			glBindVertexArray(VAO[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
			glUniform1f(scaleLocation, scale_times);
			glUniform1f(angleLocation, angleValue);
			glUniform4f(vertexColorLocation, 0.8f, 0.0f, 0.0f, 0.3f);
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

			// ��˸����
			for (int i = 1; i < 361; ++i) // ֻ������Ȧ�㣬�����൱��i=0
			{
				// ��ͬʱ��ĳ��ȱ仯
				vertices_glowing[6 * i] = static_cast<float>(cos(6 * timeValue + i * M_PI / 3) * cos(i * M_PI / 180.0)); // x����
				vertices_glowing[6 * i + 1] = static_cast<float>(cos(6 * timeValue + i * M_PI / 3) * sin(i * M_PI / 180.0)); // y����
				// ��ͬ�Ƕȵĳ��ȱ仯
				vertices_glowing[6 * i] *= 1.0f + 0.5f * static_cast<float>(cos(timeValue * i / 10));
				vertices_glowing[6 * i + 1] *= 1.0f + 0.5f * static_cast<float>(cos(timeValue * i / 10));
				// ��ɫ�仯
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
			// ��Ⱦ
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

		// glfw ������ɫ���岢��ⴥ���¼�
		glfwSwapBuffers(window); // ˫���彻����ɫ���壬���ڻ������
		glfwPollEvents(); // ��ⴥ���¼����������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
	}

	// ������Դ
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VAOs);
	glDeleteBuffers(1, &VBOs);
	glDeleteBuffers(1, &EBOs);
	glDeleteProgram(shaderProgram_glowing);

	// glfw ����
	glfwTerminate();
	return 0;
}

// �����������
void processInput(GLFWwindow* window, int &case_flag)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ��ESC�˳�
	{
		case_flag = 0;
		glfwSetWindowShouldClose(window, true);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // ��C������ʾ
	{
		case_flag = 2;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // ��S��˸
	{
		case_flag = 3;
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // ��Q��ԭ
	{
		case_flag = 1;
		return;
	}

}

// glfw �洰�ڴ�С�ı�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
