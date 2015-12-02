#pragma once

#include "LokiTypeInfo.h"
#include "AssocVector.h"
#include <exception>

namespace Loki
{

////////////////////////////////////////////////////////////////////////////////
// class template DefaultFactoryError
// Manages the "Unknown Type" error in an object factory
////////////////////////////////////////////////////////////////////////////////

    template <typename IdentifierType, class AbstractProduct>
    struct DefaultFactoryError
    {
        struct Exception : public std::exception
        {
            const char* what() const throw() { return "Unknown Type"; }
        };
        
        static AbstractProduct* OnUnknownType(IdentifierType)
        {
            throw Exception();
        }
    };

////////////////////////////////////////////////////////////////////////////////
// class template Factory
// Implements a generic object factory
////////////////////////////////////////////////////////////////////////////////

    template
    <
        class AbstractProduct, 
        typename IdentifierType,
        typename ProductCreator = AbstractProduct* (*)(),
        template<typename, class>
            class FactoryErrorPolicy = DefaultFactoryError
    >
    class Factory 
        : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
    {
    public:
        bool Register(const IdentifierType& id, ProductCreator creator)
        {
            return associations_.insert(
                typename IdToProductMap::value_type(id, creator)).second;
        }
        
        bool Unregister(const IdentifierType& id)
        {
            return associations_.erase(id) == 1;
        }
        
        AbstractProduct* CreateObject(const IdentifierType& id)
        {
            typename IdToProductMap::iterator i = associations_.find(id);
            if (i != associations_.end())
            {
                return (i->second)();
            }
            return this->OnUnknownType(id);
        }
        
    private:
        typedef AssocVector<IdentifierType, ProductCreator> IdToProductMap;
        IdToProductMap associations_;
    };

////////////////////////////////////////////////////////////////////////////////
// class template CloneFactory
// Implements a generic cloning factory
////////////////////////////////////////////////////////////////////////////////

    template
    <
        class AbstractProduct, 
        class ProductCreator = 
            AbstractProduct* (*)(const AbstractProduct*),
        template<typename, class>
            class FactoryErrorPolicy = DefaultFactoryError
    >
    class CloneFactory
        : public FactoryErrorPolicy<TypeInfo, AbstractProduct>
    {
    public:
        bool Register(const TypeInfo& ti, ProductCreator creator)
        {
            return associations_.insert(
                typename IdToProductMap::value_type(ti, creator)).second;
        }
        
        bool Unregister(const TypeInfo& id)
        {
            return associations_.erase(id) == 1;
        }
        
        AbstractProduct* CreateObject(const AbstractProduct* model)
        {
            if (model == 0) return 0;
            
            typename IdToProductMap::iterator i = 
                associations_.find(typeid(*model));
            if (i != associations_.end())
            {
                return (i->second)(model);
            }
            return this->OnUnknownType(typeid(*model));
        }
        
    private:
        typedef AssocVector<TypeInfo, ProductCreator> IdToProductMap;
        IdToProductMap associations_;
    };
} // namespace Loki

