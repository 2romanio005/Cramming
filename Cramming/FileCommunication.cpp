#include "Definition.h"

std::string	get_path(std::string extension) {
	std::string path;
	
	std::cout << "Укажите путь к файлу\n";

	while (path.size() == 0) {
		std::getline(std::cin, path);
		erese_left_void(path);
	}
	erese_right_void(path);
	
	if (path[0] == '\"') { path.erase(0, 1); }
	if (path[path.size() - 1] == '\"') { path.erase(path.size() - 1, 1); }
	erese_left_void(path);
	erese_right_void(path);
	if (path.find(extension) == std::string::npos) {
		path += extension;
	}
	return path;
}

void next_path_to_write(std::string& path) {
	int iter = (path.size() - 5);
	while (iter >= 0 && path[iter] >= '0' && path[iter] <= '9') {
		iter--;
	}
	iter++;
	
	int value = 0;
	for (unsigned int i = iter; i < path.size() - 4; i++)
	{
		value = value * 10 + path[i] - '0';
	}
	value++;

	iter -= (path[iter - 1] == ' ');

	path.replace(path.begin() + iter, path.end() - 4, ' ' + std::to_string(value));
}

void file_open() {
	std::ifstream fin;
	std::cout << "В save найдено:\n";
	system("dir save /B /D /O:-D /A:-D");
	std::cout << '\n';

	while (true)
	{		
		fin.close();

		open_path = get_path(".txt");
		fin.open(open_path);
		if (fin.is_open()) {
			break;
		}

		fin.close();
		open_path = "save\\" + open_path;
		fin.open(open_path);
		if (fin.is_open()) {
			std::cout << "Файл найден в папке save\n";
			break;
		}

		std::cout << "Файл для чтения не найден\n\n";
	}
	fin.close();

	file_read(open_path);
}
void file_read(const std::string& path) {
	std::cout << "\nЧтение файла...\n\n";

	std::ifstream fin;
	fin.open(path);

	while (!fin.eof())
	{
		fin.get(readed_char);
		switch (readed_char)
		{
		case'@':
			fin >> finish_level;
			break;
		case'#':
			fin.get(separator);
			break;
		case'\n':
		case'\t':
		case' ':
			break;
		default:
			goto read_words;
		}
	}
read_words:

	if (separator == '\0') {
		std::cout << "Введите разделитель\n";
		separator = getchar();
	}
	if (finish_level == -1) {
		std::cout << "Введите финальное количество правильных ответов\n";
		std::cin >> finish_level;
	}

	int now_level = finish_level == 0;
	bool flag_ready = now_level >= finish_level;;
	int min_level = INT_MAX;
	std::string s;
	while (!fin.eof()) {
		while (readed_char == ':') {
			fin >> now_level;
			flag_ready = now_level >= finish_level;
			min_level = min(min_level, now_level);
			
			fin.get(readed_char);
			while (!fin.eof() && check_void(readed_char)) {
				fin.get(readed_char);
			}
		}

		s.clear();
		while (!fin.eof() && s.size() == 0) {
			std::getline(fin, s);
		}

		if (fin.eof()) { break; }

		table.push_back({ new int[3]{now_level, 0, 0}, std::vector<std::string>(1, {readed_char}) });
		auto* now = &(table.end() - 1)->second;
		for (unsigned int i = 0; i < s.size(); i++)
		{
			if (s[i] == separator) {
				erese_left_void(*(now->end() - 1));
				erese_right_void(*(now->end() - 1));
				if ((now->end() - 1)->size() > 0) {
					now->push_back("");
				}
			}
			else {
				*(now->end() - 1) += s[i];
			}
		}
		if ( *((now->end() - 1)->end() - 1) == ':') {
			(now->end() - 1)->pop_back();
			(table.end() - 1)->first[2] = std::stoi(*(now->end() - 1));
			now->pop_back();
			(table.end() - 1)->first[1] = std::stoi(*(now->end() - 1));
			now->pop_back();
		}
		if ((*(now->end() - 1))[0] != '-') {
			now->push_back("");
		}

		now->shrink_to_fit();

		max_colum = (std::max)(max_colum, now->size() - 1);
		
		ready_word += flag_ready;

		fin.get(readed_char);
		while (!fin.eof() && check_void(readed_char)) {
			fin.get(readed_char);
		}
	}
	fin.close();

	bool flag_change_finish_level = 0;
	if (finish_level != 0 && ready_word == table.size()) {
		flag_change_finish_level = 1;

		std::cout << "Все слова уже выучены, текущее наименьшее количество верных ответов у слова: " << min_level << "\nВведите новое финальное количество правильных ответов\n";
		std::cin >> finish_level;

		ready_word = 0;
		for (std::pair<int*, std::vector<std::string>>& el : table) {
			ready_word += el.first[0] >= finish_level;
		}
	}

	if (finish_level == 0) {
		std::cout << "Включён режим без повторений!\n";
		ready_word = 0;                    // как только слово встретится оно будет посчитано верным даже при неверном ответе
		if (flag_change_finish_level) {
			for (std::pair<int*, std::vector<std::string>>& el : table) {
				el.first[0] = 1;
			}
		}
		else {
			for (std::pair<int*, std::vector<std::string>>& el : table) {
				ready_word += el.first[0] != 1;
			}
		}
	}

	std::cout << "\nФайл считан\n";
}

void save() {
	sort_table();

	next_path_to_write(open_path);

	{
		std::ifstream fin(open_path);
		while (fin.is_open()){
			fin.close();

			clear_screen();
			out_void(size_consol.second / 2);
			out_in_center(size_consol.first, "Файл \"" + open_path + "\" уже существует.");
			std::cout << '\n';
			out_in_center(size_consol.first, "Заменить его? Y/N");
			out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 11));

			char ans = getchar();
			while (check_void(ans)) {
				ans = getchar();
			}
			if (ans == 'y' || ans == 'Y') {
				break;
			}

			next_path_to_write(open_path);
			fin.open(open_path);
		} 
		fin.close();

		std::ofstream fout;
		fout.open(open_path);

		fout << '#' << separator << '\n';
		fout << '@' << finish_level << '\n';

		int now_level = -1;
		for (auto& line : table)
		{
			if (line.first[0] != now_level) {
				now_level = line.first[0];
				fout << ':' << line.first[0] << '\n';
			}
			
			fout << '\t';
			for (unsigned int i = 0; i < line.second.size(); i++)
			{
				if (line.second[i].size() > 0) {
					fout << line.second[i] << separator;
				}
			}
			fout << line.first[1] << separator << line.first[2] << ":\n";
		}

		fout.close();
	}

	clear_screen();
	out_void(size_consol.second / 2);
	out_in_center(size_consol.first, "Файл сохранён под именем \"" + open_path + '\"');

	out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 10));
	get_any_key();
}


bool check_void(char checked) {
	return checked == '\t' || checked == ' ' || checked == '\n';
}