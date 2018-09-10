#ifndef XSEA_XSEA_H
#define XSEA_XSEA_H
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stack>
#include <iostream>

namespace Xsea {

class Document;
class Node;
class Declaration;
class Element;
class Nonelement;
class Comment;
class Text;
class Unknown;
class Attribute;

// type alias
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Declaration> DeclarationPtr;
typedef std::shared_ptr<Element> ElementPtr;
typedef std::shared_ptr<Nonelement> NonelementPtr;
typedef std::shared_ptr<Comment> CommentPtr;
typedef std::shared_ptr<Text> TextPtr;
typedef std::shared_ptr<Unknown> UnknownPtr;
typedef std::shared_ptr<Attribute> AttributePtr;


enum class NodeType {
    _node, _declaration, _element, _nonelement, _comment, _text, _unknown, _back
};

class Document {
private:
    // node data
    DeclarationPtr _declarationPtr;
    ElementPtr _root;

    // data
    std::string _filename;
    std::string _error;

    // utility member function
    bool construct(std::istream& is); // construct the DOM tree
    inline static std::size_t startLine(const std::string& line); // jump off the space chars
    inline static NodeType judgeType(const std::string& line, std::size_t start); // judge the type
    inline static std::string tagName(const std::string& line, std::size_t start=0); // get the tag name between <>
    inline static std::string parseComment(const std::string& line, std::size_t start); // get comment value
    inline static std::string parseText(const std::string& line); // get text value
public:
    // constructor
    Document();
    explicit Document(const char* docName);
    explicit Document(const std::string& docName);

    // destructor
    ~Document() = default;

    // io
      // load
    bool loadFile(); // load the file according to the filename passed when constructed
    bool loadFile(const char* fileName); // load according to the parameter
    bool loadFile(const std::string& fileName); // load according to the parameter
    bool load(std::istream& is);

      // save
    void saveFile() const; // save the file according to the filename when loaded
    void saveFile(const char* fileName) const; // save the file according to the parameter
    void saveFile(const std::string& fileName) const; // same as above

    // observer
    ElementPtr getRootPtr() const;
    ElementPtr getRootPtr();
    const Element& getRoot() const;
    Element& getRoot();
    const DeclarationPtr getDeclarationPtr() const;
    DeclarationPtr getDeclarationPtr();
    const Declaration& getDeclaration() const;
    Declaration& getDeclaration();
    std::string getName() const;
    const char* getNameC() const;
    std::string getError() const;
    const char* getErrorC() const;
};



class Node {
    friend class Document;
    friend class Element;
public:
    // observer
    std::string getValue() const; // get the tag name of element / text of text / ...
    const char* getValueC() const; // c_type string as above
    const ElementPtr getParentPtr() const; // return the parent elementary ptr
    ElementPtr getParentPtr();
    const Element& getParent() const;
    Element& getParent();
    bool isRoot() const; // whether the node is root node
    NodeType getType() const; // return the type enum value
    const NodePtr previousPtr() const; // get previous node pointer under same parent
    NodePtr previousPtr();
    const Node& previous() const;
    Node& previous();
    inline const NodePtr nextPtr() const; // get next node pointer under same parent
    inline NodePtr nextPtr();
    const Node& next() const;
    Node& next();
    std::size_t index() const;
    virtual bool hasChildren() const = 0; // if the node has a child
    NodePtr getThisPtr();

    // modifier
    inline void setValue(const std::string& txt);
    inline void setValue(const char* txt);

    virtual void clear();
protected:
    std::string _value;
    std::weak_ptr<Element> _parent;
    std::size_t _index;
    NodeType _type = NodeType::_node;

    // constructor
    Node(ElementPtr parent, std::size_t index, const std::string& value);
    Node(ElementPtr parent, std::size_t index);
    NodePtr shared_from_this();
    const NodePtr shared_from_this() const;
};

class Nonelement: public Node {
public:
    // constructor
    Nonelement(ElementPtr p, std::size_t index);
    Nonelement(ElementPtr p, std::size_t index, const std::string& value);
    // virtual destructor
    // observer
    bool hasChildren() const override;
};

class Element: public Node {
public:
    friend class Document;
    friend class Node;
    // destructor

    // observer
    bool hasChildren() const override;
    const NodePtr frontPtr() const;
    NodePtr frontPtr();
    const Node& front() const;
    Node& front();
    const NodePtr backPtr() const;
    NodePtr backPtr();
    const Node& back() const;
    Node& back();
    std::size_t findFirst(const std::string& txt); // find the first with value
    std::size_t findFirst(const char* txt);
    std::size_t findFirst(NodePtr ptr);
    std::size_t findLast(const std::string& txt); // find the last with value
    std::size_t findLast(const char* txt);
    std::size_t findLast(NodePtr ptr);
    Attribute getAttribute(const std::string& key) const; // get attribute with key
    const std::vector<Attribute>& getAllAttributes() const;
    std::vector<Attribute>& getAllAttributes();
    const NodePtr ptrAt(std::size_t index) const;
    NodePtr ptrAt(std::size_t index);
    const Node& at(std::size_t index) const;
    Node& at(std::size_t index);
    std::size_t size() const;



    // modifier
    void addAttribute(const Attribute& attribute);
    void clear() override;
    NodePtr add(NodeType type, const std::string &value);
    NodePtr add(NodeType type, const char *value);
    NodePtr link(NodePtr ptr);

    NodePtr insert(std::size_t index, NodeType type, const std::string &value);
    NodePtr insert(std::size_t index, NodeType type, const char *value);
    NodePtr link(std::size_t index, NodePtr ptr);
protected:
    // constructor
    Element(ElementPtr p, std::size_t index);
    Element(ElementPtr p, std::size_t index, const std::string& value);
private:
    std::vector<NodePtr> _children;
    std::vector<Attribute> _attributes;
};

class Text: public Nonelement {
public:
    friend class Document;
    friend class Element;
    // destructor

protected:
    // constructor
    Text(ElementPtr p, std::size_t index);
    Text(ElementPtr p, std::size_t index, const std::string& value);

};

class Declaration: public Nonelement {
public:
    friend class Document;
    // destructor
protected:
    Declaration(ElementPtr p, std::size_t index);
    Declaration(ElementPtr p, std::size_t index, const std::string& value);

};

class Comment: public Nonelement {
public:
    friend class Document;
    friend class Element;
    // destructor
protected:
    Comment(ElementPtr p, std::size_t index);
    Comment(ElementPtr p, std::size_t index, const std::string& value);
};

class Unknown: public Nonelement {
public:
    friend class Document;
    friend class Element;
    // destructor
protected:
    Unknown(ElementPtr p, std::size_t index);
    Unknown(ElementPtr p, std::size_t index, const std::string& value);
};

class Attribute : public std::pair<std::string, std::string> {
public:
    Attribute(const std::string& key, const std::string& value);
    std::string getKey() const;
    std::string getValue() const;
};

}

#endif //XSEA_XSEA_H
