#
# TODO: Add doxygen compilation + testing
#

CXX= g++  # This is the main compiler
# CXX = clang --analyze # and comment out the linker last line for sanity

SRCEXT= cpp

# definition of the directory for the source files
SRCDIR= ../src
TSRCDIR= ../tests
vpath %.cpp $(SRCDIR) $(TSRCDIR)


# definition of the source files
SOURCES=  $(shell find $(SRCDIR)  -type f -name *.$(SRCEXT) -and -not -name $(TARGET).$(SRCEXT))
TSOURCES= $(shell find $(TSRCDIR) -type f -name *.$(SRCEXT))

# definition of the object files
BUILDDIR= build
OBJECTS= $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
TOBJECTS= $(patsubst $(TSRCDIR)/%, $(BUILDDIR)/%, $(TSOURCES:.$(SRCEXT)=.o))

# definition of the binaries
TARGET= main
TTARGET= 

# definition of the directories for the include


# definition of the directories for the libraries


# compilation options
CXXFLAGS= -g -W -Wall -std=c++11 -O3 -pthread 
LDFLAGS=
LIBS= -lboost_system -lboost_filesystem -lboost_program_options
INC=  -I $(SRCDIR)

# for OpenCV compilation
CFLAGS+= `pkg-config --cflags opencv` #openCV include flags
LIBS+= `pkg-config --libs opencv`     #openCV libs


#order-only preresquisit: any $(OBJECTS) must be built then
#$(BUILDDIR) must be built first, but if $(BUILDDIR) is out of date
#(or doesn't exist), that does not force $(OBJECTS) to be built
#$(OBJECTS): | $(BUILDDIR)
#	@echo "objects:  " $(OBJECTS)
#	@echo "tobjects: " $(TOBJECTS)

$(BUILDDIR)/%.o : %.cpp
	@mkdir -p $(BUILDDIR)
	@echo $<
	@$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

main: $(OBJECTS) $(BUILDDIR)/main.o
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIBS)"
	$(CXX) $^ -o $(TARGET) $(LIBS)

test_%: $(OBJECTS) $(BUILDDIR)/test_%.o
	@echo Linking tests...
	@echo " $(CXX) $(LDFLAGS) $^ -o $(TTARGET) $(LIBS)"
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

tests: $(TTARGET)

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET) $(TTARGET)"
	$(RM) -r $(BUILDDIR) $(TARGET) $(TTARGET)

# targets not explicitly associated with files
.PHONY: clean tests


#NAME		=	speedy
#
#CXX		=	g++
#
#RM		=	rm -f
#
#PATH		=	src/
#
#SRC		=	$(PATH)car.cpp	\
#			$(PATH)fileManager.cpp	\
#			$(PATH)error.cpp	\
#			$(PATH)main.cpp
#
#OBJ		=	$(SRC:.cpp=.o)
#
#INC		+=	-I ./inc/
#
#CXXFLAGS	+=	-W -Wall -std=c++11 $(INC)
#
#all		:	$(NAME)
#
#$(NAME)		:	$(OBJ)
#			$(CXX) -o $(NAME) $(OBJ)
#
#clean		:
#			$(RM) $(OBJ)
#
#fclean		:	clean
#			$(RM) $(NAME)
#
#re		:	fclean all
#
#.PHONY		:	all clean fclean re
