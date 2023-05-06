#
# Desktop application build
#
esport.app:

	gcc \
	-DGL_SILENCE_DEPRECATION \
	-I./include \
	-I/opt/homebrew/Cellar/cglm/0.8.9/include \
	-I/opt/homebrew/Cellar/glfw/3.3.8/include \
	-I/opt/homebrew/Cellar/glew/2.2.0_1/include \
	-L/opt/homebrew/lib \
	-lglfw -lglew -framework CoreVideo -framework OpenGL -framework GLUT \
	-framework IOKit -framework Cocoa -framework Carbon \
	src/esport.c \
	-o esport.app

clean:
	rm -f tmp/*.o
	rm -f *.out
	rm -f *.exe
	rm -f *.app

run: clean esport.app
	./esport.app

.PHONY=clean run
