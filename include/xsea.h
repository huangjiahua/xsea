#ifndef XSEA_XSEA_H
#define XSEA_XSEA_H
#include <string>
#include <vector>
#include <memory>
#include <fstream>

namespace Xsea {

class Document;
class Node;
class Declaration;
class Element;
class Nonelement;
class Comment;
class Text;
class Unknown;

// type alias
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Declaration> DeclarationPtr;
typedef std::shared_ptr<Element> ElementPtr;
typedef std::shared_ptr<Nonelement> NonelementPtr;
typedef std::shared_ptr<Comment> CommentPtr;
typedef std::shared_ptr<Text> TextPtr;
typedef std::shared_ptr<Unknown> UnknownPtr;


enum NodeType {
    _node, _declaration, _element, _nonelement, _comment, _text, _unknown
};

class Document {
private:
    // node data
    DeclarationPtr _declarationPtr;
    ElementPtr _root;

    // data
    std::string _filename;
public:
    // constructor
    Document() = default;
    explicit Document(const char* docName);
    explicit Document(const std::string& docName);

    // destructor
    ~Document() = default;

    // io
      // load
    bool loadFile(); // load the file according to the filename passed when constructed
    bool loadFile(const char* fileName); // load according to the parameter
    bool loadFile(const std::string& fileName); // load according to the parameter

      // save
    void saveFile() const; // save the file according to the filename when loaded
    void saveFile(const char* fileName) const; // save the file according to the parameter
    void saveFile(const std::string& fileName) const; // same as above

    // observer
    inline ElementPtr getRoot() const;
    inline DeclarationPtr getDeclaration() const;
    inline std::string getName() const;
    const char* getNameC() const;
};




class Node {
    friend class Document;
public:

    // observer
    inline std::string getValue() const;
    inline const char* getValueC() const;

    inline const ElementPtr getParent() const;
    inline ElementPtr getParent();

    inline bool isRoot() const;

    NodeType getType() const;

    const NodePtr previous() const;
    NodePtr previous();

    const NodePtr next() const;
    NodePtr next();

    virtual bool hasChildren() const;

    virtual void print(std::ostream& os) const;

    // modifier
    void setValue(const std::string& txt);
    void setValue(const char* txt);

    void clear();
protected:
    std::string _value;
    ElementPtr _parent;
    std::size_t _index;
    NodeType _type;

    // member functions
    static void construct(NodePtr ptr);
};

class Nonelement: public Node {
public:
    // observer
    bool hasChildren() const override;
};

class Element: public Node {
public:
    // observer
    const NodePtr front() const;
    NodePtr front();

    const NodePtr back() const;
    NodePtr back();

    std::size_t findFirst(const std::string& txt);
    std::size_t findFirst(const char* txt);
    std::size_t findFirst(const NodePtr ptr);

    std::size_t findLast(const std::string& txt);
    std::size_t findLast(const char* txt);
    std::size_t findLast(const NodePtr ptr);

    const NodePtr at(std::size_t index) const;
    NodePtr at(std::size_t index);
    // modifier
    void push_back(const Node& n);
    void push_back(const NodePtr np);

    void insert(std::size_t index, const Node& n);
    void insert(std::size_t index, const NodePtr np);

    void replace(std::size_t index, const Node& n);
    void replace(std::size_t index, const NodePtr np);

    void erase(std::size_t index);
    void erase(std::size_t index, std::size_t amount);

private:
    std::vector<NodePtr> _children;
};

class Text: public Nonelement {

};

class Declaration: public Nonelement {

};

class Comment: public Nonelement {

};

class Unknown: public Nonelement {

};

}

#endif //XSEA_XSEA_H
