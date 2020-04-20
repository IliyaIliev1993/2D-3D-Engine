CG=g++
LIBS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJ=main.cpp gl_includes/glad.c renderer/Renderer.cpp texture/Texture.cpp handle_event/HandleEvent.cpp timer_mgr/TimerMgr.cpp debug/Debug.cpp gl_includes/imgui/imgui.cpp gl_includes/imgui/imgui_demo.cpp gl_includes/imgui/imgui_draw.cpp gl_includes/imgui/imgui_impl_glfw_gl3.cpp
all: $(OBJ)
	$(CG) $(OBJ) -o exe $(LIBS)
