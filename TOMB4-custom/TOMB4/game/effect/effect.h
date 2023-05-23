#pragma once

template<class T>
class IEffect
{
public:
	IEffect() = default;
	virtual ~IEffect() = default;
	void AddEffect(T& effect) {
		m_array.push_back(effect);
	};
	virtual void Update() {};
	virtual void Draw() {};
	size_t GetCount() const;
	void Clear();
	bool operator==(const IEffect& other) const
	{
		return false;
	}
protected:
	std::vector<T> m_array;
};

template<class T>
inline size_t IEffect<T>::GetCount() const
{
	return m_array.size();
}

template<class T>
inline void IEffect<T>::Clear()
{
	m_array.clear();
}
