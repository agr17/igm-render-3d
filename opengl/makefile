spinningcube_withlight: spinningcube_withlight.cpp util/textfile.c
	gcc $^ -lGL -lGLEW -lglfw -lm -o $@

run: spinningcube_withlight
	./spinningcube_withlight

clean:
	rm -f spinningcube_withlight *.o *~
