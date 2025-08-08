#*******************************  VARIABLES  **********************************#

TEST_MODE		=	0

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME			=	libft_malloc_$(HOSTTYPE).so
TEST_NAME		=	tests_bin

# --------------- FILES --------------- #

LIST_SRC		=	\
					malloc.c


LIST_TEST_SRC	=	\
					main.c		\
					malloc.c

# ------------ DIRECTORIES ------------ #

DIR_BUILD		=	.build/
DIR_SRC 		=	src/
DIR_INCLUDE		=	include/
DIR_TEST		=	test/
DIR_TEST_MALLOC	=	$(DIR_TEST)malloc/
DIR_LIB			=	lib/

# ------------- SHORTCUTS ------------- #

OBJ				=	$(patsubst %.c, $(DIR_BUILD)%.o, $(SRC))
DEP				=	$(patsubst %.c, $(DIR_BUILD)%.d, $(SRC))
SRC				=	$(addprefix $(DIR_SRC), $(LIST_SRC))
TEST_SRC		=	$(addprefix $(DIR_TEST), $(LIST_TEST_SRC))
TEST_MALLOC_SRC	=	$(addprefix $(DIR_TEST_MALLOC), $(LIST_SRC))
TEST_DEP		=	$(patsubst %.c, $(DIR_BUILD)%.d, $(TEST_SRC) $(TEST_MALLOC_SRC))
TEST_OBJ		=	$(patsubst %.c, $(DIR_BUILD)%.o, $(TEST_SRC) $(TEST_MALLOC_SRC))

# ------------ COMPILATION ------------ #

CFLAGS			=	-Wall -Wextra -Werror

ifeq ($(TEST_MODE),1)
    CFLAGS += -DTEST_MODE
endif

DEP_FLAGS		=	-MMD -MP

# -------------  COMMANDS ------------- #

RM				=	rm -rf
MKDIR			=	mkdir -p

#***********************************  RULES  **********************************#


.PHONY: all
all:			$(NAME)

.PHONY: tests
tests:
				$(MAKE) $(TEST_NAME) TEST_MODE=1
				valgrind ./$(TEST_NAME)

$(TEST_NAME):	$(TEST_OBJ)
				$(CC) $(CFLAGS) $(TEST_OBJ) -o $(TEST_NAME)

# ---------- VARIABLES RULES ---------- #

$(NAME):		$(OBJ)
				$(CC) -shared $(OBJ) -o $(NAME)

# ---------- COMPILED RULES ----------- #

-include $(DEP)
$(DIR_BUILD)%.o: %.c
				mkdir -p $(shell dirname $@)
				$(CC) $(CFLAGS) -fPIC $(DEP_FLAGS) -c $< -o $@ -I $(DIR_INCLUDE)

-include $(TEST_DEP)
$(DIR_BUILD)$(DIR_TEST)%.o: $(DIR_TEST)%.c
				mkdir -p $(shell dirname $@)
				$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@ -I $(DIR_INCLUDE) -I $(DIR_LIB)

$(DIR_BUILD)$(DIR_TEST_MALLOC)%.o: $(DIR_SRC)%.c
				mkdir -p $(shell dirname $@)
				$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@ -I $(DIR_INCLUDE) -I $(DIR_LIB)

.PHONY: clean
clean:
				$(RM) $(DIR_BUILD)

.PHONY: fclean
fclean: clean
				$(RM) $(NAME)
				$(RM) $(TEST_NAME)

.PHONY: re
re:				fclean
				$(MAKE) all

.PHONY: check-format
check-format:
				clang-format -style=file $(TEST_SRC) $(SRC) -n --Werror

.PHONY: format
format:
				clang-format -style=file $(TEST_SRC) $(SRC) -i
