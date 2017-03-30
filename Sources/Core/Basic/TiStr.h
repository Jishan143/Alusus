/**
 * @file Core/Basic/TiStr.h
 * Contains the header of class Core::Basic::TiStr.
 *
 * @copyright Copyright (C) 2017 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef CORE_BASIC_TISTR_H
#define CORE_BASIC_TISTR_H

namespace Core { namespace Basic
{

/**
 * @brief An identifiable object that holds a string value.
 * @ingroup basic_datatypes
 */
template <class P> class TiStrBase : public P
{
  //============================================================================
  // Type Info

  TEMPLATE_TYPE_INFO(TiStrBase, P, "Core.Data", "Core", "alusus.net", (P));


  //============================================================================
  // Member Variables

  private: Str value;


  //============================================================================
  // Constructors

  public: TiStrBase()
  {
  }

  public: TiStrBase(Char const *v) : value(v)
  {
  }

  public: TiStrBase(Char const *v, Word c) : value(v, c)
  {
  }


  //============================================================================
  // Operators

  public: TiStrBase<P>& operator=(TiStrBase<P> const &v)
  {
    this->value = v.value;
    return *this;
  }

  public: TiStrBase<P>& operator=(Char const *v)
  {
    this->value = v;
    return *this;
  }

  public: operator Char const*() const
  {
    return this->value.c_str();
  }

  public: Bool operator==(TiStrBase<P> const *s) const
  {
    return this->value.compare(s.value) == 0;
  }

  public: Bool operator==(Char const *s) const
  {
    return this->value.compare(s) == 0;
  }

  public: Bool operator==(const std::string &s) const
  {
    return this->value.compare(s) == 0;
  }

  public: Bool operator==(const SbStr &s) const
  {
    return this->value.compare(s.c_str()) == 0;
  }

  public: Bool operator>(TiStrBase<P> const &s) const
  {
    return this->value.compare(s.value) > 0;
  }

  public: Bool operator>(Char const *s) const
  {
    return this->value.compare(s) > 0;
  }

  public: Bool operator>(const std::string &s) const
  {
    return this->value.compare(s) > 0;
  }

  public: Bool operator>(const SbStr &s) const
  {
    return this->value.compare(s.c_str()) > 0;
  }

  public: Bool operator<(TiStrBase<P> const &s) const
  {
    return this->value.compare(s.value) < 0;
  }

  public: Bool operator<(Char const *s) const
  {
    return this->value.compare(s) < 0;
  }

  public: Bool operator<(const std::string &s) const
  {
    return this->value.compare(s) < 0;
  }

  public: Bool operator<(const SbStr &s) const
  {
    return this->value.compare(s.c_str()) < 0;
  }


  //============================================================================
  // Member Functions

  public: void set(Char const *v)
  {
    this->value = v;
  }

  public: void set(Char const *v, Word c)
  {
    this->value.assign(v, c);
  }

  public: Char const* get() const
  {
    return this->value.c_str();
  }

  public: Str const& getStr() const
  {
    return this->value;
  }

}; // class


//==============================================================================
// Typedefs

typedef TiStrBase<TiObject> TiStr;

} } // namespace

#endif
