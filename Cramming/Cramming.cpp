#include "Definition.h"


int main()
{
	start_settings();

file_selection:
	clear_screen();

	free_up_memory();

	file_open();

	tab_len = new size_t[max_colum] {};
	for (unsigned int i = 0; i < max_colum; i++)
	{
		for (unsigned int j = 0; j < table.size(); j++)
		{
			if (table[j].second.size() - 1 > i) {
				tab_len[i] = (std::max)(tab_len[i], table[j].second[i].size() / 8 + 1);
			}
		}
		line_tab_len += tab_len[i];
	}


	std::cout << "Нажмите любую клавишу чтобы начать\n";
	_getch();

	out_table();



	while (ready_word < int(table.size())) {
		set_random_line();

	one_more_time:
		clear_screen();

		std::cout << "\\S-сохр \\L-табл \\F-знаю \\X-файл !-пропуск\t" + open_path;
		out_void(size_consol.second / 2);
		out_in_center(size_consol.first, *(table[now_line].second.end() - 2));
		out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 10));


		bool result = 1;
		std::vector<std::string> ans = get_ans(table[now_line].second.size() - 2);

		if (ans[0][0] == '\\') {
			switch (ans[0][1])
			{
			case 'S':
			case 's':
				save();
				goto one_more_time;
				break;
			case 'L':
			case 'l':
				out_table();
				goto one_more_time;
				break;
			case 'F':
			case 'f':
				table[now_line].first[2]++;
				if (table[now_line].first[0] < finish_level - 1) {
					table[now_line].first[0] = finish_level - 1;
				}
				else {
					table[now_line].first[0]++;
					ready_word++;
				}
				continue;
				break;
			case 'X':
			case 'x':
				goto file_selection;
				break;
			default:
				break;
			}
		}
		if (ans[0][0] == '!' || ans.size() != table[now_line].second.size() - 2) {
			result = 0;
		}
		else {
			for (unsigned int i = 0; i < table[now_line].second.size() - 2; i++)
			{
				if (ans[i] != table[now_line].second[i]) {
					result = 0;
					break;
				}
			}
		}

		table[now_line].first[2]++;

		clear_screen();
		if (result) {
			table[now_line].first[0]++;
			table[now_line].first[1]++;

			out_void(size_consol.second / 2 - 1);
			out_in_center(size_consol.first, "Верно");
			std::cout << "\n\n";

			out_line(size_consol.first, now_line);

			out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 11));
			get_any_key();
		}
		else {
			int pred_level = table[now_line].first[0]--;
			table[now_line].first[0] = max(table[now_line].first[0], 0);

			while (result == 0) {
				clear_screen();

				std::cout << "!-опечатка";
				out_void(size_consol.second / 2 - 2);
				out_in_center(size_consol.first, "НЕПРАВИЛЬНО");
				std::cout << "\n\n";

				std::string united_ans;
				for (unsigned int i = 0; i < ans.size(); i++)
				{
					united_ans += ans[i] + "  ";
				}
				united_ans.pop_back(); united_ans.pop_back();

				out_line(size_consol.first - (4 + united_ans.size()), now_line);
				std::cout << "  (" << united_ans << ")\n\n";

				out_in_center(size_consol.first, "Попробуй ещё");
				out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 12));

				std::vector<std::string> ans = get_ans(table[now_line].second.size() - 2);
				result = 1;

				if (ans[0][0] == '!') {
					table[now_line].first[0] = pred_level;
					table[now_line].first[2]--;
					goto one_more_time;
				}

				if (ans.size() != table[now_line].second.size() - 2) {
					result = 0;
				}
				else {
					for (unsigned int i = 0; i < table[now_line].second.size() - 2; i++)
					{
						if (ans[i] != table[now_line].second[i]) {
							result = 0;
							break;
						}
					}
				}
			}
		}

		ready_word += table[now_line].first[0] >= finish_level;    // напоминание про режим без повторений
	}

	clear_screen();

	out_void(size_consol.second / 2);
	out_in_center(size_consol.first, "ВСЕ СЛОВА ВЫУЧЕНЫ!!!");
	out_void((std::max)(2, size_consol.second - size_consol.second / 2 - 10));
	get_any_key();

	save();

	out_table();


	goto file_selection;

return 0;
}