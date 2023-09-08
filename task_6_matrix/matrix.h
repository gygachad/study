#include <map>

#pragma once

template<typename T, size_t matrix_size, T defaultValue>
class matrix
{
    typedef std::conditional_t<matrix_size == 1, T, matrix<T, matrix_size - 1, defaultValue>> mT;
    std::map<int, mT> m_matrixMap;

    int m_lazy_key;

public:

    mT m_lazyMatrix;
  
    matrix() { reset_default_value(); }

    void reset_default_value()
    {
        if constexpr (matrix_size == 1)
            m_lazyMatrix = defaultValue;
        else
            m_lazyMatrix = mT();        
    }

    bool check_default_value()
    {
        if constexpr (matrix_size == 1)
            return m_matrixMap[m_lazy_key] == defaultValue;
        else
            return m_matrixMap[m_lazy_key].size() == 0;
    }

    bool update_lazy_matrix()
    {
        bool status = false;

        if constexpr (matrix_size == 1)
            status = (m_lazyMatrix != defaultValue);
        else
            status = m_lazyMatrix.update_lazy_matrix();

        if (status)
        {
            m_matrixMap[m_lazy_key] = m_lazyMatrix;
            reset_default_value();
        }
        else
        {
            if (m_matrixMap.contains(m_lazy_key))
            {
                if (check_default_value())
                {
                    m_matrixMap.erase(m_lazy_key);
                }
            }
        }

        return status;
    }

    mT& operator[](int key)
    {
        update_lazy_matrix();
        m_lazy_key = key;

        if (m_matrixMap.contains(key))
        {
            return m_matrixMap[key];
        }

        return m_lazyMatrix;
    }

    size_t size()
    {
        update_lazy_matrix();

        size_t size = 0;

        if constexpr (matrix_size == 1)
        {
            size = m_matrixMap.size();
        }
        else
        {
            int key = 0;
            mT child;

            for (auto c : m_matrixMap)
            {
                std::tie(key, child) = c;
                size += child.size();
            }
        }

        return size;
    }

    auto begin()
    {
        update_lazy_matrix();
        return iterator(m_matrixMap.begin(), m_matrixMap.end());
    }
    
    auto end()
    {
        return iterator(m_matrixMap.end(), m_matrixMap.end());
    }

    struct iterator
    {
        typename std::map<int, mT>::iterator m_it_begin;
        typename std::map<int, mT>::iterator m_it_end;

        typedef std::conditional_t<matrix_size == 1, std::map<int, mT>, matrix<T, matrix_size - 1, defaultValue>> mdeepT;

        mdeepT::iterator m_deepit_begin;
        mdeepT::iterator m_deepit_end;

        iterator() {}

        iterator(typename std::map<int, mT>::iterator ptr_begin, typename std::map<int, mT>::iterator ptr_end)
        {
            m_it_begin = ptr_begin;
            m_it_end = ptr_end;
            
            if constexpr (matrix_size != 1)
            {
                if (ptr_begin != ptr_end)
                {
                    m_deepit_begin = ptr_begin->second.begin();
                    m_deepit_end = ptr_begin->second.end();
                }
            }
        }

        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_it_begin != b.m_it_begin; }
        friend bool operator== (const iterator& a, const iterator& b) { return a.m_it_begin == b.m_it_begin; }

        iterator* operator->()
        {
            return this;
        }

        iterator operator++(int) 
        {
            iterator _Tmp = *this;
            ++* this;
            return _Tmp;
        }

        iterator& operator++()
        {
            if constexpr (matrix_size != 1)
            {
                //Increment iterator of matrix<size-1>
                m_deepit_begin++;

                if (m_deepit_begin == m_deepit_end)
                {
                    m_it_begin++;
                    if (m_it_begin != m_it_end)
                    {
                        m_deepit_begin = m_it_begin->second.begin();
                        m_deepit_end = m_it_begin->second.end();
                    }
                }
            }
            else
            {
                m_it_begin++;
            }

            return *this;
        }

        auto operator*()
        {
            if constexpr (matrix_size == 1)
                return *m_it_begin;
            else
                return std::tuple_cat(std::make_tuple(m_it_begin->first), *m_deepit_begin);
        }
    };
    
    /*
    struct matrix_iterator<1>
    {
        typename std::map<int, T>::iterator m_it_begin;
        typename std::map<int, T>::iterator m_it_end;

        matrix_iterator() {}

        matrix_iterator(typename std::map<int, T>::iterator ptr_begin, typename std::map<int, T>::iterator ptr_end) { m_it_begin = ptr_begin; m_it_end = ptr_end; }

        matrix_iterator* operator->() { return this; }

        friend bool operator!= (const matrix_iterator& a, const matrix_iterator& b) { return a.m_it_begin != b.m_it_begin; }
        friend bool operator== (const matrix_iterator& a, const matrix_iterator& b) { return a.m_it_begin == b.m_it_begin; }

        matrix_iterator& operator++() { m_it_begin++; return *this; }

        auto operator*() { return std::tuple_cat(*m_it_begin); }
    };
    */
};

