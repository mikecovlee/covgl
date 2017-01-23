#ifndef GUARD_COVDEVKIT_H


#define GUARD_COVDEVKIT_H


#include <cstdlib>
#include <cstdio>
#include <ctime>


namespace cov
{
	inline int strlen(const char *);
	inline int copy_file(const char *, const char *);
	namespace ioctrl
	{
		inline void flush();
		inline void endl();
	}
	inline void print(const int &);
	inline void print(const double &);
	inline void print(const char &);
	inline void print(const char *);
	inline void print(void (*)());
	  template < typename Type > inline void print(const Type &);
	  template < typename Type, typename ... Argt > inline void print(const Type &,
																	  const Argt & ...);
	  template < typename > class handle;
	  template < typename, typename > class list;
	  template < typename > class mempool;
	enum types
	{
		integer = 0xEA,
		floating = 0xEB,
		boolean = 0xEC,
		character = 0xED,
		string = 0xEE
	};
	class generic_type;
	class random;
}


inline int cov::strlen(const char *str)
{
	if (*str == '\0')
		return 0;
	int siz = 0;
	while (*(str + (++siz)));
	return siz;
}


inline int cov::copy_file(const char *source, const char *target)
{
	const int buff_size = 256;
	FILE *in_file, *out_file;
	char data[buff_size];
	size_t bytes_in, bytes_out;
	if ((in_file = fopen(source, "rb")) == NULL)
		return 1;
	if ((out_file = fopen(target, "wb")) == NULL)
		return 2;
	while ((bytes_in = fread(data, 1, buff_size, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
			return 3;
	}
	fclose(in_file);
	fclose(out_file);
	return 0;
}


inline void cov::print(const int &i)
{
	printf("%d", i);
}


inline void cov::print(const double &d)
{
	printf("%f", d);
}


inline void cov::print(const char &c)
{
	printf("%c", c);
}


inline void cov::print(const char *s)
{
	printf("%s", s);
}


inline void cov::print(void (*func) ())
{
	func();
}


inline void cov::ioctrl::flush()
{
	fflush(stdout);
}


inline void cov::ioctrl::endl()
{
	printf("\n");
	fflush(stdout);
}


template < typename Type > inline void cov::print(const Type & obj)
{
	return;
}


template < typename Type, typename...Argt >
	inline void cov::print(const Type & obj, const Argt & ... argv)
{
	print(obj);
	print(argv...);
}


template < typename T > class cov::handle
{
  protected:
	T * m_ptr;
	long *ref;
  public:
  handle():m_ptr(new T), ref(new long (1))
	{
	}
	handle(const T & obj):m_ptr(new T(obj)), ref(new long (1))
	{
	}
	handle(const handle & obj):m_ptr(obj.m_ptr), ref(obj.ref)
	{
		++*ref;
	}
	~handle()
	{
		if (--*ref == 0)
		{
			delete m_ptr;
			delete ref;
		}
	}
	const T & data()
	{
		return *m_ptr;
	}
	T & reality()
	{
		if (*ref > 1)
		{
			--*ref;
			ref = new long (1);
			m_ptr = new T(*m_ptr);
		}
		return *m_ptr;
	}
	operator        const T & ()
	{
		return *m_ptr;
	}
	const T *operator->()
	{
		return m_ptr;
	}
	const T & operator=(const T & obj)
	{
		if (*ref > 1)
		{
			--*ref;
			ref = new long (1);
			m_ptr = new T(*m_ptr);
		}
		*m_ptr = obj;
		return obj;
	}
	const handle & operator=(const handle & obj)
	{
		if (&obj != this)
		{
			if (--*ref == 0)
			{
				delete m_ptr;
				delete ref;
			}
			ref = obj.ref;
			m_ptr = obj.m_ptr;
			++*ref;
		}
		return *this;
	}
};


template < typename InType, typename T = cov::handle < InType > >class cov::list
{
  protected:
	class node
	{
		friend class list;
		friend class iterator;
		T data;
		node *front = 0;
		node *behind = 0;
	};
  public:
	class iterator
	{
		friend class list;
	  protected:
		node * me;
	  public:
	  iterator():me(0)
		{
		}
	  iterator(node * ptr):me(ptr)
		{
		}
		iterator(const iterator &) = default;
		operator                bool() const
		{
			return me;
		}
		T & operator*() const
		{
			if (me)
				return me->data;
		}
		T *operator->() const
		{
			if (me)
				return &me->data;
		}
		iterator & operator++()
		{
			me = me->behind;
			return *this;
		}
		iterator & operator--()
		{
			me = me->front;
			return *this;
		}
		bool operator==(const iterator & obj)
		{
			return me == obj.me;
		}
		bool operator!=(const iterator & obj)
		{
			return me != obj.me;
		}
	};
  protected:
	node * m_head;
	node *m_end;
	long m_size = 0;
  public:
  list():m_head(0), m_end(0)
	{
	}
	list(const list & in):m_head(0), m_end(0), m_size(in.m_size)
	{
		for (node * it = in.m_head; it; it = it->behind)
		{
			node *newdat = new node;
			newdat->data = it->data;
			newdat->front = m_end;
			if (m_end)
				m_end->behind = newdat;
			m_end = newdat;
			if (!m_head)
				m_head = newdat;
		}
	}
	~list()
	{
		node *next;
		node *it = m_head;
		while (it)
		{
			next = it->behind;
			delete it;
			it = next;
		}
	}
	void clear()
	{
		node *next;
		node *it = m_head;
		while (it)
		{
			next = it->behind;
			delete it;
			it = next;
		}
		m_head = m_end = 0;
		m_size = 0;
	}
	const list & operator=(const list & in)
	{
		if (&in != this)
		{
			node *next;
			node *it = m_head;
			while (it)
			{
				next = it->behind;
				delete it;
				it = next;
			}
			for (node * it = in.m_head; it; it = it->behind)
			{
				node *newdat = new node;
				newdat->data = it->data;
				newdat->front = m_end;
				if (m_end)
					m_end->behind = newdat;
				m_end = newdat;
				if (!m_head)
					m_head = newdat;
			}
			m_size = in.m_size;
		}
		return *this;
	}
	void push_front(const T & obj)
	{
		++m_size;
		node *newdat = new node;
		newdat->data = obj;
		if (m_head)
		{
			newdat->behind = m_head;
			m_head->front = newdat;
		}
		m_head = newdat;
		if (!m_end)
			m_end = newdat;
	}
	void push_back(const T & obj)
	{
		++m_size;
		node *newdat = new node;
		newdat->data = obj;
		if (m_end)
		{
			newdat->front = m_end;
			m_end->behind = newdat;
		}
		m_end = newdat;
		if (!m_head)
			m_head = newdat;
	}
	void pop_front()
	{
		if (m_size == 0)
			return;
		--m_size;
		if (m_head == m_end)
		{
			delete m_head;
			m_head = m_end = 0;
			return;
		}
		node *newdat = m_head->behind;
		delete m_head;
		m_head = newdat;
		if (m_head)
			m_head->front = 0;
	}
	void pop_back()
	{
		if (m_size == 0)
			return;
		--m_size;
		if (m_head == m_end)
		{
			delete m_head;
			m_head = m_end = 0;
			return;
		}
		node *newdat = m_end->front;
		delete m_end;
		m_end = newdat;
		if (m_end)
			m_end->behind = 0;
	}
	void insert(iterator it, const T & obj)
	{
		node *olddat = it.me;
		node *newdat = new node;
		newdat->data = obj;
		newdat->front = olddat;
		newdat->behind = olddat->behind;
		olddat->behind->front = newdat;
		olddat->behind = newdat;
		++m_size;
	}
	void erase(iterator & it)
	{
		node *olddat = it.me;
		++it;
		if (olddat == m_head)
			pop_front();
		else if (olddat == m_end)
			pop_back();
		else
		{
			olddat->behind->front = olddat->front;
			olddat->front->behind = olddat->behind;
			delete olddat;
		}
	}
	void remove(const T & obj)
	{
		node *it = m_head;
		while (it)
			if (it->data == obj)
			{
				node *next;
				if (it == m_head)
				{
					next = it->behind;
					pop_front();
					it = next;
				}
				else if (it == m_end)
				{
					pop_back();
					return;
				}
				else
				{
					next = it->behind;
					it->behind->front = it->front;
					it->front->behind = it->behind;
					delete it;
					it = next;
					--m_size;
				}
			}
			else
				it = it->behind;
	}
	long size() const
	{
		return m_size;
	}
	iterator head()
	{
		return list < InType, T >::iterator(m_head);
	}
	iterator tail()
	{
		return list < InType, T >::iterator(m_end);
	}
};


template < typename chunk_type > class cov::mempool
{
  protected:
	struct pool_block
	{
		bool available = true;
		void *data = 0;
	};
	int pool_width;
	cov::list < pool_block * >m_pool;
	void ext(int size)
	{
		pool_block *unit = 0;
		for (int i = 0; i < size; ++i)
		{
			unit = (pool_block *)::operator                       new(sizeof(pool_block));
			unit->available = true;
			unit->data = (chunk_type *)::operator                       new(sizeof(chunk_type));
			m_pool.push_back(unit);
		}
	}
  public:
  mempool():pool_width(32)
	{
		ext(pool_width);
	}
	mempool(int pwid):pool_width(pwid)
	{
		ext(pool_width);
	}
	~mempool()
	{
		for (auto it = m_pool.head(); it; ++it)
		{
			pool_block *unit = *it;
			::operator                       delete(unit->data);
			unit->available = true;
			delete *it;
		}
	}
	chunk_type *allot()
	{
		bool find = false;
		pool_block *unit = 0;
		for (auto it = m_pool.head(); it; ++it)
		{
			unit = *it;
			if (unit->available)
			{
				find = true;
				break;
			}
		}
		if (!find)
		{
			ext(pool_width);
			unit = *m_pool.tail();
		}
		chunk_type *data = (chunk_type *) unit->data;
		new(data) chunk_type();
		unit->available = false;
		return data;
	}
	chunk_type *allot(const chunk_type & object)
	{
		bool find = false;
		pool_block *unit = 0;
		for (auto it = m_pool.head(); it; ++it)
		{
			unit = *it;
			if (unit->available)
			{
				find = true;
				break;
			}
		}
		if (!find)
		{
			ext(pool_width);
			unit = *m_pool.tail();
		}
		chunk_type *data = (chunk_type *) unit->data;
		new(data) chunk_type(object);
		unit->available = false;
		return data;
	}
	bool liberate(chunk_type * ptr)
	{
		bool finish = false;
		pool_block *unit = 0;
		for (auto it = m_pool.tail(); it; --it)
		{
			unit = *it;
			if (ptr == unit->data)
			{
				unit->available = true;
				finish = true;
				break;
			}
		}
		return finish;
	}
};


class cov::generic_type
{
  protected:
	char tmp[16];
	union
	{
		int integer;
		double floating;
		bool boolean;
		char character;
		const char *string;
	} m_value;
	int m_type;
  public:
	generic_type():m_type(types::integer)
	{
	}
	template < typename T > generic_type(T v)
	{
		assign(v);
	}
	int type()
	{
		return m_type;
	}
	template < typename T > bool assign(T v)
	{
		return false;
	}
	template < typename T > generic_type & operator=(T v)
	{
		assign(v);
		return *this;
	}
	const char *val()
	{
		switch (m_type)
		{
		case types::integer:
			sprintf(tmp, "%d", m_value.integer);
			return tmp;
		case types::floating:
			sprintf(tmp, "%f", m_value.floating);
			return tmp;
		case types::boolean:
			if (m_value.boolean)
				return "true";
			else
				return "false";
		case types::character:
			tmp[0] = m_value.character;
			tmp[1] = '\0';
			return tmp;
		case types::string:
			return m_value.string;
		}
		throw;
	}
	operator              const char *()
	{
		return val();
	}
	template < typename T > T eval()
	{
		throw;
	}
};


namespace cov
{
	template <> bool generic_type::assign < int >(int v)
	{
		m_value.integer = v;
		m_type = types::integer;
		return true;
	}


	template <> bool generic_type::assign < double >(double v)
	{
		m_value.floating = v;
		m_type = types::floating;
		return true;
	}


	template <> bool generic_type::assign < bool > (bool v)
	{
		m_value.boolean = v;
		m_type = types::boolean;
		return true;
	}


	template <> bool generic_type::assign < char >(char v)
	{
		m_value.character = v;
		m_type = types::character;
		return true;
	}


	template <> bool generic_type::assign < const char *>(const char *v)
	{
		m_value.string = v;
		m_type = types::string;
		return true;
	}


	template <> int generic_type::eval < int >()
	{
		switch (m_type)
		{
		case types::integer:
			return m_value.integer;
		case types::floating:
			return m_value.floating;
		case types::boolean:
			return m_value.boolean;
		case types::character:
			return m_value.character;
		}
		throw;
	}


	template <> double generic_type::eval < double >()
	{
		switch (m_type)
		{
		case types::integer:
			return m_value.integer;
		case types::floating:
			return m_value.floating;
		case types::boolean:
			return m_value.boolean;
		case types::character:
			return m_value.character;
		}
		throw;
	}


	template <> bool generic_type::eval < bool > ()
	{
		if (m_type == types::boolean)
			return m_value.boolean;
		throw;
	}


	template <> char generic_type::eval < char >()
	{
		if (m_type == types::character)
			return m_value.character;
		throw;
	}


	template <> const char *generic_type::eval < const char *>()
	{
		if (m_type == types::string)
			return m_value.string;
		throw;
	}
}


class cov::random
{
  public:
	random()
	{
		srand((int)time(0));
	}
	~random()
	{
		srand((int)time(0));
	}
	int get(int m, int n)
	{
		if (m == n)
			return m;
		int pos, dis;
		if (m > n)
		{
			pos = n;
			dis = m - n + 1;
		}
		else
		{
			pos = m;
			dis = n - m + 1;
		}
		return (rand() / (RAND_MAX / dis)) + pos;
	}
};


#endif