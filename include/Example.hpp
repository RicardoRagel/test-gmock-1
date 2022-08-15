#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

namespace Example
{

    // Simple class to test EXPECT_THAT and Matchers with classes
    class MyClass
    {
        public:
            MyClass(int data) : m_public_data( data ), m_private_data(data)
            {

            }

            int m_public_data;

            // To get private members, it is necessary a getter method and 
            // it must be const (so google test can not modify nothing inside)
            int getPrivateData() const
            {
                return m_private_data;
            }

        private:

            int m_private_data;
    };

}

#endif // EXAMPLE_HPP