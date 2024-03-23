#ifndef _ToolCore_ProtectedCommon_h_
#define _ToolCore_ProtectedCommon_h_


#include <ToolCore/ToolCore.h>


BEGIN_TEXTLIB_NAMESPACE


const Index<String>& GetTypecasts();
int GetTypecastCount();

const Index<String>& GetProfiles();
int GetProfileCount();

const Index<String>& GetPrimary();
int GetPrimaryCount();

const Index<String>& GetSecondary();
int GetSecondaryCount();


const VectorMap<String,String>& GetContents();
int GetContentCount();

const Vector<ContentType>& GetContrasts();
const Vector<ContentType>& GetGenerics();
const Index<String>& GetRoles();

VectorMap<String,Vector<String>>& GetTypecastSingersMale();
VectorMap<String,Vector<String>>& GetTypecastSingersFemale();
VectorMap<String,Vector<String>>& GetTypecastRappersMale();
VectorMap<String,Vector<String>>& GetTypecastRappersFemale();
VectorMap<String,Vector<String>>& GetTypecastSingers(bool gender);
VectorMap<String,Vector<String>>& GetTypecastRappers(bool gender);
VectorMap<String,Vector<String>>& GetTypecastArtists(bool rapper, bool gender);
VectorMap<String,Vector<String>>& GetRoleCompanies(bool unsafe, bool gender);


const Vector<String>& GetContrastParts();


END_TEXTLIB_NAMESPACE


#endif
