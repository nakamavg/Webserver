#ifndef MYEXCEPTION_HPP
# define MYEXCEPTION_HPP

# include <exception>

class MyException : public std::exception
{
    private:
        const char* _message;
    public:
        MyException(const char* message) : _message(message) {}
        const char* what() const throw()
        {
            return(_message);
        }
};



#endif