#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Example.hpp"

// Define some matchers
using ::testing::StartsWith;
using ::testing::MatchesRegex;
using ::testing::StrNe;
using ::testing::Eq;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::IsTrue;
using ::testing::FloatEq;
using ::testing::FloatNear;
using ::testing::IsNull;
using ::testing::IsNan;
using ::testing::Ref;
using ::testing::AllOf;
using ::testing::Each;
using ::testing::Conditional;
using ::testing::HasSubstr;
using ::testing::Field;
using ::testing::Property;
using ::testing::Truly;

// EXPECT_THAT: Allow to use gMock Matchers in the gTest Assertions:
TEST(MatchersTest, ExpectThatTest)
{
    EXPECT_THAT(1, 1);  // Not actually using Matchers, EXPECT_THAT(actual_value, expected_value)

    testing::Matcher<int> is_one = 1;
    EXPECT_THAT(1, is_one); // EXPECT_THAT(actual_value, matcher)

    std::string test_string = "This is a test";
    EXPECT_THAT(test_string, StartsWith("This"));
    EXPECT_THAT(test_string, MatchesRegex(".*test"));

    std::string test_another_string = "This is another test";
    EXPECT_THAT(test_another_string, StrNe(test_string));
    
    int test_value = 10;
    EXPECT_THAT(test_value, Eq(10));
    EXPECT_THAT(test_value, AllOf(Gt(0), Lt(100))); // Notice the compound matcher AllOf(). Other are AnyOff() or Not(AllOf())

    bool test_bool = true;
    EXPECT_THAT(test_bool, IsTrue());

    float a = 67.23456;
    float b = 67.23457; // A diff of 0.00001 is the error limit
    EXPECT_THAT(a, FloatEq(b));
    EXPECT_THAT(a, FloatNear(b, 0.00002));

    int* test_ptr = nullptr;
    EXPECT_THAT(test_ptr, IsNull());

    double test_nan = std::log(-1); // it returns NaN
    EXPECT_THAT(test_nan, IsNan());

    int& test_ref = test_value;
    EXPECT_THAT(test_ref, Ref(test_value));

    bool select_the_first_checking = true; // if false, select the second checking option
    EXPECT_THAT(test_string, Conditional(select_the_first_checking, HasSubstr("This"), HasSubstr("THIS")));

}

//-----------------------------------------------------------------------------------------------------
// Matcher for class members
TEST(MatchersTest, ClassMemberMatcherTest)
{
    Example::MyClass my_obj(1);
    
    // Accessing public data directly by Field()
    EXPECT_THAT(my_obj, Field(&Example::MyClass::m_public_data, Gt(0)));

    // Accessing private data through getters by Property
    EXPECT_THAT(my_obj, Property(&Example::MyClass::getPrivateData, Gt(0)));
}

//-----------------------------------------------------------------------------------------------------
// Custom Matcher:
//  Use MATCHER    (matcher_name, "description") to define it
//  Use MATCHER_P  (matcher_name, matcher_arg, "description") to define it
//  Use MATCHER_P2 (matcher_name, matcher_arg_1, matcher_arg_2, "description") to define it
//  ...
//  Use arg to get the EXPECT_THAT actual_value 
MATCHER_P(IsDivisibleBy, n, "")
{
    // *result_listener << "write here more info about the failure if you want";
    return ( arg % n ) == 0;
}
TEST(MatchersTest, CustomMatcherTest)
{
    EXPECT_THAT(10, IsDivisibleBy(2));
    
    // Another advance matcher: Each(), to check each one of the elements of the actual_value
    std::vector<int> v = {2, 14, 6, 8};
    EXPECT_THAT(v, Each(IsDivisibleBy(2)));
}
// TEST(MatchersTest, CustomMatcherTestFailure)
// {
//     EXPECT_THAT(9, IsDivisibleBy(2));
// }

//-----------------------------------------------------------------------------------------------------
// GMock Trully
//  It allows us to pass multiple values of our arg to our predicate function
bool isPositive(int x) { return x > 0; }
TEST(MatchersTest, TrulyTest)
{
    // Remembering:
    EXPECT_TRUE(isPositive(10));    // that's ok for test just a value
    EXPECT_PRED1(isPositive, 10);   // that's another option that is ok
    
    // But what if we want to test it non only for 10.
    std::vector<int> v = {1,2,3,4,5};
    EXPECT_THAT(v, Each(Truly(isPositive)));
}