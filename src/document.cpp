#include "../include/xsea.h"


Xsea::Document::Document(const char *docName): _filename(docName) { }

Xsea::Document::Document(const std::string &docName): _filename(docName) { }

bool Xsea::Document::loadFile() {
    // TODO
    return false;
}

bool Xsea::Document::loadFile(const char *fileName) {
    this->_filename = fileName;
    return loadFile();
}

bool Xsea::Document::loadFile(const std::string &fileName) {
    this->_filename = fileName;
    return loadFile();
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

    _root->print(os);
}

Xsea::ElementPtr Xsea::Document::getRoot() const {
    return _root;
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











