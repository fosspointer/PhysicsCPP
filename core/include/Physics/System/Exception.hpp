#pragma once
#include <ostream>
///Use this macro as follows: 'throw PHYSICS_EXCEPTION("Something went wrong");'
#define PHYSICS_EXCEPTION(message) physics::Exception(__FUNCTION__, __FILE__, __LINE__, message)

namespace physics
{
    /// @brief Base of the library's exception handling system
    class Exception
    {
    public:
        /// @brief This constructor should NOT be used manually. 
        /// Instead, use the PHYSICS_EXCEPTION macro, indirectly calling
        /// the constructor via the preprocessor
        /// @param function The signature of the function where the exception is thrown
        /// @param file The path to the file throwing the exception
        /// @param line Line where the exception is thrown
        /// @param message Message to be displayed (optional) 
        Exception(const std::string& function, const std::string& file, int line, const std::string& message = "") noexcept
            :m_function(function), m_file(file), m_line(line), m_message(message)
        {}

        /// @brief gets information about the exception
        std::string info() const noexcept
        {
            std::string message = m_message != "" ? ". Error message: \"" + m_message + '"' : ""; 
            return m_file + ":" + std::to_string(m_line) + " -> exception thrown in function "
                + m_function + message;
        }

        /// @brief Used to append the exception's information to a standard output stream
        /// @return A reference to the resulting ostream so it can be chained with '<<'
        friend std::ostream& operator <<(std::ostream& os, const Exception& exception) noexcept
        {
            os << exception.info();
            return os;
        }
    private:
        std::string m_function, m_file, m_message;
        int m_line;
    };
}