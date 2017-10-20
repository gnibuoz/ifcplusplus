/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/IFC4/include/IfcDistributionChamberElementTypeEnum.h"

// TYPE IfcDistributionChamberElementTypeEnum = ENUMERATION OF	(FORMEDDUCT	,INSPECTIONCHAMBER	,INSPECTIONPIT	,MANHOLE	,METERCHAMBER	,SUMP	,TRENCH	,VALVECHAMBER	,USERDEFINED	,NOTDEFINED);
IfcDistributionChamberElementTypeEnum::IfcDistributionChamberElementTypeEnum() {}
IfcDistributionChamberElementTypeEnum::~IfcDistributionChamberElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcDistributionChamberElementTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDistributionChamberElementTypeEnum> copy_self( new IfcDistributionChamberElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDistributionChamberElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDISTRIBUTIONCHAMBERELEMENTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_FORMEDDUCT:	stream << ".FORMEDDUCT."; break;
		case ENUM_INSPECTIONCHAMBER:	stream << ".INSPECTIONCHAMBER."; break;
		case ENUM_INSPECTIONPIT:	stream << ".INSPECTIONPIT."; break;
		case ENUM_MANHOLE:	stream << ".MANHOLE."; break;
		case ENUM_METERCHAMBER:	stream << ".METERCHAMBER."; break;
		case ENUM_SUMP:	stream << ".SUMP."; break;
		case ENUM_TRENCH:	stream << ".TRENCH."; break;
		case ENUM_VALVECHAMBER:	stream << ".VALVECHAMBER."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDistributionChamberElementTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_FORMEDDUCT:	return L"FORMEDDUCT";
		case ENUM_INSPECTIONCHAMBER:	return L"INSPECTIONCHAMBER";
		case ENUM_INSPECTIONPIT:	return L"INSPECTIONPIT";
		case ENUM_MANHOLE:	return L"MANHOLE";
		case ENUM_METERCHAMBER:	return L"METERCHAMBER";
		case ENUM_SUMP:	return L"SUMP";
		case ENUM_TRENCH:	return L"TRENCH";
		case ENUM_VALVECHAMBER:	return L"VALVECHAMBER";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcDistributionChamberElementTypeEnum> IfcDistributionChamberElementTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDistributionChamberElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDistributionChamberElementTypeEnum>(); }
	shared_ptr<IfcDistributionChamberElementTypeEnum> type_object( new IfcDistributionChamberElementTypeEnum() );
	if( boost::iequals( arg, L".FORMEDDUCT." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_FORMEDDUCT;
	}
	else if( boost::iequals( arg, L".INSPECTIONCHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_INSPECTIONCHAMBER;
	}
	else if( boost::iequals( arg, L".INSPECTIONPIT." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_INSPECTIONPIT;
	}
	else if( boost::iequals( arg, L".MANHOLE." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_MANHOLE;
	}
	else if( boost::iequals( arg, L".METERCHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_METERCHAMBER;
	}
	else if( boost::iequals( arg, L".SUMP." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_SUMP;
	}
	else if( boost::iequals( arg, L".TRENCH." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_TRENCH;
	}
	else if( boost::iequals( arg, L".VALVECHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_VALVECHAMBER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}