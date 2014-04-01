CFLAG = -I/usr/local/include -I./sdlgui -I./core -I./game_gui
LIBS = -L/usr/local/lib -lSDL2_image -lSDL2_image.dll -lSDl2_gfx -lSDL2_draw -lmingw32 -lSDL2_ttf -lSDL2_ttf.dll -lSDL2main -lSDL2 -lSDL2.dll 
ALL:
	rm ./bin/*.exe
	make test.exe
test.exe : main.cpp
	g++ main.cpp -o ./bin/test.exe $(CFLAG) $(LIBS)
	./bin/test
clip : clip.cpp sdlwindow.h
	g++ clip.cpp -o ./bin/clip.exe $(CFLAG) $(LIBS)
	./bin/clip
