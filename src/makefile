# mirra
# Copyright (C) 2016 David Jolly
# ----------------------
#
# mirra is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# mirra is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_ENG=./../include/engine/
DIR_INC_GFX=./../include/graphics/
DIR_SRC=./
DIR_SRC_ENG=./engine/
DIR_SRC_GFX=./graphics/
LIB=libmirra.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)mirra_cpu.o $(DIR_BUILD)mirra_display.o $(DIR_BUILD)mirra_exception.o $(DIR_BUILD)mirra_input.o \
		$(DIR_BUILD)mirra_object.o $(DIR_BUILD)mirra_runtime.o $(DIR_BUILD)mirra_signal.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base

### BASE ###

build_base: mirra_cpu.o mirra_display.o mirra_exception.o mirra_input.o mirra_object.o mirra_runtime.o mirra_signal.o

mirra_cpu.o: $(DIR_SRC)mirra_cpu.cpp $(DIR_INC)mirra_cpu.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_cpu.cpp -o $(DIR_BUILD)mirra_cpu.o

mirra_display.o: $(DIR_SRC)mirra_display.cpp $(DIR_INC)mirra_display.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_display.cpp -o $(DIR_BUILD)mirra_display.o

mirra_exception.o: $(DIR_SRC)mirra_exception.cpp $(DIR_INC)mirra_exception.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_exception.cpp -o $(DIR_BUILD)mirra_exception.o

mirra_input.o: $(DIR_SRC)mirra_input.cpp $(DIR_INC)mirra_input.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_input.cpp -o $(DIR_BUILD)mirra_input.o

mirra_object.o: $(DIR_SRC)mirra_object.cpp $(DIR_INC)mirra_object.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_object.cpp -o $(DIR_BUILD)mirra_object.o

mirra_runtime.o: $(DIR_SRC)mirra_runtime.cpp $(DIR_INC)mirra_runtime.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_runtime.cpp -o $(DIR_BUILD)mirra_runtime.o

mirra_signal.o: $(DIR_SRC)mirra_signal.cpp $(DIR_INC)mirra_signal.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_LVL) -c $(DIR_SRC)mirra_signal.cpp -o $(DIR_BUILD)mirra_signal.o
