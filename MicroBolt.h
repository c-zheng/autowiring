#pragma once
#include "Bolt.h"
#include "Autowired.h"
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

typedef void(*ctxtfnptr)(std::shared_ptr<CoreContext>);

struct MicroBoltUtilities {
  /// <summary>
  /// Instantiates and returns the SAME EXACT Vector per given Sigil Class and Container Type
  /// </summary>
  template<class SigilClass, class ContainerType>
  static std::vector<ContainerType>& return_head_of_bolt_enumeration_list(void) {
    static std::vector<ContainerType> AlwaysSameVector = std::vector<ContainerType>(); //runs once at init
    return AlwaysSameVector;
  }

  /// <summary>
  /// Makes sure any given ContainerType elt is inserted only once into given ContainerType vector 
  /// </summary>
  template<class SigilClass, class ContainerType, ContainerType x>
  static void one_time_insert(void) {
   static bool initialized;
   // Runs once at init
   if(!initialized) {
     initialized = true;
     auto list = return_head_of_list<SigilClass, ContainerType>();
     list.push_back(x);
    }
  }

  /// <summary>
  /// Enumerates all microbolts for given sigilclass and containertype
  /// </summary>
  template<class SigilClass, class ContainerType>
  static void enumerate_my_micro_bolts(std::shared_ptr<CoreContext> ctxtptr) {
	  auto list = return_head_of_list<SigilClass, ContainerType>();
	  for(ctxtfnptr myptr : list)
      // Call every callable in the vector, passing a ref to me each time
		  mpytr(ctxtptr);
  }
};

template<class SigilClass, class ContainerType, ContainerType x>
struct MicroBolt {
  MicroBolt(void) {
		MicroBoltUtilities:::one_time_insert<SigilClass, ContainerType, x>();
	}
};

template<class T>
void make_me_an_auto_func(std::shared_ptr<CoreContext> cptr) {
  CurrentContextPusher pshr(cptr);
  AutoRequired<T>;
  return;
}

// BOLT_TO = make_me_an_auto_func parameterized on a Sigil Class.
// Later, a vector of these are enumerated by the CoreContext with matching Sigil Class
#define BOLT_TO(SigilClass, TypeToInsert) namespace { \
	  auto localautofunc = make_me_an_auto_func<TypeToInsert>; \
	  MicroBolt<SigilClass, ctxtfnptr, localautofunc> x##__COUNTER__ ; \
  }