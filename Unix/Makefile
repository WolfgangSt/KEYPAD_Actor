ifeq ($(origin SOLUTION_DIR), undefined)
	SOLUTION_DIR := $(shell pwd)/
endif
PROJECT_DIR_KEYPADACTOR := $(dir $(lastword $(MAKEFILE_LIST)))
clean::
	rm -f libKEYPAD_Actor.so*
	rm -f libKEYPAD_Actor

libKEYPAD_Actor :
	cd $(PROJECT_DIR_KEYPADACTOR)/../source && make
	cp $(PROJECT_DIR_KEYPADACTOR)/../source/libKEYPAD_Actor.so* .
	cd $(PROJECT_DIR_KEYPADACTOR)/../source && make -i clean
	touch libKEYPAD_Actor

