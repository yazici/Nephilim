#include <Nephilim/AngelScriptEXT/ASXModule.h>
#include <Nephilim/AngelScriptEXT/ASXEngine.h>

NEPHILIM_NS_BEGIN

ASXModule::ASXModule()
: m_module(NULL)
{

}

ASXModule::ASXModule(asIScriptModule* module)
{
	m_module = module;
}

asIScriptModule* ASXModule::get()
{
	return m_module;
}


ASXModule::operator bool()
{
	return m_module != NULL;
}


NEPHILIM_NS_END