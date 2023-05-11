#pragma once
#include <SFML/Graphics.hpp>
#include <variant>

//Base units

#define Decimeters Deci
#define Centimeters Centi
#define Millimeters Milli
#define Micrometers Micro
#define Nanometers Nano
#define Picometers Pico
#define Femtometers Femto
#define Decameters Deca
#define Hectometers Hecto
#define Kilometers Kilo
#define Megameters Mega
#define Gigameters Giga
#define Terameters Tera
#define Petameters Peta
#define Meters Ones 

#define Deciseconds Deci
#define Centiseconds Centi
#define Milliseconds Milli
#define Microseconds Micro
#define Nanoseconds Nano
#define Picoseconds Pico
#define Femtoseconds Femto
#define Decaseconds Deca
#define Hectoseconds Hecto
#define Kiloseconds Kilo
#define Megaseconds Mega
#define Gigaseconds Giga
#define Teraseconds Tera
#define Petaseconds Peta
#define Seconds Ones 

#define Decigrams Deci
#define Centigrams Centi
#define Milligrams Milli
#define Micrograms Micro
#define Nanograms Nano
#define Picograms Pico
#define Femtograms Femto
#define Decagrams Deca
#define Hectograms Hecto
#define Kilograms Kilo
#define Megagrams Mega
#define Gigagrams Giga
#define Teragrams Tera
#define Petagrams Peta
#define Grams Ones 

#define Deciamperes Deci
#define Centiamperes Centi
#define Milliamperes Milli
#define Microamperes Micro
#define Nanoamperes Nano
#define Picoamperes Pico
#define Femtoamperes Femto
#define Decaamperes Deca
#define Hectoamperes Hecto
#define Kiloamperes Kilo
#define Megaamperes Mega
#define Gigaamperes Giga
#define Teraamperes Tera
#define Petaamperes Peta
#define Amperes Ones 

#define Decimoles Deci
#define Centimoles Centi
#define Millimoles Milli
#define Micromoles Micro
#define Nanomoles Nano
#define Picomoles Pico
#define Femtomoles Femto
#define Decamoles Deca
#define Hectomoles Hecto
#define Kilomoles Kilo
#define Megamoles Mega
#define Gigamoles Giga
#define Teramoles Tera
#define Petamoles Peta
#define Moles Ones 

#define Decicandelas Deci
#define Centicandelas Centi
#define Millicandelas Milli
#define Microcandelas Micro
#define Nanocandelas Nano
#define Picocandelas Pico
#define Femtocandelas Femto
#define Decacandelas Deca
#define Hectocandelas Hecto
#define Kilocandelas Kilo
#define Megacandelas Mega
#define Gigacandelas Giga
#define Teracandelas Tera
#define Petacandelas Peta
#define Candelas Ones 

//Derived units

#define Decinewtons Deci
#define Centinewtons Centi
#define Millinewtons Milli
#define Micronewtons Micro
#define Nanonewtons Nano
#define Piconewtons Pico
#define Femtonewtons Femto
#define Decanewtons Deca
#define Hectonewtons Hecto
#define Kilonewtons Kilo
#define Meganewtons Mega
#define Giganewtons Giga
#define Teranewtons Tera
#define Petanewtons Peta
#define Newtons Ones 

#define Decipascals Deci
#define Centipascals Centi
#define Millipascals Milli
#define Micropascals Micro
#define Nanopascals Nano
#define Picopascals Pico
#define Femtopascals Femto
#define Decapascals Deca
#define Hectopascals Hecto
#define Kilopascals Kilo
#define Megapascals Mega
#define Gigapascals Giga
#define Terapascals Tera
#define Petapascals Peta
#define Pascals Ones 

#define Decijoules Deci
#define Centijoules Centi
#define Millijoules Milli
#define Microjoules Micro
#define Nanojoules Nano
#define Picojoules Pico
#define Femtojoules Femto
#define Decajoules Deca
#define Hectojoules Hecto
#define Kilojoules Kilo
#define Megajoules Mega
#define Gigajoules Giga
#define Terajoules Tera
#define Petajoules Peta
#define Joules Ones 

#define Deciwatts Deci
#define Centiwatts Centi
#define Milliwatts Milli
#define Microwatts Micro
#define Nanowatts Nano
#define Picowatts Pico
#define Femtowatts Femto
#define Decawatts Deca
#define Hectowatts Hecto
#define Kilowatts Kilo
#define Megawatts Mega
#define Gigawatts Giga
#define Terawatts Tera
#define Petawatts Peta
#define Watts Ones 

