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
            :m_name(stringToLower(name))
        {}

        Material(const std::string& name, float density)
            :m_name(stringToLower(name)), m_density(density)
        {}

        Material* setDensity(float density)
        {
            m_density = density;
            return this;
        }

        operator int() const
        {
            return int(m_density * 10);
        }

        inline float getDensity() const { return m_density; }
        const std::string& getName() const { return m_name; }

    private:
        const std::string m_name;
        float m_density{1.0f};
    };
    
    class Materials
    {
    public:
        static Material* get(const std::string& name)
        {
            auto f = s_materials.find(name);

            if(f != s_materials.end())
                return f->second;
            else
                return s_materials[name] = new Material(name);
        }
    private:
        static std::unordered_map<std::string, Material*> s_materials;
    };
}