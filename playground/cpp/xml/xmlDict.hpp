#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <variant>
#include <memory>
#include <libxml/parser.h>
#include <libxml/tree.h>

/**
 * A Python-like dictionary class for XML parsing results
 * Supports nested access like: dict["config"]["firewall"]["interface"][0]["@name"]
 */
class XmlDict {
public:
    using Value = std::variant<std::string, std::shared_ptr<XmlDict>, std::vector<std::shared_ptr<XmlDict>>>;
    using Container = std::map<std::string, Value>;

private:
    Container data;

public:
    // Helper proxy class for chaining operations
    class DictProxy {
    private:
        std::shared_ptr<XmlDict> dict_;
        
    public:
        DictProxy(std::shared_ptr<XmlDict> dict) : dict_(dict) {}
        
        // String access - returns the actual string value
        operator std::string() const {
            if (dict_ && dict_->data.size() == 1) {
                auto it = dict_->data.begin();
                if (std::holds_alternative<std::string>(it->second)) {
                    return std::get<std::string>(it->second);
                }
            }
            return "";
        }
        
        DictProxy operator[](const std::string& key) const {
            if (!dict_) {
                return DictProxy(nullptr);
            }
            
            // If it's an attribute (starts with @), try to get it directly as a string
            if (!key.empty() && key[0] == '@') {
                std::string attrValue = dict_->getString(key);
                if (!attrValue.empty()) {
                    auto tempDict = std::make_shared<XmlDict>();
                    tempDict->set("#text", attrValue);
                    return DictProxy(tempDict);
                }
            }
            
            // Otherwise, treat it as a nested dictionary
            return DictProxy(dict_->getDict(key));
        }        
        // Array access by index - returns proxy for chaining
        DictProxy operator[](int index) const {
            if (dict_ && dict_->data.size() == 1) {
                auto it = dict_->data.begin();
                if (std::holds_alternative<std::vector<std::shared_ptr<XmlDict>>>(it->second)) {
                    auto& vec = std::get<std::vector<std::shared_ptr<XmlDict>>>(it->second);
                    if (index >= 0 && static_cast<size_t>(index) < vec.size()) {
                        return DictProxy(vec[index]);
                    }
                }
            }
            return DictProxy(nullptr);
        }
        
        // Get as string
        std::string asString() const {
            return operator std::string();
        }
        
        // Get as integer
        int asInt() const {
            std::string str = asString();
            try {
                return std::stoi(str);
            } catch (...) {
                return 0;
            }
        }
        
        // Check if valid
        bool isValid() const {
            return dict_ != nullptr;
        }
        
        // Print for debugging
        void print() const {
            if (dict_) {
                dict_->print();
            } else {
                std::cout << "null" << std::endl;
            }
        }
    };

    // Add a string value
    void set(const std::string& key, const std::string& value) {
        data[key] = value;
    }
    
    // Add a nested dictionary
    void set(const std::string& key, std::shared_ptr<XmlDict> value) {
        data[key] = value;
    }
    
    // Add an array of dictionaries
    void set(const std::string& key, const std::vector<std::shared_ptr<XmlDict>>& value) {
        data[key] = value;
    }
    
    // Get a string value
    std::string getString(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end() && std::holds_alternative<std::string>(it->second)) {
            return std::get<std::string>(it->second);
        }
        return "";
    }
    
    // Get a nested dictionary
    std::shared_ptr<XmlDict> getDict(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end()) {
            if (std::holds_alternative<std::shared_ptr<XmlDict>>(it->second)) {
                return std::get<std::shared_ptr<XmlDict>>(it->second);
            } else if (std::holds_alternative<std::vector<std::shared_ptr<XmlDict>>>(it->second)) {
                // If it's an array, wrap it in a temporary dict for array access
                auto tempDict = std::make_shared<XmlDict>();
                tempDict->data[key] = it->second;
                return tempDict;
            }
        }
        return nullptr;
    }
    
    // Get an array of dictionaries
    std::vector<std::shared_ptr<XmlDict>> getArray(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end() && std::holds_alternative<std::vector<std::shared_ptr<XmlDict>>>(it->second)) {
            return std::get<std::vector<std::shared_ptr<XmlDict>>>(it->second);
        }
        return {};
    }
    
    // Check if key exists
    bool has(const std::string& key) const {
        return data.find(key) != data.end();
    }
    
    // Python-like bracket operator - returns proxy for chaining
    DictProxy operator[](const std::string& key) const {
        return DictProxy(getDict(key));
    }
    
    // Get all keys
    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        for (const auto& [key, value] : data) {
            result.push_back(key);
        }
        return result;
    }
    
    // Print the dictionary structure
    void print(int indent = 0) const {
        std::string indentStr(indent * 2, ' ');
        
        for (const auto& [key, value] : data) {
            if (std::holds_alternative<std::string>(value)) {
                std::cout << indentStr << key << ": \"" << std::get<std::string>(value) << "\"" << std::endl;
            } else if (std::holds_alternative<std::shared_ptr<XmlDict>>(value)) {
                std::cout << indentStr << key << ":" << std::endl;
                std::get<std::shared_ptr<XmlDict>>(value)->print(indent + 1);
            } else if (std::holds_alternative<std::vector<std::shared_ptr<XmlDict>>>(value)) {
                std::cout << indentStr << key << " (array):" << std::endl;
                const auto& vec = std::get<std::vector<std::shared_ptr<XmlDict>>>(value);
                for (size_t i = 0; i < vec.size(); ++i) {
                    std::cout << indentStr << "  [" << i << "]:" << std::endl;
                    vec[i]->print(indent + 2);
                }
            }
        }
    }
    
    // Add an item to an array (or create array if doesn't exist)
    void appendToArray(const std::string& key, std::shared_ptr<XmlDict> value) {
        auto it = data.find(key);
        if (it != data.end() && std::holds_alternative<std::vector<std::shared_ptr<XmlDict>>>(it->second)) {
            auto& vec = std::get<std::vector<std::shared_ptr<XmlDict>>>(it->second);
            vec.push_back(value);
        } else if (it != data.end() && std::holds_alternative<std::shared_ptr<XmlDict>>(it->second)) {
            // Convert single item to array
            std::vector<std::shared_ptr<XmlDict>> vec;
            vec.push_back(std::get<std::shared_ptr<XmlDict>>(it->second));
            vec.push_back(value);
            data[key] = vec;
        } else {
            // Create new array
            std::vector<std::shared_ptr<XmlDict>> vec;
            vec.push_back(value);
            data[key] = vec;
        }
    }
};

