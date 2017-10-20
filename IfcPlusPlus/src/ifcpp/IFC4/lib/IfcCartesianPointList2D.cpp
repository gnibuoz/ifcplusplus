/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcCartesianPointList2D.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcCartesianPointList2D 
IfcCartesianPointList2D::IfcCartesianPointList2D() {}
IfcCartesianPointList2D::IfcCartesianPointList2D( int id ) { m_id = id; }
IfcCartesianPointList2D::~IfcCartesianPointList2D() {}
shared_ptr<IfcPPObject> IfcCartesianPointList2D::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCartesianPointList2D> copy_self( new IfcCartesianPointList2D() );
	copy_self->m_CoordList.resize( m_CoordList.size() );
	for( size_t ii=0; ii<m_CoordList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii = m_CoordList[ii];
		std::vector<shared_ptr<IfcLengthMeasure> >& vec_ii_target = copy_self->m_CoordList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcLengthMeasure>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcLengthMeasure>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	return copy_self;
}
void IfcCartesianPointList2D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCARTESIANPOINTLIST2D" << "(";
	writeNumericTypeList2D( stream, m_CoordList );
	stream << ");";
}
void IfcCartesianPointList2D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
const std::wstring IfcCartesianPointList2D::toString() const { return L"IfcCartesianPointList2D"; }
void IfcCartesianPointList2D::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCartesianPointList2D, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readTypeOfRealList2D( args[0], m_CoordList );
}
void IfcCartesianPointList2D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCartesianPointList::getAttributes( vec_attributes );
}
void IfcCartesianPointList2D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCartesianPointList::getAttributesInverse( vec_attributes_inverse );
}
void IfcCartesianPointList2D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCartesianPointList::setInverseCounterparts( ptr_self_entity );
}
void IfcCartesianPointList2D::unlinkFromInverseCounterparts()
{
	IfcCartesianPointList::unlinkFromInverseCounterparts();
}