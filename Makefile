CC = /opt/emsdk-portable/emscripten/1.37.28/em++
SRV = /opt/emsdk-portable/emscripten/1.37.28/emrun
SOURCE = /opt/emsdk-portable/emsdk_env.sh
all: index.cpp
	$(CC) index.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -o index.js

run:
	$(SRV) .

clean:
	rm -rf index.data index.js.mem index.js
