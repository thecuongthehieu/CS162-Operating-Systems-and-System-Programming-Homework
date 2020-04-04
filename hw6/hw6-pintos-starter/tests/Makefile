TESTS = examples threads p1 userprog p2 vm p3 filesys p4

PATH := $(shell pwd)/../src/utils:$(PATH)

all:
	@echo "This Makefile has only \`check' targets."

check:
	$(MAKE) -C .. distclean
	for d in $(TESTS); do $(MAKE) $$d || exit 1; done
	@echo All tests passed.

clean:
	rm -rf $(TESTS)

define mk-sandbox
rm -rf $@ && mkdir $@ && cp -R ../src $@/src
cd $@/src && $(MAKE) -s clean
endef

define run-tests
cd $@/src/$(PROJECT) && time $(MAKE) check
endef

define grade-tests
cd $@/src/$(PROJECT) && $(MAKE) grade
cd $@/src/$(PROJECT) && grep -q PERFECT build/grade
cd $@/src/$(PROJECT) && grep -qv warning build/grade
endef

define compile
cd $@/src/$(PROJECT) && $(MAKE)
endef

define test-clean
cd $@/src/$(PROJECT)/build && $(MAKE) clean
cd $@/src/$(PROJECT)/build && set `find -type f` > /dev/null && test $$# = 1
endef

define clean
rm -rf $@
endef

define apply-patch
(cd $@ && patch -p0) < ../solutions/$@.patch
endef

PROJECT = $@
p1: PROJECT = threads
p2: PROJECT = userprog
p3: PROJECT = vm
p4: PROJECT = filesys

# Tests that just compile the provided code.
threads::
	$(mk-sandbox)
	$(compile)
	$(run-tests) TESTS=tests/threads/alarm-single
	$(test-clean)
	$(clean)
userprog vm filesys::
	$(mk-sandbox)
	$(compile)
	$(test-clean)
	$(clean)
examples::
	$(mk-sandbox)
	$(compile)
	$(clean)

# Tests for the reference solutions.
p1 p2 p3 p4::
	$(mk-sandbox)
	$(apply-patch)
	$(run-tests)
	$(grade-tests)
	$(test-clean)
	$(clean)
