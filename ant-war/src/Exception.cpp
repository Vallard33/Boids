class Exception {
	const char* message;
public:
	Exception(const char* m) : message(m) {};
	//~Exception() { delete[] message; };
};