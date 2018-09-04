#include "../include/xsea.h"


Xsea::Document::Document(const char *docName): _filename(docName) { }

Xsea::Document::Document(const std::string &docName): _filename(docName) { }

bool Xsea::Document::loadFile() {
    std::ifstream is(_filename);
    if (!is.is_open())
        return false;
    return construct(is);
}

bool Xsea::Document::loadFile(const char *fileName) {
    this->_filename = fileName;
    return loadFile();
}

bool Xsea::Document::loadFile(const std::string &fileName) {
    this->_filename = fileName;
    return loadFile();
}

bool Xsea::Document::load(std::istream &is) {
    if (!is) return false;
    else return construct(is);
}

void Xsea::Document::saveFile() const {
    saveFile(_filename);
}

void Xsea::Document::saveFile(const char *fileName) const {
    saveFile(std::string(fileName));
}

void Xsea::Document::saveFile(const std::string &fileName) const {
    std::ofstream os(fileName);
    os << '<' << _declarationPtr->getValue() << ">\n" << std::endl;
    // TODO
}

Xsea::ElementPtr Xsea::Document::getRoot() const {
    for (NodePtr p : _root->_children) {
        if (p->getType() == NodeType::_element)
            return std::dynamic_pointer_cast<Element>(p);
    }
    return nullptr;
}

Xsea::DeclarationPtr Xsea::Document::getDeclaration() const {
    return _declarationPtr;
}

std::string Xsea::Document::getName() const {
    return _filename;
}

const char *Xsea::Document::getNameC() const {
    return _filename.c_str();
}

bool Xsea::Document::construct(std::istream &is) {
    std::string line;
    std::stack<ElementPtr> st;
    _root = std::make_shared<Element>(nullptr, 0, "");
    ElementPtr curr = _root;
    bool oneRoot = false;

    if (std::getline(is, line, '>')) {
        std::size_t start = startLine(line);
        if (line.substr(start, 5) == "<?xml") { // there is a declaration
            _declarationPtr = std::make_shared<Declaration>(nullptr, 0,
                                                            line.substr(start + 1));
            if (!std::getline(is, line, '>')) {
                _error += "No root tag\n";
                return false;
            }
        }
    } else {
        _error += "No root tag.\n";
        return false; // no root tag
    }


    do {
        std::size_t start = startLine(line);
        if (start == line.size() - 1) {
            _error += "Wrong syntax at " + line + '\n';
            return false;
        }

        NodeType type = judgeType(line, start);
        if (type == NodeType::_back) {
            if (!curr->hasChildren() && line[0] != '<')
                type = NodeType::_text;
        }

        if (oneRoot) { // the remains can only be comments
            if (type != NodeType::_comment)
                break;
        }

        switch (type) {
            case NodeType::_element: {
                curr->_children.push_back(
                        std::make_shared<Element>(curr, curr->_children.size() - 1,
                                                  tagName(line)));
                if (line.back() != '/') // not like <tag/>
                    curr = std::dynamic_pointer_cast<Element>(curr->_children.back());
                break;
            }
            case NodeType::_back: {
                std::string tag = tagName(line, start);
                if (tag != curr->_value) {
                    _error += "Back tag doesn't match previous tag at " + line + '\n';
                    return false;
                }
                curr = curr->_parent.lock();
                if (curr == _root) oneRoot = true;
                break;
            }
            case NodeType::_text: {
                std::string txt = parseText(line);
                std::size_t tagStart = txt.size();
                curr->_children.push_back(
                        std::make_shared<Text>(curr, curr->_children.size() - 1,
                                               std::move(txt)));
                std::string tag = tagName(line, tagStart);
                if (tag != curr->_value) {
                    _error += "Back tag doesn't match previous tag at " + line + '\n';
                    return false;
                }
                curr = curr->_parent.lock();
                if (curr == _root) oneRoot = true;
                break;
            }
            case NodeType::_comment: {
                if (line.size() - start < 7) { // <!----> has 7 characters
                    _error += "Comment syntax error at " + line + '\n';
                    return false;
                }
                curr->_children.push_back(
                        std::make_shared<Comment>(curr, curr->_children.size() - 1,
                                                  parseComment(line, start)));
                break;
            }
            case NodeType::_unknown: {
                curr->_children.push_back(
                        std::make_shared<Unknown>(curr, curr->_children.size() - 1,
                                                  line.substr(start + 1)));
                break;
            }
            default: break;
        }
    } while (std::getline(is, line, '>'));

    if (!oneRoot) { // there is no root
        _error += "No root\n";
        return false;
    }

    return true;
}

std::size_t Xsea::Document::startLine(const std::string &line) {
    return line.find_first_not_of(" \t\v\n\r");
}

bool Xsea::Document::allSpace(const std::string &line) {
    for (char c : line)
        if (!isspace(c))
            return false;
    return true;
}

std::pair<std::size_t, std::size_t>
        Xsea::Document::findtag(const std::string &line, const size_t pos) {
    std::pair<std::size_t, std::size_t> ret;
    ret.first = line.find_first_of('<', pos);
    ret.second = line.find_first_of('>', ret.first);
    return ret;
}

Xsea::NodeType Xsea::Document::judgeType(const std::string& line, std::size_t start) {
    if (line[start] != '<')
        return NodeType::_text;
    else if (line[start + 1] == '/')
        return NodeType::_back;
    else if (line.substr(start, 4) == "<!--")
        return NodeType::_comment;
    else if (isalpha(line[start + 1]))
        return NodeType::_element;
    else
        return NodeType::_unknown;
}

std::string Xsea::Document::tagName(const std::string &line, std::size_t start) {
    std::size_t b = line.find('<', start);
    std::string ret;
    if (line[b + 1] == '/')
        b++;
    ret = line.substr(b + 1);
    if (ret.back() == '/')
        ret.pop_back();
    return ret;
}

std::string Xsea::Document::parseComment(const std::string &line, std::size_t start) {
    return line.substr(start, line.size() - 4 - 3 - start);
}

std::string Xsea::Document::parseText(const std::string &line) {
    std::string ret;
    ret.reserve(line.size() - 2);
    std::size_t i = 0;
    if (line[0] == '\n') i++;

    for (; line[i] != '<' && i < line.size(); i++) {
        ret.push_back(line[i]);
    }
    return std::move(ret);
}

std::string Xsea::Document::getError() const {
    return _error;
}




