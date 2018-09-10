#include "../include/xsea.h"



std::string Xsea::Node::getValue() const {
    return _value;
}

const char *Xsea::Node::getValueC() const {
    return _value.c_str();
}

const Xsea::ElementPtr Xsea::Node::getParentPtr() const {
    return _parent.lock();
}

Xsea::ElementPtr Xsea::Node::getParentPtr() {
    return _parent.lock();
}

Xsea::NodeType Xsea::Node::getType() const {
    return _type;
}

const Xsea::NodePtr Xsea::Node::previousPtr() const {
    return _parent.lock()->ptrAt(_index - 1);
}

Xsea::NodePtr Xsea::Node::previousPtr() {
    return _parent.lock()->ptrAt(_index + 1);
}

const Xsea::NodePtr Xsea::Node::nextPtr() const {
    return _parent.lock()->ptrAt(_index + 1);
}

Xsea::NodePtr Xsea::Node::nextPtr() {
    return _parent.lock()->ptrAt(_index + 1);
}

bool Xsea::Node::isRoot() const {
    return _parent.lock()->_value.empty();
}

void Xsea::Node::setValue(const std::string &txt) {
    _value = txt;
}

void Xsea::Node::setValue(const char *txt) {
    _value = txt;
}

void Xsea::Node::clear() {
    _value.clear();
}

Xsea::Node::Node(Xsea::ElementPtr parent, std::size_t index, const std::string &value):
     _parent(parent), _index(index), _value(value) { }

Xsea::Node::Node(Xsea::ElementPtr parent, std::size_t index):
     _parent(parent), _index(index) { }

const Xsea::Element &Xsea::Node::getParent() const {
    return *_parent.lock();
}

Xsea::Element &Xsea::Node::getParent() {
    return *_parent.lock();
}

const Xsea::Node &Xsea::Node::previous() const {
    return *(_parent.lock()->_children[_index - 1]);
}

Xsea::Node &Xsea::Node::previous() {
    return *(_parent.lock()->_children[_index - 1]);
}

const Xsea::Node &Xsea::Node::next() const {
    return *(_parent.lock()->_children[_index + 1]);
}

Xsea::Node &Xsea::Node::next() {
    return *(_parent.lock()->_children[_index + 1]);
}

std::size_t Xsea::Node::index() const {
    return _index;
}

Xsea::NodePtr Xsea::Node::getThisPtr() {
    return shared_from_this();
}

Xsea::NodePtr Xsea::Node::shared_from_this() {
    return _parent.lock()->_children[_index];
}

const Xsea::NodePtr Xsea::Node::shared_from_this() const {
    return _parent.lock()->_children[_index];
}





