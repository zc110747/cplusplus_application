/************************************************************************************
 * std::unordered_set
 * https://www.cplusplus.com/reference/unordered_set/unordered_set/
 * unordered_sets are containers that store unique elements following a specific order.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studyunordered_set
 * 3.More reference shows in function show_unordered_set_help or command "studyunordered_set -h"
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

void show_unordered_set_help(void);
template<class T>
void show_unordered_set(std::unordered_set<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void unordered_set_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::unordered_set<int> intv(array, array+5);
    std::unordered_set<int> userunordered_set(intv);
    std::unordered_set<string> listunordered_set;
    
    //operator=
    listunordered_set = std::unordered_set<string>({"0x00", "0x01", "0x02"});
    
    show_unordered_set(listunordered_set, "listunordered_set");
    //count, clear, size, empty
    //load_factor,max_bucket_count,max_load_factor
    cout<<"count"<<listunordered_set.count("0x01");
    listunordered_set.clear();
    cout<<"size:"<<listunordered_set.size()<<endl;
    cout<<"empty:"<<listunordered_set.empty()<<endl;
    cout<<"load_factor"<<listunordered_set.load_factor()<<endl;
    cout<<"max_bucket_count"<<listunordered_set.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<listunordered_set.max_load_factor()<<endl;

    show_unordered_set(userunordered_set, "userunordered_set");

    //max_size
    cout<<"max_size:"<<userunordered_set.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    std::unordered_set<int>::iterator iterbegin = userunordered_set.begin();
    std::unordered_set<int>::iterator iterend = userunordered_set.end();
    std::unordered_set<int>::const_iterator iterconstbegin = userunordered_set.cbegin();
    std::unordered_set<int>::const_iterator iterconstend = userunordered_set.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<userunordered_set.bucket(5)<<endl;
    auto bucket_count = userunordered_set.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<userunordered_set.bucket_size(i)<<" elements."<<endl;
    }

    //emplace, find, emplace_hint
    auto it = userunordered_set.emplace(15);
    show_unordered_set(userunordered_set, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = userunordered_set.find(8);
    userunordered_set.emplace_hint(find, 9);
    show_unordered_set(userunordered_set, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::unordered_set<int>::const_iterator, std::unordered_set<int>::const_iterator> ret;
    ret = userunordered_set.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;

    //erase
    auto iter=userunordered_set.begin();
    while(iter!=userunordered_set.end())
    {
        if(*iter == 3)
        {
            iter=userunordered_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_unordered_set(userunordered_set, "erase");

    //get_allocator
    int *point = userunordered_set.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userunordered_set.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userunordered_set.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //hash_function
    typedef std::unordered_set<int> stringintMap;
    stringintMap::hasher hash_fn = userunordered_set.hash_function();
    cout<<"file hash value:"<<hash_fn(5)<<endl;

    //insert
    userunordered_set.insert(15);
    show_unordered_set(userunordered_set, "insert");

    //key_eq
    bool equal = userunordered_set.key_eq()(4, 6);
    cout<<"key_eq:"<<equal<<endl;

    //swap
    std::unordered_set<int> swapunordered_set;
    swapunordered_set.swap(userunordered_set);
    show_unordered_set(swapunordered_set, "swapunordered_set");

    //rehash, reserve
    swapunordered_set.rehash(15);
    cout<<"rehash:"<<swapunordered_set.bucket_count()<<endl;
    swapunordered_set.reserve(5);
    cout<<"reserve:"<<swapunordered_set.bucket_count()<<endl;
}

template<class T>
void show_unordered_set(std::unordered_set<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::unordered_set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_unordered_set_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("bucket           Returns the bucket number where the element with key k is located.\n");
    helpstring.append("bucket_count     Returns the number of buckets in the unordered_set container.\n");
    helpstring.append("bucket_size      Returns the number of elements in bucket n.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the unordered_set, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements with a key equivalent to k and returns the number of matches.\n");
    helpstring.append("emplace          Inserts a new element in the unordered_set if its key is unique.\n");
    helpstring.append("emplace_hint     Inserts a new element in the unordered_set if its key is unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the unordered_set is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the unordered_set container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the unordered_set.\n");
    helpstring.append("find             Searches the container for an element with a key equivalent to k and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the unordered_set.\n");
    helpstring.append("hash function    Returns the hash function object used by the unordered_set container.\n");
    helpstring.append("insert           Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.\n");
    helpstring.append("key_eq           Returns the key equivalence comparison predicate used by the unordered_set container.\n");
    helpstring.append("load_factor      Returns the current load factor in the unordered_set container.\n");
    helpstring.append("max_bucket_count Returns the maximum number of buckets that the unordered_set container can have.\n");
    helpstring.append("max_load_factor  Get or set maximum load factor.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the unordered_set container can hold.\n");
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
        show_unordered_set_help();
    else
        unordered_set_study_handle();

    return 0;
} 
