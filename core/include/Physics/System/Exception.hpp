#pragma once
#include <ostream>
///Use this macro as follows: 'throw PHYSICS_EXCEPTION("Something went wrong");'
#define PHYSICS_EXCEPTION(message) physics::Exception(__FUNCTION__, __FILE__, __LINE__, message)

namespace physics
{
    /// @brief Base of this library's exception handling system
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
            :m_Function(function), m_File(file), m_Line(line), m_Message(message)
        {}

        /// @brief Gets information about the exception
        std::string Info() const noexcept
        {
            std::string message = m_Message != "" ? ". Error message: \"" + m_Message + '"' : ""; 
            return m_File + ":" + std::to_string(m_Line) + " -> exception thrown in function "
                + m_Function + message;
        }

        /// @brief Used to append the exception's information to a standard output stream
        /// @return A reference to the resulting ostream so it can be chained with '<<'
        friend std::ostream& operator <<(std::ostream& os, const Exception& exception) noexcept
        {
            os << exception.Info();
            return os;
        }
    private:
        std::string m_Function, m_File, m_Message;
        int m_Line;
    };
}