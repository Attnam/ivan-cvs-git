#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "igraph.h"
#include "entity.h"
#include "id.h"

class god;
class object;

typedef vector2d (object::*bposretriever)(ushort) const;

class object : public entity, public id
{
 public:
  object();
  object(const object&);
  virtual ~object();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void UpdatePictures();
  material* GetMainMaterial() const { return MainMaterial; }
  virtual material* GetSecondaryMaterial() const { return 0; }
  virtual material* GetContainedMaterial() const { return 0; }
  virtual void SetSecondaryMaterial(material*, ushort = 0) { }
  virtual void ChangeSecondaryMaterial(material*, ushort = 0) { }
  virtual void SetContainedMaterial(material*, ushort = 0) { }
  virtual void ChangeContainedMaterial(material*, ushort = 0) { }
  virtual ushort GetMaterials() const { return 1; }
  virtual material* GetMaterial(ushort) const { return MainMaterial; }
  const bitmap*const* GetPicture() const;
  virtual ulong GetBaseEmitation() const { return 0; }
  virtual void SetParameters(uchar) { }
  virtual uchar GetOKVisualEffects() const { return 0; }
  uchar GetVisualEffects() const { return VisualEffects; }
  void SetVisualEffects(uchar What) { VisualEffects = What; }
  virtual uchar GetForcedVisualEffects() const { return 0; }
  ushort GetAnimationFrames() const { return GraphicData.AnimationFrames; }
  virtual bool IsAnimated() const { return GraphicData.AnimationFrames > 1; }
  virtual void CalculateEmitation();
  void LoadMaterial(inputfile&, material*&);
  virtual const std::vector<long>& GetMainMaterialConfig() const = 0;
  virtual const std::vector<long>& GetMaterialConfigChances() const = 0;
  virtual void CalculateAll() = 0;
  virtual uchar GetSpoilLevel() const { return 0; }
  void CreateWieldedBitmap(graphicid&) const;
  virtual ushort GetSpecialFlags() const;
  static void InitSparkleValidityArrays();
  void UpdatePictures(graphicdata&, vector2d, ushort, uchar, uchar, bposretriever) const;
  void InitMaterial(material*&, material*, ulong);
 protected:
  virtual bool IsSparkling(ushort) const;
  void CopyMaterial(material* const&, material*&);
  void ObjectInitMaterials(material*&, material*, ulong, material*&, material*, ulong, bool);
  void ObjectInitMaterials(material*&, material*, ulong, material*&, material*, ulong, material*&, material*, ulong, bool);
  material* SetMaterial(material*&, material*, ulong, ushort);
  void ChangeMaterial(material*&, material*, ulong, ushort);
  virtual bool CalculateHasBe() const;
  virtual uchar GetGraphicsContainerIndex() const = 0;
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const { return 0; }
  virtual ushort GetMaterialColorC(ushort) const { return 0; }
  virtual ushort GetMaterialColorD(ushort) const { return 0; }
  virtual uchar GetMaxAlpha() const { return 255; }
  virtual uchar GetBaseAlpha(ushort) const { return 255; }
  virtual uchar GetAlphaA(ushort) const;
  virtual uchar GetAlphaB(ushort) const { return 255; }
  virtual uchar GetAlphaC(ushort) const { return 255; }
  virtual uchar GetAlphaD(ushort) const { return 255; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual uchar GetOutlineAlpha(ushort) const { return 255; }
  virtual bool AddMaterialDescription(festring&, bool) const;
  virtual ushort RandomizeMaterialConfiguration();
  virtual void InitChosenMaterial(material*&, const std::vector<long>&, ulong, ushort);
  virtual void InstallDataBase(ushort) = 0;
  virtual ushort GetClassAnimationFrames() const { return 1; }
  void AddContainerPostFix(festring&) const;
  void AddLumpyPostFix(festring&) const;
  bool AddEmptyAdjective(festring&, bool) const;
  virtual vector2d GetBitmapPos(ushort) const = 0;
  void RandomizeVisualEffects();
  virtual bool HasSpecialAnimation() const { return false; }
  virtual void ModifyAnimationFrames(ushort&) const { }
  virtual uchar GetRustDataA() const;
  virtual uchar GetRustDataB() const { return NOT_RUSTED; }
  virtual uchar GetRustDataC() const { return NOT_RUSTED; }
  virtual uchar GetRustDataD() const { return NOT_RUSTED; }
  virtual ushort GetDripColor() const { return 0; }
  virtual bool AllowSparkling() const { return true; }
  virtual bool AllowRegularColors() const { return true; }
  graphicdata GraphicData;
  material* MainMaterial;
  uchar VisualEffects;
};

#endif
