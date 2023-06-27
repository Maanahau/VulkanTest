STB_INCLUDE_PATH = /home/maanahau/Libraries/cpp/stb

CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi


a.out: *.cpp *.hpp
	g++ $(CFLAGS) -o a.out *.cpp -I$(STB_INCLUDE_PATH) $(LDFLAGS)

./shaders/simple_shader.vert.spv: ./shaders/simple_shader.vert
	glslc ./shaders/simple_shader.vert -o ./shaders/simple_shader.vert.spv

all: compile_shaders

compile_shaders:
	mkdir -p shaders/compiled
	$(foreach file, $(wildcard shaders/*.vert), glslc $(file) -o $(file:shaders/%=shaders/compiled/%).spv;)
	$(foreach file, $(wildcard shaders/*.frag), glslc $(file) -o $(file:shaders/%=shaders/compiled/%).spv;)

.PHONY: test clean clean_shaders

test: a.out
	./a.out

clean:
	rm -f a.out

clean_shaders:
	rm -rf shaders/compiled