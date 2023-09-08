#include<iostream>

template <class T>
struct Node
{
    T data;
    Node* next;
    Node(const T& item) { data = item; }
    Node<T>* operator++(int) { return next; }
};

template <class T>
ostream& operator<<(ostream& os, const Node<T>& dt)
{
    std::cout << dt.data;
    return os;
}

template<typename T>
class linear_iterator
{
public:
    linear_iterator() { }
    linear_iterator(T* ptr) { ptr_ = ptr; }

    ~linear_iterator() {}

    linear_iterator operator++()
    { 
        ptr_ = (*ptr_)++;
        return *this;
    }

    linear_iterator& operator++(int)
    { 
        ptr_ = (*ptr_)++;
        return *this; 
    }

    T& operator*() { return *ptr_; }
    T* operator->() { return ptr_; }
    bool operator==(const linear_iterator& rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const linear_iterator& rhs) 
    { 
        return ptr_ != rhs.ptr_; 
    }
private:
    T* ptr_;
};

template <class T, class _Alloc = allocator<T>>
class single_linked_container
{
    using NodeAllocator = typename std::allocator_traits<_Alloc>::template rebind_alloc<Node<T>>;

    NodeAllocator m_node_allocator = NodeAllocator();
    
    Node<T>* m_head = nullptr;
    Node<T>* m_tail = nullptr;

    void insert_item(Node<T>* previousNode, Node<T>* newNode)
    {
        if (previousNode == nullptr)
        {
            // Is the first node
            if (m_head != nullptr)
            {
                // The list has more elements
                newNode->next = m_head;
            }
            else
            {
                newNode->next = nullptr;
            }
            m_head = newNode;
            m_tail = newNode;
        }
        else
        {
            if (previousNode->next == nullptr)
            {
                // Is the last node
                previousNode->next = newNode;
                newNode->next = nullptr;
                m_tail = newNode;
            }
            else
            {
                // Is a middle node
                newNode->next = previousNode->next;
                previousNode->next = newNode;
            }
        }

        m_tail = newNode;
    }

    void remove_item(Node<T>* previousNode, Node<T>* deleteNode)
    {
        if (previousNode == m_head)
        {
            // Is the first node
            if (deleteNode->next == nullptr)
            {
                // List only has one element
                m_head = nullptr;
                m_tail = nullptr;
            }
            else
            {
                // List has more elements
                m_head = deleteNode->next;
            }
        }
        else
        {
            previousNode->next = deleteNode->next;
        }
    }

    void free_item_list()
    {
        Node<T>* delete_node = nullptr;
        Node<T>* next_node = m_head;

        while (m_items)
        {
            //Always delete from head
            remove_item(m_head, next_node);

            delete_node = next_node;
            next_node = next_node->next;

            delete_node->data.~T();

            m_node_allocator.deallocate(delete_node, 1);

            m_items--;
        }
    }

    void copy_items(const Node<T>* head, const size_t& items)
    {
        const Node<T>* copied_node = head;
        Node<T>* prev_node = nullptr;

        for (m_items = 0; m_items != items; m_items++)
        {
            //Use defined allocator
            Node<T>* new_node = (Node<T>*)m_node_allocator.allocate(1);
            
            //Construct new node via traits (C++20)
            allocator_traits<NodeAllocator>::construct(m_node_allocator, new_node, Node<T>(copied_node->data));

            new_node->next = nullptr;

            //Insert to list tail
            insert_item(prev_node, new_node);

            copied_node = copied_node->next;
            prev_node = new_node;
        }
    }

public:
    size_t m_items = 0x0;

    single_linked_container() {};

    single_linked_container(const single_linked_container& other)
    {
        copy_items(other.m_head, other.m_items);
    }

    ~single_linked_container() 
    {
        free_item_list();
    }

    size_t size()
    {
        return m_items;
    }

    bool empty()
    {
        return m_items == 0;
    }

    void push_back(const T& item)
    {
        //Use defined allocator
        Node<T>* new_node = (Node<T>*)m_node_allocator.allocate(1);

        //new_node->data = T(item); ---> Seg fault while on copy string from item to data
        allocator_traits<NodeAllocator>::construct(m_node_allocator, new_node, Node<T>(item));

        //This is last item - point to null
        new_node->next = nullptr;

        //Insert always in tail
        insert_item(m_tail, new_node);
        
        m_items++;
    }

    single_linked_container& operator=(const single_linked_container& other)
    {
        // Guard self assignment
        if (this == &other)
            return *this;

        //Delete old data
        free_item_list();
        //Copy list
        copy_items(other.m_head, other.m_items);

        return *this;
    }

    T& operator[](const size_t Pos)
    {
        Node<T>* next = m_head;

        size_t counter = Pos;

        while(counter--)
            next = next->next;

        return next->data;
    }

    linear_iterator<Node<T>> begin()
    {
        auto it = linear_iterator<Node<T>>(m_head);
        return it;
    }

    linear_iterator<Node<T>> end()
    {
        //Last item always point to null
        auto it = linear_iterator<Node<T>>(nullptr);
        return it;
    }
};