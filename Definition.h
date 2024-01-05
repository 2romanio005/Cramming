#pragma once

#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include <vector>
#include <string>

#include <conio.h>
#include <ctime>
#include <fstream>


#define CODR_PAGE 1251         // кодировка


extern int now_line;

extern std::vector<std::pair<int*, std::vector<std::string>>> table;
extern size_t* tab_len;
extern int line_tab_len;
extern size_t max_colum;
extern int ready_word;

extern int finish_level;
extern char separator;

extern std::pair<int, int> size_consol;   // ширина, высота
extern HANDLE hWndCon;

extern std::string open_path;


extern char readed_char;



void start_settings();
void free_up_memory();

void sort_table();

void set_random_line();

void erese_left_void(std::string& inp);
void erese_right_void(std::string& inp);

void clear_screen();
void get_size_consol();

void out_in_left(int len, const std::string out);
void out_in_right(int len, const std::string out);
void out_in_center(int len, const std::string out);

void out_line(int len, int index_line);
void out_table();

void out_void(int namber_line);

void get_any_key();
std::vector<std::string> get_ans(int size);

std::string	get_path(const std::string extension);
void next_path_to_write(std::string& old_path);

void file_open();
void file_read(const std::string& path);
void save(); // file_write

bool check_void(char checked);
















