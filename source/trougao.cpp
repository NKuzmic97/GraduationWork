#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


GLuint KompajlujSejder(unsigned int tip, const std::string& izvorniKod) {
	// Креирамо шејдер и чувамо његову идентификацију.
	GLuint idSejdera = glCreateShader(tip);
	// Кастујемо C++ стринг у C стринг како би могли да га проследимо OpenGL функцији.
	const char* izvorniKodC = izvorniKod.c_str();
	// Пунимо наш новокреирани шејдер са спољним изворним кодом.
	glShaderSource(idSejdera, 1, &izvorniKodC, NULL);
	// Компајлујемо шејдер.
	glCompileShader(idSejdera);

	int rezultatProvere;
	// Смештамо статус компилације у резултат провере.
	glGetShaderiv(idSejdera, GL_COMPILE_STATUS, &rezultatProvere);
	if (rezultatProvere == GL_FALSE) {
		// Уколико је неуспешна компилација:
		int duzinaPoruke;
		// Дохватамо дужину поруке.
		glGetShaderiv(idSejdera, GL_INFO_LOG_LENGTH, &duzinaPoruke);
		// Креирамо низ карактера на heap-у пошто променљива duzinaPoruke није позната у време компилације, 
		// самим тим се не може креирати на stack-у.
		char* poruka = new char[duzinaPoruke];
		// Новокреирани низ пунимо записом из шејдера.
		glGetShaderInfoLog(idSejdera, duzinaPoruke, &duzinaPoruke, poruka);

		std::cout << "Kompilacija sejdera neuspesna!" << std::endl;
		std::cout << poruka << std::endl;

		// Бришемо динамички алоцирану меморију.
		delete[] poruka;
	}

	return idSejdera;
}


// Функција прима као аргументе два изворна кода у виду стрингова.
GLuint KreirajProgram(const std::string& temeSejderKod, const std::string& fragmentSejderKod) {
	// Креирамо OpenGL програм и чувамо његову идентификацију.
	GLuint programID = glCreateProgram();
	// Компајлујемо теме шејдера и чувамо његову идентификацију.
	GLuint tsID = KompajlujSejder(GL_VERTEX_SHADER, temeSejderKod);
	// Компајлујемо фрагмент шејдер и чувамо његову идентификацију.
	GLuint vsID = KompajlujSejder(GL_FRAGMENT_SHADER, fragmentSejderKod);

	// Качимо шејдере за OpenGL програм.
	glAttachShader(programID, tsID);
	glAttachShader(programID, vsID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	return programID;
}

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Zdravo svete!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Иницијализација GLEW библиотеке.
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		// Провера успешности иницијализације.
		std::cout << "Greska: " << glewGetErrorString(err) << std::endl;
	}
	// Испис тренутне верзије GLEW-а коју користимо.
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	float pozicije[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f,
	};


	// Идентификација нашег бафера (сваки објекат у OpenGL је има)
	unsigned int baferID;
	// Генеришемо један бафер и његову идентификацију смештамо у baferID
	glGenBuffers(1, &baferID);
	// Наш бафер качимо за контекст
	glBindBuffer(GL_ARRAY_BUFFER, baferID);
	// Пунимо наш бафер са подацима из низа позиција.
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pozicije, GL_STATIC_DRAW);

	// Омогућавамо атрибут темена са индексом 0 (позиција)
	glEnableVertexAttribArray(0);
	// Дефинишемо распоред меморије нашег бафера - атрибут величине два float типа.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);


	std::string temeSejderKod = R"glsl(
		#version 330 core
		layout(location = 0) in vec4 position;
		void main(){
			gl_Position = position;
		}
	)glsl";

	std::string fragmentSejderKod = R"glsl(
		#version 330 core
		layout(location = 0) out vec4 color;
		void main(){
			color = vec4(1.0, 0.0, 0.0, 1.0);
		}
	)glsl";

	GLuint sejderProgram = KreirajProgram(temeSejderKod, fragmentSejderKod);
	glUseProgram(sejderProgram);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}