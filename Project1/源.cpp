#include<iostream>
#include<iomanip>
using namespace std;

struct Srep
{
	char* s;
	int sz;
	int n;
	Srep(int nsz, const char* p)
	{
		n = 1;
		sz = nsz;
		s = new char[sz + 1];
		strcpy(s, p);
	}
	~Srep()
	{
		delete[] s;
	}
	Srep* get_own_copy()
	{
		if (n == 1)
			return this;
		n--;
		return new Srep(sz, s);
	}
	void assign(int nsz, const char* p)
	{
		if (sz != nsz)
		{
			delete[] s;
			sz = nsz;
			s = new char[sz + 1];
		}
		strcpy(s, p);
	}
private:
	Srep(const Srep& x);
	Srep& operator=(const Srep& x);
};

class String
{
	Srep* rep;
public:
	class Cref
	{
		friend class String;
		String& s;
		int i;
		Cref(String& ss, int ii)
			:s(ss), i(ii) {}
	public:
		operator char() const
		{
			return s.read(i);
		}
		void operator=(char c)
		{
			s.write(i, c);
		}
	};
	class Range {};
	String();								//x=""
	String(const char*);					//x="abc"
	String(const String& x);				//x=other string
	String& operator=(const char* s);
	String& operator=(const String& x);
	~String();
	
	void check(int i) const
	{
		if (i < 0 || rep->sz <= i) throw Range();
	}
	char read(int i) const
	{
		return rep->s[i];
	}
	void write(int i,char c)
	{
		rep = rep->get_own_copy(); rep->s[i] = c;
	}
	Cref operator[](int i)
	{
		check(i); return Cref(*this, i);
	}
	char operator[](int i)const
	{
		check(i); return rep->s[i];
	}
	int size() const
	{
		return rep->sz;
	}
	String& operator+=(const String& s)
	{
		return *this = *this + s;
	}
	String& operator==(const char* s)
	{
		return *this = *this + s;
	}
	friend ostream& operator<<(ostream& os, const String& x)
	{
		os << x.rep->s;
		return os;
	}
	friend istream&  operator>>(istream& io, String& s)
	{
		const int limit_string_size = 100;
		char inbuf[limit_string_size];
		io >> setw(limit_string_size) >> inbuf;
		s = inbuf;
		return io;
	}
	String operator+(const String& rhs) const;
	String operator+(const char* s) const;
	friend bool operator==(const String& x, const String& y)
	{
		return strcpy(x.rep->s, y.rep->s) == 0;
	}
	friend bool operator==(const String& x, const char* s)
	{
		return strcpy(x.rep->s, s) == 0;
	}
	friend bool operator!=(const String& x, const char* s)
	{
		return strcpy(x.rep->s, s) != 0;
	}
	friend bool operator!=(const String& x, const String& y)
	{
		return strcpy(x.rep->s, y.rep->s) != 0;
	}
	char* Display(const String& s)
	{
		return s.rep->s;
	}
};

String::String()		//以空串作为默认值
{
	rep = new Srep(0, "");
}

String::String(const char* s)
{
	rep = new Srep(strlen(s), s);
}

String::String(const String& x)		//复制构造函数
{
	x.rep->n++;
	rep = x.rep;		//共享表示
}

String& String::operator=(const char* s)
{
	if (rep->n == 1)
		rep->assign(strlen(s), s);
	else
	{
		rep->n--;
		rep = new Srep(strlen(s), s);
	}
	return *this;
}

String& String::operator=(const String& x)
{
	x.rep->n++;		//保护，防止"st=st"
	if (--rep->n == 0)
		delete rep;
	rep = x.rep;
	return *this;
}

String::~String()
{
	if (--rep->n == 0)
		delete rep;
}

int hash(const String& s)
{
	int h = s.read(0);
	const int max = s.size();
	for (int i = 1; i < max; i++)
		h ^= s.read(i) >> i;
	return h;
}

String String::operator+(const String& rhs) const
{
	String newstring;
	if (rhs.rep->s == 0)
		newstring = *this;
	else if (rep->s == 0)
		newstring = rhs;
	else
	{
		newstring.rep->sz = rep->sz + rhs.rep->sz;
		newstring.rep->s = new char[newstring.rep->sz + 1];
		strcpy(newstring.rep->s, rep->s);
		strcat(newstring.rep->s, rhs.rep->s);
		newstring.rep->n = 1;
	}
	return newstring;
}

String String::operator+(const char* s) const
{
	String newstring;
	if (s == 0)
		newstring = *this;
	else if (rep->s == 0)
		newstring = s;
	else
	{
		newstring.rep->sz = rep->sz + strlen(s);
		newstring.rep->s = new char[newstring.rep->sz + 1];
		strcpy(newstring.rep->s, rep->s);
		strcat(newstring.rep->s, s);
		newstring.rep->n = 1;
	}
	return newstring;
}

String f(String a, String b)
{
	a[2] = 'x';
	char c = b[3];
	cout << "in f: " << a << ' ' << b << ' ' << c << "\n";
	return b;
}

int main()
{
	String x, y;
	cout << "Enter two string:\n";
	cin >> x >> y;
	cout << "input：" << x << ' ' << y << "\n";
	String z = x;
	y = f(x, y);
	if (x != z)
		cout << "x corrupted!\n";
	x[0] = '!';
	if (x == z)
		cout << "write failed!\n";
	cout << "exit:" << x << ' ' << y << ' ' << z << '\n';
}