#include <cctype>
#include "MailParser.h"

/*** Token Tree ***/

TokenTree::TokenTree(std::string node_content, TokenTree* parent, ContentType node_type)
{
    this->parent = parent;
    content = node_content;
    type = node_type;
}


TokenTree::~TokenTree()
{
    for (TokenTree* child : children) {
        delete child;
    }
}


TokenTree* TokenTree::addKey(std::string key, ContentType value_type)
{
    if (type == ContentType::OBJ_KEYS) {
        children.push_back(new TokenTree(key, this, value_type));
        key_idx_map.insert({key, (int)children.size()-1});
        return children.back();
    } else {
        return nullptr;
    }
}


void TokenTree::addStrValue(std::string value)
{
    if (type == ContentType::STR) {
        children.push_back(new TokenTree(value, this, ContentType::NONE));
    }
}


TokenTree* TokenTree::addObj()
{
    if (type == ContentType::OBJ) {
        children.push_back(new TokenTree("", this, ContentType::OBJ_KEYS));
        return children.back();
    } else {
        return nullptr;
    }
}


TokenTree* TokenTree::addArray(ContentType array_type)
{
    if (type == ContentType::ARRAY) {
        children.push_back(new TokenTree("", this, array_type));
        return children.back();
    } else {
        return nullptr;
    }
}


TokenTree* TokenTree::getParent() const
{
    return parent;
}


const std::vector<TokenTree*>& TokenTree::getChildren() const
{
    return children;
}


const std::string& TokenTree::getValue() const
{
    static std::string empty_str = "";

    if (type == ContentType::STR) {
        return children[0]->content;
    } else {
        return empty_str;
    }
}


TokenTree* TokenTree::getArray() const
{
    if (type == ContentType::ARRAY) {
        return children[0];
    } else {
        return nullptr;
    }
}
/*** Token Tree ***/



/*** Mail Parser ***/

MailParser::MailParser(QString filename, std::ostream& out)
{
    this->out = &out;
    file = new QFile(filename);
    opened = file->open(QIODevice::ReadOnly | QIODevice::Text);

    if (opened) {
        mail_file = new QTextStream(file);
        lex_state = State::EXPECT_OBJ;
    } else {
        lex_state = State::ERROR;
    }

    lexed = false;
    parsed = false;
    mailsTaken = false;

    tree = nullptr;
    curr = nullptr;
}


MailParser::~MailParser()
{
    curr = nullptr;
    if (tree != nullptr)
        delete tree;
    delete mail_file;
    file->close();
    delete file;
}


bool MailParser::streamOK() const
{
    return mail_file != nullptr && mail_file->status() == QTextStream::Ok;
}


bool MailParser::isValid() const
{
    return opened;
}


bool MailParser::isParsed() const
{
    return parsed;
}


int MailParser::skipWhiteSpace()
{
    int count = 0;
    char c;
    *mail_file >> c;
    while(isspace(c) && !mail_file->atEnd()) {
        *mail_file >> c;
        count++;
    }

    if (mail_file->atEnd() || !streamOK()) {
        lexed = true;
    } else {
        mail_file->seek(mail_file->pos() - 1);
    }   

    return count;
}


/**
 * @brief MailParser::readString, Reads a string from the opened file.
 * Reads until the closing quotation mark is found, wich is discarded
 * from the stream.
 * @return
 */
std::string MailParser::readString()
{
    if (lex_state == State::ERROR || lex_state == State::END)
        return "";

    std::string str;
    char c; 

    *mail_file >> c;

    while(streamOK() && c != '"') {
        str = str + c;
        *mail_file >> c;
    }

    if (!streamOK()) {
        lexed = true;
    }

    if (str == "replyTo")
        str = "to";
    return str;
}


