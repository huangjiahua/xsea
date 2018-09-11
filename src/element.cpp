#include <utility>
#include "../include/xsea.h"

bool Xsea::Element::hasChildren() const {
    return !_children.empty();
}

Xsea::Element::Element(Xsea::ElementPtr p, std::size_t index) : Node(std::move(p), index) {
    _type = NodeType::_element;
}

Xsea::Element::Element(Xsea::ElementPtr p, std::size_t index, const std::string &value) :
        Node(std::move(p), index, value) {
    _type = NodeType::_element;
}

const Xsea::NodePtr Xsea::Element::frontPtr() const {
    return _children.front();
}

Xsea::NodePtr Xsea::Element::frontPtr() {
    return _children.front();
}

const Xsea::NodePtr Xsea::Element::backPtr() const {
    return _children.back();
}

Xsea::NodePtr Xsea::Element::backPtr() {
    return _children.back();
}

std::size_t Xsea::Element::findFirst(const std::string &txt) {
    for (std::size_t i = 0; i < _children.size(); i++) {
        if (_children[i]->getValue() == txt)
            return i;
    }
    return _children.size();
}

std::size_t Xsea::Element::findFirst(const char *txt) {
    return findFirst(std::string(txt));
}

std::size_t Xsea::Element::findFirst(const Xsea::NodePtr ptr) {
    for (std::size_t i = 0; i < _children.size(); i++) {
        if (_children[i] == ptr)
            return i;
    }
    return _children.size();
}

std::size_t Xsea::Element::findLast(const std::string &txt) {
    for (auto i = static_cast<int>(_children.size() - 1); i >= 0; i--) {
        if (_children[i]->getValue() == txt)
            return static_cast<size_t>(i);
    }
    return _children.size();
}

std::size_t Xsea::Element::findLast(const char *txt) {
    return findLast(std::string(txt));
}

std::size_t Xsea::Element::findLast(Xsea::NodePtr ptr) {
    for (auto i = static_cast<int>(_children.size() - 1); i >= 0; i--) {
           if (_children[i] == ptr)
               return static_cast<size_t>(i);
    }
    return _children.size();
}

const Xsea::NodePtr Xsea::Element::ptrAt(std::size_t index) const {
    return _children[index];
}

Xsea::NodePtr Xsea::Element::ptrAt(std::size_t index) {
    return _children[index];
}




void Xsea::Element::addAttribute(const Xsea::Attribute &attribute) {
    _attributes.push_back(attribute);
}

Xsea::Attribute Xsea::Element::getAttribute(const std::string &key) const {
    auto iter = std::find_if(_attributes.begin(), _attributes.end(),
            [&](const Attribute& a) {
        return a.first == key;
    });
    return *iter;
}

void Xsea::Element::clear() {
    Node::clear();
    _children.clear();
    _attributes.clear();
}

const Xsea::Node &Xsea::Element::front() const {
    return *_children.front();
}

Xsea::Node &Xsea::Element::front() {
    return *_children.front();
}

const Xsea::Node &Xsea::Element::back() const {
    return *_children.back();
}

Xsea::Node &Xsea::Element::back() {
    return *_children.back();
}

const std::vector<Xsea::Attribute> &Xsea::Element::getAllAttributes() const {
    return _attributes;
}

std::vector<Xsea::Attribute> &Xsea::Element::getAllAttributes() {
    return _attributes;
}

const Xsea::Node &Xsea::Element::at(std::size_t index) const {
    return *_children[index];
}

Xsea::Node &Xsea::Element::at(std::size_t index) {
    return *_children[index];
}

std::size_t Xsea::Element::size() const {
    return _children.size();
}


Xsea::NodePtr Xsea::Element::add(Xsea::NodeType type, const std::string &value) {
    auto thisPtr = std::dynamic_pointer_cast<Element>(shared_from_this());
    switch (type) {
        case NodeType::_element: {
            _children.emplace_back(new Element(thisPtr, _children.size(), value));
            return _children.back();
        }
        case NodeType::_text: {
            _children.emplace_back(new Text(thisPtr, _children.size(), value));
            return _children.back();
        }
        case NodeType::_comment: {
            _children.emplace_back(new Comment(thisPtr, _children.size(), value));
            return _children.back();
        }
        case NodeType::_unknown: {
            _children.emplace_back(new Unknown(thisPtr, _children.size(), value));
            return _children.back();
        }
        default: return thisPtr;
    }
}

Xsea::NodePtr Xsea::Element::add(Xsea::NodeType type, const char *value) {
    return add(type, std::string(value));
}

