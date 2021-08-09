template <typename T>
class List
{
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;
        Node(const T &d = T(), Node *p = nullptr, Node *n = nullptr)
        {
            data = d;
            prev = p;
            next = n;
        }
        Node(T &&d, Node *p = nullptr, Node *n = nullptr)
        {
            data = std::move(d);
            prev = p;
            next = n;
        }
    };
    int size_;
    Node *head;
    Node *tail;
    void init();

public:
    class const_iterator
    {
    protected:
        Node *current;
        T &retrieve() const { return current->data; }
        const_iterator(Node *p) : current(p) {}
        friend class List<T>;

    public:
        const_iterator() : current(nullptr) {}
        const T &operator*() const { return retrieve(); }
        const_iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        const_iterator &operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
        const_iterator &operator--()
        {
            current = current->prev;
            return *this;
        }
        const_iterator &operator--(int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }
        bool operator==(const const_iterator &rhs) const { return current == rhs.current; }
        bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
    };
    class iterator : public const_iterator
    {
    protected:
        iterator(Node *p) : const_iterator(p) {}
        friend class List<T>;

    public:
        iterator() {}
        T &operator*() { return const_iterator::retrieve(); }
        const T &operator*() const { return const_iterator::operator*(); }
        iterator &operator++()
        {
            this->current = this->current->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }
        iterator &operator--()
        {
            this->current = this->current->prev;
            return *this;
        }
        iterator &operator--(int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }
    };
    List() { init(); }
    List(const List &rhs);
    List(List &&rhs);
    List &operator=(const List &rhs);
    List &operator=(List &&rhs);
    iterator begin() { return head->next; }
    const_iterator begin() const { return head->next; }
    iterator end() { return tail; }
    const_iterator end() const { return tail; }
    int size() const { return size_; }
    bool empty() const { return size() == 0; }
    void clear();
    T &front() { return *begin(); }
    const T &front() const { return *begin(); }
    T &back() { return *--end(); }
    const T &back() const { return *--end(); }
    void push_front(const T &val) { insert(begin(), val); }
    void push_front(T &&val) { insert(end(), std::move(val)); }
    void push_back(const T &val) { insert(end(), val); }
    void push_back(T &&val) { insert(end(), std::move(val)); }
    void pop_front() { earse(begin()); }
    void pop_back() { earse(--end()); }
    iterator insert(iterator it, const T &val);
    iterator insert(iterator it, T &&val);
    iterator earse(iterator it);
    iterator earse(iterator from, iterator to);
    ~List();
};

template <typename T>
void List<T>::init()
{
    size_ = 0;
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
}

template <typename T>
List<T>::List(const List &rhs)
{
    init();
    for (auto &x : rhs)
        push_back(x);
}

template <typename T>
List<T>::List(List &&rhs)
{
    size_ = rhs.size_;
    head = rhs.head;
    tail = rhs.tail;
    rhs.size_ = 0;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}

template <typename T>
List<T> &List<T>::operator=(const List &rhs)
{
    List copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template <typename T>
List<T> &List<T>::operator=(List &&rhs)
{
    std::swap(size_, rhs.size_);
    std::swap(head, rhs.head);
    std::swap(tail, rhs.tail);
    return *this;
}

template <typename T>
void List<T>::clear()
{
    while (!empty())
        pop_front();
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator it, const T &val)
{
    Node *p = it.current;
    size_++;
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator it, T &&val)
{
    Node *p = it.current;
    size_++;
    return (p->prev = p->prev->next = new Node(std::move(val), p->prev, p));
}

template <typename T>
typename List<T>::iterator List<T>::earse(iterator it)
{
    Node *p = it.current;
    iterator retVal(p->next);
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    size_--;
    return retVal;
}

template <typename T>
typename List<T>::iterator List<T>::earse(iterator from, iterator to)
{
    for (iterator it = from; it != to;)
        it = earse(it);
    return to;
}

template <typename T>
List<T>::~List()
{
    clear();
    delete head;
    delete tail;
}
