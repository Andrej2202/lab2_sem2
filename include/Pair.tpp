template <typename T1, typename T2>
Pair<T1, T2>::Pair() : first(T1()), second(T2()) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(T1 f, T2 s) : first(f), second(s) {}

template <typename T1, typename T2>
bool Pair<T1, T2>::operator<(const Pair& other) const { return first < other.first; }

template <typename T1, typename T2>
bool Pair<T1, T2>::operator>(const Pair& other) const { return first > other.first; }

template <typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair& other) const { return first == other.first; }

template <typename T1, typename T2>
bool Pair<T1, T2>::operator<=(const Pair& other) const { return first <= other.first; }

template <typename T1, typename T2>
bool Pair<T1, T2>::operator>=(const Pair& other) const { return first >= other.first; }

template <typename T1, typename T2>
bool Pair<T1, T2>::operator!=(const Pair& other) const { return first != other.first; }