Xsea::NodePtr Xsea::Element::link(Xsea::NodePtr ptr) {
    NodeType type = ptr->getType();
    auto thisPtr = std::dynamic_pointer_cast<Element>(shared_from_this());
    switch (type) {
        case NodeType::_element: {
            ElementPtr newPtr(new Element(thisPtr, _children.size(), ptr->getValue()));
            ElementPtr tmpPtr = std::dynamic_pointer_cast<Element>(ptr);
            newPtr->_children = tmpPtr->_children;
            newPtr->_attributes = tmpPtr->_attributes;
            _children.push_back(newPtr);
            return newPtr;
        }
        case NodeType::_text: {
            TextPtr newPtr(new Text(thisPtr, _children.size(), ptr->getValue()));
            _children.push_back(newPtr);
            return newPtr;
        }
        case NodeType::_comment: {
            CommentPtr newPtr(new Comment(thisPtr, _children.size(), ptr->getValue()));
            _children.push_back(newPtr);
            return newPtr;
        }
        case NodeType::_unknown: {
            UnknownPtr newPtr(new Unknown(thisPtr, _children.size(), ptr->getValue()));
            _children.push_back(newPtr);
            return newPtr;
        }
        default: return ptr;
    }
}

Xsea::NodePtr Xsea::Element::insert(std::size_t index,
                                    Xsea::NodeType type, const std::string &value) {
    NodePtr newPtr;
    switch (type) {
        case NodeType::_element: {
            newPtr.reset(new Element(_parent.lock(), _index, value));
            break;
        }
        case NodeType::_text: {
            newPtr.reset(new Text(_parent.lock(), _index, value));
            break;
        }
        case NodeType::_comment: {
            newPtr.reset(new Comment(_parent.lock(), _index, value));
            break;
        }
        case NodeType::_unknown: {
            newPtr.reset(new Unknown(_parent.lock(), _index, value));
            break;
        }
        default: return newPtr;
    }
    _children.push_back(nullptr);
    for (auto i = _children.size() - 1; i > index; i--) {
        _children[i] = _children[i - 1];
        _children[i]->_index = i;
    }
    _children[index] = newPtr;
    return newPtr;
}

Xsea::NodePtr Xsea::Element::insert(std::size_t index, Xsea::NodeType type, const char *value) {
    return insert(index, type, std::string(value));
}

Xsea::NodePtr Xsea::Element::link(std::size_t index, Xsea::NodePtr ptr) {
    NodePtr retPtr;
    NodeType type = ptr->getType();
    auto thisPtr = std::dynamic_pointer_cast<Element>(shared_from_this());
    switch (type) {
        case NodeType::_element: {
            ElementPtr newPtr(new Element(thisPtr, _children.size(), ptr->getValue()));
            ElementPtr tmpPtr = std::dynamic_pointer_cast<Element>(ptr);
            newPtr->_children = tmpPtr->_children;
            newPtr->_attributes = tmpPtr->_attributes;
            retPtr = newPtr;
            break;
        }
        case NodeType::_text: {
            TextPtr newPtr(new Text(thisPtr, _children.size(), ptr->getValue()));
            retPtr = newPtr;
            break;
        }
        case NodeType::_comment: {
            CommentPtr newPtr(new Comment(thisPtr, _children.size(), ptr->getValue()));
            retPtr = newPtr;
            break;
        }
        case NodeType::_unknown: {
            UnknownPtr newPtr(new Unknown(thisPtr, _children.size(), ptr->getValue()));
            retPtr = newPtr;
            break;
        }
        default: return ptr;
    }
    _children.push_back(nullptr);
    for (auto i = _children.size() - 1; i > index; i--) {
        _children[i] = _children[i - 1];
        _children[i]->_index = i;
    }
    _children[index] = retPtr;
    return retPtr;
}

Xsea::NodePtr Xsea::Element::remove() {
    _children.pop_back();
    return _children.back();
}

Xsea::NodePtr Xsea::Element::remove(std::size_t index) {
    std::size_t sz = _children.size() - 1;
    for (std::size_t i = index; i < sz; i++) {
        _children[i] = _children[i + 1];
        _children[i]->_index = i;
    }
    _children.pop_back();
    return index == 0 ? nullptr : _children[index - 1];
}


Xsea::Attribute::Attribute(const std::string &key, const std::string &value) :
        pair(key, value) { }

std::string Xsea::Attribute::getKey() const {
    return first;
}

std::string Xsea::Attribute::getValue() const {
    return second;
}

