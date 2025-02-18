//© Copyright 2014-2022, Juan Linietsky, Ariel Manzur and the Godot community (CC-BY 3.0)
#ifndef CARDUTIL_CLASS_H
#define CARDUTIL_CLASS_H

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class CardUtil : public RefCounted
{
    GDCLASS(CardUtil, RefCounted);

    int count;

protected:
    static void _bind_methods();

public:
    CardUtil();
    ~CardUtil();

    void add(int p_value);
    void reset();
    int get_total() const;
};

#endif // CARDUTIL_CLASS_H
