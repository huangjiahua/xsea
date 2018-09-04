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

    bool construct(std::istream& is); // construct the DOM tree
    inline static std::size_t startLine(const std::string& line);
    inline static bool allSpace(const std::string& line);
    inline static std::pair<std::size_t, std::size_t>
        findtag(const std::string& line, std::size_t pos = 0);
    inline static NodeType judgeType(const std::string& line, std::size_t start);
    inline static std::string tagName(const std::string& line, std::size_t start=0);
    inline static std::string parseComment(const std::string& line, std::size_t start);
    inline static std::string parseText(const std::string& line);
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
    bool load(std::istream& is);

      // save
    void saveFile() const; // save the file according to the filename when loaded
    void saveFile(const char* fileName) const; // save the file according to the parameter
    void saveFile(const std::string& fileName) const; // same as above

    // observer
    ElementPtr getRoot() const;
    DeclarationPtr getDeclaration() const;
    std::string getName() const;
    const char* getNameC() const;
    std::string getError() const;
};



void display(Node& n, std::ostream& os);
class Node {
    friend class Document;
    friend void display(Node& n, std::ostream& os);
public:
    // constructor
    Node(ElementPtr parent, std::size_t index, const std::string& value);
    Node(ElementPtr parent, std::size_t index);
    // virtual destructor
    virtual ~Node() = default;

    // observer
    std::string getValue() const;
    const char* getValueC() const;

    const ElementPtr getParent() const;
    ElementPtr getParent();

    bool isRoot() const;

    NodeType getType() const;

    inline const NodePtr previous() const;
    inline NodePtr previous();

    inline const NodePtr next() const;
    inline NodePtr next();

    virtual bool hasChildren() const = 0;


    // modifier
    inline void setValue(const std::string& txt);
    inline void setValue(const char* txt);

    virtual void clear();
protected:
    std::string _value;
    std::weak_ptr<Element> _parent;
    std::size_t _index;
    NodeType _type = NodeType::_node;

    void display(std::ostream& os);
    // member functions
};

class Nonelement: public Node {
public:
    // constructor
    Nonelement(ElementPtr p, std::size_t index);
    Nonelement(ElementPtr p, std::size_t index, const std::string& value);
    // virtual destructor
    ~Nonelement() override = default;
    // observer
    bool hasChildren() const override;

};

class Element: public Node {
public:
    friend class Document;
    friend class Node;
    // constructor
    Element(ElementPtr p, std::size_t index);
    Element(ElementPtr p, std::size_t index, const std::string& value);
    // destructor
    ~Element() override = default;
    // observer
    bool hasChildren() const override;
    const NodePtr front() const;
    NodePtr front();

    const NodePtr back() const;
    NodePtr back();

    std::size_t findFirst(const std::string& txt);
    std::size_t findFirst(const char* txt);
    std::size_t findFirst(NodePtr ptr);

    std::size_t findLast(const std::string& txt);
    std::size_t findLast(const char* txt);
    std::size_t findLast(NodePtr ptr);

    void addAttribute(const Attribute& attribute);

    const NodePtr at(std::size_t index) const;
    NodePtr at(std::size_t index);

    // modifier
    void push_back(NodePtr np);

    void insert(std::size_t index, NodePtr np);

    void replace(std::size_t index, NodePtr np);

    void erase(std::size_t index);
    void erase(std::size_t index, std::size_t amount);

    Attribute getAttribute(const std::string& key) const;

private:
    std::vector<NodePtr> _children;
    std::vector<Attribute> _attributes;
};

class Text: public Nonelement {
public:
    // constructor
    Text(ElementPtr p, std::size_t index);
    Text(ElementPtr p, std::size_t index, const std::string& value);
    // destructor
    ~Text() override = default;

};

class Declaration: public Nonelement {
public:
    // destructor
    Declaration(ElementPtr p, std::size_t index);
    Declaration(ElementPtr p, std::size_t index, const std::string& value);
    ~Declaration() override  = default;

};

class Comment: public Nonelement {
public:
    // destructor
    Comment(ElementPtr p, std::size_t index);
    Comment(ElementPtr p, std::size_t index, const std::string& value);
    ~Comment() override = default;
};

class Unknown: public Nonelement {
public:
    // destructor
    Unknown(ElementPtr p, std::size_t index);
    Unknown(ElementPtr p, std::size_t index, const std::string& value);
    ~Unknown() override = default;
};

class Attribute : public std::pair<std::string, std::string> {
public:
    Attribute(const std::string& key, const std::string& value);
    std::string getKey() const;
    std::string getValue() const;
};

}

#endif //XSEA_XSEA_H
