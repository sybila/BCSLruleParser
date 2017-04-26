#ifndef STREAM_FACTORY_H
#define STREAM_FACTORY_H

enum CommType
{
	TYPE_JSON,
};

enum StreamType
{
	STREAM_TYPE_UNK,
	STREAM_TYPE_IO,
	STREAM_TYPE_FILE,
	STREAM_TYPE_STRING,
};

template<typename T, typename U>
struct streamFactory
{
	streamFactory(std::string type)
	{
		if (type == "stream")
			m_type = STREAM_TYPE_IO;
		else if (type.find("file:") == 0)
		{
			m_type = STREAM_TYPE_FILE;
			m_fstream.open(type.substr(sizeof("file:") - 1));
		}
		else
			m_type = STREAM_TYPE_UNK;
	}

protected:
	StreamType m_type;
	U m_fstream;
};

struct istreamFactory : public streamFactory<std::istream, std::ifstream>
{
	istreamFactory(std::string t) : streamFactory(t)
	{
		if (t.find("eq:") == 0)
		{
			m_type = STREAM_TYPE_STRING;
			m_sstream << t.substr(sizeof("eq:") - 1);
			m_sstream.seekg(0);
		}
	}

	std::istream& get()
	{
		switch (m_type)
		{
			case STREAM_TYPE_IO: return std::cin;
			case STREAM_TYPE_FILE: return m_fstream;
			case STREAM_TYPE_STRING: return m_sstream;
			default: throw std::invalid_argument("Invalid input_type");
		}
	}

private:
	std::stringstream m_sstream;
};

struct ostreamFactory : public streamFactory<std::ostream, std::ofstream>
{
	ostreamFactory(std::string t) : streamFactory(t) {}
	std::ostream& get()
	{
		switch (m_type)
		{
		case STREAM_TYPE_IO: return std::cout;
			case STREAM_TYPE_FILE: return m_fstream;
			default: throw std::invalid_argument("Invalid output_type");
		}
	}
};

#endif
