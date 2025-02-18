//© Copyright 2014-2022, Juan Linietsky, Ariel Manzur and the Godot community (CC-BY 3.0)
#include "card_util.hpp"

#include "card.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

CardUtil::CardUtil()
{
    count = 0;
    UtilityFunctions::print("Hello from GDExtension!");
}

CardUtil::~CardUtil()
{
}

void CardUtil::add(int p_value)
{
    count += p_value;
}

void CardUtil::reset()
{
    count = 0;
}

int CardUtil::get_total() const
{
    return count;
}

void CardUtil::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("add", "value"), &CardUtil::add, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("reset"), &CardUtil::reset);
    ClassDB::bind_method(D_METHOD("get_total"), &CardUtil::get_total);
}
