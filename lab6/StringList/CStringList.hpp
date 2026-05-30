#pragma once
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

class CStringList
{
	struct BaseNode
	{
		std::unique_ptr<BaseNode> next;
		BaseNode* prev = nullptr;
	};

	struct Node : BaseNode
	{
		std::string data;

		Node(std::string data)
			: data(std::move(data))
		{
		}
	};

public:
	template <bool IsConst>
	class CIterator
	{
		friend CStringList;

		template <bool OtherConst>
		friend class CIterator;

		CIterator(BaseNode* node, BaseNode* end)
			: m_node(node)
			, m_end(end)
		{
		}

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = std::string;
		using difference_type = std::ptrdiff_t;
		using reference = std::conditional_t<IsConst, const std::string&, std::string&>;
		using pointer = std::conditional_t<IsConst, const std::string*, std::string*>;

		CIterator() = default;
		operator CIterator<true>() const { return {m_node, m_end}; }

		reference operator*() const
		{
			assert(m_node != nullptr && m_node != m_end);
			return static_cast<Node*>(m_node)->data;
		}

		pointer operator->() const
		{
			assert(m_node != nullptr && m_node != m_end);
			return &**this;
		}

		CIterator& operator++()
		{
			m_node = m_node->next ? m_node->next.get() : m_end;
			return *this;
		}
		CIterator& operator--()
		{
			m_node = m_node->prev;
			return *this;
		}

		CIterator operator++(int)
		{
			auto tmp = *this;
			++(*this);
			return tmp;
		}
		CIterator operator--(int)
		{
			auto tmp = *this;
			--(*this);
			return tmp;
		}

		template <bool OtherConst>
		bool operator==(const CIterator<OtherConst>& other) const { return m_node == other.m_node; }

		template <bool OtherConst>
		bool operator!=(const CIterator<OtherConst>& other) const { return m_node != other.m_node; }

	private:
		BaseNode* m_node = nullptr;
		BaseNode* m_end = nullptr;
	};

	CStringList()
	{
		m_baseNode.prev = &m_baseNode;
	}

	CStringList(const CStringList& other)
	{
		m_baseNode.prev = &m_baseNode;
		for (const auto& s : other)
			PushBack(s);
	}

	CStringList(CStringList&& other) noexcept
	{
		m_baseNode.prev = &m_baseNode;
		if (!other.IsEmpty())
		{
			m_baseNode.next = std::move(other.m_baseNode.next);
			m_baseNode.prev = other.m_baseNode.prev;
			m_baseNode.next->prev = &m_baseNode;
			m_baseNode.prev->next = nullptr;
			m_size = other.m_size;

			other.m_baseNode.prev = &other.m_baseNode;
			other.m_size = 0;
		}
	}

	~CStringList() { Clear(); }

	CStringList& operator=(const CStringList& other)
	{
		if (this != &other)
		{
			CStringList tmp(other);
			*this = std::move(tmp);
		}
		return *this;
	}

	CStringList& operator=(CStringList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			if (!other.IsEmpty())
			{
				m_baseNode.next = std::move(other.m_baseNode.next);
				m_baseNode.prev = other.m_baseNode.prev;
				m_baseNode.next->prev = &m_baseNode;
				m_baseNode.prev->next = nullptr;
				m_size = other.m_size;

				other.m_baseNode.prev = &other.m_baseNode;
				other.m_size = 0;
			}
		}
		return *this;
	}

	CIterator<false> begin() { return CIterator<false>(m_baseNode.next ? m_baseNode.next.get() : &m_baseNode, &m_baseNode); }
	CIterator<true> begin() const { return cbegin(); }
	CIterator<false> end() { return CIterator<false>(&m_baseNode, &m_baseNode); }
	CIterator<true> end() const { return cend(); }

	CIterator<true> cbegin() const
	{
		BaseNode* s = const_cast<BaseNode*>(&m_baseNode);
		return CIterator<true>(m_baseNode.next ? m_baseNode.next.get() : s, s);
	}
	CIterator<true> cend() const
	{
		BaseNode* s = const_cast<BaseNode*>(&m_baseNode);
		return CIterator<true>(s, s);
	}

	std::reverse_iterator<CIterator<false>> rbegin() { return std::reverse_iterator(end()); }
	std::reverse_iterator<CIterator<true>> rbegin() const { return std::reverse_iterator(cend()); }
	std::reverse_iterator<CIterator<false>> rend() { return std::reverse_iterator(begin()); }
	std::reverse_iterator<CIterator<true>> rend() const { return std::reverse_iterator(cbegin()); }

	std::reverse_iterator<CIterator<true>> crbegin() const { return std::reverse_iterator(cend()); }
	std::reverse_iterator<CIterator<true>> crend() const { return std::reverse_iterator(cbegin()); }

	size_t GetSize() const { return m_size; }
	bool IsEmpty() const { return m_size == 0; }

	CIterator<false> Emplace(CIterator<true> pos, std::string data)
	{
		BaseNode* curr = pos.m_node;
		auto newNode = std::make_unique<Node>(data);
		Node* raw = newNode.get();

		newNode->prev = curr->prev;
		newNode->next = std::move(curr->prev->next);
		curr->prev = raw;
		newNode->prev->next = std::move(newNode);

		++m_size;
		return CIterator<false>(raw, &m_baseNode);
	}

	CIterator<false> Erase(CIterator<true> pos)
	{
		if (pos.m_node == &m_baseNode)
			throw std::out_of_range("Cannot erase end iterator");

		BaseNode* curr = pos.m_node;
		BaseNode* nextNode = curr->next ? curr->next.get() : &m_baseNode;

		nextNode->prev = curr->prev;
		curr->prev->next = std::move(curr->next);

		--m_size;
		return CIterator<false>(nextNode, &m_baseNode);
	}

	void Clear()
	{
		while (!IsEmpty())
			Erase(begin());
	}

	CIterator<false> PushFront(std::string const& data) { return Emplace(cbegin(), data); }
	CIterator<false> PushFront(std::string && data) { return Emplace(cbegin(), std::move(data)); }

	CIterator<false> PushBack(std::string const& data) { return Emplace(cend(), data); }
	CIterator<false> PushBack(std::string&& data) { return Emplace(cend(), std::move(data)); }

	void PopFront()
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		Erase(cbegin());
	}

	void PopBack()
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		Erase(std::prev(cend()));
	}

	std::string& GetFrontElement()
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		return *begin();
	}

	std::string const& GetFrontElement() const
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		return *cbegin();
	}

	std::string& GetBackElement()
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		return *std::prev(end());
	}

	std::string const& GetBackElement() const
	{
		if (IsEmpty())
			throw std::out_of_range("List is empty");
		return *std::prev(cend());
	}

private:
	size_t m_size = 0;
	BaseNode m_baseNode;
};