void MailParser::lex()
{
    while (true) {
        if (lex_state == State::ERROR || lex_state == State::END)
            return;

        skipWhiteSpace();
        if (lexed) {
            lex_state = State::END;
            return;
        }

        if (lex_state == State::EXPECT_OBJ) {
            char c;
            *mail_file >> c;
            if (c == '{') {
                lex_state = State::EXPECT_KEY;
                if (tree == nullptr) {
                    tree = new TokenTree();
                    curr = tree;
                } else {
                    curr = curr->addObj();
                }
                enclose_stack.push('}');
                continue;
            } else if (c == '}' && c == enclose_stack.top()) {
                enclose_stack.pop();
                curr = curr->getParent();
                skipWhiteSpace();
                *mail_file >> c;

                if (lexed) {
                    lex_state = State::END;
                    return;
                } else if (!streamOK()){
                    lex_state = State::ERROR;
                    return;
                }

                mail_file->seek(mail_file->pos() - 1);
                continue;
            } else {
                lex_state = State::ERROR;
                *out << "Expected object not found.\n";
                return;
            }
        } else if (lex_state == State::EXPECT_KEY) {
            char c;
            *mail_file >> c;
            if (c == '"' && enclose_stack.top() != '"') {
                enclose_stack.push('"');
                curr = curr->addKey(readString(), TokenTree::ContentType::STR);
                *out << curr->content << " key added.\n";
                enclose_stack.pop();
                lex_state = State::EXPECT_VALUE;
                continue;
            } else {
                lex_state = State::ERROR;
                *out << "Expected key not found.\n";
                return;
            }
        } else if (lex_state == State::EXPECT_VALUE) {
            char c;
            *mail_file >> c;

            if (c != ':') {
                lex_state = State::ERROR;
                *out << "Key value seperator not found.\n";
                return;
            }

            skipWhiteSpace();
            *mail_file >> c;

            //Reading string values
            if (c == '"' && enclose_stack.top() != '"') {
                curr->type = TokenTree::ContentType::STR;
                enclose_stack.push('"');
                curr->addStrValue(readString());
                *out << curr->getChildren()[0]->content << " value added.\n";
                curr = curr->getParent();
                enclose_stack.pop();

                skipWhiteSpace();
                *mail_file >> c;

                if (streamOK() && c == ',') {
                    lex_state = State::EXPECT_KEY;
                    continue;
                } else if (streamOK() && (c == '}' || c == ']')) {
                    mail_file->seek(mail_file->pos() - 1);
                    lex_state = State::UNWIND;
                    continue;
                } else {
                    lex_state = State::ERROR;
                    *out << "Element after value not found.\n";
                    return;
                }
            } else if (c == '[') {
                enclose_stack.push(']');
                curr->type = TokenTree::ContentType::ARRAY;
                skipWhiteSpace();

                *mail_file >> c;
                if (streamOK() && c == '{') {
                    lex_state = State::EXPECT_OBJ;
                    curr = curr->addArray(TokenTree::ContentType::OBJ);
                    mail_file->seek(mail_file->pos() - 1);
                    continue;
                } else if (streamOK() && c == '"') {
                    lex_state = State::EXPECT_KEY;
                    curr = curr->addArray(TokenTree::ContentType::OBJ_KEYS);
                    mail_file->seek(mail_file->pos() - 1);
                    continue;
                } else {
                    lex_state = State::ERROR;
                    *out << "Unexpected value in array.\n";
                    return;
                }
            } else {
                lex_state = State::ERROR;
                *out << "Expected value not found.\n";
                return;
            }
        } else if (lex_state == State::UNWIND) {
            char c;
            *mail_file >> c;

            if (mail_file && enclose_stack.top() == ']' && c == ']') {
                enclose_stack.pop();
                curr = curr->getParent();
                curr = curr->getParent(); //Twice to return to the object context that stores the array

                skipWhiteSpace();
                *mail_file >> c;
                if (streamOK() && c == ',') {
                    //Arrays must be inside an object
                    if (curr->type == TokenTree::ContentType::OBJ_KEYS)
                        lex_state = State::EXPECT_KEY;
                    else
                        lex_state = State::ERROR;
                    continue;
                } else if (streamOK() && (c == ']' || c == '}')) {
                    mail_file->seek(mail_file->pos() - 1);
                    continue;
                } else if (mail_file->status() == QTextStream::ReadPastEnd) {
                    lex_state = State::END;
                    lexed = true;
                    return;
                } else {
                    lex_state = State::ERROR;
                    *out << "Array unwind error.\n";
                    return;
                }
            } else if (streamOK() && enclose_stack.top() == '}' && c == '}') {
                enclose_stack.pop();
                curr = curr->getParent();

                skipWhiteSpace();
                *mail_file >> c;
                if (mail_file && c == ',') {

                    //comma after closing bracket only in array
                    if (curr->type == TokenTree::ContentType::OBJ)
                        lex_state = State::EXPECT_OBJ;
                    else
                        lex_state = State::ERROR;

                    continue;
                } else if (streamOK() && (c == ']' || c == '}')) {
                    mail_file->seek(mail_file->pos() - 1);
                    continue;
                } else if (mail_file->status() == QTextStream::ReadPastEnd) {
                    lex_state = State::END;
                    lexed = true;
                    return;
                } else {
                    lex_state = State::ERROR;
                    lexed = true;
                    //Get does not trigger eof flag for some reason
                    *out << "Object unwind error.\n";
                    return;
                }
            } else {
                lex_state = State::ERROR;
                *out << "Unable to unwind.\n";
            }
        }
    }
}


