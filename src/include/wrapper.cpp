#include "wrapper.hpp"
#include "glad/glad.h"
#include "imgui185/imgui_impl_glfw.h"
#include "imgui185/imgui_impl_opengl3.h"




static void gl_debug_message_callback(
	GLenum 		  source, 
	GLenum 		  type, 
	GLuint 		  id, 
	GLenum 		  severity, 
	GLsizei		  length, 
	GLchar const* message, 
	void const*   user_param
);




void Window::glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->cursor = { (float)xpos, (float)ypos };
	return;
}


void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->dims = { width, height };

	glViewport(0, 0, width, height);
	return;
}


void Window::glfw_error_callback(int error, const char* description)
{
	printf("GLFW Error %d: %s\n", error, description);
}


void Window::create(i32 WindowWidth, i32 WindowHeight)
{
	glfwSetErrorCallback(glfw_error_callback);
	ifcrashdo(!glfwInit(), { printf("GLFW Initialization Failed!\n"); });


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER , 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);


	f32 winCol[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
	memcpy(&defaultCol, &winCol, sizeof(winCol));

	dims = { WindowWidth, WindowHeight };
	window = glfwCreateWindow(WindowWidth, WindowHeight, "ImGui GLFW+OpenGL-4.5 App Window", NULL, NULL);
	ifcrashdo(!window, { 
		printf("GLFW Window Creation Failed!\n");
		glfwTerminate(); 
	});


	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this); /* get access to class inside static member functions. */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(0); // Disable vsync
	

	ifcrashdo( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), 
	{ 
		printf("Couldn't initialize GLAD\n");

		glfwDestroyWindow(window);
		glfwTerminate();
	});
    

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGui::StyleColorsDark();    
	ImGuiIO& io = ImGui::GetIO(); io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	(void)io;


	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ifcrash(!ImGui_ImplOpenGL3_Init("#version 450"));


	debug({
		printf("Active Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Current OpenGL Version %s\n", glGetString(GL_VERSION));
	});
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	debug( /* Enable Advanced Debugging if enabled. */
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug_message_callback, nullptr);
	);
}


void Window::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::beginFrame()
{
	ImVec4 color {
		defaultCol[0] * defaultCol[3],
		defaultCol[1] * defaultCol[3],
		defaultCol[2] * defaultCol[3],
		defaultCol[3]
	};
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();	
}


void Window::endFrame() 
{
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwPollEvents();
	glfwSwapBuffers(window);
}



#ifdef _DEBUG
static void gl_debug_message_callback(
	GLenum 		  source, 
	GLenum 		  type, 
	GLuint 		  id, 
	GLenum 		  severity, 
	GLsizei		  length, 
	GLchar const* message, 
	void const*   user_param
) {
	const std::pair<u32, const char*> srcStr[6] = {
		{ GL_DEBUG_SOURCE_API,             "API" 			 },
		{ GL_DEBUG_SOURCE_WINDOW_SYSTEM,   "WINDOW SYSTEM"   },
		{ GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER COMPILER" },
		{ GL_DEBUG_SOURCE_THIRD_PARTY,	   "THIRD PARTY" 	 },
		{ GL_DEBUG_SOURCE_APPLICATION,	   "APPLICATION" 	 },
		{ GL_DEBUG_SOURCE_OTHER, 		   "OTHER" 			 }
	};
	const std::pair<u32, const char*> typeStr[7] = {
		{ GL_DEBUG_TYPE_ERROR, 			     "ERROR"               },
		{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR" },
		{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "UNDEFINED_BEHAVIOR"  },
		{ GL_DEBUG_TYPE_PORTABILITY,		 "PORTABILITY" 	       },
		{ GL_DEBUG_TYPE_PERFORMANCE,		 "PERFORMANCE" 		   },
		{ GL_DEBUG_TYPE_MARKER,			   	 "MARKER" 			   },
		{ GL_DEBUG_TYPE_OTHER,			     "OTHER" 			   }
	};
	const std::pair<u32, const char*> severityStr[6] = {
		{ GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION" },
		{ GL_DEBUG_SEVERITY_LOW, 		  "LOW"		     },
		{ GL_DEBUG_SEVERITY_MEDIUM, 	  "MEDIUM"	     },
		{ GL_DEBUG_SEVERITY_HIGH, 		  "HIGH"	     }
	};
	const char* src_str      = srcStr[0].second;
	const char* type_str     = typeStr[0].second;
	const char* severity_str = severityStr[0].second;
	u32 		idx 		 = 0;
	

	while(srcStr[idx].first != source) { ++idx; }
	src_str = srcStr[idx].second;
	idx = 0;

	while(typeStr[idx].first != type)  { ++idx; }
	type_str = typeStr[idx].second;
	idx = 0;

	while(severityStr[idx].first != severity)  { ++idx; }
	severity_str = severityStr[idx].second;
	idx = 0;
	
	
	printf("OPENGL >> %s::%s::%s %u: %s\n", src_str, type_str, severity_str, id, message);
	return;
}
#endif