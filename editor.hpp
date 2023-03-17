#include <vector>

class Editor {
	public:
	char raw;
	int max;
	int curr;
	int col;
	int row;
	std::string buffer;
	enum Mode {NAV,EDIT};
	Mode current_mode;

	void inputLoop();
	void scanNavFunctions();

};
