#
# Desktop application build
#
esport.app:
	gcc -g \
	src/objloader.c \
	src/model.c src/camera.c src/mesh.c src/buffers.c src/ppm.c \
	src/texture.c src/shader.c src/esport.c \
	-o esport.app \
	-DGL_SILENCE_DEPRECATION \
	-I./include \
	-I/opt/homebrew/Cellar/cglm/0.8.9/include \
	-I/opt/homebrew/Cellar/glfw/3.3.8/include \
	-I/opt/homebrew/Cellar/glew/2.2.0_1/include \
	-L/opt/homebrew/lib \
	-lglfw -lglew -framework CoreVideo -framework OpenGL -framework GLUT \
	-framework IOKit -framework Cocoa -framework Carbon

esport.elf:
	gcc -g \
	src/objloader.c \
	src/model.c src/camera.c src/mesh.c src/buffers.c src/ppm.c \
	src/texture.c src/shader.c src/esport.c \
	-o esport.elf \
	-DGL_SILENCE_DEPRECATION \
	-I./include \
	-I./usr/include \
	-lGL -lGLEW -lglfw -lrt -ldl -lm -lcglm

clean:
	rm -f tmp/*.o
	rm -f *.out
	rm -f *.exe
	rm -f *.app
	rm -f *.elf
	# rm -fr assets/ppm
	# rm -fr assets/obj

run: clean assets/obj assets/ppm esport.app
	./esport.app

.PHONY=clean run

assets/ppm:
	mkdir -p assets/ppm
	mogrify -path assets/ppm -format ppm -fill "rgb(255,0,255)" -opaque "transparent" assets/png/*.png
	mogrify -path assets/ppm -format ppm assets/jpg/*.jpg
	# mogrify -path assets/ppm/alpha -format ppm -alpha extract assets/png/*.png

assets/obj:
	mkdir -p assets/obj
	blender -b assets/level-map.blend -o assets/obj/level-map.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/level-map.obj', use_triangles=True)"
	blender -b assets/luke.blend -o assets/obj/luke.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/luke.obj', use_triangles=True)"
	blender -b assets/cube.blend -o assets/obj/cube.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/cube.obj', use_triangles=True)"
	blender -b assets/sky.blend -o assets/obj/sky.obj --python-expr \
		"import bpy; bpy.ops.export_scene.obj(filepath='assets/obj/sky.obj', use_triangles=True)"
