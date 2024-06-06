CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -g -std=c11 -fprofile-arcs -ftest-coverage #1 flag from them doesn't work in windows
TEST_FLAGS	=	-lcheck -lm -lpthread -g
GCOV_FLAGS	=	-lcheck -fprofile --coverage
MAKE		=	make
QMAKE		=	qmake
RM			=	rm -rf
MKDIR		=	mkdir -p
CLANG_FORMAT	=	clang-format -i -style=file
CLANG_FORMAT_CHECK	=	clang-format -n
GENHTML		=	genhtml
LCOV		=	lcov
TEXI2DVI	=	texi2dvi
TEXI2PDF	=	texi2pdf

#TEXI2DVI	=	/opt/homebrew/bin/texi2dvi
#TEXI2PDF	=	/opt/homebrew/bin/texi2pdf

PRO_FILE = 3DViewer.pro
PROGRAM_NAME = 3DViewer

SRC_DIR		=	src
OBJ_DIR		=	obj
INCLS_DIR	=	includes
BUILD_DIR	=	build
FORMS_DIR	=	forms
DVI_DIR		=	dvi
PDF_DIR		=	pdf
DIST_DIR	=	dist
TEST_DIR	=	test/
TEST_LIB_SRC	=	parser.c carcass.c get_next_line_bonus.c get_next_line_utils_bonus.c
TEST_LIB_OBJ	=	$(addprefix $(TEST_DIR),$(patsubst %.c,%.o,$(TEST_LIB_SRC)))
TEST_D_FILES	=	$(addprefix $(TEST_DIR),$(patsubst %.c,%.d,$(TEST_LIB_SRC) test.c))


TAR_FILE = $(addprefix $(PROGRAM_NAME), 1.0.tar.gz)
TEX_FILE = $(addprefix $(PROGRAM_NAME),.tex)
DVI_FILE = $(addprefix $(PROGRAM_NAME), .dvi)
PDF_FILE = $(addprefix $(PROGRAM_NAME), .pdf)

DIST_CONTENT	=	$(SRC_DIR) $(INCLS_DIR) $(FORMS_DIR) \
 					$(PRO_FILE) $(TEX_FILE) QtGifImage Makefile $(TEST_DIR)

DIST_DEPENDENCIES	=	$(TEX_FILE) $(SRC_DIR)/* $(INCLS_DIR)/* $(FORMS_DIR)/* $(PRO_FILE) \
						Makefile QtGifImage/* QtGifImage/*/* QtGifImage/*/*/*

WINDOWS_PATH =  C:/Users/$(USERNAME)/$(PROGRAM_NAME)
POSIX_PATH = $(HOME)/$(PROGRAM_NAME)

ifeq ($(OS),Windows_NT)
	TEST_LIB = $(addprefix test_,$(addprefix $(PROGRAM_NAME),.lib))
	TEST_EXEC = $(addprefix test_,$(addprefix $(PROGRAM_NAME),.exe))
	TARGET = $(addprefix $(PROGRAM_NAME),.exe)
	INSTALL_PATH = $(WINDOWS_PATH)
#	RM = rm -Force -Recurse
#	MKDIR = mkdir -Force
else
	TEST_LIB = $(addprefix test_,$(addprefix $(PROGRAM_NAME),.a))
	TEST_EXEC = $(addprefix test_,$(addprefix $(PROGRAM_NAME),.out))
	TARGET = $(addprefix $(PROGRAM_NAME),.app)
	INSTALL_PATH = $(POSIX_PATH)
#	RM = rm -rf
#	MKDIR = mkdir -p
endif

#ifeq ($(OS),Windows_NT)
#	SHELL := powershell.exe
#	.SHELLFLAGS := -NoProfile -Command
#endif

FULL_PROGRAM_NAME	= $(addprefix $(addprefix $(BUILD_DIR), /bin/), $(TARGET))

.PHONY:	all install rebuild compile dist dvi pdf tests

test/%.o: */%.c
		$(CC) $(CFLAGS) -I $(INCLS_DIR) -c $< -o $@ -MMD


all:	compile

rebuild: fclean compile


