//
// Created by colin on 11/20/15.
//

#ifndef MYDATASTRUCTS_TEMPLATE_TYPES_H
#define MYDATASTRUCTS_TEMPLATE_TYPES_H

#include <initializer_list>
#include <vector>
#include <iostream>
#include <functional>
#include <type_traits>
#include <stack>
#include <cassert>
#include <memory>
#include <stdexcept>

#define PARENT(i) ((i)>>1)
#define LCHILD(i) ((i)<<1)

template<typename TKey,typename Comparer=std::less<TKey> >
class Heap {
public:
    /** default constructor*/

    
    Heap(const std::initializer_list <TKey> &list) {
         _init();
        for(auto key: list)
            add(key);
    }
    Heap(){
        _init();
    }

    ///template <typename TKey>
    friend
    void heapSwap(Heap<TKey,Comparer>& h1, Heap<TKey,Comparer> & h2);

    Heap<TKey>& operator=(Heap<TKey> other){
        heapSwap(*this, other);
        return *this;
    }


    void add(const TKey& key)
    {
        elems.push_back(key);
        int N = elems.size()-1;
        //std::cout<<"N="<<N<<" \n";
        _swim(N);
        //std::cout<<"\n[DEBUG] print content: ";
       // _print();

    }

    TKey top() const
    {
        assert(elems.size()>1);
        return elems.at(1);
    }

    size_t size() const
    {

        return elems.size()-1;
    }
    TKey pop()
    {
        TKey top = elems.at(1);
        int N = elems.size()-1;
        _swap(1,N);
        elems.pop_back();
        _sink(1);
        return top;

    }

    bool empty() const 
    {
        return elems.size() == 1;
    }

    void poppush(TKey key)
    {
        this->pop();
        this->add(key);
    }


private:

    ///size_t N; // index of tail element
    std::vector<TKey> elems;

    void _print(){
        for(int i=1;i < elems.size();i++)
            std::cout<<elems.at(i)<<",";
    }

    /**
     * disuse zero index element
     */
    void _init() {
        //leave index 0 unused
        TKey _ = TKey();
        elems.push_back(_);
    }

    void _swap(int i, int j){
        assert(i >= 0 && i < elems.size());
        assert(j >= 0 && j < elems.size());
        TKey t = elems[i];
        elems[i] = elems[j];
        elems[j] = t;
    }


    void _swim(int i){
        int p;
        for (int j = i; j > 1 && less(j,(p=PARENT(j))) ; j = p ) {
            _swap(j,p);
        }
    }

    void _sink(int i){
        int c;
        int N = elems.size()-1;
        for (int j = i; (c = LCHILD(j) ) <= N ; j =c ) {
            if(c+1 <= N && less(c+1,c))
                c++;
            if(less(j,c) )
                break;
            //else swap and continue
            _swap(c,j);
        }
    }
    
    bool less(int me, int other){
        Comparer comp;
        return comp(elems.at(me), elems.at(other)); 
    }


};

 template<typename TKey, typename Comparer>
 void heapSwap(Heap<TKey,Comparer>& h1, Heap<TKey,Comparer> & h2)
{
    h1.elems.swap(h2.elems);
}


template <typename T>
using MaxHeap = Heap<T, std::greater<T> >;

template <typename T>
using MinHeap = Heap<T, std::less<T> >;


template <typename TKey, typename Less=std::less<TKey>, typename Greater=std::greater<TKey> >
class MedianHeap
{
    public:
        MedianHeap()
        {
            assert(minheap.empty()&&maxheap.empty());
        }

        size_t size() const
        {
            return minheap.size() + maxheap.size();
        }
        void add(TKey key)
        {
        auto N = size();
         
        if(N % 2 == 0)
        {
            //right now the maxhp and minhp have equal #keys
            maxheap.add(key);
            if (minheap.empty())
                return;
            if(maxheap.top()>minheap.top() ){
                auto toMin = maxheap.pop();
                auto toMax = minheap.pop();
                minheap.add(toMin);
                maxheap.add(toMax);
            
            }
        }
        else
        //maxheap has one more key, add this to minhp
            minheap.add(key);
         

        }

        friend
        std::istream& operator>>(std::istream& is, MedianHeap<TKey,Less,Greater> & medianhp)
        {
            TKey e;
            is >> e;
            medianhp.add(e);
            return is;
        }
         

        typename std::enable_if<std::is_arithmetic<TKey>::value, TKey>::type 
        getMedian() const 
        {

            auto N = size();


            if(N % 2 == 0 )
                return (minheap.top() + maxheap.top() )/2 ;
            else
                return maxheap.top();
        }

        

    private:
        Heap<TKey,Less> minheap;
        Heap<TKey,Greater> maxheap;


};

////////////////////////////////////////////////////////////////////
/////////////////////// INTERFACES //////////////////////////
////////////////////////////////////////////////////////////////////

template<typename TKey, typename TVal>
class IMap
{
public:
    virtual void put(TKey key, TVal val) = 0;
    virtual TVal* get(TKey key) = 0;
    virtual void remove(TKey key) = 0;
    virtual bool contains(TKey key) = 0;
    virtual size_t size() const = 0 ;
    virtual bool empty() const = 0;

};

