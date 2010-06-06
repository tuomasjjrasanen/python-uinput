dist:
	@mkdir -p include
	@cp lib/libsuinput/src/suinput.h include
	@cp lib/libsuinput/src/suinput.c src
	@python setup.py sdist

clean:
	@rm -rf src/suinput.c
	@rm -rf include
	@rm -rf dist
	@rm -rf build
	@rm -rf MANIFEST
