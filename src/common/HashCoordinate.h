#ifndef HASHMODULE_H_
#define HASHMODULE_H_

#include <utility>

#include <omnetpp.h>
#include <Coord.h>

namespace std {
   template<>
   class hash<inet::Coord> {
   public:
      size_t operator()(const inet::Coord& c) const
      {
        size_t h1 = std::hash<double>{}(c.x);
        size_t h2 = std::hash<double>{}(c.y);
        return h1 ^ (h2 << 1);
      }
   };
}


#endif /* HASHMODULE_H_ */