compile:
		$(MKDIR)  $(BUILD_DIR)
		cd $(BUILD_DIR); $(QMAKE) ../$(PRO_FILE); $(MAKE);
		$(RM)  $(BUILD_DIR)/debug $(BUILD_DIR)/release $(BUILD_DIR)/Makefile.Debug
		$(RM) $(BUILD_DIR)/obj/object_script.$(PROGRAM_NAME).Debug

install:
		$(RM) $(INSTALL_PATH)
		$(MKDIR)  $(INSTALL_PATH)
		cp -r $(FULL_PROGRAM_NAME) $(INSTALL_PATH)

uninstall:
		$(RM) $(INSTALL_PATH)


$(DIST_DIR)/$(TAR_FILE): test_clean $(TEX_FILE) $(SRC_DIR)/* $(INCLS_DIR)/* $(FORMS_DIR)/* $(PRO_FILE) \
						Makefile QtGifImage/* QtGifImage/*/* QtGifImage/*/*/* $(TEST_DIR)*.c
			$(RM) $(DIST_DIR)
			$(MKDIR) $(DIST_DIR)
			tar -czvf $(DIST_DIR)/$(TAR_FILE) $(DIST_CONTENT)


dist:	$(DIST_DIR)/$(TAR_FILE)

dvi:	$(DVI_DIR)/$(DVI_FILE)

$(DVI_DIR)/$(DVI_FILE):	$(TEX_FILE)
				$(MKDIR) $(DVI_DIR)
				cd $(DVI_DIR); $(TEXI2DVI) ../$(TEX_FILE)

$(PDF_DIR)/$(PDF_FILE):	$(TEX_FILE)
						$(MKDIR) $(PDF_DIR)
						cd $(PDF_DIR); $(TEXI2PDF) ../$(TEX_FILE)

pdf:	$(PDF_DIR)/$(PDF_FILE)

style:
#	cp ../materials/linters/.clang-format ./
	$(CLANG_FORMAT) ./$(SRC_DIR)/*.c
	$(CLANG_FORMAT) ./$(SRC_DIR)/*.cpp
	$(CLANG_FORMAT) ./$(INCLS_DIR)/*.h
	$(CLANG_FORMAT) ./$(TEST_DIR)*.c
	$(CLANG_FORMAT_CHECK) ./$(SRC_DIR)/*.c
	$(CLANG_FORMAT_CHECK) ./$(SRC_DIR)/*.cpp
	$(CLANG_FORMAT_CHECK) ./$(INCLS_DIR)/*.h
	$(CLANG_FORMAT_CHECK) ./$(TEST_DIR)*.c
#	rm .clang-format

tests: $(TEST_DIR)$(TEST_LIB) $(TEST_DIR)test.o
	cd $(TEST_DIR); \
	$(CC) $(CFLAGS) test.o $(TEST_LIB) $(TEST_FLAGS) $(GCOV_FLAGS) -o $(TEST_EXEC); \
	./$(TEST_EXEC)

$(TEST_DIR)$(TEST_LIB): $(TEST_LIB_OBJ)
	ar rcs $(TEST_DIR)$(TEST_LIB) $(TEST_LIB_OBJ)
	ranlib $(TEST_DIR)$(TEST_LIB)

gcov_report: test_clean tests
	lcov -t $(TEST_DIR)$(TEST_EXEC) -o $(TEST_DIR)gcov_report.info -c -d $(TEST_DIR)
	genhtml -o $(TEST_DIR)gcov_report $(TEST_DIR)gcov_report.info
	open $(TEST_DIR)gcov_report/index.html

test_clean:
		$(RM) $(TEST_LIB_OBJ) $(TEST_DIR)test.o $(TEST_DIR)$(TEST_EXEC) $(TEST_DIR)$(TEST_LIB)
		$(RM) $(TEST_D_FILES) $(TEST_DIR)gcov_report $(TEST_DIR)*.gcda $(TEST_DIR)*.gcno $(TEST_DIR)gcov_report.info

-include $(TEST_D_FILES)

clean:
		$(RM) $(BUILD_DIR)/moc/* $(BUILD_DIR)/obj/* $(BUILD_DIR)/uic/*
		$(MAKE) test_clean

fclean: clean
		$(RM) $(BUILD_DIR) $(DVI_DIR) $(PDF_DIR) $(DIST_DIR)
		$(RM) $(TEST_DIR)$(TEST_LIB)

re:	fclean all


