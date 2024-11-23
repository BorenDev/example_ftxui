
CONTAINER_CMD = podman run -it --rm --name=build_container --mount type=bind,source=${PWD},target=/workdir arch_mplabx:latest bash
SHELL_CMD = podman run -it --rm --name=shell --mount type=bind,source=${PWD},target=/workdir arch_mplabx:latest bash

all: \
	template_cmake

template_cmake:
	cmake --workflow --preset template_cmake

clean:
	rm -rf build/
