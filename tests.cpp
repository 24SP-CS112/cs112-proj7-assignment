#define CATCH_CONFIG_MAIN
#include "List.h"
#include "catch.hpp"

TEST_CASE("constructors") {
    SECTION("default") {
        List<int> aList;
        REQUIRE(aList.mySize == 0);
        REQUIRE(aList.myFirst == nullptr);
        REQUIRE(aList.myLast == nullptr);
    }
}

TEST_CASE("Node constructors") {
    SECTION("default") {
        List<int>::Node aNode;
        REQUIRE(aNode.myItem == unsigned());
        REQUIRE(aNode.myNext == nullptr);
    }

    SECTION("explicit-value") {
        List<unsigned>::Node n1(11, nullptr);
        REQUIRE(n1.myItem == 11);
        REQUIRE(n1.myNext == nullptr);

        List<unsigned>::Node *n3 = new List<unsigned>::Node(33, nullptr);
        List<unsigned>::Node n2(22, n3);
        REQUIRE(n2.myItem == 22);
        REQUIRE(n2.myNext == n3);
    }
}

TEST_CASE("getFirst/getLast on empty list") {
    List<double> l;
    REQUIRE_THROWS_AS(l.getFirst(), underflow_error);
    REQUIRE_THROWS_AS(l.getLast(), underflow_error);
}

TEST_CASE("prepend") {
    List<int> aList;
    aList.prepend(22);
    REQUIRE(aList.getSize() == 1);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast == aList.myFirst);
    REQUIRE(aList.getFirst() == 22);
    REQUIRE(aList.getLast() == 22);
    REQUIRE(aList.myFirst->myNext == nullptr);

    // prepend to a list containing 1 Item
    aList.prepend(11);
    REQUIRE(aList.getSize() == 2);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast != nullptr);
    REQUIRE(aList.myFirst != aList.myLast);
    REQUIRE(aList.getFirst() == 11);
    REQUIRE(aList.getLast() == 22);
    REQUIRE(aList.myFirst->myNext != nullptr);
    REQUIRE(aList.myLast->myNext == nullptr);

    aList.prepend(0);
    REQUIRE(aList.getSize() == 3);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast != nullptr);
    REQUIRE(aList.getFirst() == 0);
    REQUIRE(aList.getLast() == 22);
    REQUIRE(aList.myFirst->myNext->myItem == 11);
    REQUIRE(aList.myLast->myNext == nullptr);
}

TEST_CASE("append") {
    List<float> aList;
    aList.append(11);
    REQUIRE(aList.getSize() == 1);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast == aList.myFirst);
    REQUIRE(aList.getFirst() == 11);
    REQUIRE(aList.getLast() == 11);
    REQUIRE(aList.myFirst->myNext == nullptr);

    // append to a list containing 1 Item
    aList.append(22);
    REQUIRE(aList.getSize() == 2);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast != nullptr);
    REQUIRE(aList.myFirst != aList.myLast);
    REQUIRE(aList.getFirst() == 11);
    REQUIRE(aList.getLast() == 22);
    REQUIRE(aList.myFirst->myNext != nullptr);
    REQUIRE(aList.myLast->myNext == nullptr);
    REQUIRE(aList.myFirst->myNext == aList.myLast);

    aList.append(33);
    REQUIRE(aList.getSize() == 3);
    REQUIRE(aList.myFirst != nullptr);
    REQUIRE(aList.myLast != nullptr);
    REQUIRE(aList.getFirst() == 11);
    REQUIRE(aList.getLast() == 33);
    REQUIRE(aList.myFirst->myNext->myItem == 22);
    REQUIRE(aList.myLast->myNext == nullptr);
}

TEST_CASE("destructor") {
    List<int> l;
    l.prepend(33);
    l.prepend(22);
    l.prepend(11);
    l.~List();
    // Risky getting values from an already-freed object...
    REQUIRE(l.getSize() == 0);
    REQUIRE(l.myFirst == nullptr);
    REQUIRE(l.myLast == nullptr);

    // Tester should check for memory leaks... somehow...
}

TEST_CASE("writeTo") {
    List<unsigned> l0;
    l0.prepend(33);
    l0.prepend(22);
    l0.prepend(11);
    l0.writeTo(cout);
    cout << endl << "Check that output looks like this: 11 22 33" << endl;
}

TEST_CASE("getIndexOf") {
    List<short> l;
    SECTION("empty list") {
        REQUIRE(l.getIndexOf(11) == -1);
    }
    SECTION("1-item list") {
        l.prepend(11);
        REQUIRE(l.getIndexOf(11) == 0);
        REQUIRE(l.getIndexOf(22) == -1);
    }
    SECTION(">1-item list") {
        l.prepend(11);
        l.append(22);
        l.append(33);
        l.append(44);
        REQUIRE(l.getIndexOf(11) == 0);
        REQUIRE(l.getIndexOf(22) == 1);
        REQUIRE(l.getIndexOf(33) == 2);
        REQUIRE(l.getIndexOf(44) == 3);
        REQUIRE(l.getIndexOf(55) == -1);
    }
}

TEST_CASE("copy constructor") {
    SECTION("empty list") {
        List<int> l0;
        List<int> l1(l0);
        REQUIRE(l1.getSize() == 0);
        REQUIRE(l1.myFirst == nullptr);
        REQUIRE(l1.myLast == nullptr);
    }

    SECTION("1-item list") {
        List<int> l0;
        l0.prepend(11);
        List<int> l1(l0);
        REQUIRE(l1.getSize() == 1);
        // check for deep copy
        REQUIRE(l1.myFirst != l0.myFirst);
        REQUIRE(l1.myLast != l0.myLast);
        REQUIRE(l1.myFirst != nullptr);
        REQUIRE(l1.myLast != nullptr);
        REQUIRE(l1.myLast->myNext == nullptr);
        REQUIRE(l1.myFirst->myItem == 11);
    }

    SECTION(">1-item list") {
        List<short> l0;
        l0.prepend(11);
        l0.append(22);
        l0.append(33);
        l0.append(44);
        List<short> l1(l0);
        // Using getIndexOf is a good way to test if the list is correctly formed.
        REQUIRE(l1.getIndexOf(11) == 0);
        REQUIRE(l1.getIndexOf(22) == 1);
        REQUIRE(l1.getIndexOf(33) == 2);
        REQUIRE(l1.getIndexOf(44) == 3);
        REQUIRE(l1.getIndexOf(55) == -1);
        // check deep copy
        REQUIRE(l1.myFirst != l0.myFirst);
        REQUIRE(l1.myLast != l0.myLast);
    }
}

TEST_CASE("equality") {
    SECTION("empty lists") {
        List<unsigned short> l0;
        List<unsigned short> l1;
        REQUIRE(l0 == l1);
    }

    SECTION("non-empty lists") {
        List<unsigned short> l0;
        l0.prepend(11);
        List<unsigned short> l1(l0);  // use copy constructor
        REQUIRE(l0 == l1);
        l0.prepend(8);
        REQUIRE(!(l0 == l1));   // different lengths
        l1.prepend(8);
        REQUIRE(l0 == l1);
        // same length, different values
        l0.append(22);
        l1.append(23);
        REQUIRE(!(l0 == l1));
    }
}
