class Editor {
	public:
	char raw;
	std::string buffer;
	enum Mode {NAV,EDIT};
	Mode current_mode;

	void inputLoop();
	void scanNavFunctions();

};
