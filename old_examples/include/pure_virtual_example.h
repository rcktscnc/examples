class pure_base {
public:
	virtual int get() const = 0;
};

class base: public pure_base {
private:
	int a;
public:
	base(int a): a(a) { }
	int get() const override { return a; }
};

class derived: public base {
private:
	int b;
public:
	derived(int b): base(0), b(b) { }
	int get() const override { return b; }
};

int pve_func(const pure_base& pb) {
	return pb.get();
}