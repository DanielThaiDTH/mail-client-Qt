#ifndef MAILPARSER_H
#define MAILPARSER_H

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "Email.h"

class TokenTree
{
    std::vector<TokenTree*> children;
    TokenTree* parent;
public:
    std::map<std::string, int> key_idx_map;
    //Determines how a key stores its children. None for basic values.
    enum class ContentType { ARRAY, STR, OBJ, OBJ_KEYS, NONE };
    ContentType type;
    std::string content;
    TokenTree(std::string node_content = "", TokenTree* parent = nullptr, ContentType node_type = ContentType::OBJ_KEYS);
    TokenTree* addKey(std::string key, ContentType value_type);
    void addStrValue(std::string value);
    TokenTree* addObj();
    TokenTree* addArray(ContentType array_type);
    TokenTree* getParent() const;
    const std::vector<TokenTree*>& getChildren() const;
    const std::string& getValue() const;
    TokenTree* getArray() const;
    ~TokenTree();
};


/*Parses the mail file into Email objects. Creates but does not manage the 
life time of the Email objects.*/
class MailParser
{
    enum class State { EXPECT_OBJ, EXPECT_KEY, EXPECT_VALUE, UNWIND, END, ERROR };
    QTextStream* mail_file;
    QFile* file;
    std::vector<Email*> messages;
    std::stack<char> enclose_stack;
    std::ostream* out;
    bool lexed;
    bool parsed;
    bool opened;
    bool mailsTaken;
    int skipWhiteSpace();
    std::string readString();
    void lex();
    State lex_state;
    TokenTree* tree;
    TokenTree* curr;
    bool streamOK() const;
public:
    MailParser(QString filename, std::ostream& out);
    ~MailParser();
    bool isValid() const;
    bool isParsed() const;
    void parse();
    std::vector<Email*> getMails();
};

#endif