/*Parses the token tree to generate Email objects.*/
void MailParser::parse()
{
    lex();
    if (lexed && lex_state == State::END) {
        if (tree->getChildren()[0]->getValue() != "mailbox") {
            *out << "Not a mailbox\n";
            return;
        } else {
            curr = tree->getChildren()[3]->getArray(); //Access message array
            for (TokenTree* msg : curr->getChildren()) {
                Email* mail = new Email();
                const std::vector<TokenTree*>& msgProps = msg->getChildren();
                mail->setSubject(msgProps[0]->getValue());
                mail->setToAddress(msgProps[1]->getValue());
                mail->setFromAddress(msgProps[2]->getValue());
                mail->setSendDate(msgProps[3]->getValue());
                mail->setReceiveDate(msgProps[4]->getValue());
                mail->setContent(msgProps[5]->getValue());

                if (msgProps.size() > 6) {
                    if (msgProps[6]->content == "attachments") {
                        for (TokenTree* attachment : msgProps[6]->getArray()->getChildren()) {
                            mail->addAttachment(attachment->getValue());
                        }
                    } else if (msgProps[6]->content == "inResponseTo") {
                        for (TokenTree* respMsg : msgProps[6]->getArray()->getChildren()) {
                            Email resp_mail;
                            const std::vector<TokenTree*>& respMsgProps = respMsg->getChildren();
                            int idx = -1;
                            try {
                                idx = respMsg->key_idx_map.at("subject");
                                resp_mail.setSubject(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            try {
                                idx = respMsg->key_idx_map.at("to");
                                resp_mail.setToAddress(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            try {
                                idx = respMsg->key_idx_map.at("from");
                                resp_mail.setFromAddress(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            try {
                                idx = respMsg->key_idx_map.at("sendDate");
                                resp_mail.setSendDate(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            try {
                                idx = respMsg->key_idx_map.at("receiveDate");
                                resp_mail.setReceiveDate(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            try {
                                idx = respMsg->key_idx_map.at("content");
                                resp_mail.setContent(respMsgProps[idx]->getValue());
                            } catch (std::out_of_range e) {}
                            mail->addResponseMail(resp_mail);
                        }
                    }
                }

                messages.push_back(mail);
            }

            parsed = true;
        }
    } else {
        *out << "Error in parsing mail file.\n";
    }
}


/**
 * @brief MailParser::getMails, only one call to this function can be made after a parse.
 * @return
 */
std::vector<Email*> MailParser::getMails()
{
    if (!mailsTaken) {
        mailsTaken = true;
        return messages;
    } else {
        return std::vector<Email*>();
    }
}
