#pragma once
#include <string>

namespace physics
{
    static std::string stringToLower(std::string str)
    {
        for(auto& c: str)
            c = tolower(c);
        return str;
    }

    class Material
    {
    public:
        using Pair = std::pair<Material, Material>;
        Material(const std::string& name)
            :m_Name(stringToLower(name))
        {}

        Material(const std::string& name, float density)
            :m_Name(stringToLower(name)), m_Density(density)
        {}

        Material* SetDensity(float density)
        {
            m_Density = density;
            return this;
        }

        operator int() const
        {
            return int(m_Density * 10);
        }

        inline float GetDensity() const { return m_Density; }
        const std::string& GetName() const { return m_Name; }

    private:
        const std::string m_Name;
        float m_Density{1.0f};
    };
    
    class Materials
    {
    public:
        static Material* Get(const std::string& name)
        {
            auto f = s_Materials.find(name);

            if(f != s_Materials.end())
                return f->second;
            else
                return s_Materials[name] = new Material(name);
        }
    private:
        static std::unordered_map<std::string, Material*> s_Materials;
    };
}