template<typename TItem>
class IIterator {
 
public:
    virtual bool moveNext() = 0;
    virtual TItem* current() = 0;
    virtual void reset()=0;
};





#define GET_RAW_PTR(sptr) (sptr)

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/////////////// Binary search Tree ////////////////////////////////////


//!!BUGGY: Pointer-caused segfault 
template<typename TKey, typename TVal >
class BinarySearchTree : public IMap<TKey,TVal> 
{
private:
    std::function<int(TKey,TKey)> comp;
    
    struct Node
    {
        TKey key;
        TVal value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        size_t size; // #nodes in this subtree
        Node(TKey k, TVal v): size(1),key(k),value(v),left(nullptr),right(nullptr){}


    };
    typedef std::shared_ptr<Node> PNode;

    PNode pRoot;

    int compareTo(TKey k1, TKey k2)
    {
        return comp(k1,k2);
    }

    size_t rsize(PNode x) const
    {
        return x ? x->size : 0 ;
    }

    PNode rput(PNode x, TKey k, TVal v)
    {
        if(!x) {
            return PNode(new Node(k,v));
        }
        int cmp = compareTo(k, x->key);
        if(cmp < 0) x->left = rput(GET_RAW_PTR(x->left) , k, v);
        else if (cmp > 0) x->right = rput(GET_RAW_PTR(x->right) , k, v);
        else x->value = v;
        x->size = rsize(GET_RAW_PTR(x->left)) + rsize(GET_RAW_PTR(x->right)) + 1;
        return PNode(x) ; 
    }

    TVal* rget(PNode x, TKey k)
    {
        if(!x) return nullptr;
        int cmp = compareTo(k, x->key);
        if(cmp < 0 ) return rget(GET_RAW_PTR(x->left), k);
        else if (cmp > 0) return rget(GET_RAW_PTR(x->right),k);
        else return &x->value;
    }

    PNode rmin(PNode x)
    {
        if(!x->left) {
            return x;
        }
        return rmin(x->left);
    }
    /**
     * resursive-select key of rank k (k keys less than me)
     * @param  x [root node]
     * @param  k [rank]
     * @return   [the node of rank k]
     */
    PNode rselect(PNode x, int k)
    {
        if(!x ) return nullptr;
        int t = rsize(x->left);
        if(t > k) return rselect(x->left, k);
        else if (t < k) return rselect(x->right, k-t-1);
        else return x;
    }

    int rrank(PNode x, TKey key)
    {
        if(!x) return 0;
        int cmp = compareTo(key, x->key);
        if(cmp < 0) {
            return rrank(x->left, key);
        }
        else if(cmp > 0) {
            return 1 + rsize(x->left) + rrank(x->right, key);
        }
        else {
            return rsize(x->left);
        }
    }

    PNode rremoveMin(PNode x)
    {
        if(! x->left) return x->right;
        x->left = rremoveMin(x->left);
        x->size = rsize(x->left) + rsize(x->right) + 1;
        return x;
    }


    PNode rremove(PNode x, TKey key)
    {
        if(!x) return nullptr;
        int cmp = compareTo(key, x->key);
        if(cmp < 0) x->left = rremove(x->left, key);
        else if (cmp > 0) x->right = rremove(x->right, key);
        else{
            if (!x->right) return x->left;
            if(!x->left) return x->right;
            PNode t = x;
            x = rmin(t->right); // x is replaced with its successor 
            x->right = rremoveMin(t->right);
            x->left = t->left;
        }

        x->size = rsize(x->left) + rsize(x->right) + 1;
        return x; 
    }

public:
    BinarySearchTree():pRoot(nullptr){
        comp = [](TKey k1, TKey k2)->int {
            if (k1 < k2) return -1 ;
            if (k1 == k2 ) return 0;
            if (k1 > k2 ) return 1;
        };

    }
    /**
     * pass in a comparator on construction
     */
    explicit BinarySearchTree(std::function<int(TKey,TKey)> arg_comp):comp(arg_comp),pRoot(nullptr){ }


    /**
     * put a k-v pair
     * @param key key 
     * @param val value
     */
    virtual void put(TKey key, TVal val)  override
    {
        pRoot = rput(GET_RAW_PTR(pRoot), key, val);
    }
    virtual TVal* get(TKey key) override
    {
        return rget(GET_RAW_PTR(pRoot), key);
    }

    TKey select(int k)
    {
        if(!pRoot){
            throw std::invalid_argument("root node is null\n");
        }

        return rselect(pRoot,k)->key;
    }

    int rank(TKey key)
    {
        return rrank(pRoot, key);
    }

    void removeMin()
    {
        pRoot = rremoveMin(pRoot);
    }


    virtual void remove(TKey key) override
    {
        pRoot = rremove(pRoot,key);
    }

    virtual bool contains(TKey key) override
    {
        return get(key) != nullptr ;
    }

    virtual size_t size() const override
    {
        return rsize(GET_RAW_PTR(pRoot));
    }
    ;
    virtual bool empty() const 
    {
        return pRoot == nullptr; 
    }

};





#endif //MYDATASTRUCTS_TEMPLATE_TYPES_H
