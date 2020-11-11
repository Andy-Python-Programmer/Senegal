BUILD_DIR := build
OBJS:=$(wildcard *.o)

clean:
	@ rm -rf $(BUILD_DIR)
	@ rm -rf gen

profile:
	@ $(MAKE) -f senegal.make NAME=senegalp MODE=profile SOURCE_DIR=src
	@ cp build/senegal senegal

debug:
	@ $(MAKE) -f senegal.make NAME=senegald MODE=debug SOURCE_DIR=src
	@ cp build/senegal senegal

run:
	@ $(MAKE) -f senegal.make NAME=senegal MODE=release SOURCE_DIR=src
	@ cp build/senegal senegal
	@ ./senegal $(ARGS)

release:
	@ $(MAKE) -f senegal.make NAME=senegal MODE=release SOURCE_DIR=src
	@ cp build/senegal senegal
	@ cp build/senegal test/senegal