#define Decicoulombs Deci
#define Centicoulombs Centi
#define Millicoulombs Milli
#define Microcoulombs Micro
#define Nanocoulombs Nano
#define Picocoulombs Pico
#define Femtocoulombs Femto
#define Decacoulombs Deca
#define Hectocoulombs Hecto
#define Kilocoulombs Kilo
#define Megacoulombs Mega
#define Gigacoulombs Giga
#define Teracoulombs Tera
#define Petacoulombs Peta
#define Coulombs Ones 


#define Deciohms Deci
#define Centiohms Centi
#define Milliohms Milli
#define Microohms Micro
#define Nanoohms Nano
#define Picoohms Pico
#define Femtoohms Femto
#define Decaohms Deca
#define Hectoohms Hecto
#define Kiloohms Kilo
#define Megaohms Mega
#define Gigaohms Giga
#define Teraohms Tera
#define Petaohms Peta
#define Ohms Ones 


namespace physics
{
    class Value
    {
    public:
        enum Type
        {
            Scalar, Vector
        };

        inline static Value Ones(float value){ return Value(value); }

        inline static Value Deci(float value){ return Value(value * 1e-1f); } 
        inline static Value Centi(float value){ return Value(value * 1e-2f); } 
        inline static Value Milli(float value){ return Value(value * 1e-3f); } 
        inline static Value Micro(float value){ return Value(value * 1e-6f); } 
        inline static Value Nano(float value){ return Value(value * 1e-9f); } 
        inline static Value Pico(float value){ return Value(value * 1e-12f); } 
        inline static Value Femto(float value){ return Value(value * 1e-15f); } 
      
        inline static Value Deca(float value){ return Value(value * 1e+1f); }
        inline static Value Hecto(float value){ return Value(value * 1e+2f); }
        inline static Value Kilo(float value){ return Value(value * 1e+3f); }
        inline static Value Mega(float value){ return Value(value * 1e+6f); }
        inline static Value Giga(float value){ return Value(value * 1e+9f); }
        inline static Value Tera(float value){ return Value(value * 1e+12f); }
        inline static Value Peta(float value){ return Value(value * 1e+15); }

        Value(const Value& other)
            :m_Type(other.m_Type), m_Scalar{0}
        {
            switch(m_Type)
            {
            case Type::Scalar: m_Scalar = other.m_Scalar; return;
            case Type::Vector: m_Vector = other.m_Vector; return;
            }
        }

        Value& operator=(const Value& other)
        {
            *this = Value(other);
            return *this;
        }

        Value operator+(const Value& other)
        {
            if(m_Type != other.m_Type) throw PHYSICS_EXCEPTION("Disallowed operation: scalar-vector addition");

            switch(m_Type)
            {
            case Type::Scalar: return m_Scalar + other.m_Scalar;
            case Type::Vector: return m_Vector + other.m_Vector;
            }
        }

        Value operator-(const Value& other)
        {
            if(m_Type != other.m_Type) throw PHYSICS_EXCEPTION("Disallowed operation: scalar-vector subtraction");

            switch(m_Type)
            {
            case Type::Scalar: return m_Scalar - other.m_Scalar;
            case Type::Vector: return m_Vector - other.m_Vector;
            }
        }

        Value operator*(const Value& other)
        {
            if(m_Type == Type::Vector && other.m_Type == Type::Vector) throw PHYSICS_EXCEPTION("Disallowed operation: vector-vector multiplication");

            switch(m_Type)
            {
            case Type::Scalar:
                switch(other.m_Type)
                {
                case Type::Scalar: return m_Scalar * other.m_Scalar;
                case Type::Vector: return m_Scalar * other.m_Vector;
                }
            case Type::Vector: return m_Vector * other.m_Scalar;
            }
        }

        Value operator/(const Value& other)
        {
            if(other.m_Type == Type::Vector) throw PHYSICS_EXCEPTION("Disallowed operation: devision by vector");

            switch(m_Type)
            {
            case Type::Scalar: return m_Scalar / other.m_Scalar;
            case Type::Vector: return m_Vector / other.m_Scalar;
            }
        }
    private:
        Value(float scalar)
            :m_Scalar(scalar), m_Type(Type::Scalar)
        {}

        Value(float vector_x, float vector_y)
            :m_Vector(sf::Vector2f{vector_x, vector_y}), m_Type(Type::Vector)
        {}

        Value(const sf::Vector2f& vector)
            :m_Vector(sf::Vector2f{vector.x, vector.y}), m_Type(Type::Vector)
        {}

        union
        {
            float m_Scalar;
            sf::Vector2f m_Vector;
        };

        Type m_Type;
        friend class Tools;
    };

}