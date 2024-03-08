#include "Definition.h"



void start_settings() {
	//_setmode(_fileno(stdout), _O_U16TEXT);
	//_setmode(_fileno(stdin), _O_U16TEXT);
	//_setmode(_fileno(stderr), _O_U16TEXT);
	SetConsoleCP(CODR_PAGE);
	SetConsoleOutputCP(CODR_PAGE);
	srand(unsigned int(time(NULL)));

	hWndCon = GetStdHandle(STD_OUTPUT_HANDLE);

	atexit(free_up_memory);  // функция вызываемая после завешнения программы
}

void free_up_memory() {
	for (auto& el : table) {
		delete[] el.first;
		el.first = nullptr;
	}
	table.clear();

	delete[] tab_len;
	tab_len = nullptr;

	line_tab_len = 0;
	max_colum = 0;
	ready_word = 0;
	finish_level = -1;
	separator = '\0';
}


void sort_table() {   // !!!!!!!!!!!!!!!! не знаю как улучшить
	std::pair<int*, std::vector<std::string>> find_line = table[now_line];

	bool (*pred)(std::pair<int*, std::vector<std::string>> one, std::pair<int*, std::vector<std::string>> two) = [](std::pair<int*, std::vector<std::string>> one, std::pair<int*, std::vector<std::string>> two) {
		if (one.first[0] == two.first[0]) {
			return one.second < two.second;
		}
		return one.first[0] < two.first[0];
	};

	std::sort(table.begin(), table.end(), pred);

	now_line = std::lower_bound(table.begin(), table.end(), find_line, pred) - table.begin();  // !!!!!!!!!!!!!
}

void set_random_line() {
	if (finish_level == 0) {
		do
		{
			now_line = rand() % table.size();
		} while (table[now_line].first[0] != 1);
		return;
	}

	int pred = now_line;
	do
	{
		now_line = rand() % table.size();
	} while ((now_line == pred && ready_word < int(table.size() - 1)) || (rand() % finish_level) < table[now_line].first[0]);
}


void erese_left_void(std::string& inp) {
	unsigned int iter = 0;
	while (iter < inp.size() && check_void(inp[iter])) { iter++; }
	inp.erase(inp.begin(), inp.begin() + iter);
}

void erese_right_void(std::string& inp) {
	int iter = inp.size() - 1;
	while (iter >= 0 && check_void(inp[iter])) { iter--; }
	inp.erase(inp.begin() + (iter + 1), inp.end());
}


void clear_screen() {
	system("cls");
	get_size_consol();
}

void get_size_consol() {
	CONSOLE_SCREEN_BUFFER_INFO csbiData;

	GetConsoleScreenBufferInfo(hWndCon, &csbiData);

	size_consol = { csbiData.dwSize.X, csbiData.srWindow.Bottom + 1 };   // ширина, высота
}


void out_in_left(int len, const std::string out) {
	std::cout << out;
	for (int i = out.size(); i < len; i++)
	{
		std::cout << ' ';
	}
}

void out_in_right(int len, const std::string out) {
	for (int i = out.size(); i < len; i++)
	{
		std::cout << ' ';
	}
	std::cout << out;
}

void out_in_center(int len, const std::string out) {
	int tmp = (len - int(out.size())) / 2;
	tmp = max(tmp, 0);
	for (int i = 0; i < tmp; i++)
	{
		std::cout << ' ';
	}
	out_in_left(len - tmp, out);
}


void out_line(int len, int index_line) {
	
	for (int i = 0; i < (len - (line_tab_len * 8 + 6)) / 2; i++)
	{
		std::cout << ' ';
	}

	int yes = 0;
	for (unsigned int i = 0; i < table[index_line].second.size() - 2; i++)
	{
		out_in_left(tab_len[i] * 8, table[index_line].second[i]);
		yes += tab_len[i];
	}
	yes = min(yes, line_tab_len);
	out_in_right((line_tab_len - yes) * 8, *(table[index_line].second.end() - 2));

	std::cout << ' ' << table[index_line].first[0] << ' ' << table[index_line].first[1] << '/' << table[index_line].first[2] << '\t' << *(table[index_line].second.end() - 1);
}

void out_table() {
	sort_table();

	int start = 0;
	int k1, k2;
	while (start < int(table.size())) {
		clear_screen();
		get_size_consol();

		int fin = min(start + size_consol.second - 3, int(table.size()));
		for (int i = start; i < fin; i++)
		{
			out_line(size_consol.first, i);
			std::cout << '\n';
		}

		if (fin == table.size()) {
			out_in_right(line_tab_len * 8 + (size_consol.first - line_tab_len * 8) / 2 + 4, "До " + std::to_string(finish_level));
		}
		std::cout << "\n\n";

		out_in_center(size_consol.first, "Используйте клавиши управления курсором для перемещения по списку");

	one_more_click:
		k1 = _getch();
		if (k1 == 0x20 || k1 == 0x0D) {
			start = fin;
		}
		else if (k1 == 0x08) {
			start = (std::max)(start - size_consol.second + 3, 0);
		}
		else if (k1 == 0xE0 || k1 == 0x00)
		{
			k2 = _getch();
			switch (k2)
			{
			case 0x48: // верх
			case 0x4B: // лево
				start = (std::max)(start - size_consol.second + 3, 0);
				break;
			case 0x50: // низ
			case 0x4D: // право
				start = fin;
				break;
			case 0x6B: // f4
				if (k1 == 0x00) exit(0); // проверка на всякий случай
			default:
				goto one_more_click;
				break;
			}
		}	
		else {
			goto one_more_click;
		}
	}

	/*
	while (iter_line < int(table.size())) {
		clear_screen();

		int fin = min(iter_line + size_consol.second - 2, int(table.size()));
		for (; iter_line < fin; iter_line++)
		{
			out_line(size_consol.first, iter_line);
			std::cout << '\n';
		}

		if (iter_line == table.size()) {
			out_in_right(line_tab_len * 8 + (size_consol.first - line_tab_len * 8) / 2 + 4, "До " + std::to_string(finish_level));
		}
		std::cout << '\n';

		get_any_key();
	}
	*/
}


void out_void(int namber_line) {
	for (int i = 0; i < namber_line; i++)
	{
		std::cout << '\n';
	}
}


void get_any_key() {
	out_in_center(size_consol.first, "Нажмите любую клавишу для продолжения");
	_getch();
}

std::vector<std::string> get_ans(int size) {
	std::string inp;
	std::vector<std::string> ans(1, "");

	while (int(ans.size()) <= size) {
		inp.clear();
		while (inp.size() == 0) {
			std::getline(std::cin, inp);
			erese_left_void(inp);
		}

		erese_right_void(inp);

		
		if (inp[0] == '!') {
			return std::vector<std::string>(1, {'!'});
		}
		if (inp[0] == '\\') {
			return std::vector<std::string>(1, { '\\', inp[1] });
		}

		for (unsigned int i = 0; i < inp.size(); i++)
		{
			if (inp[i] == separator) {
				erese_left_void(*(ans.end() - 1));
				erese_right_void(*(ans.end() - 1));
				ans.push_back("");
			}
			else {
				*(ans.end() - 1) += inp[i];
			}
		}

		erese_left_void(*(ans.end() - 1));
		erese_right_void(*(ans.end() - 1));
		ans.push_back("");
	}
	ans.pop_back();
	return ans;
}


