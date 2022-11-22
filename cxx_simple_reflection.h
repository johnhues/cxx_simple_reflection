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
template < typename T >
void* NewFnT()
{
	return new T();
}
typedef void* (*NewFn)();

//------------------------------------------------------------------------------
// GetClassByName()
//------------------------------------------------------------------------------
std::map< std::string, class Class* > s_classes;
const class Class* GetClassByName( const char* name )
{
	auto iter = s_classes.find( name );
	if ( iter != s_classes.end() )
	{
		return iter->second;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
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
	std::vector< const class Var* > vars;
};

//------------------------------------------------------------------------------
// BasicType
//------------------------------------------------------------------------------
enum class BasicType
{
	None,
	Int32,
	Float,
	// Others...
};
inline std::ostream& operator<<( std::ostream& os, BasicType t )
{
	switch ( t )
	{
		case BasicType::None: return ( os << "None" );
		case BasicType::Int32: return ( os << "Int32" );
		case BasicType::Float: return ( os << "Float" );
		default: return ( os << "" );
	}
}
template < typename T > BasicType GetBasicType() { return BasicType::None; }
template <> BasicType GetBasicType< int32_t >() { return BasicType::Int32; }
template <> BasicType GetBasicType< float >() { return BasicType::Float; }

//------------------------------------------------------------------------------
// Var
//------------------------------------------------------------------------------
class Var
{
public:
	Var( Class* c, const char* name, int32_t varOffset, BasicType basicType ) :
		name( name ),
		varOffset( varOffset ),
		basicType( basicType )
	{
		c->vars.push_back( this );
	}
	
	std::string name;
	int32_t varOffset = 0;
	BasicType basicType = BasicType::None;
};

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define REGISTER_CLASS( _c ) Class meta_##_c = Class( #_c, &NewFnT< _c > )
#define REGISTER_CLASS_VAR( _c, _v ) Var meta_##_c_##_v = Var( &meta_##_c, #_v, offsetof( _c, _v ), GetBasicType< decltype(_c::_v) >() )
