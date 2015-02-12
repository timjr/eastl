/*
Copyright (C) 2009,2010,2012 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#ifndef EASTL_SET_H
#define EASTL_SET_H


#include <eastl/internal/config.h>
#include <eastl/internal/red_black_tree.h>
#include <eastl/functional.h>
#include <eastl/utility.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
    #pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{

    /// EASTL_SET_DEFAULT_NAME
    ///
    /// Defines a default container name in the absence of a user-provided name.
    ///
    #ifndef EASTL_SET_DEFAULT_NAME
        #define EASTL_SET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " set" // Unless the user overrides something, this is "EASTL set".
    #endif


    /// EASTL_MULTISET_DEFAULT_NAME
    ///
    /// Defines a default container name in the absence of a user-provided name.
    ///
    #ifndef EASTL_MULTISET_DEFAULT_NAME
        #define EASTL_MULTISET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " multiset" // Unless the user overrides something, this is "EASTL multiset".
    #endif


    /// EASTL_SET_DEFAULT_ALLOCATOR
    ///
    #ifndef EASTL_SET_DEFAULT_ALLOCATOR
        #define EASTL_SET_DEFAULT_ALLOCATOR allocator_type(EASTL_SET_DEFAULT_NAME)
    #endif

    /// EASTL_MULTISET_DEFAULT_ALLOCATOR
    ///
    #ifndef EASTL_MULTISET_DEFAULT_ALLOCATOR
        #define EASTL_MULTISET_DEFAULT_ALLOCATOR allocator_type(EASTL_MULTISET_DEFAULT_NAME)
    #endif



    /// set
    ///
    /// Implements a canonical set. 
    ///
    /// The large majority of the implementation of this class is found in the rbtree
    /// base class. We control the behaviour of rbtree via template parameters.
    ///
    /// Note that the 'bMutableIterators' template parameter to rbtree is set to false.
    /// This means that set::iterator is const and the same as set::const_iterator.
    /// This is by design and it follows the C++ standard defect report recommendation.
    /// If the user wants to modify a container element, the user needs to either use
    /// mutable data members or use const_cast on the iterator's data member. Both of 
    /// these solutions are recommended by the C++ standard defect report.
    /// To consider: Expose the bMutableIterators template policy here at the set level
    /// so the user can have non-const set iterators via a template parameter.
    ///
    /// Pool allocation
    /// If you want to make a custom memory pool for a set container, your pool 
    /// needs to contain items of type set::node_type. So if you have a memory
    /// pool that has a constructor that takes the size of pool items and the
    /// count of pool items, you would do this (assuming that MemoryPool implements
    /// the Allocator interface):
    ///     typedef set<Widget, less<Widget>, MemoryPool> WidgetSet;    // Delare your WidgetSet type.
    ///     MemoryPool myPool(sizeof(WidgetSet::node_type), 100);       // Make a pool of 100 Widget nodes.
    ///     WidgetSet mySet(&myPool);                                   // create a map that uses the pool.
    ///
    template <typename Key, typename compare = eastl::less<Key>, typename Allocator = EASTLAllocatorType>
    class set
        : public rbtree<Key, Key, compare, Allocator, eastl::useSelf<Key>, false, true>
    {
    public:
        typedef rbtree<Key, Key, compare, Allocator, eastl::useSelf<Key>, false, true> base_type;
        typedef set<Key, compare, Allocator>                                            this_type;
        typedef typename base_type::size_type                                           size_type;
        typedef typename base_type::value_type                                          value_type;
        typedef typename base_type::iterator                                            iterator;
        typedef typename base_type::const_iterator                                      const_iterator;
        typedef typename base_type::reverse_iterator                                    reverse_iterator;
        typedef typename base_type::const_reverse_iterator                              const_reverse_iterator;
        typedef typename base_type::allocator_type                                      allocator_type;
        // Other types are inherited from the base class.

        using base_type::begin;
        using base_type::end;
        using base_type::find;
        using base_type::lower_bound;
        using base_type::upper_bound;
        using base_type::mcompare;

    public:
        set(const allocator_type& allocator = EASTL_SET_DEFAULT_ALLOCATOR);
        set(const compare& compare, const allocator_type& allocator = EASTL_SET_DEFAULT_ALLOCATOR);
        set(const this_type& x);

        template <typename Iterator>
        set(Iterator itBegin, Iterator itEnd); // allocator arg removed because VC7.1 fails on the default arg. To do: Make a second version of this function without a default arg.

    public:
        size_type erase(const Key& k);
        iterator  erase(iterator position);
        iterator  erase(iterator first, iterator last);

        reverse_iterator erase(reverse_iterator position);
        reverse_iterator erase(reverse_iterator first, reverse_iterator last);

        size_type count(const Key& k) const;

        eastl::pair<iterator, iterator>             equalRange(const Key& k);
        eastl::pair<const_iterator, const_iterator> equalRange(const Key& k) const;

    }; // set





    /// multiset
    ///
    /// Implements a canonical multiset.
    ///
    /// The large majority of the implementation of this class is found in the rbtree
    /// base class. We control the behaviour of rbtree via template parameters.
    ///
    /// See notes above in 'set' regarding multable iterators.
    ///
    /// Pool allocation
    /// If you want to make a custom memory pool for a multiset container, your pool 
    /// needs to contain items of type multiset::node_type. So if you have a memory
    /// pool that has a constructor that takes the size of pool items and the
    /// count of pool items, you would do this (assuming that MemoryPool implements
    /// the Allocator interface):
    ///     typedef multiset<Widget, less<Widget>, MemoryPool> WidgetSet;   // Delare your WidgetSet type.
    ///     MemoryPool myPool(sizeof(WidgetSet::node_type), 100);           // Make a pool of 100 Widget nodes.
    ///     WidgetSet mySet(&myPool);                                       // create a map that uses the pool.
    ///
    template <typename Key, typename compare = eastl::less<Key>, typename Allocator = EASTLAllocatorType>
    class multiset
        : public rbtree<Key, Key, compare, Allocator, eastl::useSelf<Key>, false, false>
    {
    public:
        typedef rbtree<Key, Key, compare, Allocator, eastl::useSelf<Key>, false, false>    base_type;
        typedef multiset<Key, compare, Allocator>                                           this_type;
        typedef typename base_type::size_type                                               size_type;
        typedef typename base_type::value_type                                              value_type;
        typedef typename base_type::iterator                                                iterator;
        typedef typename base_type::const_iterator                                          const_iterator;
        typedef typename base_type::reverse_iterator                                    reverse_iterator;
        typedef typename base_type::const_reverse_iterator                              const_reverse_iterator;
        typedef typename base_type::allocator_type                                          allocator_type;
        // Other types are inherited from the base class.

        using base_type::begin;
        using base_type::end;
        using base_type::find;
        using base_type::lower_bound;
        using base_type::upper_bound;
        using base_type::mcompare;

    public:
        multiset(const allocator_type& allocator = EASTL_MULTISET_DEFAULT_ALLOCATOR);
        multiset(const compare& compare, const allocator_type& allocator = EASTL_MULTISET_DEFAULT_ALLOCATOR);
        multiset(const this_type& x);

        template <typename Iterator>
        multiset(Iterator itBegin, Iterator itEnd); // allocator arg removed because VC7.1 fails on the default arg. To do: Make a second version of this function without a default arg.

    public:
        size_type erase(const Key& k);
        iterator  erase(iterator position);
        iterator  erase(iterator first, iterator last);

        reverse_iterator erase(reverse_iterator position);
        reverse_iterator erase(reverse_iterator first, reverse_iterator last);

        size_type count(const Key& k) const;

        eastl::pair<iterator, iterator>             equalRange(const Key& k);
        eastl::pair<const_iterator, const_iterator> equalRange(const Key& k) const;

        /// equalRangeSmall
        /// This is a special version of equalRange which is optimized for the 
        /// case of there being few or no duplicated keys in the tree.
        eastl::pair<iterator, iterator>             equalRangeSmall(const Key& k);
        eastl::pair<const_iterator, const_iterator> equalRangeSmall(const Key& k) const;

    }; // multiset





    ///////////////////////////////////////////////////////////////////////
    // set
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, typename compare, typename Allocator>
    inline set<Key, compare, Allocator>::set(const allocator_type& allocator)
        : base_type(allocator)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline set<Key, compare, Allocator>::set(const compare& compare, const allocator_type& allocator)
        : base_type(compare, allocator)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline set<Key, compare, Allocator>::set(const this_type& x)
        : base_type(x)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    template <typename Iterator>
    inline set<Key, compare, Allocator>::set(Iterator itBegin, Iterator itEnd)
        : base_type(itBegin, itEnd, compare(), EASTL_SET_DEFAULT_ALLOCATOR)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::size_type
    set<Key, compare, Allocator>::erase(const Key& k)
    {
        const iterator it(find(k));

        if(it != end()) // If it exists...
        {
            base_type::erase(it);
            return 1;
        }
        return 0;
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::iterator
    set<Key, compare, Allocator>::erase(iterator position)
    {
        // We need to provide this version because we override another version 
        // and C++ hiding rules would make the base version of this hidden.
        return base_type::erase(position);
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::iterator
    set<Key, compare, Allocator>::erase(iterator first, iterator last)
    {
        // We need to provide this version because we override another version 
        // and C++ hiding rules would make the base version of this hidden.
        return base_type::erase(first, last);
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::size_type
    set<Key, compare, Allocator>::count(const Key& k) const
    {
        const const_iterator it(find(k));
        return (it != end()) ? (size_type)1 : (size_type)0;
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::reverse_iterator
    set<Key, compare, Allocator>::erase(reverse_iterator position)
    {
        return reverse_iterator(erase((++position).base()));
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename set<Key, compare, Allocator>::reverse_iterator
    set<Key, compare, Allocator>::erase(reverse_iterator first, reverse_iterator last)
    {
        // Version which erases in order from first to last.
        // difference_type i(first.base() - last.base());
        // while(i--)
        //     first = erase(first);
        // return first;

        // Version which erases in order from last to first, but is slightly more efficient:
        return reverse_iterator(erase((++last).base(), (++first).base()));
    }


    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename set<Key, compare, Allocator>::iterator,
                       typename set<Key, compare, Allocator>::iterator>
    set<Key, compare, Allocator>::equalRange(const Key& k)
    {
        // The resulting range will either be empty or have one element,
        // so instead of doing two tree searches (one for lower_bound and 
        // one for upper_bound), we do just lower_bound and see if the 
        // result is a range of size zero or one.
        const iterator itLower(lower_bound(k));

        if((itLower == end()) || mcompare(k, *itLower)) // If at the end or if (k is < itLower)...
            return eastl::pair<iterator, iterator>(itLower, itLower);

        iterator itUpper(itLower);
        return eastl::pair<iterator, iterator>(itLower, ++itUpper);
    }
    

    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename set<Key, compare, Allocator>::const_iterator, 
                       typename set<Key, compare, Allocator>::const_iterator>
    set<Key, compare, Allocator>::equalRange(const Key& k) const
    {
        // See equalRange above for comments.
        const const_iterator itLower(lower_bound(k));

        if((itLower == end()) || mcompare(k, *itLower)) // If at the end or if (k is < itLower)...
            return eastl::pair<const_iterator, const_iterator>(itLower, itLower);

        const_iterator itUpper(itLower);
        return eastl::pair<const_iterator, const_iterator>(itLower, ++itUpper);
    }





    ///////////////////////////////////////////////////////////////////////
    // multiset
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, typename compare, typename Allocator>
    inline multiset<Key, compare, Allocator>::multiset(const allocator_type& allocator)
        : base_type(allocator)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline multiset<Key, compare, Allocator>::multiset(const compare& compare, const allocator_type& allocator)
        : base_type(compare, allocator)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline multiset<Key, compare, Allocator>::multiset(const this_type& x)
        : base_type(x)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    template <typename Iterator>
    inline multiset<Key, compare, Allocator>::multiset(Iterator itBegin, Iterator itEnd)
        : base_type(itBegin, itEnd, compare(), EASTL_MULTISET_DEFAULT_ALLOCATOR)
    {
        // Empty
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::size_type
    multiset<Key, compare, Allocator>::erase(const Key& k)
    {
        const eastl::pair<iterator, iterator> range(equalRange(k));
        const size_type n = (size_type)eastl::distance(range.first, range.second);
        base_type::erase(range.first, range.second);
        return n;
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::iterator
    multiset<Key, compare, Allocator>::erase(iterator position)
    {
        // We need to provide this version because we override another version 
        // and C++ hiding rules would make the base version of this hidden.
        return base_type::erase(position);
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::iterator
    multiset<Key, compare, Allocator>::erase(iterator first, iterator last)
    {
        // We need to provide this version because we override another version 
        // and C++ hiding rules would make the base version of this hidden.
        return base_type::erase(first, last);
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::size_type
    multiset<Key, compare, Allocator>::count(const Key& k) const
    {
        const eastl::pair<const_iterator, const_iterator> range(equalRange(k));
        return (size_type)eastl::distance(range.first, range.second);
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::reverse_iterator
    multiset<Key, compare, Allocator>::erase(reverse_iterator position)
    {
        return reverse_iterator(erase((++position).base()));
    }


    template <typename Key, typename compare, typename Allocator>
    inline typename multiset<Key, compare, Allocator>::reverse_iterator
    multiset<Key, compare, Allocator>::erase(reverse_iterator first, reverse_iterator last)
    {
        // Version which erases in order from first to last.
        // difference_type i(first.base() - last.base());
        // while(i--)
        //     first = erase(first);
        // return first;

        // Version which erases in order from last to first, but is slightly more efficient:
        return reverse_iterator(erase((++last).base(), (++first).base()));
    }


    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename multiset<Key, compare, Allocator>::iterator,
                       typename multiset<Key, compare, Allocator>::iterator>
    multiset<Key, compare, Allocator>::equalRange(const Key& k)
    {
        // There are multiple ways to implement equalRange. The implementation mentioned
        // in the C++ standard and which is used by most (all?) commercial STL implementations
        // is this:
        //    return eastl::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
        //
        // This does two tree searches -- one for the lower bound and one for the 
        // upper bound. This works well for the case whereby you have a large container
        // and there are lots of duplicated values. We provide an alternative version
        // of equalRange called equalRangeSmall for cases where the user is confident
        // that the number of duplicated items is only a few.

        return eastl::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }


    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename multiset<Key, compare, Allocator>::const_iterator, 
                       typename multiset<Key, compare, Allocator>::const_iterator>
    multiset<Key, compare, Allocator>::equalRange(const Key& k) const
    {
        // See comments above in the non-const version of equalRange.
        return eastl::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }


    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename multiset<Key, compare, Allocator>::iterator,
                       typename multiset<Key, compare, Allocator>::iterator>
    multiset<Key, compare, Allocator>::equalRangeSmall(const Key& k)
    {
        // We provide alternative version of equalRange here which works faster
        // for the case where there are at most small number of potential duplicated keys.
        const iterator itLower(lower_bound(k));
        iterator       itUpper(itLower);

        while((itUpper != end()) && !mcompare(k, itUpper.mpNode->mValue))
            ++itUpper;

        return eastl::pair<iterator, iterator>(itLower, itUpper);
    }


    template <typename Key, typename compare, typename Allocator>
    inline eastl::pair<typename multiset<Key, compare, Allocator>::const_iterator, 
                       typename multiset<Key, compare, Allocator>::const_iterator>
    multiset<Key, compare, Allocator>::equalRangeSmall(const Key& k) const
    {
        // We provide alternative version of equalRange here which works faster
        // for the case where there are at most small number of potential duplicated keys.
        const const_iterator itLower(lower_bound(k));
        const_iterator       itUpper(itLower);

        while((itUpper != end()) && !mcompare(k, *itUpper))
            ++itUpper;

        return eastl::pair<const_iterator, const_iterator>(itLower, itUpper);
    }



} // namespace eastl


#endif // Header include guard




















