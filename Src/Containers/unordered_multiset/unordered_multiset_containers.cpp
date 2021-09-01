/************************************************************************************
 * std::unordered_multiset
 * reference:http://www.cplusplus.com/reference/unordered_set/unordered_multiset/
 * unordered_multisets are containers that store unique elements following a specific order.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlunordered_multiset
 * 3.More reference shows in function show_unordered_multiset_help or command "studyunordered_multiset -h"
***********************************************************************************/
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;

typedef enum
{
    SHOW_MODE_ENUM_LAMBDA = 0,
    SHOW_MODE_ENUM_TYPE_AUTO,
    SHOW_MODE_ENUM_TYPE_ITERATOR,
}SHOW_MODE_ENUM;

void show_unordered_multiset_help(void);
template<class T>
void show_unordered_multiset(std::unordered_multiset<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void unordered_multiset_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::unordered_multiset<int> intv(array, array+5);
    std::unordered_multiset<int> userunordered_multiset(intv);
    std::unordered_multiset<string> listunordered_multiset;
    
    //operator=
    listunordered_multiset = std::unordered_multiset<string>({"0x00", "0x01", "0x02"});
    
    show_unordered_multiset(listunordered_multiset, "listunordered_multiset");
    //count, clear, size, empty
    //load_factor,max_bucket_count,max_load_factor
    cout<<"count"<<listunordered_multiset.count("0x01");
    listunordered_multiset.clear();
    cout<<"size:"<<listunordered_multiset.size()<<endl;
    cout<<"empty:"<<listunordered_multiset.empty()<<endl;
    cout<<"load_factor"<<listunordered_multiset.load_factor()<<endl;
    cout<<"max_bucket_count"<<listunordered_multiset.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<listunordered_multiset.max_load_factor()<<endl;

    show_unordered_multiset(userunordered_multiset, "userunordered_multiset");

    //max_size
    cout<<"max_size:"<<userunordered_multiset.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    std::unordered_multiset<int>::iterator iterbegin = userunordered_multiset.begin();
    std::unordered_multiset<int>::iterator iterend = userunordered_multiset.end();
    std::unordered_multiset<int>::const_iterator iterconstbegin = userunordered_multiset.cbegin();
    std::unordered_multiset<int>::const_iterator iterconstend = userunordered_multiset.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<userunordered_multiset.bucket(5)<<endl;
    auto bucket_count = userunordered_multiset.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<userunordered_multiset.bucket_size(i)<<" elements."<<endl;
    }

    //emplace, find, emplace_hint
    auto it = userunordered_multiset.emplace(15);
    show_unordered_multiset(userunordered_multiset, "emplace");
    cout<<"emplace:"<<*it<<endl;
    auto find = userunordered_multiset.find(8);
    userunordered_multiset.emplace_hint(find, 9);
    show_unordered_multiset(userunordered_multiset, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::unordered_multiset<int>::const_iterator, std::unordered_multiset<int>::const_iterator> ret;
    ret = userunordered_multiset.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;

    //erase
    auto iter=userunordered_multiset.begin();
    while(iter!=userunordered_multiset.end())
    {
        if(*iter == 3)
        {
            iter=userunordered_multiset.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_unordered_multiset(userunordered_multiset, "erase");

    //get_allocator
    int *point = userunordered_multiset.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userunordered_multiset.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userunordered_multiset.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //hash_function
    typedef std::unordered_multiset<int> stringintMap;
    stringintMap::hasher hash_fn = userunordered_multiset.hash_function();
    cout<<"file hash value:"<<hash_fn(5)<<endl;

    //insert
    userunordered_multiset.insert(15);
    show_unordered_multiset(userunordered_multiset, "insert");

    //key_eq
    bool equal = userunordered_multiset.key_eq()(4, 6);
    cout<<"key_eq:"<<equal<<endl;

    //swap
    std::unordered_multiset<int> swapunordered_multiset;
    swapunordered_multiset.swap(userunordered_multiset);
    show_unordered_multiset(swapunordered_multiset, "swapunordered_multiset");

    //rehash, reserve
    swapunordered_multiset.rehash(15);
    cout<<"rehash:"<<swapunordered_multiset.bucket_count()<<endl;
    swapunordered_multiset.reserve(5);
    cout<<"reserve:"<<swapunordered_multiset.bucket_count()<<endl;
}

template<class T>
void show_unordered_multiset(std::unordered_multiset<T> value, string qstring, SHOW_MODE_ENUM mode)
{
    //empty, size
    if(!qstring.empty())
    {
        for(auto index=qstring.size(); index<13; index++)
            qstring.push_back(' ');
        qstring += ":";
        cout<<qstring;
    }

    //begin, end
    if(mode == SHOW_MODE_ENUM_LAMBDA)
    {
        std::for_each(value.begin(), value.end(), [](const T &ref_value){
            cout<<ref_value<<" ";
        });
    }
    //auto
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref : value)
            cout<<ref<<" ";
    }
    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::unordered_multiset<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_unordered_multiset_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("bucket           Returns the bucket number where the element with key k is located.\n");
    helpstring.append("bucket_count     Returns the number of buckets in the unordered_multiset container.\n");
    helpstring.append("bucket_size      Returns the number of elements in bucket n.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the unordered_multiset, and container size is zero.\n");
    helpstring.append("count            Searches the container for elements with a key equivalent to k and returns the number of matches.\n");
    helpstring.append("emplace          Inserts a new element in the unordered_multiset if its key is unique.\n");
    helpstring.append("emplace_hint     Inserts a new element in the unordered_multiset if its key is unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the unordered_multiset is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the unordered_multiset container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the unordered_multiset.\n");
    helpstring.append("find             Searches the container for an element with a key equivalent to k and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the unordered_multiset.\n");
    helpstring.append("hash function    Returns the hash function object used by the unordered_multiset container.\n");
    helpstring.append("insert           Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.\n");
    helpstring.append("key_eq           Returns the key equivalence comparison predicate used by the unordered_multiset container.\n");
    helpstring.append("load_factor      Returns the current load factor in the unordered_multiset container.\n");
    helpstring.append("max_bucket_count Returns the maximum number of buckets that the unordered_multiset container can have.\n");
    helpstring.append("max_load_factor  Get or set maximum load factor.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the unordered_multiset container can hold.\n");
    helpstring.append("operator=        Assigns new contents to the container, replacing its current content.\n");
    helpstring.append("rehash            Sets the number of buckets in the container to n or more.\n");
    helpstring.append("reserve          Sets the number of buckets in the container (bucket_count) to the most appropriate to contain at least n elements.\n");
    helpstring.append("size             Returns number of elements in the container.\n");    
    helpstring.append("swap             Exchange the content of the containder.\n");

    cout<<helpstring;
}

int main(int argc, char* argv[])
{
    int opt, mode(0);

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
        case 'h':
            mode = 1;
            break;
        
        default:

            break;
        }
    }

    if(mode == 1)
        show_unordered_multiset_help();
    else
        unordered_multiset_study_handle();

    return 0;
} 
