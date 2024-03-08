#include "Definition.h"

int now_line = 0;

std::vector<std::pair<int*, std::vector<std::string>>> table;
size_t* tab_len = nullptr;
int line_tab_len = 0;
size_t max_colum = 0;
int ready_word = 0;

int finish_level = -1;
char separator = '\0';



std::pair<int, int> size_consol;
HANDLE hWndCon;

std::string open_path;




char readed_char;


