spinningcube_withlight: spinningcube_withlight.cpp textfile.c
	gcc $^ -lGL -lGLEW -lglfw -lm -o $@

run: spinningcube_withlight
	./spinningcube_withlight

clean:
	rm -f *.o *~

cleanall: clean
	rm -f spinningcube_withlight
