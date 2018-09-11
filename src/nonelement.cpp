#include "../include/xsea.h"

bool Xsea::Nonelement::hasChildren() const {
    return false;
}

Xsea::Nonelement::Nonelement(Xsea::ElementPtr p, std::size_t index) : Node(p, index) {
    _type = NodeType::_nonelement;
}

Xsea::Nonelement::Nonelement(Xsea::ElementPtr p, std::size_t index, const std::string &value) :
        Node(p, index, value) {
    _type = NodeType::_nonelement;
}

Xsea::Text::Text(Xsea::ElementPtr p, std::size_t index) : Nonelement(p, index) {
    _type = NodeType::_text;
}

Xsea::Text::Text(Xsea::ElementPtr p, std::size_t index, const std::string &value) :
        Nonelement(p, index, value) {
    _type = NodeType ::_text;
}

Xsea::Declaration::Declaration(Xsea::ElementPtr p, std::size_t index) : Nonelement(p, index) {
    _type = NodeType::_declaration;
}

Xsea::Declaration::Declaration(Xsea::ElementPtr p, std::size_t index, const std::string &value)
        : Nonelement(p, index, value) {
    _type = NodeType::_declaration;
}


Xsea::Comment::Comment(Xsea::ElementPtr p, std::size_t index) : Nonelement(p, index) {
    _type = NodeType::_comment;
}

Xsea::Comment::Comment(Xsea::ElementPtr p, std::size_t index, const std::string &value)
        : Nonelement(p, index, value) {
    _type = NodeType::_comment;
}

Xsea::Unknown::Unknown(Xsea::ElementPtr p, std::size_t index) : Nonelement(p, index) {
    _type = NodeType::_unknown;
}

Xsea::Unknown::Unknown(Xsea::ElementPtr p, std::size_t index, const std::string &value)
        : Nonelement(p, index, value) {
    _type = NodeType::_unknown;
}
