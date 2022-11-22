//------------------------------------------------------------------------------
// Headers
//------------------------------------------------------------------------------
#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
// NewFnT
//------------------------------------------------------------------------------
template < typename T > void* NewFnT() { return new T(); }
typedef void* (*NewFn)();

//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
std::map< std::string, class Class* > s_classes;
class Class
{
public:
	Class( const char* name, NewFn newFn ) :
		name( name ),
		newFn( newFn )
	{
		s_classes[ name ] = this;
	}
	
	std::string name;
	NewFn newFn = nullptr;
	std::map< std::string, const class Var* > vars;
};

//------------------------------------------------------------------------------
// Var
//------------------------------------------------------------------------------
class Var
{
public:
	Var( Class* c, const char* name, int32_t varOffset, const class BasicType* basicType ) :
		name( name ),
		varOffset( varOffset ),
		basicType( basicType )
	{
		c->vars[ name ] = this;
	}
	
	std::string name;
	int32_t varOffset = 0;
	const class BasicType* basicType = nullptr;
};

//------------------------------------------------------------------------------
// BasicType
//------------------------------------------------------------------------------
class BasicType
{
public:
	virtual const char* GetName() const = 0;
	virtual void SetValueByString( void* value, const char* str ) const = 0;
	virtual std::string GetValueAsString( const void* value ) const = 0;
};
template < typename T > const BasicType* GetBasicType();

#define REGISTER_BASIC_TYPE( _t, _fromStr, _toStr )\
class BasicType_##_t : public BasicType { public:\
	const char* GetName() const override { return #_t; }\
	void SetValueByString( void* value, const char* str ) const override { *(_t*)value = _fromStr( str ); }\
	std::string GetValueAsString( const void* value ) const override { return _toStr( *(_t*)value ); }\
};\
template <> const BasicType* GetBasicType< _t >() { static BasicType_##_t s_basicType; return &s_basicType; }

REGISTER_BASIC_TYPE( int32_t,
	[]( const char* s ){ return atoi( s ); },
	[]( int32_t i ){ return std::to_string( i ); }
);
REGISTER_BASIC_TYPE( float,
	[]( const char* s ){ return atof( s ); },
	[]( float f ){ return std::to_string( f ); }
);

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define REGISTER_CLASS( _c ) Class meta_##_c = Class( #_c, &NewFnT< _c > )
#define REGISTER_CLASS_VAR( _c, _v ) Var meta_##_c_##_v = Var( &meta_##_c, #_v, offsetof( _c, _v ), GetBasicType< decltype(_c::_v) >() )

//------------------------------------------------------------------------------
// GetClassByName()
//------------------------------------------------------------------------------
inline const class Class* GetClassByName( const char* name )
{
	auto iter = s_classes.find( name );
	return ( iter != s_classes.end() ) ? iter->second : nullptr;
}

//------------------------------------------------------------------------------
// SetVar()
//------------------------------------------------------------------------------
inline void SetVar( const Class* c, void* object, const char* varName, const char* value )
{
	if ( object )
	{
		auto iter = c->vars.find( varName );
		if ( iter != c->vars.end() )
		{
			const class Var* var = iter->second;
			var->basicType->SetValueByString( ( (uint8_t*)object + var->varOffset ), value );
		}
	}
}
