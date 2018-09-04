#include "../include/xsea.h"



std::string Xsea::Node::getValue() const {
    return _value;
}

const char *Xsea::Node::getValueC() const {
    return _value.c_str();
}

const Xsea::ElementPtr Xsea::Node::getParent() const {
    return _parent.lock();
}

Xsea::ElementPtr Xsea::Node::getParent() {
    return _parent.lock();
}

Xsea::NodeType Xsea::Node::getType() const {
    return _type;
}

const Xsea::NodePtr Xsea::Node::previous() const {
    return _parent.lock()->at(_index - 1);
}

Xsea::NodePtr Xsea::Node::previous() {
    return _parent.lock()->at(_index + 1);
}

const Xsea::NodePtr Xsea::Node::next() const {
    return _parent.lock()->at(_index + 1);
}

Xsea::NodePtr Xsea::Node::next() {
    return _parent.lock()->at(_index + 1);
}

bool Xsea::Node::isRoot() const {
    return _parent.lock() == nullptr;
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
    _parent(std::move(parent)), _index(index), _value(value) { }

Xsea::Node::Node(Xsea::ElementPtr parent, std::size_t index):
    _parent(std::move(parent)), _index(index) { }

void Xsea::Node::display(std::ostream& os) {
    os << _value << std::endl;
    if (_type == NodeType::_element) {
        Node* np = this;
        Element* ep = dynamic_cast<Element*>(np);
        for (auto p : ep->_children)
            p->display(os);
        os << _value << std::endl;
    }
}

void Xsea::display(Xsea::Node &n, std::ostream& os) {
    n.display(os);
}


