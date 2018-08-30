#include "../include/xsea.h"

void Xsea::Node::print(std::ostream &os) const {

}

std::string Xsea::Node::getValue() const {
    return _value;
}

const char *Xsea::Node::getValueC() const {
    return _value.c_str();
}

const Xsea::ElementPtr Xsea::Node::getParent() const {
    return _parent;
}

Xsea::ElementPtr Xsea::Node::getParent() {
    return _parent;
}

Xsea::NodeType Xsea::Node::getType() const {
    return _type;
}

const Xsea::NodePtr Xsea::Node::previous() const {
    return _parent->at(_index - 1);
}


bool Xsea::Node::isRoot() const {
    return _parent == nullptr;
}

Xsea::NodePtr Xsea::Node::previous() {
    return _parent->at(_index + 1);
}

