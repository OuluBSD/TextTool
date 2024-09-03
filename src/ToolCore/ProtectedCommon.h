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


const Index<String>& GetPersonas();
const Vector<ContentType>& GetNiches();
int GetNicheCount();
VectorMap<String,Vector<String>>& GetPersonaSafe(bool gender);
VectorMap<String,Vector<String>>& GetPersonaUnsafe(bool gender);
VectorMap<String,Vector<String>>& GetPersonaNiches(bool unsafe, bool gender);


const Index<String>& GetCharacters();
const Vector<ContentType>& GetTropes();
int GetTropeCount();
VectorMap<String,Vector<String>>& GetCharacterSafeMale();
VectorMap<String,Vector<String>>& GetCharacterSafeFemale();
VectorMap<String,Vector<String>>& GetCharacterUnsafeMale();
VectorMap<String,Vector<String>>& GetCharacterUnsafeFemale();
VectorMap<String,Vector<String>>& GetCharacterSafe(bool gender);
VectorMap<String,Vector<String>>& GetCharacterUnsafe(bool gender);
VectorMap<String,Vector<String>>& GetCharacterTropes(bool unsafe, bool gender);



const Index<String>& GetStyles();
const Vector<ContentType>& GetApproaches();
int GetApproachCount();

VectorMap<String,Vector<String>>& GetStyleSafeMale();
VectorMap<String,Vector<String>>& GetStyleSafeFemale();
VectorMap<String,Vector<String>>& GetStyleUnsafeMale();
VectorMap<String,Vector<String>>& GetStyleUnsafeFemale();
VectorMap<String,Vector<String>>& GetStyleSafe(bool gender);
VectorMap<String,Vector<String>>& GetStyleUnsafe(bool gender);
VectorMap<String,Vector<String>>& GetStyleApproaches(bool unsafe, bool gender);




const Index<String>& GetPersuasiveElements();
const Vector<ContentType>& GetCallToActions();
int GetCallToActionCount();

VectorMap<String,Vector<String>>& GetTriggerSafeMale();
VectorMap<String,Vector<String>>& GetTriggerSafeFemale();
VectorMap<String,Vector<String>>& GetTriggerUnsafeMale();
VectorMap<String,Vector<String>>& GetTriggerUnsafeFemale();
VectorMap<String,Vector<String>>& GetTriggerSafe(bool gender);
VectorMap<String,Vector<String>>& GetTriggerUnsafe(bool gender);
VectorMap<String,Vector<String>>& GetPersuasiveTriggers(bool unsafe, bool gender);



const Index<String>& GetProgramGenres();
const Vector<ContentType>& GetProgrammingApproaches();


END_TEXTLIB_NAMESPACE


#endif
