
CONTAINER_CMD = podman run -it --rm --name=build_container --mount type=bind,source=${PWD},target=/workdir arch_mplabx:latest bash
SHELL_CMD = podman run -it --rm --name=shell --mount type=bind,source=${PWD},target=/workdir arch_mplabx:latest bash

all: \
	example_ftxui

example_ftxui:
	cmake --workflow --preset example_ftxui

clean:
	rm -rf build/
