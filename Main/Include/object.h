#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "igraph.h"
#include "entity.h"
#include "id.h"
#include "fearray.h"

class god;
class object;

typedef vector2d (object::*bposretriever)(int) const;

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
  virtual void SetSecondaryMaterial(material*, int = 0) { }
  virtual void ChangeSecondaryMaterial(material*, int = 0) { }
  virtual int GetMaterials() const { return 1; }
  virtual material* GetMaterial(int) const { return MainMaterial; }
  const bitmap*const* GetPicture() const;
  virtual color24 GetBaseEmitation() const { return 0; }
  virtual void SetParameters(int) { }
  virtual int GetOKVisualEffects() const { return 0; }
  int GetVisualEffects() const { return VisualEffects; }
  void SetVisualEffects(int What) { VisualEffects = What; }
  virtual int GetForcedVisualEffects() const { return 0; }
  int GetAnimationFrames() const { return GraphicData.AnimationFrames; }
  virtual bool IsAnimated() const { return GraphicData.AnimationFrames > 1; }
  virtual void CalculateEmitation();
  void LoadMaterial(inputfile&, material*&);
  virtual const fearray<long>& GetMaterialConfigChances() const = 0;
  virtual long GetMaterialConfigChanceSum() const = 0;
  virtual void CalculateAll() = 0;
  virtual int GetSpoilLevel() const { return 0; }
  void CreateWieldedBitmap(graphicid&) const;
  virtual int GetSpecialFlags() const;
  static void InitSparkleValidityArrays();
  void UpdatePictures(graphicdata&, vector2d, int, alpha, int, bposretriever) const;
  void InitMaterial(material*&, material*, long);
  virtual bool DetectMaterial(const material*) const;
 protected:
  virtual bool IsSparkling(int) const;
  void CopyMaterial(material* const&, material*&);
  void ObjectInitMaterials(material*&, material*, long, material*&, material*, long, bool);
  material* SetMaterial(material*&, material*, long, int);
  void ChangeMaterial(material*&, material*, long, int);
  virtual bool CalculateHasBe() const;
  virtual int GetGraphicsContainerIndex() const = 0;
  virtual color16 GetMaterialColorA(int) const;
  virtual color16 GetMaterialColorB(int) const { return 0; }
  virtual color16 GetMaterialColorC(int) const { return 0; }
  virtual color16 GetMaterialColorD(int) const { return 0; }
  virtual alpha GetMaxAlpha() const { return 255; }
  virtual alpha GetAlphaA(int) const;
  virtual alpha GetAlphaB(int) const { return 255; }
  virtual alpha GetAlphaC(int) const { return 255; }
  virtual alpha GetAlphaD(int) const { return 255; }
  virtual color16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const { return 255; }
  virtual bool AddRustLevelDescription(festring&, bool) const;
  virtual bool AddMaterialDescription(festring&, bool) const;
  int RandomizeMaterialConfiguration();
  virtual int GetClassAnimationFrames() const { return 1; }
  void AddContainerPostFix(festring&) const;
  void AddLumpyPostFix(festring&) const;
  bool AddEmptyAdjective(festring&, bool) const;
  virtual vector2d GetBitmapPos(int) const = 0;
  void RandomizeVisualEffects();
  virtual void ModifyAnimationFrames(int&) const { }
  virtual int GetRustDataA() const;
  virtual int GetRustDataB() const { return NOT_RUSTED; }
  virtual int GetRustDataC() const { return NOT_RUSTED; }
  virtual int GetRustDataD() const { return NOT_RUSTED; }
  virtual color16 GetDripColor() const { return 0; }
  virtual bool AllowSparkling() const { return true; }
  virtual bool AllowRegularColors() const { return true; }
  graphicdata GraphicData;
  material* MainMaterial;
  int VisualEffects;
};

#endif
