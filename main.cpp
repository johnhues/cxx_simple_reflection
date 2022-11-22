#include "cxx_simple_reflection.h"
#include "aether.h"

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
	int thingInt = 7;
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

	for ( auto v : somethingElseClass->vars )
	{
		AE_INFO( "#::#\ttype:# offset:#", somethingElseClass->name, v->name, v->basicType, v->varOffset );
	}
	
	SomethingElse* somethingElse = (SomethingElse*)somethingElseClass->newFn();
	AE_INFO( "somethingElse->thingInt: #", somethingElse->thingInt );
	AE_INFO( "somethingElse->thingFloat: #", somethingElse->thingFloat );
	
	// TODO 1 SetVar/GetVar
	//somethingElseClass->SetVar( somethingElse, "thingInt", "345" );
	
	// TODO 2
	// inheritence
	
	AE_INFO( "done!" );
	return 0;
}
