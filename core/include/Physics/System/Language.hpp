#pragma once
#include "SFML/System/String.hpp"
#include <Physics/System/NonCopyable.hpp>
#include <Physics/System/Exception.hpp>
#include <unordered_map>
#include <array>
#define PHYSICS_LANGUAGES_SUPPORTED 2
#define PHYSICS_LANGUAGE_PAIR(key, map) {key, map}

namespace physics
{
    class Language : public NonCopyable
    {
    public:
        /**
        * @brief String representing std::u32string, implementation may or may not change
        */
        using String = std::u32string;
        using CharMap = std::unordered_map<const char*, String>;
        using MapArray = std::array<CharMap, PHYSICS_LANGUAGES_SUPPORTED>;

        /**
         * @brief Enum representing all supported languages
         */
        enum Option
        {
            Greek, English
        };

        /**
         * @brief Get a language property, as set in "Physics/Config/<Language>" (relative path), where <Language> is the supported language
         * @param name The name of the relative in snake_case and lowercase alphanumeric (plus '_') characters 
         * @param language Enumerator representing the language for which to return the property
         * @return String representing the property's value 
         */
        static String GetTextSTD(const char* name, const Option& language = SelectedLanguage)
        {
            static CharMap greek_map = {
                #include <Physics/Config/Greek>
            };

            static CharMap english_map = {
                #include <Physics/Config/English>
            };

            switch(language)
            {
            case Option::Greek: return greek_map.at(name);
            case Option::English: return english_map.at(name);
            default: throw PHYSICS_EXCEPTION("Invalid language! No language with index '" + std::to_string(SelectedLanguage) + "' exists!");
            }
        }

        /**
         * @brief Does the same as GetStringSTD(), but converts the string to the sfml format before returning
         * @param name The name of the relative in snake_case and lowercase alphanumeric (plus '_') characters 
         * @param language Enumerator representing the language for which to return the property
         * @return sf::String representing the property's value
         */
        static sf::String GetTextSFML(const char* name, const Option& language = SelectedLanguage)
        {
            String str = GetTextSTD(name, language);
            return sf::String::fromUtf32(str.begin(), str.end());
        }
        static Option SelectedLanguage;
    };
    Language::Option Language::SelectedLanguage = Option::Greek;
}