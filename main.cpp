#define CXX_SIMPLE_REFLECTION_MAIN
#include "cxx_simple_reflection.h"
#include "aether.h"

class BasicType_int16_t : public BasicType
{
public:
	const char* GetName() const override { return "int16_t"; }
	void SetValueByString( void* value, const char* str ) const override { *(int16_t*)value = atoi( str ); }
	std::string GetValueAsString( const void* value ) const override { return std::to_string( *(int16_t*)value ); }
};
template <>
const BasicType* GetBasicType< int16_t >()
{
	static BasicType_int16_t s_basicType;
	return &s_basicType;
}

//------------------------------------------------------------------------------
// Something
//------------------------------------------------------------------------------
class Something
{
public:
	int thing = 0;
};
Class meta_Something = Class( "Something", &NewFnT< Something > );
Var meta_Something_thing = Var( &meta_Something, "thing", offsetof( Something, thing ), GetBasicType< decltype(Something::thing) >() );

//------------------------------------------------------------------------------
// SomethingElse
//------------------------------------------------------------------------------
class SomethingElse
{
public:
	SomethingElse() { AE_INFO( "SomethingElse ctor" ); }
	int32_t thingInt = 7;
	float thingFloat = 12.34f;
};
REGISTER_CLASS( SomethingElse );
REGISTER_CLASS_VAR( SomethingElse, thingInt );
REGISTER_CLASS_VAR( SomethingElse, thingFloat );

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main()
{
	const Class* somethingElseClass = GetClassByName( "SomethingElse" );
	for ( auto var : somethingElseClass->vars )
	{
		const Var* v = var.second;
		AE_INFO( "#::#\ttype:# offset:#", somethingElseClass->name, v->name, v->basicType->GetName(), v->varOffset );
	}
	
	SomethingElse* somethingElse = (SomethingElse*)somethingElseClass->newFn();
	AE_INFO( "somethingElse->thingInt: #", somethingElse->thingInt );
	AE_INFO( "somethingElse->thingFloat: #", somethingElse->thingFloat );
	
	SetVar( somethingElseClass, somethingElse, "thingInt", "345" );
	SetVar( somethingElseClass, somethingElse, "thingFloat", "23.45" );
	AE_INFO( "somethingElse->thingInt: #", somethingElse->thingInt );
	AE_INFO( "somethingElse->thingFloat: #", somethingElse->thingFloat );
	
	// @TODO: inheritence
	
	AE_INFO( "done!" );
	return 0;
}
