//
// Created by colin on 12/1/15.
//

#include <iostream>
#include <algorithm>
#include <string>
#include <functional>
#include <stdexcept>
#include <stdio.h>
#include <gtest/gtest.h>
#include "template_types.h"

struct Date
{
        int year;
        int month;
        int day;
        Date():year(0),month(0),day(0){}
        Date(int y, int m, int d):year(y),month(m),day(d) {}
};

struct YearCmp {
    bool operator()(const Date& d1, const Date& d2) const noexcept {
        return d1.year < d2.year;
    }
};

TEST(TestHeap, UDT)
{
    Heap<Date, YearCmp> dh;
    dh.add(Date(1999,2,3));
    dh.add(Date(1988,2,4));
    dh.add(Date(1902,3,4));
    EXPECT_EQ(dh.top().year, 1902) << "get top is incorrect \n";
}


TEST(TestHeap, TestPop){
    Heap<int> hp {{3,5,1,-3}};
    EXPECT_EQ(hp.top(),-3);
    hp.pop();
    EXPECT_EQ(hp.top(),1);
}

TEST(TestHeap, TestAdd){
    Heap<int> h;
    h.add(3);
    h.add(-2);
    EXPECT_EQ(h.top(),-2);
    h.add(-9);
    EXPECT_EQ(h.top(),-9);
}
TEST(TestMaxHeap,Add){
    MaxHeap<int> mh;
    mh.add(4);

    mh.add(8);
    mh.add(9);
    EXPECT_EQ(mh.top(),9);
}


TEST(TestMedianHeap,Median)
{
    MedianHeap<int> h;
    h.add(2);
    EXPECT_EQ(2,h.getMedian());
    h.add(9);
    EXPECT_EQ(h.getMedian(),5);
    h.add(7);
    EXPECT_EQ(7,h.getMedian());

}

TEST(TestMedianHeap, doubleMedian)
{
    
    MedianHeap<double> sh;
    sh.add(2.3);
    EXPECT_DOUBLE_EQ(sh.getMedian(),2.3);
    sh.add(4.5);
    sh.add(8.5);
    EXPECT_DOUBLE_EQ(sh.getMedian(),4.5);


    //EXPECT_THROW(sh.getMedian(), std::invalid_argument);
}



class TestBSt: public ::testing::Test
{
protected:
    BinarySearchTree<int,int> tree;
    BinarySearchTree<std::string, std::string> dict;
    virtual void SetUp()
    {
        tree.put(1,2);
        tree.put(2,4);
        tree.put(3,6);
        dict.put("name","colin");
        dict.put("lastname","lin");
        dict.put("hobby","run");
    }
    
    virtual void TearDown()
    {

    } 
    
};


TEST_F(TestBSt, remove)
{
    tree.remove(1);
    EXPECT_EQ(tree.get(1), nullptr);
}

TEST_F(TestBSt, strKey)
{
    auto pStr = dict.get("name");
    EXPECT_STREQ(pStr->c_str(), "colin");
}

TEST_F(TestBSt, putOp)
{
    int* v = tree.get(3);
    EXPECT_EQ(6, *v);
    tree.put(1,99);
    EXPECT_EQ(*tree.get(1),99);
}

TEST_F(TestBSt, getOp)
{
    EXPECT_EQ(*tree.get(1),2);
    EXPECT_EQ(*tree.get(2),4);
    EXPECT_EQ(*tree.get(3),6);
}




void testBST()
{
    BinarySearchTree<int,int> bst;
    bst.put(1,2);
     bst.put(2,4);
}


class TestBSTIter: public ::testing::Test {
protected:
     BinarySearchTree<int,int> tree;
     virtual void SetUp()
    {
        tree.put(1,2);
        tree.put(2,4);
        tree.put(3,6);
        tree.put(-1,1);
        tree.put(-2,2);
    }
    
    virtual void TearDown()
    {

    } 
};


TEST_F(TestBSTIter, testMove)
{
    
    BSTIterator<int,int> iter(tree.getRoot());
    EXPECT_EQ(*iter.nextKey() , -2);
    EXPECT_EQ(*iter.nextKey(),-1);
    EXPECT_EQ(*iter.nextKey(),1);
}


void foo()
{
    BinarySearchTree<int,int> tree;
    tree.put(-3,2);
    tree.put(-4,5);
    tree.put(8,2);
    BSTIterator<int,int> iter(tree.getRoot());

    while (iter.hasNext())
        printf("key: %d\n", iter.next()->key );

}


int main(int argc, char** argv){

    ::testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();

    // testBST();
//    foo();
    return 0;

}
