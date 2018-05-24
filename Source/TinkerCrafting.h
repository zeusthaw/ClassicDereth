class CClient;
class BinaryWriter;

class TinkerCrafting : public CWeenieObject
//class TinkerCrafting : public CPlayerWeenie
{
public:
	CWeenieObject *CWO;
	json jsonMaterialData;
	bool CalculateChance(CWeenieObject *Salvage, CWeenieObject *Equipment,int PlayerSkill);
	float TinkerAttemptMod(int tink);
	bool MaterialInfo(unsigned int ct, std::string & name, int & mod, int & imbue);
};
