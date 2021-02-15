.DEFAULT_GOAL := release

### Vars
APPLICATION := uwf
COMMIT_SHA=$(shell git rev-parse --short HEAD)
VERSION ?= $(COMMIT_SHA)
BUILD_DIR=release-${VERSION}
INCLUDES=-lImlib2 -lX11
SRCS=main.c utils.c handlers.c icons.c window.c launcher.c error.c background.c manager.c
###

.PHONY: linux-amd64
linux-amd64:
	gcc ${SRCS} ${INCLUDES} -o ${APPLICATION}

.PHONY: setup
setup: clean-rel
	@mkdir -p $(BUILD_DIR)

.PHONY: release
release: setup linux-amd64

.PHONY: clean-rel
clean:
	@echo "Cleaning"
	@rm -rf $(BUILD_DIR)