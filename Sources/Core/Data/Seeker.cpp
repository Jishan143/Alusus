/**
 * @file Core/Data/Seeker.cpp
 * Contains the implementation of class Core::Data::Seeker.
 *
 * @copyright Copyright (C) 2018 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#include "core.h"

namespace Core { namespace Data
{

//==============================================================================
// Initialization

void Seeker::initBindingCaches()
{
  Basic::initBindingCaches(this, {
    &this->set,
    &this->remove,
    &this->foreach,
    &this->setByIdentifier,
    &this->setByIdentifier_level,
    &this->setByIdentifier_scope,
    &this->removeByIdentifier,
    &this->removeByIdentifier_level,
    &this->removeByIdentifier_scope,
    &this->foreachByIdentifier,
    &this->foreachByIdentifier_level,
    &this->foreachByIdentifier_scope,
    &this->setByLinkOperator,
    &this->setByLinkOperator_routing,
    &this->setByLinkOperator_scopeDotIdentifier,
    &this->setByLinkOperator_mapDotIdentifier,
    &this->removeByLinkOperator,
    &this->removeByLinkOperator_routing,
    &this->removeByLinkOperator_scopeDotIdentifier,
    &this->removeByLinkOperator_mapDotIdentifier,
    &this->foreachByLinkOperator,
    &this->foreachByLinkOperator_routing,
    &this->foreachByLinkOperator_scopeDotIdentifier,
    &this->foreachByLinkOperator_mapDotIdentifier
  });
}


void Seeker::initBindings()
{
  // Main seek functions
  this->set = &Seeker::_set;
  this->remove = &Seeker::_remove;
  this->foreach = &Seeker::_foreach;

  // Identifier seek functions
  this->setByIdentifier = &Seeker::_setByIdentifier;
  this->setByIdentifier_level = &Seeker::_setByIdentifier_level;
  this->setByIdentifier_scope = &Seeker::_setByIdentifier_scope;
  this->removeByIdentifier = &Seeker::_removeByIdentifier;
  this->removeByIdentifier_level = &Seeker::_removeByIdentifier_level;
  this->removeByIdentifier_scope = &Seeker::_removeByIdentifier_scope;
  this->foreachByIdentifier = &Seeker::_foreachByIdentifier;
  this->foreachByIdentifier_level = &Seeker::_foreachByIdentifier_level;
  this->foreachByIdentifier_scope = &Seeker::_foreachByIdentifier_scope;

  // LinkOperator seek functions
  this->setByLinkOperator = &Seeker::_setByLinkOperator;
  this->setByLinkOperator_routing = &Seeker::_setByLinkOperator_routing;
  this->setByLinkOperator_scopeDotIdentifier = &Seeker::_setByLinkOperator_scopeDotIdentifier;
  this->setByLinkOperator_mapDotIdentifier = &Seeker::_setByLinkOperator_mapDotIdentifier;
  this->removeByLinkOperator = &Seeker::_removeByLinkOperator;
  this->removeByLinkOperator_routing = &Seeker::_removeByLinkOperator_routing;
  this->removeByLinkOperator_scopeDotIdentifier = &Seeker::_removeByLinkOperator_scopeDotIdentifier;
  this->removeByLinkOperator_mapDotIdentifier = &Seeker::_removeByLinkOperator_mapDotIdentifier;
  this->foreachByLinkOperator = &Seeker::_foreachByLinkOperator;
  this->foreachByLinkOperator_routing = &Seeker::_foreachByLinkOperator_routing;
  this->foreachByLinkOperator_scopeDotIdentifier = &Seeker::_foreachByLinkOperator_scopeDotIdentifier;
  this->foreachByLinkOperator_mapDotIdentifier = &Seeker::_foreachByLinkOperator_mapDotIdentifier;
}


//==============================================================================
// Helper Functions

Bool Seeker::trySet(TiObject const *ref, TiObject *target, TiObject *val, Word flags)
{
  Bool result = false;
  this->doSet(ref, target, [=,&result](TiObject *&obj, Notice*)->Verb {
    obj = val;
    result = true;
    return Verb::PERFORM_AND_MOVE;
  }, flags);
  return result;
}


Bool Seeker::tryRemove(TiObject const *ref, TiObject *target, Word flags)
{
  Bool ret = false;
  this->doRemove(ref, target, [&ret](TiObject *o, Notice*)->Verb {
    ret = true;
    return Verb::PERFORM_AND_MOVE;
  }, flags);
  return ret;
}


Bool Seeker::tryGet(TiObject const *ref, TiObject *target, TiObject *&retVal, Word flags)
{
  Bool ret = false;
  this->doForeach(ref, target, [&ret,&retVal](TiObject *o, Notice*)->Verb {
    retVal = o;
    ret = true;
    return Verb::STOP;
  }, flags);
  return ret;
}


//==============================================================================
// Main Seek Functions

void Seeker::_set(TiObject *self, TiObject const *ref, TiObject *target, SetCallback const &cb, Word flags)
{
  PREPARE_SELF(seeker, Seeker);
  if (ref->isA<Ast::Identifier>()) {
    seeker->setByIdentifier(static_cast<Ast::Identifier const*>(ref), target, cb, flags);
  } else if (ref->isA<Ast::LinkOperator>()) {
    seeker->setByLinkOperator(static_cast<Ast::LinkOperator const*>(ref), target, cb, flags);
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("ref"), STR("Unrecognized reference type."));
  }
}


void Seeker::_remove(TiObject *self, TiObject const *ref, TiObject *target, RemoveCallback const &cb, Word flags)
{
  PREPARE_SELF(seeker, Seeker);
  if (ref->isA<Ast::Identifier>()) {
    seeker->removeByIdentifier(static_cast<Ast::Identifier const*>(ref), target, cb, flags);
  } else if (ref->isA<Ast::LinkOperator>()) {
    seeker->removeByLinkOperator(static_cast<Ast::LinkOperator const*>(ref), target, cb, flags);
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("ref"), STR("Unrecognized reference type."));
  }
}


void Seeker::_foreach(TiObject *self, TiObject const *ref, TiObject *target, ForeachCallback const &cb, Word flags)
{
  PREPARE_SELF(seeker, Seeker);
  if (ref->isA<Ast::Identifier>()) {
    seeker->foreachByIdentifier(static_cast<Ast::Identifier const*>(ref), target, cb, flags);
  } else if (ref->isA<Ast::LinkOperator>()) {
    seeker->foreachByLinkOperator(static_cast<Ast::LinkOperator const*>(ref), target, cb, flags);
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("ref"), STR("Unrecognized reference type."));
  }
}


//==============================================================================
// Identifier set

void Seeker::_setByIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, SetCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<SharedRepository>()) {
    auto repo = static_cast<SharedRepository*>(data);
    for (Int i = repo->getLevelCount() - 1; i >= 0; --i) {
      auto data = repo->getLevelData(i).get();
      if (data == 0) continue;
      if (!Seeker::isMove(seeker->setByIdentifier_level(identifier, data, cb, flags))) return;
    }
  } else if (data->isDerivedFrom<Node>()) {
    auto node = static_cast<Node*>(data);
    while (node != 0) {
      if (!Seeker::isMove(seeker->setByIdentifier_level(identifier, node, cb, flags))) return;
      if (flags & Seeker::Flags::SKIP_OWNERS) break;
      node = node->getOwner();
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Invalid data type."));
  }
}


Seeker::Verb Seeker::_setByIdentifier_level(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, SetCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<Ast::Scope>()) {
    return seeker->setByIdentifier_scope(identifier, static_cast<Ast::Scope*>(data), cb, flags);
  } else {
    return Seeker::Verb::MOVE;
  }
}


Seeker::Verb Seeker::_setByIdentifier_scope(
  TiObject *self, Data::Ast::Identifier const *identifier, Ast::Scope *scope, SetCallback const &cb, Word flags
) {
  Seeker::Verb verb = Seeker::Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (isPerform(verb)) {
        def->setTarget(getSharedPtr(obj));
      }
      if (!Seeker::isMove(verb)) break;
    }
  }
  if (Seeker::isMove(verb)) {
    TiObject *obj = 0;
    verb = cb(obj, 0);
    if (isPerform(verb)) {
      // Add a new definition.
      auto def = Data::Ast::Definition::create();
      def->setName(identifier->getValue());
      def->setTarget(getSharedPtr(obj));
      scope->add(def);
    }
  }
  return verb;
}


//==============================================================================
// Identifier remove

void Seeker::_removeByIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, RemoveCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<SharedRepository>()) {
    auto repo = static_cast<SharedRepository*>(data);
    for (Int i = repo->getLevelCount() - 1; i >= 0; --i) {
      auto data = repo->getLevelData(i).get();
      if (data == 0) continue;
      if (!Seeker::isMove(seeker->removeByIdentifier_level(identifier, data, cb, flags))) return;
    }
  } else if (data->isDerivedFrom<Node>()) {
    auto node = static_cast<Node*>(data);
    while (node != 0) {
      if (!Seeker::isMove(seeker->removeByIdentifier_level(identifier, node, cb, flags))) return;
      if (flags & Seeker::Flags::SKIP_OWNERS) break;
      node = node->getOwner();
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Invalid data type."));
  }
}


Seeker::Verb Seeker::_removeByIdentifier_level(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, RemoveCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<Ast::Scope>()) {
    return seeker->removeByIdentifier_scope(identifier, static_cast<Ast::Scope*>(data), cb, flags);
  } else {
    return Seeker::Verb::MOVE;
  }
}


Seeker::Verb Seeker::_removeByIdentifier_scope(
  TiObject *self, Data::Ast::Identifier const *identifier, Ast::Scope *scope, RemoveCallback const &cb, Word flags
) {
  Seeker::Verb verb = Seeker::Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (isPerform(verb)) {
        scope->remove(i);
        --i;
      }
      if (!Seeker::isMove(verb)) return verb;
    }
  }
  return verb;
}


//==============================================================================
// Identifier foreach

void Seeker::_foreachByIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, ForeachCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<SharedRepository>()) {
    auto repo = static_cast<SharedRepository*>(data);
    for (Int i = repo->getLevelCount() - 1; i >= 0; --i) {
      auto data = repo->getLevelData(i).get();
      if (data == 0) continue;
      if (!Seeker::isMove(seeker->foreachByIdentifier_level(identifier, data, cb, flags))) return;
    }
  } else if (data->isDerivedFrom<Node>()) {
    auto node = static_cast<Node*>(data);
    while (node != 0) {
      if (!Seeker::isMove(seeker->foreachByIdentifier_level(identifier, node, cb, flags))) return;
      if (flags & Seeker::Flags::SKIP_OWNERS) break;
      node = node->getOwner();
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Invalid data type."));
  }
}


Seeker::Verb Seeker::_foreachByIdentifier_level(
  TiObject *self, Data::Ast::Identifier const *identifier, TiObject *data, ForeachCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (data->isDerivedFrom<Ast::Scope>()) {
    return seeker->foreachByIdentifier_scope(identifier, static_cast<Ast::Scope*>(data), cb, flags);
  } else {
    return Seeker::Verb::MOVE;
  }
}


Seeker::Verb Seeker::_foreachByIdentifier_scope(
  TiObject *self, Data::Ast::Identifier const *identifier, Ast::Scope *scope, ForeachCallback const &cb, Word flags
) {
  Seeker::Verb verb = Seeker::Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (!Seeker::isMove(verb)) return verb;
    }
  }
  return verb;
}


//==============================================================================
// LinkOperator set

void Seeker::_setByLinkOperator(
  TiObject *self, Ast::LinkOperator const *link, TiObject *data, SetCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  auto first = link->getFirst().get();
  seeker->doForeach(first, data,
    [=](TiObject *newData, Notice*)->Verb
    {
      return seeker->setByLinkOperator_routing(link, newData, cb, flags);
    },
    flags
  );
}


Seeker::Verb Seeker::_setByLinkOperator_routing(
  TiObject *self, Ast::LinkOperator const *link, TiObject *data, SetCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (link->getType() == STR(".")) {
    auto second = link->getSecond().get();
    if (second->isA<Ast::Identifier>()) {
      if (data->isDerivedFrom<Ast::Scope>()) {
        return seeker->setByLinkOperator_scopeDotIdentifier(
          static_cast<Ast::Identifier*>(second), static_cast<Ast::Scope*>(data), cb, flags
        );
      } else {
        MapContainer *map = ti_cast<MapContainer>(data);
        if (map != 0) {
          return seeker->setByLinkOperator_mapDotIdentifier(static_cast<Ast::Identifier*>(second), map, cb, flags);
        } else {
          throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Unrecognized target data type."));
        }
      }
    } else {
      throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unrecognized type for link operator's second part."));
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unknown link operator type."), link->getType());
  }
}


Seeker::Verb Seeker::_setByLinkOperator_scopeDotIdentifier(
  TiObject *self, Ast::Identifier const *identifier, Ast::Scope *scope, SetCallback const &cb, Word flags
) {
  Verb verb = Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (isPerform(verb)) {
        def->setTarget(getSharedPtr(obj));
      }
      if (!Seeker::isMove(verb)) break;
    }
  }
  if (Seeker::isMove(verb)) {
    TiObject *obj = 0;
    verb = cb(obj, 0);
    if (isPerform(verb)) {
      // Add a new definition.
      auto def = Data::Ast::Definition::create();
      def->setName(identifier->getValue());
      def->setTarget(getSharedPtr(obj));
      scope->add(def);
    }
  }
  return verb;
}


Seeker::Verb Seeker::_setByLinkOperator_mapDotIdentifier(
  TiObject *self, Ast::Identifier const *identifier, MapContainer *map, SetCallback const &cb, Word flags
) {
  Verb verb = Verb::MOVE;
  auto obj = map->get(identifier->getValue().get());
  verb = cb(obj, 0);
  if (isPerform(verb)) {
    map->set(identifier->getValue().get(), obj);
  }
  return verb;
}


//==============================================================================
// LinkOperator remove

void Seeker::_removeByLinkOperator(
  TiObject *self, Data::Ast::LinkOperator const *link, TiObject *data, RemoveCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  auto first = link->getFirst().get();
  seeker->doForeach(first, data,
    [=](TiObject *newData, Notice*)->Verb
    {
      return seeker->removeByLinkOperator_routing(link, newData, cb, flags);
    },
    flags
  );
}


Seeker::Verb Seeker::_removeByLinkOperator_routing(
  TiObject *self, Data::Ast::LinkOperator const *link, TiObject *data, RemoveCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (link->getType() == STR(".")) {
    auto second = link->getSecond().get();
    if (second->isA<Ast::Identifier>()) {
      if (data->isDerivedFrom<Ast::Scope>()) {
        return seeker->removeByLinkOperator_scopeDotIdentifier(
          static_cast<Ast::Identifier*>(second), static_cast<Ast::Scope*>(data), cb, flags
        );
      } else {
        MapContainer *map = ti_cast<MapContainer>(data);
        if (map != 0) {
          return seeker->removeByLinkOperator_mapDotIdentifier(static_cast<Ast::Identifier*>(second), map, cb, flags);
        } else {
          throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Unrecognized target data type."));
        }
      }
    } else {
      throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unrecognized type for link operator's second part."));
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unknown link operator type."), link->getType());
  }
}


Seeker::Verb Seeker::_removeByLinkOperator_scopeDotIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, Data::Ast::Scope *scope, RemoveCallback const &cb, Word flags
) {
  Verb verb = Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (isPerform(verb)) {
        scope->remove(i);
        --i;
      }
      if (!Seeker::isMove(verb)) break;
    }
  }
  return verb;
}


Seeker::Verb Seeker::_removeByLinkOperator_mapDotIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, Data::MapContainer *map, RemoveCallback const &cb, Word flags
) {
  Verb verb = Verb::MOVE;
  auto index = map->findIndex(identifier->getValue().get());
  if (index != -1) {
    auto obj = map->get(index);
    verb = cb(obj, 0);
    if (isPerform(verb)) {
      map->remove(index);
    }
  }
  return verb;
}


//==============================================================================
// LinkOperator foreach

void Seeker::_foreachByLinkOperator(
  TiObject *self, Data::Ast::LinkOperator const *link, TiObject *data, ForeachCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  auto first = link->getFirst().get();
  seeker->doForeach(first, data,
    [=](TiObject *newData, Notice*)->Verb
    {
      return seeker->foreachByLinkOperator_routing(link, newData, cb, flags);
    },
    flags
  );
}


Seeker::Verb Seeker::_foreachByLinkOperator_routing(
  TiObject *self, Data::Ast::LinkOperator const *link, TiObject *data, ForeachCallback const &cb, Word flags
) {
  PREPARE_SELF(seeker, Seeker);
  if (link->getType() == STR(".")) {
    auto second = link->getSecond().get();
    if (second->isA<Ast::Identifier>()) {
      if (data->isDerivedFrom<Ast::Scope>()) {
        return seeker->foreachByLinkOperator_scopeDotIdentifier(
          static_cast<Ast::Identifier*>(second), static_cast<Ast::Scope*>(data), cb, flags
        );
      } else {
        MapContainer *map = ti_cast<MapContainer>(data);
        if (map != 0) {
          return seeker->foreachByLinkOperator_mapDotIdentifier(static_cast<Ast::Identifier*>(second), map, cb, flags);
        } else {
          throw EXCEPTION(InvalidArgumentException, STR("data"), STR("Unrecognized target data type."));
        }
      }
    } else {
      throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unrecognized type for link operator's second part."));
    }
  } else {
    throw EXCEPTION(InvalidArgumentException, STR("link"), STR("Unknown link operator type."), link->getType());
  }
}


Seeker::Verb Seeker::_foreachByLinkOperator_scopeDotIdentifier(
  TiObject *self, Data::Ast::Identifier *identifier, Data::Ast::Scope *scope, ForeachCallback const &cb, Word flags
) {
  Verb verb = Verb::MOVE;
  for (Int i = 0; i < scope->getCount(); ++i) {
    auto def = ti_cast<Data::Ast::Definition>(scope->get(i));
    if (def != 0 && def->getName() == identifier->getValue()) {
      auto obj = def->getTarget().get();
      verb = cb(obj, 0);
      if (!Seeker::isMove(verb)) break;
    }
  }
  return verb;
}


Seeker::Verb Seeker::_foreachByLinkOperator_mapDotIdentifier(
  TiObject *self, Data::Ast::Identifier const *identifier, Data::MapContainer *map, ForeachCallback const &cb,
  Word flags
) {
  auto obj = map->get(identifier->getValue().get());
  return cb(obj, 0);
}

} } // namespace
