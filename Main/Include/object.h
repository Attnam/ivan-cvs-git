#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "igraph.h"
#include "entity.h"
#include "id.h"

class god;

class object : public entity, public id
{
 public:
  object();
  object(const object&);
  virtual ~object();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void InitMaterials(material*, bool = true);
  virtual void UpdatePictures();
  material* GetMainMaterial() const { return MainMaterial; }
  virtual material* GetSecondaryMaterial() const { return 0; }
  virtual material* GetContainedMaterial() const { return 0; }
  virtual material* GetConsumeMaterial() const { return MainMaterial; }
  void SetMainMaterial(material*, ushort = 0);
  void ChangeMainMaterial(material*, ushort = 0);
  virtual void SetConsumeMaterial(material*, ushort = 0);
  virtual void ChangeConsumeMaterial(material*, ushort = 0);
  virtual void SetSecondaryMaterial(material*, ushort = 0);
  virtual void ChangeSecondaryMaterial(material*, ushort = 0);
  virtual void SetContainedMaterial(material*, ushort = 0);
  virtual void ChangeContainedMaterial(material*, ushort = 0);
  virtual ushort GetMaterials() const { return 1; }
  virtual material* GetMaterial(ushort) const { return MainMaterial; }
  const std::vector<bitmap*>& GetPicture() const;
  virtual ulong GetBaseEmitation() const { return 0; }
  virtual void SetParameters(uchar) { }
  virtual uchar GetOKVisualEffects() const { return 0; }
  uchar GetVisualEffects() const { return VisualEffects; }
  void SetVisualEffects(uchar What) { VisualEffects = What; }
  virtual uchar GetForcedVisualEffects() const { return 0; }
  void SetAnimationFrames(ushort What) { AnimationFrames = What; }
  ushort GetAnimationFrames() const { return AnimationFrames; }
  bool IsAnimated() const { return AnimationFrames > 1; }
  virtual void CalculateEmitation();
  void LoadMaterial(inputfile&, material*&);
  ushort GetConfig() const { return Config; }
  virtual void Draw(bitmap*, vector2d, ulong, bool) const;
  void SolidDraw(bitmap*, vector2d, ulong, bool) const;
  void Draw(bitmap*, vector2d, ulong, bool, bool) const;
  virtual god* GetMasterGod() const;
  virtual const std::vector<long>& GetMainMaterialConfig() const = 0;
  virtual const std::vector<long>& GetMaterialConfigChances() const = 0;
  void SetConfig(ushort);
  virtual void CalculateAll() = 0;
  virtual uchar GetSpoilLevel() const { return 0; }
 protected:
  virtual bool IsSparkling(ushort) const;
  void CopyMaterial(material* const&, material*&);
  void ObjectInitMaterials(material*&, material*, ulong, material*&, material*, ulong, bool);
  void ObjectInitMaterials(material*&, material*, ulong, material*&, material*, ulong, material*&, material*, ulong, bool);
  virtual ulong GetDefaultMainVolume() const { return 0; }
  virtual ulong GetDefaultSecondaryVolume() const { return 0; }
  virtual ulong GetDefaultContainedVolume() const { return 0; }
  void InitMaterial(material*&, material*, ulong);
  material* SetMaterial(material*&, material*, ulong, ushort);
  void ChangeMaterial(material*&, material*, ulong, ushort);
  bool CalculateHasBe() const;
  virtual uchar GetSpecialFlags() const;
  virtual uchar GetGraphicsContainerIndex() const = 0;
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const { return 0; }
  virtual ushort GetMaterialColorC(ushort) const { return 0; }
  virtual ushort GetMaterialColorD(ushort) const { return 0; }
  virtual uchar GetMaxAlpha(ushort) const { return 255; }
  virtual uchar GetBaseAlpha(ushort) const { return 255; }
  virtual uchar GetAlphaA(ushort) const;
  virtual uchar GetAlphaB(ushort) const { return 255; }
  virtual uchar GetAlphaC(ushort) const { return 255; }
  virtual uchar GetAlphaD(ushort) const { return 255; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool AddMaterialDescription(std::string&, bool) const;
  virtual ushort RandomizeMaterialConfiguration();
  virtual void GenerateMaterials();
  virtual void InitChosenMaterial(material*&, const std::vector<long>&, ulong, ushort);
  virtual void InstallDataBase() = 0;
  virtual ushort GetClassAnimationFrames() const { return 1; }
  void AddContainerPostFix(std::string&) const;
  void AddLumpyPostFix(std::string&) const;
  bool AddEmptyAdjective(std::string&, bool) const;
  virtual vector2d GetBitmapPos(ushort) const = 0;
  void RandomizeVisualEffects();
  virtual bool HasSpecialAnimation() const { return false; }
  material* MainMaterial;
  std::vector<graphicid> GraphicId;
  std::vector<bitmap*> Picture;
  ushort Config;
  uchar VisualEffects;
  ushort AnimationFrames;
};

#endif
