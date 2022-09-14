PROJNAME = Project
TARGET   = $(PROJNAME)
CPPC  	 = g++
ASMC 	 = as
ASMFLAGS = -O0
CXXFLAGS =  -c \
			-pedantic \
			-Wall \
			-Wextra \
			-masm=intel \


CVERSION   = c11
CXXVERSION = c++17




# -pedantic \
# -Werror \
# -Wno-missing-braces \
# -Wno-missing-field-initializers \
# -Wno-unused-function \
# -Wno-unused-variable \
# -march=native \
# -masm=intel \
# -msse4.1 \
# -msse4.2 \
# -fif-conversion \
# -fif-conversion2 \


LIB_INC_PATHS = -I/usr/local/include -I/usr/include
LIB_PATHS     = -L/usr/local/lib


LIB_FILES = -lglfw


LDFLAGS = \
		$(LIB_FILES) \
		-static-libgcc \
		-static-libstdc++ \
		-lGL \
		-lXinerama \
		-lXcursor \
		-lrt \
		-lm \
		-ldl \
		-lXi \
		-lX11 \
		-lpthread \
		-lxcb \
		-lXau \
		-lXdmcp \
		-lXxf86vm \
		-lXrandr \


BUILDDIR  := build
LIBDIR    := deps
ASSETDIR  := assets

SRCDIR    := src
OBJDIR    := $(BUILDDIR)/obj
OUTPUTDIR := $(BUILDDIR)/bin



rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


CSRC    = $(call rwildcard,$(SRCDIR),*.c)
CPPSRC += $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC  = $(call rwildcard,$(SRCDIR),*.asm)


OBJS  = $(patsubst $(SRCDIR)/%.asm,$(OBJDIR)/%_asm.o,$(ASMSRC))
OBJS += $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_c.o,$(CSRC))
OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%_cpp.o,$(CPPSRC))




# Compile All C, C++, ASM Files that are part of the source directory
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo compiling ASM File $^ ...
	@ mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -o $@


$(OBJDIR)/%_c.o: $(SRCDIR)/%.c
	@ echo compiling C File $^ ...
	@ mkdir -p $(@D)
	$(CPPC) -std=$(CVERSION) $(CXXFLAGS) $(LIB_INC_PATHS) $^ -o $@ 



$(OBJDIR)/%_cpp.o: $(SRCDIR)/%.cpp
	@ echo compiling CPP File $^ ...
	@ mkdir -p $(@D)
	$(CPPC) -std=$(CXXVERSION) $(CXXFLAGS) $(LIB_INC_PATHS) $^ -o $@




link:
	@ echo Linking ...
	$(CPPC) $(LIB_PATHS) $(OBJS) -o $(OUTPUTDIR)/$(TARGET) $(LDFLAGS) 

# $(CPPC) -o $(OUTPUTDIR)/$(TARGET) $(LIB_PATHS) $(LDFLAGS) $(OBJS)
# $(CPPC) $(LIB_PATHS) $(LDFLAGS) -o $(OUTPUTDIR)/$(TARGET) $(OBJS)





bld: $(OBJS) link


rel: CXXFLAGS += -O3
rel: bld

debug: CXXFLAGS +=-g -O1 -D _DEBUG
debug: bld


nocons: LDFLAGS += -mwindows
nocons: rel


# Don't Forget to Pipe the Output to a text file! (==> make debug_compile &> debug_out.txt)
debug_compile: CXXFLAGS += --verbose -pipe
debug_compile: rel


profile: CXXFLAGS +=-pg -fprofile-arcs -ftest-coverage
profile: LDFLAGS +=-pg -g -fprofile-arcs -ftest-coverage
profile: debug




clean:
	@ echo -n Deleting Compiled Files ...
	-@ rm -r $(OBJDIR)/* > /dev/null || true
	@ echo  Done!


cleanbin:
	@ echo -n Deleting Project Executable ...
	-@ rm -r $(OUTPUTDIR)/$(TARGET) > /dev/null || true
	@ echo  Done!


cleanall: cleanshaders clean


cleanprof: allclean
cleanprof: 
	@ echo -n Deleting gcc Profile Data...
	-@ rm -r gmon.out > /dev/null || true
	@ echo -n Done!


run:
	@ echo Running Compiled Executable ...
	./$(OUTPUTDIR)/$(TARGET)

info:
	@ echo -e "File List: $(ASMSRC) $(CSRC) $(CPPSRC)\nObject List: $(OBJS)\n"