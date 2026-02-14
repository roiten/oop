// copying.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

class PostCard
{
public:
	PostCard(const std::string& text)
		: m_text{ text }
	{
		std::cout << "Post card was created: " << m_text << std::endl;
	}
	// Копирующий конструктор
	PostCard(const PostCard& other)
		: m_text{ other.m_text }
	{
		std::cout << "Post card is copied: " << m_text << std::endl;
	}
	~PostCard() { std::cout << "Post card was destroyed: " << m_text << std::endl; }

	const std::string& GetText() const { return m_text; }

	void SetText(const std::string& text) { m_text = text; }

private:
	std::string m_text;
};

PostCard MakeModifiedPostCard(PostCard postCard) {
	std::cout << "Enter MakeModifiedPostCard" << std::endl;
	postCard.SetText(postCard.GetText() + " - modified");
	std::cout << "Exit MakeModifiedPostCard" << std::endl;
	return postCard;
}

struct Envelope
{
	PostCard postCard;
};

Envelope MakeModifiedPostcardInEnvelope(Envelope envelope)
{
	std::cout << "Enter MakeModifiedPostCard" << std::endl;
	envelope.postCard.SetText(envelope.postCard.GetText() + " - modified");
	std::cout << "Exit MakeModifiedPostCard" << std::endl;
	return envelope;
}

class String
{
public:
	String() noexcept
		: m_chars{ s_emptyString }
	{
	}
	String(const char* text)
		: m_size{ std::strlen(text) }
		, m_capacity{ m_size }
		, m_chars{ Allocate(m_capacity + 1) }
	{
		std::uninitialized_copy_n(text, m_size + 1, m_chars);
	}
	String(const String& other)
		: m_size{other.m_size}
		, m_capacity{other.m_capacity}
		, m_chars{ Allocate(m_capacity + 1) }
	{
		std::uninitialized_copy_n(other.m_chars, m_size + 1, m_chars);
	}
	~String()
	{
		if (m_chars != s_emptyString)
		{
			std::destroy_n(m_chars, m_size + 1);
			Deallocate(m_chars);
		}
	}

private:
	static char* Allocate(size_t size)
	{
		return static_cast<char*>(operator new(size));
	}
	static void Deallocate(char* buffer) noexcept
	{
		operator delete(buffer);
	}

	inline static char s_emptyString[] = { '\0' };

	size_t m_size = 0;
	size_t m_capacity = 0;
	char* m_chars;
};

int main()
{
	using namespace std::literals;
#if 0
	PostCard postCard{ "Hello" };
	auto modifiedPostCard = MakeModifiedPostCard(postCard);
	std::cout << modifiedPostCard.GetText() << std::endl;
#endif

#if 0
	auto modifiedPostCard = MakeModifiedPostCard("Hello"s);
	std::cout << modifiedPostCard.GetText() << std::endl;
#endif

#if 0
	Envelope env{.postCard = "Hello" };
	auto modifiedEnv = MakeModifiedPostcardInEnvelope(env);
	std::cout << modifiedEnv.postCard.GetText() << std::endl;
#endif
}
