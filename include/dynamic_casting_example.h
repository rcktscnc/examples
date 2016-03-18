class ASD {
public:
	int a;
	virtual void func() { }
};
class BSD : public ASD {
public:
	int b;
	
};
class CSD : public ASD {
public:
	int c;
};
class DSD {
public:
	int d;
	virtual void func() {}
};

class ESD : public BSD {
public:
	int e;
};

void dynamic() {
	BSD bsd;
	bsd.a = 9;
	bsd.b = 11;
	ASD* pasd = &bsd;
	
	CSD csd; csd.c = 5;
	CSD* pcsd = &csd;
	
	DSD dsd; dsd.d = 3;
	DSD* pdsd = &dsd;
	
	ESD esd; esd.e = 55; esd.b = 88;
	ESD* pesd = &esd;
	
	BSD* pbsd = dynamic_cast<BSD*>(pcsd);
	if (pbsd == nullptr) {
		std::cout << "weird";
	}
	else {
		std::cout << pbsd->b;
	}
}