/**
 * Wrapper class to enable Python-like syntax directly on parsed XML
 */
class XmlRoot {
private:
    std::shared_ptr<XmlDict> root_;
    
public:
    XmlRoot(std::shared_ptr<XmlDict> root) : root_(root) {}
    
    template<typename T>
    XmlDict::DictProxy operator[](const T& key) const {
        if (root_) {
            return (*root_)[std::string(key)];
        }
        return XmlDict::DictProxy(nullptr);
    }
    
    // Check if valid
    bool isValid() const {
        return root_ != nullptr;
    }
    
    // Get the raw pointer for other operations
    std::shared_ptr<XmlDict> get() const {
        return root_;
    }
    
    // Print the structure
    void print() const {
        if (root_) {
            root_->print();
        }
    }
    
    // Boolean conversion for null checks
    operator bool() const {
        return root_ != nullptr;
    }
};

/**
 * Enhanced XML Parser that creates Python-like dictionary structure
 */
class EnhancedXmlParser {
private:
    std::shared_ptr<XmlDict> parseNode(xmlNode* node) {
        auto nodeDict = std::make_shared<XmlDict>();
        
        // Add attributes with @ prefix (like Python's xmltodict)
        for (xmlAttr* attr = node->properties; attr != nullptr; attr = attr->next) {
            xmlChar* attrValue = xmlGetProp(node, attr->name);
            if (attrValue) {
                std::string attrKey = "@" + std::string((char*)attr->name);
                nodeDict->set(attrKey, std::string((char*)attrValue));
                xmlFree(attrValue);
            }
        }
        
        // Handle child elements
        std::map<std::string, std::vector<std::shared_ptr<XmlDict>>> childGroups;
        
        for (xmlNode* child = node->children; child; child = child->next) {
            if (child->type == XML_ELEMENT_NODE) {
                std::string childName = std::string((char*)child->name);
                auto childDict = parseNode(child);
                childGroups[childName].push_back(childDict);
            } else if (child->type == XML_TEXT_NODE) {
                xmlChar* content = xmlNodeGetContent(child);
                if (content && xmlStrlen(content) > 0) {
                    std::string textContent = std::string((char*)content);
                    // Remove whitespace-only content
                    textContent.erase(0, textContent.find_first_not_of(" \t\n\r"));
                    textContent.erase(textContent.find_last_not_of(" \t\n\r") + 1);
                    if (!textContent.empty()) {
                        nodeDict->set("#text", textContent);
                    }
                }
                if (content) xmlFree(content);
            }
        }
        
        // Add child groups to the dictionary
        for (const auto& [childName, children] : childGroups) {
            if (children.size() == 1) {
                nodeDict->set(childName, children[0]);
            } else {
                nodeDict->set(childName, children);
            }
        }
        
        return nodeDict;
    }
    
public:
    XmlRoot parseFile(const std::string& filePath) {
        // Initialize libxml2
        xmlInitParser();
        LIBXML_TEST_VERSION
        
        // Parse the XML file
        xmlDocPtr doc = xmlReadFile(filePath.c_str(), nullptr, 0);
        if (doc == nullptr) {
            std::cerr << "Error: could not parse file " << filePath << std::endl;
            xmlCleanupParser();
            return XmlRoot(nullptr);
        }
        
        // Get the root element
        xmlNode* root = xmlDocGetRootElement(doc);
        if (root == nullptr) {
            std::cerr << "Error: empty document" << std::endl;
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return XmlRoot(nullptr);
        }
        
        // Create root dictionary
        auto rootDict = std::make_shared<XmlDict>();
        std::string rootName = std::string((char*)root->name);
        auto parsedRoot = parseNode(root);
        rootDict->set(rootName, parsedRoot);
        
        // Cleanup
        xmlFreeDoc(doc);
        xmlCleanupParser();
        
        return XmlRoot(rootDict);
    }
};
