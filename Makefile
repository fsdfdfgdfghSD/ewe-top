# Directories

src_dir = ./src
build_dir = ./build

include_dir = $(src_dir)/include
ui_dir = $(include_dir)/ui

# List of *.c files to compile
error = $(include_dir)/error.c
top = $(include_dir)/top.c
ui = $(ui_dir)/ui.c

# Outputing files
main = $(src_dir)/main.c
out_main = $(build_dir)/main

# Main
all: run

run:
	gcc -Iinclude $(main) $(error) $(top) -o $(out_main) -lprocps -lncurses
	$(out_main)

clean:
	rm -rf $(build_dir)/*
