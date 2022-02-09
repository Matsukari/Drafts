#ifndef CONFIGURES_H
#define CONFIGURES_H

enum GAME_KEY_ACTION {
	KEY_CONFIRM, KEY_LEFT, KEY_RIGHT, KEY_TOP, KEY_BOTTOM, 
	KEY_BASICATTACK, KEY_LEFTRUN, KEY_RIGHTRUN, KEYJUMPUP, KEY_JUMPDOWN,
	KEY_TOTAL
};

struct Key_Configures {
public:
	Key_Configures(std::vector<SDL_Scancode> _keys)
	{
		printf("\nConstructor");
		this->Re_Configure(_keys);
	};
	Key_Configures(void) = default;

	void Re_Configure(std::vector<SDL_Scancode> _keys)
	{
		if (_keys.size() == KEY_TOTAL) {
			keys = _keys;
			for (unsigned short i = 0; i < _keys.size(); i++)
				std::cout << "\n" << _keys[i] ;
		}
	}
	void Save(bool newfile = false)
	{
		if (keys.size() == KEY_TOTAL) {
			std::string filename{"Data/Key_Configures-"};
			if (newfile) {
				static int files = 1;
				filename = filename + std::to_string(files) + ".txt"; 
				files++;
			}

			printf("\nFilename : %s", filename.c_str());

			m_data_file.open(filename.c_str(), std::ios::out | std::ios::trunc);

			m_data_file << (int)keys.size() << " ";
			for (unsigned short i = 0; i < keys.size(); i++) {
				m_data_file << (int)keys[i] << " ";
			}

			m_data_file.close();
		}

	}

	// Under Construction. Load()
	void Load(std::string filename = "../../Data/Key_Configures.txt")
	{
		m_data_file.open(filename.c_str(), std::ios::in);

		int length; m_data_file >> length;
		std::vector<int> temp(length);

		for (unsigned short i = 0; i < temp.size(); i++) {
			m_data_file >> temp[i];
			keys[i] = (SDL_Scancode)temp[i];
		}


		m_data_file.close();
	}

	std::vector<SDL_Scancode> keys;

private:
	// Storage
	std::fstream m_data_file;

}; 

#endif