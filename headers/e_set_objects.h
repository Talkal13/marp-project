#include <set>
#include <algorithm>
#include "e_object.h"


class e_set_objects {
    public:
        std::set<e_object> _o;
        double _v;

        e_set_objects() {
            _o = std::set<e_object>();
            _v = 0;
        }

        e_set_objects(std::set<e_object> o) {
            _o = o;
            _v = 0;
            for (auto it = _o.begin(); it != _o.end(); ++it) {
                _v += it->v;
            }
        }

        e_set_objects(e_object o) {
            _o = std::set<e_object>();
            _o.insert(o);
            _v = o.v;

        }

        void add(e_object element) {
            _o.insert(element);
            _v += element.v;
        }

        int size() {
            return _o.size();
        }

}