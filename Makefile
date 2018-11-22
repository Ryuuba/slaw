INETDIR = /mnt/c/Users/Geo/inet

BUILD_OPTIONS = -f --deep -I$(INETDIR)/src/inet/common/geometry/common/ -I$(INETDIR)/src/inet/common/ -I$(INETDIR)/src/ -I$(INETDIR)/src/inet/mobility/base/ -I$(INETDIR)/src/inet/mobility/contract/ -I$(INETDIR)/src/inet/mobility/single/ -L$(INETDIR)/out/gcc-release/src/ -lINET -o slaw

all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake $(BUILD_OPTIONS)

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
