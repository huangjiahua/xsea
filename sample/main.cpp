#include <iostream>
#include "../include/xsea.h"
using namespace std;
using namespace Xsea;

void display(const Xsea::Node &node, std::ostream& os,
             int indent=0) {
    auto type = node.getType();
    if (type != Xsea::NodeType::_text && type != Xsea::NodeType::_element)
        return;
    os << std::string(indent, '\t') << node.getValue();
    if (type == Xsea::NodeType::_element) {
        os << ": ";
        const auto& element = dynamic_cast<const Xsea::Element&>(node);
        const auto& attributes = element.getAllAttributes();
        std::size_t sz = element.size();
        for (const auto& p : attributes)
            os << p.getKey() << "=" << "\"" << p.getValue() << "\" ";
        os << std::endl;
        for (int i = 0; i < sz; i++)
            display(element.at(static_cast<size_t>(i)), os, indent + 1);
    }
    else os << std::endl;
}

int main(int argc, const char** argv) {
    std::string filename;
    if (argc <= 1)
        filename = "test.xml";
    else
        filename = argv[1];
    Document doc(filename);
    doc.loadFile();
    ElementPtr ptr = doc.getRootPtr();
    dynamic_pointer_cast<Element>(ptr->insert(0, NodeType::_element, "newTag"));
    ptr = dynamic_pointer_cast<Element>(ptr->insert(1, NodeType::_element, "tag"));
    ptr->add(NodeType::_text, "add a new tag");
    display(doc.getRoot(), cout);
    doc.saveFile("new.xml");
    return 0;
}